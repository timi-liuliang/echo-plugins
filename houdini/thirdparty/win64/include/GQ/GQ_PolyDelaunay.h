/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Delaunay triangulation tool
 *
 */

#ifndef __GQ_PolyDelaunay_h__
#define __GQ_PolyDelaunay_h__

#include "GQ_API.h"
#include <GA/GA_Edge.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Pair.h>
#include <UT/UT_Array.h>
#include <UT/UT_String.h>
#include <UT/UT_Vector3.h>

class GU_Detail;
class GQ_Detail;
class GQ_Point;
class GQ_Edge;
class GQ_Face;
class GA_Primitive;
class GA_EdgeGroup;
class GA_PrimitiveGroup;
class GA_PointGroup;

/// These internal helper classes are named generically enough that they could
/// potentially conflict with classes in other files.  We don't want to expose
/// their implementation, and we can't forward declare embedded helper classes,
/// so we resort to wrapping them in a name space as the next best thing.
namespace GQ_PolyDelaunayImpl {
    class gqTriangleSplitNode;
    class gqFaceQueue;
    class gqGBEdgeGroup;
    class gqEdgeQueue;
}

/// Delaunay triangulation (and refinement) of a set of 2D points.
///
/// The algorithm used is a randomized incremental algorithm, as presented in
///   de Berg, van Kreveld, Overmars and Schwarzkopf. "Computational Geometry",
///   2nd edition, 1999.
///
/// It's adapted from:
///   Guibas, Knuth and Sharir. "Randomized incremental construction of
///     Delaunay and Voronoi diagrams." Algorithmica, 7:381-413, 1992.
///
/// Most of it follows the earlier paper:
///   Guibas and Stolfi. "Primitives for the Manipulation of General
///     Subdivisions and the Computation of Voronoi Diagrams", ACM Transactions
///     on Graphics, 4(2):74-123, April 1985.
/// except for the data structure for determining which triangle a point is
/// in.
///
///
/// After triangulation is complete, an optional Delaunay refinement
/// process is applied. This follows
///   Shewchuk. "Delaunay Refinement Algorithms for Triangular Mesh
///     Generation". Computational Geometry: Theory and Applications
///     22(1-3):21-74, May 2002.
///     http://www.cs.berkeley.edu/~jrs/papers/2dj.ps
///
/// Shewchuk describes a number of algorithms; I've implemented the
/// pseudocode from pages 47-48. I used the Terminator algorithm, using
/// concentric circles instead of diametral lenses. I haven't implemented
/// one part of his algorithm: the key "terminator" innovation, his
/// splitPermitted routine.
///
/// A rough overview of the algorithm can be found at:
/// 	http://www-2.cs.cmu.edu/~quake/tripaper/triangle3.html	
///
/// Shewchuk's main inspiration is this paper:
///
///   Ruppert. "A Delaunay Refinement Algorithm for Quality 2-Dimensional
///     Mesh Generation", Journal of Algorithms 18(3):548-585, May 1995.
///
///
///
/// TODO: this could be extended to work with 3D points under a 2D
/// parameterization (i.e., use texture co-ordinates instead of positions).
/// I'm not sure how we would handle u-wrap or v-wrap, though...
/// There's some discussion of this problem in
///   Chew, L. Paul. Guaranteed-Quality Mesh Generation for Curved
///     Surfaces. Proceedings of the 9th Annual Symposium on Computational
///     Geometry, pages 274-280. ACM, May 1993.
///
/// Better yet, we could do what Shewchuk describes in section 7.1 of his
/// paper: triangulate a set of 3D primitives independently, but keeping
/// track of insertions on the boundaries that are shared by multiple
/// primitives. Apparently, it would require a way of calculating geodesic
/// distance between points on different primitives. I'm sure other changes
/// to my algorithm would be required too.
///
///
/// Notes:
/// * We use the GA_Edge class unconventionally here. Constraint edges
///   are stored as GA_Edges in a GA_EdgeGroup. The endpoint order matters:
///   the region to the right of p0->p1 is in the "interior" of the
///   constraint. The optional "primitive" member of the GA_Edge points to
///   the constraint primitive. However, the primitive member will be null
///   for constraints that have no holes (i.e., have no bridge edges). This
///   signals that there is no need to remove triangles from the inside of
///   the hole.

class GQ_API GQ_PolyDelaunay
{
public:
    /// Some local typedefs for our helper classes to avoid having to fully
    /// qualify the names everywhere.
    typedef GQ_PolyDelaunayImpl::gqTriangleSplitNode gqTriangleSplitNode;
    typedef GQ_PolyDelaunayImpl::gqFaceQueue gqFaceQueue;
    typedef GQ_PolyDelaunayImpl::gqGBEdgeGroup gqGBEdgeGroup;
    typedef GQ_PolyDelaunayImpl::gqEdgeQueue gqEdgeQueue;

    explicit	     GQ_PolyDelaunay(GU_Detail *gdp);
    virtual	    ~GQ_PolyDelaunay();

