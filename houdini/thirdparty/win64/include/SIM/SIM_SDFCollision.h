/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SDFCollision_h__
#define __SIM_SDFCollision_h__

#include "SIM_API.h"
#include "SIM_SweptCollisionData.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_ValArray.h>
#include <GA/GA_PageHandle.h>
#include <GA/GA_Handle.h>
#include <GA/GA_Range.h>

class SIM_Object;
class SIM_Engine;
class GU_Detail;

struct SIM_API SIM_SDFCollideInfo
{
    bool	usesdfvelocity;
    bool	usepointvelocity;
    float	voloffset;
};

// The function SIMdetectAndResolveSDFCollisions can be used
// for one-way collision detection and collision resolution.
// Only the positions of a are modified to avoid collisions.
//
// Given linear trajectories of a set of points a, and a set
// of triangles b between times t_start and t_end,
// find modified positions for a such that the trajectories
// of a and b are collision free between t_start and t_end.
// "Collision free" means that the points of a don't
// intersect any of the triangles of b during a linear sweep
// from t_start to t_end.
// In addition, find a "projected" end velocity that
// makes the time-derivatives of the contact distances
// non-negative (as much as possible).
//
// Thickness is a diameter, not radius measure, so matches
// the behaviour of pscale.
extern bool SIM_API
SIMdetectAndResolveSDFCollisions(
    SIM_Engine	     &engine,
    // A Topology
    SIM_Object	     *obj,
    GU_Detail	     *gdp_a,
    GA_Range	      range_a,
    GA_RWPageHandleV3 &position_points_a_resolved_end,
    GA_RWPageHandleV3 &velocity_points_a_resolved_end,
    bool usepscale,
    GA_ROPageHandleF  &thickness_points_a,
    GA_ROPageHandleF  &bounce_points_a,
    GA_ROPageHandleF  &friction_points_a,
    GA_ROPageHandleF  &dynamicfriction_points_a,
    GA_ROPageHandleF  &bounceforward_points_a,
    GA_RWPageHandleV3 &position_points_a_start,

    // B Topology
    const UT_ValArray<SIM_Object *> &affectors,
    const UT_Array<SIM_SDFCollideInfo> &affector_info,
    bool			createimpacts,
    bool			addfeedbacks,
    bool			moveoutofsdf,

    // Timing
    const fpreal64 t_start,
    const fpreal64 t_end
);

extern bool SIM_API
SIMdetectSDFCollisions(
    SIM_Engine	     &engine,
    // A Topology
    SIM_Object	     *obj,
    GU_Detail	     *gdp_a,
    GA_Range	      range_a,
    GA_RWPageHandleV3 &position_points_a_resolved_end,
    bool usepscale,
    GA_ROPageHandleF  &thickness_points_a,
    GA_RWPageHandleV3 &position_points_a_start,

    // B Topology
    const UT_ValArray<SIM_Object *> &affectors,
    const UT_Array<SIM_SDFCollideInfo> &affector_info,

    // Timing
    const fpreal64 t_start,
    const fpreal64 t_end,

    // Results
    GA_RWHandleI	&hitnum_a,
    GA_RWHandleV3	&hitpos_a,
    GA_RWHandleV3	&hitnml_a,
    GA_RWHandleF	&hittime_a,
    GA_RWHandleS	&hitpath_a,
    GA_RWHandleI	&hitprim_a,
    GA_RWHandleV3	&hituv_a,
    GA_RWHandleV3	&hitvel_a
);

#endif
