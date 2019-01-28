/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Math.h (SYS Library, C++)
 *
 * COMMENTS:	Houdini math interface. 
 *		statements like: a = sin(x); b = sinf(y);
 *		we use:          a = SYSsin(x); b = SYSsin(y);
 *
 * Functions defined here:
 *	SYSmin(a,b)			- Returns the minimum of a or b
 *	SYSmin(a,b,c)			- Returns the minimum of a, b or c
 *	SYSmin(a,b,c,d)			- Returns minimum of a, b, c or d
 *	SYSargmin(a,b)			- Returns index of minimum number
 *	SYSargmin(a,b,c)		- Returns index of minimum number
 *	SYSargmin(a,b,c,d)		- Returns index of minimum number
 *	SYSmax(a,b)			- Returns the maximum of a or b
 *	SYSmax(a,b,c)			- Returns the maximum of a, b or c
 *	SYSmax(a,b,c,d)			- Returns maximum of a, b, c or d
 *	SYSsort(a, b)			- Sorts so a < b
 *	SYSsort(a, b, c)		- Sorts so a < b < c
 *	SYSargmax(a,b)			- Returns index of largest number
 *	SYSargmax(a,b,c)		- Returns index of largest number
 *	SYSargmax(a,b,c,d)		- Returns index of largest number
 *	SYSabs(x)			- Works for all types (including fpreal)
 *	SYSavg(a,b,c)			- Return average value of 3 values
 *	SYSavg(a,b,c,d)			- Return average value of 4 values
 *
 *	SYSminmax(a,b,&min.&max)	- Find min and max of 2 values
 *	SYSminmax(a,b,c,&min.&max)	- Find min and max of 3 values
 *	SYSminmax(a,b,c,d,&min.&max)	- Find min and max of 4 values
 *
 *	SYSwrapmod(a, b)		- Returns [0..b) interval for +ve b.
 *
 *      SYSsignum(a)                    - Numerically robust Sign function:
 *                                        -1, if a is negative
 *                                         1, if a is positive
 *                                         0, otherwise:
 *
 *  Comparison:
 *	SYSequalZero(a, [tol])		- Is a equal to zero
 *	SYSisEqual(a, b, tol)		- 
 *	SYSalmostEqual(a, b, ulps)	- Almost equal given units in last place
 *
 *  Interpolation/Range:
 *	SYSclamp(a, min, max)		- Clamp value between min and max
 *	SYSsmooth(min, max, a)		- Ease-in ease-out curve
 *	SYSsmooth(min, max, a, roll)	- Ease-in ease-out curve (with rolloff)
 *	SYSlerp(v1, v2, bias)		- Linear interpolation
 *	SYSfit(v, omin,omax, nmin,nmax)	- Map v in (omin,omax) to (nmin,nmax)
 *
 *   The bilerp function expects:
 *        u0v1--u1v1
 *         |     |
 *        u0v0--u1v0
 *   Where u is horizontal and v is vertical in the diagram above.
 *	SYSbilerp(u0v0, u1v0, u0v1, u1v1, u, v)
 *
 *  Standard math (single/double precision signatures):
 *	SYSsin(x)
 *	SYScos(x)
 *	SYStan(x)
 *	SYSsqrt(x)
 *	SYSlog(x)
 *	SYSfabs(x)
 *	SYStrunc(x)
 *	SYSfloor(x)
 *	SYSceil(x)
 *	SYScbrt(x)      -- Use #include <SYS/SYS_MathCbrt.h>
 *	SYSlog10(x)
 *	SYSfmod(x,y)
 *	SYSpow(x,y)
 *	SYSsafepow(x, y)
 *	SYSatan(x)
 *	SYSatan(y,x)	-- Note, SYSatan(y,x) is equivalent to SYSatan2(y,x)
 *	SYSatan2(y,x)
 *	SYShypot(x,y)
 *	SYSsafediv(x,y)		- Divide only if y != 0
 *	SYSsafesqrt(x)		- Compute sqrt only if x >= 0
 *	SYSsafefmod(x, y)	- Mod only if y != 0
 *
 *  Random:
 *	SYSpointerHash		- Generate a hash key for a pointer
 *	SYSwang_inthash		- 32-bit Integer hashing function
 *	SYSwang_inthash64	- 64-bit Integer hashing function
 *	SYSreal_hash		- Generate a hash for a real number
 *	SYSreal_hashseed	- Generate a hash for a real number
 *	SYSvector_hash		- Generate a hash for a real vector
 *	SYSfastRandom		- Really fast random number generator (0,1)
 *	SYSrandom		- Fast random number (fewer artifacts) (0,1)
 *	SYSfastRandomZero	- Really fast random number generator (-.5, .5)
 *	SYSrandomZero		- Fast random number (fewer artifacts) (-.5, .5)
 *
 *  Rounding:
 *	SYSroundDownToMultipleOf	-
 *	SYSroundUpToMultipleOf	-
 *	SYSroundAngle		-
 *
 *  Misc:
 *      SYSisNan                - Is this a valid floating-point number?
 *	SYSisFinite		- Is not a NAN or Infinity
 *	SYSisInt		- Is the string an integer
 *	SYSisFloat		- Is the string a float
 *	SYSisPrime		- Is it a prime?
 *	SYSsameSign(a,b)	- Do the non-zero numbers have the same sign?
 *				(Note: does not work if either number is zero)
 *	SYSmakePrime		- Make it a prime
 *	SYSnextPrime		- Next prime
 *	SYSgetSinCosFromSlope	- Compute sin/cos given a slope
 *	SYShexCharToInt		- map '0'-'9,'a'-'z' to 0-15, or -1
 *	SYSintToHexChar		- map 0-15 to '0'-'9,'a'-'z'
 *      SYSdivMod(n,d,q,r)      - q = n/d; r = n%d; The compiler should make
 *                                just one DIV or IDIV instruction.
 *
 *  For information on almostEqual, please see:
 *	http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
 *
 */

#ifndef __SYS_Math__
#define __SYS_Math__

#include "SYS_API.h"
#include "SYS_Types.h"

/*
 * System dependent includes
 */
#if defined(WIN32)
#  include "SYS_NTMath.h"
#endif

#if defined(LINUX)
#  include "SYS_FastMath.h" 
#endif

#include <float.h>
#include <limits>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// We do not want to use the built-ins for srand48 or drand48 as they
// are not threadsafe.  This version uses thread local storage to
// avoid that.
SYS_API void 		SYSsrand48(long seed);
SYS_API double		SYSdrand48();

#if !defined(SESI_ALLOW_DRAND48)

