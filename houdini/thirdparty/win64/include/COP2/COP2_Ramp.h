/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_Ramp.h
 *
 * COMMENTS:
 *	Constant Ramp COP
 */
#ifndef __COP2_RAMP_H__
#define __COP2_RAMP_H__

#include "COP2_API.h"
class cop2_RampData;
struct cop2_RampPoint;

#include <UT/UT_ValArray.h>
#include "COP2_Generator.h"

#define ARG_RAMP_TYPE		(myParmBase + 0)
#define ARG_RAMP_INTERPOLATION	(myParmBase + 1)
#define ARG_RAMP_CYCLES		(myParmBase + 2)
#define ARG_RAMP_PHASE		(myParmBase + 3)
#define ARG_RAMP_ROTATION	(myParmBase + 4)
#define ARG_RAMP_CENTER		(myParmBase + 5)
#define ARG_RAMP_WRAP		(myParmBase + 6)
#define ARG_RAMP_REPEAT		(myParmBase + 7)
#define ARG_RAMP_NUM_POINTS	(myParmBase + 8)

class COP2_API COP2_Ramp : public COP2_Generator
{
public:
    static OP_Node		*myConstructor(OP_Network*, const char *,
					       OP_Operator *);
    static OP_TemplatePair	 myTemplatePair;
    static OP_VariablePair	 myVariablePair;
    static PRM_Template		 myTemplateList[];
    static CH_LocalVariable	 myVariableList[];
    static PRM_Template		 myObsoleteList[];

    void			 getRampColor(float col[4],float pos,float t);
    
protected:
    virtual bool		 updateParmsFlags();
    
    virtual OP_ERROR		generateTile(COP2_Context &context,
					     TIL_TileList *tilelist);

    virtual COP2_ContextData	*newContextData(const TIL_Plane *, int,
						float t, int xres, int yres,
						int thread,
						int max_threads);
 
    virtual	~COP2_Ramp();
    
private:
		 COP2_Ramp(OP_Network *parent, const char *name,
			   OP_Operator *entry);

    void	evaluatePointParameters(UT_ValArray<cop2_RampPoint *>&points,
					float scale, float t);

    void	makeHorizontal(cop2_RampData *rdata,
			       float *d1, float *d2, float *d3, float *d4,
			       int x1, int y1, int x2, int y2);
    
    void	makeVertical(cop2_RampData *rdata,
			     float *d1, float *d2, float *d3, float *d4,
			     int x1, int y1, int x2, int y2);
    
    void	makeRadial(cop2_RampData *rdata,
			   float *d1, float *d2, float *d3, float *d4,
			   int x1, int y1, int x2, int y2, float cx, float cy);
    
    void	makeConcentric(cop2_RampData *rdata,
			       float *d1, float *d2, float *d3, float *d4,
			       int x1, int y1, int x2, int y2,
			       float cx, float cy);
    
    void	makeRotated(cop2_RampData *rdata,
			    float *d1, float *d2, float *d3, float *d4,
			    int x1, int y1, int x2, int y2,
			    float rot, float cx, float cy);
    
    void	fillData(TIL_TileList *,
			 TIL_Tile *, TIL_Tile *, TIL_Tile *, TIL_Tile *,
			 float *, float *, float *, float *);
    
    int		RAMP_TYPE()
		{ return evalInt(ARG_RAMP_TYPE, 0, 0); }

    bool	WRAP_FIRST_LAST()
		{ return evalInt(ARG_RAMP_WRAP, 0, 0); }

    bool	REPEAT()
		{ return evalInt(ARG_RAMP_REPEAT, 0, 0); }

    float	NUM_CYCLES(float t)
		{ return evalFloat(ARG_RAMP_CYCLES, 0, t); }
    
    float	PHASE(float t)
		{ return evalFloat(ARG_RAMP_PHASE, 0, t); }

    float	ROTATION(float t)
		{ return evalFloat(ARG_RAMP_ROTATION, 0, t); }
    
    void	CENTER(float &x, float &y, float t)
		{ x = evalFloat(ARG_RAMP_CENTER, 0, t);
		  y = evalFloat(ARG_RAMP_CENTER, 1, t); }

    int		GLOBAL_INTERP()
		{ return evalInt(ARG_RAMP_INTERPOLATION, 0, 0); }

    int		NUM_POINTS() { return evalInt(ARG_RAMP_NUM_POINTS, 0, 0.0f); }

    // Ramp points.
    bool	POINT_ENABLE(int index);
    float	POINT_POS(int index, float t);
    void	POINT_COLOR(int index, float color[4], float t);
    int		INTERPOLATION(int index);
    
    virtual const char  *getOperationInfo();
    
    int		 myParmBase;
};

class COP2_API cop2_RampData : public COP2_ContextData
{
public:
		 cop2_RampData()
		{ myRamp[0] = myRamp[1] = myRamp[2] = myRamp[3] = 0; }
    
    virtual	~cop2_RampData()
		{ delete [] myRamp[0]; delete [] myRamp[1];
		  delete [] myRamp[2]; delete [] myRamp[3]; }

    void	 blendRamp(int index, cop2_RampPoint *p1, cop2_RampPoint *p2,
			   float blend);

    int		 mySize;
    int		 myRampType;
    float	*myRamp[4];
    float	 myRotation;
    float	 myCenterX, myCenterY;
    bool	 myRepeat;

};
#endif
