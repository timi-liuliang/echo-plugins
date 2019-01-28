/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_MathPermute.h ( SYS Library, C++)
 *
 * COMMENTS:	This header file defines all the impure permutations of
 *		fpreal32 and fpreal64 for the SYS_Math.h library functions.
 *		All fpreal32's are cast up to fpreal64 for the computations.
 */

#ifndef __SYS_MathPermute__
#define __SYS_MathPermute__

#ifndef __SYS_Math__
  #error "SYS_MathPermute.h should only be included by SYS_Math.h"
#endif

#define ARGS_2(fname) \
    static inline fpreal64 fname(fpreal32 a, fpreal64 b) \
	{ return fname((fpreal64)a, b); } \
    static inline fpreal64 fname(fpreal64 a, fpreal32 b) \
	{ return fname(a, (fpreal64)b); } \

#define ARGS_3(fname) \
    static inline fpreal64 fname(fpreal32 a, fpreal64 b, fpreal64 c) \
	{ return fname((fpreal64)a, b, c); } \
    static inline fpreal64 fname(fpreal64 a, fpreal32 b, fpreal64 c) \
	{ return fname(a, (fpreal64)b, c); } \
    static inline fpreal64 fname(fpreal32 a, fpreal32 b, fpreal64 c) \
	{ return fname((fpreal64)a, (fpreal64)b, c); } \
    static inline fpreal64 fname(fpreal64 a, fpreal64 b, fpreal32 c) \
	{ return fname(a, b, (fpreal64)c); } \
    static inline fpreal64 fname(fpreal32 a, fpreal64 b, fpreal32 c) \
	{ return fname((fpreal64)a, b, (fpreal64)c); } \
    static inline fpreal64 fname(fpreal64 a, fpreal32 b, fpreal32 c) \
	{ return fname(a, (fpreal64)b, (fpreal64)c); }

#define ARGS_4(fname) \
    static inline fpreal64 fname(fpreal64 a,fpreal32 b,fpreal32 c,fpreal32 d) \
	{ return fname(a, (fpreal64)b, (fpreal64)c, (fpreal64)d); } \
    static inline fpreal64 fname(fpreal32 a,fpreal64 b,fpreal32 c,fpreal32 d) \
	{ return fname((fpreal64)a, b, (fpreal64)c, (fpreal64)d); } \
    static inline fpreal64 fname(fpreal64 a,fpreal64 b,fpreal32 c,fpreal32 d) \
	{ return fname(a, b, (fpreal64)c, (fpreal64)d); } \
    static inline fpreal64 fname(fpreal32 a,fpreal32 b,fpreal64 c,fpreal32 d) \
	{ return fname((fpreal64)a, (fpreal64)b, c, (fpreal64)d); } \
    static inline fpreal64 fname(fpreal64 a,fpreal32 b,fpreal64 c,fpreal32 d) \
	{ return fname(a, (fpreal64)b, c, (fpreal64)d); } \
    static inline fpreal64 fname(fpreal32 a,fpreal64 b,fpreal64 c,fpreal32 d) \
	{ return fname((fpreal64)a, b, c, (fpreal64)d); } \
    static inline fpreal64 fname(fpreal64 a,fpreal64 b,fpreal64 c,fpreal32 d) \
	{ return fname(a, b, c, (fpreal64)d); } \
    static inline fpreal64 fname(fpreal32 a,fpreal32 b,fpreal32 c,fpreal64 d) \
	{ return fname((fpreal64)a, (fpreal64)b, (fpreal64)c, d); } \
    static inline fpreal64 fname(fpreal64 a,fpreal32 b,fpreal32 c,fpreal64 d) \
	{ return fname(a, (fpreal64)b, (fpreal64)c, d); } \
    static inline fpreal64 fname(fpreal32 a,fpreal64 b,fpreal32 c,fpreal64 d) \
	{ return fname((fpreal64)a, b, (fpreal64)c, d); } \
    static inline fpreal64 fname(fpreal64 a,fpreal64 b,fpreal32 c,fpreal64 d) \
	{ return fname(a, b, (fpreal64)c, d); } \
    static inline fpreal64 fname(fpreal32 a,fpreal32 b,fpreal64 c,fpreal64 d) \
	{ return fname((fpreal64)a, (fpreal64)b, c, d); } \
    static inline fpreal64 fname(fpreal64 a,fpreal32 b,fpreal64 c,fpreal64 d) \
	{ return fname(a, (fpreal64)b, c, d); } \
    static inline fpreal64 fname(fpreal32 a,fpreal64 b,fpreal64 c,fpreal64 d) \
	{ return fname((fpreal64)a, b, c, d); }