// These macros are to aggressively prevent anyone from accidetnally
// using thread unsafe functions
#undef srand48
#undef drand48
#define srand48(X) static_assert(0, "Use SYSsrand48() instead")
#define drand48(X) 0; { static_assert(0, "Use SYSdrand48() instead"); }

#endif

// It would be useful to disable rand() and srand() as well, but
// they are used by std::rand :<

template< typename F >
inline int SYSsignum( const F a )
{
    return ( int( 0 < a ) - int( a < 0 ) );
}

// Determine whether f is not-a-number (NAN) for any floating point type f
template< typename F > inline bool
SYSisNan(const F f)
{
    return(f != f);
}

template <> inline bool
SYSisNan(const fpreal16 f)
{
    return f.isNan();
}

/// @{
/// Check whether a number is finite.  That is, not Nan and not infinity.
/// @see SYSisNan()
#if defined(LINUX)
#include <cmath>
inline bool SYSisFinite(fpreal64 f) { return std::isfinite(f) != 0; }
inline bool SYSisFinite(fpreal32 f) { return std::isfinite(f) != 0; }
#elif defined(MBSD)
inline bool SYSisFinite(fpreal64 f) { return isfinite(f) != 0; }
inline bool SYSisFinite(fpreal32 f) { return isfinite(f) != 0; }
#elif defined(WIN32)
inline bool SYSisFinite(fpreal64 f) { return ::_finite(f) != 0; }
inline bool SYSisFinite(fpreal32 f) { return ::_finite(f) != 0; }
#endif
inline bool SYSisFinite(fpreal16 f) { return f.isFinite(); }
inline bool SYSisFinite(int32 f) { return true; }
inline bool SYSisFinite(int64 f) { return true; }
/// @}

/// SYSisNan() checks whether the string represents a non-finite number
SYS_API bool		SYSisNan(const char *number);
SYS_API bool		SYSisInt(const char *str);
SYS_API bool		SYSisFloat(const char *str);
SYS_API fpreal32	SYSroundAngle(fpreal32 base, fpreal32 source);
SYS_API fpreal64	SYSroundAngle(fpreal64 base, fpreal64 source);
SYS_API fpreal64	SYSroundAngle(int32 base, int32 source);
SYS_API fpreal64	SYSroundAngle(int64 base, int64 source);
SYS_API bool		SYSisPrime(uint num);
SYS_API bool		SYSisPrime(uint64 num);
SYS_API uint		SYSmakePrime(uint num);
SYS_API uint64		SYSmakePrime(uint64 num);

#if defined(__cplusplus)

// NOTE:
// These have been carefully written so that in the case of equality
// we always return the first parameter.  This is so that NANs in
// in the second parameter are suppressed.
#define h_min(a, b)	(((a) > (b)) ? (b) : (a))
#define h_argmin(a, b)  (((a) > (b)) ? 1 : 0)
#define h_max(a, b)	(((a) < (b)) ? (b) : (a))
#define h_argmax(a, b)  (((a) < (b)) ? 1 : 0)
// DO NOT CHANGE THE ABOVE WITHOUT READING THE COMMENT
#define h_abs(a)	(((a) > 0) ? (a) : -(a))
#define h_sgn(a)	(((a) > 0) ? 1 : (((a) < 0) ? -1 : 0))


static constexpr inline  int16 SYSmin(int16 a, int16 b)		{ return h_min(a,b); }
static constexpr inline  int16 SYSmax(int16 a, int16 b)		{ return h_max(a,b); }
static constexpr inline  int16 SYSabs(int16 a)			{ return h_abs(a); }
static constexpr inline  int16 SYSsgn(int16 a)			{ return h_sgn(a); }
static constexpr inline    int SYSargmin(int16 a, int16 b)	{ return h_argmin(a,b);}
static constexpr inline    int SYSargmax(int16 a, int16 b)	{ return h_argmax(a,b);}
static constexpr inline  int32 SYSmin(int32 a, int32 b)		{ return h_min(a,b); }
static constexpr inline  int32 SYSmax(int32 a, int32 b)		{ return h_max(a,b); }
static constexpr inline  int32 SYSabs(int32 a)			{ return h_abs(a); }
static constexpr inline  int32 SYSsgn(int32 a)			{ return h_sgn(a); }
static constexpr inline    int SYSargmin(int32 a, int32 b)	{ return h_argmin(a,b);}
static constexpr inline    int SYSargmax(int32 a, int32 b)	{ return h_argmax(a,b);}
static constexpr inline  int64 SYSmin(int64 a, int64 b)		{ return h_min(a,b); }
static constexpr inline  int64 SYSmax(int64 a, int64 b)		{ return h_max(a,b); }
static constexpr inline  int64 SYSmin(int32 a, int64 b)		{ return h_min(a,b); }
static constexpr inline  int64 SYSmax(int32 a, int64 b)		{ return h_max(a,b); }
static constexpr inline  int64 SYSmin(int64 a, int32 b)		{ return h_min(a,b); }
static constexpr inline  int64 SYSmax(int64 a, int32 b)		{ return h_max(a,b); }
static constexpr inline  int64 SYSabs(int64 a)			{ return h_abs(a); }
static constexpr inline  int64 SYSsgn(int64 a)			{ return h_sgn(a); }
static constexpr inline    int SYSargmin(int64 a, int64 b)	{ return h_argmin(a,b);}
static constexpr inline    int SYSargmax(int64 a, int64 b)	{ return h_argmax(a,b);}
static constexpr inline uint16 SYSmin(uint16 a, uint16 b)		{ return h_min(a,b); }
static constexpr inline uint16 SYSmax(uint16 a, uint16 b)		{ return h_max(a,b); }
static constexpr inline    int SYSargmin(uint16 a, uint16 b)	{ return h_argmin(a,b);}
static constexpr inline    int SYSargmax(uint16 a, uint16 b)	{ return h_argmax(a,b);}
static constexpr inline uint32 SYSmin(uint32 a, uint32 b)		{ return h_min(a,b); }
static constexpr inline uint32 SYSmax(uint32 a, uint32 b)		{ return h_max(a,b); }
static constexpr inline    int SYSargmin(uint32 a, uint32 b)	{ return h_argmin(a,b);}
static constexpr inline    int SYSargmax(uint32 a, uint32 b)	{ return h_argmax(a,b);}
static constexpr inline uint64 SYSmin(uint64 a, uint64 b)		{ return h_min(a,b); }
static constexpr inline uint64 SYSmax(uint64 a, uint64 b)		{ return h_max(a,b); }
static constexpr inline    int SYSargmin(uint64 a, uint64 b)	{ return h_argmin(a,b);}
static constexpr inline    int SYSargmax(uint64 a, uint64 b)	{ return h_argmax(a,b);}
static           inline fpreal16 SYSmin(fpreal16 a, fpreal16 b)	{ return h_min(a,b); }
static           inline fpreal16 SYSmax(fpreal16 a, fpreal16 b)	{ return h_max(a,b); }
static           inline fpreal16 SYSsgn(fpreal16 a)		{ return h_sgn(a); }
static           inline      int SYSargmin(fpreal16 a, fpreal16 b){ return h_argmin(a,b);}
static           inline      int SYSargmax(fpreal16 a, fpreal16 b){ return h_argmax(a,b);}
static constexpr inline fpreal32 SYSmin(fpreal32 a, fpreal32 b)	{ return h_min(a,b); }
static constexpr inline fpreal32 SYSmax(fpreal32 a, fpreal32 b)	{ return h_max(a,b); }
static constexpr inline fpreal32 SYSsgn(fpreal32 a)		{ return h_sgn(a); }
static constexpr inline      int SYSargmin(fpreal32 a, fpreal32 b){ return h_argmin(a,b);}
static constexpr inline      int SYSargmax(fpreal32 a, fpreal32 b){ return h_argmax(a,b);}
static constexpr inline fpreal64 SYSmin(fpreal64 a, fpreal64 b)	{ return h_min(a,b); }
static constexpr inline fpreal64 SYSmax(fpreal64 a, fpreal64 b)	{ return h_max(a,b); }
static constexpr inline fpreal64 SYSsgn(fpreal64 a)		{ return h_sgn(a); }
static constexpr inline      int SYSargmin(fpreal64 a, fpreal64 b){ return h_argmin(a,b);}
static constexpr inline      int SYSargmax(fpreal64 a, fpreal64 b){ return h_argmax(a,b);}

