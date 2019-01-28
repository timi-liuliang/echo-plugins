/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_ColorCurve.h
 *
 * COMMENTS:
 *	A simple 2D curve for quick evaluation (for color curves).
 */
#ifndef RU_SMOOTHCURVE_H
#define RU_SMOOTHCURVE_H

#include "RU_API.h"
#include <UT/UT_FloatArray.h>
#include <UT/UT_Singleton.h>
#include <UT/UT_Vector2Array.h>
#include <TIL/TIL_Defines.h>

#include "RU_PixelFunction.h"

class GU_Detail;

class RU_API RU_ColorCurve
{
public:
     RU_ColorCurve();
    ~RU_ColorCurve();

    void reset();
    void addPoint(float pos, float val, float sharpness);
    void create(bool extend_slopes = true);

    float evaluate(float pos) const;

// data
    UT_FloatArray myDivs;
    UT_FloatArray myValues;
    UT_FloatArray mySharp;
    UT_Vector2Array myPoints;
    float	 myPreSlope, myPostSlope;
    bool	 myExtendSlope;
    int		 myNumPoints;
    int		 myNumSplines;
};


class RU_API RU_ColorCurveFunc : public RU_PixelFunction
{
public:
    RU_ColorCurveFunc(RU_ColorCurve *curve);
    RU_ColorCurveFunc(RU_ColorCurve **curves, int num);
    
    virtual ~RU_ColorCurveFunc();

protected:
    virtual bool	 eachComponentDifferent() const { return myNumCurves > 1; }
    
    static float	 curve(RU_PixelFunction *pf, float val, int);
    virtual RUPixelFunc  getPixelFunction() const { return curve; }
    virtual void	 getPixelShader(UT_String &frag_shader);

    RU_ColorCurve *myCurves[4];
    int myNumCurves;
    float *myCurveData;
};

class RU_API RU_HueCurveFunc : public RU_PixelFunction
{
public:
    // mode = 0, adjust sat. mode = 1, adjust lum.
    RU_HueCurveFunc(RU_ColorCurve *curve, int mode);
    
    virtual ~RU_HueCurveFunc();

protected:
    virtual bool	 needAllComponents() const { return true; }
    
    static void		hueSat(RU_PixelFunction *f, float **vals,
			       const bool *scope);
    static void		hueLum(RU_PixelFunction *f, float **vals,
			       const bool *scope);
    virtual RUVectorFunc getVectorFunction() const
			 { return myMode ? hueLum:hueSat; }
    virtual void	 getPixelShader(UT_String &frag_shader);

    struct CurveData
    {
	CurveData(RU_HueCurveFunc *hf);
	~CurveData();

	float	  *myScales;
	float	   myAvgScale;
    };

    RU_ColorCurve *myCurve;
    int		    myMode;
    UT_SingletonWithLock<CurveData, true> myCurveData;
};


#endif
