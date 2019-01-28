/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_RecursiveTimedLock.h (UT Library, C++)
 *
 * COMMENTS:	Recursive mutex that supports timedLock(), required
 *		by UT_AbortableLock.
 *
 */

#ifndef __UT_RECURSIVETIMEDLOCK_H_INCLUDED__
#define __UT_RECURSIVETIMEDLOCK_H_INCLUDED__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_Lockable.h"
#include "UT_NonCopyable.h"
#include <SYS/SYS_Types.h>

#if defined(USE_PTHREADS) && !defined(MBSD)
//
// pthread implementation
//

#include <pthread.h>
#include <sys/time.h>
#include <errno.h>

#if defined(MBSD)
    #define UT_PTHREAD_MUTEX_RECURSIVE	PTHREAD_MUTEX_RECURSIVE
#else
    #define UT_PTHREAD_MUTEX_RECURSIVE	PTHREAD_MUTEX_RECURSIVE_NP
#endif

class UT_RecursiveTimedLock : UT_NonCopyable
{
public:
    UT_RecursiveTimedLock()
    {
	pthread_mutexattr_t attr;
	UT_VERIFY(pthread_mutexattr_init(&attr) == 0);
        UT_VERIFY(pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE)==0);
	UT_VERIFY(pthread_mutex_init(&myMutex, &attr) == 0);
	UT_VERIFY(pthread_mutexattr_destroy(&attr) == 0);
    }
    ~UT_RecursiveTimedLock()
    {
	pthread_mutex_destroy(&myMutex);
    }

    bool tryLock()
    {
	int res = pthread_mutex_trylock(&myMutex);
	UT_ASSERT(res == 0 || res == EBUSY);
	return (res == 0);
    }

    void lock()
    {
	UT_VERIFY(pthread_mutex_lock(&myMutex) == 0);
    }

    bool timedLock(int ms)
    {
	const uint64	THOUSAND = 1000;
	const uint64	MILLION  = THOUSAND * THOUSAND;
	struct timeval	now;
	struct timespec timeout;
	uint64		usec;

	gettimeofday(&now, NULL);
	usec = now.tv_usec + THOUSAND*(ms + now.tv_sec*THOUSAND);
	timeout.tv_sec  = (time_t)(usec / MILLION);
	timeout.tv_nsec = (long)((usec % MILLION) * THOUSAND);

	int res = pthread_mutex_timedlock(&myMutex, &timeout);
	UT_ASSERT(res == 0|| res == ETIMEDOUT);
	return (res == 0);
    }

    void unlock()
    {
	UT_VERIFY(pthread_mutex_unlock(&myMutex) == 0);
    }

    bool isLocked()
    {
	if (tryLock())
	{
	    unlock();
	    return false;
	}
	return true;
    }

    /// Class for auto-unlocking
    typedef UT_LockScopeType<UT_RecursiveTimedLock> Scope;

private:
    pthread_mutex_t myMutex;
};

#elif defined(_WIN32) || defined(MBSD)
//
// hboost::recursive_timed_mutex is fastest on Windows and OSX,
// beats std::recursive_timed_mutex on those platforms.
//

#include <SYS/SYS_BoostThread.h>

/// Lock adapter for boost mutexes
template <class HBOOST_MUTEX>
class UT_BoostLockable : public UT_Lockable<HBOOST_MUTEX>
{
public:
    bool timedLock(int ms)
    {
	return UT_Lockable<HBOOST_MUTEX>::mutex()
			.timed_lock(hboost::posix_time::milliseconds(ms));
    }
};

// Use hboost::recursive_timed_mutex
typedef UT_BoostLockable<hboost::recursive_timed_mutex> UT_RecursiveTimedLock;

#else
//
// C++11 standard implementation
//
// NOTE: Currently unused because:
//	 - It slower than hboost::recursive_timed_mutex on Windows/OSX
//         (see testut -it TESTUT_BoostThread)
//	 - On gcc 4.8, it's completely broken, only fixed in g++ >= 4.9
//

#include <chrono>
#include <mutex>

/// Lock adapter for std mutexes
template <class STD_MUTEX>
class UT_StdLockable : public UT_Lockable<STD_MUTEX>
{
public:
    bool timedLock(int ms)
    {
	return UT_Lockable<STD_MUTEX>::mutex()
			.try_lock_for(std::chrono::milliseconds(ms));
    }
};
typedef UT_StdLockable<std::recursive_timed_mutex> UT_RecursiveTimedLock;

#endif

#endif // __UT_RECURSIVETIMEDLOCK_H_INCLUDED__
