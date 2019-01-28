/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_PixelEngine.h
 *
 * COMMENTS:
 *	Performs a variety of pixel functions on each pixel.
 */
#ifndef RU_PIXELENGINE_H
#define RU_PIXELENGINE_H

#include "RU_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_Lock.h>

#include "RU_Algorithm.h"

class ru_lut8;
class ru_lut16;
class ru_lut16fp;
class RU_PixelFunction;

class RU_API RU_PixelEngine : public RU_Algorithm
{
public:
		 RU_PixelEngine();
    virtual	~RU_PixelEngine();

    DECLARE_PIXEL_OP(RU_PixelEngineOp)
    
    // Parameters.
    RU_PixelFunction *myFunctionList;

// The remainder of this class should be treated as private.
    
    // Lookup table optimizations. Called internally, do not call.
    ru_lut8	*buildLUT8(unsigned int b, unsigned w);
    ru_lut16	*buildLUT16(unsigned int b, unsigned w);
    ru_lut16fp	*buildLUT16FP();

    UT_Lock	myLUTLock8, myLUTLock16, myLUTLock16FP;

    UT_ValArray<ru_lut8 *>  myLUT8;
    UT_ValArray<ru_lut16 *> myLUT16;
    ru_lut16fp		   *myLUT16FP;
};


#endif
