/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Noise generator for utility library (C++)
 *
 * COMMENTS:
 *	Two types of noise are currently supported.  Perlin & Sparse
 *	convolution.  This class basically produces space coherent noise
 *	(meaning that points which are close in space will generate
 *	similar noise values).
 *
 *	To control the noise, you can change the frequency & offset
 *	by scaling space before sending the values to noise...
 *
 */

#ifndef __UT_Noise_h__
#define __UT_Noise_h__

#include "UT_API.h"
#include "UT_NonCopyable.h"
#include "UT_VectorTypes.h"
#include <SYS/SYS_Types.h>


class UT_API UT_Noise : UT_NonCopyable
{
public:
//
//  Following is an enumeration of the types of noise supported:
    enum UT_NoiseType
    {
	FAST,		// Perlin based noise generator
	SPARSE,		// Slower but higher quality noise generator
	FAST_FIX,	// Perlin based noise which was fixed
	ALLIGATOR	// Slight modification on sparse
    };

    explicit		UT_Noise(unsigned seed = 0, UT_NoiseType type = FAST);
			~UT_Noise();

//
//  Initialization methods
    void		setType(UT_NoiseType type = FAST);
    void		setSeed(unsigned seed = 0);
    void		initialize(unsigned seed, UT_NoiseType type)
			{
			    if (myType == type)
			    {
				setSeed(seed);
			    }
			    else
			    {
				mySeed = seed;
				setType(type); // will initialize with mySeed
			    }
			}

//
// Query methods: Allow us to tell if we need to rebuild.
    UT_NoiseType	getType() const { return myType; }
    int			getSeed() const { return mySeed; }

//
//  Methods for getting noise out
    fpreal		turbulence(
			    const UT_Vector3F &pos,
			    unsigned fractalDepth,
			    fpreal rough = 0.5,
			    fpreal atten = 1.0) const;
    void		turbulence(
			    const UT_Vector3 &pos,
			    unsigned fractalDepth,
			    UT_Vector3 &noise,
			    fpreal rough = 0.5,
			    fpreal atten = 1.0) const;
    void		turbulence(
			    const UT_Vector3D &pos,
			    unsigned fractalDepth,
			    UT_Vector3D &noise,
			    fpreal rough = 0.5,
			    fpreal atten = 1.0) const;

    const void *	getTextureData(
			    int tex_index,
			    int &xsize,
			    int &ysize,
			    int &comps,
			    bool &fp) const;

// Exposed only for internal use to avoid additional nesting
    class ut_NoiseVector;

private:
    unsigned int	mySeed;
    UT_NoiseType	myType;
    ut_NoiseVector *	myNoise;
};

#endif
