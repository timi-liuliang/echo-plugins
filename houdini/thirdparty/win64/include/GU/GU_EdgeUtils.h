/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library.
 *
 * COMMENTS:	This file contains useful edge related tools.
 *
 */

#pragma once

#ifndef __GU_EdgeUtils_h__
#define __GU_EdgeUtils_h__

#include "GU_API.h"
#include <GA/GA_Edge.h>
#include <GA/GA_EdgeSet.h>
#include <GA/GA_ElementWrangler.h>
#include <UT/UT_Map.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>
#include <stddef.h>

class GEO_Face;
class GEO_PrimPoly;
class GEO_Detail;
class GA_EdgeGroup;
class GA_PointGroup;
class GA_PrimitiveGroup;
class GU_Detail;

class GU_API GU_EdgeDiv
{
public:
    GU_EdgeDiv(const GEO_PrimPoly &poly, const GA_Edge &edge, int numcuts,
               float fraction) :
        myPoly(poly), myEdge(edge), myNumCuts(numcuts), myFraction(fraction)
    {
        UT_ASSERT(SYSisLessOrEqual(fraction * numcuts, 1.0f));
    }

    const GEO_PrimPoly &myPoly;
    GA_Edge myEdge;
    int myNumCuts;
    float myFraction;
};

////////////////////////////////////////////////////////////////////
// Useful edge-related functions

// Computes the position resulting from insetting vertex at index "vtx" of
// "face" by an amount of "inset".
GU_API extern UT_Vector3 GUcomputeInsetPos(const GEO_Face &face, int vtx,
	                                   float inset);

// Returns true if the three points are collinear and false otherwise.
GU_API extern bool	 GUisPointInline(const UT_Vector3 &start,
					 const UT_Vector3 &mid,
					 const UT_Vector3 &end, float tol);

// Gets the points corresponding to edge edgenum in face.  Returns true if 
// successful, and false otherwise.  A true return status does not imply that 
// both p0 and p1 are valid.
GU_API extern bool	 GUgetEdgePoints(const GEO_Face &face, int edgenum,
					 GA_Offset &p0, GA_Offset &p1);

/// Iterates through the primitives of the detail, calling edgeApply
/// on each primitive to count the number of times each (unordered) edge is
/// present, and adds any edges that appear only once.
/// If prims_on_edges is true, each edge in the edge group will
/// reference the primitive it's associated with.
/// WARNING: A lot of things can go bonkers if you use prims_on_edges!
///          For example, toggling the edge group will result in a
///          group that contains all of the original edges plus
///          every edge, so more edges than the total number.
GU_API void GUfindUnsharedEdges(
    const GEO_Detail &detail,
    GA_EdgeGroup &edges,
    const GA_PrimitiveGroup *primgroup = NULL,
    bool prims_on_edges = false);

/// Iterates through the primitives of the detail, calling edgeApply
/// on each primitive to count the number of times each (unordered) edge is
/// present, and adds any edges that appear only once, in the direction
/// they appear.
/// WARNING: GA_DirectedEdgeSet is an unordered set, so iteration order
///          can't be relied upon if you use that version.
/// @{
GU_API void GUfindUnsharedEdges(
    const GEO_Detail &detail,
    GA_DirectedEdgeSet &edges,
    const GA_PrimitiveGroup *primgroup = NULL);
GU_API void GUfindUnsharedEdges(
    const GEO_Detail &detail,
    UT_Array<GA_DirectedEdge> &edges,
    const GA_PrimitiveGroup *primgroup = NULL);
/// @}

// convenience function to split an edge loop on the given detail with the
// loops(s) specified by pt0, pt1 and either u, or if splits is non-zero, splits
// number of edge loops evenly spaced along the edge, or if parallel is true
// then it is an equal edge length split
// This is also an example of how to calculate the edge loop and then apply it
// if you want to do it yourself elsewhere
// warningCoords is filled with coordinates of points that are clamped when 
// a parallel loop hits the edge of the defining loop
GU_API void GUedgeLoopSplit(GU_Detail &gdp, GA_Offset pt0, GA_Offset pt1,
	fpreal u, exint splits = 0, bool parallel = false,
        GA_EdgeGroup *edgeGroup = NULL,
        UT_Fpreal32Array* warningCoords = NULL);

// utility class to help identify individual splits when splitting a new edge
// loop - used by topobuild state and the polysplit sop
class GU_API GU_LoopFinder
{
public:
    // pt0 pt1 defines the edge which the edge loop will be generated from
    GU_LoopFinder(const GU_Detail &gdp, GA_Offset pt0, GA_Offset pt1);

