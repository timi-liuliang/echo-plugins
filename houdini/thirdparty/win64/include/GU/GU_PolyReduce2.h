/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Polygonal reduction tool (C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PolyReduce_h__
#define __GU_PolyReduce_h__

#include "GU_API.h"
#include "GU_Detail.h"

#include <GEO/GEO_PolyInterface.h>

#include <UT/UT_Array.h>
#include <UT/UT_Classifier.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Map.h>
#include <UT/UT_StringStream.h>

#include <SYS/SYS_Types.h>

class GA_EdgeGroup;
class GA_PrimitiveGroup;

namespace GU_PolyReduce2
{

using HedgeArray = UT_Array<GEO_Hedge>;
using SHedgeArray = UT_Array<GEO_SHedge>;
using BoolArray = UT_Array<bool>;

class GU_API Parms
{
public:
    			 Parms() = default;

    			 Parms(const Parms &src);

    SYS_FORCE_INLINE
    fpreal		 boundaryWeight() const { return myBoundaryWeight; }
    void 		 boundaryWeight(fpreal w) { myBoundaryWeight = w; }
    
    SYS_FORCE_INLINE
    fpreal		 softFeatureEdgeWeight() const
    			    { return mySoftFeatureEdgeWeight; }
    void 		 softFeatureEdgeWeight(fpreal w)
    			    { mySoftFeatureEdgeWeight = w; }
    
    SYS_FORCE_INLINE
    fpreal		 softFeaturePointWeight() const
    			    { return mySoftFeaturePointWeight; }
    void 		 softFeaturePointWeight(fpreal w)
    			    { mySoftFeaturePointWeight = w; }

    SYS_FORCE_INLINE
    fpreal		 seamWeight() const
    			    { return mySeamWeight; }

    void 		 seamWeight(fpreal w)
    			    { mySeamWeight = w; }

    void 		 seamAttribs(const UT_StringHolder &attribs)
    			    { mySeamAttribs = attribs; }

    const UT_String	&seamAttribs() const { return mySeamAttribs; }

    SYS_FORCE_INLINE
    fpreal		 retentionWeight() const { return myRetentionWeight; }
    void 		 retentionWeight(fpreal w) { myRetentionWeight = w; }

    SYS_FORCE_INLINE
    fpreal		 silhouetteWeight() const { return mySilhouetteWeight; }
    void 		 silhouetteWeight(fpreal w) { mySilhouetteWeight = w; }

    SYS_FORCE_INLINE
    fpreal		 frontFacingWeight() const
    			    { return myFrontFacingWeight; }

    void 		 frontFacingWeight(fpreal w)
    			    { myFrontFacingWeight = w; }

    SYS_FORCE_INLINE
    fpreal		 silhouetteFalloffDist() const
    			    { return mySilhouetteFalloffDist; }

    void 		 silhouetteFalloffDist(fpreal d)
    			    { mySilhouetteFalloffDist = d; }

    SYS_FORCE_INLINE
    fpreal		 frontFacingFalloffDist() const
    			    { return myFrontFacingFalloffDist; }

    void 		 frontFacingFalloffDist(fpreal d)
    			    { myFrontFacingFalloffDist = d; }

    SYS_FORCE_INLINE
    fpreal		 lengthWeight() const
    			    { return myLengthWeight; }

    void 		 lengthWeight(fpreal w)
    			    { myLengthWeight = w; }

    SYS_FORCE_INLINE
    fpreal		 quadCollapseTolerance() const
    			    { return myQuadCollapseTolerance; }

    void 		 quadCollapseTolerance(fpreal w)
    			    { myQuadCollapseTolerance = w; }

    SYS_FORCE_INLINE
    bool		 snapToExisting() const { return mySnapToExisting; }
    void 		 snapToExisting(bool b) { mySnapToExisting = b; }


    struct ControlAttrib
    {
	GA_Attribute 	*attrib;
	fpreal		 weight;
	bool		 active;
    };

    using ControlAttribArray = UT_Array<ControlAttrib>;

    void		 addControlAttrib(GA_Attribute *attrib, fpreal weight,
				bool active)
    {
	myControlAttribs.append();
	myControlAttribs.last().attrib = attrib;
	myControlAttribs.last().weight = weight;
	myControlAttribs.last().active = active;
    }

    const
    ControlAttribArray	&controlAttribs() const { return myControlAttribs; }

private:

    ControlAttribArray	 myControlAttribs;

    fpreal 		 myBoundaryWeight 		= 0.0;
    fpreal 		 mySoftFeatureEdgeWeight 	= 0.0;
    fpreal 		 mySoftFeaturePointWeight 	= 0.0;
    fpreal 		 mySeamWeight 			= 0.0;
    fpreal 		 mySilhouetteWeight 		= 0.0;
    fpreal 		 mySilhouetteFalloffDist	= 0.0;
    fpreal 		 myFrontFacingWeight 		= 0.0;
    fpreal 		 myFrontFacingFalloffDist	= 0.0;
    fpreal 		 myRetentionWeight 		= 0.0;
    fpreal 		 myLengthWeight 		= 0.0;
    fpreal 		 myQuadCollapseTolerance 	= 0.0;

    UT_String		 mySeamAttribs;
    bool 		 mySnapToExisting 		= false;
};

class RingSet;

class GU_API Mesh
{
public:
    using Parms = GU_PolyReduce2::Parms;

			 Mesh() = default;

    explicit		 Mesh(GU_Detail *gdp,
			 	const GA_PrimitiveGroup *prims = nullptr);

    			~Mesh();

    SYS_FORCE_INLINE
    bool		 hasGroup() const { return myPolys != nullptr; }

    SYS_FORCE_INLINE
    GA_PrimitiveGroup	*getGroup() { return myPolys; }

    SYS_FORCE_INLINE
    GA_Range		 groupPolys()
    			    { return myGdp->getPrimitiveRange(myPolys); }

    SYS_FORCE_INLINE
    GA_Range		 nonGroupPrims()
    			    { return GA_Range(myGdp->getPrimitiveMap(), myPolys,
					      /* complement */ true); }

    SYS_FORCE_INLINE
    bool		 hasPoly(GA_Offset poly) const
    			    { return myPolys == nullptr
					|| myPolys->containsOffset(poly); }

    SYS_FORCE_INLINE
    GA_Offset 		 vertexPoint(GA_Offset vtx) const
    			    { return myGdp->vertexPoint(vtx); }

    SYS_FORCE_INLINE
    GA_Offset 		 pointVertex(GA_Offset vtx) const
    			    { return myGdp->pointVertex(vtx); }

    SYS_FORCE_INLINE
    GA_Offset 		 vertexToNextVertex(GA_Offset vtx) const
    			    { return myGdp->vertexToNextVertex(vtx); }

    SYS_FORCE_INLINE
    GEO_Hedge		 polyHedge(GA_Offset poly) const
    			    { return myPip->polyHedge(poly); }

    SYS_FORCE_INLINE
    GA_Offset		 hedgePoly(GEO_Hedge h) const
    			    { return myPip->hedgePoly(h); }

    SYS_FORCE_INLINE
    GA_Offset		 vertexPoly(GA_Offset vtx) const
    			    { return myGdp->vertexPrimitive(vtx); }

    SYS_FORCE_INLINE
    GEO_Hedge		 lnext(GEO_Hedge h) const
    			    { return myPip->lnext(h); }

    SYS_FORCE_INLINE
    GEO_Hedge		 lprev(GEO_Hedge h) const
    			    { return myPip->lprev(h); }

    SYS_FORCE_INLINE
    GEO_Hedge		 sym(GEO_Hedge h) const
    			    { return myPip->sym(h); }

    SYS_FORCE_INLINE
    GA_Offset 		 srcPoint(GEO_Hedge h) const
    			    { return myPip->srcPoint(h); }

    SYS_FORCE_INLINE
    GA_Offset 		 srcVertex(GEO_Hedge h) const
    			    { return myPip->srcVertex(h); }

    SYS_FORCE_INLINE
    GA_Offset		 dstPoint(GEO_Hedge h) const
    			    { return myPip->dstPoint(h); }

    SYS_FORCE_INLINE
    GA_Offset 		 dstVertex(GEO_Hedge h) const
    			    { return myPip->dstVertex(h); }

    SYS_FORCE_INLINE
    GA_Offset		 preSrcPoint(GEO_Hedge h) const
    			    { return myPip->preSrcPoint(h); }

    SYS_FORCE_INLINE
    bool		 isValid(GEO_Hedge h) const
    			    { return myPip->isValidHedge(h); }

    SYS_FORCE_INLINE
    bool		 isPrimary(GEO_Hedge h) const
    			    { return myPip->isPrimary(h); }

    SYS_FORCE_INLINE
    GEO_Hedge		 primary(GEO_Hedge h) const
    			    { return myPip->primary(h); }

    SYS_FORCE_INLINE
    bool		 isValidPoly(GA_Offset poly) const
    			    { return myPip->isValidPoly(poly); }

    SYS_FORCE_INLINE
    GA_Offset		 polyFirst(GA_Offset vtx) const
    			    { return myPip->polyFirst(vtx); }

    SYS_FORCE_INLINE
    GA_Offset		 polyNext(GA_Offset vtx) const
    			    { return myPip->polyNext(vtx); }

    SYS_FORCE_INLINE
    GA_Offset		 polyPrev(GA_Offset vtx) const
    			    { return myPip->polyPrev(vtx); }

    SYS_FORCE_INLINE
    bool		 hasNonTris() const { return myNumNonTris > 0; }

    SYS_FORCE_INLINE
    bool		 hasQuads() const { return myNumQuads > 0; }

    SYS_FORCE_INLINE
    exint		 numNonTris() const { return myNumNonTris; }

    SYS_FORCE_INLINE
    exint		 numQuads() const { return myNumQuads; }


    SYS_FORCE_INLINE
    bool		 isQuad(GA_Offset poly) const
    			    { return myPip->vertexCount(poly) == 4; }

    SYS_FORCE_INLINE
    bool		 isQuadHedge(GEO_Hedge h) const
    			    { return isQuad(hedgePoly(h)); }

    SYS_FORCE_INLINE
    GEO_Hedge		 quadOpposite(GEO_Hedge h) const
    {
	GEO_Hedge h_opp = lnext(lnext(h));
	return lnext(lnext(h_opp)) == h ? h_opp : GEO_INVALID_HEDGE;
    }


    SYS_FORCE_INLINE
    int			 vertexCount(GA_Offset poly) const
    			    { return myPip->vertexCount(poly); }

    SYS_FORCE_INLINE
    exint		 numPolys() const
    			    { return myPolys == nullptr
				     ? myGdp->getNumPrimitives()
				     : myPolys->entries(); }
    SYS_FORCE_INLINE
    exint		 numPoints() const
    			    { return myGdp->getNumPoints()
				     - myNumNonGroupPts; }

    GU_Detail		*getDetail() { return myGdp; }
    GEO_PolyInterface	*getPolyInterface()  { return myPip; }


    struct PtVtxIterator
    {
	SYS_FORCE_INLINE PtVtxIterator() = default;
	SYS_FORCE_INLINE PtVtxIterator(const GU_Detail *gdp, GA_Offset pt,
				const GA_PrimitiveGroup *grp) :
			    myGdp(gdp), myVtx(gdp->pointVertex(pt)),
			    myGroup(grp)
	{
	    if (myGroup)
		groupAdvance();
	}

	SYS_FORCE_INLINE
	GA_Offset 	 operator*() const { return myVtx; }

	SYS_FORCE_INLINE
	bool 		 operator!=(const PtVtxIterator &other) const
				{ return myVtx != other.myVtx; }

	SYS_FORCE_INLINE
	PtVtxIterator 	&operator++()
	{
	    myVtx = myGdp->vertexToNextVertex(myVtx);
	    if (myGroup)
		groupAdvance();
	    return *this;
	}

    private:

	SYS_FORCE_INLINE
	void		 groupAdvance()
	{
	    while (GAisValid(myVtx)
		   && !myGroup->containsOffset(myGdp->vertexPrimitive(myVtx)))
		myVtx = myGdp->vertexToNextVertex(myVtx);
	}

	using Group = GA_PrimitiveGroup;
	GA_Offset	 myVtx				= GA_INVALID_OFFSET;
	const GU_Detail	*myGdp				= nullptr;
	const Group	*myGroup			= nullptr;
    };

    struct PtVtxRange
    {
	SYS_FORCE_INLINE PtVtxRange(const GU_Detail *gdp, GA_Offset pt,
				const GA_PrimitiveGroup *grp) :
			    myGdp(gdp), myPt(pt), myGroup(grp) { }

	SYS_FORCE_INLINE
	PtVtxIterator 	 begin() const
			    { return PtVtxIterator(myGdp, myPt, myGroup); }

	SYS_FORCE_INLINE
	PtVtxIterator 	 end() const
			    { return PtVtxIterator(); }

    private:
	using Group = GA_PrimitiveGroup;
	const
	GU_Detail 	*myGdp;
	GA_Offset 	 myPt;
	const Group 	*myGroup;
    };

    PtVtxRange		 pointVertices(GA_Offset pt)
    			    { return PtVtxRange(myGdp, pt, myPolys); }

    SYS_FORCE_INLINE
    GA_Offset		 contract(GEO_Hedge h, bool collapse_on_dst)
    {
	return myPip->contract(h, collapse_on_dst, -1.0,
		               /* check_contractible */ false,
		               /* internal */ false);
    }

protected:
    exint		 myNumQuads 			= 0;
    exint		 myNumNonTris			= 0;
    exint		 myNumNonGroupPts		= 0;

    GA_PrimitiveGroup	*myPolys			= nullptr;
    GU_Detail		*myGdp				= nullptr;
    GEO_PolyInterface	*myPip				= nullptr;

};

template <typename T>
class GU_API DecimatorT
{
public:
    using AttribArray = UT_Array<GA_Attribute *>;

    			 DecimatorT(const GU_Detail *gdp, const Parms &parms,
			    	const GA_PrimitiveGroup *prims = nullptr,
				const GA_PointGroup *hard_feature_pts = nullptr,
				const GA_EdgeGroup *hard_feature_edges = nullptr,
				const GA_PointGroup *soft_feature_pts = nullptr,
				const GA_EdgeGroup *soft_feature_edges = nullptr,
				const GA_Attribute *retention_attrib = nullptr,
				bool preserve_quads = false,
			 	const GU_Detail *rest_gdp = nullptr,
			 	const GU_Detail *view_gdp = nullptr,
			 	bool build_cache = true);

    			~DecimatorT();

    // Returns the final number of triangles.
    fpreal		 reduce(GU_Detail *gdp, exint target_num_polys,
			    	exint target_num_pts, T min_error,
				T max_error,
				const GU_Detail *rest_gdp = nullptr,
    				const char* error_attrib_name = nullptr);

    exint		 initialNumPolys() const
    			    { return myInitialNumPolys; }

    exint 		 initialNumPoints() const
    			    { return myInitialNumPoints; }

    // Check whether we're done with reduction for the required numbers.
    // Negative values for target_num_tris and target_num_pts are treated
    // as 'don't care'. If both negative, there's nothing to achieve.

    int			 numWarnings() const { return int(myWarnings.size()); }
    const UT_StringRef	&getWarning(int i) const { return myWarnings(i); }

private:

    using TArray = UT_Array<T>;
    using TVector2 = UT_Vector2T<T>;
    using TVector3 = UT_Vector3T<T>;
    using TVector4 = UT_Vector4T<T>;
    using TVector2Array = UT_Array<TVector2>;
    using TVector3Array = UT_Array<TVector3>;
    using TVector4Array = UT_Array<TVector4>;
    using TSymMatrix3 = UT_SymMatrix3T<T>;
    using TSymMatrix3Array = UT_Array<TSymMatrix3>;
    using TMatrix3 = UT_Matrix3T<T>;

    SYS_FORCE_INLINE
    fpreal		 hasReachedTarget(exint target_num_polys,
				exint target_num_pts, T min_error,
				T max_error) const
    {
	if (min_error >= 0)
	{
	    if (myMesh && (!myQueue
			   || (myQueue->size() > 0
			       && (*myQueue)(0).first <= min_error)))
		return false;
	}

	return (target_num_polys >= myMesh->numPolys())
	        || (target_num_pts >= myMesh->numPoints()
		|| (max_error >= 0 && myLastCollapseCost >= max_error));
    }

    class Quadric;
    class Affine;
    class AttribHandler;
    class WedgeSet;
    class WedgeBundle;


    // Return an ordered list of representative vertices incident to the
    // given point, one per incident wedge in the given wedge set.
    void		 getPointVtxWedgeRepVtxs(WedgeSet *wset, GA_Offset pt,
				UT_Array<GA_Offset> &rep_vtxs);

    // Processes the view geometry and generates a collection of view groups,
    // each consisting of a number of points returned in view_pts, with the
    // index of the first element of each group returned in view_grp_first.
    // NB. The latter array has an extra tail element set to the length of
    // view_pts.
    void 		 findViewGroups(TVector3Array &view_pts,
			      	UT_IntArray &view_grp_first);

    // Calculate position based error quadric for all of the mesh points.
    void 		 buildPosWedgeSet(const GA_Attribute *retention_attr,
				const GA_PointGroup *soft_feature_pts,
				TVector3Array &poly_normals);

    // Generate a temporary triangulation for all the non-triangle polygons
    // in the mesh. poly_normals pass the precalculated normals for the
    // polygons which are only used in case of n-gons with n >= 5.
    // tri_vtxs return vertex offsets of the triangles in sequences of 3.
    // poly_first_tri_vtx holds for each primitive its corresponding start
    // of the triangles in tri_vtxs. NB. the latter array contains a trailing
    // element equal to the length of tri_vtxs.
    exint 		 buildTriangulation(const TVector3Array &poly_normals,
    				GA_OffsetArray &tri_vtxs,
				UT_Int32Array &poly_first_tri_vtx);

    // Generate wedge bundles for point and vertex attributes.
    void		 buildWedgeBundles(const TVector3Array &poly_normals);

    // Go over all edge and merges of vertex wedges of all vertex attribute
    // wedge sets for the end vertices if their values are the same across the
    // edge. This is essentially the process that builds nontrivial "wedges"
    // out of vertices. Every other reference to wedges, e.g. point attribute
    // wedges, refer to trivial wedges which coincide with a single element.
    void		 mergeVtxWedgesAcrossEdges(HedgeArray &boundary_hedges,
				HedgeArray &seam_hedges);


    // Add virtual quadrics to protect a single edge.
    void		 addHedgeQuadric(GEO_Hedge h, T wt,
				const GA_Attribute *retention_attrib);

    // Add virtual quadrics to protect boundaries. These will correspond to
    // planes orthogonal to the boundary edges with appropriate set "areas".
    void		 addBoundaryQuadrics(const HedgeArray &hedges, T wt,
    				const GA_Attribute *retention_attrib);

    // Add virtual quadrics to protect vertex attribute seams (discontinuities).
    void 		 addSeamQuadrics(const HedgeArray &hedges, T wt,
    				const GA_Attribute *retention_attrib);

    // Add virtual quadrics to protect vertex attribute seams (discontinuities).
    void 		 addSoftFeatureEdgeQuadrics(const GA_EdgeGroup *edges,
				T wt, const GA_Attribute *retention_attrib);


    // Extract the attributes from the wedges and write them into the detail.
    // This is only used for the uncached use of the decimator where the
    // work detail is simply returned as output. For the cached version a
    // similar process is replicated in reduceFromHistor().
    void		 extractAttribs();

    // Find out the collapse cost and position for all hedges.
    void		 calcInitialHedgeCollapseData();

    // Find out the collapse cost and position for all given hedges.
    void		 calcHedgeCollapseData(const HedgeArray &hedges);

    // Calculate the collapse cost of quad rings.
    void		 calcRingCollapseData();

    // Classify the quads into rings (initialize myRingSet).
    void		 findInitialQuadRings(T tri_tolerance);

    // Calculate the added cost of from attribute values to if the hedge
    // h is to be collapsed to point x.
    T			 calcAttribCollapseCost(GEO_Hedge h, TVector3 x);

    // Given a quadric, find the point x and minimizes it. x0 will represent
    // The default answer (typically midpoint of the edge being collapsed)
    // eigen_ratio restricts the ratio between the smallest smallest and
    // largest eigenvalues of the quadric when generating the pseudo-inverse
    // using x0.

    static bool		 minimizeQuadric(const Quadric &Q, TVector3 &x,
    				TVector3 x0, T eigen_ratio);


    // Modify quadric Q for a point pt, to reflect "active" attributes. This
    // is not used in our current implementation of PolyReduce sop, as all
    // controlling attributes contribute passively (they add to the cost of
    // collapse but do not participate in determining the collapse location).
    void		 adjustForActiveWedges(Quadric &Q,
				GA_Offset pt);

    // Find collapse cost and position for a single hedge.
    void		 calcHedgeCollapseData(GEO_Hedge h);

    // Test whether collapsing a single edge causes an "inversion" in the mesh.
    bool 		 hedgeCollapseCausesInversion(GEO_Hedge h);

    // Test whether collapsing an entire ring causes an "inversion" in the mesh.
    bool		 ringCollapseCausesInversion(int r);

    // Clear the knots in the ring ahead of its collapse. Check the notes
    // above the implementation.
    bool		 clearRingKnots(int r, GA_OffsetArray &collapse_pts,
				UT_Int32Array &ring_merge_pairs,
				bool probe_only = false);

    // Generate a an additional attribute to store current point positions and
    // replace current positions with those from the rest detail (the method
    // name is a bit of a misnomer since the attribute holds the actual
    // positions and not the rest position of the points).
    void		 setupRestPosAttrib();

    // Restore the position of the points from the rest attribute. This is
    // only called in the cache-free mode where the actual work detail is
    // returned. In the cached mode, this is handled inside reduceFromHistory().
    void		 restoreRestPos();

    // Create a detached point group for boundary points.
    void		 buildBoundaryPointsGroup(const HedgeArray &bd_hedges);

    // Create a detached point group for hard points.
    void		 buildHardPointsGroup(const GA_PointGroup *hard_pts,
			  	const GA_EdgeGroup *hard_edges);

    // Create a detached point group for attribute seam (discontinuity) points.
    void		 buildSeamPointsGroup(const HedgeArray &hedges);


    // Collapse an edge onto a given position. This assumes the edge is
    // contractible.
    GA_Offset		 collapse(GEO_Hedge h_collapse, TVector3 collapse_pos,
				bool collapse_on_dst, T cost = -1);

    // Look up the position of a point in the mesh.
    SYS_FORCE_INLINE
    TVector3		 getPos3(GA_Offset vtx) const
    			    { return TVector3(myGdp->getPos3(vtx)); }

    // Loop up whether a given poin is a seam point.
    SYS_FORCE_INLINE
    bool		 isSeamPoint(GA_Offset pt) const
    			    { return (mySeamPts != nullptr
				      && mySeamPts->containsOffset(pt)); }

    // Calculate an affine map in 3-space based on given values on three
    // points in space. The first three components of the calculated map
    // will coincide with the gradient of the map which lie on the plane
    // of the triangle defined by the three points.
    static TVector4 	 affineFromGradientOnTri(const TVector3 *p,
				const T *s, TVector3 n);


    // Calculate normal and area of a polygon in 3-space.
    static void 	 calcPolyNormalAndArea(const TVector3 *pos, int len,
				TVector3 &normal, T &area);

    SYS_FORCE_INLINE
    TVector3		 hedgeCollapsePos(GEO_Hedge h) const
    			    { return myHedgeCollapsePos(
					exint(myMesh->srcVertex(h))); }

    // Set the collapse position, cost, and choice of surviving point
    // offset for a given hedge. This also ensure that the information
    // is also set for hedges equivalent to h appropriately.
    SYS_FORCE_INLINE
    void		 setHedgeCollapseData(GEO_Hedge h,
				T cost, TVector3 pos = TVector3(0, 0, 0),
				bool on_dst = true, bool syms = true);

    // Return the collapse cost for a given hedge. NB. the sign is used
    // to decide the surviving point, hence the absolute value.
    SYS_FORCE_INLINE
    fpreal		 hedgeCollapseCost(GEO_Hedge h) const
    {
	return SYSabs(myHedgeCollapseCost(exint(myMesh->srcVertex(h))));
    }

    SYS_FORCE_INLINE
    bool		 collapsesOnDst(GEO_Hedge h) const
    {
	return myHedgeCollapseCost(exint(myMesh->srcVertex(h))) >= 0.0;
    }

    SYS_FORCE_INLINE
    bool		 isHardPoint(GA_Offset pt) const
    			    { return myHardPts
				     && myHardPts->containsOffset(pt); }

    SYS_FORCE_INLINE
    bool		 isBoundaryPoint(GA_Offset pt) const
    			    { return myBoundaryPts
				     && myBoundaryPts->containsOffset(pt); }

    // Decide whether we are allowed to collapse a hedge. Hard points for
    // example can render an edge collapse forbidden.
    bool		 isEdgeCollapseAllowed(GEO_Hedge h);

    // Decide whether we're allowed to collapse a ring.
    bool		 isRingCollapseAllowed(int r);

    // Check whether the given hedge (or one of its equivalents) belong to
    // a quad ring.
    SYS_FORCE_INLINE
    bool		 isRingHedge(GEO_Hedge h) const;

    // Determine whether a given polygon is a quad ring knot.
    SYS_FORCE_INLINE
    bool		 isRingKnot(GA_Offset poly) const
    {
    	return (myRingKnots != nullptr && myRingKnots->containsOffset(poly));
    }

    GU_Detail		*getDetail() { return myGdp; }

    int		 	 numRings() const;

    // Return the index of the ring a hedge belongs to (-1 if none).
    int		 	 hedgeRing(GEO_Hedge h) const;

    // Calculate the collapse cost of a ring.
    T 		 	 calcRingCollapseCost(int r);

    // Collapse all the half-edges in the given list and return the resulting
    // points in collapse_pts.
    void		 collapseHedges(const HedgeArray &hedges,
    				GA_OffsetArray &collapse_pts,
    				T cost);

    // Determine whether a hedge is incident to precisely three quads on
    // both ends.
    bool 		 hasDegree3OnBothEnds(GEO_Hedge h);

    // Reduce the geometry in gdp (which should match the geometry based on
    // which we have created our cache) down to the given target sizes using
    // the recorded history. If a rest detail was passed to the constructor,
    // an "equivalent" detail must also be passed here.
    fpreal		 reduceFromHistory(GU_Detail *gdp,
				exint target_num_polys,
				exint target_num_pts,
				T min_error, T max_error,
    				const GU_Detail *rest_gdp = nullptr,
    				const char *error_attrib_name = nullptr);

    // Look ahead and fetch the list of hedges that are to be collapsed next
    // in a batch. Normally, this would be a single hedge but when reducing
    // quads whole rings of hedges may come up in single batches, in which
    // case the list of pairs of rings that should be merged after the collapse
    // is also returned.
    T		 	 fetchNextCollapseBatch(HedgeArray &collapse_batch,
				GA_OffsetArray &collapse_pts,
				UT_Int32Array &ring_merge_pairs,
				bool probe_only = false);

    // Refresh collapse data for all of the hedges in the given list of
    // affected_hedges and apply the merging of the rings given in
    // ring_merge_pairs.
    void		 refreshCollapseData(HedgeArray &affected_hedges,
				const UT_Int32Array &ring_merge_pairs);

    // Populate the reduction priority queue (done only once).
    void		 populateQueue();

    // Retire the mesh and all associate objects. This is done when we've
    // reduced the mesh until there's nothing more to do, or if we're getting
    // destroyed.
    void		 retireMesh();

    // Reduce the mesh to reach a target, recording the reduction history
    // unless we're going to return the reduced mesh as output.
    void		 reduceToReachTarget(exint target_num_polys,
				exint target_num_pts,
    				T min_error, T max_error);

    void		 dumpRing(int r, bool dump_costs = false);


    class Queue;
    struct HistoryNode;

    using MeshUptr = UT_UniquePtr<Mesh>;
    using RingSetUptr = UT_UniquePtr<RingSet>;
    using AttribRefMapUptr = UT_UniquePtr<GA_AttributeRefMap>;
    using AttribHandlerUptr = UT_UniquePtr<AttribHandler>;
    using WedgeSetUptr = UT_UniquePtr<WedgeSet>;
    using WedgeBundleUptr = UT_UniquePtr<WedgeBundle>;
    using History = UT_Array<HistoryNode>;
    using QueueUptr = UT_UniquePtr<Queue>;

    const Parms		 myParms;
    GEO_PolyInterface	*myPip				= nullptr;
    GU_Detail		*myGdp				= nullptr;
    const GU_Detail	*myRestGdp			= nullptr;
    const GU_Detail	*myViewGdp			= nullptr;

    MeshUptr		 myMesh;

    // Attribute handlers for point and vertex attributes
    AttribHandlerUptr	 myPtAttribs;
    AttribHandlerUptr 	 myVtxAttribs;

    // Position wedge set carrying quadric error metric
    WedgeSetUptr	 myPosWedgeSet;

    // Wedge bundles for point and vertex attribute wedges
    WedgeBundleUptr	 myPtWedgeBundle;
    WedgeBundleUptr	 myVtxWedgeBundle;

    // Copies of the initialized point and attribute wedge sets for retracing
    WedgeBundleUptr	 myOrigPtWedgeBundle;
    WedgeBundleUptr	 myOrigVtxWedgeBundle;

    // Hedge collapse data
    TVector3Array	 myHedgeCollapsePos;
    TArray	 	 myHedgeCollapseCost;
    T			 myLastCollapseCost		= -1;
    T			 myAreaSum			= 0;

    // Initial number of points and polygons in the input mesh (inside group)
    exint		 myInitialNumPolys		= -1;
    exint		 myInitialNumPoints		= -1;

    // Rest position attribute and the its index in the point attribute handler
    GA_Attribute 	*myRestPos;
    int			 myRestPosAttribHandlerIdx	= -1;

    // Detached group of original primitive group
    GA_PrimitiveGroup	*myOrigPrims			= nullptr;

    // Detached group for isolated points in input geometry
    GA_PointGroup	*myIsolatedPoints		= nullptr;

    // Seam, hard, and boundary points (detached groups)
    GA_PointGroup	*mySeamPts			= nullptr;
    GA_PointGroup	*myHardPts			= nullptr;
    GA_PointGroup	*myBoundaryPts			= nullptr;

    UT_StringArray	 myWarnings;
    int			 myNumConstructorWarnings	= 0;

    // Quad rings

    int		 	 myBaseRing			= -1;
    RingSetUptr		 myRingSet;

    // Hedge to ring mapping
    UT_Int32Array	 myHedgeRing;

    // Detached group of ring knots
    GA_PrimitiveGroup	*myRingKnots			= nullptr;

    // Ring collapse data
    TArray 		 myRingCollapseCost;

    // Arrays for temporary use in non-reentrant methods.

    HedgeArray	 	 myTempPointHedgeMap;
    UT_IntArray		 myTempPointHedgeMapSerial;

    int		 	 myPointEdgeMapSerial		= 0;

    HedgeArray 		 myTempHedgeArray;
    GA_OffsetArray	 myTempOffsetArray;
    TArray 		 myTempTArray;

    History		 myHistory;
    QueueUptr		 myQueue;

    bool		 myCacheHistory				= true;
    bool		 myBatchStarted				= false;

    enum HistoryOp
    {
	BATCH_START = 0,
	BATCH_END,
	VERTEX_MERGE,
	COLLAPSE,
	DISSOLVE
    };

    // We use 32 bit integers to store offsets used in history nodes (by far
    // most of which are of the COLLAPSE type) to reduce cache memory usage.

    struct HistoryNode
    {
	SYS_FORCE_INLINE HistoryNode() = default;
	SYS_FORCE_INLINE HistoryNode(int op) : op(op) { }

	SYS_FORCE_INLINE HistoryNode(GA_Offset f, GA_Offset t) :
			    op(VERTEX_MERGE), from(int(f)),
			    to(int(t)), poly_count(-1) { }

	SYS_FORCE_INLINE HistoryNode(GA_Offset f, GA_Offset t, TVector3 p,
				T c, T b, exint n) :
			    op(COLLAPSE), from(int(f)), to(int(t)),
			    pos(UT_Vector3F(p)), cost(fpreal32(c)),
			    bias(fpreal32(b)), poly_count(int(n)) { }

	SYS_FORCE_INLINE HistoryNode(GA_Offset f, GA_Offset t, GA_Offset tn,
				exint n) :
			    op(DISSOLVE), from(int(f)), to(int(t)),
			    to_next(int(tn)), poly_count(int(n)) { }

	int		 op;
	int 	 	 from, to;
	union
	{
	    UT_Vector3F  pos;
	    int	 	 to_next;
	};
	fpreal32	 bias;
	fpreal32 	 cost;
	int		 poly_count;
    };

    SYS_FORCE_INLINE
    void		 recordBatchStart()
    {
	if (!myCacheHistory)
	    return;

	UT_ASSERT(!myBatchStarted);
	myHistory.append(HistoryNode(BATCH_START));
	myBatchStarted = true;
    }

    SYS_FORCE_INLINE
    void		 recordBatchEnd()
    {
	if (!myCacheHistory)
	    return;

	UT_ASSERT(myBatchStarted);
	if (myHistory.last().op == BATCH_START)
	    myHistory.removeLast();
	else
	    myHistory.append(HistoryNode(BATCH_END));
	myBatchStarted = false;
    }

    SYS_FORCE_INLINE
    bool		 isInBatch() const
    			    { return myCacheHistory && myBatchStarted; }

    SYS_FORCE_INLINE
    void 		 recordCollapse(GA_Offset from, GA_Offset to,
			    	TVector3 pos, T cost = -1, T bias = -1)
    {
	if (myCacheHistory)
	    myHistory.append(HistoryNode(from, to, pos, cost, bias,
					 myMesh->numPolys()));
    }

    SYS_FORCE_INLINE
    void		 recordDissolve(GA_Offset from, GA_Offset to,
				GA_Offset to_next)
    {
	if (myCacheHistory)
	    myHistory.append(HistoryNode(from, to, to_next,
					 myMesh->numPolys()));
    }

    SYS_FORCE_INLINE
    void		 recordVertexMerge(GA_Offset v0, GA_Offset v1)
    {
	if (myCacheHistory)
	    myHistory.append(HistoryNode(v0, v1));
    }

    void		 pushEdge(GEO_Hedge h, T cost);
    void		 pushRing(int r, T cost);

    SYS_FORCE_INLINE
    void 		 queueHedgeAppend(GEO_Hedge h, T cost)
    			    { myQueue->append(cost,
					      int(myMesh->srcVertex(h))); }

    SYS_FORCE_INLINE
    void 		 queueRingAppend(int r, T cost)
    			    { myQueue->append(cost, r + myBaseRing); }


    SYS_FORCE_INLINE
    int 		 queueHedgeInsert(GEO_Hedge h, T cost)
    {
	return myQueue->insert(cost, int(myMesh->srcVertex(h)));
    }

    SYS_FORCE_INLINE
    int 		 queueRingInsert(int r, T cost)
    			    { return myQueue->insert(cost, r + myBaseRing); }

    SYS_FORCE_INLINE
    int 		 queueHedgeUpdate(GEO_Hedge h, T cost)
    			    { return myQueue->update(queueHedgeIndex(h),
						     cost); }

    SYS_FORCE_INLINE
    int 		 queueRingUpdate(int r, T cost)
    			    { return myQueue->update(queueRingIndex(r), cost); }

    SYS_FORCE_INLINE
    int 		 queueHedgeIndex(GEO_Hedge h) const
    			    { return myQueue->index(int(myMesh->srcVertex(h))); }

    SYS_FORCE_INLINE
    int 		 queueRingIndex(int r) const
    			    { return myQueue->index(r + myBaseRing); }

    SYS_FORCE_INLINE
    void 		 queueHedgeRemove(GEO_Hedge h)
    {
	int idx = queueHedgeIndex(h);
	if (idx >= 0)
	    myQueue->remove(idx);
    }

    // We implement our own dedicated priority queue (binary heap)
    // for better performance and for handling some corner cases.

    class Queue
    {
    public:
	using Element = std::pair<T, int>;
	using ElementArray = UT_Array<Element>;

	Queue(int size)
	{
	    myElements.setCapacity(size);
	    myElementIndex.setSizeNoInit(size);
	    myElementIndex.constant(-1);
	}

	SYS_FORCE_INLINE
	const Element	&operator()(exint i) { return myElements(i); }

	void 		 heapify()
	{
	    for (int i = parent(int(myElements.size()) - 1); i >= 1; i--)
		bubbleDown(i);
	}

	SYS_FORCE_INLINE
	int 		 update(int idx, T new_cost)
	{
	    myElements(idx).first = new_cost;
	    int new_idx = bubbleUp(idx);
	    if (new_idx != idx)
		return new_idx;

	    return bubbleDown(idx);
	}

	SYS_FORCE_INLINE
	int 		 insert(T cost, int thing)
	{
	    auto idx = int(myElements.append(Element(cost, thing)));
	    myElementIndex(thing) = idx;
	    return bubbleUp(idx);
	}


	SYS_FORCE_INLINE
	void 		 append(T cost, int thing)
	{
	    myElementIndex(thing) =
		    int(myElements.append(Element(cost, thing)));
	}


	SYS_FORCE_INLINE
	void 		 remove(int idx)
	{
	    myElementIndex(myElements(idx).second) = -1;

	    if (myElements.size() > 1)
	    {
		// Are we deleting the last element in the queue?
		if (idx == myElements.size() - 1)
		{
		    myElements.removeLast();
		    return;
		}

		// Replace the deleted one with the last queue element.
		myElements(idx) = myElements.last();
		myElements.removeLast();
		myElementIndex(myElements(idx).second) = idx;
		bubbleDown(idx);
	    }
	    else
		myElements.clear();
	}

	SYS_FORCE_INLINE
	int 		 index(int thing) const
			    { return myElementIndex(thing); }

	SYS_FORCE_INLINE
	int 		 size() const
			    { return int(myElements.size()); }
    private:

	SYS_FORCE_INLINE
	bool 		 compare(const Element &a, const Element &b) const
			    { return a.first > b.first; }

	SYS_FORCE_INLINE
	int 		 parent(int idx) { return (idx - 1) >> 1; }

	SYS_FORCE_INLINE
	int 		 leftChild(int idx) { return (idx << 1) + 1; }

	SYS_FORCE_INLINE
	void 		 swap(int a, int b)
	{
	    Element temp = myElements(a);
	    myElements(a) = myElements(b);
	    myElements(b) = temp;

	    myElementIndex(myElements(a).second) = a;
	    myElementIndex(myElements(b).second) = b;
	}

	SYS_FORCE_INLINE
	int 		 bubbleUp(int idx)
	{
	    while (idx > 0)        // while not root
	    {
		int parent_idx = parent(idx);
		if (!compare(myElements(parent_idx), myElements(idx)))
		    return idx;

		swap(idx, parent_idx);
		idx = parent_idx;
	    }
	    return 0;
	}

	SYS_FORCE_INLINE
	int 		 bubbleDown(int idx)
	{
	    while (true)
	    {
		int l = leftChild(idx);
		int r = l + 1;
		int new_idx = idx;

		if (r < myElements.size())        // both children exist
		{
		    if (compare(myElements(idx), myElements(l)))
		    {
			if (compare(myElements(l), myElements(r)))
			    new_idx = r;
			else
			    new_idx = l;
		    }
		    else if (compare(myElements(idx), myElements(r)))
			new_idx = r;
		}
		else if (l < myElements.size()) // just left child
		{
		    if (compare(myElements(idx), myElements(l)))
			new_idx = l;
		}

		if (idx == new_idx)
		    return idx;

		swap(idx, new_idx);
		idx = new_idx;
	    }
	}
	ElementArray 	 myElements;
	UT_Int32Array 	 myElementIndex;
    };

};


typedef DecimatorT<fpreal32>	DecimatorF;
typedef DecimatorT<fpreal64>	DecimatorD;

} // namespace PolyReduce2

#endif


