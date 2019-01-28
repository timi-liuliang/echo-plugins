/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Floor (C++)
 *
 * COMMENTS:	Fast floating point floor function (faster than floorf)
 *		There are two versions, a fast inline and a slower (but
 *		still faster than floorf) out-lined function.
 *
 */

#ifndef __SYS_Floor_H__
#define __SYS_Floor_H__

#include "SYS_API.h"
#include <math.h>
#include "SYS_Types.h"

SYS_API extern fpreal32	SYSfloor(fpreal32 val);
SYS_API extern fpreal32	SYSceil(fpreal32 val);
SYS_API extern fpreal32	SYSfrac(fpreal32 val);
SYS_API extern fpreal64	SYSfrac(fpreal64 val);

/// @{
/// SYSniceNumber() will limit the precision of decimal digits, but leave the
/// integer portion of a number at full precision.  Thus: @code
///	SYSniceNumber(1234.56, 2) == 1235;	// (note the rounding)
///	SYSniceNumber(0.0123456, 2) == 0.012;
///	SYSniceNumber(1.23456e-14,2) == 1.2e-14;
/// @endcode
/// Note that 6 digits is almost enough to represent a single precision float.
SYS_API extern fpreal64	SYSniceNumber(fpreal64 num, int digits = 6);
static  inline fpreal32 SYSniceNumber(fpreal32 num, int digits = 6)
		       { return (fpreal32)SYSniceNumber((fpreal64)num, digits);}
/// @}

static inline fpreal64 SYSfloor(fpreal64 a)	{ return floor(a); }
static inline fpreal64 SYSceil(fpreal64 a)	{ return ceil(a); }

/// Floor function that is incorrect for negative integers
/// @{
static inline int32
SYSfastFloor(fpreal32 a) { return a < 0 ? (int32)a - 1 : (int32)a; }
static inline int64
SYSfastFloor(fpreal64 a) { return a < 0 ? (int64)a - 1 : (int64)a; }
/// @}

/// Ceiling function that is incorrect for positive integers
/// @{
static inline int32
SYSfastCeil(fpreal32 a) { return a <= 0 ? (int32)a : (int32)a + 1; }
static inline int64
SYSfastCeil(fpreal64 a) { return a <= 0 ? (int64)a : (int64)a + 1; }
/// @}

inline fpreal32
SYSfloorIL(fpreal32 val)
{
    SYS_FPRealUnionF	tmp;
    unsigned int	shift;

    tmp.fval = val;
    shift = (tmp.uval >> 23) & 0xff;

    if (shift < 0x7f)
    {
	tmp.fval = (tmp.uval > 0x80000000) ? -1.0F : 0.0F;
    }
    else if (shift < 0x96)
    {
	unsigned mask = 0xffffffff << (0x96 - shift);
	if (tmp.uval & 0x80000000)
	{
	    if ((tmp.uval & ~mask) & 0x7fffff)
	    {
		tmp.uval &= mask;
		tmp.fval--;
	    }
	}
	else tmp.uval &= mask;
    }
    return tmp.fval;
}

inline fpreal64 SYSfloorIL(fpreal64 val) { return SYSfloor(val); }
inline fpreal32 SYSceilIL(fpreal32 val)	{ return SYSceil(val); }
inline fpreal64 SYSceilIL(fpreal64 val) { return SYSceil(val); }

template <typename T, typename S>
static inline void
SYSsplitFloatT(T &val, S &ival)
{
    val += 1;		// Deal with numbers between (-1, 1)
    T fval = SYSfloorIL(val);
    ival = ((S)fval) - 1;
    val  = val - fval;
}

//
// The following function splits a floating point value into its fractional and
// integer components.  The integer returned is the floor() of the fractional
// value, while the fractional component is guaranteed to always be in the
// range [0, 1).  This is different from (val - floor(val)) which can cause
// problems if the value is very close to -0.
//
static inline void SYSsplitFloat(fpreal32 &val, int32 &ival)
    { SYSsplitFloatT(val, ival); }
static inline void SYSsplitFloat(fpreal32 &val, int64 &ival)
    { SYSsplitFloatT(val, ival); }
static inline void SYSsplitFloat(fpreal32 &val, fpreal32 &ival)
    { SYSsplitFloatT(val, ival); }
static inline void SYSsplitFloat(fpreal32 &val, fpreal64 &ival)
    { SYSsplitFloatT(val, ival); }
static inline void SYSsplitFloat(fpreal64 &val, int32 &ival)
    { SYSsplitFloatT(val, ival); }
static inline void SYSsplitFloat(fpreal64 &val, int64 &ival)
    { SYSsplitFloatT(val, ival); }
static inline void SYSsplitFloat(fpreal64 &val, fpreal32 &ival)
    { SYSsplitFloatT(val, ival); }
static inline void SYSsplitFloat(fpreal64 &val, fpreal64 &ival)
    { SYSsplitFloatT(val, ival); }

// fastSplitFloat() is not as correct as the above code.  Using the fast
// version, you can get somewhat unexpected results.  However, in many cases,
// it's quite acceptable to get these results.
//   a) Denormalization of -ve values close to zero (i.e. -1e-10) will result
//      in the fractional value being 1.0 exactly and the integer value = -1
template <typename REAL_TYPE, typename INT_TYPE>
static inline void
SYSfastSplitFloat(REAL_TYPE &val, INT_TYPE &ival)
{
    ival = SYSfastFloor(val);
    val -= REAL_TYPE(ival);
}

inline fpreal32
SYSfracIL(fpreal32 val)
{
    // On Intel chips, val may be a double precision register value.  In this
    // case, a small value less than zero will cause the fractional result to
    // be outside the range [0-1), which is obviously invalid.  Thus, we have
    // to have special code to handle this case properly.  On non-intel chips,
    // which support floating point operations, we can simply de-normalize the
    // value and return the fractional component properly.
#if defined(WIN32) || defined(LINUX)
    val = val - SYSfloorIL(val);
    return (val < 0 || val >= 1) ? 0 : val;
#else
    val += 1;		// Normalize fpreal32 first
    return val - SYSfloorIL(val);
#endif
}

inline fpreal64
SYSfracIL(fpreal64 val)
{
    return SYSfrac(val);
}

inline fpreal32
SYSrint(fpreal32 val)
{
    return SYSfloorIL(val + 0.5F);
}

inline fpreal64
SYSrint(fpreal64 val)
{
    return SYSfloor(val + 0.5);
}


#endif
