/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PolyDelaunay.h
 *
 */

#ifndef __GU_PolyDelaunay_h__
#define __GU_PolyDelaunay_h__

#include "GU_API.h"
#include <UT/UT_FastRandom.h>
#include <UT/UT_GeometryPredicate.h>
#include <UT/UT_Map.h>
#include <UT/UT_StringStream.h>

#include <GEO/GEO_HedgeInterface.h>

#define DELAUNAY_TINY_AREA_TOLERANCE (std::numeric_limits<fpreal>::min())

class GU_Detail;
class GA_EdgeGroup;
class GA_PrimitiveGroup;
class GA_PointGroup;


/// Delaunay triangulation (and refinement) of a set of 2D points.
///
/// The Delaunay triangulation algorithm used is the randomized incremental
/// algorithm, as presented in
///	de Berg, van Kreveld, Overmars and Schwarzkopf.
/// 	"Computational Geometry", 2nd edition, 1999,
/// in turn adapated from:
/// 	Guibas, Knuth and Sharir. "Randomized incremental construction of
/// 	Delaunay and Voronoi diagrams." Algorithmica, 7:381-413, 1992.
///
/// Most ideas are present in the earlier paper:
/// 	Guibas and Stolfi. "Primitives for the Manipulation of General
///     Subdivisions and the Computation of Voronoi Diagrams", ACM Transactions
///     on Graphics, 4(2):74-123, April 1985.
/// except for the point location DAG.
///
/// Constraint enforcement is done through removal of edges that intersect
/// constraints and retriangulating the resulting poligons. The code closely
/// follows the appraoch of Shewchuk in his Triangle package.
///
/// After triangulation is complete, an optional Delaunay refinement
/// process is applied. This follows
///   Shewchuk. "Delaunay Refinement Algorithms for Triangular Mesh
///     Generation". Computational Geometry: Theory and Applications
///     22(1-3):21-74, May 2002.
///     http://www.cs.berkeley.edu/~jrs/papers/2dj.ps
///
/// A good resource for quick lookup is Shewchuk's presentation slides on
/// this subject:
///	http://www.cs.berkeley.edu/~jrs/papers/imrtalk.pdf
///

class GU_API GU_PolyDelaunay
{
public:

    /// Some local typedefs for our helper classes to avoid having to fully
    /// qualify the names everywhere.

    explicit 	 	 GU_PolyDelaunay(GU_Detail *gdp,
             	 	                 uint32 rand_seed = 5678U);

    virtual 		~GU_PolyDelaunay();

    /// Set the plane where triangulation happens.
    void 	 	 setPlane(const UT_Vector3 &normal, fpreal distance);

    /// Fit a plane to the input points. Returns true if it succeeded.
    bool 	 	 fitPlane(const GA_PointGroup *ptgrp = 0,
				UT_Vector3 *out_normal = 0,
				fpreal *out_distance = 0,
				UT_Vector3 *out_center = 0);

    void 	 	 setRemoveDuplicatePoints(bool value)
				{ myRemoveDuplicatePoints = value; }

    /// This one is mostly here for debugging - if enabled, we don't
    /// delete the bounding triangle that's added during construction.
    void 	 	 setKeepBoundingTriangle(bool value)
				{ myKeepBoundingTriangle = value; }

    enum RemoveOutsidePolicy
    {
        REMOVE_NONE = 0,
        REMOVE_IF_OUT,
        REMOVE_IF_OUT_BUT_NOT_IN
    };

    void		 setRemoveFromConvexHull(bool value)
			    { myRemoveFromConvexHull = value; }

    /// If enabled, delete all edges that are outside the constrained
    /// edges.
    void		 setRemoveOutside(bool value,
        		                  bool skip_if_also_inside = false);


    /// Allow new points - e.g., constraint edges are split to maintain
    /// the Delaunay property.
    void 		 allowConstraintSplit(bool enabled)
			    { myCanSplitConstraints = enabled; }

    // if the new point group is set by setConstraintNewPointGroup, then
    // setting the group name is ignored
    void 		 setConstraintSplitPointGroupName(const UT_String &name)
			    { myConstraintNewPointGroupName.harden(name); }

