/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_KeyBase.h
 *
 * COMMENTS:
 *	Interface class for all keying Cops.
 */
#ifndef _COP2_KEYBASE_H_
#define _COP2_KEYBASE_H_

#include "COP2_API.h"
#include "COP2_MaskOp.h"

#include <TIL/TIL_CompOptions.h>
class TIL_TileList;
class COP2_Context;
class UT_ComputeGPU;

enum cop2_InterpType
{
    COP2_KEY_INTERP_LINEAR   =0,
    COP2_KEY_INTERP_EASE_IN  =1,
    COP2_KEY_INTERP_EASE_OUT =2,
    COP2_KEY_INTERP_HALF_COS =3,
    COP2_KEY_INTERP_GAUSSIAN =4
};

typedef float (*cop2_InterpFunc)    (float w);
typedef float (*cop2_Interpolate)   (float v1, float v2, float w);
typedef float (*COP2_KeyFunction)   (void *data, const float val[4]);

class COP2_API COP2_KeyBase : public COP2_MaskOp
{
public:
    virtual bool hasSpillParms() const;
    virtual void setColorParms(float hmin, float hmax,
			       float smin, float smax,
			       float lmin, float lmax,
			       bool add, float t);
    
    virtual void setSpillParms(float hmin, float hmax,
			       float smin, float smax,
			       float lmin, float lmax,
			       bool add, float t);

    virtual void setStateControl(int state);

    // Interpolation functions.
    static float  hueInterp(float v1, float v2, float w);
    static float  linear(float v1, float v2, float w);

    // Comparison functions.
    static float  hueComp(float h, float hue1, float hue2, float spill = 0.0f,
			  float (*ifunc)(float) = 0);
    static float linearComp(float v, float v1, float v2, float spill = 0.0f,
			    float (*ifunc)(float) = 0);

    // does a bounding box expansion on the HSL region
    static void expandRegion(float h, float s, float l,
			     float &hmin, float &hmax,
			     float &smin, float &smax,
			     float &lmin, float &lmax);

    static cop2_InterpFunc   getInterpolation(cop2_InterpType type);
protected:
    // override these methods.
    virtual const char *getKeyPlane(COP2_Context &context);
    virtual int         getSuperSample(COP2_Context & /*context*/) { return 1; }

    virtual bool       convertToColorSpace(COP2_Context &context,
					   const float *c1,
					   const float *c2,
					   const float *c3,
					   const float *c4,
					   float *&d1,
					   float *&d2,
					   float *&d3,
					   float *&d4,
					   int size);
    
    virtual COP2_KeyFunction getKeyFunction(COP2_Context &context,
					    void *&data);
    
    virtual void	colorFunction(COP2_Context &context,
				      float val[4], float key);

    virtual cop2_Interpolate	getInterpFunction(COP2_Context &context,
						  int component);
    
protected:    
		 COP2_KeyBase(OP_Network *parent,
			      const char *name,
			      OP_Operator *entry);
    virtual	~COP2_KeyBase();

    virtual TIL_Sequence *  cookSequenceInfo(OP_ERROR &error);
    
    virtual OP_ERROR	doCookMyTile(COP2_Context &context,
				     TIL_TileList *tilelist);

    OP_ERROR		doGPUCook(COP2_Context &context,
				  TIL_TileList *tilelist,
				  const TIL_Plane *keyplane,
				  int supersample);
    void		doKeyOperation(TIL_TileList	*tiles,
				       int		 supersample,
				       COP2_Context	&context,
				       const float	*c1, 
				       const float	*c2,
				       const float	*c3,
				       const float	*c4);

    virtual void	getInputDependenciesForOutputArea(
			    COP2_CookAreaInfo &output_area,
			    const COP2_CookAreaList &input_areas,
			    COP2_CookAreaList &needed_areas);

    virtual void	getShaderAndConstants(COP2_Context &context,
					      UT_String    &shader_funcs,
					      UT_ComputeGPU &engine);

    bool	myAlphaGenerated;
};

#endif
