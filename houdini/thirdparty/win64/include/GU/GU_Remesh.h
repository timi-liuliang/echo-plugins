/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Remesh.h (GU Library, C++)
 *
 * COMMENTS:s
 */


#ifndef GU_REMESH_H_
#define GU_REMESH_H_

#include <GA/GA_ElementWrangler.h>
// #include <GA/GA_Topology.h>
#include "GU_Detail.h"
#include <GEO/GEO_HedgeInterface.h>

#include "GU_API.h"

typedef enum {
    POINT_STATUS_NONE 		= 0,
    POINT_STATUS_INGROUP	= 1 << 0,
    POINT_STATUS_BOUNDARY 	= 1 << 1,
    POINT_STATUS_NONMANIFOLD 	= 1 << 2,
    POINT_STATUS_HARDENED 	= 1 << 3,
    POINT_STATUS_TARGET_DEGREE  = 0xF0
} guPointStatusMask;


class GU_API GU_Remesh
{
public:
    GU_Remesh(GU_Detail *gdp, GA_PrimitiveGroup *group, GA_EdgeGroup *hard_edges);
    ~GU_Remesh();

    void
    remesh(bool adaptive, int iterations, fpreal density, fpreal gradation,
	   fpreal target_edge_len = -1.0, fpreal min_edge_len = -1.0, fpreal max_edge_len = -1.0,
	   fpreal smoothing_level = 0.0, bool use_input_pts_only = false);

private:

    inline bool
    isBoundaryPoint(GA_Offset pt)
    {
	return (myPointStatus.get(pt) & POINT_STATUS_BOUNDARY);
    }

    inline bool
    isManifoldPoint(GA_Offset pt)
    {
	return !(myPointStatus.get(pt) & POINT_STATUS_NONMANIFOLD);
    }

    inline bool
    isHardenedPoint(GA_Offset pt)
    {
	return (myPointStatus.get(pt) & POINT_STATUS_HARDENED);
    }

    inline bool
    isGroupPoint(GA_Offset pt)
    {
	return (myPointStatus.get(pt) & POINT_STATUS_INGROUP);
    }

    inline void
    setPointStatus(GA_Offset pt, uint8 mask)
    {
	myPointStatus.set(pt, myPointStatus.get(pt) | mask);
    }

    inline void
    clearPointStatus(GA_Offset pt, uint8 mask)
    {
	myPointStatus.set(pt, myPointStatus.get(pt) & ~mask);
    }

    inline void
    setTargetDegree(GA_Offset pt, uint8 deg)
    {
	if (deg > 0x0F)
	    deg = 0x0F;

	uint8 val = myPointStatus.get(pt);
	uint8 mask = deg << 4;
	val &= 0x0F;
	val |= mask;
	myPointStatus.set(pt, val);
    }

    inline int
    getTargetDegree(GA_Offset pt)
    {
	return (((uint8) myPointStatus.get(pt)) >> 4);
    }

    bool isGroupBoundaryHedge(GEO_Hedge e);

    void updateWranglers()
    {
	if (myWranglers)
	    delete myWranglers;

	myWranglers = new GA_ElementWranglerCache(*myGdp,
						  GA_PointWrangler::EXCLUDE_P);
	myWranglers->getPoint();
	myWranglers->getVertex();

	myHdp->setWranglerCache(myWranglers);
    }


    void markHardPoints();
    void computePointNormals(GA_Attribute * attr, bool compute_deviations = true);
    void computePointNormalsNew(GA_Attribute *atN);

    // remeshing stages:
    int splitLongEdges();
    int contractShortEdges(bool use_curvature = false);
    int equalizeValences();
    int relaxPoints(bool use_curvature = false, fpreal smooting_level = 0.0);
    int deleteDegree3Points();

    // The main remesh method:
    void runIterations(int itertions, fpreal smoothing_level, bool input_pts_only = false);

    float bestQuadRidgeExtension(GA_Offset p, GA_VertexGroup *quadridge,
			     GA_RWHandleI &crossed, GA_Offset &v1,
			     GA_Offset &v2);
    void generateQuads();

    void addEdgeToVertexGroup(GA_VertexGroup *grp, GA_Offset v);

    // ???
    void delaunizeEdge(GEO_Hedge e);
    int makeMeshLocallyDelaunay();
    int delaunizeHardPointAdjacentEdges();

