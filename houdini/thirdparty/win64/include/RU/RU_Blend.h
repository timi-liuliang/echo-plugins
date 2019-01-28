/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_Blend.h
 *
 * COMMENTS:
 *	Does a composite between two images.
 */
#ifndef RU_BLEND_H
#define RU_BLEND_H

#include "RU_API.h"
#include "RU_Algorithm.h"

class RU_API RU_Blend : public RU_Algorithm
{
public:
		 RU_Blend()
		     : myBaseBlend(1.0f), myBlend1(0.0f), myBlend2(1.0f),
		       mySingleFactorBlend(true), myBlendInvert(false) {}
    virtual	~RU_Blend() { }

    float	 myBaseBlend;
    float	 myBlend1;
    float	 myBlend2;
    bool	 mySingleFactorBlend;
    bool	 myBlendInvert;

    DECLARE_BINARY_OP(RU_BlendOp)
    DECLARE_PIXEL_OP(RU_BlendSingleOp)
    DECLARE_FILTER_OP(RU_BlendFilterOp)
};


#endif