    /// Set the plane where triangulation happens.
    void	     setPlane(const UT_Vector3 &normal, fpreal distance);
    /// Fit a plane to the input points. Returns true if it succeeded.
    bool	     fitPlane(const GA_PointGroup *pointGroup = 0,
			      UT_Vector3 *outNormal = 0,
			      fpreal *outDistance = 0);

    void             setRemoveDuplicatePoints(bool value)
	             { myRemoveDuplicatePoints = value; }

    /// This one is mostly here for debugging - if enabled, we don't
    /// delete the bounding triangle that's added during construction.
    void	     setKeepBoundingTriangle(bool value)
		     { myKeepBoundingTriangle = value; }
    /// If enabled, delete all edges that are outside the constrained
    /// edges.
    void	     setRemoveOutsideConstraints(bool value)
		     { myRemoveOutsideConstraints = value; }
    /// Allow new points - e.g., constraint edges are split to maintain
    /// the Delaunay property.
    void	     enableNewConstraintPoints(bool enabled)
		     { myNewConstraintPoints = enabled; }
    // if the new point group is set by setConstraintNewPointGroup, then
    // setting the group name is ignored
    void	     setConstraintNewPointGroupName(const UT_String &name)
		     { myConstraintNewPointGroupName.harden(name); }
    void	     setConstraintNewPointGroup(GA_PointGroup *new_pt_grp)
		     { myConstraintNewPointGroup = new_pt_grp; }
    /// Allow refinement.
    void	     enableRefinement(bool enabled)
		     { myRefinement = enabled; }
    void	     setMaxNewPoints(int maxNewPoints)
		     { myMaxNewPoints = maxNewPoints; }
    /// Set refinement criteria. Any triangle that fails the maximum area
    /// test will be refined by inserting a new Steiner point.
    /// To disable this test, set to <= 0.
    void	     setMaxArea(fpreal maxArea)
		     { myMaxArea = maxArea; }
    /// Set refinement criteria. Any triangle that fails the minimum angle
    /// test will be refined by inserting a new Steiner point.
    /// To disable this test, set to <= 0.
    ///
    /// Warning: high values can cause this to halt. Shewchuk says that
    /// there's a proof that it works for below 21 degrees, and he finds it
    /// works below 33, but he's had problems with higher values.
    void	     setMinAngle(fpreal minAngle)
		     { myMinAngle = minAngle; }

    void	     triangulate(const GA_PointGroup *pointGroup,
				 const GA_PrimitiveGroup *constrainedPrimGroup);
    void	     buildGeometry(bool updatePointNormals = true,
				   GA_PrimitiveGroup *out_grp = NULL);

    /// Project 3D point onto plane.
    UT_Vector2	     make2D(const UT_Vector4 &pos) const;
    UT_Vector4	     make3D(const UT_Vector2 &pos) const;

private:
    enum gqInsertStatus
    {
	GQ_Success,
	GQ_DuplicateVertex,
	GQ_OutsideBounding
    };

    /// A journal of events during a point insertion. It is used to allow
    /// the point insertion to be undone.
    struct gqJournal
    {
	typedef UT_Pair<GA_Index,GA_Index> gqEdgePointIds;
	typedef UT_Pair<gqEdgePointIds, GA_Primitive *> gqConstraint;

	/// The sequence of edge flips performed, from first to last.
	/// The point ids are from after the flip.
	UT_Array<gqEdgePointIds>  myEdgeFlips;
	/// The constraints that have been added and removed.
	UT_Array<gqConstraint>    myConstraintsAdded;
	UT_Array<gqConstraint>    myConstraintsRemoved;

	/// If this point was inserted on an existing edge, these are the
	/// point ids of that original edge.
	gqEdgePointIds		     myOnEdge;
    };

