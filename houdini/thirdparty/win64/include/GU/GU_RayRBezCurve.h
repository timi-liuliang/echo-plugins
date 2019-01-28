/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Ray intersction objects for faces, this does not
 *	include closed faces (Ie: polygons)
 *
 */

#ifndef __GU_RayRBezCurve_h__
#define __GU_RayRBezCurve_h__

#include "GU_API.h"
#include <UT/UT_Matrix4.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4Array.h>
#include <UT/UT_Matrix.h>
#include <UT/UT_BoundingBox.h>

#include "GU_RayIntersect.h"

class GEO_Primitive;
class GU_RayInfo;
class GU_MinInfo;
class GU_RayRBezCurve;
class GU_RayRBezSurf;
class TS_MetaExpression;
class GU_IsectCurveSet;
class GU_IsectCurve;


//________________________________________________________
//
//  Bezier segment ray intersection class
//
//  Note: working in homogeneous coordinate
//________________________________________________________
//

class GU_API GU_RayRBezCurve
{
public:
    void	init(int order, float uStart, float uLength, int isRational);
    void	buildBBox();
    int		intersect(const UT_Vector3 &org,
			  const UT_Vector3 &dir,
			  GU_RayInfo &hitInfo, int depth) const;
    int		intersect(const UT_Vector3 &p,
			  GU_RayInfo &hitInfo, int depth) const;
    int		intersect(const GU_RayRBezCurve &curve,
			  GU_RayInfo &hitInfo, int depth) const;
    int		intersect(GU_RayRBezSurf &surf, GU_RayInfo &hitInfo,
			  int depth) const;
    int		intersectQR(GU_RayRBezSurf &surf, GU_RayInfo &hitInfo,
			  int depth) const;
    int		intersectAP(GU_RayRBezSurf &surf, GU_RayInfo &hitInfo);
    
    // checks the proximity of the ray to the end points of the curve, 
    // and compares it with the tolerance given in hit_info.
    // If ray passes within the given tolerance, in the hemispherical cup
    // at the end of the curve, that "seals" the cylidrical zone around
    // the curve, it is considered as a hit, and 1 is returned.
    // INPUTS:
    //   org, dir - the origin and direction of the ray
    //   hit_info - its tolerance is treated as a radius of a hemisphere at the 
    //		    endpoints, within which to test the ray's presence
    // OUTPUT
    //   hit_info - if hit, a hit is added
    // RETURN:
    //  1 if hit: ray passes within tolerance radius of endpoint, otherwise 0
    int		intersectFirstPointZone(const UT_Vector3 &org,
					const UT_Vector3 &dir,
					GU_RayInfo &hit_info) const;
    int		intersectLastPointZone(const UT_Vector3 &org,
					const UT_Vector3 &dir,
					GU_RayInfo &hit_info) const;

    // Start/end are in 0,1 range.
    int		polishCurveSurf(const GU_RayRBezSurf &surf,
				UT_MatrixF &C1, UT_MatrixF &C2, UT_MatrixF &R, 
				UT_MatrixF &tmp, float start, float end, 
				UT_Array<GU_RayInfoHit> &hits,
				GU_RayFindType findType,
				int firstiter = 3, float firsttol = 1e-2F,
				int finaliter = 50, float finaltol = 1e-6F,
				float ttol = 1e-3F);

    int		minimum(const UT_Vector3 &p,
			  GU_MinInfo &mininfo, int depth) const;
    int		minimum(const GU_RayRBezCurve &curve,
			  GU_MinInfo &mininfo, int depth,
			  int noBBoxTest = 0) const;

    void	split(GU_RayRBezCurve *left, GU_RayRBezCurve *right,
		      float tol) const;
    // Does an inplace cut at both start and end, ustart < uend.
    void	carve(float ustart, float uend);
    void	evaluate(float t, UT_Vector3 &pos) const;
    void	evaluate(float t, UT_Vector3 &pos, 
			  UT_Vector3 &der) const;
    void	evaluate(float t, UT_Vector3 &pos, 
			 UT_Vector3 &der, UT_Vector3 &der2) const;

    void	print() const;

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myCV.getMemoryUsage(false);
        return mem;
    }

public:
    UT_BoundingBox	 	myBBox;
    UT_Vector4Array		myCV;
    float			myUStart, myULength;
    int				myRational;
};

#endif

