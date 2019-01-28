/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ShowCounts.h ( UT Library, C++)
 *
 * COMMENTS:	Some macros for defining counters.  This file should never be
 *		included in a header for obvious reasons.  Define SHOW_COUNTS
 *		before including the header.
 */

#ifndef __UT_ShowCounts__
#define __UT_ShowCounts__

#if defined(SHOW_COUNTS) && UT_ASSERT_LEVEL > 0
    #if !defined(UT_DEBUG)
	#define UT_DEBUG 1
	#include "UT_Counter.h"
	#undef UT_DEBUG
    #else
	#include "UT_Counter.h"
    #endif

    #define UT_COUNTER(name, label)	static UT_Counter	name(label);
    #define UT_INC_COUNTER(name)		name++;
    #define UT_ADD_COUNTER(name, amount)	name += amount;
    #define UT_DEC_COUNTER(name)		name--;
    #define UT_MAX_COUNTER(name, value)		name.max(value);
#else
    #define UT_COUNTER(name, label)
    #define UT_INC_COUNTER(name)
    #define UT_ADD_COUNTER(name, amount)
    #define UT_DEC_COUNTER(name)
    #define UT_MAX_COUNTER(name, value)
#endif
#endif