    void 		 setConstraintSplitPointGroup(GA_PointGroup *new_pt_grp)
			    { myConstraintNewPointGroup = new_pt_grp; }

    /// Allow refinement.
    void 		 enableRefinement(bool enabled)
			    { myDoRefinement = enabled; }

    /// Cap the maximum allowed number of new ponts
    void 		 setMaxNewPoints(int maxNewPoints)
			    { myMaxNewPoints = maxNewPoints; }

    /// Whether to keep the triangulation on the projection plane or
    /// backproject it back to the original point positions
    void 		 setRestorePositions(bool val)
			    { myRestorePositions = val; }

    /// Set refinement maximum area criterion. Any triangle with larger area
    /// than this is subdivided. To disable, set to <= 0.0.
    void 		 setMaxArea(fpreal max_area);

    /// Set refinement minmum angle. Any triangle with an angle less than
    /// this is subdivided (modulo satisfying Miller-Pav-Walkington rule)
    /// The argument angle is in radians. To disable this test, set to <= 0.0.
    ///
    /// IMPORTANT: Large minimum angle values drive the refinement to infinity, 
    /// or until the maximum allowed number of points is reached. If minimum
    /// required angle is <= 21 degrees, the algorithm provably terminates.
    /// In practice, the algorithm almost always terminates for minimum
    /// angles of up to about 33 degrees. Beyond that, the algorithm almost
    /// surely goes on for ever or until it hits the set limit.
    void		 setMinAngle(fpreal min_angle);
    void 		 setMinEdgeLength(fpreal min_edge_length);

    /// Perform the triangulation
    void 		 triangulate(const GA_PointGroup *point_group,
				const GA_PrimitiveGroup *constraint_prims,
				const GA_EdgeGroup *constraint_edges);

    void		 copyTriangles(bool updatePointNormals = true,
				GA_PrimitiveGroup *out_grp = NULL);

    const std::string	 getWarningMessage()
			    { return myWarningMessage.str().toStdString(); }

    void 		 getBoundingSquareCorners(UT_Vector3 *corners);
    bool 		 setPositionAttribute(const char *attrib);

    void 		 getProjectedConstrainedEdges(
				UT_Array<UT_Vector3> &endpoints);

    void 		 getEnforcedEdges(GA_EdgeGroup &edge_group);

    /// Query whether or not the triangulation consolidated some duplicate
    /// points.
    bool		 hasNonEmptyDuplicatePointMap() const
				{ return myDuplicateMap.size() != 0; }

    /// Query how the triangulation consolidated duplicate points.
    void		 getDuplicatePointMapForInput(
				UT_Map<GA_Offset, GA_Offset> &map) const;

private:

    typedef UT_Array<GEO_Hedge> GEO_HedgeArray;

    bool 		 verifyMarks();

    template <class T>
    struct MultiQueue
    {
	MultiQueue(int num_queues = 1, int rand_seed = -1, int init_size = -1);
	void 			 append(const T &t, int queue_idx = 0);
	T 			 pick();
	int 			 entries() { return myEntries; }
    public:
	typedef UT_Array<T> 	 SingleQueue;

	int			 myPickQueueIdx;
	int			 myEntries;
	bool 			 myShuffle;
	UT_Array<SingleQueue> 	 myQueues;
	UT_FastRandom 		 myRand;
    };

    struct PinnedEdge
    {
	PinnedEdge(GEO_Hedge e, GA_Offset src, GA_Offset dst) :
	    myHedge(e), mySrc(src), myDst(dst) {}
	GEO_Hedge myHedge;
	GA_Offset mySrc;
	GA_Offset myDst;
    };

    PinnedEdge 		 pinEdge(GEO_Hedge e);
    bool 		 isPresent(PinnedEdge &se);

    struct PinnedFace
    {
	PinnedFace(GA_Offset face, GA_Offset p0, GA_Offset p1, GA_Offset p2) :
	    myFace(face), myP0(p0), myP1(p1), myP2(p2) {}

	GA_Offset myFace;
	GA_Offset myP0;
	GA_Offset myP1;
	GA_Offset myP2;