    /// Set up the bounding triangle, myGqd, and the locating DAG.
    void	     setup();
    /// Set up my*Transform member variables from the plane
    /// normal/distance.
    void	     calcTransforms(const UT_Vector3 &normal, fpreal distance);
    /// Check the constraint edges to see if any of their endpoints match
    /// an old point, and edit them to reference the new point if they
    /// do. The "old point" can be defined either by its position (with
    /// *no* tolerance), or by its id.
    ///
    /// Very inefficient.
    enum gq_ChangeConstraintType
    {
	gq_ByPos,
	gq_ByPointId
    };
    void	     changeConstraints(gq_ChangeConstraintType type,
				       const UT_Vector2 &oldPos,
				       int oldPointId,
				       GA_Offset newPoint,
				       gqGBEdgeGroup &constrainedEdges,
				       gqJournal *journal) const;
    /// Part of the core triangulation algorithm. Insert a new point into
    /// the existing triangulation.
    /// If the point is intended to lie on an existing edge (e.g.,
    /// splitting an existing edge), the "onEdge" parameter can be used to
    /// indicate that.
    gqInsertStatus   insertPoint(int pid, GQ_Edge *onEdge,
				 gqGBEdgeGroup &constrainedEdges,
				 gqJournal *journal);
    /// Undo an insertion.
    void	     undoInsertPoint(int pid, gqGBEdgeGroup &constrainedEdges,
				     const gqJournal &journal);
    /// Flip a single edge. Does not update the point location data
    /// structure.
    void	     flipEdge(GQ_Edge &edge, bool updateDAG);
    /// Part of the core triangulation algorithm. Parameter point has just
    /// been inserted, and edge is being tested to see if it's legal
    /// according to the Delaunay criterion; if not, the edge is flipped
    /// and we recurse.
    void	     legalizeEdge(const GQ_Point &point, GQ_Edge &edge,
				  gqGBEdgeGroup &constrainedEdges,
				  gqJournal *journal);
    /// After triangulation, ensure that all constraints are met.
    /// Returns false if constraints are contradictory and cannot be
    /// enforced.
    bool	     enforceConstraints(gqGBEdgeGroup &constrainedEdges);
    /// Remove the bounding triangle
    void	     removeBoundingTriangle();
    /// Remove outside edges that aren't in the given group.
    void	     removeOutsideEdges(const gqGBEdgeGroup &constrainedEdges);
    /// Remove edges that are inside holes.
    void	     removeHoles(const gqGBEdgeGroup &constrainedEdges);
    /// Destroy all edges in queue, and recursively destroy any
    /// neighbouring edges that are not constraints. This is Shewchuk's
    /// "triangle eating virus" used to remove edges outside the boundary
    /// and inside holes.
    /// For holes, it will leave one large non-triangular face after
    /// completion.
    void	     eatEdges(gqEdgeQueue &queue,
			      const gqGBEdgeGroup &constrainedEdges);
    /// Insert new points to satisfy refinement criteria.
    /// Returns true if the insertion ran successfully.  False otherwise.
    bool	     refine(gqGBEdgeGroup &constrainedEdges);
    /// Calculate stats related to refinement of a face.
    void	     calcFaceStats(const GQ_Face &face,
				   fpreal &area,
				   const gqGBEdgeGroup *constrainedEdges,
				   fpreal *minAngle) const;
    bool	     isFaceTooLarge(const GQ_Face &face) const;
    /// Test if a given face is "bad" - i.e., excessive area, or too small
    /// an angle.
    bool	     isFaceBad(const GQ_Face &face,
			       const gqGBEdgeGroup &constrainedEdges) const;
    /// Test if the given edge is "encroached" - i.e., if the triangles on
    /// side have an obtuse angle opposite to the edge.
    bool	     isEncroached(const GQ_Edge &edge,
				  fpreal tol = 0.05f) const;
    /// Test if the given edge is encroached to its left side only.
    bool	     isEncroachedLeft(const GQ_Edge &edge,
				      fpreal tol = 0.05f) const;
    /// Insert a point to split an non/existing edge at the given fraction
    /// along its length.
    GQ_Point	    *insertSplitPoint(GA_Offset p0, GA_Offset p1,
				      fpreal fraction,
				      gqGBEdgeGroup &constrainedEdges,
				      GQ_Edge *onEdge,
				      bool isConstraintEdge);
    /// Split an existing edge at somewhere-near-the-midpoint.
    GQ_Point	    *splitEdge(GQ_Edge &edge,
			       gqGBEdgeGroup &constrainedEdges);
    /// Split the given existing constraint edge using the given point.
    /// Just updates the constraintEdges group.
    void	     splitConstraint(gqGBEdgeGroup &constrainedEdges,
				     const GA_Edge &gbEdge,
				     GA_Offset newPoint,
				     gqJournal *journal) const;
    bool	     splitEncroachedEdges(gqGBEdgeGroup &constrainedEdges,
					  gqEdgeQueue &edgeQueue,
					  gqFaceQueue *faceQueuePtr);
    void	     updateQueuesAfterNewPoint(const GQ_Point &newPoint,
				const gqGBEdgeGroup &constrainedEdges,
				gqEdgeQueue &edgeQueue,
				gqFaceQueue *faceQueue);
    void	     deletePoint(int pid, gqGBEdgeGroup &constrainedEdges,
				 gqEdgeQueue *edgeQueue,
				 gqFaceQueue *faceQueue);

    /// Verify that myGqd and the locating DAG make sense.
    void	     verify();

    /// Transform from 3D space to plane
    UT_Matrix4	     my3to2Transform;
    /// Transform from plane to 3D space
    UT_Matrix4	     my2to3Transform;
    bool	     myNewConstraintPoints;
    int		     myMaxNewPoints;
    int		     myNumNewPoints;
    bool	     myRefinement;
    bool             myRemoveDuplicatePoints;
    bool	     myKeepBoundingTriangle;
    bool	     myRemoveOutsideConstraints;
    fpreal	     myMaxArea, myMinAngle;
    bool	     myHasBoundingTriangle;
    UT_String	     myConstraintNewPointGroupName;

    GU_Detail	    *myGdp;
    GQ_Detail	    *myGqd;
    GQ_Point	    *myOutsideP[3];
    GA_PointGroup   *myConstraintNewPointGroup;
    /// Root of DAG for doing point->triangle lookups
    gqTriangleSplitNode
		    *myTriSplitRoot;
};

#endif
