/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 * COMMENTS: Utility tool to compute inset positions for a one or more
 * (open or closed) chains of point positions.
 *
 * NB: At this point (and likely for the foreseeable future), GU_Insetter
 * will be based on an idea which I call the "weak straight skeleton" of the
 * input chains. I don't know of any references to this concept in the
 * literature and I worked it out on my own due to certain advantages it
 * provides and the way it functions as a middle ground for efficient yet
 * acceptable insetting.
 *
 * The standard straight skeleton (defined for straight-line embedded graph
 * in 2D), is defined by creating a "wavefront" by sliding the edges of the
 * graph at equal speed parallel to themselves and tracking the paths the
 * vertices of the graph travel. As the edges are slid during this process,
 * two type of "events" can take place:
 *
 *     An "Edge Event" traditionally refers to the moment at which the length
 *     of an edge becomes zero, i.e. the straight skeleton branches that
 *     correspond to the trajectories of the two vertices at two ends of the
 *     edge in question collide. Such an event merges the two trajectories into
 *     one. Note that the topology of the wavefront stays unchanged under the
 *     effect of an edge event.
 *
 *     A "Vertex Event" refers to the moment at which a vertex of the moving
 *     graph hits an edge to which it is not adjacent. The hit edge is then
 *     split and the connected component of the wavefront that contains the
 *     colliding parties is split into two (or more). Therefore, this type of
 *     event causes the topology of the wavefront to change, in particular
 *     spawning two ore more branches in the skeleton with trajectories
 *     diverging out of the collision point.
 *
 *
 * The weak straight skeleton is defined using only the first type of event.
 * In other words, we follow the trajectories of the vertices of the graph but
 * systematically ignore collisions of the vertices with edges and only catch
 * collisions between trajectories of "neighbouring" vertices along the
 * wavefront. Unlike the wavefronts of the straight skeleton, those of the weak
 * skeleton can self-intersect but in a limited manner. However, there are at
 * a handful of significant consequences to this choice:
 *
 *     1. The topology of the wavefront never changes. This maybe indispensable
 *        if, say, we want to bridge the original chain and the inset one. In
 *        particular, there is no reason to define the weak skeleton on general
 *        graphs but rather on "chains" which would be open or closed sequences
 *        of (possibly repeated) vertices.
 *
 *     2. It is far far cheaper to compute the weak skeleton than do the
 *        standard one, since it is the detection of vertex events that
 *        is the real challenge in the computation of the straight skeleton and
 *        takes upwards of quadratic time per event or O(n^3) in general. In
 *        contrast the weak skeleton can be computed in O(n log n) time.
 *
 *     3. It is also a fair bit easier to turn the weak skeleton computation
 *        into a robust algorithm due to the strong restriction on the way the
 *        topology is modified along the way.
 *
 *     4. It is much easier to generalize the algorithm to non-planar curves.
 *        due to the fact that the collisions only need to be sought (or
 *        interpreted) between neighbouring vertices along the wavefront.
 *
 */

#ifndef __GU_Insetter_h__
#define __GU_Insetter_h__

#include "GU_API.h"
#include "GU_Detail.h"

#include <UT/UT_PriorityQueue.h>

class GU_Detail;


// Normal differences with cosine smaller than this value (~= cos(30)) will be
// treated as non-coplanar when insetting and the insetting direction follows
// the cross product of the normals.

#define INSET_COPLANARITY_TOL		0.866

class GU_API GU_Insetter
{
public:

    /// OffsetConversion tells the insetter to optionally convert a joint
    /// given using a vertex offset into its wired primitive or point offset
    /// before evaluating the position attribute handle on it.

    enum OffsetConversion
    {
	IDENTITY = 0,
	VERTEX_TO_POINT,
	VERTEX_TO_PRIMITIVE
    };

#if 0
    constexpr static
    fpreal	 	 CoplanarityTolerance = 0.866;
#endif

