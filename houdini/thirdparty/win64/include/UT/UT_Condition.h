/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Condition.h ( UT Library, C++)
 *
 * COMMENTS:
 *	A condition class the implements pthread-like condition variables.
 */

#ifndef __UT_Condition_h__
#define __UT_Condition_h__

#include "UT_API.h"
#include "UT_Lock.h"
#include <SYS/SYS_Types.h>
#ifdef USE_PTHREADS
    #include <pthread.h>
#endif
#if defined(WIN32)
    #include <SYS/SYS_AtomicInt.h>
    #include <SYS/SYS_Handle.h>
#endif

class UT_API UT_Condition
{
public:
    UT_Condition();
    ~UT_Condition();

    // Block until we're triggered.  The lock must be held when this method is
    // called it will be released when waiting for the trigger and re-acquired
    // when this method returns.
    void waitForTrigger(UT_Lock &lock)
    #ifdef USE_PTHREADS
    {
	// We inline this method for speed.
	#if (UT_ASSERT_LEVEL > UT_ASSERT_LEVEL_NONE)
	    int r = pthread_cond_wait(&myCond, &lock.myLock);
	    UT_ASSERT(r == 0);
	#else
	    pthread_cond_wait(&myCond, &lock.myLock);
	#endif
    }
    #else
    ;
    #endif

    // Trigger all waiting threads.  The caller is responsible for holding
    // the same lock that was passed to waitForTrigger(), otherwise lost
    // wakeups are possible.
    void triggerAll()
    #ifdef USE_PTHREADS
    {
	// We inline this method for speed.
	#if (UT_ASSERT_LEVEL > UT_ASSERT_LEVEL_NONE)
	    int r = pthread_cond_broadcast(&myCond);
	    UT_ASSERT(r == 0);
	#else
	    pthread_cond_broadcast(&myCond);
	#endif
    }
    #else
    ;
    #endif

    // Trigger one waiting thread.  The caller is responsible for holding
    // the same lock that was passed to waitForTrigger(), otherwise lost
    // wakeups are possible.
    void triggerOne()
    #ifdef USE_PTHREADS
    {
	// We inline this method for speed.
	#if (UT_ASSERT_LEVEL > UT_ASSERT_LEVEL_NONE)
	    int r = pthread_cond_signal(&myCond);
	    UT_ASSERT(r == 0);
	#else
	    pthread_cond_signal(&myCond);
	#endif
    }
    #else
    ;
    #endif

private:
    // Prevent users from calling the copy constructor or assignment operator
    // by declaring them as private.  They are not implemented.
    UT_Condition(const UT_Condition &copy);
    UT_Condition &operator=(const UT_Condition &copy);

#ifdef USE_PTHREADS
    pthread_cond_t myCond;
#elif defined(WIN32)
    UT_Lock myCriticalSectionLock;
    SYS_AtomicInt32 myNumWaitingThreads;
    SYS_WinHandle myEvents[2];
#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined(WIN32)
    #define USE_SPINLOCK_FOR_CONDITIONLOCK
#endif

#ifdef USE_SPINLOCK_FOR_CONDITIONLOCK
    #include "UT_SpinLock.h"
#endif

// A condition class that doesn't require an external lock
class UT_API UT_ConditionLock
{
public:
    UT_ConditionLock()
    #ifdef USE_SPINLOCK_FOR_CONDITIONLOCK
    : myLock(false)
    #else
    : myLock(*this)
    #endif
    {}

    // Block until we're triggered
    void	waitForTrigger()
    {
    #ifdef USE_SPINLOCK_FOR_CONDITIONLOCK
        myLock.waitForTrigger();
    #else
        myLock.lock();
        myCond.waitForTrigger(myLock);
        myLock.unlock();
    #endif
    }

    // Trigger one waiting thread
    void        triggerOne()
    #ifdef USE_SPINLOCK_FOR_CONDITIONLOCK
    { myLock.trigger(); }
    #else
    { myCond.triggerOne(); }
    #endif

private:
    UT_ConditionLock(const UT_ConditionLock &copy);		// not allowed
    UT_ConditionLock &operator=(const UT_ConditionLock &copy);	// not allowed

#ifdef USE_SPINLOCK_FOR_CONDITIONLOCK
    UT_SpinLock myLock;
#else
    UT_Lock     myLock;
    UT_Condition myCond;
#endif
};

#endif
