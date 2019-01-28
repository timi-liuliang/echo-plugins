/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GU_PathFinder.h
 *
 * COMMENTS:
 *	Finds paths of various specifications between various element types
 *	in polygon meshes.
 */

#ifndef __GU_PathFinder__
#define __GU_PathFinder__

#include "GU_API.h"
#include "GU_Detail.h"

#include <GEO/GEO_HedgeInterface.h>
#include <GA/GA_DataBitArray.h>
#include <UT/UT_PriorityQueue.h>

class gu_ShortestPathCost;

using GU_SHedgeArray = UT_Array<GEO_SHedge>;


class GU_EdgeSuccessor
{
public:
    enum Type
    {
	QUAD_LEFT = 0,
	QUAD_RIGHT,
	QUAD_OPPOSITE,
	OPPOSITE,
	BOUNDARY,
	ANY,
	NUM_TYPES = ANY
    };

    enum Mask
    {
	ANY_MASK 		= 0,
	QUAD_LEFT_MASK 		= 1 << QUAD_LEFT,
	QUAD_RIGHT_MASK		= 1 << QUAD_RIGHT,
	QUAD_OPPOSITE_MASK	= 1 << QUAD_OPPOSITE,
	OPPOSITE_MASK		= 1 << OPPOSITE,
	BOUNDARY_MASK		= 1 << BOUNDARY
    };

    			 GU_EdgeSuccessor() { clear(); }

    GEO_SHedge 		&operator()(int i) { return mySucc[i]; }
    GEO_SHedge 	 	 operator()(int i) const { return mySucc[i]; }

    static
    Mask 		 typeMask(Type t)
    			    { return t < NUM_TYPES ? Mask(1 << t) : ANY_MASK; }

    void clear()
    {
	for (int i = 0; i < NUM_TYPES; ++i)
	    mySucc[i] = GEO_INVALID_SHEDGE;
    }

private:
    GEO_SHedge 		 mySucc[NUM_TYPES];
};

// Template parameter T is the cost class

template <class T = gu_ShortestPathCost>
class GU_API GU_PathFinder
{
public:

    class PathEdge
    {
    public:
	PathEdge() : mySHedge(GEO_INVALID_SHEDGE), myPrev(GEO_INVALID_SHEDGE)
	{ }

	explicit
	PathEdge(GEO_SHedge sh,
		  GEO_SHedge prev = GEO_INVALID_SHEDGE) :
	     mySHedge(sh), myPrev(prev) { myPathCost.zero(); }

	PathEdge(GEO_SHedge sh, GEO_SHedge prev, const T &cost) :
	     mySHedge(sh), myPrev(prev), myPathCost(cost) { }

        bool 		 operator()(PathEdge &p1, PathEdge &p2) const
				{ return (p1.myPathCost > p2.myPathCost); }

        T 		 getPathCost() { return myPathCost; }
        GEO_SHedge 	 getSHedge() { return mySHedge; }
        GEO_SHedge	 getPrev() { return myPrev; }
        bool		 hasPrev() { return myPrev.isValid(); }

    private:
        GEO_SHedge 	 mySHedge, myPrev;
        T	 	 myPathCost;
    };

    using CostType = T;

public:

    // If the uv_attrib parameter is passed, then the search takes place
    // in UV space (boundaries, connectivity, etc follow the UVs).

    explicit		 GU_PathFinder(const GEO_DetachedHedgeInterface *dhip,
			 	const GA_Attribute *uv_attrib = nullptr);

    // Paths are formed of *signed* half-edges, where the sign indicates
    // whether the half-edge is being travelled from source to destination
    // (positive) or the other way around (negative). Dual paths are sequences
    // of edges (represented by half-edges) which are typically separated by
    // polygons but may share endpoints. In specifying the start and end
    // targets for path search, we used signed half-edges to indicate the
    // direction of departure or arrival at the start or end edges
    // respectively. In the dual path search, we only use positive half-edges
    // since all that we care about are the start and end edges. Note that
    // a search can specify multiple start and end (signed) half-edges. This
    // is particularly used in implementing primitive, point, and vertex loops.