// Some systems have size_t as a seperate type from uint.  Some don't.
#if (defined(LINUX) && defined(IA64)) || defined(MBSD)
static constexpr inline size_t SYSmin(size_t a, size_t b)		{ return h_min(a,b); }
static constexpr inline size_t SYSmax(size_t a, size_t b)		{ return h_max(a,b); }
static constexpr inline    int SYSargmin(size_t a, size_t b)	{ return h_argmin(a,b);}
static constexpr inline    int SYSargmax(size_t a, size_t b)	{ return h_argmax(a,b);}
#endif

#undef h_min
#undef h_max
#undef h_abs
#undef h_sgn

#define h_clamp(val, min, max, tol)	\
	    ((val <= min+tol) ? min : ((val >= max-tol) ? max : val))

    static constexpr inline int
    SYSclamp(int v, int min, int max)
	{ return h_clamp(v, min, max, 0); }

    static constexpr inline uint
    SYSclamp(uint v, uint min, uint max)
	{ return h_clamp(v, min, max, 0); }

    static constexpr inline int64
    SYSclamp(int64 v, int64 min, int64 max)
	{ return h_clamp(v, min, max, CONST_INT64(0)); }

    static constexpr inline uint64
    SYSclamp(uint64 v, uint64 min, uint64 max)
	{ return h_clamp(v, min, max, CONST_UINT64(0)); }

    static constexpr inline fpreal32
    SYSclamp(fpreal32 v, fpreal32 min, fpreal32 max, fpreal32 tol=(fpreal32)0)
	{ return h_clamp(v, min, max, tol); }

    static constexpr inline fpreal64
    SYSclamp(fpreal64 v, fpreal64 min, fpreal64 max, fpreal64 tol=(fpreal64)0)
	{ return h_clamp(v, min, max, tol); }

#undef h_clamp

/// This converts from one integer type to another by clamping
/// the range, instead of the usual wrapping.
template<typename OUTTYPE,typename INTYPE>
OUTTYPE SYSclampInt(INTYPE value)
{
    value = SYSclamp(value, (INTYPE)std::numeric_limits<OUTTYPE>::min(),
			    (INTYPE)std::numeric_limits<OUTTYPE>::max());
    return OUTTYPE(value);
}

// Wrapper for libm math function calls 
#if defined(LINUX)
#  define SYS_MF(X) SYS_FastMath::X
#else
#  define SYS_MF(X) ::X
#endif
    
