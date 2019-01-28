/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_PixelFunctions.h
 *
 * COMMENTS:
 *	Defines simple functions on a pixel value that may be chained together.
 */
#ifndef RU_PixelFunctions_h
#define RU_PixelFunctions_h

#include "RU_API.h"
#include <SYS/SYS_Math.h>
#include <UT/UT_Matrix4.h>
#include <PXL/PXL_Lookup.h>
#include <CL/CL_Clip.h>
#include "RU_PixelFunction.h"

class UT_ComputeGPU;

// ----------------------------------------------------------------------
// Simple scalar functions.

class RU_API RU_AddFunc : public RU_PixelFunction
{ 
public:
			 RU_AddFunc(float addend) : myAddend(addend) {}
protected:
    static float	 add(RU_PixelFunction *pf, float val, int)
		 	 { return val +((RU_AddFunc*)pf)->myAddend; }
    
    virtual RUPixelFunc  getPixelFunction() const { return add; }
    virtual void	 getPixelShader(UT_String &frag_shader);
    
    float	myAddend;
};


class RU_API RU_MultFunc : public RU_PixelFunction
{ 
public:
			 RU_MultFunc(float fact) : myFactor(fact) {}
protected:
    static float	 mult(RU_PixelFunction *pf, float val, int)
		 	 { return val * ((RU_MultFunc*)pf)->myFactor; }
    
    virtual RUPixelFunc  getPixelFunction() const { return mult; }
    virtual void	 getPixelShader(UT_String &frag_shader);
    
    float		 myFactor;
};

class RU_API RU_MAddFunc : public RU_PixelFunction
{ 
public:
			 RU_MAddFunc(float fact, float add);
			 RU_MAddFunc(float fact[4], float add[4]);

protected:
    static float	 madd(RU_PixelFunction *pf, float val, int comp)
		{ return val * ((RU_MAddFunc*)pf)->myFactor[comp]
			     + ((RU_MAddFunc*)pf)->myAddend[comp]; }
    
    virtual RUPixelFunc  getPixelFunction() const { return madd; }
    virtual void	 getPixelShader(UT_String &frag_shader);
    
    virtual bool	 eachComponentDifferent() const { return myCompSep; }
    
    bool		 myCompSep;
    float		 myFactor[4], myAddend[4];
};

class RU_API RU_ScaleFunc : public RU_PixelFunction
{ 
public:
			 RU_ScaleFunc(float scale, float pivot)
			     : myScale(scale), myPivot(pivot) {}

protected:
    static float	 scale(RU_PixelFunction *pf, float val, int )
		{ return (val - ((RU_ScaleFunc*)pf)->myPivot)
		      * ((RU_ScaleFunc*)pf)->myScale
		      + ((RU_ScaleFunc*)pf)->myPivot; }
    
    virtual RUPixelFunc  getPixelFunction() const { return scale; }
    virtual void	 getPixelShader(UT_String &frag_shader);
    
    float		 myScale, myPivot;
};


class RU_API RU_GammaFunc : public RU_PixelFunction
{
public:
		        RU_GammaFunc(float g) : myCompSep(false)
			{ float ig = 1.0f / g; myInvGamma[0] = myInvGamma[1] =
			  myInvGamma[2] = myInvGamma[3] = ig; }
    
			 RU_GammaFunc(float r, float g, float b, float w)
			     : myCompSep(true)
			{ myInvGamma[0] = 1.0f / r; myInvGamma[1] = 1.0f / g;
			  myInvGamma[2] = 1.0f/  b; myInvGamma[3] = 1.0f / w; }
protected:
    static float	 gamma(RU_PixelFunction *pf, float val, int);
    static float	 gamma(float val, float g);
    
    virtual bool	 eachComponentDifferent() const { return myCompSep; }
    virtual RUPixelFunc  getPixelFunction() const { return gamma; }
    virtual void	 getPixelShader(UT_String &frag_shader);
    
    float		 myInvGamma[4];
    bool		 myCompSep;
};


class RU_API RU_LimitFunc : public RU_PixelFunction
{
public:
    
