/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * (In contrast, static collision detection techniques only test
 * for penetrations and proximities at a fixed point in time, which allows
 * points to tunnel through polygons undetected.)
 *
 * This swept-collision code is used by DOPS, but it does not depend
 * on DOPS. For example, it does not know about SIM_Geometry or SIM_Object.
 * Therefore, it should be easy to re-use this code in another place,
 * for example POPs.
 */

#ifndef __SIM_SweptCollision_h__
#define __SIM_SweptCollision_h__

#include "SIM_API.h"
#include "SIM_SweptCollisionData.h"
#include <UT/UT_Vector3.h>
#include <GA/GA_PageHandle.h>
#include <GA/GA_Handle.h>
#include <GA/GA_Range.h>
#include <vector>

class SIM_Object;
class SIM_Impacts;
class GU_Detail;

class SIM_API SIM_SweptCollisionImpactAccumulator
{
public:
    SIM_SweptCollisionImpactAccumulator(
        SIM_Impacts*const impacts_a,
        const int impact_flags_a,
        const GU_Detail*const gdp_a,
	
        const std::vector<SIM_Impacts*>* impacts_b,
	const int impact_flags_b,	
        const std::vector<const SIM_Object*>& undated_objects_b,
        const SIM_TriangleConnectivity& triangles_b
    );
    
    // Apply opposite and equal impulses to A and B,
    // where A is a point and B is a position on a triangle.
    // pof_a is a valid point offset for 'gdp_a'
    // b is a valid index of a triangle of 'triangles_b'
    void addPointTriangleImpulsePair(
        const GA_Offset pof_a,
        const UT_Vector3D& ra,
	
	const exint b,
	const fpreal64 barys_b[3],
	
        const UT_Vector3D& impulse_direction,
	const fpreal64 impulse_magnitude,
	const fpreal64 t
    );

private:
    SIM_Impacts*const impacts_a_;
    const int impact_flags_a_;
    const GU_Detail*const gdp_a_;
	
    const std::vector<SIM_Impacts*>* impacts_b_;
    const int impact_flags_b_;	
    const std::vector<const SIM_Object*>& undated_objects_b_;
    const SIM_TriangleConnectivity& triangles_b_;

    // Disallow
    SIM_SweptCollisionImpactAccumulator();    
    SIM_SweptCollisionImpactAccumulator(SIM_SweptCollisionImpactAccumulator&);
    SIM_SweptCollisionImpactAccumulator& operator=(SIM_SweptCollisionImpactAccumulator&);
};

// The function SIMdetectAndResolveCollisions can be used
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
//
// The optional "impact_acculumator" receives all the impacts
// that were applied to make A's point trajectories collision free.
// NOTE: this is fragile, this depends on the impulses being
// applied in exactly the right way. This is an architectural problem.
// In some uses cases, the end positions can't be changed directly here.
//
extern bool SIM_API
SIMdetectAndResolveCollisions(
    SIM_SweptCollisionImpactAccumulator*const impact_acculumator,

    // A Topology
    GA_Range	      range_a,
    GA_RWPageHandleV3 &position_points_a_resolved_end,
    GA_RWPageHandleV3 &velocity_points_a_resolved_end,
    GA_ROPageHandleF  &thickness_points_a,
    GA_ROPageHandleF  &mass_points_a,
    GA_ROPageHandleF  &bounce_points_a,
    GA_ROPageHandleF  &friction_points_a,
    GA_ROPageHandleF  &bounceforward_points_a,
    GA_RWPageHandleV3 &position_points_a_start,
    bool	       pscaleisradius,

    // B Topology
    const SIM_TriangleConnectivity& triangles_b,
    const std::vector<fpreal32>& thickness_points_b,
    const std::vector<fpreal32>& bounce_points_b,
    const std::vector<fpreal32>& friction_points_b,
    const std::vector<fpreal32>& bounceforward_points_b,
    const SIM_PositionPoints& position_points_b_start,
    const SIM_PositionPoints& position_points_b_end,
    const SIM_VelocityPoints& velocity_points_b_end,

    // Timing
    const fpreal64 t_start,
    const fpreal64 t_end
);

extern bool SIM_API
SIMdetectCollisions(
    // A Topology
    GA_Range	      range_a,
    GA_RWPageHandleV3 &position_points_a_resolved_end,
    GA_ROPageHandleF  &thickness_points_a,
    GA_RWPageHandleV3 &position_points_a_start,
    bool	       pscaleisradius,

    // B Topology
    const SIM_TriangleConnectivity& triangles_b,
    const std::vector<fpreal32>& thickness_points_b,
    const SIM_PositionPoints& position_points_b_start,
    const SIM_PositionPoints& position_points_b_end,

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
