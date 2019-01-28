/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_Convolve.h
 *
 * COMMENTS:
 *	Does a convolution. 
 */
#ifndef RU_CONVOLVE_H
#define RU_CONVOLVE_H

#include "RU_API.h"
#include <UT/UT_FilterType.h>

#include "RU_Algorithm.h"

class RU_API RU_Convolve : public RU_Algorithm
{
public:
		 RU_Convolve();
    virtual	~RU_Convolve();

    DECLARE_FILTER_OP(RU_ConvolveOp);
    
    // Parameters.
    float		myConvolveEffect;
    int			myConvolveSizeX,   myConvolveSizeY;
    float		*myConvolveFilter;
};


#endif