#define SYS_UNARY(func)							      \
    static inline fpreal64 SYS##func(fpreal64 arg) 			      \
				{ return SYS_MF(func)(arg); } 		      \
    static inline fpreal32 SYS##func(fpreal32 arg) 			      \
				{ return SYS_MF(func##f)(arg); } 	      \
    static inline fpreal64 SYS##func(int64 arg) 			      \
				{ return SYS_MF(func)((fpreal64)arg); }	      \
    static inline fpreal64 SYS##func(int32 arg) 			      \
				{ return SYS_MF(func)((fpreal64)arg); }
#define SYS_BINARY(func)						      \
    static inline fpreal64 SYS##func(fpreal64 a, fpreal64 b) 		      \
				{ return SYS_MF(func)(a,b); } 		      \
    static inline fpreal32 SYS##func(fpreal32 a, fpreal32 b) 		      \
				{ return SYS_MF(func##f)(a, b); }

#if defined(WIN32)
#define hypotf(x,y)	hypot((x),(y))
#endif
    
    SYS_UNARY(sin)
    SYS_UNARY(cos)
    SYS_UNARY(tan)
    SYS_UNARY(sinh)
    SYS_UNARY(cosh)
    SYS_UNARY(tanh)
    SYS_UNARY(sqrt)
    SYS_UNARY(trunc)
    SYS_UNARY(exp)
    SYS_BINARY(fmod)
    SYS_BINARY(pow)
    SYS_BINARY(atan2)
    SYS_BINARY(hypot)

    static inline fpreal32 SYSsafediv(fpreal32 x, fpreal32 y)
	{ return x/(y != 0 ? y : SYS_Types<fpreal32>::infinity()); }
    static inline fpreal64 SYSsafediv(fpreal64 x, fpreal64 y)
	{ return x/(y != 0 ? y : SYS_Types<fpreal64>::infinity()); }
    static inline fpreal32 SYSsafesqrt(fpreal32 x)
	{ return x > 0 ? SYSsqrt(x) : 0; }
    static inline fpreal64 SYSsafesqrt(fpreal64 x)
	{ return x > 0 ? SYSsqrt(x) : 0; }
    static inline fpreal32 SYSsafefmod(fpreal32 x, fpreal32 y)
	{ return y != 0 ? SYSfmod(x, y) : 0; }
    static inline fpreal64 SYSsafefmod(fpreal64 x, fpreal64 y)
	{ return y != 0 ? SYSfmod(x, y) : 0; }

#if 0
#include <xmmintrin.h>
    /// Computes 1/sqrt(x) to about 11.4 bits of accuracy
    static inline fpreal32 SYSrsqrt11(fpreal32 x)
    {
#if defined(WIN32) || defined(LINUX) || defined(MBSD)
        union {
            fpreal32 val;
            __m128 vec;
        };
        val = x;
        vec = _mm_rsqrt_ss(vec);
        return val;
#else
        return 1.0f/SYSsqrt(x);
#endif
    }

    /// Computes 1/sqrt(x) to about 22.2 bits of accuracy
    static inline fpreal32 SYSrsqrt22(fpreal32 x)
    {
#if defined(WIN32) || defined(LINUX) || defined(MBSD)
        fpreal32 approximate = SYSrsqrt11(x);
        // -(((1+e)/sqrt(x))^2 * x - 3) * ((1+e)/sqrt(x)) / 2
        // -(-1 + e + (e^2)/2) * ((1+e)/sqrt(x))
        // -(-1 + e + (e^2)/2 - e + e^2 + (e^3)/2) / sqrt(x)
        // (1 - 3(e^2)/2 - (e^3)/2) / sqrt(x);
        fpreal32 result = (approximate*approximate*x - 3)*approximate*-0.5f;
#else
        fpreal32 result = 1.0f/SYSsqrt(x);
#endif
        return result;
    }
#endif

    static inline fpreal32 SYSlog(fpreal32 v)	{ return v <= 0 ? 0 : SYS_MF(logf)(v); }
    static inline fpreal64 SYSlog(fpreal64 v)	{ return v <= 0 ? 0 : SYS_MF(log)(v); }
    static inline fpreal32 SYSlog10(fpreal32 v)	{ return v <= 0 ? 0 : SYS_MF(log10f)(v); }
    static inline fpreal64 SYSlog10(fpreal64 v)	{ return v <= 0 ? 0 : SYS_MF(log10)(v); }

#if defined(WIN32)
    static inline fpreal32 SYSexpm1(fpreal32 x) { return SYSexp(x) - 1; }
    static inline fpreal64 SYSexpm1(fpreal64 x) { return SYSexp(x) - 1; }
    static inline fpreal32 SYSlog1p(fpreal32 x) { return SYSlog(x+1); }
    static inline fpreal64 SYSlog1p(fpreal64 x) { return SYSlog(x+1); }
#else
    SYS_UNARY(expm1)
    SYS_UNARY(log1p)
#endif

#undef SYS_UNARY
#undef SYS_BINARY
#undef hypotf

static inline fpreal32 SYSabs(fpreal32 a)	{ return SYS_MF(fabsf)(a); }
static inline fpreal64 SYSabs(fpreal64 a)	{ return SYS_MF(fabs)(a); }
static inline fpreal32 SYSfabs(fpreal32 a)	{ return SYS_MF(fabsf)(a); }
static inline fpreal64 SYSfabs(fpreal64 a)	{ return SYS_MF(fabs)(a); }

#include "SYS_Floor.h"

static inline fpreal32 SYSasin(fpreal32 a)
    { return SYS_MF(asinf)(SYSclamp(a, (fpreal32)-1, (fpreal32)1)); }
static inline fpreal32 SYSacos(fpreal32 a)
    { return SYS_MF(acosf)(SYSclamp(a, (fpreal32)-1, (fpreal32)1)); }
static inline fpreal32 SYSatan(fpreal32 a)
    { return SYS_MF(atanf)(a); }
static inline fpreal32 SYSatan(fpreal32 y, fpreal32 x)
    { return SYS_MF(atan2f)(y, x); }
static inline fpreal64 SYSasin(fpreal64 a)
    { return SYS_MF(asin) (SYSclamp(a, (fpreal64)-1, (fpreal64)1)); }
static inline fpreal64 SYSacos(fpreal64 a)
    { return SYS_MF(acos) (SYSclamp(a, (fpreal64)-1, (fpreal64)1)); }
static inline fpreal64 SYSatan(fpreal64 a)
    { return SYS_MF(atan) (a); }
static inline fpreal64 SYSatan(fpreal64 y, fpreal64 x)
    { return SYS_MF(atan2)(y, x); }

static inline fpreal32 SYSsafepow(fpreal32 x, fpreal32 y)
    { return SYSpow(x, x < 0 ? SYSrint(y) : y); }
static inline fpreal64 SYSsafepow(fpreal64 x, fpreal64 y)
    { return SYSpow(x, x < 0 ? SYSrint(y) : y); }

static inline fpreal32 SYSdegToRad(fpreal32 a) { return a*(fpreal32)(M_PI/180.0); }
static inline fpreal64 SYSdegToRad(fpreal64 a) { return a*(fpreal64)(M_PI/180.0); }
static inline fpreal64 SYSdegToRad(int32 a) { return a*(fpreal64)(M_PI/180.0); }
static inline fpreal64 SYSdegToRad(int64 a) { return a*(fpreal64)(M_PI/180.0); }

static inline fpreal32 SYSradToDeg(fpreal32 a) { return a*(fpreal32)(180.0/M_PI); }
static inline fpreal64 SYSradToDeg(fpreal64 a) { return a*(fpreal64)(180.0/M_PI); }
static inline fpreal64 SYSradToDeg(int32 a) { return a*(fpreal32)(180.0/M_PI); }
static inline fpreal64 SYSradToDeg(int64 a) { return a*(fpreal64)(180.0/M_PI); }

#define h_compare(func, code)	\
	static inline bool func(fpreal32 a, fpreal32 b, \
				fpreal32 tol=SYS_FTOLERANCE) \
	    { return code; } \
	static inline bool func(fpreal64 a, fpreal64 b, \
				fpreal64 tol=SYS_FTOLERANCE_D) \
	    { return code; }

    static inline bool
    SYSequalZero(fpreal32 a, fpreal32 tol=SYS_FTOLERANCE)
	{ return a >= -tol && a <= tol; }

    static inline bool
    SYSequalZero(fpreal64 a, fpreal64 tol=SYS_FTOLERANCE_D)
	{ return a >= -tol && a <= tol; }

    static inline bool
    SYSequalZero(int32 a, fpreal64 tol=SYS_FTOLERANCE_D)
	{ return a >= -tol && a <= tol; }

    static inline bool
    SYSequalZero(int64 a, fpreal64 tol=SYS_FTOLERANCE_D)
	{ return a >= -tol && a <= tol; }


    h_compare(SYSisEqual, SYSabs(a-b)<=tol)
    h_compare(SYSisGreater, (a-b) > tol)
    h_compare(SYSisGreaterOrEqual, (a-b) >= -tol)
    h_compare(SYSisLess, (a-b) < -tol)
    h_compare(SYSisLessOrEqual, (a-b) <= tol)

inline bool SYSisEqual(int32 a, int32 b) { return a == b; }
inline bool SYSisEqual(int64 a, int64 b) { return a == b; }
inline bool SYSisGreater(int32 a, int32 b) { return a > b; }
inline bool SYSisGreater(int64 a, int64 b) { return a > b; }
inline bool SYSisGreaterOrEqual(int32 a, int32 b) { return a >= b; }
inline bool SYSisGreaterOrEqual(int64 a, int64 b) { return a >= b; }
inline bool SYSisLess(int32 a, int32 b) { return a < b; }
inline bool SYSisLess(int64 a, int64 b) { return a < b; }
inline bool SYSisLessOrEqual(int32 a, int32 b) { return a <= b; }
inline bool SYSisLessOrEqual(int64 a, int64 b) { return a <= b; }

#undef h_compare

static inline bool
SYSalmostEqual(fpreal32 a, fpreal32 b, int ulps=50)
{
    SYS_FPRealUnionF	ai, bi;
    ai.fval = a;
    bi.fval = b;
    if (ai.ival < 0)
	ai.ival = 0x80000000 - ai.ival;
    if (bi.ival < 0)
	bi.ival = 0x80000000 - bi.ival;
    // We're very careful to avoid signed integer overflow here.
    if ((ai.uval > bi.uval ? ai.uval - bi.uval : bi.uval - ai.uval) <= ulps)
	return true;
    // If both single-precision floats are very close to zero, we consider them
    // equal.
    return SYSabs(a) < 1e-6 && SYSabs(b) < 1e-6;
}

static inline bool
SYSalmostEqual(fpreal64 a, fpreal64 b, int64 ulps)
{
    SYS_FPRealUnionD	ai, bi;
    ai.fval = a;
    bi.fval = b;
    if (ai.ival < 0)
	ai.ival = CONST_INT64(0x8000000000000000) - ai.ival;
    if (bi.ival < 0)
	bi.ival = CONST_INT64(0x8000000000000000) - bi.ival;
    // We're very careful to avoid signed integer overflow here.
    if ((ai.uval > bi.uval ? ai.uval - bi.uval : bi.uval - ai.uval) <= ulps)
	return true;
    // If both double-precision floats are very close to zero, we consider them
    // equal.
    return SYSabs(a) < 1e-15 && SYSabs(b) < 1e-15;
}

static inline bool
SYSalmostEqual(fpreal64 a, fpreal64 b, int32 ulps=50)
{
    return SYSalmostEqual(a, b, (int64)ulps);
}

#define h_max3(type)	\
	    static inline type \
	    SYSmax(type v0, type v1, type v2) { \
	        return SYSmax(v2, SYSmax(v0, v1)); \
	    }
#define h_max4(type)	\
	    static inline type \
	    SYSmax(type v0, type v1, type v2, type v3) { \
		return SYSmax(SYSmax(v0, v1), SYSmax(v2, v3)); \
	    }
#define h_argmax3(type)	\
	    static inline int \
	    SYSargmax(type v0, type v1, type v2) { \
		return v2 > SYSmax(v0, v1) ? 2 : SYSargmax(v0, v1); \
	    }
#define h_argmax4(type)	\
	    static inline int \
	    SYSargmax(type v0, type v1, type v2, type v3) { \
		return SYSmax(v0, v1) < SYSmax(v2, v3) ? \
		    (SYSargmax(v2, v3) + 2) : SYSargmax(v0, v1); \
	    }
#define h_min3(type)	\
	    static inline type \
	    SYSmin(type v0, type v1, type v2) { \
	        return SYSmin(v2, SYSmin(v0, v1)); \
	    }
#define h_min4(type)	\
	    static inline type \
	    SYSmin(type v0, type v1, type v2, type v3) { \
		return SYSmin(SYSmin(v0, v1), SYSmin(v2, v3)); \
	    }
#define h_argmin3(type) \
	    static inline int \
	    SYSargmin(type v0, type v1, type v2) { \
		return v2 < SYSmin(v0, v1) ? 2 : SYSargmin(v0, v1); \
	    }
#define h_argmin4(type)	\
	    static inline int \
	    SYSargmin(type v0, type v1, type v2, type v3) { \
		return SYSmin(v0, v1) > SYSmin(v2, v3) ? \
		    (SYSargmin(v2, v3) + 2) : SYSargmin(v0, v1); \
	    }


#define h_max(type)	h_min3(type) h_min4(type) h_max3(type) h_max4(type) \
			h_argmin3(type) h_argmin4(type)  \
			h_argmax3(type) h_argmax4(type)

#define h_avg3(type)	\
	    static inline type \
	    SYSavg(type v0, type v1, type v2) { \
	        return (v0+v1+v2) * ((type)(1.0/3.0)); \
	    }
#define h_avg4(type)	\
	    static inline type \
	    SYSavg(type v0, type v1, type v2, type v3) { \
		return (v0+v1+v2+v3) * ((type)0.25); \
	    }

#define h_avg(type)	h_avg3(type) h_avg4(type)

h_max(int8)
h_max(uint8)
h_max(int16)
h_max(uint16)
h_max(int32)
h_max(uint32)
h_max(int64)
h_max(uint64)
h_max(fpreal32) h_avg(fpreal32)
h_max(fpreal64) h_avg(fpreal64)

static inline int32
SYSavg(int32 a, int32 b, int32 c)
{ return (a + b + c + 1) / 3; }
static inline int32
SYSavg(int32 a, int32 b, int32 c, int32 d)
{ return (a + b + c + d + 2) / 4; }

static inline int64
SYSavg(int64 a, int64 b, int64 c)
{ return (a + b + c + 1) / 3; }
static inline int64
SYSavg(int64 a, int64 b, int64 c, int64 d)
{ return (a + b + c + d + 2) / 4; }

// Some systems have size_t as a seperate type from uint.  Some don't.
#if (defined(LINUX) && defined(IA64)) || defined(MBSD)
h_max(size_t)
#endif

#undef h_min3
#undef h_min4
#undef h_max3
#undef h_max4
#undef h_argmin3
#undef h_argmin4
#undef h_argmax3
#undef h_argmax4
#undef h_avg3
#undef h_avg4
#undef h_max
#undef h_avg

/// @{
/// Linear interpolation between v0 and v1.
static inline fpreal32
SYSlerp(fpreal32 v1, fpreal32 v2, fpreal32 t)
{
    return v1 + (v2 - v1)*t;
}

static inline fpreal64
SYSlerp(fpreal64 v1, fpreal64 v2, fpreal64 t)
{
    return v1 + (v2 - v1)*t;
}
/// @}

/// @{
/// Bilinear interpolation over a quadrilateral: @code
///  (u=0,v=1) u0v1     u1v1 (u=1,v=1)
///              +--------+
///              |        |
///              |        |
///              +--------+
///  (u=0,v=0) u0v0     u1v0 (u=1,v=0)
/// @endcode
static inline fpreal32
SYSbilerp(fpreal32 u0v0, fpreal32 u1v0, fpreal32 u0v1, fpreal32 u1v1,
			fpreal32 u, fpreal32 v)
{
    return SYSlerp(SYSlerp(u0v0, u0v1, v), SYSlerp(u1v0, u1v1, v), u);
}

static inline fpreal64
SYSbilerp(fpreal64 u0v0, fpreal64 u1v0, fpreal64 u0v1, fpreal64 u1v1,
			fpreal64 u, fpreal64 v)
{
    return SYSlerp(SYSlerp(u0v0, u0v1, v), SYSlerp(u1v0, u1v1, v), u);
}
/// @}

/// @{
/// Barycentric coordinates do interpolation over the triangle specified by the
/// three vertices (v0, v1, v2): @code
/// |              (u=0,v=1)               |
/// |                  v2                  |
/// |                 / \                  |
/// |                /   \                 |
/// |               /     \                |
/// |    (u=0,v=0) v0------v1 (u=1,v=0)    |
/// @endcode
static inline fpreal32
SYSbarycentric(fpreal32 v0, fpreal32 v1, fpreal32 v2, fpreal32 u, fpreal32 v)
{
    return v0*(1-u-v) + v1*u + v2*v;
}

static inline fpreal64
SYSbarycentric(fpreal64 v0, fpreal64 v1, fpreal64 v2, fpreal64 u, fpreal64 v)
{
    return v0*(1-u-v) + v1*u + v2*v;
}
/// @}

/// @{
/// Wrap mod is like mod, but returns [0..b) interval thereby
/// usually giving a more useful result.
/// This is always defined as safe!
/// @}
static inline fpreal32
SYSwrapmod(fpreal32 a, fpreal32 b)
{
    fpreal32 r = SYSsafefmod(a, b);
    return ((a < 0) ^ (b < 0) && r) ? r+b : r;
}

static inline fpreal64
SYSwrapmod(fpreal64 a, fpreal64 b)
{
    fpreal64 r = SYSsafefmod(a, b);
    return ((a < 0) ^ (b < 0) && r) ? r+b : r;
}

static inline int32
SYSwrapmod(int32 a, int32 b)
{
    if (b == 0)
	return 0;
    int32 r = a % b;
    return ((a < 0) ^ (b < 0) && r) ? r+b : r;
}

static inline int64
SYSwrapmod(int64 a, int64 b)
{
    if (b == 0)
	return 0;
    int64 r = a % b;
    return ((a < 0) ^ (b < 0) && r) ? r+b : r;
}

static inline fpreal32
SYSsmooth(fpreal32 min, fpreal32 max, fpreal32 val)
{
    fpreal32	t;
    if (val <= min) return 0;
    if (val >= max) return 1;
    t = max - min;
    if (SYSequalZero(t, (fpreal32)1e-8)) return (fpreal32).5;
    t = (val - min) / t;
    return t*t*((fpreal32)3.0 - (fpreal32)2.0*t);
}

static inline fpreal64
SYSsmooth(fpreal64 min, fpreal64 max, fpreal64 val)
{
    fpreal64	t;
    if (val <= min) return 0;
    if (val >= max) return 1;
    t = max - min;
    if (SYSequalZero(t, (fpreal64)1e-18)) return (fpreal64).5;
    t = (val - min) / t;
    return t*t*((fpreal64)3.0 - (fpreal64)2.0*t);
}

static inline fpreal32
SYSsmooth(fpreal32 min, fpreal32 max, fpreal32 value, fpreal32 roll)
{
    fpreal32	f;
    if (roll > 0)
    {
	f = SYSsmooth(min, max, value);
	return roll < (fpreal32)1 ? (fpreal32)1-SYSpow((fpreal32)1-f,
				    (fpreal32)1/roll) : SYSpow(f, roll);
    }
    return (fpreal32)1;
}

static inline fpreal64
SYSsmooth(fpreal64 min, fpreal64 max, fpreal64 value, fpreal64 roll)
{
    fpreal64	f;
    if (roll > 0)
    {
	f = SYSsmooth(min, max, value);
	return roll < (fpreal64)1 ? (fpreal64)1-SYSpow((fpreal64)1-f,
				    (fpreal64)1/roll) : SYSpow(f, roll);
    }
    return (fpreal64)1;
}

static inline fpreal32
SYSfit(fpreal32 val, fpreal32 omin, fpreal32 omax, fpreal32 nmin, fpreal32 nmax)
{
    fpreal32	d = omax - omin;
    if (SYSequalZero(d, (fpreal32)1e-8))
	return (nmin+nmax)*(fpreal32).5;
    if (omin < omax)
    {
	if (val < omin) return nmin;
	if (val > omax) return nmax;
    }
    else
    {
	if (val < omax) return nmax;
	if (val > omin) return nmin;
    }
    return nmin + (nmax-nmin)*(val-omin)/d;
}

static inline fpreal64
SYSfit(fpreal64 val, fpreal64 omin, fpreal64 omax, fpreal64 nmin, fpreal64 nmax)
{
    fpreal64	d = omax - omin;
    if (SYSequalZero(d, (fpreal64)1e-18))
	return (nmin+nmax)*(fpreal64).5;
    if (omin < omax)
    {
	if (val < omin) return nmin;
	if (val > omax) return nmax;
    }
    else
    {
	if (val < omax) return nmax;
	if (val > omin) return nmin;
    }
    return nmin + (nmax-nmin)*(val-omin)/d;
}

/// SYSefit() will not clamp the values to the range
static inline fpreal32
SYSefit(fpreal32 v, fpreal32 omin, fpreal32 omax, fpreal32 nmin, fpreal32 nmax)
{
    fpreal32	d = omax - omin;
    if (SYSequalZero(d, (fpreal32)1e-8))
	return (nmin+nmax)*0.5f;
    return nmin + (nmax-nmin)*(v-omin)/d;
}

static inline fpreal64
SYSefit(fpreal64 v, fpreal64 omin, fpreal64 omax, fpreal64 nmin, fpreal64 nmax)
{
    fpreal64	d = omax - omin;
    if (SYSequalZero(d, (fpreal64)1e-18))
	return (nmin+nmax)*0.5f;
    return nmin + (nmax-nmin)*(v-omin)/d;
}

static inline fpreal32
SYSfit01(fpreal32 val, fpreal32 nmin, fpreal32 nmax)
{
    if (val < 0) return nmin;
    if (val > 1) return nmax;
    return SYSlerp(nmin, nmax, val);
}

static inline fpreal64
SYSfit01(fpreal64 val, fpreal64 nmin, fpreal64 nmax)
{
    if (val < 0) return nmin;
    if (val > 1) return nmax;
    return SYSlerp(nmin, nmax, val);
}

// Linear hat function over kernel width dx.
static inline fpreal32
SYShat(fpreal32 x, fpreal32 dx)
{
    const fpreal32 ax = SYSabs(x);
    return (ax > dx ? 0 : 1 - ax / dx);
}

static inline fpreal64
SYShat(fpreal64 x, fpreal64 dx)
{
    const fpreal64 ax = SYSabs(x);
    return (ax > dx ? 0 : 1 - ax / dx);
}

// Derivative of the linear hat function over kernel width dx.
static inline fpreal32
SYSdhat(fpreal32 x, fpreal32 dx)
{
    return (x < -dx || x > dx ? 0 : -SYSsgn(x) / dx);
}

static inline fpreal64
SYSdhat(fpreal64 x, fpreal64 dx)
{
    return (x < -dx || x > dx ? 0 : -SYSsgn(x) / dx);
}


// For integer types.
template<typename T>
inline T
SYSroundDownToMultipleOf(T val, T multiple)
{
    // Only handle multiples of 2 and up.
    if (multiple <= 1)
	return val;

    int rem = val % multiple;
    if (rem == 0)
	return val;

    if (val < 0)
	return val - multiple - rem;
    else
	return val - rem;
}

template<>
inline fpreal32
SYSroundDownToMultipleOf<fpreal32>(fpreal32 val, fpreal32 multiple)
{
    fpreal32	modulus;
    fpreal32	retval;
    if (SYSequalZero(multiple))
	return val;
    modulus = SYSfmod(val, multiple);
    if (SYSequalZero(modulus) || SYSequalZero(modulus-multiple))
	return val;
    retval = val - modulus;
    if( val < (fpreal32)0 && modulus!=(fpreal32)0 )
	retval -= multiple;
    return retval;
}

template<>
inline fpreal64
SYSroundDownToMultipleOf<fpreal64>(fpreal64 val, fpreal64 multiple)
{
    fpreal64	modulus;
    fpreal64	retval;
    if (SYSequalZero(multiple))
	return val;
    modulus = SYSfmod(val, multiple);
    if (SYSequalZero(modulus) || SYSequalZero(modulus-multiple))
	return val;
    retval = val - modulus;
    if( val < (fpreal64)0 && modulus!=(fpreal64)0 )
	retval -= multiple;
    return retval;
}

template<typename T>
inline T
SYSroundUpToMultipleOf(T val, T multiple)
{
    // Only handle multiples of 2 and up.
    if (multiple <= 1)
	return val;

    int rem = val % multiple;
    if (rem == 0)
	return val;

    if (val < 0)
	return val - rem;
    else
	return val + multiple - rem;
}

template<>
inline fpreal32
SYSroundUpToMultipleOf<fpreal32>(fpreal32 val, fpreal32 multiple)
{
    fpreal32	modulus;
    if (SYSequalZero(multiple))
	return val;
    modulus = SYSfmod(val, multiple);
    if (SYSequalZero(modulus) || SYSequalZero(modulus-multiple))
	return val;
    if (val > (fpreal32)0)
	val += multiple;
    return val - modulus;
}

template<>
inline fpreal64
SYSroundUpToMultipleOf<fpreal64>(fpreal64 val, fpreal64 multiple)
{
    fpreal64	modulus;
    if (SYSequalZero(multiple))
	return val;
    modulus = SYSfmod(val, multiple);
    if (SYSequalZero(modulus) || SYSequalZero(modulus-multiple))
	return val;
    if (val > (fpreal64)0)
	val += multiple;
    return val - modulus;
}

inline uint32
SYSroundUpPow2(uint32 val)
{
    // From http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    uint32 v = val;

    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}

static inline uint32
SYSwang_inthash(uint32 key)
{
    // From http://www.concentric.net/~Ttwang/tech/inthash.htm
    key += ~(key << 16);
    key ^=  (key >> 5);
    key +=  (key << 3);
    key ^=  (key >> 13);
    key += ~(key << 9);
    key ^=  (key >> 17);
    return key;
}

static inline uint32
SYSmultiplicative_inthash(uint32 key)
{
    // Multiply by the golden mean of 2^32 (Knuth's multiplicative method) to
    // get a uniformly distributed hash value.  Do a google search for
    // 2654435761 for more information.
    return key * 2654435761u;
}

static inline uint64
SYSwang_inthash64(uint64 key)
{
    // From http://www.concentric.net/~Ttwang/tech/inthash.htm
    key += ~(key << 32ULL);
    key ^= (key >> 22);
    key += ~(key << 13);
    key ^= (key >> 8);
    key += (key << 3);
    key ^= (key >> 15);
    key += ~(key << 27);
    key ^= (key >> 31);
    return key;
}

static inline uint
SYSreal_hash(fpreal16 a, int lowmask=0x3)
{
    return SYSwang_inthash(a.bits() & (~lowmask));
}

static inline uint
SYSreal_hashseed(fpreal16 a, uint seed, int lowmask=0x3)
{
    return SYSwang_inthash(seed + (a.bits() & (~lowmask)));
}

static inline uint
SYSreal_hash(fpreal32 a, int lowmask=0xf)
{
    SYS_FPRealUnionF	ai;
    ai.fval = a;
    return SYSwang_inthash(ai.uval & (~lowmask));
}

static inline uint
SYSreal_hashseed(fpreal32 a, uint seed, int lowmask=0xf)
{
    SYS_FPRealUnionF	ai;
    ai.fval = a;
    return SYSwang_inthash(seed + (ai.uval & (~lowmask)));
}

static inline uint
SYSreal_hash(fpreal64 a, int lowmask=0xf)
{
    SYS_FPRealUnionD	ai;
    ai.fval = a;
    return SYSwang_inthash64(ai.uval & (~lowmask));
}

static inline uint
SYSreal_hashseed(fpreal64 a, uint seed, int64 lowmask=0xf)
{
    SYS_FPRealUnionD	ai;
    ai.fval = a;
    return SYSwang_inthash64(seed + (ai.uval & (~lowmask)));
}

static inline uint
SYSvector_hash(const int32 *vector, int size)
{
    uint	hash = 0;
    for (int i = 0; i < size; ++i)
	hash = SYSwang_inthash(hash + vector[i]);
    return hash;
}

static inline uint
SYSvector_hash(const int64 *vector, int size)
{
    uint	hash = 0;
    for (int i = 0; i < size; ++i)
	hash = (uint)SYSwang_inthash64(hash + vector[i]);
    return hash;
}

static inline uint
SYSvector_hash(const fpreal16 *vector, int size)
{
    uint	hash = 0;
    for (int i = 0; i < size; ++i)
	hash = SYSreal_hashseed(vector[i], hash);
    return hash;
}

static inline uint
SYSvector_hash(const fpreal32 *vector, int size)
{
    uint	hash = 0;
    for (int i = 0; i < size; ++i)
	hash = SYSreal_hashseed(vector[i], hash);
    return hash;
}

static inline uint
SYSvector_hash(const fpreal64 *vector, int size)
{
    uint	hash = 0;
    for (int i = 0; i < size; ++i)
	hash = SYSreal_hashseed(vector[i], hash);
    return hash;
}

// Spatial hashing function for 3-vectors
// Reference:
//   "Optimized Spatial Hashing for Collision Detection of Deformable Objects"
//
template<class P>
static inline size_t
SYSvector3_hash(const P &vector)
{
    static const size_t p1 = 73856093;
    static const size_t p2 = 19349663;
    static const size_t p3 = 83492791;

    return size_t(vector.x()*p1) ^ size_t(vector.y()*p2) ^ size_t(vector.z()*p3);
}

static inline uint
SYSpointerHash(const void *ptr)
{
#if SIZEOF_VOID_P == 8
    // 64 bit pointers
    return (uint)((int64)ptr & 0xffffffff);
#else
    // 32 bit pointers
    return (uint)ptr;
#endif
}

// Convert a uniform random bit pattern to a random float in the range [0, 1)
inline static fpreal32
SYShashToFloat01(uint hash)
{
    SYS_FPRealUnionF	tmp;
    tmp.uval = 0x3f800000 | (0x007fffff & hash);
    return tmp.fval-1.0F;
}

// Generate a random number in range [0, 1)
inline static fpreal32
SYSfastRandom(uint &seed)
{
    seed = seed*1664525 + 1013904223;
    return SYShashToFloat01(seed);
}

inline static fpreal32
SYSrandom(uint &seed)
{
    seed = seed*1664525 + 1013904223;
    return SYShashToFloat01(SYSwang_inthash(seed));
}

inline static uint
SYSfastRandomInt(uint &seed)
{
    seed = seed*1664525 + 1013904223;
    return SYSwang_inthash(seed);
}

inline static fpreal32
SYSfastRandomZero(uint &seed)
{
    return SYSfastRandom(seed) - 0.5F;
}

inline static fpreal32
SYSrandomZero(uint &seed)
{
    return SYSrandom(seed) - 0.5F;
}

template <typename T>
inline static void
SYSminmax(T v0, T v1, T v2, T v3, T &min, T &max)
{
    min = SYSmin(v0, v1, v2, v3);
    max = SYSmax(v0, v1, v2, v3);
}

template <typename T>
inline static void
SYSminmax(T v0, T v1, T v2, T &min, T &max)
{
    min = SYSmin(v0, v1, v2);
    max = SYSmax(v0, v1, v2);
}

template <typename T>
inline static void
SYSminmax(T v0, T v1, T &min, T &max)
{
    min = SYSmin(v0, v1);
    max = SYSmax(v0, v1);
}

inline static void
SYSgetSinCosFromSlope(fpreal32 slope, fpreal32 &sintheta, fpreal32 &costheta)
{
    fpreal32	one_over_m;
    sintheta = slope / SYSsqrt(slope*slope + (fpreal32)1);
    if ((slope = SYSabs(slope)) > (fpreal32)1)
    {
	one_over_m = (fpreal32)1 / slope;
	costheta = one_over_m / SYSsqrt(one_over_m*one_over_m + 1);
    }
    else
	costheta = SYSsqrt((fpreal32)1 - sintheta*sintheta);
}

inline static void
SYSgetSinCosFromSlope(fpreal64 slope, fpreal64 &sintheta, fpreal64 &costheta)
{
    fpreal64	one_over_m;
    sintheta = slope / SYSsqrt(slope*slope + (fpreal64)1);
    if ((slope = SYSabs(slope)) > (fpreal64)1)
    {
	one_over_m = (fpreal64)1 / slope;
	costheta = one_over_m / SYSsqrt(one_over_m*one_over_m + 1);
    }
    else
	costheta = SYSsqrt((fpreal64)1 - sintheta*sintheta);
}

inline static bool
SYSsameSign( fpreal32 v0, fpreal32 v1 )
{
    return (v0*v1)>0;
}

inline static bool
SYSsameSign( fpreal64 v0, fpreal64 v1 )
{
    return (v0*v1)>0;
}

inline static bool
SYSsameSign( int32 v0, int32 v1 )
{
    return (v0 ^ v1) >= 0;
}

inline static bool
SYSsameSign( int64 v0, int64 v1 )
{
    return (v0 ^ v1) >= 0;
}

static inline uint
SYSnextPrime(uint num)
{
    return SYSmakePrime(num+1);
}

static inline int
SYShexCharToInt(char c)
{
    // Given a hexidecimal character, return it's corresponding value between
    // 0 and 15, or -1 if it's not a hexidecimal character.
    if (c >= '0' && c <= '9')
	return c - '0';
    if (c >= 'a' && c <= 'f')
	return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
	return c - 'A' + 10;
    return -1;
}

static inline char
SYSintToHexChar(int value)
{
    // The value must be from 0-15 for this function to return a valid result.
    return value < 10 ? '0' + value : 'a' + value - 10;
}

SYS_API void SYSsort(int &a, int &b);
SYS_API void SYSsort(int &a, int &b, int &c);
SYS_API void SYSsort(int64 &a, int64 &b);
SYS_API void SYSsort(int64 &a, int64 &b, int64 &c);
SYS_API void SYSsort(float &a, float &b);
SYS_API void SYSsort(float &a, float &b, float &c);
SYS_API void SYSsort(double &a, double &b);
SYS_API void SYSsort(double &a, double &b, double &c);

// Compute both integer division and integer modulus
// They are compiled into a single instruction
static inline void
SYSdivMod(int numerator, int denominator, int &quotient, int &remainder)
{
    quotient  = numerator / denominator;
    remainder = numerator % denominator;
}

static inline void
SYSdivMod(int64 numerator, int64 denominator, int64 &quotient, int64 &remainder)
{
    quotient  = numerator / denominator;
    remainder = numerator % denominator;
}

// Include permutations of fpreal32/fpreal64
#include "SYS_MathPermute.h"
#include "SYS_MathRestrictive.h"

#else	/* cplusplus */
#define SYSmax(a,b)	((a) > (b) ? (a) : (b))
#define SYSmin(a,b)	((a) < (b) ? (a) : (b))
#define SYSabs(a)	((a) < 0 ? (a) : -(a))
#endif

#endif
