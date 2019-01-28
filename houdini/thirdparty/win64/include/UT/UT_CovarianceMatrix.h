/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 * 	Covariance Matrix calculations for points, segments, and triangles
 * 	in 2 and 3 dimensions. The point covariance is the usual one often
 * 	computed in geometry processing. The segment and triangle versions
 * 	integrate it over all the points of all the given segments or
 * 	triangles.
 *
 * 	The calculations are taken from the thesis of Stefan Gottschalk:
 * 	    "Collision Queries using Oriented Bounding Boxes",
 * 	pages 52 and 57.
 *
 */

#ifndef __UT_CovarianceMatrix__
#define __UT_CovarianceMatrix__

#include "UT_ValArray.h"
#include "UT_Vector2.h"
#include "UT_Vector3.h"
#include "UT_Matrix2.h"
#include "UT_SymMatrix3.h"

#include <SYS/SYS_Types.h>


namespace UT_CovarianceMatrix
{

// Calculate a 2x2 covariance matrix for a set of points in the plane.
// PT_POS must implemented a single parameter operator()(int i) to return
// a UT_Vector2T<T> for integer point number parameter values
// 0, ..., num_pts - 1.

template <typename T, typename PT_POS>
UT_Matrix2T<T>
pointCovariance2(int num_pts, PT_POS pt_pos, UT_Vector2T<T> &centroid)
{
    centroid = 0.0;

    for (int i = 0, ie = num_pts; i < ie; i++)
	centroid += pt_pos(i);

    centroid = centroid / num_pts;

    T cxx = 0.0, cxy = 0.0, cyy = 0.0;

    for (int i = 0, ie = num_pts; i < ie; i++)
    {
	auto p = pt_pos(i) - centroid;

	cxx += p.x() * p.x();
	cxy += p.x() * p.y();
	cyy += p.y() * p.y();
    }

    return { cxx, cxy, cxy, cyy };
}

// Calculate a 3x3 covariance matrix for a set of points in the space.
// PT_POS must implemented a single parameter operator()(int i) to return
// a UT_Vector3T<T> for integer point number parameter values
// 0, ..., num_pts - 1.

template <typename T, typename PT_POS>
UT_SymMatrix3T<T>
pointCovariance3(int num_pts, PT_POS pt_pos, UT_Vector3T<T> &centroid)
{
    centroid = 0.0;

    for (int i = 0, ie = num_pts; i < ie; i++)
	centroid += pt_pos(i);

    centroid = centroid / num_pts;

    T cxx = 0.0, cxy = 0.0, cxz = 0.0, cyy = 0.0, cyz = 0.0, czz = 0.0;

    for (int i = 0, ie = num_pts; i < ie; i++)
    {
	auto p = pt_pos(i) - centroid;

	cxx += p.x() * p.x();
	cxy += p.x() * p.y();
	cxz += p.x() * p.z();
	cyy += p.y() * p.y();
	cyz += p.y() * p.z();
	czz += p.z() * p.z();
    }

    return { cxx, cxy, cyy, cxz, cyz, czz };
}

// Calculate a 2x2 covariance matrix for a set of segments in the plane.
// PT_POS must implemented a single parameter operator()(int i) called
// with argument in range 0, ..., 2 * num_segs - 1.

template <typename T, typename PT_POS>
UT_Matrix2T<T>
segmentCovariance2(int num_segs, PT_POS seg_pt_pos, UT_Vector2T<T> &centroid)
{
    centroid = T(0);

    UT_Array<T> seg_len;
    UT_Array< UT_Vector2T<T> > seg_centroid_x2;

    seg_len.setSizeNoInit(num_segs);
    seg_centroid_x2.setSizeNoInit(num_segs);

    T len = 0.0;
    for (int s = 0, se = num_segs; s < se; s++)
    {
	auto p = seg_pt_pos(2 * s);
	auto q = seg_pt_pos(2 *s + 1);

	seg_centroid_x2(s) = p + q;

	auto v = q - p;

	// Add the cross product as unit normal weighted by (twice) tri area.

	seg_len(s) = v.length();
	len += seg_len(s);

	centroid += (seg_len(s) * seg_centroid_x2(s));
    }

    auto centroid_x2 = centroid / len;

    if (len == 0.0)
        return { T(0), T(0), T(0), T(0) };

    // Centroid of the all triangles:
    centroid = centroid_x2 / 2.0;

    T cxx = 0.0, cxy = 0.0, cyy = 0.0;

    for (int s = 0, se = num_segs; s < se; s++)
    {
	auto p = seg_pt_pos(2 * s) - centroid;
	auto q = seg_pt_pos(2 * s + 1) - centroid;

	auto &&scx2 = seg_centroid_x2(s) - centroid_x2;
	auto &&sl = seg_len(s);

	cxx += sl * (scx2.x() * scx2.x() + p.x() * p.x() + q.x() * q.x());
	cxy += sl * (scx2.x() * scx2.y() + p.x() * p.y() + q.x() * q.y());
	cyy += sl * (scx2.y() * scx2.y() + p.y() * p.y() + q.y() * q.y());
    }

    len *= T(6.0);

    cxx /= len;
    cxy /= len;
    cyy /= len;

    return { cxx, cxy, cxy, cyy };
}

// Calculate a 3x3 covariance matrix for a set of segments in the space.
// PT_POS must implemented a single parameter operator()(int i) called
// with argument in range 0, ..., 2 * num_segs - 1.

template <typename T, typename PT_POS>
UT_SymMatrix3T<T>
segmentCovariance3(int num_segs, PT_POS seg_pt_pos, UT_Vector3T<T> &centroid)
{
    centroid = 0.0;

    UT_Array<T> seg_len;
    UT_Array< UT_Vector3T<T> > seg_centroid_x2;

    seg_len.setSizeNoInit(num_segs);
    seg_centroid_x2.setSizeNoInit(num_segs);

    T len = 0.0;
    for (int s = 0, se = num_segs; s < se; s++)
    {
	auto p = seg_pt_pos(2 * s);
	auto q = seg_pt_pos(2 * s + 1);

	seg_centroid_x2(s) = p + q;

	auto v = q - p;

	// Add the cross product as unit normal weighted by (twice) tri area.

	seg_len(s) = v.length();
	len += seg_len(s);

	centroid += (seg_len(s) * seg_centroid_x2(s));
    }

    if (len == T(0))
        return {  T(0),  T(0),  T(0),  T(0),  T(0),  T(0) };

    auto centroid_x2 = centroid / len;

    // Centroid of the all triangles:
    centroid = centroid_x2 / 2.0;

    T cxx = 0.0, cxy = 0.0, cxz = 0.0, cyy = 0.0, cyz = 0.0, czz = 0.0;

    for (int s = 0, se = num_segs; s < se; s++)
    {
	auto p = seg_pt_pos(2 * s) - centroid;
	auto q = seg_pt_pos(2 * s + 1) - centroid;

	auto &&scx2 = seg_centroid_x2(s) - centroid_x2;
	auto &&sl = seg_len(s);

	cxx += sl * (scx2.x() * scx2.x() + p.x() * p.x() + q.x() * q.x());
	cxy += sl * (scx2.x() * scx2.y() + p.x() * p.y() + q.x() * q.y());
	cxz += sl * (scx2.x() * scx2.z() + p.x() * p.z() + q.x() * q.z());
	cyy += sl * (scx2.y() * scx2.y() + p.y() * p.y() + q.y() * q.y());
	cyz += sl * (scx2.y() * scx2.z() + p.y() * p.z() + q.y() * q.z());
	czz += sl * (scx2.z() * scx2.z() + p.z() * p.z() + q.z() * q.z());
    }

    len *= T(6.0);

    cxx /= len;
    cxy /= len;
    cxz /= len;
    cyy /= len;
    cyz /= len;
    czz /= len;

    return { cxx, cxy, cyy, cxz, cyz, czz };
}

// Calculate a 2x2 covariance matrix for a set of triangles in the plane.
// PT_POS must implemented a single parameter operator()(int i) called
// with argument in range 0, ..., 3 * num_tris - 1.

template <typename T, typename PT_POS>
UT_Matrix2T<T>
trianglesCovariance2(int num_tris, PT_POS tri_pt_pos, UT_Vector2T<T> &centroid)
{
    centroid = T(0);

    UT_Array<T> tri_area_x2;				// triangle area x 2
    UT_Array< UT_Vector2T<T> > tri_centroid_x3;		// triangle centroid x 3

    tri_area_x2.setSizeNoInit(num_tris);
    tri_centroid_x3.setSizeNoInit(num_tris);

    T area_x2 = 0.0;
    for (int t = 0, te = num_tris; t < te; t++)
    {
        auto t0 = 3 * t;
	auto p = tri_pt_pos(t0);
	auto q = tri_pt_pos(t0 + 1);
	auto r = tri_pt_pos(t0 + 2);

	tri_centroid_x3(t) = p + q + r;
	tri_area_x2(t) = SYSabs((q.x() - p.x()) * (r.y() - p.y())
				- (q.y() - p.y()) * (r.x() - p.x()));

	area_x2 += tri_area_x2(t);
	centroid += (tri_area_x2(t) * tri_centroid_x3(t));
    }

    if (area_x2 == T(0))
        return { T(0), T(0), T(0), T(0) };

    auto centroid_x3 = centroid / area_x2;

    // Centroid of the all triangles:
    centroid = centroid_x3 / 3.0;

    T cxx = 0.0, cxy = 0.0, cyy = 0.0;

    for (int t = 0, te = num_tris; t < te; t++)
    {
        auto t0 = 3 * t;
	auto p = tri_pt_pos(t0) - centroid;
	auto q = tri_pt_pos(t0 + 1) - centroid;
	auto r = tri_pt_pos(t0 + 2) - centroid;

	auto &&tcx3 = tri_centroid_x3(t) - centroid_x3;
	auto &&tax2 = tri_area_x2(t);

	cxx += tax2 * (tcx3.x() * tcx3.x()
		       + p.x() * p.x() + q.x() * q.x() + r.x() * r.x());
	cxy += tax2 * (tcx3.x() * tcx3.y()
		       + p.x() * p.y() + q.x() * q.y() + r.x() * r.y());
	cyy += tax2 * (tcx3.y() * tcx3.y()
		       + p.y() * p.y() + q.y() * q.y() + r.y() * r.y());
    }

    area_x2 *= T(12.0);

    cxx /= area_x2;
    cxy /= area_x2;
    cyy /= area_x2;

    return { cxx, cxy, cxy, cyy };
}

// Calculate a 3x3 covariance matrix for a set of triangles in the space.
// PT_POS must implemented a single parameter operator()(int i) called
// with argument in range 0, ..., 3 * num_tris - 1.

template <typename T, typename PT_POS>
UT_SymMatrix3T<T>
triangleCovariance3(int num_tris, PT_POS tri_pt_pos, UT_Vector3T<T> &centroid,
		    UT_Vector3T<T> &average_normal)
{
    centroid = 0.0;
    average_normal = 0.0;

    UT_Array<T> tri_area_x2;
    UT_Array< UT_Vector3T<T> > tri_centroid_x3;

    tri_area_x2.setSizeNoInit(num_tris);
    tri_centroid_x3.setSizeNoInit(num_tris);

    T area_x2 = 0.0;
    for (int t = 0, te = num_tris; t < te; t++)
    {
        auto t0 = 3 * t;
	auto p = tri_pt_pos(t0);
	auto q = tri_pt_pos(t0 + 1);
	auto r = tri_pt_pos(t0 + 2);

	tri_centroid_x3(t) = p + q + r;

	auto n = cross(q - p, r - p);

	// Add the cross product as unit normal weighted by (twice) tri area.
	average_normal += n;

	tri_area_x2(t) = n.length();
	area_x2 += tri_area_x2(t);

	centroid += (tri_area_x2(t) * tri_centroid_x3(t));
    }

    if (area_x2 == T(0))
        return {  T(0),  T(0),  T(0),  T(0),  T(0),  T(0) };

    auto centroid_x3 = centroid / area_x2;

    // Centroid of the all triangles:
    centroid = centroid_x3 / 3.0;
    average_normal /= area_x2;

    T cxx = 0.0, cxy = 0.0, cxz = 0.0, cyy = 0.0, cyz = 0.0, czz = 0.0;

    for (int t = 0, te = num_tris; t < te; t++)
    {
        auto t0 = 3 * t;
	auto p = tri_pt_pos(t0) - centroid;
	auto q = tri_pt_pos(t0 + 1) - centroid;
	auto r = tri_pt_pos(t0 + 2) - centroid;

	auto &&tcx3 = tri_centroid_x3(t) - centroid_x3;
	auto &&tax2 = tri_area_x2(t);

	cxx += tax2 * (tcx3.x() * tcx3.x()
		       + p.x() * p.x() + q.x() * q.x() + r.x() * r.x());
	cxy += tax2 * (tcx3.x() * tcx3.y()
		       + p.x() * p.y() + q.x() * q.y() + r.x() * r.y());
	cxz += tax2 * (tcx3.x() * tcx3.z()
		       + p.x() * p.z() + q.x() * q.z() + r.x() * r.z());
	cyy += tax2 * (tcx3.y() * tcx3.y()
		       + p.y() * p.y() + q.y() * q.y() + r.y() * r.y());
	cyz += tax2 * (tcx3.y() * tcx3.z()
		       + p.y() * p.z() + q.y() * q.z() + r.y() * r.z());
	czz += tax2 * (tcx3.z() * tcx3.z()
		       + p.z() * p.z() + q.z() * q.z() + r.z() * r.z());
    }

    area_x2 *= T(12.0);

    cxx /= area_x2;
    cxy /= area_x2;
    cxz /= area_x2;
    cyy /= area_x2;
    cyz /= area_x2;
    czz /= area_x2;

    return { cxx, cxy, cyy, cxz, cyz, czz };
}

} // namespace UT_CovarianceMatrix

#endif
