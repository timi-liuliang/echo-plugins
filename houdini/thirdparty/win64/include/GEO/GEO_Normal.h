/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GEO_Normal.h (GEO Library, C++)
 *
 * COMMENTS:    Functions for computing normals
 */

#pragma once

#ifndef __GEO_Normal__
#define __GEO_Normal__

#include "GEO_API.h"
#include <GA/GA_Handle.h>
#include <stddef.h>

class GEO_Detail;
class GA_Group;

#define GEO_DEFAULT_CUSP_ANGLE 60.0

/// Magic tolerance factor of 1.001 on GEO_DEFAULT_CUSP_ANGLE so that
/// hexagonal tubes will be smooth by default, even with a bit of roundoff
/// or slight deformation.
#define GEO_DEFAULT_ADJUSTED_CUSP_ANGLE 60.06

/// This determines how contributions from multiple vertices around a point
/// get weighted when computing a normal for that point.
enum class GEO_NormalMethod
{
    UNIFORM_WEIGHTED, // Each vertex gets equal weight
    ANGLE_WEIGHTED,   // Each vertex is weighted by angle (usually best results)
    AREA_WEIGHTED     // Each vertex is weighted by total face area
};

/// Computes normals into given attribute using detail->getP().
/// If group is specified, only the associated elements are updated.
/// If normalattrib is a detail attribute and group is given,
/// it is only computed based on the primitives associated with the group.
/// If vertex normals, a cusp angle of 0 or less cusps everything, and
/// a cusp angle of 180 or more cusps nothing.
void GEO_API GEOcomputeNormals(
    const GEO_Detail &detail,
    const GA_RWHandleV3 &normalattrib,
    const GA_Group *group = NULL,
    const float cuspangledegrees = GEO_DEFAULT_ADJUSTED_CUSP_ANGLE,
    const GEO_NormalMethod method = GEO_NormalMethod::ANGLE_WEIGHTED);

/// Computes normals into given attribute using pos_attrib (owned by detail).
/// If group is specified, only the associated elements are updated.
/// If normalattrib is a detail attribute and group is given,
/// it is only computed based on the primitives associated with the group.
/// If vertex normals, a cusp angle of 0 or less cusps everything, and
/// a cusp angle of 180 or more cusps nothing.
void GEO_API GEOcomputeNormals(
    const GEO_Detail &detail,
    const GA_ROHandleV3 &posattrib,
    const GA_RWHandleV3 &normalattrib,
    const GA_Group *group = NULL,
    const float cuspangledegrees = GEO_DEFAULT_ADJUSTED_CUSP_ANGLE,
    const GEO_NormalMethod method = GEO_NormalMethod::ANGLE_WEIGHTED);

#endif
