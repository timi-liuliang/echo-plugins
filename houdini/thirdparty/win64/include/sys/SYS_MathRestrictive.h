/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_ObsoleteFunctions.h ( SYS Library, C++)
 *
 * COMMENTS:	Since there are SYS versions of many functions, we want to
 *		catch these in our builds.  Thus, we want to have defines which
 *		cause compile errors when they are used.  This file is included
 *		from SYS_Math.  It is turned off during the HDK (since we can
 *		allow users to make std calls.
 */

#ifndef __SYS_ObsoleteFunctions__
#define __SYS_ObsoleteFunctions__

//#define SYS_RESTRICTIVE_MATH

#if defined(SYS_RESTRICTIVE_MATH)

#define SYS_RESTRICT_UNARY(name)	\
    static inline fpreal64 name(fpreal64 x, int use_sys=0) { return x; } \
    static inline fpreal32 name##f(fpreal32 x, int use_sys=0) { return x; }

#define SYS_RESTRICT_BINARY(name)	\
    static inline fpreal64 name(fpreal64 x, fpreal64 y, int use_sys=0) \
	{ return x; } \
    static inline fpreal32 name##f(fpreal32 x, fpreal32 y, int use_sys=0) \
	{ return x; }

SYS_RESTRICT_UNARY(sin)
SYS_RESTRICT_UNARY(cos)
SYS_RESTRICT_UNARY(tan)
SYS_RESTRICT_UNARY(asin)
SYS_RESTRICT_UNARY(acos)
SYS_RESTRICT_UNARY(atan)
SYS_RESTRICT_UNARY(sinh)
SYS_RESTRICT_UNARY(cosh)
SYS_RESTRICT_UNARY(tanh)
SYS_RESTRICT_UNARY(asinh)
SYS_RESTRICT_UNARY(acosh)
SYS_RESTRICT_UNARY(atanh)

SYS_RESTRICT_UNARY(sqrt)
SYS_RESTRICT_UNARY(cbrt)
SYS_RESTRICT_UNARY(log)
SYS_RESTRICT_UNARY(log1p)
SYS_RESTRICT_UNARY(log10)
SYS_RESTRICT_UNARY(exp)
SYS_RESTRICT_UNARY(expm1)

SYS_RESTRICT_UNARY(fabs)
SYS_RESTRICT_UNARY(trunc)
SYS_RESTRICT_UNARY(floor)
SYS_RESTRICT_UNARY(ceil)

SYS_RESTRICT_BINARY(fmod)
SYS_RESTRICT_BINARY(pow)
SYS_RESTRICT_BINARY(atan2)
SYS_RESTRICT_BINARY(hypot)

#endif

#endif
