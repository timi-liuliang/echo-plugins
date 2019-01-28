/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Clean Tools (C++)
 *
 * COMMENTS:	Tools used for cleaning geometry.
 */

#ifndef __GU_CLEANTOOLS_H__
#define __GU_CLEANTOOLS_H__

#include "GU_API.h"
#include <UT/UT_RTree.h>
#include <UT/UT_String.h>

class GU_Detail;
class GA_PointGroup;
class GA_PrimitiveGroup;

namespace GU_CleanTools
{

    // the following are used to determine whether we want to MARK, REPAIR, or
    // plainly IGNORE bad geometry
    enum gu_Action
    {
	GU_IGNORE_PROBLEM = 0,
	GU_MARK_PROBLEM = 1,
	GU_REPAIR_PROBLEM = 2
    };

    // -------------------------- filtering routines --------------------------
    // these routines use lazy deletion on primitives and points, thus leaving
    // the process bitarray valid even if they are used consecutively.
    //
    // Name Convention:
    // process* - marks and repairs geometry problems (mutates geometry)
    // repair*	- repairs geometry problems possibly depending on input group
    // mark*	- just marks bad geometry as BAD by putting it into the input
    //		  group. (no mutation)

    GU_API extern void processBadOrientation(GU_Detail *gdp,
	    gu_Action bad_orientation_actn,
	    const GA_PrimitiveGroup *process,
	    GA_PrimitiveGroup *marked_polys);

    GU_API extern void precomputePolyAreas(GU_Detail *gdp,
	    UT_ValArray<fpreal> &arr_poly_area,
	    const GA_PrimitiveGroup *process);

    GU_API extern void processLocallySmallAreas(
	    GU_Detail *gdp, gu_Action loc_small_area_actn, fpreal area_loc_threshold,
	    uint random_seed, const UT_ValArray<fpreal> &arr_poly_area,
	    const GA_PrimitiveGroup *process, GA_PrimitiveGroup *marked_polys,
	    GA_PointGroup *marked_pts, GA_PrimitiveGroup *prims_to_remove,
	    GA_PointGroup *pts_to_remove);

    GU_API extern void processGloballySmallAreas(
	    GU_Detail *gdp, gu_Action glo_small_area_actn, fpreal area_glo_threshold,
	    uint random_seed, const UT_ValArray<fpreal> &arr_poly_area,
	    const GA_PrimitiveGroup *process, GA_PrimitiveGroup *marked_polys,
	    GA_PointGroup *marked_pts, GA_PrimitiveGroup *prims_to_remove,
	    GA_PointGroup *pts_to_remove);

    GU_API extern void processLocallySmallEdges(GU_Detail *gdp,
	    gu_Action loc_small_edge_actn,
	    fpreal edge_size_loc_threshold,
	    const GA_PrimitiveGroup *process,
	    GA_PrimitiveGroup *marked_polys,
	    GA_PointGroup *marked_pts,
	    GA_PrimitiveGroup *prims_to_remove,
	    GA_PointGroup *pts_to_remove);

    GU_API extern void processGloballySmallEdges(GU_Detail *gdp,
	    gu_Action glo_small_edge_actn,
	    fpreal edge_size_glo_threshold,
	    const GA_PrimitiveGroup *process,
	    GA_PrimitiveGroup *marked_polys,
	    GA_PointGroup *marked_pts,
	    GA_PrimitiveGroup *prims_to_remove,
	    GA_PointGroup *pts_to_remove);

    GU_API extern void processNonUniform(gu_Action non_uniform_actn,
	    fpreal uni_threshold,
	    GA_PrimitiveGroup *process,
	    GA_PrimitiveGroup *marked_polys,
	    GA_PointGroup *marked_pts,
	    GA_PrimitiveGroup *prims_to_remove,
	    GA_PointGroup *pts_to_remove,
	    GU_Detail *gdp);

    GU_API extern void processConsecutiveDoubleVertices(
	    GU_Detail *gdp,
	    gu_Action actn, 
	    const GA_PrimitiveGroup *process,
	    GA_PrimitiveGroup *marked_polys,
	    GA_PointGroup     *marked_pts,
	    GA_PrimitiveGroup *prims_to_remove);

    GU_API extern void processIllFormed(
	    GU_Detail *gdp,
	    gu_Action ill_formed_actn,
	    GA_PrimitiveGroup *process,
	    GA_PrimitiveGroup *marked_polys,
	    GA_PointGroup *marked_pts,
	    GA_PrimitiveGroup *prims_to_remove);

    GU_API extern void processDisconnectedPts(GU_Detail *gdp,
	    GA_PointGroup *marked_pts);

    GU_API extern void processSelfIntersecting(GU_Detail *gdp,
	    fpreal intersection_threshold,
	    const GA_PrimitiveGroup *process,
	    GA_PrimitiveGroup *marked_polys,
	    GA_PointGroup *marked_pts);

    GU_API extern void processManyEdges(gu_Action many_edges_actn,
	    GA_PrimitiveGroup *process,
	    GA_PrimitiveGroup *marked_polys,
	    GU_Detail *gdp);

    GU_API extern void processNonConvex(int actn, GA_PrimitiveGroup *process,
	    GA_PrimitiveGroup *marked_polys,
	    GA_PointGroup *marked_pts, GU_Detail *gdp);

    GU_API extern void processOverlapping(const GA_PrimitiveGroup *process,
	    GA_PrimitiveGroup *prims_to_remove,
	    bool pairoverlaps, GU_Detail *gdp);

    GU_API extern void processTopology(GA_PointGroup *marked_pts,
	    GA_PointGroup *repaired_pts,
	    gu_Action nonmanifold_pts_actn,
	    bool prefer_large_angles,
	    bool ignore_windings,
	    bool visualize_maximal_manifolds,
	    bool export_manifold_numbers,
	    bool fix_windings, int min_manifold_size,
	    GU_Detail *gdp, UT_String &msg);
}


#endif
