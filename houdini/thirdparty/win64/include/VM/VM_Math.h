/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VM_Math.h ( VM Library, C++)
 *
 * COMMENTS:	Vector Math.
 */

#ifndef __VM_Math__
#define __VM_Math__

#include "VM_API.h"
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Floor.h>
#include <string.h>
#include "VM_SIMD.h"

class VM_SPUMath;

#define VM_SIV	static inline void

// On platforms that don't support vector instructions, we define empty
// function signatures for SIMD operations.  The runtime check on theSIMD
// should prevent them from being called.
#if defined(CPU_HAS_SIMD_INSTR)
#define VM_SIMDFUNC(signature) signature;
#define VM_SIMDFUNCR(signature) signature;
#else
#define VM_SIMDFUNC(signature) signature {}
#define VM_SIMDFUNCR(signature) signature { return 0; }
#endif

#define VM_DECL_vFvFvF(name, expr)	\
    VM_SIV	name(fpreal32 *d, const fpreal32 *a, \
				const fpreal32 *b, exint num) { \
	if (theSIMD)	name##SIMD(d, a, b, num); \
	else		for (exint n=0; n<num; n++) expr; \
    } \
    VM_SIV	name(fpreal64 *d, const fpreal64 *a, \
				const fpreal64 *b, exint num) { \
	for (exint n=0; n<num; n++) expr; \
    } \
    VM_SIMDFUNC(static void	name##SIMD(fpreal32 *d, const fpreal32 *a, const fpreal32 *b, exint num))

#define VM_DECL_vFvFuF(name, expr)	\
    VM_SIV	name(fpreal32 *d, const fpreal32 *a, \
				fpreal32 b, exint num) { \
	if (theSIMD)	name##SIMD(d, a, b, num); \
	else		for (exint n=0; n<num; n++) expr; \
    } \
    VM_SIV	name(fpreal64 *d, const fpreal64 *a, \
				fpreal64 b, exint num) { \
	for (exint n=0; n<num; n++) expr; \
    } \
    VM_SIMDFUNC(static void	name##SIMD(fpreal32 *d, const fpreal32 *a, fpreal32 b, exint num))

#define VM_DECL_vFuFvF(name, expr)	\
    VM_SIV	name(fpreal32 *d, fpreal32 a, \
				const fpreal32 *b, exint num) { \
	if (theSIMD)	name##SIMD(d, a, b, num); \
	else		for (exint n=0; n<num; n++) expr; \
    } \
    VM_SIV	name(fpreal64 *d, fpreal64 a, const fpreal64 *b, exint num) { \
	for (exint n=0; n<num; n++) expr; \
    } \
    VM_SIMDFUNC(static void	name##SIMD(fpreal32 *d, fpreal32 a, const fpreal32 *b, exint num))

#define VM_DECL_vFuFuF(name, expr)	\
    VM_SIV	name(fpreal32 *d, fpreal32 a, \
				fpreal32 b, exint num) { \
	if (theSIMD)	name##SIMD(d, a, b, num); \
	else		for (exint n=0; n<num; n++) expr; \
    } \
    VM_SIV	name(fpreal64 *d, fpreal64 a, \
				fpreal64 b, exint num) { \
	for (exint n=0; n<num; n++) expr; \
    } \
    VM_SIMDFUNC(static void	name##SIMD(fpreal32 *d, fpreal32 a, fpreal32 b, exint num))

#define VM_DECL_vFvF(name, expr)	\
    VM_SIV	name(fpreal32 *d, const fpreal32 *a, exint num) { \
	if (theSIMD)	name##SIMD(d, a, num); \
	else		for (exint n=0; n<num; n++) expr; \
    } \
    VM_SIV	name(fpreal64 *d, const fpreal64 *a, exint num) { \
	for (exint n=0; n<num; n++) expr; \
    } \
    VM_SIMDFUNC(static void	name##SIMD(fpreal32 *d, const fpreal32 *a, exint num))

#define VM_DECL_vFuF(name, expr)	\
    VM_SIV	name(fpreal32 *d, fpreal32 a, exint num) { \
	if (theSIMD)	name##SIMD(d, a, num); \
	else		for (exint n=0; n<num; n++) expr; \
    } \
    VM_SIMDFUNC(static void	name##SIMD(fpreal32 *d, fpreal32 a, exint num))

#define VM_DECL_vIvF(name, expr)	\
    VM_SIV	name(int32 *d, const fpreal32 *a, exint num) { \
	if (theSIMD)	name##SIMD(d, a, num); \
	else		for (exint n=0; n<num; n++) expr; \
    } \
    VM_SIMDFUNC(static void	name##SIMD(int32 *d, const fpreal32 *a, exint num))

#define VM_DECL_vIvF_nonconst(name, expr)	\
    VM_SIV	name(int32 *d, fpreal32 *a, exint num) { \
	if (theSIMD)	name##SIMD(d, a, num); \
	else		for (exint n=0; n<num; n++) expr; \
    } \
    VM_SIMDFUNC(static void	name##SIMD(int32 *d, fpreal32 *a, exint num))

#define VM_DECL_vIvFvF(name, expr)	\
    VM_SIV	name(int32 *d, const fpreal32 *a, const fpreal32 *b, exint num) { \
		    if (theSIMD) name##SIMD(d, a, b, num); \
		    else	for (exint n=0; n<num; n++) expr; \
		} \
    VM_SIV	name(int32 *d, const fpreal64 *a, const fpreal64 *b, exint num) { \
		    for (exint n=0; n<num; n++) expr; \
		} \
    VM_SIMDFUNC(static void	name##SIMD(int32 *d, const fpreal32 *a, const fpreal32 *b, exint num))

#define VM_DECL_vIvVFvF(name, expr)	\
    VM_SIV	name(int32 *d, fpreal32 *a, const fpreal32 *b, exint num) { \
		    if (theSIMD) name##SIMD(d, a, b, num); \
		    else	for (exint n=0; n<num; n++) expr; \
		} \
    VM_SIMDFUNC(static void	name##SIMD(int32 *d, fpreal32 *a, const fpreal32 *b, exint num))

#define VM_DECL_vIvFuF(name, expr)	\
    VM_SIV	name(int32 *d, const fpreal32 *a, fpreal32 b, exint num) { \
		    if (theSIMD) name##SIMD(d, a, b, num); \
		    else	for (exint n=0; n<num; n++) expr; \
		} \
    VM_SIV	name(int32 *d, const fpreal64 *a, fpreal64 b, exint num) { \
		    for (exint n=0; n<num; n++) expr; \
		} \
    VM_SIMDFUNC(static void	name##SIMD(int32 *d, const fpreal32 *a, fpreal32 b, exint num))

#define VM_DECL_WP(type, name, expr)	\
    VM_SIV	name(type *d, const fpreal32 *a, fpreal32 b, \
				     fpreal32 c, fpreal32 e, exint num) { \
		    if (theSIMD) name##SIMD(d, a, b, c, e, num); \
		    else	for (exint n=0; n<num; n++) expr; \
		} \
    VM_SIMDFUNC(static void	name##SIMD(type *d, const fpreal32 *a, fpreal32 b, fpreal32 c, fpreal32 e, exint num))

#define VM_DECL_IWP(type, name, expr)	\
    VM_SIV	name(fpreal32 *d, const type *a, fpreal32 b, \
				     type e, exint num) { \
		    if (theSIMD) name##SIMD(d, a, b, e, num); \
		    else	for (exint n=0; n<num; n++) expr; \
		} \
    VM_SIMDFUNC(static void	name##SIMD(fpreal32 *d, const type *a, fpreal32 b, type e, exint num))

#define VM_DECL_vIvIvI(name, expr)	\
    VM_SIV	name(int32 *d, const int32 *a, const int32 *b, exint num) { \
		    if (theSIMD) name##SIMD(d, a, b, num); \
		    else	for (exint n=0; n<num; n++) expr; \
		} \
    VM_SIMDFUNC(static void	name##SIMD(int32 *d, const int32 *a, const int32 *b, exint num))

#define VM_DECL_vIvIuI(name, expr)	\
    VM_SIV	name(int32 *d, const int32 *a, int32 b, exint num) { \
		    if (theSIMD) name##SIMD(d, a, b, num); \
		    else	for (exint n=0; n<num; n++) expr; \
		} \
    VM_SIMDFUNC(static void	name##SIMD(int32 *d, const int32 *a, int32 b, exint num))

#define VM_DECL_uIvI(name, expr)	\
    static inline int32 name##SISD(const int32 *a, exint num) { \
			    int32	d = 0; \
			    for (exint n=0; n < num; n++) expr; \
			    return d; \
			} \
    VM_SIMDFUNCR(static int32	name##SIMD(const int32 *a, exint num)) \
    static inline int32 name(const int32 *a, exint num) { \
			    return theSIMD ? name##SIMD(a, num) : \
					     name##SISD(a, num); \
			} \

// Declare the 8 variations of a single comparison operator
// - vector and scalar b
// - fast and standard
// - int32 and fpreal32
#define VM_DECL_CMP(name, op) \
    VM_DECL_vIvFvF(name, d[n] = a[n] op b[n]) \
    VM_DECL_vIvFuF(name, d[n] = a[n] op b) \
    VM_DECL_vIvFvF(fast##name, d[n] = a[n] op b[n]) \
    VM_DECL_vIvFuF(fast##name, d[n] = a[n] op b) \
    VM_DECL_vIvIvI(name, d[n] = a[n] op b[n]) \
    VM_DECL_vIvIuI(name, d[n] = a[n] op b) \
    VM_DECL_vIvIvI(fast##name, d[n] = a[n] op b[n]) \
    VM_DECL_vIvIuI(fast##name, d[n] = a[n] op b)

template <class T>
static inline void
wpoint(T &d, fpreal32 v, fpreal32 wpoint, fpreal32 max, fpreal32 offset)
{
    d = (T)SYSclamp(v*wpoint + offset, 0.0F, max);
}

template <class T>
static inline void
iwpoint(fpreal32 &d, T v, fpreal32 iwpoint, T offset)
{
    d = (fpreal32)(v - offset) * iwpoint;
}

class VM_API VM_Math {
public:
    static bool		isSIMD()	{ return theSIMD; }

    /// The fast operations assume that any non-zero return value is
    /// interpreted as true.  Standard operations always set the result to
    /// either 0 or 1.
    ///
    /// VM_Math::lt(d, a, b)  :=  d[i] = a[i] < b[i]
    /// VM_Math::lt(d, a, b)  :=  d[i] = a[i] < b
    /// VM_Math::fastlt(d, a, b)  :=  d[i] = a[i] < b[i]
    /// VM_Math::fastlt(d, a, b)  :=  d[i] = a[i] < b
    VM_DECL_CMP(lt, <)
    VM_DECL_CMP(le, <=)
    VM_DECL_CMP(gt, >)
    VM_DECL_CMP(ge, >=)
    VM_DECL_CMP(eq, ==)
    VM_DECL_CMP(ne, !=)

    /// Bitwise operations
    VM_DECL_vIvIvI(bor, d[n] = a[n] | b[n])
    VM_DECL_vIvIuI(bor, d[n] = a[n] | b)
    VM_DECL_vIvIvI(band, d[n] = a[n] & b[n])
    VM_DECL_vIvIuI(band, d[n] = a[n] & b)
    VM_DECL_vIvIvI(bxor, d[n] = a[n] ^ b[n])
    VM_DECL_vIvIuI(bxor, d[n] = a[n] ^ b)

    /// VM_Math::add(d, a, b) :=  d[i] = a[i] + b[i]
    VM_DECL_vFvFvF(add, d[n] = a[n] + b[n])
    VM_DECL_vFvFuF(add, d[n] = a[n] + b)
    VM_DECL_vIvIvI(add, d[n] = a[n] + b[n])
    VM_DECL_vIvIuI(add, d[n] = a[n] + b)

    /// VM_Math::sub(d, a, b) :=  d[i] = a[i] - b[i]
    VM_DECL_vFvFvF(sub, d[n] = a[n] - b[n])
    VM_DECL_vFvFuF(sub, d[n] = a[n] - b)
    VM_DECL_vFuFvF(sub, d[n] = a - b[n])

    /// VM_Math::mul(d, a, b) :=  d[i] = a[i] * b[i]
    VM_DECL_vFvFvF(mul, d[n] = a[n] * b[n])
    VM_DECL_vFvFuF(mul, d[n] = a[n] * b)

    /// VM_Math::div(d, a, b) :=  d[i] = a[i] / b[i]
    VM_DECL_vFvFvF(div, d[n] = a[n] / b[n])
    VM_DECL_vFuFvF(div, d[n] = a / b[n])

    /// VM_Math::safediv(d, a, b) :=  d[i] = b[i] != 0 ? a[i] / b[i] : a[i]
    VM_DECL_vFvFvF(safediv, d[n] = SYSsafediv(a[n], b[n]))
    VM_DECL_vFuFvF(safediv, d[n] = SYSsafediv(a, b[n]))

#if 0 // Turn this off to use reciprocal multiplication
    VM_DECL_vFvFuF(div, d[n] = a[n] / b);
    VM_SIV safediv(fpreal32 *d, const fpreal32 *a, fpreal32 b, exint num)
		    {
			if (b == 0) set(d, 0.f, num);
			else div(d, a, b, num);
    		    }
#else
    // Multiply by reciprocal rather than dividing by a constant
    VM_SIV	div(fpreal32 *d, const fpreal32 *a, fpreal32 b, exint num)
		    { mul(d, a, 1/b, num); }
    VM_SIV	safediv(fpreal32 *d, const fpreal32 *a, fpreal32 b, exint num)
		    {
			if (b == 0) set(d, 0.0F, num);
			else mul(d, a, 1/b, num);
		    }
#endif

    /// VM_Math::fdiv(d, a, b) :=  d[i] = a[i] * (1.0 / b[i])
    /// A faster version than div(), but less accurate since it uses the
    /// reciprocal.
    VM_DECL_vFvFvF(fdiv, d[n] = a[n] / b[n])
    VM_DECL_vFuFvF(fdiv, d[n] = a / b[n])
    VM_SIV	fdiv(fpreal32 *d, const fpreal32 *a, fpreal32 b, exint num)
		    { b = 1/b; mul(d, a, b, num); }

    /// VM_Math::max(d, a, b) :=  d[i] = SYSmax(a[i] * b[i])
    VM_DECL_vFvFvF(vmmax, d[n] = SYSmax(a[n], b[n]) )
    VM_DECL_vFvFuF(vmmax, d[n] = SYSmax(a[n], b) )

    /// VM_Math::min(d, a, b) :=  d[i] = SYSmin(a[i] * b[i])
    VM_DECL_vFvFvF(vmmin, d[n] = SYSmin(a[n], b[n]) )
    VM_DECL_vFvFuF(vmmin, d[n] = SYSmin(a[n], b) )

    /// VM_Math::clamp(d, a, b) := d[i] = SYSclamp(a[i], min, max)
    VM_SIV	clamp(fpreal32 *d, const fpreal32 *a,
				   fpreal32 min, fpreal32 max, exint num)
		{
		    if (theSIMD) clampSIMD(d, a, min, max, num);
		    else	 for (exint n=0; n<num; n++) d[n] = SYSclamp(a[n], min, max);
		}
    VM_SIMDFUNC(static void	clampSIMD(fpreal32 *d, const fpreal32 *a, fpreal32 min, fpreal32 max, exint num))

    /// VM_Math::dot(a,b,n) :=  return sum(a[i]*b[i], i=0,n)
    static inline fpreal64 dot(const fpreal32 *a, const fpreal32 *b, exint n)
		{ return (theSIMD) ? dotSIMD(a, b, n) : dotSISD(a, b, n); }
    static inline double dotSISD(const fpreal32 *a, const fpreal32 *b, exint n)
		{
		    exint		i;
		    double	sum = 0;
		    for (i = 0; i < n; i++)
			sum += a[i]*b[i];
		    return sum;
		}
    VM_SIMDFUNCR(static double	dotSIMD(const fpreal32 *a, const fpreal32 *b, exint n))

    static inline double maddAndNorm(fpreal32 *d, const fpreal32 *a, fpreal s, exint n)
	    { return (theSIMD) ? maddAndNormSIMD(d, a, s, n) : maddAndNormSISD(d, a, s, n); }
    static inline double maddAndNormSISD(fpreal32 *d, const fpreal32 *a, fpreal s, exint n)
	    {
		exint		i;
		double		sum = 0;
		for (i = 0; i < n; i++)
		{
		    d[i] += a[i] * s;
		    sum += d[i] * d[i];
		}
		return sum;
	    }
    VM_SIMDFUNCR(static double	maddAndNormSIMD(fpreal32 *d, const fpreal32 *a, fpreal32 s, exint n))

    static inline double mulAndDotDA(fpreal32 *d, const fpreal32 *a, const fpreal32 *b, exint n)
	    { return (theSIMD) ? mulAndDotDASIMD(d, a, b, n) : mulAndDotDASISD(d, a, b, n); }
    static inline double mulAndDotDASISD(fpreal32 *d, const fpreal32 *a, const fpreal32 *b, exint n)
	    {
		exint		i;
		double		sum = 0;
		for (i = 0; i < n; i++)
		{
		    d[i] = a[i] * b[i];
		    sum += d[i] * a[i];
		}
		return sum;
	    }
    VM_SIMDFUNCR(static double	mulAndDotDASIMD(fpreal32 *d, const fpreal32 *a, const fpreal32 *s, exint n))

    /// VM_Math::zerocount(a,n) := return sum(a[i]==0, i=0,n)
    VM_DECL_uIvI(zerocount, d += a[n] == 0)

    /// VM_Math::scaleoffset(d, a, b) :=  d[i] = d[i]*a[i] + b[i]
    VM_DECL_vFvFvF(scaleoffset, d[n] = d[n]*a[n] + b[n])
    VM_DECL_vFvFuF(scaleoffset, d[n] = d[n]*a[n] + b)
    VM_DECL_vFuFvF(scaleoffset, d[n] = d[n]*a + b[n])
    VM_DECL_vFuFuF(scaleoffset, d[n] = d[n]*a + b)

    /// VM_Math::madd(d, a, b) :=  d[i] = d[i] + a[i]*b[i]
    VM_DECL_vFvFvF(madd, d[n] += a[n]*b[n])
    VM_DECL_vFvFuF(madd, d[n] += a[n]*b)

    /// VM_Math::sqrt(d, a) := d[i] = sqrt(a[i]);
    VM_DECL_vFvF(sqrt,	d[n] = SYSsqrt(a[n]) )

    /// VM_Math::fsqrt(d, a) := d[i] = 1.0 / isqrt(a[i]);
    /// This is a faster, but far less accurate version of sqrt() since it uses
    /// the reciprocal sqrt().
    VM_DECL_vFvF(fsqrt, d[n] = SYSsqrt(a[n]) )

    /// VM_Math::isqrt(d, a) := d[i] = 1.0 / sqrt(a[i])
    VM_DECL_vFvF(isqrt,	d[n] = 1/SYSsqrt(a[n]) )

    /// VM_Math::floor(a) := SYSfloorIL(a)
    VM_DECL_vIvF(floor, d[n] = (int)SYSfloorIL(a[n]) )

    VM_DECL_vIvF_nonconst(splitFloat, SYSfastSplitFloat(a[n], d[n]) )
    VM_DECL_vIvVFvF(splitFloat, { a[n] = b[n]; SYSfastSplitFloat(a[n], d[n]); } )

    /// VM_Math::cast(a) := (int)a
    VM_DECL_vIvF(cast, d[n] = (int)a[n] )

    /// VM_Math::negate(d, a) := d[i] = -a[i]
    VM_DECL_vFvF(negate,	d[n] = -a[n] )

    /// VM_Math::invert(d, a) := d[i] = 1.0 / a[i]
    VM_DECL_vFvF(invert,	d[n] = 1.0 / a[n] )

    /// VM_Math::abs(d, a) := d[i] = abs(a[i])
    VM_DECL_vFvF(abs,	d[n] = SYSabs(a[n]) )

    /// VM_Math::wpoint(d,a,b,c,e) := d[i] = SYSclamp(a[i]*b+e+0.5F, 0, c)
    VM_DECL_WP(fpreal32, wpoint, ::wpoint<fpreal32>(d[n], a[n], b, c, e+0.5F));
    VM_DECL_WP(uint8, wpoint, ::wpoint<uint8>(d[n], a[n], b, c, e+0.5F));
    VM_DECL_WP(uint16, wpoint, ::wpoint<uint16>(d[n], a[n], b, c, e+0.5F));

    /// VM_Math::iwpoint(d,a,b,e) := d[i] = (fpreal32)(a[i]-e)/b;
    VM_DECL_IWP(fpreal32, iwpoint, ::iwpoint<fpreal32>(d[n], a[n], 1.0F/b, e));
    VM_DECL_IWP(uint8, iwpoint, ::iwpoint<uint8>(d[n], a[n], 1.0F/b, e));
    VM_DECL_IWP(uint16, iwpoint, ::iwpoint<uint16>(d[n], a[n], 1.0F/b, e));

    VM_DECL_vFuF(set, d[n] = a )
    /// VM_Math::set(d, a) := d[i] = a
    VM_SIV	set(int32 *d, int a, exint num)
		    { for (exint n=0; n<num; n++) d[n] = a; }

    /// VM_Math::set(d, a, disabled) := d[i] = disabled[i] ? d[i] : a[i]
    VM_SIV	set(fpreal32 *d, const fpreal32 *a, exint num,
		    const uint32 *disabled)
		{ set((int32 *)d, (const int32 *)a, num, disabled); }
    VM_SIV	set(int32 *d, const int32 *a, exint num,
		    const uint32 *disabled)
		{
		    if (theSIMD)
			setSIMD(d, a, num, disabled);
		    else
		    {
			exint		i;
			for (i = 0; i < num; i++)
			    d[i] = disabled[i] ? d[i] : a[i];
		    }
		}
    VM_SIMDFUNC(static void	setSIMD(int32 *d, const int32 *a, exint num, const uint32 *disabled))
    VM_SIV	set(fpreal32 *d, fpreal32 a, exint num,
		    const uint32 *disabled)
		{
		    SYS_FPRealUnionF	fu;
		    fu.fval = a;
		    set((int32 *)d, fu.ival, num, disabled);
		}
    VM_SIV	set(int32 *d, int32 a, exint num,
		    const uint32 *disabled)
		{
		    if (theSIMD)
			setSIMD(d, a, num, disabled);
		    else
		    {
			exint		i;
			for (i = 0; i < num; i++)
			    d[i] = disabled[i] ? d[i] : a;
		    }
		}
    VM_SIMDFUNC(static void	setSIMD(int32 *d, int32 a, exint num,
		    const uint32 *disabled))


    VM_SIV	swap(fpreal32 *a, fpreal32 *b, exint num)
		{
		    if (theSIMD)	swapSIMD(a, b, num);
		    else		swapSISD<fpreal32>(a, b, num);
		}
    VM_SIV	swap(fpreal64 *a, fpreal64 *b, exint num)
		{
		    swapSISD<fpreal64>(a, b, num);
		}
    template <typename T> 
    VM_SIV	swapSISD(T *a, T *b, exint num)
		{
		    for (exint i = 0; i < num; i++)
		    {
			T t = a[i];
			a[i] = b[i];
			b[i] = t;
		    }
		}
    VM_SIMDFUNC(static void	swapSIMD(fpreal32 *a, fpreal32 *b, exint num))


    /// VM_Math::lerp(d, a, b, t) := d[i] = a[i] + (b[i]-a[i])*t[i]
    VM_SIV	lerp(fpreal32 *d, const fpreal32 *a, const fpreal32 *b,
				    const fpreal32 *t, exint num)
		{
		    if (theSIMD)
		    {
			subSIMD(d, b, a, num);
			scaleoffsetSIMD(d, t, a, num);
		    }
		    else
		    {
			for (exint n=0; n<num; n++)
			    d[n] = a[n] + (b[n]-a[n])*t[n];
		    }
		}
    VM_SIV	lerp(fpreal32 *d, const fpreal32 *a, const fpreal32 *b,
				    fpreal32 t, exint num)
		{
		    if (theSIMD)
		    {
			mulSIMD (d, a, 1-t, num);
			maddSIMD(d, b, t,   num);
		    }
		    else
		    {
			for (exint n=0; n<num; n++)
			    d[n] = SYSlerp(a[n], b[n], t);
		    }
		}

    /// Vector Functions.
    /// The following methods assume that the values coming in are "vectors".
    /// The mnemonics are:
    ///		3 - Vector3 or Matrix3
    ///		4 - Vector4 or Matrix4
    /// The enable_flags are an array corresponding 1-1 to the vectors to be
    /// processed.  Unlike typical flags, the vector will be processed
    /// if the flag is set to 0.  This is to match the VEX style calling.
    /// If the VEX processor mask flag changes, the mask type here should
    /// change too.
    static void	mulRowVec44(fpreal32 *v4, const fpreal32 m1[4][4], exint nv,
				const uint32 *enable_flags=0);
    static void	mulRowVec34(fpreal32 *v3, const fpreal32 m1[4][4], exint nv,
				const uint32 *enable_flags=0);
    static void	mulColVec44(fpreal32 *v4, const fpreal32 m1[4][4], exint nv,
				const uint32 *enable_flags=0);
    static void	mulColVec34(fpreal32 *v3, const fpreal32 m1[4][4], exint nv,
				const uint32 *enable_flags=0);
    /// Multiplication, but treating the matrix as a 3x3 (i.e. no translate)
    static void	mulRowVec44_3(fpreal32 *v4, const fpreal32 m1[4][4], exint nv,
				const uint32 *enable_flags=0);
    static void	mulRowVec34_3(fpreal32 *v3, const fpreal32 m1[4][4], exint nv,
				const uint32 *enable_flags=0);
    static void	mulColVec44_3(fpreal32 *v4, const fpreal32 m1[4][4], exint nv,
				const uint32 *enable_flags=0);
    static void	mulColVec34_3(fpreal32 *v3, const fpreal32 m1[4][4], exint nv,
				const uint32 *enable_flags=0);

    // Add a 4-touple to an array of floats assumed to be a 4-tuple
    static void	vadd4u4(fpreal32 *v4, const fpreal32 a[4], exint nv,
				const uint32 *enable_flags=0);
    static void	vsub4u4(fpreal32 *v4, const fpreal32 a[4], exint nv,
				const uint32 *enable_flags=0);
    static void	vmul4u4(fpreal32 *v4, const fpreal32 a[4], exint nv,
				const uint32 *enable_flags=0);

    static void	forceSIMD(bool onoff)		{ theSIMD = onoff; }
    static void	setSPUMath(VM_SPUMath *math)	{ theSPUMath = math; }
    static VM_SPUMath *getSPUMath()		{ return theSPUMath; }

private:
    static bool		 theSIMD;
    static VM_SPUMath	*theSPUMath;
};

#endif
