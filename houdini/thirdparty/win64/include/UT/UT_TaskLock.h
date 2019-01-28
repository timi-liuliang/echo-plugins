/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TaskLock.h (UT Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __UT_TASKLOCK_H_INCLUDED__
#define __UT_TASKLOCK_H_INCLUDED__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_Debug.h"
#include "UT_NonCopyable.h"
#include "UT_Array.h"
#include "UT_TaskArena.h"
#include "UT_TaskScope.h"
#include <SYS/SYS_BoostThread.h>

#include <stddef.h>

// Set the following line to #if 1 to enable debug message output
#ifndef UT_TASKLOCK_DBG
  #if 0
    #define UT_TASKLOCK_DBG(ZZ)	UT_DBGOUT(ZZ)
  #else
    #define UT_TASKLOCK_DBG(ZZ)
  #endif
#endif

/// A recursive mutex class for synchronizing amongst tasks defined by
/// UT_TaskScope.
/// 
/// The assumptions that UT_TaskScope uses for tasks are:
///	- Tasks are hierarchical in a forest of trees
///	- When child tasks are running, the parent task is not
///	- Child tasks always finish before their parent task
///
/// Requirements:
///	- You must unlock before your own task ends
///	- You must unlock your lock before waiting for a child task to end
///
template <bool NESTED>
class UT_TaskLockT : UT_NonCopyable
{
private:
    friend class Scope;

    typedef hboost::mutex		ut_Mutex;
    typedef ut_Mutex::scoped_lock	ut_MutexLock;
    typedef hboost::condition_variable	ut_Condition;

    struct ut_Data
    {
	const UT_TaskScope *myOwner;
	int		    myLockCount;

	ut_Data()
	    : myOwner(NULL)
	    , myLockCount(0)
	{
	}
    };

    ut_Mutex		    myMutex;
    ut_Condition	    myCondition;
    int			    myNumWaitingThreads;
    ut_Data		    myData;
    UT_Array<ut_Data>	    myDataStack;

    bool unsafeTryLock(const UT_TaskScope &task, bool &was_first)
    {
	UT_ASSERT(myData.myLockCount >= 0);
	if (myData.myLockCount == 0 || myData.myOwner == &task)
	{
	    UT_ASSERT(myData.myLockCount > 0 || myData.myOwner == NULL);
	    myDataStack.append(myData);
	    myData.myOwner = &task;
	    ++myData.myLockCount;
	    was_first = (myData.myLockCount == 1);
	    UT_TASKLOCK_DBG(("Acquired first lock %p for owner %p (count %d)",
			     this, myData.myOwner, myData.myLockCount));
	    return true;
	}
	UT_TASKLOCK_DBG(("Failed fast lock %p for task %p, owner %p "
			 "(count %d), waiting threads %d",
			 this, &task, myData.myOwner, myData.myLockCount,
			 myNumWaitingThreads));
	return false;
    }

    // Returns true if locked within timeout.
    bool privateLock(hboost::system_time const &wait_until, bool &was_first)
    {
	ut_MutexLock		lock_scope(myMutex);
	const UT_TaskScope &	task = getTaskScope();

	// If nobody holds the lock then acquire it right away
	if (unsafeTryLock(task, was_first))
	    return true;
	// If we have positive lock count, then we should already have an owner
	UT_ASSERT(myData.myOwner != NULL);

	// Perform lock, blocking if needed
	bool ok = true;
	while (ok)
	{
	    // Test if we can acquire the lock
	    if (myData.myOwner == NULL 
		|| task.isAncestor(*myData.myOwner))
	    {
		UT_ASSERT(myData.myOwner != NULL || myData.myLockCount == 0);
		myDataStack.append(myData);
		myData.myOwner = &task;
		myData.myLockCount = 1;
		was_first = true;
		UT_TASKLOCK_DBG(("Took lock %p for owner %p from"
				 " %p (count %d)",
				 this, myData.myOwner,
				 myDataStack.last().myOwner,
				 myDataStack.last().myLockCount));
		return true;
	    }

	    // unlock myMutex and wait until it can be acquired again
	    UT_TASKLOCK_DBG(("Waiting on lock %p with owner %p (count %d) "
			     "prev waiting threads %d",
			     this, myData.myOwner, myData.myLockCount,
			     myNumWaitingThreads));
	    ++myNumWaitingThreads;
	    if (wait_until.is_pos_infinity())
	    {
		myCondition.wait(lock_scope);
	    }
	    else
	    {
		ok = myCondition.timed_wait(lock_scope, wait_until);
	    }
	    --myNumWaitingThreads;
	    UT_ASSERT(myNumWaitingThreads >= 0);
	}

	return false;
    }

    bool privateTryLock()
    {
	ut_MutexLock		lock_scope(myMutex);
	const UT_TaskScope &	task = getTaskScope();
	bool			was_first = false;

	return unsafeTryLock(task, was_first);
    }

    void privateUnlock()
    {
	ut_MutexLock	lock_scope(myMutex);
	bool			notify;

	UT_TASKLOCK_DBG(("Release lock %p for owner %p, "
			 "new owner %p (count %d), waiting threads %d",
			 this, myData.myOwner,
			 myDataStack.last().myOwner,
			 myDataStack.last().myLockCount,
			 myNumWaitingThreads));

	UT_ASSERT(myData.myLockCount >= 1);
	notify = (myData.myLockCount == 1);

	myData = myDataStack.last();
	myDataStack.removeLast();

	// Release the lock if the count goes down to zero.
	UT_ASSERT(myData.myLockCount >= 0);
	if (notify)
	{
	    UT_ASSERT(myData.myLockCount > 0 || myData.myOwner == NULL);

	    // Signal all the threads that are waiting.  We don't want to
	    // signal just one, since the next one wouldn't get signalled until
	    // unlock() is called again, and we want to let in all descendent
	    // tasks
	    if (myNumWaitingThreads > 0)
	    {
		myCondition.notify_all();
	    }
	}
    }

    bool privateHasLock()
    {
	ut_MutexLock	lock_scope(myMutex);
	const UT_TaskScope &	task = getTaskScope();
	return (myData.myLockCount > 0 && myData.myOwner == &task);
    }

    static const UT_TaskScope &getTaskScope()
    {
	if (NESTED)
	    return UT_TaskScope::getOrCreateCurrent();
	else
	    return UT_TaskScope::getOrCreateRoot();
    }

public:

    UT_TaskLockT()
	: myNumWaitingThreads(0)
    {
    }
    ~UT_TaskLockT()
    {
    }

    void lock()
    {
	bool was_first = false;
	(void) privateLock(hboost::system_time(hboost::posix_time::pos_infin),
			   was_first);
    }

    /// Same as lock() except it also returns if it was the first time this
    /// task scope obtained the lock (ie. non-recursively).
    void lock(bool &was_first)
    {
	(void) privateLock(hboost::system_time(hboost::posix_time::pos_infin),
			   was_first);
    }

    bool timedLock(int timeout)
    {
	bool was_first = false;
	return privateLock(hboost::get_system_time()
			    + hboost::posix_time::milliseconds(timeout),
			   was_first);
    }

    bool tryLock()
    {
	return privateTryLock();
    }

    bool safeLock()
    {
	lock();
	return true;
    }

    void unlock()
    {
	privateUnlock();
    }

    bool hasLock()
    {
	return privateHasLock();
    }

    /// Class for auto-unlocking
    typedef UT_LockScopeType<UT_TaskLockT<NESTED> > Scope;
};

/// A recursive mutex class for synchronizing amongst tasks defined by
/// UT_TaskScope.
/// 
/// The assumptions that UT_TaskScope uses for tasks are:
///	- Tasks are hierarchical in a forest of trees
///	- When child tasks are running, the parent task is not
///	- Child tasks always finish before their parent task
///
/// Requirements:
///	- You must unlock before your own task ends
///	- You must unlock your lock before waiting for a child task to end
///
typedef UT_TaskLockT<true>	UT_TaskLock;

typedef UT_TaskLockT<false>	UT_TaskRootLock;


/// UT_TaskLock that avoids deadlocks when used with TBB task scheduling.
class UT_API UT_TaskLockWithArena : private UT_TaskLock
{
public:

    /// Performs the functor F while inside this lock scope in UT_TaskArena.
    /// This method allows the optimization that if we're calling this while
    /// the lock is already held in the same task scope, then we can avoid
    /// creating an unnecessary task arena.
    template <typename F>
    void
    lockedExecute(const F &functor)
    {
	bool was_first = false;
	(void) UT_TaskLock::lock(was_first);
	if (!was_first)
	{
	    functor();
	}
	else
	{
	    UT_TaskArena arena;
	    arena.execute(functor);
	}
	UT_TaskLock::unlock();
    }

    /// Performs the functor F while inside this lock scope OUTSIDE an arena.
    /// @note Only do this if you know functor will never spawn tasks!
    template <typename F>
    void
    lockedExecuteWithoutArena(const F &functor)
    {
	UT_TaskLock::lock();
	functor();
	UT_TaskLock::unlock();
    }

    using UT_TaskLock::hasLock;
};

#endif // __UT_TASKLOCK_H_INCLUDED__