    const UT_Array<GA_Edge> &getSplits() const { return mySplits; }

    // cu is the edge percentage to create a single loop at. Ignored if
    // splits > 0
    // splits is the number of loops to create. Values > 0 force the positions
    // pts0 is the starts of the edges
    // pts1 is the ends of the new edges
    // pos is the positions of the new points 
    // us is the list of edge percentages of the new points
    // pts0, pts1, pos, and us are to be passed in as newly initialized
    // structures which are filled in by this method. Each element of pts0 is
    // paired with an element of pts1. If the pts1 element is GA_INVALID_INDEX,
    // then the point has already been created by a previous split edge, and the
    // pts0 element is just the index of that point. pos and us only contain an
    // element for each non-GA_INVALID_INDEX value in pts1, and have been
    // compacted, so a separate index needs to be maintained while iterating
    // over these arrays.
    void findAllLoops(fpreal cu, exint splits, GA_IndexArray &pts0,
	GA_IndexArray &pts1, UT_Vector3Array &pos, UT_FprealArray &us) const;
    // similare to the previous funcion, but follows one side of the defining
    // geometry. dist is the distance along each edge to cross at, and 
    // warningCoords will be filled with a list of positions which are collided
    // with when the distance is larger than the edge being crossed. This can
    // be used to render these points as warnings in MSS files.
    void findAllParallelLoops(fpreal dist, exint splits, GA_IndexArray &pts0,
	GA_IndexArray &pts1, UT_Vector3Array &pos, UT_FprealArray &us,
        UT_Fpreal32Array* warningCoords = NULL) const;

private:
    void split(const GEO_PrimPoly *poly, exint i);
    void splitH15_5(const GEO_PrimPoly *poly, exint i);
    bool getManifoldQuads(GA_Offset pt0, GA_Offset pt1,
			  GA_Offset &vtx0, GA_Offset &vtx1,
			  GA_OffsetArray &scratch) const;

    const GU_Detail &myGdp;
    // list of edge pairs defining polygons being split
    UT_Array<GA_Edge> mySplits;
};

// utility class to actually change the geometry of the detail to add splits
// used by the topobuild and polysplit sops
class GU_API GU_PolySplitHelper
{
public:
    GU_PolySplitHelper(GU_Detail &gdp);

    GA_Offset splitEdge(GA_Offset pt0, GA_Offset pt1, fpreal u);
    void splitPolygons(GA_Offset pt0, GA_Offset pt1);

private:
    GU_Detail &myGdp;
    GA_PointWrangler myPointWrangler;
    GA_PrimitiveWrangler myPrimWrangler;
    GA_VertexWrangler myVtxWrangler;
};

GU_API void GUgetEdges(UT_Set<GA_Edge> &edges, const GA_PrimitiveGroup &grp);
GU_API void GUgetEdges(UT_Set<GA_Edge> &edges, const GA_PointGroup &grp);

// utility function that will move connected points into circles
// only loops containing points participating in two edges will be moved
GU_API void GUcircleEdges(UT_Map<GA_Offset, UT_Vector3> &edits,
			  const GU_Detail &gdp,
			  const UT_Set<GA_Edge> &gaedges,
			  const fpreal *radius=nullptr);

// utility function that tries to make all connected edges the same length
// each connected island edges will move towards its own target length
GU_API void GUevenlySpaceEdges(UT_Map<GA_Offset, UT_Vector3> &edits,
			       const GU_Detail &gdp,
			       const UT_Set<GA_Edge> &gaedges);

// utility function that will project connected points along a line
// only points participating in two edges will be moved
GU_API void GUstraightenEdges(UT_Map<GA_Offset, UT_Vector3> &edits,
			      const GU_Detail &gdp,
			      const UT_Set<GA_Edge> &gaedges,
			      const UT_Vector3 *constrain_nml);

// utility function to identify vertices along bridge paths
GU_API bool GUgetBridgePaths(GA_OffsetArray &path0,
			     GA_OffsetArray &path1,
			     const GU_Detail &gdp,
			     const UT_Set<GA_Edge> &gaedges);

// returns a list of points found walking along unshared edges
GU_API void GUgetUnsharedPath(GA_OffsetArray &path,
			      const GU_Detail &gdp,
			      GA_Offset start,
			      GA_Offset end);

// creates bridge polygons
GU_API void GUbridgeEdges(GU_Detail &gdp,
			  const UT_Set<GA_Edge> &gaedges,
			  bool connected);

#endif
