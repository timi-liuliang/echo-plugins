/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_RotoSpline.h
 *
 * COMMENTS:
 *	RotoSpline creation class.
 */
#ifndef RU_ROTOSPLINE_H
#define RU_ROTOSPLINE_H

#include "RU_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_VectorTypes.h>

class RU_RotoPoint;
class GU_Detail;

enum RU_RotoFill
{
    ROTO_FILL_OPEN = 0,
    ROTO_FILL_CLOSED,
    ROTO_FILL_HOLLOW
};

enum RU_RotoType
{
    ROTO_TYPE_POLYGON = 0,
    ROTO_TYPE_BEZIER  = 1,
    ROTO_TYPE_NURBS   = 2
};

class RU_API RU_RotoSpline
{
public:
		 RU_RotoSpline(RU_RotoType curvetype, RU_RotoFill type,
			       float detail =0.5f, float aspect = 1.0f);
		~RU_RotoSpline();

    exint	  entries() const		{ return myPoints.entries(); }
    RU_RotoPoint *getPoint(int i)		{ return myPoints(i); }
    RU_RotoPoint *getPointByIndex(float index);
    void	  addPoint(RU_RotoPoint *point);

    RU_RotoFill	  getFillType() const { return myFillType; }
    

	
    GU_Detail	 *build(GU_Detail *rawcurve = 0);
    GU_Detail	 *buildFeather(float basewidth = 0.5F, GU_Detail *rawcurve = 0);

    static void	  buildFreehand(const UT_Vector2Array &ipoints,
				UT_Vector2Array &points,
				UT_Vector2Array &slopes1,
				UT_Vector2Array &slopes2,
				float tolerance,bool isbezier, bool close);

    // The feather parameter controls whether or not we create a point
    // attribute containing each points feather width.
    void	  buildRawCurve(GU_Detail &, bool feather=false);


private:
    UT_Array<RU_RotoPoint *>    myPoints;
    RU_RotoFill			myFillType;
    float			myAspect;
    float			myDetail;
    RU_RotoType			myCurveType;
};


class RU_API RU_RotoPoint
{
public:
    RU_RotoPoint(float index, float u, float v, float w,
		 float u1 =0.0F, float v1 = 0.0F,
		 float u2 = 0.0F, float v2 = 0.0F,
		 float feather = 0.0F)
	: myU(u), myV(v), myThickness(w),
	  mySlopeU1(u1), mySlopeU2(u2), mySlopeV1(v1), mySlopeV2(v2),
	  myIndex(index), myFeatherWidth(feather) {}
	
    float myU, myV;
    float myThickness;
    float mySlopeU1, mySlopeU2, mySlopeV1, mySlopeV2; 
    float myIndex;
    float myFeatherWidth;
};



#endif
