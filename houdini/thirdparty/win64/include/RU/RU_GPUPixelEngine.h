/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_GPUPixelEngine.h
 *
 * COMMENTS:
 *	Performs a variety of pixel functions on each pixel using the GPU
 *	instead of the CPU.
 */
#ifndef RU_GPU_PIXELENGINE_H
#define RU_GPU_PIXELENGINE_H

#include "RU_Algorithm.h"

class RU_PixelFunction;

class RU_API RU_GPUPixelEngine : public RU_Algorithm
{
public:
	     RU_GPUPixelEngine();
    virtual ~RU_GPUPixelEngine();

    DECLARE_PIXEL_OP(RU_GPUPixelEngineOp)
       
    RU_PixelFunction *myFunctionList;
};

#endif