    /// GU_Insetter works on one or more sequences of (point/primitive/vertex)
    /// offsets on a detail. Each sequence of offsets, along with a *position*
    /// attribute defines a *chain* of edges. The end-points of the edges, which
    /// we call the *joints* of the chain, can be repeated along the chain (an
    /// offset may appear multiple times in the same chain). Each edge of the
    /// chain is supplied with a velocity vector along which it will be slid.
    /// It is also possible to override the otherwise calculated joint
    /// velocities.
    ///
    /// The number of chains is specified by 'num_chains'. The 'chain_lengths'
    /// array should have at least num_chains elements and specifies the number
    /// of joints in each chain. The 'joint_offsets' array must present the
    /// joint offsets for all chains back to back and therefore should have
    /// length (at least) equal to the sum of the elements in 'chain_lengths'.
    ///
    /// Each chain can be "closed" or "open", respectively meaning an edge is
    /// or is not interpreted to exist between the last joint of the chain and
    /// its first. Setting 'closed_flags' to NULL indicates that all the chains
    /// are "closed". Otherwise the array to which it points must have least
    /// num_chains elements.
    ///
    /// The 'edge_vel' array must always be supplied with exactly the same
    /// length as joint_offsets to indicate the velocity vector for the edge
    /// between the joint at a corresponding position and its successor.
    /// The velocity vector paired with the last joint of an open chain is
    /// ignored.
    ///
    /// The 'joint_vel' array, provides overrides for the joint velocities that
    /// are automatically calculated based on edge velocities. Joints with
    /// override velocity set to (0, 0, 0) still get a calculated velocity.
    /// However, joints with preset velocities are ensured to always move in
    /// the given direction even after merging with other trajectories. If two
    /// joints with preset velocities collide, the insetting limit is reached.
    ///
    /// *Two sided* insetters can evaluate both positive and negative inset
    /// amounts. If 'two-sided' is set to false, evaluation of inset position
    /// for negative isnet amounts will only use the initial joint velocity
    /// vectors.
    ///
    /// 'collinearity_tol' is the cosine of the largest angle between the
    /// normals of the planes scanned by two edge velocity vectors below which
    /// the planes are considered parallel.

			 GU_Insetter(const GU_Detail *gdp,
				int num_chains,
				int *chian_lengths,
				GA_Offset *joint_offsets,
				bool *closed_flags,
				const UT_Vector3 *edge_vel,
				GA_ROHandleV3 pos,
				const UT_Vector3 *joint_vel = nullptr,
				bool two_sided = true,
				OffsetConversion off_conv = IDENTITY,
				fpreal coplanar_tol = INSET_COPLANARITY_TOL,
				bool merge_branches = true,
				bool common_limit = false);

			~GU_Insetter();

    UT_Vector3 		 getSrcInsetVector(int idx, fpreal inset,
                                int *is_limit = nullptr);
    UT_Vector3 		 getDstInsetVector(int idx, fpreal inset,
                                int *is_limit = nullptr);

    /// Returns the inset value beyond which insetting freezes, ie, the
    /// outcome stays the same for larger values. If no limit exists, -1 is
    /// returned.
    /// If the insetter is two-sided, the returned limit is the the limit of
    /// the positive side if 'positive' is set true or that of the negative
    /// side otherwise. If single-sided, the parameter is ignored. Note that
    /// the limit of the negative side is returned as a positive number if
    /// such a limit exists (and as -1 otherwise) and the actual limit value
    /// would be the negative of the returned amount.

    fpreal		 getLimit(bool positive = true);

    /// Helper methods for setting up an insetter:

    /// Return the velocity vector for sliding an edge of a polygon with
    /// direction edge_vect, inward on the polygon parallel to itself.
    /// prim_nml must be a unit vector but edge_vect need not be.

    static inline
    UT_Vector3		 edgeVelocity(UT_Vector3 edge_vect,
				UT_Vector3 prim_nml);

