/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_ConvexHull3D.h ( GU Library, C++)
 *
 */

#ifndef __GU_ConvexHull3D_h__
#define __GU_ConvexHull3D_h__

#include "GU_API.h"

#include <GEO/GEO_PolyCounts.h>
#include <UT/UT_Array.h>

class GA_PointGroup;
class GU_Detail;

/// Utility for computing (and optionally expanding or shrinking) convex hulls.
/// This uses Bullet's convex hull algorithm (btConvexHullComputer).
class GU_API GU_ConvexHull3D
{
public:
    /// Computes the convex hull of the given point cloud.
    /// @param shrink_amount If greater than zero, each face will be moved
    /// towards the center along its normal by this amount. The value will be
    /// clamped to not exceed the minimum distance of any face to the center of
    /// the convex hull.
    void computeAndShrink(fpreal shrink_amount, const UT_Vector3D *points,
                          int npoints, bool remove_inline);

    /// Calls computeAndShrink() with the point positions from the given
    /// detail.
    void computeAndShrink(
        fpreal shrink_amount, const GU_Detail &src_gdp,
        const GA_PointGroup *src_grp, bool remove_inline);

    /// Expands the convex hull outwards by the given amount.
    void expand(fpreal expand_amount);

    /// Appends the convex hull's geometry to the given detail, optionally
    /// copying attributes from the corresponding points in the source detail.
    void getGeometry(GU_Detail &gdp, const GU_Detail *src_gdp = nullptr,
                     const GA_PointGroup *src_grp = nullptr) const;

    /// Returns a list of the points in the convex hull.
    const UT_Array<UT_Vector3D> &getPoints() const { return myPoints; }

    /// Returns the source point index for each point in the convex hull.
    const UT_Array<int> &getPointSources() const { return myPointSources; }

    /// Returns a list of the polygons in the convex hull.
    const GEO_PolyCounts &getPolygonSizes() const { return myPolygonSizes; }

    /// Returns a list of the vertices. Use getPolygonSizes() to iterate over
    /// the polygons.
    const UT_Array<int> &getVertices() const { return myVertices; }

private:
    SYS_FORCE_INLINE const UT_Vector3D &vertexPoint(exint vtx) const
    {
        return myPoints(myVertices(vtx));
    }

    UT_Array<UT_Vector3D> myPoints;
    UT_Array<int> myPointSources;
    GEO_PolyCounts myPolygonSizes;
    UT_Array<int> myVertices;
};

#endif
