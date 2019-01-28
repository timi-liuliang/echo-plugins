/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_DoubleLock.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_DoubleLock__
#define __UT_DoubleLock__

#include "UT_API.h"

#include "UT_NonCopyable.h"
#include "UT_Lock.h"
#include <SYS/SYS_MemoryOrder.h>

///
/// A double-checked lock.  Only locks the thread lock when the value is 0.
/// 
/// To use this lock:
///
/// static UT_Lock theLock;
/// static OBJECT *theObj = 0;
///
/// OBJECT *
/// getSingleton()
/// {
///     UT_DoubleLock<OBJECT *> lock(theLock, theObj);
///
///     if (!lock.getValue())
///     {
///         // NOTE: This doesn't set theObj.  theObj is set when
///         //       destructing lock.
///         lock.setValue(new OBJECT());
///     }
///     return lock.getValue();
/// }
///
/// NOTE: DO NOT roll your own double checked lock!  If you
///       don't know when to use which memory fences and why,
///       odds are high that your custom implementation will be
///       thread-unsafe.
///
/// For simple singleton initialization, consider using UT_Singleton.
///
template <typename T, typename LOCK_T = UT_Lock>
class UT_DoubleLock : UT_NonCopyable
{
public:
    UT_DoubleLock(LOCK_T &lock, volatile T &val)
	: myLock(lock)
	, myValue(val)
	, myPendingValue(0)
	, myIsLocked(false)
	, myNeedInit(false)
    {
	if (!val)
	{
	    myLock.lock();
	    myIsLocked = true;
	    if (!val)
		myNeedInit = true;
	}
    }
    
    ~UT_DoubleLock()
    {
	if (myIsLocked)
	{
	    if (myNeedInit)
	    {
		UT_ASSERT_P(myPendingValue);

                // Ensure that the data that myPendingValue refers to is
                // written out to main memory before setting myValue.
                SYSstoreFence();

		myValue = myPendingValue;

                // NOTE: LOCK_T::unlock also does a store fence before
                //       unlocking, which ensures that myValue is written out to
                //       main memory before the lock is unlocked.  If it didn't,
                //       the next thread might read myValue of 0 and try to
                //       initialize it again.  The store fence above is also
                //       needed.
	    }
	    myLock.unlock();
	}
    }

    T		 getValue()
    {
        T v = myPendingValue;
	if (v)
	    return v;

	v = myValue;

        // This load fence is to ensure that any side effects of allocation
        // in another thread, including values in the heap itself, are not
        // speculatively read before the read of myValue.  Also, some places
        // do more than allocate a single item pointed to by v, especially
        // for cases with UT_DoubleLock<bool>, so those count as side
        // effects here.
        SYSloadFence();

        return v;
    }
    void	 setValue(T val)
    {
	UT_ASSERT_P(myNeedInit);
	myPendingValue = val;
    }

    /// Abort writing back out to the val passed in the constructor upon
    /// destruction.
    void	 abort()
    {
	myNeedInit = false;
    }
    
private:
    LOCK_T	&myLock;
    volatile T	&myValue;
    T		 myPendingValue;
    bool	 myIsLocked;
    bool	 myNeedInit;
};

#endif