	bool operator==(const PinnedFace &other)
	{
	    return myFace == other.myFace &&
		((myP0 == other.myP0 && myP1 == other.myP1 &&
		     myP2 == other.myP2) ||
		 (myP0 == other.myP1 && myP1 == other.myP2 &&
		     myP2 == other.myP0) ||
		 (myP0 == other.myP2 && myP1 == other.myP0 &&
		     myP2 == other.myP1));
	}
    };

    PinnedFace 		 pinFace(GA_Offset face);
    bool 		 isPresent(PinnedFace &sf);

    typedef MultiQueue<PinnedEdge> 	EdgeQueue;
    typedef MultiQueue<PinnedFace> 	FaceQueue;

    // Container class used for the set of constrained edges
    // Each element is a pair of point offsets (possibly with an optional
    // primitive (used for removing holes) if it is a boundary edge
    struct Constraint
    {
	Constraint(const GA_Edge &e, bool h) { edge = e; is_boundary = h; }

	GA_Edge 	edge;
	bool		is_boundary;
    };

    struct LocatorNode
    {
	enum
	{
	    NSUCCESSORS = 3
	};
	// associated mesh face; 0 for sinks and deleted faces.
	GA_Offset myFaceOffset;
	GA_Offset myPointOffset[3];	// 3 points of the covered triangle
	int myRefCount;			// number of nodes pointing to this
	int mySuccessors[NSUCCESSORS];	// non-NULLs at the start
	int myParent;			// one of immediate ancestors
    };


    // Methods to manipulate the Point Location DAG:
    int 		 newLocatorNode();
    void 		 freeLocatorNode(int idx);
    void 		 attachLocatorNodeToFace(int idx, GA_Offset faceoff);
    void 		 detachLocatorNodeFromFace(int idx);
    void 		 setLocatorNodeSuccessors(int idx, int s1, int s2,
				int s3);

    void 		 splitLocatorNode(int idx, GA_Offset f0, GA_Offset f1,
         		        GA_Offset f2 = GA_INVALID_OFFSET);

    UT_Vector3 		 signedDistance(const UT_Vector2 &pos, int node_idx);

    inline UT_Vector2 	 barycentricCoordinates(const UT_Vector2 &x,
				 UT_Vector2 &a, UT_Vector2 &b);

    inline UT_Vector3 	 barycentricCoordinates(const UT_Vector2 &x,
                      	         GA_Offset faceoff, GA_Offset &aoff,
                      	         GA_Offset &boff, GA_Offset &coff);

    GA_Offset 		 resolveDuplicate(GA_Offset ptoff);
    GA_Edge 		 resolveDuplicate(GA_Edge edge);
    void 		 mapDuplicate(GA_Offset frompt, GA_Offset topt);

    /// Project 3D point onto plane.
    UT_Vector2	     	 make2D(const UT_Vector3 &pos) const;
    UT_Vector3	     	 make3D(const UT_Vector2 &pos) const;

    inline GA_Offset 	 getConcentricShellCenter(GA_Offset p);
    inline void 	 setConcentricShellCenter(GA_Offset p, GA_Offset c);

    inline int 		 getLocatorNodeIndex(GA_Offset faceoff);

    /// Find a leaf node with a triangle that encloses the given point.
    /// Returns the node, and if the point falls on an edge or a vertex of the
    /// triangle, returns those as well.

    GA_Offset 		 locateUsingLocatorDAG(const UT_Vector2 &pos,
				GEO_Hedge &on_edge, GA_Offset &on_point);

    int 		 locateInSuccessors(int dnode_idx,
				const UT_Vector2 &pos, UT_Vector3 &sd);

    GA_Offset 		 locateByWalking(const UT_Vector2 &searchpoint,
				GA_Offset searchface, GEO_Hedge &on_edge,
				GA_Offset &on_point);


    inline fpreal 	 counterclockwise(const UT_Vector2 &a,
				const UT_Vector2 &b, const UT_Vector2 &c)
			    { return UT_GeometryPredicate::orient2d(b, a, c); }

    inline fpreal 	 counterclockwise( GA_Offset pa, GA_Offset pb,
				GA_Offset pc);

