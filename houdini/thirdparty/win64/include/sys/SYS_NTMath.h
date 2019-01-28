#ifndef _SYS_NTMath_h_
#define _SYS_NTMath_h_

#include "SYS_API.h"
#include "SYS_Types.h"

// Include these system libraries first so that our tests for math defines
// won't try to refine them.
#include <stdlib.h>
#include <math.h>

#if defined(CELLRSX)
    #include <ppu_intrinsics.h>
#endif

#ifdef WIN32

    #define SYS_HAS_NO_DRAND
    #define SYS_HAS_NO_RANDOM

    static inline int finite(double x)	    { return ::_finite(x); }
    static inline int finitef(double x)	    { return ::_finite(x); }
    static inline int isfinite(double x)    { return ::_finite(x); }

    #ifdef __cplusplus
	#include <ieeefp.h>

	// trunc() and truncf() don't exist, so create them.
	static inline fpreal32	truncf(fpreal32 value)
	{
	    return (value < 0.0f) ?  ceilf(value) : floorf(value);
	}

	static inline fpreal64	trunc(fpreal64 value)
	{
	    return (value < 0.0) ?  ceil(value) : floor(value);
	}
    #endif /* __cplusplus */
#endif /* WIN32 */

#if defined(GAMEOS)
    #define SYS_HAS_NO_DRAND
    #define SYS_HAS_NO_RANDOM
    #define SYS_HAS_NO_PI
    static inline int finite(double) { return 1; }
#endif

#if defined(SYS_HAS_NO_DRAND) && defined(__cplusplus)
// We carefully do *not* specify srand48 and drand48 in these
// cases!  Use SYSsrand48 and SYSdrand48 instead!
#endif

#if defined(SYS_HAS_NO_RANDOM) && defined(__cplusplus)
    // Likewise, random() and srandom() are considered harmful.
#endif

#if defined(SYS_HAS_NO_PI)
    #undef M_PI
    #undef M_PI_2
    #undef M_PI_4
    #define M_PI         3.14159265358979323846
    #define M_PI_2       1.57079632679489661923
    #define M_PI_4       0.78539816339744830962
#endif

#if !defined(M_SQRT2)
    #define M_SQRT2	1.41421356237309504880
#endif

#endif /* _SYS_NTMath_h_ */
