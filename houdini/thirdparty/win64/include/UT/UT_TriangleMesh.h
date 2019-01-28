/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TriangleMesh.h (UT Library, C++)
 *
 * COMMENTS: A class to represents a constant (unmodifiable) abstract
 * (topological) triangulation of a 2-manifold (surface) possibly with
 * boundary, providing the usual traversal methods. Note that no geometric
 * information is kept in this class.
 *
 * Be particularly careful about what a manifold "boundary point" is! The
 * neighbourhood of a boundary point must be homeomorphic to a half-disc
 * (in particular, the neighbourhood minus the point itself must be connected),
 * meaning pinched boundaries (such as what you get from two triangles sharing
 * a single point) cannot be represented by this class.
 *
 */

#ifndef __UT_TriangleMesh__
#define __UT_TriangleMesh__

#include "UT_API.h"
#include "UT_ValArray.h"


// We use the usual Houdini terminology of "points", "vertices", and half-edges
// (hedges for short): a vertex belongs to a single triangle and is mapped to a
// point which exists independently. A half-edge is the share of a single
// triangle of an edge and is oriented consistently with triangle's winding
// with a source and a destination point (and vertex). A neighbouring triangle
// with sharing the same edge has its own corresponding half-edge with the same
// endpoints. The two half-edges are said to be *equivalent* and point to each
// other through the sym() method. Boundary half-edges have a sym() of -1.
// Manifold requirement dictates that each non-boundary half-edge has exactly
// only sym() which in turn would have that half-edge as its sym(). Furthermore,
// the source of a half-edge is always the destination of its sym() and
// vice versa.
//
// Triangles are numbered between 0 and numTriangles() - 1, points are numbered
// 0 to numPoints() - 1, and vertices (or equivalently half-edges) are numbered
// 0..numHedges() - 1. Note that numHedges() is the same as numVertices() and
// is 3 * numTriangles().
//
// NB. Any correspondence between elements of the mesh and the world outside
// should be tracked externally and should never be added to this class!

class UT_API UT_TriangleMesh
{
public:
    			 UT_TriangleMesh() = default;

    // Build a triangulation from a list of triangle points. The length
    // of tri_pts must be a multiple of 3 and should represent a manifold
    // surface. Triangle edges with the same endpoints are identified as
    // paired half-edges, unless forced apart by passing "boundary_edge_pts",
    // which is an array of even length broken into pairs of consecutive point
    // numbers which if represent as an edge of a triangle should not be
    // identified with any other triangle edges and therefore forced to be
    // boundary edges.
    //
    // The created triangle mesh will have as many triangles as in tri_pts.
    // The number of points will be one plus the largest integer in the
    // tri_pts array. Note that there can be unused points if tri_pts array
    // does not cover a contiguous list starting from 0. In that case, if
    // reindex_points is set to true, the points are re-indexed to to the
    // range 0 .. num_points - 1. The index of a point will agree with its
    // rank (or rather the rank of its first occurrence in tri_pts).

    void		 build(const UT_IntArray &tri_pts,
				const UT_IntArray *boundary_edge_pts = nullptr,
			    	bool reindex_points = false,
			    	const UT_IntArray *tri_hedge_grp = nullptr);

    SYS_FORCE_INLINE
    int		 	 numTriangles() const
    			    { return int(myVertexPoint.size()) / 3; }

    SYS_FORCE_INLINE
    int		 	 numPoints() const
    			    { return int(myPointVertex.size()); }

    SYS_FORCE_INLINE
    int		 	 numHedges() const { return int(myVertexPoint.size()); }

    SYS_FORCE_INLINE
    int		 	 numVertices() const { return numHedges(); }

    // The jth hedge of triangle t.
    SYS_FORCE_INLINE
    int	 	 	 triangleHedge(int t, int j = 0) const
			    { return 3 * t + j % 3; }

    // The jth vertex of triangle t (same as jth hedge).
    SYS_FORCE_INLINE
    int	 	 	 triangleVertex(int t, int j = 0) const
			    { return triangleHedge(t, j); }

    // Triangle of hedge h.
    SYS_FORCE_INLINE
    int	 	 	 hedgeTriangle(int h) const { return h / 3; }

    // Triangle of vertex v.
    SYS_FORCE_INLINE
    int	 	 	 vertexTriangle(int v) const
    			    { return hedgeTriangle(v); }

    // The jth point of triangle t.
    SYS_FORCE_INLINE
    int	 	 	 trianglePoint(int t, int j = 0) const
			    { return myVertexPoint(3 * t + j % 3); }

    SYS_FORCE_INLINE
    int	 	 	 srcVertex(int h) const { return h; }

    SYS_FORCE_INLINE
    int	 	 	 dstVertex(int h) const
			    { return (h % 3 == 2) ? h - 2 : h + 1; }

    SYS_FORCE_INLINE
    int	 	 	 apxVertex(int h) const
			    { return (h % 3 == 0) ? h + 2 : h - 1; }

    SYS_FORCE_INLINE
    int	 	 	 onext(int h) const { return sym(lprev(h)); }

    SYS_FORCE_INLINE
    int	 	 	 srcPoint(int h) const
    			    { return myVertexPoint(srcVertex(h)); }

    SYS_FORCE_INLINE
    int	 	 	 dstPoint(int h) const
    			    { return myVertexPoint(dstVertex(h)); }

    SYS_FORCE_INLINE
    int	 	 	 apxPoint(int h) const
    			    { return myVertexPoint(apxVertex(h)); }

    SYS_FORCE_INLINE
    int	 	 	 lnext(int h) const { return dstVertex(h); }

    SYS_FORCE_INLINE
    int	 	 	 lprev(int h) const { return apxVertex(h); }

    SYS_FORCE_INLINE
    int	 	 	 sym(int h) const { return myHedgeSym(h); }

    SYS_FORCE_INLINE
    int			 pointVertex(int p) const { return myPointVertex(p); }

    SYS_FORCE_INLINE
    int			 pointHedge(int p) const { return myPointVertex(p); }

    SYS_FORCE_INLINE
    bool		 isBoundaryHedge(int h) const { return sym(h) == -1; }

    SYS_FORCE_INLINE
    bool		 isBoundaryPoint(int p) const
    			    { return isBoundaryHedge(pointHedge(p)); }

    // Number of boundary components
    SYS_FORCE_INLINE
    int 		 numBoundaries() const
    			    { return int(myBoundaryRepHedges.size()); }

    // Return a first (representative) hedge for component comp.
    SYS_FORCE_INLINE
    int			 boundaryHedge(int comp) const
    			    { return myBoundaryRepHedges(comp); }

    // Get all the boundary component hedges in order.
    void		 traceBoundary(int h0, UT_IntArray &bd) const;

    int			 numInteriorHedges() const
    			    { return myNumInteriorHedges; }

    int			 numBoundaryHedges() const
    			    { return numHedges() - numInteriorHedges(); }

    int			 numBoundaryPoints() const
    			    { return numBoundaryHedges(); }

    int			 numInteriorPoints() const
    			    { return numPoints() - numBoundaryPoints(); }

private:
    UT_IntArray		 myVertexPoint;	// Points of triangles (3 per tri)
    UT_IntArray		 myHedgeSym;	// Syms of triangle hedges (3 per tri)
    UT_IntArray		 myPointVertex;	// First hedge of each point

    // One half-edge representing each boundary component.
    UT_IntArray		 myBoundaryRepHedges;
    int 		 myNumInteriorHedges 			= 0;
};

#endif

