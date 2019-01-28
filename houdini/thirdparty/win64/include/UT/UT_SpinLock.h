/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SpinLock.h ( UT Library, C++)
 *
 * COMMENTS:	Cross-platform Lock class for semaphoring.
 */

#ifndef UT_SPINLOCK_H
#define UT_SPINLOCK_H

#include "UT_API.h"
#include "UT_LockUtil.h"
#include "UT_NonCopyable.h"
// Needed for ut_thread_id_t
#include "UT_Thread.h"

#include <SYS/SYS_Align.h>
#include <SYS/SYS_AtomicInt.h>

// Helper class to minimize data noise in the spinlock class
namespace ut_internal
{
template<bool R>
class ut_SpinLockRecursiveT
{
public:
    void 		setThreadId(ut_thread_id_t) {}
    ut_thread_id_t 	getThreadId() 		{ return UT_INVALID_THREAD_ID; }
    void 		setLockCount(int) 	{}
    int 		getLockCount() 		{ return 0; }
};

template<>
class ut_SpinLockRecursiveT<true>
{
    ut_thread_id_t		myLockedThread;
    int				myLockCount;
public:
    ut_SpinLockRecursiveT() :
	myLockedThread(UT_INVALID_THREAD_ID),
	myLockCount(0)
    {}
    
    void 		setThreadId(ut_thread_id_t thread_id)
			{ myLockedThread = thread_id; }
			
    ut_thread_id_t 	getThreadId()
			{ return myLockedThread; }
			
    void 		setLockCount(int lock_count)
			{ myLockCount = lock_count; }
			
    int 		getLockCount()
			{ return myLockCount; }
};

template<bool R>
class ut_SpinLockDebugT
{
public:
    bool	debugThisLock() const	{ return false; }
    void	setDebugThisLock(bool v) {}
    bool	isActive() const	{ return true; }
    int		getCollisions() const	{ return 0; }
    void	addCollision()		{}
};

template <>
class ut_SpinLockDebugT<true>
{
public:
    ut_SpinLockDebugT()
	: myDebugThisLock(false)
	, myIsActive(true)
	, myCollisions(0)
    {
    }
    ~ut_SpinLockDebugT()
    {
	myIsActive = false;
    }

    bool	isActive() const	{ return myIsActive; }
    bool	debugThisLock() const	{ return myDebugThisLock; }
    void	setDebugThisLock(bool v) { myDebugThisLock = v; }
    int		getCollisions() const	{ return myCollisions; }
    void	addCollision()		{ myCollisions++; }
private:
    uint	myIsActive : 1;
    uint	myDebugThisLock : 1;
    uint	myCollisions : 30; // Collision counter
};

}

// If RecursiveLock is true, the locking thread is allowed to re-enter
// code areas that it locked. The lock is incremented & decremented.

// Align to a cache-line boundary to avoid bus noise when there are
// multiple spinlocks declared next to each other.
template <bool RecursiveLock = false, bool LockDebug = true>
class UT_API UT_SpinLockT : UT_NonCopyable
{
public:
        explicit UT_SpinLockT(int lockstate = 0);
		~UT_SpinLockT();

    int64	getMemoryUsage(bool inclusive) const
    { return inclusive ? sizeof(*this) : 0; }

    // tries the lock once, returns true if the lock was obtained.
    bool	tryLock();

    // tries for N ms to get the lock, returns true if the lock was obtained.
    bool	lock(int ms);

    // blocks until the lock is obtained.
    void	lock();
    
    bool	safeLock()  { lock(); return true; }

    // release the lock.
    void	unlock();

    // trigger/wait on trigger methods. waitForTrigger will block until
    // trigger is called. Cannot be used with thread locks. 
    void	waitForTrigger();
    void	trigger();

    void	debugLock(bool debug)
		    { myDebug.setDebugThisLock(debug); }

    bool	isLocked()
		{ return RecursiveLock ? 
			myRecursive.getLockCount() : (int32)mySpinLock.load(SYS_MEMORY_ORDER_LOAD);
		}

    int		getCollisions()	{ return myDebug.getCollisions(); }

    typedef UT_LockScopeType<UT_SpinLockT<RecursiveLock,LockDebug> > Scope;

private:
    SYS_AtomicInt32	mySpinLock;
    
    // Data for recursive locks. Not present for non-recursive locks.
    ut_internal::ut_SpinLockRecursiveT<RecursiveLock> myRecursive;
    ut_internal::ut_SpinLockDebugT<LockDebug>	myDebug;
};

using UT_SpinLock = UT_SpinLockT<false,false>;
using UT_RecursiveSpinLock = UT_SpinLockT<true,false>;
using UT_AutoSpinLock =  UT_SpinLock::Scope;


#endif