    // Set a start or ending half-edge for the next search. If and_equiv
    // is set to true, then all equivalent half-edges to the given one are
    // also added as possible start or end half-edges. Note that any generated
    // path should follow the direction of one of these starting half-edges.
    void 		 setStartHedge(GEO_Hedge sh, bool and_equiv = true);
    void 		 setEndHedge(GEO_Hedge eh, bool and_equiv = true);

    // Set a start or end *signed* half-edge. Specifying the start or end
    // half-edge this way can be used to free us from the actual direction of
    // existing half-edges, particularly when the intended direction may not
    // be realized by any half-edges, mostly notably in case of boundaries when
    // we have a single half-edge. If and_reverse is set to true, the half-edge
    // is added in both directions.
    void 		 setStartSHedge(GEO_SHedge ssh,
         		          bool and_reverse = false);
    void 		 setEndSHedge(GEO_SHedge ssh,
         		          bool and_reverse = false);

    // Specify the search start and end by points. These get translated to
    // appropriate half-edges.
    void		 setStartPoint(GA_Offset pt);
    void		 setEndPoint(GA_Offset pt);

    // Specify the search start and end by vertices. These get translated to
    // appropriate half-edges. Note that vertex path returns "representative"
    // vertices between the two ends which are themselves treated as
    // representatives of their respective points (or their respective island
    // points if there's an active UV attribute).

    void		 setStartVertex(GA_Offset vtx);
    void		 setEndVertex(GA_Offset vtx);

    // Set start and end primitives. These get translated to appropriate
    // half-edges.
    void		 setStartPrimitive(GA_Offset prim);
    void		 setEndPrimitive(GA_Offset prim);


    void		 clearStartSHedges();
    void		 addStartSHedge(GEO_SHedge sh,
        		          bool and_reverse = false);

    void		 clearEndSHedges();
    void		 addEndSHedge(GEO_SHedge sh, bool and_reverse = false);

    // The main method to search for a the shortest path between a start
    // signed half edge and an end one. The successor type mask allows
    // to prioritize various types of successors and change the cost values
    // and is utilized by the template parameter cost class.
    T	 		 findPath(GU_SHedgeArray &path,
         		          GU_EdgeSuccessor::Mask succ_type);

    // Find a dual path between a source and a destination half-edge.
    T	 		 findDualPath(GU_SHedgeArray &path,
         		          GU_EdgeSuccessor::Mask succ_type);


    // Assign a group of points to be avoided by search paths.
    void		 avoidPoints(GA_PointGroup *grp)
				{ myAvoidPoints = grp; }

    // Assign a group of edges to be avoided by search paths.
    void		 avoidEdges(GA_EdgeGroup *grp)
				{ myAvoidEdges = grp; }

    // Assign a group of primitives to be avoided by search paths.
    void		 avoidPrimitives(GA_PrimitiveGroup *grp)
				{ myAvoidPrimitives = grp; }

    // Assign a group of vertices to be avoided by search paths.
    void		 avoidVertices(GA_VertexGroup *grp)
				{ myAvoidVertices = grp; }

    // Reset partial search results. This clears the start and end elements,
    // unless heap_only is set to true in which case only the partial search
    // tree is cleared.
    void		 reset(bool heap_only = false);

    const
    GU_SHedgeArray	&getStartSHedges() const
				{ return myStartSHedges; }

    SYS_FORCE_INLINE
    T	 		&getBestCost(GEO_SHedge sh);

    // Bypass cycles in the path or dual path.
    void		 simplifyPath(GU_SHedgeArray &path);
    void		 simplifyDualPath(GU_SHedgeArray &path);

    // Extend an open path on both ends by walking straight at regular
    // quad junctions until the two ends meet or irregular points are reached.
    // Returns true if the path changes and false otherwise.
    bool		 extendPath(GU_SHedgeArray &path,
        		        bool trim_crossing = true);

    // Extend an open dual path on both ends by walk over opposite quad edges
    // until a closed edge ring is formed or a non-quad is reached.
    bool	 	 extendDualPath(GU_SHedgeArray &path,
        		        bool trim_crossing = true,
        		        bool is_for_prim_loo = false);

    // Determine whether the start or end edges are boundary edges. When a
    // UV attribute is given, this is determined with regard to that.

