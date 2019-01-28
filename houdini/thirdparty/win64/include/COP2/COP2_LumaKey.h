/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_LumaKey.h
 *
 * COMMENTS:
 *	LumaKey Op.
 */
#ifndef _COP2_LUMAKEY_H_
#define _COP2_LUMAKEY_H_

#include "COP2_API.h"
#define ARG_LUMAKEY_MIN_LUM	(myParmBase + 0)
#define ARG_LUMAKEY_MIN_VALUE	(myParmBase + 1)
#define ARG_LUMAKEY_MAX_LUM	(myParmBase + 2)
#define ARG_LUMAKEY_MAX_VALUE	(myParmBase + 3)
#define ARG_LUMAKEY_DISCARD	(myParmBase + 4)
#define ARG_LUMAKEY_ROLLOFF	(myParmBase + 5)
#define ARG_LUMAKEY_ROLL_FUNC	(myParmBase + 6)
#define ARG_LUMAKEY_FUNCTION	(myParmBase + 7)
#define ARG_LUMAKEY_SUPER_SAMPLE (myParmBase +8)
#define ARG_LUMAKEY_KEY_PLANE	(myParmBase + 9)

#include "COP2_KeyBase.h"

class cop2_LumaContext;

class COP2_API COP2_LumaKey : public COP2_KeyBase
{
public:
    static OP_Node		*myConstructor(OP_Network*, const char *,
					       OP_Operator *);
    static OP_TemplatePair	 myTemplatePair;
    static OP_VariablePair	 myVariablePair;
    static PRM_Template		 myTemplateList[];
    static CH_LocalVariable	 myVariableList[];
    static const char		*myInputLabels[];
    
    virtual void setColorParms(float hmin, float hmax,
			       float smin, float smax,
			       float lmin, float lmax,
			       bool add, float t);
protected:
    virtual bool		 updateParmsFlags();
    
    virtual COP2_ContextData	*newContextData(const TIL_Plane *, int,
						float t, int xres, int yres,
						int thread,
						int max_threads);
    
    virtual const char *getKeyPlane(COP2_Context &context);
    
    virtual int         getSuperSample(COP2_Context &context);
    
    virtual bool        convertToColorSpace(COP2_Context &context,
					    const float *c1,  const float *c2,
					    const float *c3,  const float *c4,
					    float *&d1,	      float *&d2,
					    float *&d3,	      float *&d4,
					    int size);
    
    virtual COP2_KeyFunction getKeyFunction(COP2_Context &context,
					    void *&data);
    
    virtual bool	 isFrameEffectHandled()  { return false; }
private:
		COP2_LumaKey(OP_Network *parent, const char *name,
			  OP_Operator *entry);
    virtual	~COP2_LumaKey();

    virtual void	getShaderAndConstants(COP2_Context &context,
					      UT_String    &shader_funcs,
					      UT_ComputeGPU &engine);
    
    bool	MIN_LUMINANCE()
		{ return (bool) evalInt(ARG_LUMAKEY_MIN_LUM, 0,0); }
    float	MIN_VALUE(float t)
		{ return evalFloat(ARG_LUMAKEY_MIN_VALUE, 0,t); }
    
    bool	MAX_LUMINANCE()
		{ return (bool) evalInt(ARG_LUMAKEY_MAX_LUM, 0,0); }
    float	MAX_VALUE(float t)
		{ return evalFloat(ARG_LUMAKEY_MAX_VALUE, 0,t); }
    
    float	ROLLOFF(float t)
		{ return evalFloat(ARG_LUMAKEY_ROLLOFF, 0,t); }
    
    int		ROLLOFF_FUNCTION()
		{ return evalInt(ARG_LUMAKEY_ROLL_FUNC,0,0); }
    
    int		DISCARD()
		{ return evalInt(ARG_LUMAKEY_DISCARD,0,0); }
    
    int		FUNCTION()
		{ return evalInt(ARG_LUMAKEY_FUNCTION,0,0); }
    
    int		SUPER_SAMPLE(float t)
		{ return evalInt(ARG_LUMAKEY_SUPER_SAMPLE,0,t); }
    
    void	KEY_PLANE(UT_String &st)
		{ evalString(st, ARG_LUMAKEY_KEY_PLANE, 0, 0); }

    int		myParmBase;
};

class COP2_API cop2_LumaContext : public COP2_ContextData
{
public:
		 cop2_LumaContext() {}
    virtual	~cop2_LumaContext() {}
    
    virtual bool createPerRes() const   { return false; }

    int		myMinLum;
    float	myMinVal;
    int		myMaxLum;
    float	myMaxVal;

    float	myRolloff;
    int		myRollFunc;
    int		myDiscard;
    int		myFunction;
    int		mySuperSample;
    UT_String	myKeyPlane;
    float	(*myInterpFunc)(float );
    float	myInvRoll;
};
#endif