    // size function estimation
    void computeInitialLfs();
    int computeBasicLfs(GA_RWHandleF fh);
    int computeLFS(int max_splits = -1);

    GA_Offset tryCollapseHedge(GEO_Hedge e, UT_Array < GEO_Hedge > *shortened, bool checklength = true);

    inline fpreal targetLengthMin(GEO_Hedge e);
    inline fpreal targetLengthMax(GEO_Hedge e);


    int manifoldPatchDegree(GA_Offset vtx);
    fpreal manifoldPatchAngle(GA_Offset vtx);

    // void verifyOrientations();
    fpreal angleAtHedge(GEO_Hedge e);

    bool isHardPoint(GA_Offset pt)
    {
	return myHardPoints ? myHardPoints->containsOffset(pt) : false;
    }

    bool isHardHedge(GA_Offset vtx)
    {
	return myHardHedges ? myHardHedges->containsOffset(vtx) : false;
    }

    bool isHardHedge(GEO_Hedge e)
    {
	return
	    myHardHedges ?
		myHardHedges->containsOffset(myHdp->srcVertex(e)) : false;
    }

    void hardenEdge(GEO_Hedge e);


    GEO_Hedge splitHedge(GEO_Hedge e, fpreal ratio);

    fpreal getLFSUpperBound(GA_Offset x, GA_PointGroup *marked);

    bool splitOpposite(UT_Vector3 &pv, fpreal lfsp, UT_Vector3 &qv, fpreal lfsq,
		   UT_Vector3 &rv, fpreal lfsr, fpreal &ratio, bool verbose);

    void relaxLFSAtPoint(GA_Offset p, GA_PointGroup *marked, GA_PointGroup *border, GA_RWHandleI &srch,
		     GA_RWHandleF &gdsrch, GA_OffsetArray &newpts);

    GA_Offset getBestSrc(GA_Offset x, GA_PointGroup *marked, GA_RWHandleI& srch,
		     GA_RWHandleF& gdsrch, fpreal &res);

    GA_Offset splitHedgeAtBisector(GEO_Hedge e, fpreal lfsp, fpreal lfsq,
			       GA_PointGroup *marked, GA_RWHandleI& srch,
			       GA_RWHandleF& gdsrch);
    bool splitOppositeSimple(UT_Vector3 &pv, fpreal lfsp, UT_Vector3 &qv,
				     fpreal lfsq, UT_Vector3 &rv, fpreal lfsr);

    fpreal worstLinearApprox(UT_Vector3 &pv, fpreal lfsp, UT_Vector3 &qv,
			      fpreal lfsq, UT_Vector3 &rv, fpreal lfsr);

private:
    GA_ElementWranglerCache 	*myWranglers;

    GU_Detail 			*myGdp;
    GEO_HedgeInterface 	*myHdp;
    GA_PrimitiveGroup		*myGroup;


    GA_Attribute 		*myAttrLfs;	// local feature size (target edge length at point)
    GA_RWHandleF 		myLfs;
    GA_Attribute 		*myAttrN;	// point normals
    GA_RWHandleV3 		myN;

    GA_Attribute 		*myAttrNdiv;		// normal deviation at point
    GA_RWHandleF 		myNdiv;

    GA_Attribute		*myAttrPointStatus;
    GA_RWHandleI		myPointStatus;

    GA_EdgeGroup 		*myInputHardEdges;	// input edge group for crease edges
    GA_VertexGroup 		*myHardHedges;
    GA_PointGroup 		*myHardPoints;
    GA_PointGroup		*myBoundaryPoints;	// points incident to edges belonging to only one input polygon
    GA_PointGroup		*myNonManifoldPoints;	// nonmanifold points

    fpreal 			myMinLfs, myMaxLfs;
    fpreal 			myDensity;
    fpreal 			myGradation;
    fpreal 			myExpansion;
    fpreal 			myApproxFactor;
    fpreal 			myMinEdgeLength;
    fpreal 			myMaxEdgeLength;
    fpreal 			myTargetEdgeLength;

    bool 			myAdaptive;
    bool 			myLimitAbove;
    bool 			myLimitBelow;
    fpreal 			myHigh, myLow, myLocalHigh, myLocalLow;
};


#endif /* GU_REMESH_H_ */