    bool		 isStartOnBoundary() { return myStartOnBoundary; }
    bool		 isEndOnBoundary() { return myEndOnBoundary; }

private:

    void 		 dumpSHedge(GEO_SHedge sh) const;

    SYS_FORCE_INLINE
    void 		 mark(GEO_SHedge sh);

    SYS_FORCE_INLINE
    bool 		 isMarked(GEO_SHedge sh);

    void		 initializeEdgeHeap();

    SYS_FORCE_INLINE
    void 		 setBestCost(GEO_SHedge sh, const T &cost,
				GEO_SHedge prev_sh = GEO_INVALID_SHEDGE);

    SYS_FORCE_INLINE
    GEO_SHedge 	 	 getPrev(GEO_SHedge sh);

    SYS_FORCE_INLINE
    bool		 hasPrev(GEO_SHedge sh)
    			    { return getPrev(sh).hedge() != GEO_INVALID_HEDGE; }

    // Search for the destination point of the first signed half-edge in the
    // path that matches pt and truncates the rest of the path. Returns true
    // if the point is found or false otherwise.
    bool		 trimPathAtPoint(GU_SHedgeArray &path, GA_Offset pt);

    // Search for a half-edge in the path incident to a primitive and truncates
    // the rest of the path after that half-edge. Returns true if the primitive
    // is found.
    bool		 trimDualPathAtPrimitive(GU_SHedgeArray &path,
				GA_Offset prim);

    GA_Offset		 trimCrossingPaths(GU_SHedgeArray &w0,
				GU_SHedgeArray &w1);

    GA_Offset		 trimCrossingDualPaths(GU_SHedgeArray &w0,
				GU_SHedgeArray &w1);


    // fills path with signed hedges from the optimal path found from a start
    // signed hedge to esh.
    void		 extractPath(GEO_SHedge esh, GU_SHedgeArray &path);
    void		 extractDualPath(GEO_SHedge esh, GU_SHedgeArray &path);

    bool		 isStartSHedge(GEO_SHedge sh);
    bool		 isStartSHedgePrimary(GEO_SHedge sh);

    bool		 isEndSHedge(GEO_SHedge sh);
    bool		 isEndSHedgePrimary(GEO_SHedge sh);

    GEO_SHedge		 findMarkedEndSHedge();
    GEO_SHedge		 findMarkedEndSHedgePrimary();

    // Some signed hedge helper methods

    GEO_SHedge		 primary(GEO_SHedge sh) const;
    bool 		 isPrimary(GEO_SHedge sh) const;

    // Returns the right sign for h to have pt as it source.
    SYS_FORCE_INLINE
    int			 relativeSign(GEO_Hedge h, GA_Offset pt) const
    			    { return pt == myDhip->srcPoint(h) ? 1 : -1; }


    SYS_FORCE_INLINE
    GA_Edge		 gaEdge(GEO_Hedge h) const
			    { return GA_Edge(myDhip->srcPoint(h),
					     myDhip->dstPoint(h)); }

    SYS_FORCE_INLINE
    GA_Edge		 gaEdge(GEO_SHedge sh) const;

    using EdgeHeap = UT_PriorityQueue<PathEdge, PathEdge, false>;
    using HedgeInterface = GEO_DetachedHedgeInterface;
    using SuccessorMask = GU_EdgeSuccessor::Mask;

    const
    HedgeInterface	*myDhip;

    const GA_Detail 	*myGdp;
    GA_ROHandleV2	 myUV;

    GA_DataBitArray 	 myPosMarked, myNegMarked;
    GU_SHedgeArray 	 myPosPrev, myNegPrev;

    UT_Array<T>		 myPosCost, myNegCost;

    EdgeHeap		 myEdgeHeap;

    GU_SHedgeArray	 myStartSHedges;
    GU_SHedgeArray	 myStartSHedgePrimaries;
    GU_SHedgeArray	 myEndSHedges;
    GU_SHedgeArray	 myEndSHedgePrimaries;

    GEO_SHedge		 myLastStartSHedge		= GEO_INVALID_SHEDGE;
    bool		 myLastStartSHedgeFlag		= false;

