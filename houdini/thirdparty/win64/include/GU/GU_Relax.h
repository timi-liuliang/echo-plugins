/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:     GU_Relax.h (GU Library, C++)
 *
 * COMMENTS: Declarations of functions for relaxing points away from each other
 *
 */

#pragma once

#ifndef __GU_Relax_h__
#define __GU_Relax_h__

#include "GU_API.h"
#include <GA/GA_Handle.h>
#include <UT/UT_Array.h>
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Types.h>

class GA_Detail;
class GA_Range;
class GEO_Face;
class GU_RayIntersect;
class UT_KDTree;

#define GU_USE_POINT_BVH 1
#if GU_USE_POINT_BVH
namespace GEO {
template<uint NAXES> class PointBVHT;
}
namespace GU {
using PointBVH = const GEO::PointBVHT<3>;
}
#else
class GEO_PointTreeGAOffset;

namespace GU {
using PointBVH = GEO_PointTreeGAOffset;
}
#endif

/// Computes new positions of points into newpositions, (which
/// is indexed by point offset, *not* index), such that the points
/// would be more spread out in the 2D space tangent to the
/// given normals.  It returns true if at least one
/// point would be moved, and false if no points would be moved.
///
/// NOTE: If you're relaxing points in volumes/tetrahedra/curves, you probably
///       want to use guRelaxPoints3D or guRelaxPoints1D instead.  This is more
///       for surfaces.
///
/// pttree must contain exactly the points that can influence the points
/// in relaxrange.  It does not need to be all of the points in gdp,
/// but the points must be in gdp.  If radiusattrib is invalid, pttree
/// shouldn't use radii.
///
/// If normalattrib is invalid, normalvalue is used to get the tangent
/// plane for each point.
/// If radiusattrib is invalid, radiusvalue is used to indicate the
/// radius of each point either influencing or being relaxed.
///
/// stepweight indicates the portion of the distance toward the target
/// position that the move represents.  1.0 moves directly to the target
/// on this iteration.  0.5 moves halfway there for extra stability.
/// 2.0 moves past the target, which may spread out points faster or may
/// not converge at all.
///
/// NOTE: It is now the caller's responsibility to call buildIfNeeded()
///       on the tree before calling this!  The point tree is
///       not modified by this function.
GU_API bool guRelaxPoints(
    UT_Array<UT_Vector3> &newpositions,
    const GA_Detail *gdp,
    const GA_Range &relaxrange,
    GU::PointBVH &pttree,
    const GA_ROHandleV3 &normalattrib,
    const UT_Vector3 &normalvalue,
    const GA_ROHandleF &radiusattrib,
    const fpreal radiusvalue,
    const GA_ROHandleF &maxstepattrib = GA_ROHandleF(),
    const fpreal stepweight=1.0,
    const bool scaleattrib=false);

/// Computes new positions of points into newpositions, (which
/// is indexed by point offset, *not* index), such that the points
/// would be more spread out in 3D space.  It returns true if at least one
/// point would be moved, and false if no points would be moved.
///
/// NOTE: If you're relaxing points on a surface/curve, you probably want
///       to use guRelaxPoints or guRelaxPoints1D instead.  This is more for
///       volumes/tetrahedra.
///
/// pttree must contain exactly the points that can influence the points
/// in relaxrange.  It does not need to be all of the points in gdp,
/// but the points must be in gdp.  If radiusattrib is invalid, pttree
/// shouldn't use radii.
///
/// If radiusattrib is invalid, radiusvalue is used to indicate the
/// radius of each point either influencing or being relaxed.
///
/// stepweight indicates the portion of the distance toward the target
/// position that the move represents.  1.0 moves directly to the target
/// on this iteration.  0.5 moves halfway there for extra stability.
/// 2.0 moves past the target, which may spread out points faster or may
/// not converge at all.
///
/// NOTE: It is now the caller's responsibility to call buildIfNeeded()
///       on the tree before calling this!  The point tree is
///       not modified by this function.
GU_API bool guRelaxPoints3D(
    UT_Array<UT_Vector3> &newpositions,
    const GA_Detail *gdp,
    const GA_Range &relaxrange,
    GU::PointBVH &pttree,
    const GA_ROHandleF &radiusattrib,
    const fpreal radiusvalue,
    const GA_ROHandleF &maxstepattrib = GA_ROHandleF(),
    const fpreal stepweight=1.0,
    const bool scaleattrib=false);