    /// Vectors v0 and v1 are oriented consecutive edge vectors whose common
    /// end-point's insetting direction is determined by this method. b0 and b1
    /// are the directions along which the edges are shifted during insetting.
    ///
    /// If b0 is 0, then the inset direction will be along -v0 and if b1 is 0,
    /// then the inset direction will be along v1.
    ///
    /// min_coplanar_nml_diff_cos is cosine of the smallest angle between
    /// normals of the planes respectively defined by v0, b0 and v1, b1,
    /// beyond which the two planes are taken as distinct and the line shared
    /// between them treated as a fold, causing the inset vector to move along
    /// this fold line. The velocity of the movement is then determined by
    /// the edge that makes a smaller angle with the fold.

    static inline
    UT_Vector3	 	 jointVelocity(UT_Vector3 v0, UT_Vector3 v1,
				UT_Vector3 b0, UT_Vector3 b1,
				fpreal min_coplanar_nml_diff_cos
				    = INSET_COPLANARITY_TOL,
				bool *is_folded_joint = nullptr);

    /// Test used to determine if a joint defined by velocity vectors b0 and b1
    /// should be treated as a fold. In contrast to a flat joint where the
    /// joint moves along the bisector of the joint edges during insetting,
    /// a folded joint moves along the line of intersection of the two planes
    /// defined by the two velocity vectors. If n0 or n1 are non-NULL, the
    /// respective plane normals of the joint are returned.

    static inline
    bool		 isFoldedJoint(UT_Vector3 v0, UT_Vector3 v1,
				UT_Vector3 b0, UT_Vector3 b1,
				fpreal min_coplanar_nml_diff_cos,
				UT_Vector3 *n0 = nullptr,
				UT_Vector3 *n1 = nullptr);

private:

    class Tree
    {
    public:

			 Tree(const GU_Detail *gdp,
			      int num_chains,
			      int *chian_lengths,
			      GA_Offset *joint_offsets,
			      bool *closed_flags,
			      const UT_Vector3 *edge_vel,
			      GA_ROHandleV3 pos,
			      const UT_Vector3 *joint_vel,
			      bool backward,
			      OffsetConversion off_conv = IDENTITY,
			      fpreal coplanar_tol = INSET_COPLANARITY_TOL,
			      bool merge_branches = true,
			      bool common_limit = true);

	UT_Vector3 	 getSrcInsetVector(int idx, fpreal inset,
                              int *is_limit = nullptr);

	UT_Vector3 	 getDstInsetVector(int idx, fpreal inset,
                              int *is_limit = nullptr);

	fpreal		 getMaxStopTime() { return myLimitTime; }

	fpreal		 getLimit();

	struct Edge
	{
	    explicit	 Edge() : mySrcIdx(-1), myDstIdx(-1) {}
	    explicit	 Edge(int sidx, int didx = -1) :
			    mySrcIdx(sidx), myDstIdx(didx) {}

	    int 	 srcIdx() const { return mySrcIdx; }
	    int 	 dstIdx() const { return myDstIdx; }

	    bool	 isValid()
			    { return (mySrcIdx >= 0 && myDstIdx >= 0); }

	    bool	 operator==(const Edge &other)
			    { return mySrcIdx == other.mySrcIdx &&
				myDstIdx == other.myDstIdx; }
	    bool	 operator!=(const Edge &other)
			    { return mySrcIdx != other.mySrcIdx ||
				myDstIdx != other.myDstIdx; }

	private:
	    int		 mySrcIdx;
	    int		 myDstIdx;
	};

	static Edge 	InvalidEdge;

	struct NodeInfo
	{
	    explicit	 NodeInfo(int idx = -1, Edge left_edge = InvalidEdge,
				 Edge right_edge = InvalidEdge) :
				 myLeftEdge(left_edge),
				 myRightEdge(right_edge),
				 myVelocity(0, 0, 0),
				 myStartPos(0, 0, 0),
				 myStartTime(-1),
				 myStopTime(-1),
				 myParentIdx(-1),
				 myIndex(idx),
                                 myChainLimitTime(-1)
			     {}