    RU_LimitFunc(bool lower, float lval, bool upper, float uval, bool clamp)
	: myLClip(lower),
	  myUClip(upper),
	  myClamp(clamp),
	  myLLimit(lval),
	  myULimit(uval) {}

protected:
    static float	 limit(RU_PixelFunction *pf, float val, int);
    
    virtual RUPixelFunc  getPixelFunction() const { return limit; }
    virtual void	 getPixelShader(UT_String &frag_shader);

    unsigned char myLClip:1,
		  myUClip:1,
		  myClamp:1;
    float	  myLLimit, myULimit;
};


class RU_API RU_QuantizeFunc : public RU_PixelFunction
{
public:
    RU_QuantizeFunc(float step, float offset)
	: myStep(step), myOffset(offset) {}

protected:
    static float	 quantize(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc  getPixelFunction() const { return quantize; }
    virtual void	 getPixelShader(UT_String &frag_shader);

    float		 myStep, myOffset;
};

// ------------------------------------------------------------------------
// Vector based functions.

class RU_API RU_NTSCLuminanceFunc : public RU_PixelFunction
{
public:
	    RU_NTSCLuminanceFunc() {}

protected:
    static void		luminance(RU_PixelFunction *f, float **vals,
				   const bool *scope);
    
    virtual bool		needAllComponents() const { return true; }
    virtual RUVectorFunc	getVectorFunction() const { return luminance; }
    virtual void		getPixelShader(UT_String &frag_shader);
};

class RU_API RU_LuminanceFunc : public RU_PixelFunction
{
public:
	    RU_LuminanceFunc() {}

protected:
    static void		luminance(RU_PixelFunction *f, float **vals,
				   const bool *scope);
    
    virtual bool		needAllComponents() const { return true; }
    virtual RUVectorFunc	getVectorFunction() const { return luminance; }
    virtual void		getPixelShader(UT_String &frag_shader);
};


class RU_API RU_HueFunc : public RU_PixelFunction
{
public:
	    RU_HueFunc() {}

protected:
    static void		hue(RU_PixelFunction *f, float **vals,
				   const bool *scope);
    
    virtual bool		needAllComponents() const { return true; }
    virtual RUVectorFunc	getVectorFunction() const { return hue; }
    virtual void		getPixelShader(UT_String &frag_shader);
    
};


class RU_API RU_SaturationFunc : public RU_PixelFunction
{
public:
	    RU_SaturationFunc() {}

protected:
    static void		saturation(RU_PixelFunction *f, float **vals,
				   const bool *scope);
    
    virtual bool		needAllComponents() const { return true; }
    virtual RUVectorFunc	getVectorFunction() const { return saturation; }
    virtual void		getPixelShader(UT_String &frag_shader);
};


class RU_API RU_MaxFunc : public RU_PixelFunction
{
public:
	    RU_MaxFunc() {}

protected:
    static void		maxfunc(RU_PixelFunction *f, float **vals,
				   const bool *scope);
    
    virtual bool		needAllComponents() const { return true; }
    virtual RUVectorFunc	getVectorFunction() const { return maxfunc; }
    virtual void		getPixelShader(UT_String &frag_shader);
};


class RU_API RU_MinFunc : public RU_PixelFunction
{
public:
    RU_MinFunc() {}

protected:
    static void		minfunc(RU_PixelFunction *f, float **vals,
				   const bool *scope);
    
    virtual bool		needAllComponents() const { return true; }
    virtual RUVectorFunc	getVectorFunction() const { return minfunc; }
    virtual void		getPixelShader(UT_String &frag_shader);
};


class RU_API RU_AverageFunc : public RU_PixelFunction
{
public:
    RU_AverageFunc() {}

protected:
    static void		average(RU_PixelFunction *f, float **vals,
				   const bool *scope);
    
    virtual bool		needAllComponents() const { return true; }
    virtual RUVectorFunc	getVectorFunction() const { return average; }
    virtual void		getPixelShader(UT_String &frag_shader);
};


class RU_API RU_LengthFunc : public RU_PixelFunction
{
public:
    RU_LengthFunc() {}

protected:
    static void		length(RU_PixelFunction *f, float **vals,
				   const bool *scope);
    
