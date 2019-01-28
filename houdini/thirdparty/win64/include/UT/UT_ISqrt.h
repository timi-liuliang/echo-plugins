/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library (C++)
 *
 * COMMENTS:	Fast lookup to get integer square roots.  The square roots
 *		will round, thus UTisqrt(3) = 2, UTisqrt(2) = 1
 *		The table is good for positive integers up to 1024
 *
 */

#ifndef __UT_ISqrt_h__
#define __UT_ISqrt_h__

#include "UT_API.h"
#include <SYS/SYS_Types.h>

#define MAX_ISQRT	1024

UT_API extern int	UTisqrtTable[];
UT_API extern float	UTinvSqrtTable[];

UT_API unsigned int	UTbigISqrt(unsigned int val);

inline int	UTisqrtFast(unsigned val)	  { return UTisqrtTable[val]; }
inline float	UTinverseSqrtFast(unsigned val)	{ return UTinvSqrtTable[val]; }

inline int
UTisqrt(unsigned val)
{
    return val <= MAX_ISQRT ? UTisqrtTable[val] : UTbigISqrt(val);
}

inline float
UTinverseSqrt(unsigned val)
{
    return val <= MAX_ISQRT ? UTinvSqrtTable[val]
			    : 1.0F/(float)UTbigISqrt(val);
}

#endif