    GEO_Hedge 		 myLastStartHedge		= GEO_INVALID_HEDGE;
    bool		 myLastStartHedgeFlag		= false;

    bool		 myStartOnBoundary		= false;
    bool		 myEndOnBoundary		= false;

    GA_Offset		 myLastStartPoint		= GA_INVALID_OFFSET;
    GA_Offset		 myLastStartPrimitive		= GA_INVALID_OFFSET;
    GA_Offset		 myLastStartVertex		= GA_INVALID_OFFSET;


    GA_EdgeGroup	*myAvoidEdges			= nullptr;
    GA_PointGroup	*myAvoidPoints			= nullptr;
    GA_PrimitiveGroup	*myAvoidPrimitives		= nullptr;
    GA_VertexGroup	*myAvoidVertices		= nullptr;

    SuccessorMask 	 mySuccessorTypeMask;
};

class gu_ShortestPathCost
{
public:
    			 gu_ShortestPathCost() = default;
    explicit 		 gu_ShortestPathCost(fpreal l) : myLength(l) { }

    			 gu_ShortestPathCost(GEO_SHedge sh,
                        	const GEO_DetachedHedgeInterface *dhip)
    			    { myLength = dhip->length(sh.hedge()); }

    			 gu_ShortestPathCost(const gu_ShortestPathCost &c)
				 = default;

    SYS_FORCE_INLINE
    void 		 zero() { myLength = 0.0; }

    SYS_FORCE_INLINE
    void 		 unset() { myLength = -1.0; }

    SYS_FORCE_INLINE
    bool 		 isSet() { return myLength > -0.5; }

    SYS_FORCE_INLINE
    bool 		 operator>(const gu_ShortestPathCost &c) const
		   	    { return myLength > c.myLength; }

    SYS_FORCE_INLINE
    gu_ShortestPathCost &operator=(const gu_ShortestPathCost &c) = default;

    SYS_FORCE_INLINE
    const
    gu_ShortestPathCost &operator+=(const gu_ShortestPathCost &c)
    {
	myLength += c.myLength;
	return *this;
    }

    SYS_FORCE_INLINE
    static
    gu_ShortestPathCost	 turnCost(const GEO_DetachedHedgeInterface *dhip,
			 	GEO_SHedge from_sh, GEO_SHedge to_sh,
				const GU_EdgeSuccessor &exits)
    {
	return gu_ShortestPathCost(dhip->length(to_sh.hedge()));
    }

    SYS_FORCE_INLINE
    fpreal 		 getLength() { return myLength; }

private:
    fpreal 		 myLength			= -1.0;
};

class gu_EdgeLoopCost
{
public:
    			 gu_EdgeLoopCost() = default;
    			 gu_EdgeLoopCost(GEO_SHedge sh,
                    		const GEO_DetachedHedgeInterface *dhip) :
				 myPenalty(0),
				 myPathLength(dhip->length(sh.hedge())) { }

    			 gu_EdgeLoopCost(fpreal len, int bends) :
				myPathLength(len), myPenalty(bends) { }

    			 gu_EdgeLoopCost(const gu_EdgeLoopCost &c) :
				myPathLength(c.myPathLength),
				myPenalty(c.myPenalty) { }

    SYS_FORCE_INLINE
    void 		 zero()
    {
	myPathLength = 0.0;
	myPenalty = 0;
    }

    SYS_FORCE_INLINE
    void 		 unset() { myPathLength = -1.0; }

    SYS_FORCE_INLINE
    bool 		 isSet() { return myPathLength > -0.5; }

    SYS_FORCE_INLINE
    bool 		 operator>(const gu_EdgeLoopCost &c) const
    {
	if (myPenalty != c.myPenalty)
	    return myPenalty > c.myPenalty;

	return myPathLength > c.myPathLength;
    }

    gu_EdgeLoopCost 	&operator=(const gu_EdgeLoopCost &c) = default;

    SYS_FORCE_INLINE
    const
    gu_EdgeLoopCost 	&operator+=(const gu_EdgeLoopCost &c)
    {
	myPenalty += c.myPenalty;
	myPathLength += c.myPathLength;
	return *this;
    }