    virtual bool		needAllComponents() const { return true; }
    virtual RUVectorFunc	getVectorFunction() const { return length; }
    virtual void		getPixelShader(UT_String &frag_shader);
};
   
class RU_API RU_ComponentFunc : public RU_PixelFunction
{
public:
    RU_ComponentFunc(int comp) : myComponent(comp) {}

protected:
    static void		component(RU_PixelFunction *f, float **vals,
				  const bool *scope);
    
    virtual bool		needAllComponents() const { return true; }
    virtual RUVectorFunc	getVectorFunction() const { return component; }
    virtual void	 getPixelShader(UT_String &frag_shader);

    int myComponent;
};
   

class RU_API RU_NormalizeFunc : public RU_PixelFunction
{
public:
    RU_NormalizeFunc() {}

protected:
    static void		normalize(RU_PixelFunction *f, float **vals,
				   const bool *scope);
    
    virtual bool		needAllComponents() const { return true; }
    virtual RUVectorFunc	getVectorFunction() const { return normalize; }
    virtual void		getPixelShader(UT_String &frag_shader);
};

class RU_API RU_HSVConvertFunc : public RU_PixelFunction
{
public:
    enum RU_HSVConvertOp {
	CONVERT_TO_HSV,
	CONVERT_TO_RGB,
        ADJUST_HSV
    };
    
    RU_HSVConvertFunc(RU_HSVConvertOp op, float hues = 0.0f,
		      float *sat = 0, float *val = 0, bool maintain_lum = true)
	: myHSVOp(op),
	  myHueShift(hues),
	  mySatScale(sat?sat[0]:1.0f),
	  mySatShift(sat?sat[1]:0.0f),
	  myValueScale(val?val[0]:1.0f),
	  myValueShift(val?val[1]:0.0f),
	  myMaintainLum(maintain_lum) {}

protected:
    static void		toHSV(RU_PixelFunction *f, float **vals,
				   const bool *scope);
    static void		fromHSV(RU_PixelFunction *f, float **vals,
				   const bool *scope);
    static void		adjustHSV(RU_PixelFunction *f, float **vals,
				   const bool *scope);
    
    virtual bool		needAllComponents() const { return true; }
    virtual RUVectorFunc	getVectorFunction() const
				{ if(myHSVOp==CONVERT_TO_HSV) return toHSV;
				  if(myHSVOp==CONVERT_TO_RGB) return fromHSV;
				  return adjustHSV; }
    virtual void		getPixelShader(UT_String &frag_shader);

    RU_HSVConvertOp	myHSVOp;
    float		myHueShift;
    float		mySatScale, mySatShift;
    float		myValueScale, myValueShift;
    bool		myMaintainLum;
};

class RU_API RU_VectorFunc : public RU_PixelFunction
{
public:
    RU_VectorFunc(const UT_Matrix4 &matrix) : myMatrix(matrix) {}
    
protected:
    static void		matrixmult(RU_PixelFunction *f, float **vals,
				   const bool *scope);
    
    virtual bool		needAllComponents() const { return true; }
    virtual RUVectorFunc	getVectorFunction() const { return matrixmult; }
    virtual void		getPixelShader(UT_String &frag_shader);

    UT_Matrix4	myMatrix;
};


class RU_API RU_LookupClipFunc : public RU_PixelFunction
{
public:
    RU_LookupClipFunc(const CL_Clip *clip) : myClip(clip) {}
protected:
    static float	lookup(RU_PixelFunction *pf, float val, int);
    static void		vlookup(RU_PixelFunction *f, float **vals,
			       const bool *scope);
    
    virtual bool		needAllComponents() const
				{ return myClip->getNumTracks() > 1; }
    virtual RUVectorFunc	getVectorFunction() const { return vlookup; }
    virtual RUPixelFunc		getPixelFunction() const  { return lookup; }
    virtual void		getPixelShader(UT_String &frag_shader);

    const CL_Clip *myClip;
};

class RU_API RU_LookupTableFunc : public RU_PixelFunction
{
public:
    RU_LookupTableFunc(PXL_Lookup *table) : myTable(table) {}

