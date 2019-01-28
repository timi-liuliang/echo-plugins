/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TaskState.h (UT Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __UT_TASKSTATE_H_INCLUDED__
#define __UT_TASKSTATE_H_INCLUDED__

#include "UT_API.h"

#include "UT_Task.h"
#include "UT_TaskArena.h"
#include "UT_Thread.h"
#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_AtomicPtr.h>
#include <utility>

/// A task representing a wait for completion of a task by another thread.
/// We maintain a thread-safe linked list of these objects in UT_TaskState.
class UT_API UT_TaskStateProxy : public UT_EmptyTask
{
public:
    UT_TaskStateProxy()
	: myNext(nullptr)
    {
    }

private:
    UT_TaskStateProxy	*myNext;

    friend class UT_TaskState;
};

/// A task node for managing which thread is currently working on a given task
class UT_API UT_TaskState
{
    /// Special value used to indicate that myWaitingTasks is not accepting
    /// additions.
    ///
    /// When the task state is "unplugged", it means that it's still accepting
    /// compute or wait tasks.  When it's "plugged" it no longer accepts tasks.
    static UT_TaskStateProxy *plugged()
	{ return (UT_TaskStateProxy *)uintptr_t(-1); }

public:
    enum TaskStatus
    {
        FREE,           /// Thread has acquired responsibility to evaluate node.
        BUSY_WITH_ARENA,/// Another thread is busy evaluating the node with an arena.
        BUSY_NO_ARENA,  /// Another thread is busy evaluating the node without an arena.
        DONE_WITH_ARENA,/// The node has been evaluated with an arena.
        DONE_NO_ARENA   /// The node has been evaluated without an arena.
    };

    UT_TaskState()
        : myArenaAndGroupRefCount(0)
    {
        SYS_STATIC_ASSERT_MSG(sizeof(UT_TaskState) == 2*sizeof(SYS_AtomicInt32) + sizeof(void*),
            "This is just to check that the union is putting the two pointers "
            "in the same place.");

	reset();
    }
    ~UT_TaskState()
    {
        UT_ASSERT_P(myArenaAndGroupRefCount.relaxedLoad() == 0);
        UT_ASSERT_P(myArenaAndGroup.relaxedLoad() == nullptr || myWaitingTasks.relaxedLoad() == plugged());
    }

    /// Set the state to be "free" with no waiting tasks.
    /// This cannot be called from within threaded code.
    void
    reset()
    {
        myStatus.relaxedStore(FREE);
        UT_ASSERT_P(myArenaAndGroupRefCount.relaxedLoad() == 0);
        // This will also assign nullptr to myWaitingTasks.
        // It doesn't need to be atomic, because nothing else is allowed to
        // access it while we're resetting.
        myArenaAndGroup.relaxedStore(nullptr);
    }

    /// Test whether the task state is marked as DONE.
    bool isDone() const
    {
        auto state = myStatus.relaxedLoad();
        return state == DONE_NO_ARENA || state == DONE_WITH_ARENA;
    }

    /// Attempt to claim this node for the calling thread, returning the
    /// current status.
    TaskStatus
    tryMarkAsBusy(bool run_in_task_arena=false)
    {
        auto state = myStatus.relaxedLoad();
        if (state == DONE_WITH_ARENA || state == DONE_NO_ARENA || state == BUSY_NO_ARENA)
            return (TaskStatus)state;

        // Increment the reference count before checking myStatus again,
        // to avoid a race condition where a thread might delete it
        // early.  We decrement after if it turned out that we don't need
        // the reference after all.  Just in case run_in_task_arena is set
        // inconsistently between threads, we need the same increment
        // regardless of whether run_in_task_arena is true, and regardless
        // of whether state was already BUSY_WITH_ARENA.
        myArenaAndGroupRefCount.add(1);

        // If we're currently FREE, only allow one thread to change the status
        // to BUSY.  Otherwise, return the BUSY/DONE state.
        state = myStatus.compare_swap(FREE, run_in_task_arena ? BUSY_WITH_ARENA : BUSY_NO_ARENA);

        if (state == DONE_NO_ARENA || state == BUSY_NO_ARENA || (!run_in_task_arena && state == FREE))
            myArenaAndGroupRefCount.add(-1);
        else if (state == DONE_WITH_ARENA)
            decrefArenaGroup();

        return (TaskStatus)state;
    }
    /// Assuming that we're done, claim the node for the calling thread.
    /// Returns FREE if succeeded, BUSY if failed.
    TaskStatus
    tryMarkAsBusyFromDone()
    {
        UT_ASSERT_MSG(0, "This is untested code.  Note that calling code in the build is now unused."
            "  If myArenaAndGroup was used before, it must be used on subsequent executes.");
	if (myStatus.compare_swap(DONE_NO_ARENA, BUSY_NO_ARENA) == DONE_NO_ARENA)
	{
            // If myWaitingTasks is plugged(), we need to clear it to null,
            // but if we're using a task arena, myArenaAndGroupRefCount may
            // still be non-zero, depending on how this functions is supposed
            // to be used.  If it is non-zero, this code is wrong.
            UT_ASSERT(myArenaAndGroupRefCount.relaxedLoad() == 0);
            myArenaAndGroup.relaxedStore(nullptr);
	    return FREE;
	}
	return BUSY_NO_ARENA;
    }
    /// Assuming that we're busy, add a waiting task to be spawned when we're
    /// free again. This is done a lock-free linked-list style.
    void
    addWaitingTask(UT_Task &parent_task)
    {
        UT_ASSERT_P(myStatus.relaxedLoad() == BUSY_NO_ARENA || myStatus.relaxedLoad() == DONE_NO_ARENA);

        UT_TaskStateProxy *proxy = new (parent_task.allocate_child()) UT_TaskStateProxy();

        UT_TaskStateProxy *old;
	do
	{
	    old = myWaitingTasks;
	    if (old == plugged())
	    {
		// List was plugged by markAsDone() after we checked it in
		// tryMarkAsBusy() before we got here.
		//
		// In tryMarkAsBusyFromDone(), it's possible myWaitingTasks is
		// still plugged() when a separate thread adds a waiting task.
		// Therefore, the task which adds waiting tasks should always
		// recycle themselves after adding waiting tasks.
		parent_task.spawnChild(*proxy);
		return;
	    }
	    proxy->myNext = old;

	    SYSstoreFence();	// Ensure the task state memory is stored
	}
	while (myWaitingTasks.compare_swap(old, proxy) != old);
    }
    /// Mark this node as being free. We walk through our waiting tasks and
    /// spawn them. Since the proxy tasks are just empty tasks, they will
    /// complete immediately and decrement the ref count of their parent task.
    /// If the ref count of the parent task goes down to 0, then it will then
    /// be runnable in the task scheduler.
    void
    markAsDone(UT_Task *parent_task, bool run_in_task_arena)
    {
        if (parent_task)
        {
            // Plug myWaitingTasks so that we move into the DONE state
            UT_TaskStateProxy *proxy = myWaitingTasks.exchange(plugged());

            // Spawn off any tasks that we're waiting while we were busy
            while (proxy != nullptr)
            {
                UT_TaskStateProxy *next = proxy->myNext;
                parent_task->spawnChild(*proxy);
                proxy = next;
            }
            // Set myStatus to DONE so others can reclaim us.
            myStatus.exchange(DONE_NO_ARENA);
        }
        else
        {
            // Set myStatus to DONE so others can reclaim us.
            myStatus.exchange(run_in_task_arena ? DONE_WITH_ARENA : DONE_NO_ARENA);
        }
    }

    /// Non-threaded version of marking as done.
    void
    markAsDoneNoThread()
    {
	// Plug myWaitingTasks so that we move into the DONE state.
	// We should have no proxy tasks in this case!
	UT_VERIFY(myWaitingTasks.exchange(plugged()) == nullptr);

	// Clear myStatus so that others can reclaim us.
	myStatus.exchange(DONE_NO_ARENA);
    }

    /// This does a fast (non-atomic) check of the status.
    TaskStatus
    relaxedLoadStatus() const
    {
        return (TaskStatus)myStatus.relaxedLoad();
    }

    using ArenaAndGroup = std::pair<UT_TaskArena,UT_TaskGroup>;

    void setAndRetainArenaGroup(ArenaAndGroup *p)
    {
        UT_ASSERT_P(myArenaAndGroupRefCount.relaxedLoad() > 0 &&
            (myStatus.relaxedLoad() == BUSY_WITH_ARENA || myStatus.relaxedLoad() == DONE_WITH_ARENA));
        myArenaAndGroup.relaxedStore(p);
    }
    ArenaAndGroup *getArenaGroup() const
    {
        UT_ASSERT_P(myArenaAndGroupRefCount.relaxedLoad() > 0 &&
            (myStatus.relaxedLoad() == BUSY_WITH_ARENA || myStatus.relaxedLoad() == DONE_WITH_ARENA));
        return myArenaAndGroup.relaxedLoad();
    }
    /// Decrements the reference count for myArenaAndGroup
    /// and deletes it if the reference count reaches zero.
    void decrefArenaGroup()
    {
        UT_ASSERT_P(myStatus.relaxedLoad() == DONE_WITH_ARENA);
        if (myArenaAndGroupRefCount.add(-1) == 0)
        {
            // NOTE: This must be an atomic exchange, since
            //       there is a race condition where two threads
            //       can both get a ref count of zero, (one of which
            //       incremented and immediately decremented in
            //       tryMarkAsBusy()).
            auto *arena_group = myArenaAndGroup.exchange(nullptr);
            delete arena_group;
        }
    }
private:
    SYS_AtomicInt32 myStatus;
    SYS_AtomicInt32 myArenaAndGroupRefCount;
    union {
        SYS_AtomicPtr<ArenaAndGroup> myArenaAndGroup;
        SYS_AtomicPtr<UT_TaskStateProxy> myWaitingTasks;
    };
};

#endif // __UT_TASKSTATE_H_INCLUDED__
