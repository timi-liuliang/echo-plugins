/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_SurfaceDistance.h (GU Library, C++)
 *
 * COMMENTS:
 *      Approximates geodesic distance.
 *      This code is related to the soft falloff code in GEO_Transform, but is
 *      mostly a replacement for it as of Houdini 16.5. 
 */

#ifndef __GU_SurfaceDistance_h__
#define __GU_SurfaceDistance_h__

#include "GU_API.h"

#include <GA/GA_Types.h>
#include <GA/GA_Handle.h>
#include <GEO/GEO_Detail.h>
#include <UT/UT_Map.h>
#include <UT/UT_PriorityQueue.h>
#include <UT/UT_Set.h>

class SoftSymmetryParms;

// keep this synced with the parameter in $PRM/PRM_Shared.C
enum GU_API GU_SoftDistanceMetric
{
    GU_SOFT_CUSTOM = 0,
    GU_SOFT_DISTANCE_ALONG_CONNECTED_EDGES,
    GU_SOFT_GLOBAL_DISTANCE,
    GU_SOFT_GLOBAL_DISTANCE_WITH_CONNECTIVITY,
    GU_SOFT_SURFACE_DISTANCE,
    GU_SOFT_METRIC_INVALID
};

// This class contains all the data needed to efficiently compute surface
// distances.  It will automatically invalidate cache items when inputs to the
// query are incompatible with the cached data.  This can help SOPs that want
// to save time on repeated calls, typically due to small changes in the
// radius.
class GU_API GU_SurfaceDistanceCache
{
    // set of points used to identify an edge, the offsets are always specified
    // in ascending order
    typedef std::pair<GA_Offset, GA_Offset> Edge;

    // describes distances and lead points for the specified edge
    struct QueueEntry
    {
	QueueEntry(const Edge &e, fpreal len,
		   fpreal mindist, fpreal dist0, fpreal dist1,
		   GA_Offset src0, GA_Offset src1)
	    : myEdge(e)
	    , myEdgeLength(len)
	    , myMinDist(mindist)
	    , myDist0(dist0)
	    , myDist1(dist1)
	    , mySrc0(src0)
	    , mySrc1(src1)
	    , myPos(-1)
	{}

	Edge myEdge;
	fpreal myEdgeLength;
	fpreal myMinDist; // distance to edge
	fpreal myDist0; // distance to myEdge.first
	fpreal myDist1; // distance to myEdge.second
	GA_Offset mySrc0; // closest lead point to myEdge.first
	GA_Offset mySrc1; // closest lead point to myEdge.second
	unsigned int myPos; // position in the priority queue
    };

    // order items in the priority queue so edges with smaller distance bounds
    // are explored first
    class QueueEntryCmp
    {
    public:
	bool operator()(const QueueEntry *a, const QueueEntry *b) const
		{ return a->myMinDist > b->myMinDist; }
    };

    class PriorityQueue :
	public UT_PriorityQueue<QueueEntry *, QueueEntryCmp, true>
    {
    public:
	virtual ~PriorityQueue() {}
	virtual void changedPosition(QueueEntry *e, unsigned int idx) const
	    { e->myPos = idx; }
    };

    // This is a utility class for traversing geometry.  It will respect the
    // side of the symmetry plane assigned in the constructor.
    class Walker
    {
    public:
	Walker(int side) : myRadius(-1), mySide(side) {}
	~Walker() { clear(); }

	void clear();

	// adds an edge to the graph where pt0 is the closest point
	void insertEdge(GA_Offset pt0, GA_Offset pt1, fpreal dist,
			GA_Offset src, const GA_ROHandleV3 &p_attrib,
			const GEO_Detail::SoftSymmetryParms *symmetry);
	// adds an edge to the graph where traversal across a primitive to
	// pt0 and pt1 are possible
	void insertEdge(const GEO_Detail &gdp, const QueueEntry &qe,
			GA_Offset pt0, GA_Offset pt1,
			const GA_ROHandleV3 &p_attrib,
			const GEO_Detail::SoftSymmetryParms *symmetry);

	// returns true if all edges have been traversed
	bool isEmpty(fpreal radius) const
	    { return myQueue.isEmpty() || myRadius > radius; }