    inline fpreal 	 incircle(GA_Offset pa, GA_Offset pb, GA_Offset pc,
				GA_Offset pd);

    fpreal 		 exactDot(const UT_Vector2 &u, const UT_Vector2 &v);

    UT_Vector2 		 circumcenter(GA_Offset pa, GA_Offset pb, GA_Offset pc);


    GA_Offset 		 appendPoint();
    void 		 deletePoint(GA_Offset pt,
         		        bool from_input_only = false);

    void 		 removeFace(GA_Offset faceoff);

    // Subdivide an edge by inserting a new point along it and dividing
    // the triangles on its two sides. The effect on input edge and the
    // the returned edge are as in divideEdge(). If a valid ptoff is
    // supplied the new point will have that offset and the corresponding
    // index in myPointList. The faces to the left and right of e before
    // splitting stay those the left and right of it afterwards.
    GEO_Hedge 		 splitEdge(GEO_Hedge e, fpreal bias,
				GA_Offset ptoff = GA_INVALID_OFFSET);

    void 		 collapseEdge(GEO_Hedge e);
    GEO_Hedge 		 findEdge(GA_Offset p0, GA_Offset p1);

    /// A journal of events during a point insertion. It is used to allow
    /// the point insertion to be undone.
    struct Journal
    {
	Journal() : myOldFace(GA_INVALID_OFFSET),
	    myOldEdge(GEO_INVALID_HEDGE), myNewEdge(GEO_INVALID_HEDGE),
	    myNewPoint(GA_INVALID_OFFSET), myRecording(false) {}

	void reset()
	{
	    myOldFace = GA_INVALID_OFFSET;
	    myOldEdge = GEO_INVALID_HEDGE;
	    myNewEdge = GEO_INVALID_HEDGE;
	    myNewPoint = GA_INVALID_OFFSET;
	    myFlippedEdges.entries(0);
	}

	void 		startRecording() { myRecording = true; }
	void 		stopRecording() { myRecording = false; }
	bool 		isRecording() { return myRecording; }

	GEO_HedgeArray	myFlippedEdges;
        GA_Offset 	myOldFace;
        GEO_Hedge	myOldEdge;
        GEO_Hedge	myNewEdge;
        GA_Offset	myNewPoint;
        bool 		myRecording;
    };

    bool 		 maxNewPointsReached();

    GA_Offset 		 inputDetailPoint(GA_Offset ptoff) const;
    GA_Offset 		 workDetailPoint(GA_Offset ptoff) const;

    GA_Offset 		 importInputDetailPoint(GA_Offset inptoff);
    GA_Offset 		 exportWorkDetailPoint(GA_Offset ptoff);

    void 		 importInputPoints(const GA_PointGroup *pts);
    void 		 importInputConstraints(const GA_PrimitiveGroup *prims,
                                const GA_EdgeGroup *constraint_edges);

    void 		 insertInputPoints(const GA_PointGroup *point_group,
         		        GA_PointGroup *duplicate_grp);

    void 		 cleanUp(GA_PointGroup *duplicate_grp,
         		        const GA_PrimitiveGroup *constraint_prims);

    /// Set up the bounding triangle, myGqd, and the locating DAG.
    void 		 setup();
    void 		 retireLocatorDAG();

    /// Set up my*Transform member variables from the plane
    /// normal/distance.
    void 		 calcTransforms(const UT_Vector3 &normal,
				fpreal distance);

    enum InsertStatus
    {
	INSERTED_IN_FACE, INSERTED_ON_EDGE, DUPLICATE, OUTSIDE
    };

    GEO_Hedge 		 insertOnEdge(GA_Offset pt, GEO_Hedge e);
    void 		 insertInFace(GA_Offset pt, GA_Offset f);

    /// Insert a new point into the existing triangulation.
    InsertStatus 	 insertPoint(GA_Offset ptoff,
				GEO_Hedge on_edge = GEO_INVALID_HEDGE,
				bool delaunize = true,
				GEO_Hedge *out_edge = NULL,
				GA_Offset walk_from_face = GA_INVALID_OFFSET);

