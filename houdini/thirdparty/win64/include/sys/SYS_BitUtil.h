/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        SYS_BitUtil.h (SYS Library, C++)
 *
 * COMMENTS:    Functions for fast bit manipulation.
 *
 *      SYSfirstBitSet          - Get the index of the first (lowest-order) bit
 *                                equal 1, else -1 if input is zero.
 *      SYSlastBitSet           - Get the index of the last (highest-order) bit
 *                                equal 1, else -1 if input is zero.
 *      SYScountBitsSet         - Counts the number of bits equal to 1
 *      SYSisPow2               - Is it a power of two?
 *      SYSmakePow2             - Round up to nearest power of two
 *      SYSceilLog2             - ceil(log2(n))
 */

#ifndef __SYS_BitUtil__
#define __SYS_BitUtil__

#include "SYS_Inline.h"
#include "SYS_Types.h"

#if defined(_MSC_VER)
    #include <intrin.h>
#endif

static SYS_FORCE_INLINE int SYSfirstBitSet(uint v)
{
#if defined(_MSC_VER)
    unsigned long result;
    if (_BitScanForward(&result, v))
        return int(result);
    return -1;
#elif defined(GCC3)
    int		c;
    asm(
	"bsf %1, %0\n"
	"cmove %2, %0\n"
	: "=r" (c) : "r" (v), "r" (-1));
    return c;
#else
    if (!v)
        return -1;
    int result = 0;
    if (v & 0x0000FFFF)
        v &= 0x0000FFFF;
    else
        result |= 0x10;

    if (v & 0x00FF00FF)
        v &= 0x00FF00FF;
    else
        result |= 0x08;

    if (v & 0x0F0F0F0F)
        v &= 0x0F0F0F0F;
    else
        result |= 0x04;

    if (v & 0x33333333)
        v &= 0x33333333;
    else
        result |= 0x02;

    if (v & 0x55555555)
        v &= 0x55555555;
    else
        result |= 0x01;

    return result;
#endif
}

static SYS_FORCE_INLINE int SYSfirstBitSet(uint64 v)
{
#if defined(_MSC_VER) && defined(AMD64)
    unsigned long result;
    if (_BitScanForward64(&result, v))
        return int(result);
    return -1;
#elif defined(GCC4) && defined(AMD64)
    uint64	c;
    asm(
	"bsf %1, %0\n"
	"cmove %2, %0\n"
	: "=r" (c) : "r" (v), "r" (uint64(-1)));
    return c;
#else
    // Unfortunately, 64-bit BSF isn't supported on 32-bit CPUs
    if (v & 0x00000000FFFFFFFFULL)
        return SYSfirstBitSet(uint(v));
    int result = SYSfirstBitSet(uint(v>>32));
    if (result >= 0)
        result += 32;
    return result;
#endif
}

static SYS_FORCE_INLINE int SYSfirstBitSet(int v)
{
    return SYSfirstBitSet(uint(v));
}

static SYS_FORCE_INLINE int SYSfirstBitSet(int64 v)
{
    return SYSfirstBitSet(uint64(v));
}

#if defined(MBSD)
static SYS_FORCE_INLINE int SYSfirstBitSet(size_t v)
{
    return SYSfirstBitSet(uint64(v));
}
#endif

static SYS_FORCE_INLINE int SYSlastBitSet(uint v)
{
#if defined(_MSC_VER)
    unsigned long result;
    if (_BitScanReverse(&result, v))
        return int(result);
    return -1;
#elif defined(GCC3)
    int		c;
    asm(
	"bsr %1, %0\n"
	"cmove %2, %0\n"
	: "=r" (c) : "r" (v), "r" (-1));
    return c;
#else
    if (!v)
        return -1;
    int result = 0;
    if (v & 0xFFFF0000)
    {
        v &= 0xFFFF0000;
        result |= 0x10;
    }
    if (v & 0xFF00FF00)
    {
        v &= 0xFF00FF00;
        result |= 0x08;
    }
    if (v & 0xF0F0F0F0)
    {
        v &= 0xF0F0F0F0;
        result |= 0x04;
    }
    if (v & 0xCCCCCCCC)
    {
        v &= 0xCCCCCCCC;
        result |= 0x02;
    }
    if (v & 0xAAAAAAAA)
    {
        v &= 0xAAAAAAAA;
        result |= 0x01;
    }
    return result;
#endif
}

