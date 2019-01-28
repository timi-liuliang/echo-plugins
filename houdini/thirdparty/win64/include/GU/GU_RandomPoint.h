/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_RandomPoint.h ( GU Library, C++)
 *
 * COMMENTS:	Create a cloud of points on a surface
 */

#ifndef __GU_RandomPoint__
#define __GU_RandomPoint__

#include "GU_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_String.h>
#include <GA/GA_AttributeRef.h>

class GU_Detail;
class GA_PrimitiveGroup;
class GA_PointGroup;

class GU_API GU_RandomPoint
{
public:
    //!Random Point Generation
    //
    /// This method generates random points from the primitives in the detail.
    ///   gdp     - geometry containing the primitives (and which will contain
    ///             the points)
    ///   npoints - The number of points to generate
    ///   seed    - random number seed
    ///   probability - A probability array.  There should be one entry per
    ///             primitive which determines the probabilty of that primitive
    ///             having a point generated from it.
    ///   group   - Optionally, only generate from this group of primitives
    ///   prim_num - Store the primitive number associated with the scattered
    ///	             point
    ///   prim_uv  - Store the primitive's u/v associated with the point
    ///
    /// Points will be generated using as close to a uniform distribution as
    /// possible (in uv space on the primitives).  The point locations should
    /// remain constant based on primitive number, regardless of shearing or
    /// scaling.
    static void		generatePoints(GU_Detail &gdp, int npoints, fpreal seed,
				UT_FloatArray &probability,
				const GA_PrimitiveGroup *group,
				const GA_RWAttributeRef &prim_num=GA_RWAttributeRef(),
				const GA_RWAttributeRef &prim_uv=GA_RWAttributeRef());


    /// This method generates random points from the points in the detail.
    ///   dstgdp    geometry which will contain the generated points
    ///   srcgdp    geometry which will contains the source points (can be
    ///			the same as dstgdp)
    ///	  srcptgrp  the source point group from which to generate points
    ///   ptsperpt- Whether the npts parameter indicates the total number of
    ///             number of points to generate or the number per point.
    ///   npoints - The number of points to generate (total or per-point)
    ///   seed    - random number seed
    ///   ptnum   - Store the point number of the source point.
    ///   ptidx   - Store the index for each point within a particular source
    ///             point.  For example if a particular source point generates
    ///             ten points, this attribute's values will range from 0-9.
    ///   emit    - An attribute that holds the probability for each source
    ///             point, which determines the probabilty of that point
    ///             having points generated from it.
    ///   attribs - A pattern of attributes to copy from the source points to
    ///             the generated points.
    ///
    ///   Returns - a contiguous range containing the generated points.
    ///
    /// In ptsperpt mode, the source point's "id" attribute will be used for
    /// stable random probability generation.
    static GA_Range generatePointsFromPoints(GU_Detail *dstgdp,
					const GU_Detail *srcgdp,
					const GA_PointGroup *srcptgrp,
					fpreal ptsperpt, GA_Size npts,
					fpreal seed,
					const GA_RWAttributeRef &dstptnumref,
					const GA_RWAttributeRef &dstptidxref,
					const GA_ROAttributeRef &srcemitref,
					const UT_String &attribpattern);

};

#endif
