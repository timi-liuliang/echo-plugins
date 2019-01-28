/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_AbortableLock.h (UT Library, C++)
 *
 * COMMENTS:	A mutex class that supports dynamic deadlock detection amongst
 *		all instances of UT_AbortableLock.
 */

#ifndef __UT_ABORTABLELOCK_H_INCLUDED__
#define __UT_ABORTABLELOCK_H_INCLUDED__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_IntArray.h"
#include "UT_LockUtil.h"
#include "UT_NonCopyable.h"
#include "UT_RecursiveTimedLock.h"
#include "UT_ThreadSpecificValue.h"
#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_AtomicPtr.h>

namespace UT_AbortableLockImpl
{

class UT_API LockState : UT_NonCopyable
{
public:
		LockState();

    bool	hasLock(int thread_id);

    void	markAsLocked(int thread_id);
    void	markAsUnlocked();

    void	markAsWaiting(int thread_id);
    void	markAsNotWaiting(int thread_id);

    bool	findDeadlock(int thread_id);
    bool	verifyDeadlock(
		    int start, const UT_IntArray &threads,
		    const UT_ValArray<LockState *> &locks);
    void	printDeadlock(
		    int start, const UT_IntArray &threads,
		    const UT_ValArray<LockState *> &locks);

    void	markDeadlock(int thread_id);

    static bool	getPrintDeadlocks();
    static void	setPrintDeadlocks(bool flag);

    static bool	runUnitTest(bool print_progress,
			    bool for_performance);

private:
    SYS_AtomicInt32 myThreadOwner;
    SYS_AtomicInt32 myLockCount;
    SYS_AtomicInt32 myUnlockCount;

    // theWaitingLock keeps track of the current UT_AbortableLock waiting to
    // acquire ownership for a specific thread.
    static UT_ThreadSpecificValue< SYS_AtomicPtr<LockState> >
		    theWaitingLock;

    // Keep track if the current thread hit a deadlock
    static UT_ThreadSpecificValue<int>
		    theDeadlockFlag;

    // If true, we will print our cycle when we hit a deadlock
    static bool	    thePrintDeadlocks;
};

} // namespace UT_AbortableLockImpl


/// A mutex class that supports dynamic deadlock detection amongst all
/// instances of UT_AbortableLock.
template <typename LOCKABLE>
class UT_AbortableLock : UT_NonCopyable
{
public:
    UT_AbortableLock()	{ }
    ~UT_AbortableLock()	{ }

    /// Attempt lock on this mutex. Returns true on success, false if deadlock
    bool    safeLock();

    /// Tries the lock without blocking. Returns true if the lock was obtained.
    bool    tryLock();

    /// Locks the underlying mutex without testing for deadlocks
    void    lock();

    /// Release lock. Undefined if it was not previously locked.
    void    unlock();

    /// Check if a thread has acquired this lock
    bool    hasLock(int thread);

    /// Check if the current thread has acquired this lock.
    /// It is equivalent to hasLock(SYSgetSTID()).
    bool    hasLock();		    

    /// Use UT_AbortableLock::Scope for scope/exception safety
    typedef UT_LockScopeType<UT_AbortableLock> Scope;

private:
    LOCKABLE				myMutex;
    UT_AbortableLockImpl::LockState	myImpl;
};


/// A form of UT_Lock with deadlock detection
typedef UT_AbortableLock<UT_RecursiveTimedLock>	UT_AbortableRecursiveLock;

// UT_AbortableTaskLock does not work because UT_AbortableLock assumes that
// it's underlying mutex only ever has 1 owner thread
#if 0
// A form of UT_TaskLock with deadlock detection
typedef UT_AbortableLock<UT_TaskLock>		UT_AbortableTaskLock;
#endif


// Implementation
#include "UT_AbortableLockImpl.h"

#endif // __UT_ABORTABLELOCK_H_INCLUDED__
