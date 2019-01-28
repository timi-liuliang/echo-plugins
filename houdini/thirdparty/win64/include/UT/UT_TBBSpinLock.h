/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TBBSpinLock.h (UT Library, C++)
 *
 * COMMENTS:	
 *		
 */

#ifndef __UT_TBBSPINLOCK_H_INCLUDED__
#define __UT_TBBSPINLOCK_H_INCLUDED__

#include "UT_API.h"
#include "UT_Lockable.h"
#include <tbb/spin_mutex.h>

/// Spin lock implementation from TBB
/// @note This is @b NOT recursive!
typedef UT_Lockable<tbb::spin_mutex>    UT_TBBSpinLock;

#endif // __UT_TBBSPINLOCK_H_INCLUDED__
