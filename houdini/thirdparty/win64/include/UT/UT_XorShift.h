/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_XorShift.h (UT Library, C++)
 *
 * COMMENTS:    Implementation of an XorShift128+ pseudorandom number generator,
 *              faster and better quality than Mersenne Twister, but
 *              with a shorter period of only pow(2,128)-1, or over 10 trillion
 *              years at 1 billion numbers per nanosecond.  Alternatively,
 *              with 10 billion differently-seeded generators each generating 1
 *              trillion numbers, there is a 0.00003% chance of having two of
 *              the 10 billion sequences overlap somewhere.
 */

#ifndef __UT_XorShift__
#define __UT_XorShift__

#include <SYS/SYS_BitUtil.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>

class UT_XorShift
{
public:
    // NOTE: Default constructor leaves state uninitialized, to avoid cost
    //       of extra reseed if it will just be reseeded before its first use
    //       anyway.
    SYS_FORCE_INLINE UT_XorShift() {}
    SYS_FORCE_INLINE UT_XorShift(uint64 seed0, uint64 seed1 = 0)
    {
        reseed(seed0, seed1);
    }
    SYS_FORCE_INLINE UT_XorShift(uint32 seed0, uint32 seed1 = 0, uint32 seed2 = 0, uint32 seed3 = 0)
    {
        reseed(seed0, seed1, seed2, seed3);
    }

    /// This is useful for after copy constructing from another UT_XorShift.
    /// It will give a new random number sequence, consistently dependent
    /// on (though uncorrelated with) the original, without affecting the
    /// original.
    SYS_FORCE_INLINE void reseedSelf()
    {
        reseed(state[0], state[1]);
    }
    void reseed(uint64 seed0, uint64 seed1 = 0)
    {
        // NOTE: Yes, the seeds must be mangled here, else there's a very
        //       long burn-in period as the dependence on each bit cycles
        //       around.  It is critical that either state[0] or state[3]
        //       depends on all seeds.  It does *not* suffice to just
        //       individually mangle the seeds and do a few get() calls.
        uint64 seed01 = SYSwang_inthash64(seed0);
        state[0] = seed01;
        seed01 ^= seed1;
        seed01 = SYSwang_inthash64(seed01);
        state[1] = seed01;
        // NOTE: The state must be seeded so that it is not everywhere zero.
        if (seed01 == 0 && state[0] == 0)
        {
            // Arbitrary random bits
            state[0] = 0xdd8d9063f8c6a49aULL;
            state[1] = 0x5b3f183183528aaaULL;
        }
    }
    SYS_FORCE_INLINE void reseed(uint32 seed0, uint32 seed1 = 0, uint32 seed2 = 0, uint32 seed3 = 0)
    {
        reseed(uint64(seed0) | (uint64(seed1)<<32), uint64(seed2) | (uint64(seed3)<<32));
    }
    SYS_FORCE_INLINE uint64 get64()
    {
        // xorshift128+ algorithm:
        // fastest generator passing BigCrush without systematic errors
        uint64 s1 = state[0];
        const uint64 s0 = state[1];
        state[0] = s0;
        s1 ^= (s1 << 23);
        s1 = (s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26));
        state[1] = s1;
        return s1 + s0;
    }
    SYS_FORCE_INLINE uint32 get()
    {
        return uint32(get64());
#if 0
        // xorshift128
        uint32 temp = state[0] ^ (state[0] << 11);
        state[0] = state[1];
        state[1] = state[2];
        state[2] = state[3];
        state[3] = state[3] ^ (state[3] >> 19) ^ temp ^ (temp >> 8);
        return state[3];
#endif
    }
    SYS_FORCE_INLINE float getFloat()
    {
        return intToFloat01(get());
    }
    SYS_FORCE_INLINE double getDouble()
    {
        return int64ToDouble01(get64());
    }

    static SYS_FORCE_INLINE fpreal32 intToFloat01(uint32 i)
    {
        if (i == 0)
            return 0.0f;
        int e = SYSlastBitSet(i);
        i <<= (31-e);
        i = (((127-32)+e)<<SYS_FPRealUnionF::MANTISSA_BITS) | ((i>>SYS_FPRealUnionF::EXPONENT_BITS)&((1<<SYS_FPRealUnionF::MANTISSA_BITS)-1));
        return *(fpreal32*)&i;
    }
    static SYS_FORCE_INLINE fpreal64 int64ToDouble01(uint64 i)
    {
        if (i == 0)
            return 0.0;
        int e = SYSlastBitSet(i);
        i <<= (63-e);
        i = (uint64((1023-64)+e)<<SYS_FPRealUnionD::MANTISSA_BITS) | ((i>>SYS_FPRealUnionD::EXPONENT_BITS)&((uint64(1)<<SYS_FPRealUnionD::MANTISSA_BITS)-1));
        return *(fpreal64*)&i;
    }
#if 0
    // Interface for compatibility with UT_MersenneTwister

    uint32 urandom() { return get(); }
    float frandom() { return getFloat(); }
    void setSeed(const uint32 *seeds, exint nseeds)
    {
        uint32 combinedseeds[4] = {0, 0, 0, 0};
        exint m = SYSmin(nseeds, 4);
        for (exint i = 0; i < m; ++i)
            combinedseeds[i] = seeds[i];
        for (exint i = 4; i < nseeds; ++i)
            combinedseeds[i & 0x3] = uint32(SYSwang_inthash64((uint64(seeds[i])<<32) | uint64(combinedseeds[i & 0x3])));
        reseed(combinedseeds[0], combinedseeds[1], combinedseeds[2], combinedseeds[3]);
    }
#endif

private:
    uint64 state[2];
};

class UT_XorShiftCached : public UT_XorShift
{
public:
    // NOTE: Default constructor leaves state uninitialized, to avoid cost
    //       of extra reseed if it will just be reseeded before its first use
    //       anyway.
    SYS_FORCE_INLINE UT_XorShiftCached() {}
    SYS_FORCE_INLINE UT_XorShiftCached(uint64 seed0, uint64 seed1 = 0)
    {
        reseed(seed0, seed1);
    }
    SYS_FORCE_INLINE UT_XorShiftCached(uint32 seed0, uint32 seed1 = 0, uint32 seed2 = 0, uint32 seed3 = 0)
    {
        reseed(seed0, seed1, seed2, seed3);
    }
    SYS_FORCE_INLINE void reseed(uint32 seed0, uint32 seed1 = 0, uint32 seed2 = 0, uint32 seed3 = 0)
    {
        reseed(uint64(seed0) | (uint64(seed1)<<32), uint64(seed2) | (uint64(seed3)<<32));
    }
    void reseed(uint64 seed0, uint64 seed1 = 0)
    {
        UT_XorShift::reseed(seed0, seed1);
        myHasCached = false;
    }
    SYS_FORCE_INLINE uint32 get()
    {
        if (myHasCached)
        {
            myHasCached = false;
            return myCached;
        }
        uint64 result = get64();
        myCached = uint32(result >> 32);
        myHasCached = true;
        return uint32(result);
    }
    SYS_FORCE_INLINE float getFloat()
    {
        return intToFloat01(get());
    }
private:
    uint32 myCached;
    bool myHasCached;
};

#endif