/// Computes new positions of points into newpositions, (which
/// is indexed by point offset, *not* index), such that the points would
/// be more spread out along the given curve.  Distances are measured as
/// arc-lengths between the u values of the points.  uvwattrib is updated
/// safely, (after all points have been moved, to avoid order-dependence).
/// It returns true if at least one point would be moved, and false if no
/// points would be moved.
///
/// NOTE: If you're relaxing points on a surface/volume/tetrahedra, you
///       probably want to use guRelaxPoints or guRelaxPoints3D instead.
///       This is *only* for curves.
/// NOTE: u values must be in the range [0,1].
///
/// If radiusattrib is invalid, radiusvalue is used to indicate the
/// radius (half-width) of each point either influencing or being relaxed.
///
/// stepweight indicates the portion of the distance toward the target
/// position that the move represents.  1.0 moves directly to the target
/// on this iteration.  0.5 moves halfway there for extra stability.
/// 2.0 moves past the target, which may spread out points faster or may
/// not converge at all.
GU_API bool guRelaxPoints1D(
    UT_Array<UT_Vector3> &newpositions,
    const GA_Detail *gdp,
    const GA_Range &relaxrange,
    const GA_ROHandleF &radiusattrib,
    fpreal radiusvalue,
    const GA_RWHandleV3 &uvwattrib,
    const GEO_Face &face,
    const fpreal stepweight=1.0);

/// Computes new (2D) positions of points into newpositions, such that the
/// points would be more spread out. It returns true if at least one
/// point would be moved, and false if no points would be moved.
///
/// pttree must have dimension 2 and consist of *exactly all points in
/// oldpositions*, *in the same order as in pttree*.  If radii is empty,
/// pttree shouldn't use radii.
///
/// If radii is empty, radiusvalue is used to indicate the
/// radius of each point either influencing or being relaxed.
///
/// If unitspacewrap is true, space will be treated as wrapping to
/// always stay within the unit square between 0 and 1 in both x and y,
/// e.g. points near x=0 may be adjacent to points near x=1.  However,
/// input points themselves are then assumed to be between 0 and 1,
/// and output points will be between 0 and 1.
///
/// stepweight indicates the portion of the distance toward the target
/// position that the move represents.  1.0 moves directly to the target
/// on this iteration.  0.5 moves halfway there for extra stability.
/// 2.0 moves past the target, which may spread out points faster or may
/// not converge at all.
///
/// NOTE: It is now the caller's responsibility to call buildIfNeeded()
///       on the tree before calling this!  GEO_2DTree handles this
///       automatically in its constructor.  The point tree is
///       not modified by this function.
GU_API bool guRelaxPoints2D(
    UT_Array<UT_Vector2> &newpositions,
    const UT_Array<UT_Vector2> &oldpositions,
    UT_KDTree &pttree,
    const UT_Array<float> &radii,
    const fpreal radiusvalue,
    const bool wrapunitspace = true,
    const fpreal stepweight=1.0);

/// Projects the points in projectrange of gdp onto the closest points of the
/// surface specified by surfaceintersect.
/// (Uses minimum distance, *NOT* projecting along normal.)
///
/// If normalattrib is valid, the normal on the surface at the new position
/// will be written to it.  If surfacenormalattrib is specified, it will be
/// used to compute the surface normal, instead of using the surface
/// primitive's evaluateNormalVector().
///
/// If primnumattrib or primuvwattrib are specified, the surface primitive
/// index or uvw of each points will be stored.
///
/// If radiusattrib is specified, it will be used as an upper bound on the
/// distance to the surface to speed up querying the GU_RayIntersect.  Points
/// more than this distance from the surface will not be moved.  If
/// radiusattrib is invalid, radiusvalue will be used for all points.
GU_API void guProjectPointsOntoSurface(
    GA_Detail *gdp,
    const GA_Range &projectrange,
    const GU_RayIntersect *surfaceintersect,
    const GA_RWHandleV3 &normalattrib = GA_RWHandleV3(),
    const GA_ROHandleV3 &surfacenormalattrib = GA_ROHandleV3(),
    const GA_RWHandleT<int64> &primnumattrib = GA_RWHandleT<int64>(),
    const GA_RWHandleV3 &primuvwattrib = GA_RWHandleV3(),
    const GA_ROHandleF &radiusattrib = GA_ROHandleF(),
    fpreal radiusvalue = FLT_MAX,
    const bool scaleattrib=false);

#endif