    /// Undo an insertion.
    void 		 undoInsertPoint();

    /// Flip a single edge. Does not update the point location data structure.
    void 		 flipEdge(GEO_Hedge edge);
    void 		 unflipEdge(GEO_Hedge edge);

    void 		 delaunizePointLink(GA_Offset pt);

    /// Part of the core triangulation algorithm. Parameter point has just
    /// been inserted, and edge is being tested to see if it's legal
    /// according to the Delaunay criterion; if not, the edge is flipped
    /// and we recurse.
    void 		 delaunizeEdge(GA_Offset point, GEO_Hedge edge);

    enum ConstraintStatus
    {
	ENFORCED, PENDING, BLOCKED
    };

    void 		 intersectConstraints(GEO_Hedge &esplit, GA_Offset p1);
    int 		 turnToFace(GEO_Hedge &esearch, GA_Offset p);
    ConstraintStatus 	 trackConstraint(GEO_Hedge &esearch, GA_Offset p1,
				bool is_boundary);

    void 		 delaunayFixup(GEO_Hedge efixup, bool leftside);
    ConstraintStatus 	 enforceConstraint(GEO_Hedge estart, GA_Offset p1,
                                bool is_boundary);

    void 		 enforceInputConstraints();

    /// Remove the bounding triangle
    void 		 removeBoundingTriangle();
    /// Remove outside edges that aren't in the given group.
    void 		 removeOutside();
    /// Remove edges that are inside holes.
    void 		 removeHoles();

    void 		 propagateGroup(GA_PrimitiveGroup *grp, GA_Offset face);
    void 		 propagateRemoved(GA_Offset face, bool is_seed = false);

    /// Insert new points to satisfy refinement criteria.
    void 		 refine();

    /// Test if a given face is "bad" - i.e., excessive area, or too small
    /// an angle and add it to the bad triangles pile if so.
    void 		 testTriangle(GA_Offset face);

    // Test if a given edge is encroached and add it to the encroached edges
    // pile if so.
    void 		 testEdge(GEO_Hedge edge, bool on_left_only = false);

    /// Test if the given edge is "encroached" - i.e., if a triangles on
    /// some side has an obtuse angle opposite to the edge.
    bool 		 isEncroached(GEO_Hedge edge,
				bool on_left_only = false);

    /// Insert a point to split an non/existing edge at the given fraction
    /// along its length. Returns the edge along the old edge with origin
    /// equal to its midpoint.
    GEO_Hedge 		 insertSplitPoint(GEO_Hedge on_edge, fpreal bias,
                                bool delaunize = true);

    /// Split an existing edge at somewhere-near-the-midpoint.
    GEO_Hedge 		 concentricShellSplit(GEO_Hedge edge);

    void 		 splitEncroachedEdges(bool check_for_bad_faces);
    void 		 testLinkEdges(GA_Offset pt);
    void 		 testIncidentTriangles(GA_Offset pt);

    void 		 markEnforced(GEO_Hedge edge, bool enforced = true);
    bool 		 isEnforced(GEO_Hedge edge);
    void 		 markBoundary(GEO_Hedge edge, bool boundary = true);
    bool 		 isBoundary(const GEO_Hedge edge);

    bool 		 isBoundingTrianglePoint(GA_Offset ptoff);
    bool 		 isBoundingTriangleHedge(GEO_Hedge edge);

    void 		 remove(GA_Offset primoff);
    bool 		 isRemoved(GA_Offset primoff);

    void 		 markInputPoint(GA_Offset ptoff);
    bool 		 isInputPoint(GA_Offset ptoff);
    void 		 markFirstSplit(GA_Offset ptoff);
    bool 		 isFirstSplit(GA_Offset ptoff);

    // Some notation simplification

    inline GA_Offset 	 src(GEO_Hedge e) const
			    { return myHi->srcPoint(e); }

    inline GA_Offset 	 dst(GEO_Hedge e) const
			    { return myHi->dstPoint(e); }

    inline GA_Offset 	 apx(GEO_Hedge e) const
			    { return myHi->preSrcPoint(e); }