static SYS_FORCE_INLINE int SYSlastBitSet(uint64 v)
{
#if defined(_MSC_VER) && defined(AMD64)
    unsigned long result;
    if (_BitScanReverse64(&result, v))
        return int(result);
    return -1;
#elif defined(GCC4) && defined(AMD64)
    uint64	c;
    asm(
	"bsr %1, %0\n"
	"cmove %2, %0\n"
	: "=r" (c) : "r" (v), "r" (uint64(-1)));
    return c;
#else
    // Unfortunately, 64-bit BSR isn't supported on 32-bit CPUs
    if (v & 0xFFFFFFFF00000000ULL)
        return SYSlastBitSet(uint(v>>32)) + 32;
    return SYSlastBitSet(uint(v));
#endif
}

static SYS_FORCE_INLINE int SYSlastBitSet(int v)
{
    return SYSlastBitSet(uint(v));
}

static SYS_FORCE_INLINE int SYSlastBitSet(int64 v)
{
    return SYSlastBitSet(uint64(v));
}

/// For a given bit index, finds the next bit above it that is set and returns
/// its index, or -1 if no bits are set. Use SYSfirstBitSet to get a seed bit,
/// if iterating over bits from lowest to highest set.
template<typename T>
static SYS_FORCE_INLINE int SYSnextBitSet(T v, int bit)
{
    bit++;
    if (bit <= 0 || bit >= std::numeric_limits<T>::digits)
	return -1;
    
    int nbit = SYSfirstBitSet(v >> bit);
    if (nbit == -1) 
	return -1;
    return nbit + bit;
}

#if defined(MBSD)
static SYS_FORCE_INLINE int SYSlastBitSet(size_t v)
{
    return SYSlastBitSet(uint64(v));
}
#endif

// Count number of bits set without using a lookup table.
// NOTE: There are intrinsics on recent Intel/AMD CPUs to count bits
// in hardware (e.g. __builtin_popcount on gcc and __popcnt in
// MSVC). However, they're dependent on certain CPU features
// being set, so we won't use them here. The code below pipelines
// pretty well anyway.
static SYS_FORCE_INLINE int SYScountBitsSet(uint v)
{
    v = (v&0x55555555U) + ((v>>1 )&0x55555555U);
    v = (v&0x33333333U) + ((v>>2 )&0x33333333U);
    v = (v&0x0F0F0F0FU) + ((v>>4 )&0x0F0F0F0FU);
    // The additions won't overflow 8 bits, so no more masks
    // are needed until the end
    v += (v>>8 );
    v += (v>>16);
    return ((int)v)&0xFF;
}

static SYS_FORCE_INLINE int SYScountBitsSet(uint64 v)
{
    v = (v&0x5555555555555555ULL) + ((v>>1 )&0x5555555555555555ULL);
    v = (v&0x3333333333333333ULL) + ((v>>2 )&0x3333333333333333ULL);
    v = (v&0x0F0F0F0F0F0F0F0FULL) + ((v>>4 )&0x0F0F0F0F0F0F0F0FULL);
    // The additions won't overflow 8 bits, so no more masks
    // are needed until the end
    v += (v>>8 );
    v += (v>>16);
    v += (v>>32);
    return ((int)v)&0xFF;
}

/// NOTE: This does *not* consider zero to be a power of two
template <typename T>
static SYS_FORCE_INLINE bool SYSisPow2(T v)
{
    return v>0 && !(v&(v-1));
}

/// Round up to the nearest power of 2 greater (or equal) to v
template <typename T>
static SYS_FORCE_INLINE T SYSmakePow2(T v)
{
    if (v <= 0)
        return 1;
    if (SYSisPow2(v))
        return v;
    T v2 = (T(1) << (SYSlastBitSet(v) + 1));
    return v2;
}

template <typename T>
static SYS_FORCE_INLINE int SYSceilLog2(T v)
{
    if (v <= 0)
        return 0;
    bool isPow2 = SYSisPow2(v);
    return SYSlastBitSet(v) + !isPow2;
}

