/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Measure.h ( GU Library, C++)
 *
 * COMMENTS:	Compute some measurements on geometry.
 */

#ifndef __GU_Measure__
#define __GU_Measure__

#include "GU_API.h"
#include <GA/GA_Handle.h>
#include <UT/UT_Vector3.h>
#include <SYS/SYS_Types.h>

class GU_Detail;
class GA_PointGroup;
class GA_PrimitiveGroup;

class GU_API GU_Measure
{
public:
    enum CurvatureType
    {
	CURVE_ALL,
	CURVE_CONCAVE,
	CURVE_CONVEX
    };

    //! Measure the perimeter of primitives
    /// The perimeter of each primitive will be computed and stored in the
    /// attribute handle passed in.  The attribute handle MUST point to a
    /// primitive attribute.
    static void	computePerimeter(
		    GU_Detail &gdp,
		    const GA_RWHandleF &perimeter,
		    const GA_PrimitiveGroup *group);

    //! Measure the area of primitives
    /// The area of each primitive will be computed and stored in the
    /// attribute handle passed in.  The attribute handle MUST point to a
    /// primitive attribute.
    static void	computeArea(
		    GU_Detail &gdp,
		    const GA_RWHandleF &area,
		    const GA_PrimitiveGroup *group);

    //! Measure the curvature of points
    /// The curvature of each point will be computed and stored in the
    /// attribute handle passed in.  The normal attribute handle should point
    /// to an attribute containing the normal for each point (which does not
    /// have to be normalized).  This is used in the curvature computations.
    /// The curvature computed is an approximation which is more accurate for
    /// polygon meshes.  No special code is used to compute curvature on higher
    /// order surfaces.
    /// mode 
    static void	computeCurvature(
		    GU_Detail &gdp,
		    const GA_RWHandleF &curvature,
		    const GA_ROHandleV3 &normal,
		    const GA_PointGroup *pgroup,
		    const CurvatureType type,
		    fpreal min,
		    fpreal max);

    static void computeVolume(
		    GU_Detail &gdp,
		    const UT_Vector3 &refpt,
		    const GA_RWHandleF &volume,
		    const GA_PrimitiveGroup *group);
};

#endif
