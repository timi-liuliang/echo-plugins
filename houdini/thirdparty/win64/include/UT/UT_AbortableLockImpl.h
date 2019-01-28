/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_AbortableLockImpl.h (UT Library, C++)
 *
 * COMMENTS:	Implementation for UT_AbortableLock
 *
 */

#ifndef __UT_ABORTABLELOCKIMPL_H_INCLUDED__
#define __UT_ABORTABLELOCKIMPL_H_INCLUDED__

#ifndef UT_VAR
#define UT_VAR(X)   (X)
#endif

template <typename LOCKABLE>
bool
UT_AbortableLock<LOCKABLE>::safeLock()
{
    int	    thread_id = SYSgetSTID();
    int	    timeout = 1000;		// initial time out in ms
    bool    locked = false;

    // Fast path for low contention case
    if (UT_VAR(myMutex).tryLock())
    {
	myImpl.markAsLocked(thread_id);
	return true;
    }

    // mark us as waiting in this thread
    myImpl.markAsWaiting(thread_id);

    // attempt to acquire the lock in a loop with exponential back off
    while (true)
    {
	if (UT_VAR(myMutex).timedLock(timeout))
	{
	    // We're no longer waiting in this thread
	    // NB: We *must* clear theWaitingLock before marking ourselves as
	    //     the thread owner or else some other thread waiting on the
	    //     mutex suddenly might think that we're waiting on the lock
	    //     while having already acquired it!
	    myImpl.markAsNotWaiting(thread_id);
	    myImpl.markAsLocked(thread_id);
	    locked = true;
	    break;
	}
	else if (myImpl.findDeadlock(thread_id))
	{
	    // We're no longer waiting in this thread
	    myImpl.markAsNotWaiting(thread_id);
	    locked = false;
	    break;
	}
	// else retry with double the duration
	timeout *= 2;
    }

    return locked;
}

template <typename LOCKABLE>
bool
UT_AbortableLock<LOCKABLE>::tryLock()
{
    if (UT_VAR(myMutex).tryLock())
    {
	myImpl.markAsLocked(SYSgetSTID());
	return true;
    }

    return false;
}

template <typename LOCKABLE>
void
UT_AbortableLock<LOCKABLE>::lock()
{
    UT_VAR(myMutex).lock();
    myImpl.markAsLocked(SYSgetSTID());
}

template <typename LOCKABLE>
void
UT_AbortableLock<LOCKABLE>::unlock()
{
    myImpl.markAsUnlocked();
    UT_VAR(myMutex).unlock();
}

template <typename LOCKABLE>
inline bool
UT_AbortableLock<LOCKABLE>::hasLock(int thread)
{
    return myImpl.hasLock(thread);
}

template <typename LOCKABLE>
bool
UT_AbortableLock<LOCKABLE>::hasLock()
{
    return hasLock(SYSgetSTID());
}

#endif // __UT_ABORTABLELOCKIMPL_H_INCLUDED__