ARGS_2(SYSmin)
ARGS_3(SYSmin)
ARGS_4(SYSmin)
ARGS_2(SYSmax)
ARGS_3(SYSmax)
ARGS_4(SYSmax)
ARGS_3(SYSavg)
ARGS_4(SYSavg)

ARGS_3(SYSclamp)
ARGS_3(SYSlerp)
ARGS_3(SYSsmooth)
ARGS_4(SYSsmooth)

// TODO: SYSminmax
// TODO: SYSfit

ARGS_2(SYSatan)
ARGS_2(SYSatan2)
ARGS_2(SYShypot)

ARGS_2(SYSroundAngle)

static inline bool
SYSequalZero(fpreal32 v, fpreal64 tol)
    { return SYSequalZero(v, (fpreal32)tol); }

static inline bool
SYSequalZero(fpreal64 v, fpreal32 tol)
    { return SYSequalZero(v, (fpreal64)tol); }

#define CMP_PERM(fname)	\
    static inline bool fname(fpreal32 a, fpreal32 b, fpreal64 tol) \
	    { return fname(a, b, (fpreal32)tol); } \
    static inline bool fname(fpreal64 a, fpreal64 b, fpreal32 tol) \
	    { return fname(a, b, (fpreal64)tol); } \
    static inline bool fname(fpreal32 a, fpreal64 b, fpreal32 tol) \
	    { return fname((fpreal64)a, b, (fpreal64)tol); } \
    static inline bool fname(fpreal64 a, fpreal32 b, fpreal32 tol) \
	    { return fname(a, (fpreal64)b, (fpreal64)tol); } \
    static inline bool fname(fpreal32 a, fpreal64 b, fpreal64 tol=SYS_FTOLERANCE_D) \
	    { return fname((fpreal64)a, b, tol); } \
    static inline bool fname(fpreal64 a, fpreal32 b, fpreal64 tol=SYS_FTOLERANCE_D) \
	    { return fname(a, (fpreal64)b, tol); } \
    static inline bool fname(fpreal32 a, int b, fpreal32 tol=SYS_FTOLERANCE) \
	    { return fname(a, (fpreal32)b, tol); } \
    static inline bool fname(fpreal32 a, int b, fpreal64 tol) \
	    { return fname((fpreal64)a, (fpreal64)b, tol); } \
    static inline bool fname(fpreal64 a, int b, fpreal64 tol=SYS_FTOLERANCE_D) \
	    { return fname(a, (fpreal64)b, tol); } \
    static inline bool fname(int a, fpreal32 b, fpreal32 tol=SYS_FTOLERANCE) \
	    { return fname((fpreal32)a, b, tol); } \
    static inline bool fname(int a, fpreal32 b, fpreal64 tol) \
	    { return fname((fpreal64)a, (fpreal64)b, tol); } \
    static inline bool fname(int a, fpreal64 b, fpreal64 tol=SYS_FTOLERANCE_D) \
	    { return fname((fpreal64)a, b, tol); }

CMP_PERM(SYSisEqual)
CMP_PERM(SYSisGreater)
CMP_PERM(SYSisGreaterOrEqual)
CMP_PERM(SYSisLess)
CMP_PERM(SYSisLessOrEqual)

#undef CMP_PERM
#undef ARGS_2
#undef ARGS_3
#undef ARGS_4

#endif
