/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_SubPixel.h
 *
 * COMMENTS:
 *	Does a subpixel interpolation. Requires a region 1 pixel larger in X and
 *	Y then the tile. The weights are arranged in a 2x2 matrix of:
 *
 *	0 1
 *	2 3
 */
#ifndef RU_SUBPIXEL_H
#define RU_SUBPIXEL_H

#include "RU_API.h"
#include <UT/UT_FilterType.h>

#include "RU_Algorithm.h"

class RU_API RU_SubPixel : public RU_Algorithm
{
public:
		 RU_SubPixel();
    virtual	~RU_SubPixel();

    DECLARE_FILTER_OP(RU_SubPixelOp)

    void	setWeights(float x, float y);
    
    // Parameters.
    float		mySubPixelWeights[4];
};


#endif
