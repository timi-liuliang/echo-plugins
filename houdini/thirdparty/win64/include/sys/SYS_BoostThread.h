/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_BoostThread.h (UT Library, C++)
 *
 * COMMENTS:	This header should be included by anyone wanting to use
 *		boost thread, rather than including the boost header directly.
 */

#ifndef __SYS_BOOSTTHREAD_H_INCLUDED__
#define __SYS_BOOSTTHREAD_H_INCLUDED__

// Including <hboost/thread.hpp> results in <windows.h> being included
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Disable use of HBOOST_THREAD_WIN32_HAS_GET_TICK_COUNT_64 by tricking it
// by include sdkddkver.h to fix bug 66054

#if defined(_MSC_VER)
#include <sdkddkver.h>
#endif

#if defined(MBSD) && _POSIX_C_SOURCE >= 200112L
// getpagesize(3) has been deprecated in 10.7.6
inline int getpagesize()
{
    return (int)sysconf(_SC_PAGESIZE);
}

#endif

#include "SYS_Pragma.h"

// Keep this list of includes lean and mean.
// Only include what is absolutely needed.
#include <hboost/thread/condition_variable.hpp>
#include <hboost/thread/mutex.hpp>
#include <hboost/thread/recursive_mutex.hpp>
#include <hboost/thread/tss.hpp>
#include <hboost/thread/thread_time.hpp>
#include <hboost/thread/barrier.hpp>

// Double-check that we do not have this defined
#ifdef HBOOST_THREAD_WIN32_HAS_GET_TICK_COUNT_64
#error Do not use GetTickCount64()!
#endif

#endif // __SYS_BOOSTTHREAD_H_INCLUDED__
