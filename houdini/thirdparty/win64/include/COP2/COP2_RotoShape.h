/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_RotoShape.h
 *
 * COMMENTS:
 *	Generates multiple spline or polygon shapes.
 */
#ifndef __COP2_ROTOSHAPE_H__
#define __COP2_ROTOSHAPE_H__

#include "COP2_API.h"
class cop2_RotoCurve;
class GU_Detail;

#include <UT/UT_ValArray.h>

#include "COP2_Generator.h"

class COP2_API COP2_RotoShape : public COP2_Generator
{
public:
    static OP_Node		*myConstructor(OP_Network*, const char *,
					       OP_Operator *);
    static OP_TemplatePair	 myTemplatePair;
    static OP_VariablePair	 myVariablePair;
    static PRM_Template		 myTemplateList[];
    static CH_LocalVariable	 myVariableList[];

    // only used by the state to init the UI defaults.
    static int	getDefaultCurveType();
    static void	getCurveDefaults(int &comp, int &fill,
				 bool &fillout, float &thick, int &feather,
				 float &fwidth, float color[4]);

    // used only be the state: 0 = off, 1 = no bounds shift, 2 = raw spline.
    void	generateRawCurve(int rawmode);
    
protected:
    virtual bool		 updateParmsFlags();
    
    virtual COP2_ContextData	*newContextData(const TIL_Plane *, int,
						float t, int xres, int yres,
						int thread, int max_threads);
    virtual OP_ERROR	 generateTile(COP2_Context &context,
				      TIL_TileList *tilelist);
    
    virtual void	getGeneratedBounds(COP2_Context &context,
					   int &bx1, int &by1,
					   int &bx2, int &by2);
    virtual	~COP2_RotoShape();
private:
		 COP2_RotoShape(OP_Network *parent, const char *name,
			    OP_Operator *entry);

    // global.
    int		NUM_CURVES() { return evalInt("numcurves",0,0); }
    float	LOD(float t) { return evalFloat("detail",0,t); }
    void	ANTIALIAS(int &x, int &y, float t)
		{ x = evalInt("antialias", 0, t);
		  y = evalInt("antialias", 1, t); }

    // per-curve.
    int		NUM_POINTS(int curve);
    int		CURVE_TYPE(int curve);
    int		COMPOSITE(int curve);
    int		FILL(int curve);
    bool	FILL_OUTSIDE(int curve);
    float	THICKNESS(int curve, float t);
    bool	FEATHER(int curve);
    int		FEATHER_DROPOFF(int curve);
    float	FEATHER_WIDTH(int curve, float t);
    void	COLOR(int curve, float color[4], float t);
    
    void	TRANSLATE(int curve, float &tx, float &ty, float t);
    void	PIVOT(int curve, float &px, float &py, float t);
    void	SCALE(int curve, float &sx, float &sy, float t);
    float	ROTATE(int curve, float t);	

    // per-point, per-curve
    void	POINT_POS(int curve, int point, float &x, float &y, float t);
    void	POINT_SLOPE1(int curve, int point, float &x,float &y, float t);
    void	POINT_SLOPE2(int curve, int point, float &x,float &y, float t);
    float	POINT_THICKNESS(int curve, int point, float t);
    float	POINT_FEATHER(int curve, int point, float t);

    int		myGenerateRaw;
};

class COP2_API cop2_RotoShapeData : public COP2_ContextData
{
public:
	     cop2_RotoShapeData();
    virtual ~cop2_RotoShapeData();

    GU_Detail *getCurveDetail(int i);
    GU_Detail *getFeatherDetail(int i);

    UT_ValArray<cop2_RotoCurve *> myShapes;
    
    int		  myMaxX;
    int		  myMinX;
    int		  myMaxY;
    int		  myMinY;
};

#endif
