/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GEO_Rewire.h (GEO Library, C++)
 *
 * COMMENTS:    Functions for rewiring vertices in bulk
 */

#pragma once

#ifndef __GEO_Rewire__
#define __GEO_Rewire__

#include "GEO_API.h"
#include <GA/GA_Handle.h>

class GEO_Detail;
class GA_Group;
class UT_WorkBuffer;

/// Rewires vertices in group to the point indices specified by rewire_to.
/// rewire_to can be a vertex, point, or primitive attribute.
/// group can be a point, primitive, vertex, or edge group, and it
/// will be expanded based on the owner of rewire_to.
void GEO_API GEOrewireVertices(
    GEO_Detail &detail,
    const GA_RWHandleT<exint> &rewire_to,
    const GA_Group *group = nullptr,
    bool rewire_points_recursively = false,
    bool keep_unused_points = false,
    UT_WorkBuffer *warnings = nullptr
);

#endif
