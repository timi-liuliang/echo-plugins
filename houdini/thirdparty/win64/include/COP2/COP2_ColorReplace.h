/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_ColorReplace.h
 *
 * COMMENTS:
 * 	Colour replacement op.
 */
#ifndef _COP2_COLORREPLACE_H_
#define _COP2_COLORREPLACE_H_

#include "COP2_API.h"
#define ARG_COLORREPLACE_SRCHUE		(myParmBase + 0)
#define ARG_COLORREPLACE_SRCLUM		(myParmBase + 1)
#define ARG_COLORREPLACE_TGTCLR		(myParmBase + 2)
#define ARG_COLORREPLACE_HUE_ROLLOFF	(myParmBase + 3)
#define ARG_COLORREPLACE_SAT_ROLLOFF	(myParmBase + 4)
#define ARG_COLORREPLACE_LUM_ROLLOFF	(myParmBase + 5)
#define ARG_COLORREPLACE_ROLL_FUNC	(myParmBase + 6)
#define ARG_COLORREPLACE_OPERATION	(myParmBase + 7)

class RU_PixelFunction;

#include "COP2_PixelOp.h"
#include "COP2_KeyBase.h"

class COP2_API COP2_ColorReplace : public COP2_KeyBase
{
public:
    
    static OP_Node		*myConstructor(OP_Network*, const char *,
					       OP_Operator *);
    static OP_TemplatePair	 myTemplatePair;
    static OP_VariablePair	 myVariablePair;
    static PRM_Template		 myTemplateList[];
    static CH_LocalVariable	 myVariableList[];
    static const char *		 myInputLabels[];

    // Implementation of functions from COP2_KeyBase:
    virtual bool hasSpillParms() const { return true; }
    virtual void setColorParms(float hmin, float hmax,
			       float smin, float smax,
			       float lmin, float lmax,
			       bool add, float t);
    
    virtual void setSpillParms(float hmin, float hmax,
			       float smin, float smax,
			       float lmin, float lmax,
			       bool add, float t);

 protected:
    virtual COP2_ContextData	*newContextData(const TIL_Plane *, int,
						float t, int xres, int yres,
						int thread,
						int max_threads);
    
    virtual bool		 usesPartialFrameScope() { return false; }
    virtual bool		 isFrameEffectHandled()  { return false; }
    
    virtual bool        convertToColorSpace(COP2_Context &context,
					    const float *c1,  const float *c2,
					    const float *c3,  const float *c4,
					    float *&d1,	      float *&d2,
					    float *&d3,	      float *&d4,
					    int size);
    
    virtual COP2_KeyFunction getKeyFunction(COP2_Context &context,
					    void *&data);
    
    virtual void	colorFunction(COP2_Context &context,
				      float val[4], float key);
    virtual int         getSuperSample(COP2_Context &context);

private:
		COP2_ColorReplace(OP_Network *parent, const char *name,
				  OP_Operator *entry);
    virtual	~COP2_ColorReplace();
    
    virtual const char  *getOperationInfo();

    void	SRC_HUE(float &h1, float &h2, float t)
		{
		    h1 = evalFloat("srchuesat", 0, t);
		    h2 = evalFloat("srchuesat", 1, t);
		}
    void	SRC_SAT(float &s1, float &s2, float t)
		{
		    s1 = evalFloat("srchuesat", 2, t);
		    s2 = evalFloat("srchuesat", 3, t);
		}
    void	SRC_LUM(float &l1, float &l2, float t)
		{
		    l1 = evalFloat("srclum", 0, t);
		    l2 = evalFloat("srclum", 1, t);
		}

    float	TARGET_RED(float t)
		{ return evalFloat("targetclr", 0, t); }
    float	TARGET_GREEN(float t)
		{ return evalFloat("targetclr", 1, t); }
    float	TARGET_BLUE(float t)
		{ return evalFloat("targetclr", 2, t); }

    float	HUE_ROLLOFF(float t)
		{ return evalFloat("hrolloff", 0, t); }
    float	SAT_ROLLOFF(float t)
		{ return evalFloat("srolloff", 0, t); }
    float	LUM_ROLLOFF(float t)
		{ return evalFloat("lrolloff", 0, t); }

    int		ROLLOFF_FUNCTION()
		{ return evalInt("rollfunc", 0, 0.0F); }

    int		OPERATION()
		{ return evalInt("operation", 0, 0.0F); }

    int		SUPER_SAMPLE(float t)
		{ return evalInt("supersample", 0, t); }

    int		 myParmBase;
};

class COP2_API cop2_ColorReplaceData : public COP2_ContextData
{
public:
		 cop2_ColorReplaceData() {}
    virtual	~cop2_ColorReplaceData() {}
    
    virtual bool createPerRes() const   { return false; }

    float	myHueMin;
    float	myHueMax;
    float	mySatMin;
    float	mySatMax;
    float	myLumMin;
    float	myLumMax;

    float	myHueRoll;
    float	mySatRoll;
    float	myLumRoll;
    cop2_InterpType	myRolloffFunc;

    float	myR;
    float	myG;
    float	myB;

    float	myHue;
    float	mySat;
    float	myLumScale;

    int		myOperation;

    int		mySuperSample;
};
#endif
