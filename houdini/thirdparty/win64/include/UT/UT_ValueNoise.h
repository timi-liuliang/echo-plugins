/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: UT_ValueNoise.h ( UT Library, C++ )
 *
 * COMMENTS: For a description of value noise, see Wikipedia.
 *           This is a renovation of the old noise routine from UT_Noise.h
 *
 */

#ifndef __UT_ValueNoise_h__
#define __UT_ValueNoise_h__

#include "UT_API.h"
#include "UT_NonCopyable.h"
#include <SYS/SYS_Types.h>
#include <VM/VM_SIMD.h>


enum class UT_ValueNoiseType
{
    FAST,	// Perlin based noise generator
    SPARSE,	// Slower but higher quality noise generator
    FAST_FIX,	// Perlin based noise which was fixed
    ALLIGATOR	// Slight modification on sparse
};

template <UT_ValueNoiseType type>
class ut_NoiseGenerator;

template <UT_ValueNoiseType type = UT_ValueNoiseType::FAST>
class UT_API UT_ValueNoise final : UT_NonCopyable
{
public:
    explicit UT_ValueNoise(unsigned const seed = 0);
    ~UT_ValueNoise();

    void setSeed(unsigned const seed);

    // Specialised for fpreal32, fpreal64, v4uf
    template <typename T> T
    turbulence(const T x[3],
               unsigned depth,
               T rough = T(0.5),
               const T atten = T(1.0)) const;
    template <typename T> void
    turbulence(T f[3],
               const T x[3],
               unsigned depth,
               T rough = T(0.5),
               const T atten = T(1.0)) const;

    //
    // Periodic version of the above
    //
    // Specialised for:
    //   <fpreal32, int32>
    //   <fpreal64, int64>
    //   <v4uf, v4uu>
    //
    template <typename T, typename I> T
    turbulenceP(const T x[3],
                const I p[3],
                unsigned depth,
                T rough = T(0.5),
                const T atten = T(1.0)) const;
    template <typename T, typename I> void
    turbulenceP(T f[3],
                const T x[3],
                const I p[3],
                unsigned depth,
                T rough = T(0.5),
                const T atten = T(1.0)) const;

private:
    unsigned mySeed;

    // Not storing this on the stack.
    // Hides some implementation details and avoids blowing up the stack.
    // 
    // This contains the details of the particular type of noise generator.
    // Still statically polymorphic.
    ut_NoiseGenerator<type>* const myGen;
};

UT_EXTERN_TEMPLATE(UT_ValueNoise<UT_ValueNoiseType::FAST>);
UT_EXTERN_TEMPLATE(UT_ValueNoise<UT_ValueNoiseType::SPARSE>);
UT_EXTERN_TEMPLATE(UT_ValueNoise<UT_ValueNoiseType::FAST_FIX>);
UT_EXTERN_TEMPLATE(UT_ValueNoise<UT_ValueNoiseType::ALLIGATOR>);

#endif // !__UT_ValueNoise_h__
