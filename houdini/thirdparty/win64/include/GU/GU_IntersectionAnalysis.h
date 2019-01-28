/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_IntersectionAnalysis.h
 *
 */

#pragma once

#include "GU_API.h"
#include "GU_Detail.h"
#include <UT/UT_Array.h>
#include <UT/UT_GeometryPredicate.h>
#include <UT/UT_UniquePtr.h>
#include <utility>
#include <string>
#include <hboost/optional.hpp>

class GU_API GU_IntersectionAnalysis
{
public:
    /// Shorthands for common vector types
    using Real  = fpreal64;
    using Vec3  = UT_Vector3T<Real>;
    using Vec3f = UT_Vector3T<fpreal32>;
    using Vec2  = UT_Vector2T<Real>;
    using Vec4  = UT_Vector4T<Real>;

    /// Supporting data structures specifying the information about triangle and
    /// segment intersections
    
    /// Where on a segment does the intersection lie
    enum SegmentIntersectionType
    {
	SEGMENT_INTERIOR = 0, // intersection occurs in the interior of the segment
	SEGMENT_VERTEX,	      // intersection occurs at the segment vertex
	SEGMENT_SEGMENT	      // the intersection forms a line segment
    };

    struct SegmentIntersectionInfo
    {
	SegmentIntersectionInfo() { }
	SegmentIntersectionInfo(Real t) // know the type from t value
	    : type(t==0||t==1 ? SEGMENT_VERTEX : SEGMENT_INTERIOR), t(t), t2(-1)
	{ }
	SegmentIntersectionInfo(Real t, Real t2) // type is always segment
	    : type(SEGMENT_SEGMENT), t(t), t2(t2)
	{ }
	SegmentIntersectionType type;

	///      t is the intersection parameter giving the intersection point:
	///	     x = (1-t)*a + t*b
	///      where (a,b) marks the corresponding segment
	Real t;
	Real t2;  // valid only when type == SEGMENT_SEGMENT
    };

    /// Where on the triangle does the intersection lie
    enum TriangleIntersectionType
    {
	TRIANGLE_INTERIOR = 0,  // intersection point inside the triangle
	TRIANGLE_EDGE,		// intersection point on triangle boundary
	TRIANGLE_VERTEX,	// intersection point on triangle vertex
	TRIANGLE_SEGMENT	// intersection forms a line segment
    };

    struct TriangleIntersectionInfo
    {
	TriangleIntersectionInfo() { }
	TriangleIntersectionInfo(TriangleIntersectionType t, int i)
	    : type(t), idx(i), idx2(-1) { }
	TriangleIntersectionInfo(TriangleIntersectionType t, int i, int i2)
	    : type(t), idx(i), idx2(i2) { }
	TriangleIntersectionType type;

	// edge idx if type == TRIANGLE_EDGE, and vtx idx if type == TRIANGLE_VERTEX
	int idx;
	int idx2;	// (WIP) not -1 only when type == TRIANGLE_SEGMENT
	Real u, v;
	Real u2, v2;	// (WIP) valid only when type == TRIANGLE_SEGMENT
    };

    /// This routine computes the intersection between a segment and a triangle.
    /// PRE:  p0,p1,p2 are triangle vertex positions
    ///       n is the triangle normal ( e.g. cross(p1-p0, p2-p0).normalize() )
    ///       a,b are segment vertex positions
    ///	      Assume that all points are distinct.
    /// POST: Returns the intersection info as defined above. This method
    ///       guarantees that the local primitive 1-u-v,u,v, coordinates are in the
    ///	      triangle interior (i.e. no coordinate is exactly 0 or 1) if
    ///       intersection type is TRIANGLE_INTERIOR. Naturally, one of 1-u-v,u,v
    ///       is zero if the type is TRIANGLE_EDGE, and two are 0 if type is
    ///       TRIANGLE_VERTEX.
    static hboost::optional<std::pair<SegmentIntersectionInfo, TriangleIntersectionInfo>>
    triangleSegmentIntersect(
		const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& n,
		const Vec3& a,  const Vec3& b);
    template<bool USEFILTER>
    static hboost::optional<std::pair<SegmentIntersectionInfo, TriangleIntersectionInfo>>
    triangleSegmentIntersect(
		const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& n,
		const Vec3& a,  const Vec3& b, const UT_GeometryPredicates<Real,USEFILTER>& pred);

