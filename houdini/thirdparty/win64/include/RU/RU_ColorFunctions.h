/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_ColorFunctions.h
 *
 * COMMENTS:
 * 	Various pixel functions for manipulating colors.
 */
#ifndef RU_ColorFunctions_h
#define RU_ColorFunctions_h

#include "RU_API.h"
#include "RU_PixelFunction.h"
     
typedef float (*RU_InterpFunction)(float);

class RU_API RU_ColorReplace : public RU_PixelFunction
{
public:
    RU_ColorReplace( float hmin, float hmax, float hspill,
		     float smin, float smax, float sspill,
		     float lmin, float lmax, float lspill,
		     RU_InterpFunction interp,
		     float hshift, float sscale, float lscale)
    {
	myHueMin = hmin; myHueMax = hmax; myHueSpill = hspill;
	mySatMin = smin; mySatMax = smax; mySatSpill = sspill;
	myLumMin = lmin; myLumMax = lmax; myLumSpill = lspill;

	myInterp = interp;

	myHueShift = hshift; mySatScale = sscale; myLumScale = lscale;
    }

    inline float	hueKey(float hue) const;
    inline float	satKey(float sat) const;
    inline float	lumKey(float lum) const;
protected:
    static void		replacecolor(RU_PixelFunction *f, float **vals,
				     const bool *scope);

    virtual bool		needAllComponents() const { return true; }
    virtual RUVectorFunc	getVectorFunction() const
				{ return replacecolor; }

private:
    // Source region parameters
    float			myHueMin;
    float			myHueMax;
    float			myHueSpill;
    float			mySatMin;
    float			mySatMax;
    float			mySatSpill;
    float			myLumMin;
    float			myLumMax;
    float			myLumSpill;
    RU_InterpFunction		myInterp;

    // Color transform parameters
    float			myHueShift;
    float			mySatScale;
    float			myLumScale;
};

#endif
