/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	EXPR_Lock.h (EXPR Library, C++)
 *
 * COMMENTS:	This header allows us to enable/disable locking when evaluating
 *		parameters and hscript expressions.
 */

#ifndef __EXPR_LOCK_H_INCLUDED__
#define __EXPR_LOCK_H_INCLUDED__

#include "EXPR_API.h"

#include <UT/UT_LockUtil.h>
#include <UT/UT_TaskLock.h>
#include <UT/UT_TBBSpinLock.h>

// Modify this to enable/disable parm expr locking
#define EXPR_LOCKS 1

#if EXPR_LOCKS
    typedef UT_TBBSpinLock	    EXPR_Lock;
    typedef UT_TaskLockWithArena    EXPR_GlobalStaticLock;
#else
    typedef UT_NullLock		    EXPR_Lock;
    typedef UT_NullLock		    EXPR_GlobalStaticLock;
#endif

typedef EXPR_Lock::Scope	    EXPR_AutoLock;

/// The global evaluation lock is a global mutex for protecting any
/// non-threadsafe evaluation (both expressions and node) code. This is
/// necessary to guard against code that might access global objects.
EXPR_API EXPR_GlobalStaticLock &    ev_GlobalEvalLock();

#endif // __EXPR_LOCK_H_INCLUDED__
