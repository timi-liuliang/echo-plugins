/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Lock.h ( UT Library, C++)
 *
 * COMMENTS:
 *	Locking class
 */

#ifndef __UT_Lock_h__
#define __UT_Lock_h__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_LockUtil.h"
#include "UT_NonCopyable.h"
#include "UT_String.h"
#include <SYS/SYS_Types.h>

class UT_Condition;
class UT_ConditionLock;

#if defined(LINUX) || defined(MBSD)
#define HAVE_PLATFORM_LOCK
#include <pthread.h>
#elif defined(WIN32)
#define HAVE_PLATFORM_LOCK
#endif

#ifdef HAVE_PLATFORM_LOCK

// Enable a printout of the number of calls to lock() along with a
// backtrace showing where the lock was first called.  Currently only
// available on linux/mac.
//#define DEBUG_LOCK_CALLERS

class UT_API UT_Lock : UT_NonCopyable
{
    friend class UT_Condition;
public:
    // thread_lock *must* be true
    explicit UT_Lock(bool initially_locked = false);
    ~UT_Lock();

    // tries the lock once, returns true if the lock was obtained.
    bool	tryLock();

    // blocks until the lock is obtained.  lock() is inlined for speed.
#if defined(WIN32) || defined(DEBUG_LOCK_CALLERS)
    void	lock();
#elif defined(LINUX) || defined(MBSD)
    void	lock()
    {
	UT_VERIFY(pthread_mutex_lock(&myLock) == 0);

        // NOTE: pthread_mutex_lock does a memory fence as appropriate.
        //       See POSIX.1-2008 Section 4.11 Memory Synchronization.
        //       If it didn't, we would need at least a SYSloadFence() here,
        //       after acquiring the lock.
    }
#endif

    bool	safeLock() { lock(); return true; }

    // tries for N ms to get the lock, returns true if the lock was obtained.
    bool	lock(int ms);

    // release the lock.  unlock() is inlined for speed.
#if defined(LINUX) || defined(MBSD)
    void	unlock()
    {
        // NOTE: pthread_mutex_unlock does a memory fence as appropriate.
        //       See POSIX.1-2008 Section 4.11 Memory Synchronization.
        //       If it didn't, we would need at least a SYSstoreFence() here,
        //       before releasing the lock.
    #if (UT_ASSERT_LEVEL > UT_ASSERT_LEVEL_NONE)
	int result = pthread_mutex_unlock(&myLock);
	UT_ASSERT(result == 0);
    #else
	(void) pthread_mutex_unlock(&myLock);
    #endif
    }
#elif defined(WIN32)
    void	unlock();
#endif

    bool	isLocked();
    int		getCollisions()	{ return 0; }

    int64	getMemoryUsage(bool inclusive) const;

    /// Class for auto-unlocking
    typedef UT_LockScopeType<UT_Lock> Scope;

private:

#ifndef WIN32
    // UT_ConditionLock uses error check pthread locks, which Windows does not
    // support (it uses UT_SpinLock on Windows). This allows UT_ConditionLock to
    // use conditions on platforms that support them without asserting.
    explicit UT_Lock(UT_ConditionLock &);
    friend class UT_ConditionLock;
#endif

private:
#if defined(LINUX) || defined(MBSD)
    pthread_mutex_t	 myLock;
    pthread_mutexattr_t	 myLockAttributes;
#elif defined(WIN32)
    struct Impl		// Actually a Windows CRITICAL_SECTION
    {
	uint8		 myBuffer[40];

	class CritSec;
	CritSec	*cast();
    };

    Impl		 myLock;
#endif

#if defined(DEBUG_LOCK_CALLERS)
    int			 myLockCount;
    UT_String		 myBacktrace;
#endif
};

#else // HAVE_PLATFORM_LOCK

#include "UT_SpinLock.h"
typedef UT_RecursiveSpinLock UT_Lock;

#endif

typedef UT_AutoLockType<UT_Lock>	UT_AutoLock;
typedef UT_DebugLockType<UT_Lock>	UT_DebugLock;
typedef UT_ObjLockType<UT_Lock>		UT_ObjLock;
typedef UT_AutoObjLockType<UT_Lock>	UT_AutoObjLock;

#endif