    SYS_FORCE_INLINE
    static
    gu_EdgeLoopCost	 turnCost(const GEO_DetachedHedgeInterface *dhip,
				GEO_SHedge from_sh, GEO_SHedge to_sh,
				const GU_EdgeSuccessor &successors)
    {
	fpreal len = dhip->length(to_sh.hedge());

	GEO_SHedge bd_succ = successors(GU_EdgeSuccessor::BOUNDARY);
	if (bd_succ.isValid() && dhip->areEquivalent(to_sh, bd_succ))
	    return gu_EdgeLoopCost(len, 0);

	GEO_SHedge sided_quad_succ = successors(GU_EdgeSuccessor::QUAD_LEFT);
	if (!sided_quad_succ.isValid())
	    sided_quad_succ = successors(GU_EdgeSuccessor::QUAD_RIGHT);

	GEO_SHedge opposite = successors(GU_EdgeSuccessor::QUAD_OPPOSITE);
	if (!opposite.isValid())
	    opposite = successors(GU_EdgeSuccessor::OPPOSITE);

	bool to_sided_quad_succ = (sided_quad_succ.isValid() &&
	    dhip->areEquivalent(to_sh, sided_quad_succ));

	bool to_opposite = opposite.isValid() &&
	    dhip->areEquivalent(to_sh, opposite);

	if (to_sided_quad_succ)
	    return gu_EdgeLoopCost(len, 1);

	if (to_opposite)
	    return gu_EdgeLoopCost(len, 99);

	return gu_EdgeLoopCost(len, 100);
    }

    SYS_FORCE_INLINE
    fpreal 		 getLength() { return myPathLength; }

private:
    fpreal 		 myPathLength			= -1.0;
    int 		 myPenalty			= -1;
};



class gu_EdgeRingCost
{
public:
    			 gu_EdgeRingCost() = default;
    			 gu_EdgeRingCost(GEO_SHedge sh,
                      		const GEO_DetachedHedgeInterface *dhip) :
				 myPenalty(0),
				 myPathLength(dhip->length(sh.hedge())) { }

    			 gu_EdgeRingCost(fpreal len, int bends) :
				 myPathLength(len), myPenalty(bends) { }


    			 gu_EdgeRingCost(const gu_EdgeRingCost &c) = default;

    SYS_FORCE_INLINE
    void 		 zero()
    {
	myPathLength = 0.0;
	myPenalty = 0;
    }

    SYS_FORCE_INLINE
    void 		 unset() { myPathLength = -1.0; }

    SYS_FORCE_INLINE
    bool 		 isSet() { return myPathLength > -0.5; }

    SYS_FORCE_INLINE
    bool 		 operator>(const gu_EdgeRingCost &c) const
    {
	if (myPenalty != c.myPenalty)
	    return myPenalty > c.myPenalty;

	return myPathLength > c.myPathLength;
    }

    gu_EdgeRingCost 	&operator=(const gu_EdgeRingCost &c) = default;

    SYS_FORCE_INLINE
    const
    gu_EdgeRingCost 	&operator+=(const gu_EdgeRingCost &c)
    {
	myPenalty += c.myPenalty;
	myPathLength += c.myPathLength;
	return *this;
    }

    SYS_FORCE_INLINE
    static
    gu_EdgeRingCost	 turnCost(const GEO_DetachedHedgeInterface *dhip,
				GEO_SHedge from_sh, GEO_SHedge to_sh,
				const GU_EdgeSuccessor &successors)
    {
	fpreal len = dhip->length(to_sh.hedge());

	GEO_SHedge bd_succ = successors(GU_EdgeSuccessor::BOUNDARY);
	bool to_bd_succ = bd_succ.isValid() &&
	    dhip->areEquivalent(to_sh, bd_succ);

	if (to_bd_succ)
	    return gu_EdgeRingCost(len, 0);

	GEO_SHedge sided_quad_succ = successors(GU_EdgeSuccessor::QUAD_LEFT);
	if (!sided_quad_succ.isValid())
	    sided_quad_succ = successors(GU_EdgeSuccessor::QUAD_RIGHT);

	GEO_SHedge opposite = successors(GU_EdgeSuccessor::QUAD_OPPOSITE);
	if (!opposite.isValid())
	    opposite = successors(GU_EdgeSuccessor::OPPOSITE);

	bool to_sided_quad_succ = (sided_quad_succ.isValid() &&
	    dhip->areEquivalent(to_sh, sided_quad_succ));

	bool to_opposite = opposite.isValid() &&
	    dhip->areEquivalent(to_sh, opposite);

	if (to_sided_quad_succ)
	    return gu_EdgeRingCost(len, 1);

	if (to_opposite)
	    return gu_EdgeRingCost(len, 99);

	return gu_EdgeRingCost(len, 100);
    }