    virtual ~RU_LookupTableFunc() { delete myTable; }

protected:
    static float	lookup(RU_PixelFunction *pf, float val, int);
    static void		vlookup(RU_PixelFunction *f, float **vals,
			       const bool *scope);
    
    virtual bool	eachComponentDifferent() const { return myTable->getNumChannels() > 1; }
    virtual bool	needAllComponents() const
			{ return myTable->is3DLUT(); }
    
    virtual RUPixelFunc	 getPixelFunction() const { return lookup; }
    virtual RUVectorFunc getVectorFunction() const { return vlookup; }

    PXL_Lookup		*myTable;
};

    
// Mathematical functions.

class RU_API RU_AbsFunc : public RU_PixelFunction
{
public:
    RU_AbsFunc() {}
protected:
    static float	absolute(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return absolute; }
    virtual void	getPixelShader(UT_String &frag_shader);
};

class RU_API RU_SignFunc : public RU_PixelFunction
{
public:
    RU_SignFunc() {}
protected:
    static float	sign(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return sign; }
    virtual void	getPixelShader(UT_String &frag_shader);
};

class RU_API RU_PowerFunc : public RU_PixelFunction
{
public:
    RU_PowerFunc(float exponent) : myExponent(exponent) {}
protected:
    static float	power(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return power; }
    virtual void	getPixelShader(UT_String &frag_shader);
    float myExponent;
};

class RU_API RU_RaiseFunc : public RU_PixelFunction
{
public:
    RU_RaiseFunc(float base) : myBase(base) {}
protected:
    static float	raise(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return raise; }
    virtual void	getPixelShader(UT_String &frag_shader);
    float myBase;
};

class RU_API RU_LogFunc : public RU_PixelFunction
{
public:
    RU_LogFunc(float base, bool replace, float value);
protected:
    static float	logarithm(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return logarithm; }
    virtual void	getPixelShader(UT_String &frag_shader);
    float myBase;
    bool  myDoReplace;
    float myValue;
};

class RU_API RU_SinFunc : public RU_PixelFunction
{
public:
    RU_SinFunc(float scale, float shift) : myScale(scale), myShift(shift) {}
protected:
    static float	sinfunc(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return sinfunc; }
    virtual void	getPixelShader(UT_String &frag_shader);
    float myScale;
    float myShift;
};

class RU_API RU_SinhFunc : public RU_PixelFunction
{
public:
    RU_SinhFunc(float scale) : myScale(scale) {}
protected:
    static float	sinhfunc(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return sinhfunc; }
    virtual void	getPixelShader(UT_String &frag_shader);
    float myScale;
};

class RU_API RU_CoshFunc : public RU_PixelFunction
{
public:
    RU_CoshFunc(float scale) : myScale(scale) {}
protected:
    static float	coshfunc(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return coshfunc; }
    virtual void	getPixelShader(UT_String &frag_shader);
    float myScale;
};

class RU_API RU_TanFunc : public RU_PixelFunction
{
public:
    RU_TanFunc(float scale, bool rep, float v)
	: myScale(scale), myDoReplace(rep), myValue(v) {}
protected:
    static float	tanfunc(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return tanfunc; }
    virtual void	getPixelShader(UT_String &frag_shader);
    float myScale;
    bool  myDoReplace;
    float myValue;
};

class RU_API RU_TanhFunc : public RU_PixelFunction
{
public:
    RU_TanhFunc(float scale) : myScale(scale) {}
protected:
    static float	tanhfunc(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return tanhfunc; }
    virtual void	getPixelShader(UT_String &frag_shader);
    float myScale;
};

class RU_API RU_AtanFunc : public RU_PixelFunction
{
public:
    RU_AtanFunc(float scale) : myScale(scale) {}
protected:
    static float	atanfunc(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return atanfunc; }
    virtual void	getPixelShader(UT_String &frag_shader);
    float myScale;
};

class RU_API RU_AsinFunc : public RU_PixelFunction
{
public:
    RU_AsinFunc(float scale, bool rep, float v)
	: myScale(scale), myDoReplace(rep), myValue(v) {}
protected:
    static float	asinfunc(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return asinfunc; }
    virtual void	getPixelShader(UT_String &frag_shader);
    float myScale;
    bool  myDoReplace;
    float myValue;
};

class RU_API RU_AcosFunc : public RU_PixelFunction
{
public:
    RU_AcosFunc(float scale, bool rep, float v)
	: myScale(scale), myDoReplace(rep), myValue(v) {}
protected:
    static float	acosfunc(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return acosfunc; }
    virtual void	getPixelShader(UT_String &frag_shader);
    float myScale;
    bool  myDoReplace;
    float myValue;
};

class RU_API RU_NegateFunc : public RU_PixelFunction
{
public:
    RU_NegateFunc() {}
protected:
    static float	negate(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return negate; }
    virtual void	getPixelShader(UT_String &frag_shader);
};

class RU_API RU_InvertFunc : public RU_PixelFunction
{
public:
    RU_InvertFunc(bool rep, float v) : myDoReplace(rep), myValue(v) {}
protected:
    static float	invert(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return invert; }
    virtual void	getPixelShader(UT_String &frag_shader);
    bool  myDoReplace;
    float myValue;
};

class RU_API RU_FloorFunc : public RU_PixelFunction
{
public:
    RU_FloorFunc() {}
protected:
    static float	floorfunc(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return floorfunc; }
    virtual void	getPixelShader(UT_String &frag_shader);
};

class RU_API RU_CeilFunc : public RU_PixelFunction
{
public:
    RU_CeilFunc() {}
protected:
    static float	ceilfunc(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return ceilfunc; }
    virtual void	getPixelShader(UT_String &frag_shader);
};

class RU_API RU_RoundFunc : public RU_PixelFunction
{
public:
    RU_RoundFunc() {}
protected:
    static float	roundfunc(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc	getPixelFunction() const	{ return roundfunc; }
    virtual void	getPixelShader(UT_String &frag_shader);
};

class RU_API RU_LevelsFunc : public RU_GammaFunc
{
public:
    /// passing all data down, rather than precomputing MAdds, so that we can
    /// correctly handle the degenerate cases of 0-width input ranges.
    /// @param effect frame scope effect,
    /// applied by the ctor to the ranges but not the gammas
    /// (caller should apply it to the gammas)
    RU_LevelsFunc(float effect, 
		  float global_in_range_min,  float global_in_range_max,
		  float global_out_range_min, float global_out_range_max,
		  float global_gamma,
		  float comp_in_range_min[4],  float comp_in_range_max[4],
		  float comp_out_range_min[4], float comp_out_range_max[4],
		  float comp_gamma[4]);
    