	// returns next edge to traverse and marks it as visited
	QueueEntry *pop()
	{
	    QueueEntry *qe = myQueue.head();
	    myQueue.remove(0);
	    qe->myPos = -1;

	    myRadius = qe->myMinDist;
	    return qe;
	}

    private:
	// returns true if the edge has already been visited
	bool isVisited(GA_Offset pt0, GA_Offset pt1) const;
	void insertEdge(GA_Offset pt0, GA_Offset pt1, fpreal len,
			fpreal dist, fpreal dist0, fpreal dist1,
			GA_Offset src0, GA_Offset src1);

	// set of all discovered edges (including visited ones)
	UT_Map<Edge, QueueEntry *> myEdges;
	// queue of edges needing to be visited
	PriorityQueue myQueue;
	// all points closer than this radius have already been discovered
	fpreal myRadius;
	// side of the symmetry plane being respected
	int mySide;
    };

public:
    GU_SurfaceDistanceCache() { clear(); }
    ~GU_SurfaceDistanceCache() { clear(); }

    void clear();

    // utility method to support multithreading of the surfdist VEX function
    bool canReuse(const GEO_Detail &gdp, fpreal radius,
		 GU_SoftDistanceMetric metric) const;

    // group of points with valid distances (and lead points)
    const GA_PointGroup *getAffectedGroup() const
	{ return myAffectedGroup.get(); }

    // handle to detached attribute describing measured distances
    GA_ROHandleF getDistances() const
	{ return GA_ROHandleF(myDistances.get()); }

    // handle to detached attribute describing closest lead point
    GA_ROHandleID getSourcePoints() const
	{ return GA_ROHandleID(mySourcePoints.get()); }

    // compute distances for points within the radius
    void updateDistances(const GEO_Detail &gdp,
			 const GA_Group *srcgroup,
			 fpreal radius,
			 GU_SoftDistanceMetric metric,
			 const char *p_attr = "P",
			 const GEO_Detail::SoftSymmetryParms *symmetry = nullptr,
			 const GEO_Rolloff *rolloff = nullptr);

private:
    // invalidate cache if needed
    void clearIfChanged(const GEO_Detail &gdp,
			const GA_PointGroup *srcgroup,
			fpreal radius,
			GU_SoftDistanceMetric metric,
			const GA_ROHandleV3 &p_attrib,
			const GEO_Detail::SoftSymmetryParms *symmetry);

    void getAdjacentBoundaryVertices(GA_OffsetArray &verts,
				     const GEO_Detail &gdp, GA_Offset vtx);

    void getFaceVertices(UT_Array<GA_OffsetArray> &verts, const GEO_Detail &gdp,
			 GA_Offset vtx0, GA_Offset vtx1);

    void updateSurfAndEdgeDistances(const GEO_Detail &gdp,
				    const GA_PointGroup *srcgroup,
				    fpreal radius,
				    GU_SoftDistanceMetric metric,
				    const GA_ROHandleV3 &p_attrib,
				    const GEO_Detail::SoftSymmetryParms *symmetry);

    UT_SortedMap<int, Walker *> myWalkers;
    // we convert hulls and polysoups to a polygon cage representation for
    // efficiently measuring surface distances
    GA_OffsetArray myVertices;
    UT_Array<exint> myPolygons;
    UT_Map<GA_Offset, UT_Array<std::pair<exint, exint> > > myLookup;
    UT_Set<exint> myCurves;

    // set of points with computed distances (and lead points)
    UT_UniquePtr<GA_PointGroup> myAffectedGroup;
    // distance to the closest point in the input group
    UT_UniquePtr<GA_Attribute> myDistances;
    // the point in the input group that the current point is closest to
    UT_UniquePtr<GA_Attribute> mySourcePoints;

    // values used to determine when to invalidate cached data
    GU_SoftDistanceMetric myMetric;
    fpreal myRadius;
    exint myGdpId;
    exint myGdpPrimitiveId;
    exint myGdpTopologyId;

    bool myHasSymmetryParms;
    bool myHasPtGroup;
    GEO_Detail::SoftSymmetryParms mySymmetryParms;
    UT_String myPAttrib;
    GA_OffsetArray myPts;
};

#endif