    SYS_FORCE_INLINE
    fpreal 		 getLength() { return myPathLength; }

private:
    fpreal 		 myPathLength			= -1.0;
    int 		 myPenalty			= -1;
};


// Marking of signed half-edges: Each half-edge in each orientation is
// given a flag which can be set or checked.

template <class T>
void
GU_PathFinder<T>::mark(GEO_SHedge sh)
{
    if (sh.isPositive())
	myPosMarked.set(myDhip->srcVertex(sh.hedge()), true);
    else
	myNegMarked.set(myDhip->srcVertex(sh.hedge()), true);

}

template <class T>
bool
GU_PathFinder<T>::isMarked(GEO_SHedge sh)
{
    if (sh.isPositive())
	return myPosMarked.get(myDhip->srcVertex(sh.hedge()));

    return myNegMarked.get(myDhip->srcVertex(sh.hedge()));
}

template <class T>
void
GU_PathFinder<T>::setBestCost(GEO_SHedge sh, const T &cost, GEO_SHedge prev_sh)
{
    GA_Offset src_vtx = myDhip->srcVertex(sh.hedge());
    if (sh.isPositive())
    {
	myPosCost(src_vtx) = cost;
	myPosPrev(src_vtx) = prev_sh;
    }
    else
    {
	myNegCost(src_vtx) = cost;
	myNegPrev(src_vtx) = prev_sh;
    }
}

template <class T>
T &
GU_PathFinder<T>::getBestCost(GEO_SHedge sh)
{
    if (sh.isPositive())
	return myPosCost(myDhip->srcVertex(sh.hedge()));
    else
	return myNegCost(myDhip->srcVertex(sh.hedge()));
}


template <class T>
GEO_SHedge
GU_PathFinder<T>::getPrev(GEO_SHedge sh)
{
    if (sh.isPositive())
	return myPosPrev(myDhip->srcVertex(sh.hedge()));
    else
	return myNegPrev(myDhip->srcVertex(sh.hedge()));
}


template <class T>
GA_Edge
GU_PathFinder<T>::gaEdge(GEO_SHedge sh) const
{
    return sh.isNegative() ?
	GA_Edge(myDhip->dstPoint(sh.hedge()), myDhip->srcPoint(sh.hedge())) :
	GA_Edge(myDhip->srcPoint(sh.hedge()), myDhip->dstPoint(sh.hedge()));
}



typedef GU_PathFinder<gu_EdgeLoopCost> 	GU_EdgeLoopFinder;
typedef GU_PathFinder<gu_EdgeRingCost> 	GU_EdgeRingFinder;

enum GU_WalkEndReason
{
    GU_NO_SUCCESSOR,		// No viable successors to take
    GU_HIT_BOUNDARY,		// Reached a boundary
    GU_HIT_SELF,		// Crossed itself
    GU_COMPLETED		// Closed the path
};

GU_WalkEndReason GU_API  guEdgeWalk(const GEO_DetachedHedgeInterface *dhip,
				const GA_ROHandleV2 &uvh,
				GU_SHedgeArray &path,
				GU_SHedgeArray &walk,
				bool backward = false,
				bool no_self_intersection = false,
				bool include_ends = false);

GU_WalkEndReason GU_API  guDualEdgeWalk(const GEO_DetachedHedgeInterface *dhip,
				const GA_ROHandleV2 &uvh,
				GU_SHedgeArray &path,
				GU_SHedgeArray &walk,
				bool backward = false,
				bool no_self_intersection = false,
				bool include_ends = false);

#endif

