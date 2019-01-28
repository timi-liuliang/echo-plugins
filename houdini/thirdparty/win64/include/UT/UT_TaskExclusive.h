/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TaskExclusive.h (UT Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __UT_TaskExclusive__
#define __UT_TaskExclusive__

#include "UT_Task.h"
#include "UT_TaskScope.h"
#include "UT_TaskState.h"
#include "UT_TaskArena.h"
#include "UT_Thread.h"
#include <SYS/SYS_Pragma.h>
#include <utility>

/// This is a lock-free implementation for exclusive task execution.  That is,
/// a task which needs to be performed once (i.e. std::once).  However, this
/// construct will allow TBB to recycle blocked tasks so they can be used for
/// other processing.
///
/// This can be used as an alternate to a lock.  If code underneath a lock
/// calls into TBB, this can lead to a deadlock since TBB can steal a child
/// task to complete a parent task outside the lock.  This typically requires a
/// separate task arena.  A lock will also block a thread, preventing it from
/// participating in other tasks.
///
/// UT_TaskExclusive provides a good alternative, ensuring that only one thread
/// will execute the functor, and all other threads which wait for the functor
/// to finish will be allowed to participate in other computation (even to help
/// computing parallel tasks in the functor).
///
/// The class is templated on a functor which is used to actually perform the
/// execution.  The template functor needs to have a operator()() method.
///
/// For example, given a single Object which has a deferredInitialize() method
/// that may get called from multiple threads: @code
///    class Object
///    {
///        UT_TaskExclusive<Object> myExclusive;
///
///    public:
///        // Users call deferredInitialize when they want the object to be
///        // initialized.  However, it's possible the object may need to be
///        // initialized by multiple threads.  The object uses UT_TaskExclusive
///        // to ensure doInitialization() is only executed one time.
///        void deferredInitialize()
///        {
///            myExclusive.execute(*this);
///        }
///        void operator()()
///        {
///            // This method will only be called once, even if
///            // multiple threads invoke the deferredInitialize() method
///            // simultaneously.
///            doInitialization();
///        }
///    }
/// @endcode
/// If you have multiple methods that should only be called one time, you can
/// always create a nested object functor.
///
/// If the functor is likely to create further tbb tasks, you can ensure these
/// tasks are run in their own task arena by setting @c run_in_task_arena to
/// true (the default).
template <class T>
class UT_TaskExclusive
{
public:
    UT_TaskExclusive()
	: myState()
    {
    }

    /// Execute the compute task.  This will guarantee the function has been
    /// run before the execute() function returns.  However, no locking will be
    /// done.
    ///
    /// If multiple threads try to call the function simultaneously, only one
    /// function will run, while the other will yield its cycles to other
    /// parallel tasks.  When the first task completes, both threads will
    /// return.
    ///
    /// If the functor is likely to create further tbb tasks, you can ensure
    /// these tasks are run in their own task arena by setting @c
    /// run_in_task_arena to true (the default).  The primary reason for having
    /// a separate task arena is that the if the functor creates further tasks,
    /// and one of these tasks is also dependent on the task exclusive, this
    /// can lead to a tbb deadlock (a tbb stack lock).
    void execute(
        T &func,
        bool run_in_task_arena = true,
        bool use_spinlock = false)
    {
	UT_TaskState::TaskStatus status = myState.tryMarkAsBusy(run_in_task_arena);
	if (status == UT_TaskState::DONE_WITH_ARENA || status == UT_TaskState::DONE_NO_ARENA)
	{
	    // The task has been run, so just return
	    return;
	}

        if (run_in_task_arena && status == UT_TaskState::FREE)
        {
            // I win and I get to run the compute function
SYS_PRAGMA_PUSH_WARN()
SYS_PRAGMA_DISABLE_ATTRIBUTES()
            // NOTE: The arena & group must be on the heap, since this
            //       thread might have returned before all tasks in the
            //       arena, waiting on the group, have finished.
            auto *arena_group = new UT_TaskState::ArenaAndGroup();
            UT_TaskArena *arena = &arena_group->first;
            const UT_TaskScope *parent_task_scope = UT_TaskScope::getCurrent();

            // execute will be called on the arena from multiple threads,
            // but this one will always run before any others can start,
            // so we don't need to explicitly call initialize.
            //arena->initialize();
            arena->execute([this,&func,arena_group,parent_task_scope]()
            {
                UT_TaskGroup *group = &arena_group->second;
                group->run([this,&func,parent_task_scope]()
                {
                    UT_TaskScope task_scope(parent_task_scope);
                    func();
                    // Can't explicitly wake up waiting tasks,
                    // so we must pass nullptr here.
                    myState.markAsDone(nullptr, true);
                });
                // Ensure the initialized arena and group are written out to main memory
                // before the accessible pointer is set.
                SYSstoreFence();
                // The arena/group pointer must be set after run has been
                // called on the group, to ensure that other threads,
                // waiting for this pointer to be non-null, won't try to
                // call wait on the group until there's a task in the group.
                myState.setAndRetainArenaGroup(arena_group);

                // NOTE: We probably don't need a UT_TaskScope here; the one
                //       inside the lambda passed to run() should suffice.
                group->wait();
            });
SYS_PRAGMA_POP_WARN()
            myState.decrefArenaGroup();
        }
        else if (status == UT_TaskState::BUSY_WITH_ARENA)
        {
            // NOTE: Even if run_in_task_arena is false, if the first one
            //       called it with true, we need to be consistent.
            //       e.g. VGEO_MotionBlurTree was clearing myBoxes during its
            //       building, and code outside the exclusive was checking
            //       the size to determine whether to parallelize,
            //       (i.e. whether to use a task arena.)

            auto *arena_group = myState.getArenaGroup();
            if (!arena_group)
            {
                // Spin until the group task gets created.
                UT_ThreadBackoff backoff;
                do
                {
                    backoff.wait();
                    arena_group = myState.getArenaGroup();
                } while (!arena_group);
            }

            // NOTE: We probably don't need a SYSloadFence() here,
            //       because speculative reads of memory pointed to by
            //       arena_group would fail, since it was previously null.
            //       As soon as it's non-null, it's safe to read, as ensured
            //       by the SYSstoreFence() above.

            // Enter the task arena running the group task,
            // and wait on the group.
            UT_TaskArena *arena = &arena_group->first;
            UT_TaskGroup *group = &arena_group->second;
            arena->execute([group]()
            {
                // NOTE: This task doesn't need its own task scope,
                //       because any tasks executed during the group->wait()
                //       should have their own task scopes, which are
                //       automatically descendents of the task scope in the
                //       main compute task.
                group->wait();
            });
            myState.decrefArenaGroup();
        }
        else if (use_spinlock)
        {
            if (status == UT_TaskState::FREE)
            {
                // If other threads will be using a spin lock, we don't
                // need to create a new task.
                func();
                myState.markAsDone(nullptr, false);
            }
            else
            {
                UT_ThreadBackoff backoff;
                do
                {
                    backoff.wait();
                } while (myState.relaxedLoadStatus() == UT_TaskState::BUSY_NO_ARENA);
            }
        }
        else
        {
            if (status == UT_TaskState::FREE)
            {
                // We need a compute task, just so that we can specify
                // a parent task for spawning child tasks upon completion.
                // There might be some way to eliminate this.
                ComputeTask *root = ComputeTask::createRoot(myState, func);
                ComputeTask::spawnRootAndWait(*root);
            }
            else
            {
                // I lost and I have to wait for the opportunity to help.
                DummyTask *root = DummyTask::createRoot(myState);
                DummyTask::spawnRootAndWait(*root);
            }
        }
    }

    /// Executes the compute task in this thread without any locking
    /// or protection.  Useful if the caller has already setup the
    /// appropriate lock.
    void executeNoThread(T &func)
    {
	if (!hasRun())
	{
	    func();
	    myState.markAsDoneNoThread();
	}
    }

    /// Resetting the exclusive task should only be done when there's no
    /// possibility that any threads are trying to execute or relying on the
    /// results of the computation.
    void reset() { myState.reset(); }

    /// Test whether the function has been executed.  This is thread-safe, but
    /// doesn't count on other threads which may be in the process of running
    /// it.
    bool hasRun() const { return myState.isDone(); }

private:
    class ComputeTask : public UT_Task
    {
    public:
        ComputeTask(UT_TaskState &state, T &func)
            : myState(state)
            , myFunc(func)
        {
        }
        static ComputeTask *createRoot(UT_TaskState &state, T &func)
        {
            return new (allocate_root()) ComputeTask(state, func);
        }

        virtual UT_Task *run()
        {
            myFunc();
            myState.markAsDone(this, false);
            return nullptr;
        }
        T               &myFunc;
        UT_TaskState    &myState;
    };
    class DummyTask : public UT_Task
    {
    public:
        DummyTask(UT_TaskState &state)
            : myState(state)
        {
        }
        static DummyTask *createRoot(UT_TaskState &state)
        {
            return new (allocate_root()) DummyTask(state);
        }

        virtual UT_Task *run()
        {
            if (myState.relaxedLoadStatus() != UT_TaskState::BUSY_NO_ARENA)
                return nullptr;

            // Someone else is performing the computation, so
            //  a)  Add a child task to this task (introducing a dependency)
            //  b)  Add the child task to myState so it can be run when the
            //      compute is done...  The child (wait task) isn't run
            //      until the compute is complete.
            //  c)  Recycle my task so that it's put back on the TBB
            //      scheduler.  When the child task is complete, this task
            //      will be run again, but the state will be marked as
            //      done, so we can return.

            // Increment the reference count 2 times
            //  - Once since I have a child
            //  - Once since I'm recycling myself
            incrementRefCount();
            incrementRefCount();
            myState.addWaitingTask(*this);
            recycleAsSafeContinuation();
            return nullptr;
        }
        UT_TaskState &myState;
    };

    UT_TaskState myState;
};

#endif