// A macro to compute floor(log2(x)) of an 8-bit number at compile time. Useful for computing
// bit masks.
#define SYS_LOG2F(x)	((((x)>=(1<< 1))?1:0) + (((x)>=(1<< 2))?1:0) + (((x)>=(1<< 3))?1:0) + (((x)>=(1<< 4))?1:0) + \
			 (((x)>=(1<< 5))?1:0) + (((x)>=(1<< 6))?1:0) + (((x)>=(1<< 7))?1:0) + (((x)>=(1<< 8))?1:0))

static SYS_FORCE_INLINE int SYSfloorLog2(float v)
{
    if (v <= 0)
        return 0x80000000;
    SYS_FPRealUnionF fori;
    fori.fval = v;
    int e = (fori.uval >> SYS_FPRealUnionF::MANTISSA_BITS) - 127;
    // NOTE: Not quite right for denormal numbers or infinities.
    return e;
}
static SYS_FORCE_INLINE int SYSfloorLog2(double v)
{
    if (v <= 0)
        return 0x80000000;
    SYS_FPRealUnionD fori;
    fori.fval = v;
    int e = (fori.uval >> SYS_FPRealUnionD::MANTISSA_BITS) - 1023;
    // NOTE: Not quite right for denormal numbers or infinities.
    return e;
}
static SYS_FORCE_INLINE int SYSroundLog2(float v)
{
    if (v <= 0)
        return 0x80000000;
    SYS_FPRealUnionF fori;
    fori.fval = v;
    int e = (fori.uval >> SYS_FPRealUnionF::MANTISSA_BITS) - 127;
    // NOTE: Not quite right for denormal numbers or infinities.
    // Magic number 3474676 is ceil((2^0.5 - 1)*(2^23))
    if ((fori.uval & ((1<<SYS_FPRealUnionF::MANTISSA_BITS)-1)) >= 3474676)
        ++e;
    return e;
}
static SYS_FORCE_INLINE int SYSroundLog2(double v)
{
    if (v <= 0)
        return 0x80000000;
    SYS_FPRealUnionD fori;
    fori.fval = v;
    int e = (fori.uval >> SYS_FPRealUnionD::MANTISSA_BITS) - 1023;
    // NOTE: Not quite right for denormal numbers or infinities.
    // Magic number 1865452045155277 is ceil((2^0.5 - 1)*(2^52))
    if ((fori.uval & ((1ULL<<SYS_FPRealUnionD::MANTISSA_BITS)-1)) >= 1865452045155277ULL)
        ++e;
    return e;
}
static SYS_FORCE_INLINE int SYSceilLog2(float v)
{
    if (v <= 0)
        return 0x80000000;
    SYS_FPRealUnionF fori;
    fori.fval = v;
    int e = (fori.uval >> SYS_FPRealUnionF::MANTISSA_BITS) - 127;
    // NOTE: Not quite right for denormal numbers or infinities.
    if ((fori.uval & ((1<<SYS_FPRealUnionF::MANTISSA_BITS)-1)) != 0)
        ++e;
    return e;
}
static SYS_FORCE_INLINE int SYSceilLog2(double v)
{
    if (v <= 0)
        return 0x80000000;
    SYS_FPRealUnionD fori;
    fori.fval = v;
    int e = (fori.uval >> SYS_FPRealUnionD::MANTISSA_BITS) - 1023;
    // NOTE: Not quite right for denormal numbers or infinities.
    if ((fori.uval & ((1ULL<<SYS_FPRealUnionD::MANTISSA_BITS)-1)) != 0)
        ++e;
    return e;
}

static SYS_FORCE_INLINE float SYSintExp2AsFloat(int e)
{
    // Avoid include for SYSclamp by doing it manually
    if (e < -127)
        e = -127;
    else if (e > 128)
        e = 128;
    SYS_FPRealUnionF fori;
    fori.uval = (127+e)<<SYS_FPRealUnionF::MANTISSA_BITS;
    return fori.fval;
}

static SYS_FORCE_INLINE double SYSintExp2AsDouble(int e)
{
    // Avoid include for SYSclamp by doing it manually
    if (e < -1023)
        e = -1023;
    else if (e > 1024)
        e = 1024;
    SYS_FPRealUnionD fori;
    fori.uval = int64(1023+e)<<SYS_FPRealUnionD::MANTISSA_BITS;
    return fori.fval;
}

#endif