    /// PRE:  Two segments p0-p1 and q0-q1. Assume that all points are distinct.
    /// POST: Find t coordinates for the two segments respectively
    ///       if the intersection is close to endpoints p0,p1,q0,q1 then these
    ///       will be accordinly snapped to 0 or 1 so you can test them against
    ///       1 and 0 later with operator==
    static hboost::optional< std::pair<SegmentIntersectionInfo, SegmentIntersectionInfo> >
    segmentSegmentIntersect(
	    const Vec3& p0, const Vec3& p1,
	    const Vec3& q0, const Vec3& q1);
    template<bool USEFILTER>
    static hboost::optional< std::pair<SegmentIntersectionInfo, SegmentIntersectionInfo> >
    segmentSegmentIntersect(
	    const Vec3& p0, const Vec3& p1,
	    const Vec3& q0, const Vec3& q1, const UT_GeometryPredicates<Real,USEFILTER>& pred);

    /// Approximate version of the function above.
    /// tol is the distance tolerance between line segments. If tol == 0, this
    /// function simply calls the function above
    static hboost::optional< std::pair<SegmentIntersectionInfo, SegmentIntersectionInfo> >
    segmentSegmentIntersect(
	    const Vec3& p0, const Vec3& p1,
	    const Vec3& q0, const Vec3& q1, Real tol);
    template<bool USEFILTER>
    static hboost::optional< std::pair<SegmentIntersectionInfo, SegmentIntersectionInfo> >
    segmentSegmentIntersect(
	    const Vec3& p0, const Vec3& p1,
	    const Vec3& q0, const Vec3& q1, Real tol, const UT_GeometryPredicates<Real,USEFILTER>& pred);

    /// Given valid details gdp0 and gdp1, returns points of intersection
    /// between the two details or self-intersection if gdp1 is null. This routine
    /// requires a valid gdp containing triangles and curves only, as ensured by
    /// validateGeometry
    static UT_Vector3DArray findIntersectionPoints(
	    // input
	    const GEO_Detail* gdp0,
	    const GA_PrimitiveGroup* prim_grp0,
	    const GEO_Detail* gdp1 = NULL,
	    const GA_PrimitiveGroup* prim_grp1 = NULL,
	    Real prox_tol = 0,
	    // output
	    UT_Array<UT_Int32Array>* input_num_arr = NULL,
	    UT_Array<UT_Int64Array>* prim_num_arr = NULL,
	    UT_Array<UT_Fpreal32Array>* prim_uvw_arr = NULL,
	    UT_Array<UT_Int64Array>* pt_num_arr = NULL);

    /// Returns a shared pointer to a GU_Detail containing validated geometry
    /// from the given input GU_Detail. A warning message is provided if the
    /// input detail needed significant changes for validation.
    /// This routine insures that the detail passed into findIntersectionPoints
    /// is valid.
    static UT_UniquePtr<GU_Detail> validateGeometry(const GU_Detail* geo, std::string& warning);

private:
    /// Given a triangle p0-p1-p2 and a segment a-b on the 2D plane, return
    /// the intersection data. Incomplete WIP
    static hboost::optional< std::pair<SegmentIntersectionInfo, TriangleIntersectionInfo> >
    triangleSegmentIntersect2D(
		const Vec2& p0, const Vec2& p1, const Vec2& p2,
		const Vec2& a,  const Vec2& b);

    // figure out intersection between the two given intervals:
    // [a[0] a[1]] and [b[0] b[1]]
    // the returned Vec2 denote intervals as fractions of the input intervals
    // respectively. E.g. (0,1) will denote the full input interval
    static hboost::optional< std::pair<Vec2,Vec2> >
    intervalIntersection(Vec2 a, Vec2 b, Real tol);
};
