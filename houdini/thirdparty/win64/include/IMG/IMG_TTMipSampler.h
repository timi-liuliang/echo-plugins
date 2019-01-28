/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_TTMipSampler.h ( IMG Library, C++)
 *
 * COMMENTS:	A general mipmap sampler for tiled textures
 */

#ifndef __IMG_TTMipSampler__
#define __IMG_TTMipSampler__

#include "IMG_API.h"
#include <UT/UT_Vector3.h>
#include "IMG_FileTTMap.h"

class IMG_TextureFilter;
class IMG_TTChannel;

class IMG_API IMG_TTMipSampler
{
public:
     IMG_TTMipSampler(int xres, int yres, IMG_TTChannel *ch);
    ~IMG_TTMipSampler();

    int64	 getMemoryUsage(bool inclusive) const
    {
        // NOTE: We don't own myChannel.
        return inclusive ? sizeof(*this) : 0;
    }

    /// This method returns raw pixel data.  iu and iv are in screen space.
    void	 getPixel(void *pixel, int iu, int iv);

    void	 sample(float *result, int nchan,
		        float u, float v,
		        const IMG_TextureFilter &filter);
    void	 sample(float *result, float &visible, int nchan,
		        float u, float v, float du, float dv,
		        const IMG_TextureFilter &filter,
			float mipblend);
    void	 sampleBilinear(float *result, int nchan,
			float u, float v, float &visible,
			const IMG_TextureFilter &filter);
    void	 sampleBicubic(float *result, int nchan,
			float u, float v, float &visible,
			const IMG_TextureFilter &filter);
    void	 sampleBiquadratic(float *result, int nchan,
			float u, float v, float &visible,
			const IMG_TextureFilter &filter);

private:
    float		 myXres, myYres;	// Float to minimize conversions
    float		 myXres5, myYres5;	// Half the xres/yres
    int			 myIXres, myIYres;	// Integer values for res
    float		 myDx, myDy;	// Size of a pixel in my map
    IMG_TTChannel	*myChannel;
};

#endif

