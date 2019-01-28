/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_Convert.h
 *
 * COMMENTS:
 *	Conversion routines for converting a pixel to other pixel types.
 */

#ifndef PXL_CONVERT_H
#define PXL_CONVERT_H

#include "PXL_API.h"
#include "PXL_Pixel.h"

template<class ToType, int fast1, class FromType, int fast2>
class PXL_Convert
{
public:
    // conversion function from any type to any type.
    inline static void convert(	     PXL_Pixel<ToType,  fast1> &,
			       const PXL_Pixel<FromType,fast2> &);
};

#include "PXL_ConvertImpl.h"

#endif
