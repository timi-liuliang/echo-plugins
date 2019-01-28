/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Lock.h (OP Library, C++)
 *
 * COMMENTS:	This header allows us to enable/disable locking when cooking.
 */

#ifndef __OP_LOCK_H_INCLUDED__
#define __OP_LOCK_H_INCLUDED__

#include "OP_API.h"

#include <UT/UT_Lock.h>

// Modify this to enable/disable node cook locking
#define OP_LOCKS 1

#if OP_LOCKS
    typedef UT_Lock			OP_Lock;
    typedef UT_Lock			OP_CookLock;
#else
    typedef UT_NullLock			OP_Lock;
    typedef UT_NullLock			OP_CookLock;
#endif

#endif // __OP_LOCK_H_INCLUDED__