	    Edge	 leftEdge() const { return myLeftEdge; }
	    Edge	 rightEdge() const { return myRightEdge; }
	    UT_Vector3 	 getVelocity() const { return myVelocity; }
	    void	 setVelocity(UT_Vector3 e) { myVelocity = e; }
	    UT_Vector3 	 getStartPos() const { return myStartPos; }
	    void	 setStartPos(UT_Vector3 s) { myStartPos = s; }
	    UT_Vector3	 getPosAtTime(fpreal t) const;
	    int		 getParentIdx() const { return myParentIdx; }
	    void	 setParentIdx(int idx) { myParentIdx = idx; }
	    fpreal	 getStartTime() const { return myStartTime; }
	    void	 setStartTime(fpreal m) { myStartTime = m;  }
	    fpreal	 getStopTime() const { return myStopTime; }
	    void	 setStopTime(fpreal m) { myStopTime = m;  }
	    int		 getIndex() const { return myIndex; }
            fpreal       getChainLimitTime() const { return myChainLimitTime; }
            void         setChainLimitTime(fpreal t) { myChainLimitTime = t; }

	private:
	    int		 myIndex, myParentIdx;
	    Edge	 myLeftEdge, myRightEdge;
	    UT_Vector3	 myStartPos, myVelocity;
	    fpreal	 myStartTime, myStopTime;
	    fpreal       myChainLimitTime;
	};

	typedef UT_Array<NodeInfo> NodeInfoArray;

	struct Node
	{
	    explicit	 Node() : myIndex(-1) {}
	    explicit	 Node(int index) : myIndex(index) {}
	    int 	 idx() const { return myIndex; }
	    bool	 operator==(const Node &other)
			    { return myIndex == other.myIndex; }
	private:
	    int		 myIndex;
	};

	typedef UT_Array<Node> NodeArray;



	enum EventType
	{
	    RAY,
	    COLLISION
	};

	// An event encodes information about collision of the ray associated
	// to node idx0 to that of the node idx1.
	struct Event
	{
			 Event() :
			     myType(RAY), myNode0(-1), myNode1(-1), myTime(-1)
			 {}

			 Event(Node n, fpreal t = 0.0) :
			     myType(RAY), myNode0(n), myNode1(n), myTime(t) {}

			 Event(EventType type, Node n0, Node n1, fpreal t) :
			     myType(type), myNode0(n0), myNode1(n1), myTime(t) {}

			 Event(const Event &other) :
			     myType(other.myType), myNode0(other.myNode0),
			     myNode1(other.myNode1), myTime(other.myTime) {}

	    EventType 	 getType() { return myType; }
	    Node	 getNode0() { return myNode0; }
	    Node	 getNode1() { return myNode1; }
	    fpreal	 getTime () { return myTime; }

	private:
	    EventType	 myType;
	    Node 	 myNode0, myNode1;
	    fpreal 	 myTime;
	};

	class EventCompare
	{
	public:
	    EventCompare() {}
	    bool operator()(Event e1, Event e2) const
	    {
		return e1.getTime() >= e2.getTime();
	    }
	};

	typedef UT_PriorityQueue<Event, EventCompare> 	Queue;



    private:

	UT_Vector3	 getInsetPos(Node n, fpreal t, int *is_limit = nullptr);

	void		 build();

	void		 buildChain(int chain_num);

	Node		 newNode(Edge left_edge, Edge right_edge,
				bool is_leaf = false);

	Node		 newChainNode(int chain, Edge left_edge,
				Edge right_edge, bool is_leaf = false);

	bool	 	 hasParent(Node n)
			    { return (myNodes(n.idx()).getParentIdx() >= 0); }

	Node		 getParent(Node n)
			    { return Node(myNodes(n.idx()).getParentIdx()); }

	Node		 getRoot(Node n);

	inline
	UT_Vector3	 edgeVector(Edge e)
			    { return dstJointPos(e) - srcJointPos(e); }
	inline
	UT_Vector3	 edgeVelocity(Edge e)
			    { return myEdgeVel[e.srcIdx()]; }

