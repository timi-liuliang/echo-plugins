/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_LatinSampler.h ( UT Library, C++)
 *
 * COMMENTS:	This class distributes samples in a 2D latin square
 *		pattern.
 */

#ifndef __UT_LatinSampler__
#define __UT_LatinSampler__

#include "UT_API.h"

// Generate latin hypercube samples
class UT_API UT_LatinSampler {
public:
	     UT_LatinSampler();
    virtual ~UT_LatinSampler();

    void	init(int xsamples, int ysamples);
    
    template<typename R>
    void	fillSamples(float *s, float *t,
			    float sscale, float tscale,
			    R &rand,
			    bool permute = false);

private:
    int		 myXSamples;
    int		 myYSamples;
    int		 mySamples;
};

// Generates samples in the unit circle by filling in concentric rings.
class UT_API UT_DiskSampler {
public:
     UT_DiskSampler();
    ~UT_DiskSampler();

    template<typename R>
    void	init(int total_samples, R &rand);
    
    template<typename R>
    void	fillSamples(float *s, float *t,
			    float sscale, float tscale,
			    R &rand);

private:
    int		 mySamples;
    float	*myS;
};

#endif

