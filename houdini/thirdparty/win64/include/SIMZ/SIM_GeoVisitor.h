/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_GeoVisitor_h__
#define __SIM_GeoVisitor_h__

#include "SIMZ_API.h"

#include <UT/UT_VectorTypes.h>

class GA_PointGroup;
class GEO_Quadric;
class GU_Detail;

class SIMZ_API SIM_GeoVisitor
{
public:
    virtual ~SIM_GeoVisitor();

    /// Called for each sphere or tube primitive in the geometry.
    virtual void onPrimQuadric(const GEO_Quadric *quadric) = 0;

    /// Called for each convex hull shape in the geometry.
    virtual void onConvexHull(const GU_Detail &gdp,
                              const GA_PointGroup *points) = 0;

    /// Called for each box shape that is found in the geometry.
    virtual void onBox(const UT_Vector3 &center, const UT_Vector3 &size,
                       const UT_Matrix3 &rot,
                       const GU_Detail &gdp,
                       const GA_PointGroup *points) = 0;

    /// Get the refiner user to refine to polygons to computer the
    /// the convex hulls.
    virtual void refine(const GU_Detail *src, GU_Detail &dest,
                        const UT_Matrix4D &xform) = 0;
};

/// If group_by_connectivity is false, a single shape will be created for the
/// entire geometry. Otherwise, a separate shape will be created for each
/// connected component in the geometry.
void SIMZ_API SIMvisitGeometry(const GU_Detail &gdp, SIM_GeoVisitor &visitor,
                               bool group_by_connectivity);

#endif