	inline Edge	 leftEdge(Node n) const
			    { return myNodes(n.idx()).leftEdge(); }

	inline Edge	 rightEdge(Node n) const
			    { return myNodes(n.idx()).rightEdge(); }

	inline Node	 srcLeaf(Edge e)
			    { return myJointLeaf(e.srcIdx()); }

	inline Node	 dstLeaf(Edge e)
			    { return myJointLeaf(e.dstIdx()); }

	inline
	UT_Vector3 	 getVelocity(Node n)
			    { return myNodes(n.idx()).getVelocity(); }

	inline void	 setVelocity(Node n, UT_Vector3 v)
			    { myNodes(n.idx()).setVelocity(v); }

	inline
	UT_Vector3 	 getStartPos(Node n)
			    { return myNodes(n.idx()).getStartPos(); }

	inline void	 setStartPos(Node n, UT_Vector3 p)
			    { myNodes(n.idx()).setStartPos(p); }

	inline
	UT_Vector3 	 getPosAtTime(Node n, fpreal t)
			    { return myNodes(n.idx()).getPosAtTime(t); }

	inline fpreal 	 getStartTime(Node n)
			    { return myNodes(n.idx()).getStartTime(); }

	inline void	 setStartTime(Node n, fpreal t)
			    { myNodes(n.idx()).setStartTime(t); }

	inline fpreal 	 getStopTime(Node n)
			    { return myNodes(n.idx()).getStopTime(); }

	inline void	 setStopTime(Node n, fpreal t)
			    { myNodes(n.idx()).setStopTime(t); }
        
        inline fpreal 	 getChainLimitTime(Node n)
			    { return myNodes(n.idx()).getChainLimitTime(); }

	inline void	 setChainLimitTime(Node n, fpreal t)
			    { myNodes(n.idx()).setChainLimitTime(t); }        

	inline void	 setParent(Node child, Node parent)
			    { myNodes(child.idx()).setParentIdx(parent.idx()); }

	inline GA_Offset jointOffset(int idx)
	{
	    switch (myOffsetConv)
	    {
	    case IDENTITY:
		return myJointOffsets[idx];
	    case VERTEX_TO_POINT:
		return myGdp->vertexPoint(myJointOffsets[idx]);
	    case VERTEX_TO_PRIMITIVE:
		return myGdp->vertexPrimitive(myJointOffsets[idx]);
	    default:
		return myJointOffsets[idx];
	    }
	}

	// access to individual chain data
	inline int	 chainSize(int i)
			    { return myChainStarts(i + 1) - myChainStarts(i); }

	inline bool	 isChainClosed(int i)
			    { return !myClosedFlags || myClosedFlags[i]; }

	inline int	 jointIndex(int chain_num, int i);
			    // { return myChainStarts(chain_num) + i; }

	inline
	UT_Vector3	 srcJointPos(Edge e)
			    { return myPosHandle.get(jointOffset(e.srcIdx())); }

	inline
	UT_Vector3	 dstJointPos(Edge e)
			    { return myPosHandle.get(jointOffset(e.dstIdx())); }

	UT_Vector3	 initialVelocity(Node node, bool &is_folded_joint);
	UT_Vector3	 internalNodeVelocity(Node node);
	fpreal		 collisionTime(Node n0, Node n1);


	inline Node	 srcLeaf(int idx)
			    { return myJointLeaf(idx); }

	inline Node	 dstLeaf(int idx)
			    { return myJointLeaf(idx); }

	int		 myNumEdges;
	const
	UT_Vector3	*myInitialVel;
	const
	UT_Vector3	*myEdgeVel;

	NodeInfoArray	 myNodes;
	bool		 myIsBuilt;
	bool		 myIsBackward;

	UT_IntArray	 myChainFirstNodeIdx;
	UT_IntArray	 myChainNumNodes;

	NodeArray	 myJointLeaf;
	fpreal		 myLimitTime;

	int		 myNumChains;
	bool		*myClosedFlags;
	UT_IntArray	 myChainStarts;
	GA_Offset	*myJointOffsets;

