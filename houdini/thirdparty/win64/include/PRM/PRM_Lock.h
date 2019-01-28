/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Lock.h (PRM Library, C++)
 *
 * COMMENTS:	This header allows us to enable/disable locking when cooking.
 */

#ifndef __PRM_LOCK_H_INCLUDED__
#define __PRM_LOCK_H_INCLUDED__

#include "PRM_API.h"

#include <tbb/spin_rw_mutex.h>

// Modify this to enable/disable node cook locking
#define PRM_LOCKS 1

#if PRM_LOCKS
    typedef tbb::spin_rw_mutex		    PRM_SpinLock;
    typedef tbb::spin_rw_mutex::scoped_lock PRM_SpinLockScope;

    class PRM_API PRM_SpinLockReadScope : private PRM_SpinLockScope
    {
    public:
	PRM_SpinLockReadScope()
	    : PRM_SpinLockScope()
	{
	}

	PRM_SpinLockReadScope(PRM_SpinLock &lock)
	    : PRM_SpinLockScope(lock, /*write*/false)
	{
	}

	// Acquire lock on given mutex
	void acquire(PRM_SpinLock &lock)
	{
	    PRM_SpinLockScope::acquire(lock, /*write*/false);
	}

	// Change reader lock to a writer lock. Returns false if the lock was
	// released in favour of another upgrade request and then reacquired;
	// true otherwise.
	bool upgradeToWriter()
	{
	    return PRM_SpinLockScope::upgrade_to_writer();
	}
    };
    class PRM_API PRM_SpinLockWriteScope : private PRM_SpinLockScope
    {
    public:
	PRM_SpinLockWriteScope()
	    : PRM_SpinLockScope()
	{
	}

	PRM_SpinLockWriteScope(PRM_SpinLock &lock)
	    : PRM_SpinLockScope(lock, /*write*/true)
	{
	}

	// Acquire lock on given mutex
	void acquire(PRM_SpinLock &lock)
	{
	    PRM_SpinLockScope::acquire(lock, /*write*/true);
	}
    };

#else
    typedef UT_NullLock			    PRM_SpinLock;
    typedef UT_NullLock::Scope		    PRM_SpinLockScope;
#endif

#endif // __PRM_LOCK_H_INCLUDED__