    RU_LevelsFunc(float effect,
		  float global_in_range_min,  float global_in_range_max,
		  float global_out_range_min, float global_out_range_max,
		  float global_gamma,
		  float comp_in_range_min, float comp_in_range_max,
		  float comp_out_range_min, float comp_out_range_max,
		  float comp_gamma);

protected:
    /// for the usual case, MAdd, gamma, MAdd, gamma
    static float	fastLevels(RU_PixelFunction *pf, float val, int);

    /// handles degenerate input level ranges
    static float	safeLevels(RU_PixelFunction *pf, float val, int);
    float		safeLevels(float val, int);

    static void		colourMap(float const effect,
				  const float from_min, const float from_max,
				  const float to_min,   const float to_max,
				  float& factor, float& addend);

    
    virtual RUPixelFunc  getPixelFunction() const { return myLevelsFunc; }
    virtual void	getPixelShader(UT_String &frag_shader);

    float	myGlobalGamma;

    // for fast levels
    float	myGlobalFactor;
    float	myGlobalAddend;
    float	myFactor[4];
    float	myAddend[4];

    // for safe levels
    float	myGlobalInRangeMin;
    float	myGlobalInRangeMax;
    float	myGlobalOutRangeMin;
    float	myGlobalOutRangeMax;
    float	myCompInRangeMin[4];
    float	myCompInRangeMax[4];
    float	myCompOutRangeMin[4];
    float	myCompOutRangeMax[4];

    RUPixelFunc	myLevelsFunc;
};

#endif