	GA_ROHandleV3	 myPosHandle;
	OffsetConversion myOffsetConv;

	fpreal		 myCoplanarTol;

	bool		 myMergeBranches;
	bool		 myCommonLimit;

	const GA_Detail *myGdp;
    };

    bool		 myTwoSided;

    Tree		 myPosTree;
    Tree		 myNegTree;
};

int
GU_Insetter::Tree::jointIndex(int chain_num, int i)
{
    int n = chainSize(chain_num);

    if (isChainClosed(chain_num))
	return myChainStarts(chain_num) + ((i + n) % n);
    else
    {
	if (i < 0 || i > n - 1)
	    return -1;

	return myChainStarts(chain_num) + i;
    }
}

bool
GU_Insetter::isFoldedJoint(UT_Vector3 v0, UT_Vector3 v1,
			   UT_Vector3 b0, UT_Vector3 b1,
			   fpreal min_coplanar_nml_diff_cos,
			   UT_Vector3 *n0, UT_Vector3 *n1)
{
    UT_Vector3 nn0 = cross(b0, v0);
    UT_Vector3 nn1 = cross(b1, v1);
    nn0.normalize();
    nn1.normalize();

    if (n0)
	*n0 = nn0;

    if (n1)
	*n1 = nn1;

    return dot(nn0, nn1) < min_coplanar_nml_diff_cos;
}


UT_Vector3
GU_Insetter::jointVelocity(UT_Vector3 v0, UT_Vector3 v1,
			   UT_Vector3 b0, UT_Vector3 b1,
			   fpreal min_coplanar_nml_diff_cos,
			   bool *is_folded_joint)
{
    UT_Vector3 vel;
    fpreal scale;

    if (b0.isZero())
    {
	vel = -v0;
	scale = b1.length2() / dot(vel, b1);
    }
    else if (b1.isZero())
    {
	vel = v1;
	scale = b0.length2() / dot(vel, b0);
    }
    else
    {
	UT_Vector3 n0, n1;
	if (isFoldedJoint(v0, v1, b0, b1, min_coplanar_nml_diff_cos, &n0, &n1))
	{
	    vel = cross(n0, n1);
	    if (is_folded_joint)
		*is_folded_joint = true;
	}
	else
	{
	    fpreal b0l2 = b0.length2(), b1l2 = b1.length2();
	    if (SYSisEqual(b0l2, b1l2, 0.01 * SYSmin(b0l2, b1l2)))
	    {
		vel = 0.5 * (b0 + b1);
	    }
	    else
	    {
		fpreal u0, u1;
		UT_Vector3 n = cross(v0, v1);
		UT_Vector3 m0 = cross(n, b0);
		UT_Vector3 m1 = cross(n, b1);
		fpreal denum = intersectLines<fpreal>(b0, m0, b1, m1, u0, u1);

		UT_ASSERT(!SYSequalZero(denum));

		if (SYSequalZero(denum))
		    vel = 0.5 * (b0 + b1);
		else
		    vel = b0 + u0 * m0;
	    }

	    if (is_folded_joint)
		*is_folded_joint = false;
	}

	fpreal d0 = dot(vel, b0);
	fpreal d1 = dot(vel, b1);

	scale = SYSmin(b0.length2() / d0, b1.length2() / d1);

	// The averaging alternative:
	// scale = (b0.length2() + b1.length2())/ (d0 + d1);
    }

    return scale * vel;
}

UT_Vector3
GU_Insetter::edgeVelocity(UT_Vector3 edge_vect,
			   UT_Vector3 prim_nml)
{
    edge_vect.cross(prim_nml);
    edge_vect.normalize();
    return edge_vect;
}


fpreal 		 GUweightedBisectingAngle(fpreal angle_sum,
			fpreal wt0, fpreal wt1);

UT_Vector3 	 GUweightedBisector(const UT_Vector3 &v0, const UT_Vector3 &v1,
			fpreal w0, fpreal w1, const UT_Vector3 &cn);

#endif