    inline GEO_Hedge 	 sym(GEO_Hedge e) const
			    { return myHi->nextEquivalentHedge(e); }

    inline GEO_Hedge 	 lnext(GEO_Hedge e) const
			    { return myHi->lnext(e); }

    inline GEO_Hedge 	 onext(GEO_Hedge e) const
			    { return myHi->onext(e); }

    inline GEO_Hedge 	 lprev(GEO_Hedge e) const
			    { return myHi->lprev(e); }

    inline GEO_Hedge 	 oprev(GEO_Hedge e) const
			    { return myHi->oprev(e); }

    inline UT_Vector2 	 getPos2(GA_Offset ptoff) const
			    { return myPos2.get(ptoff); }

    inline bool 	 isValid(GEO_Hedge e) const
			    { return myHi->isValidHedge(e); }

    inline bool 	 isPrimary(GEO_Hedge e) const
			    { return myHi->isPrimary(e); }

    GA_Offset 		 left(GEO_Hedge e);
    GA_Offset 		 right(GEO_Hedge e);
    GEO_Hedge 		 hedge(GA_Offset faceoff);


    typedef UT_Array<Constraint> ConstraintArray;
    typedef UT_Map<GA_Offset, GA_Offset> OffsetMap;

    // Locator DAG
    typedef UT_Array<LocatorNode> LocatorNodeArray;

    LocatorNodeArray	 myLocatorNodes;
    UT_IntArray 	 myLocatorNodesFreeIndices;
    int			 myLocatorRootIndex;

    // Transform matrices between 2d and 3d
    UT_Matrix4		 my3to2Transform;
    UT_Matrix4		 my2to3Transform;

    // Flags
    bool		 myHasClosedConstraints: 1;
    bool		 myHasConflictingConstraints: 1;
    bool		 myCanSplitConstraints: 1;
    bool		 myRestorePositions: 1;
    bool	     	 myDoRefinement: 1;
    bool        	 myRemoveDuplicatePoints: 1;
    bool	    	 myKeepBoundingTriangle: 1;
    bool		 myMapDuplicates: 1;
    bool		 myWrangleAttributes: 1;


    bool		 myRemoveFromConvexHull;
    RemoveOutsidePolicy  myRemoveOutsidePolicy;

    int			 myMaxNewPoints;
    int			 myNumNewPoints;
    fpreal		 myMinEdgeLengthSqr;
    fpreal	    	 myMaxArea, myMaxAngleCosSqr;


    UT_String   	 myConstraintNewPointGroupName;

    GU_Detail		*mySrcGdp;
    GU_Detail		*myGdp;
    GEO_HedgeInterface	*myHi;
    GA_PointWrangler	*myWrangler;

    bool		 myUsePos2Attrib;
    UT_String	 	 myPos2AttribName;
    GA_RWHandleV2   	 myPos2;

    ConstraintArray 	 myInputConstraints;
    OffsetMap 		 myDuplicateMap;

    // Points and hedges of the bounding triangle
    GA_Offset 		 myBoundingTrianglePoints[3];
    GEO_Hedge		 myBoundingTriangleHedges[3];

    // 3d positions of the bounding square (to help draw guide)
    UT_Vector3		myBoundingSquareCorners[4];

    // Point group accumulating constraint split points
    GA_PointGroup   	*myConstraintNewPointGroup;

    // Journal for tracking changes during the last insertion to allow undo
    Journal		 myJournal;

    EdgeQueue		 myEncroachedEdges;
    FaceQueue		 myBadTriangles;

    int			 myRandSeed;

    UT_StringStream	 myWarningMessage;

    fpreal		 myTollerance;

    GA_VertexGroup	*myEnforcedHedges;
    GA_VertexGroup	*myBoundaryHedges;

    GA_PrimitiveGroup	*myRemovedFaces;

    GA_PointGroup	*myInputPoints;
    GA_PointGroup	*myFirstSplitPoints;

    GA_RWHandleI	 myShellCenter;
    GA_RWHandleI	 myLocatorIndex;
    GA_RWHandleI	 myInputDetailPoint;
    GA_RWHandleI	 myWorkDetailPoint;
};

#endif
