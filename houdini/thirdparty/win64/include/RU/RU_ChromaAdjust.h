/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_ChromaAdjust.h
 *
 * COMMENTS:
 *	Removes chroma spill from a tile.
 */
#ifndef RU_CHROMAADJUST_H
#define RU_CHROMAADJUST_H

#include "RU_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_Lock.h>

#include "RU_Algorithm.h"

class ru_lut8;
class ru_lut16;
class RU_PixelFunction;

class RU_API RU_ChromaAdjust : public RU_Algorithm
{
public:
	         RU_ChromaAdjust() : myAlpha(0) {}
    virtual	~RU_ChromaAdjust() {}

    DECLARE_PIXEL_OP(RU_ChromaAdjustOp);
    
    // Alpha is modified.
    float	*myAlpha;
    float	myChroma[3];
};


#endif
