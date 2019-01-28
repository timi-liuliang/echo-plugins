/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PolyFill.h (GU Librayr, C++)
 *
 * COMMENTS: Caps polygons
 *
 */

#ifndef __GU_PolyFill_h__
#define __GU_PolyFill_h__

#include "GU_API.h"

#include <GA/GA_Types.h>
class GU_Detail;
class GA_PrimitiveGroup;
class GA_PointGroup;

namespace GU_PolyFill
{

// The following are some bitmasks for a couple of the methods which can fail.
// fan can return ODD_SIZED_HOLE
// quadFan can return ODD_SIZED_HOLE and CONFLICTING_CORNERS
// quads can return ODD_SIZED_HOLE and NO_PLANE_NORMAL
// perfectGrid can return ODD_SIZED_HOLE, ONLY_ONE_CORNER, CONFLICTING_CORNERS
    enum PolyFillReturnCodes
    {
        ODD_SIZED_HOLE         = 0x1,
        NO_PLANE_NORMAL        = 0x2,
        ONLY_ONE_CORNER        = 0x4,
        CONFLICTING_CORNERS    = 0x8
    };

/*
 * README
 * The holes that will be patched are determined by rings. Each GA_OffsetArray
 * will be treated as a list of point offsets that describe a closed polygon
 * (the hole)  which is the surface of the patch. Attributes will be
 * interpolated based on the geometry surrounding the hole. Note that each pair
 * of consecutive points don't need to correspond to a real edge. For example,
 * when auto-complete boundaries is off, one of the edges won't exist.
 * Theoretically the code might be able to handle a polygone consisting
 * entirely of free floating points, but this has never been tested in any way
 * at all, and there are some snags which are noted in the code. Use with
 * caution. Everything should work if all edges exist though.
 * The distinction between rings and ringOrigs is that the generated polygons
 * will be attached to the points in rings, but tangents for surface matching
 * and attributes for interpolating will be taken from the geometry surrounding
 * ringOrigs. This is useful for when you want to have the PolyFill be attached
 * to unique points.
 * If you want to reverse the orientation of the generated polygons, simply
 * reverse each GA_OffsetArray in rings and ringOrigs.
 *
 * For functions that accept both a patch group and a loopgroup, the diagram 
 * below shows which polygons will be put into each group
 * p - patch group
 * l - loop group
 * e - pre-existing geometry (not in any group)
 *
 *         :     :    :
 *    e    :  e  : e  :   e
 * _ _ _ __|_____|____|__ _ _ _
 *         |\__l_|_l_/|
 *    e    |l| p | p|l|   e
 * _ _ _ __|_|___|__|_|__ _ _ _
 *         | | p | p| |
 *    e    |l|___|__|l|   e
 * _ _ _ __|/__l_|_l_\|__ _ _ _
 *         |     |    |
 *    e    :  e  : e  :   e
 *         :     :    :
 *
 * For functions with cornerOffset, the topology that's generated will be
 * rotated by the given amount around the ring. 
 *
 * For functions with customCorners, this allows for defining the corners of 
 * quads explicitly instead of letting the function choose what it wants. If a
 * hole does not have any boundary points in the group, automatic choosing will
 * still be used. If conflicting corners are input (for example three points in
 * a row on a perfect grid of boundary size ten), the return value will set
 * CONFLICTING_CORNERS high.
 *
 * For functions with an edgeloop, the bool is for whether or not to add one.
 * If this is false, the loopgroup will always be empty.
 * Otherwise a point is inserted at percentage along each edge for points on
 * the hole boundary with an even number of adjacent polygons in the patch. For
 * boundary points with an odd number of adjacent polygons, the middle polygon
 * is evaluated at percentage into it's surface. Note that since this is only
 * currently possible with quad-generating fill modes, so if you add a fill
 * mode that breaks this assumption, you will need to visit sop_makeedgeloop in
 * GU_PolyFill.C
 * The percentage must be in the range [0,1].
 *
 * For functions with a smooth parameter, the bool determines whether or not to
 * pass the patch group to the smooth sop with default parameters except for
 * the given strength. The smoothing occurs before edge loop generation so that
 * the edge loop appears to be on the surface of the patch.
 *
 * For functions with a ctrTranslation parameter, deform patch determines
 * whether or not the center points should translated along the hole's normals
 * by the given amount. perfectGrid does something a bit more complex, see the
 * GU_NSidedSubdivPatch it uses for more information.
 */
    GU_API void singlePolys(
            GU_Detail* gdp,
            UT_Array<GA_OffsetArray>& rings,
            UT_Array<GA_OffsetArray>& ringOrigs,
            GA_PrimitiveGroup* patchGroup);

