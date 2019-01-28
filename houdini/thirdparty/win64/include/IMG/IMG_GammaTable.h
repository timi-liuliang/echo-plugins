/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_GammaTable.h ( IMG Library, C++)
 *
 * COMMENTS:	Gamma table computer
 */

#ifndef __IMG_GammaTable__
#define __IMG_GammaTable__

#include <SYS/SYS_Types.h>
#include "IMG_API.h"

class IMG_API IMG_GammaTable
{
public:
     IMG_GammaTable();

     // Convert 8 bit value to 0-255 range with color space conversion
     float	sRGBtoLinear(uint8 v) const { return myRGBtoLinear256[v]; }
     float	linearTosRGB(uint8 v) const { return myLinearToRGB256[v]; }

     // Convert 8 bit value to 0-1 range with color space conversion
     float	sRGBtoLinear01(uint8 v) const { return myRGBtoLinear1[v]; }
     float	linearToRGB01(uint8 v) const { return myRGBtoLinear1[v]; }
     // Convert 8-bit to float value
     float	linear01(uint8 v) const	{ return myLinear01[v]; }

     // Approximate gamma conversion from standard 8bit files.
     // 256 entries, mapping to 0..255 range.
     const float *get8bitsRGBtoLinear() const { return myRGBtoLinear256; }
     const float *get8bitLinearTosRGB() const { return myLinearToRGB256; }

private:
     float myRGBtoLinear256[256];	// 0-255
     float myLinearToRGB256[256];	// 0-255
     float myRGBtoLinear1[256];		// 0-1
     float myLinearToRGB1[256];		// 0-1
     float myLinear01[256];		// 0-1
};

extern IMG_API IMG_GammaTable IMGgammaTable;

#endif
