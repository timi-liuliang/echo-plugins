/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_ChromaKey.h
 *
 * COMMENTS:
 *	ChromaKey Op.
 */
#ifndef _COP2_CHROMAKEY_H_
#define _COP2_CHROMAKEY_H_

#include "COP2_API.h"
#define ARG_CHROMAKEY_HUE		(myParmBase + 0)
#define ARG_CHROMAKEY_LUM		(myParmBase + 1)
#define ARG_CHROMAKEY_DISCARD		(myParmBase + 2)
#define ARG_CHROMAKEY_HUE_ROLLOFF	(myParmBase + 3)
#define ARG_CHROMAKEY_SAT_ROLLOFF	(myParmBase + 4)
#define ARG_CHROMAKEY_LUM_ROLLOFF	(myParmBase + 5)
#define ARG_CHROMAKEY_ROLL_FUNC		(myParmBase + 6)
#define ARG_CHROMAKEY_SUPER_SAMPLE	(myParmBase + 7)
#define ARG_CHROMAKEY_KEY_PLANE		(myParmBase + 8)

#include "COP2_KeyBase.h"

class cop2_ChromaContext;

class COP2_API COP2_ChromaKey : public COP2_KeyBase
{
public:
    static OP_Node		*myConstructor(OP_Network*, const char *,
					       OP_Operator *);
    static OP_TemplatePair	 myTemplatePair;
    static OP_VariablePair	 myVariablePair;
    static PRM_Template		 myTemplateList[];
    static CH_LocalVariable	 myVariableList[];
    static const char		*myInputLabels[];
    
protected:
    virtual COP2_ContextData	*newContextData(const TIL_Plane *, int,
						float t, int xres, int yres,
						int thread,int max_threads);
    
    virtual bool	 isFrameEffectHandled() { return false; }
    
    virtual void setColorParms(float hmin, float hmax,
			       float smin, float smax,
			       float lmin, float lmax,
			       bool add, float t);
    
    virtual void setSpillParms(float hmin, float hmax,
			       float smin, float smax,
			       float lmin, float lmax,
			       bool add, float t);
    
private:
		COP2_ChromaKey(OP_Network *parent, const char *name,
			  OP_Operator *entry);
    virtual	~COP2_ChromaKey();

    virtual const char *getKeyPlane(COP2_Context &context);
    virtual int         getSuperSample(COP2_Context &context);

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
    
    virtual void	getShaderAndConstants(COP2_Context &context,
					      UT_String    &shader_funcs,
					      UT_ComputeGPU &engine);
    
    void	HUE(float &h1, float &h2, float t)
		{ h1 = evalFloat(ARG_CHROMAKEY_HUE, 0, t);
		  h2 = evalFloat(ARG_CHROMAKEY_HUE, 1, t); }
    
    void	SAT(float &s1, float &s2, float t)
		{ s1 = evalFloat(ARG_CHROMAKEY_HUE, 2, t);
		  s2 = evalFloat(ARG_CHROMAKEY_HUE, 3, t); }

    void	LUM(float &l1, float &l2, float t)
		{ l1 = evalFloat(ARG_CHROMAKEY_LUM, 0, t);
		  l2 = evalFloat(ARG_CHROMAKEY_LUM, 1, t); }
    
    float	HUE_ROLLOFF(float t)
		{ return evalFloat(ARG_CHROMAKEY_HUE_ROLLOFF, 0,t); }
    
    float	SAT_ROLLOFF(float t)
		{ return evalFloat(ARG_CHROMAKEY_SAT_ROLLOFF, 0,t); }
    
    float	LUM_ROLLOFF(float t)
		{ return evalFloat(ARG_CHROMAKEY_LUM_ROLLOFF, 0,t); }
    
    int		ROLLOFF_FUNCTION()
		{ return evalInt(ARG_CHROMAKEY_ROLL_FUNC,0,0); }
    
    int		DISCARD()
		{ return evalInt(ARG_CHROMAKEY_DISCARD,0,0); }
    
    int		SUPER_SAMPLE(float t)
		{ return evalInt(ARG_CHROMAKEY_SUPER_SAMPLE,0,t); }
    
    void	KEY_PLANE(UT_String &st)
		{ evalString(st, ARG_CHROMAKEY_KEY_PLANE, 0, 0.0f); }
    
    int		myParmBase;
};

class COP2_API cop2_ChromaContext : public COP2_ContextData
{
public:
		 cop2_ChromaContext() {}
    virtual	~cop2_ChromaContext() {}

    float	myHueMin, myHueMax;
    float	mySatMin, mySatMax;
    float	myLumMin, myLumMax;
    float	myHueRolloff;
    float	mySatRolloff;
    float	myLumRolloff;
    UT_String	myKeyPlane;
    int		myRolloffFunc;
    int		mySuperSample;
    int		myDiscard;
    float	 (*myInterpFunc)(float);
};

#endif