    // builds a fan out of polygons with degree edges each. Returns
    // ODD_SIZED_HOLE if any of the holes could not be filled because they were
    // not the correct size (for example a ten sided hole with hexagons). In
    // this case any holes that were the correct size will still be filled.
    GU_API exint fan(
            GU_Detail* gdp,
            UT_Array<GA_OffsetArray>& rings,
            UT_Array<GA_OffsetArray>& ringOrigs,
            GA_PrimitiveGroup* patchgroup = nullptr,
            GA_PrimitiveGroup* loopgroup = nullptr,
            exint degree = 3,
            bool deformpatch = false,
            fpreal ctrTranslation = 0.0,
            bool edgeloop = false,
            fpreal edgelooppercentage = 0.5);

    // handles the customcorner checking by rotating the rings, and then passes
    // it off to fan() to finish the job
    // can return ODD_SIZED_HOLE and CONFLICTING_CORNERS
    GU_API exint quadFan(
            GU_Detail* gdp,
            UT_Array<GA_OffsetArray>& rings,
            UT_Array<GA_OffsetArray>& originalRings,
            GA_PrimitiveGroup* patchgroup = nullptr,
            GA_PrimitiveGroup* loopgroup = nullptr,
            GA_PointGroup* customcorners = nullptr,
            exint cornerOffset = 0,
            bool deformPatch = false,
            fpreal ctrTranslation = 0.0,
            bool edgeloop = false,
            fpreal edgeloopPercentage = 0.5);

    // uses a greedy algorithm to fill holes as a human might
    // can return ODD_SIZED_HOLE and NO_PLANE_NORMAL
    GU_API exint quads(
            GU_Detail* gdp,
            UT_Array<GA_OffsetArray>& rings,
            UT_Array<GA_OffsetArray>& originalRings,
            GA_PrimitiveGroup* patchgroup,
            GA_PrimitiveGroup* loopgroup,
            bool smooth,
            fpreal smoothstrength,
            bool edgeloop,
            fpreal edgeloopPercentage);

    // creates a topologically perfect grid patch by interpolating rows and
    // columns across the hole. When deformpatch is true, it creates a
    // GU_NSidedSubdivPatch which guarantees a smooth patch. tanstrength can be
    // tuned to make it smooth with the surrounding geometry as well. Note that
    // there can be unfortunate stretching of the outer ring of polygons when
    // tanstrength is too high.  This could be made to scale somehow with the
    // surrounding geometry size, but when I tried I couldn't find any
    // reasonable, easy to understand methods.
    // can return ODD_SIZED_HOLE, ONLY_ONE_CORNER, CONFLICTING_CORNERS
    GU_API exint perfectGrid(
            GU_Detail* gdp,
            UT_Array<GA_OffsetArray>& rings,
            UT_Array<GA_OffsetArray>& originalRings,
            GA_PrimitiveGroup* patchgroup,
            GA_PrimitiveGroup* loopgroup,
            GA_PointGroup* customcorners,
            exint corneroffset,
            bool smooth,
            fpreal smoothstrength,
            bool edgeloop,
            fpreal edgeloopPercentage,
            bool deformPatch,
            fpreal ctrTranslation,
            fpreal tanstrength);
} // namespace GU_PolyFill
#endif
