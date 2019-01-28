/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Lockable.h (UT Library, C++)
 *
 * COMMENTS:	
 *		
 */

#ifndef __UT_LOCKABLE_H_INCLUDED__
#define __UT_LOCKABLE_H_INCLUDED__

#include "UT_API.h"
#include "UT_LockUtil.h"
#include "UT_NonCopyable.h"
#include <SYS/SYS_Types.h>

/// UT_Lock interface adapter for C++11 mutexes
template <typename MUTEX>
class UT_Lockable : UT_NonCopyable
{
public:
    UT_Lockable()	{ }
    ~UT_Lockable()	{ }

    typedef UT_LockScopeType< UT_Lockable<MUTEX> >
			Scope;

    bool tryLock()	{ return myMutex.try_lock(); }
    void lock()		{ myMutex.lock(); }
    void unlock()	{ myMutex.unlock(); }
    bool safeLock()	{ lock(); return true; }

    bool isLocked()
    {
	if (tryLock())
	{
	    unlock();
	    return false;
	}
	return true;
    }

    exint getMemoryUsage(bool inclusive) const
    {
	return (inclusive ? sizeof(*this) : 0);
    }

protected:
    MUTEX& mutex()	{ return myMutex; }

private:
    MUTEX		myMutex;
};

#endif // __UT_LOCKABLE_H_INCLUDED__
