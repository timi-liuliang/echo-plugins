/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 */

#ifndef __GU_Flatten2_h__
#define __GU_Flatten2_h__

#include "GU_API.h"
#include "GU_Detail.h"

#include <GEO/GEO_HedgeInterface.h>

#include <GA/GA_Edge.h>
#include <GA/GA_EdgeGroup.h>

#include <UT/UT_Map.h>
#include <UT/UT_ParallelUtil.h>
#include <UT/UT_UniquePtr.h>

// #define REPORT_ABF_ANGLES

namespace GU_Flatten2
{

using ROHandleV2R = GA_ROHandleT<UT_Vector2R>;
using RWHandleV2R = GA_RWHandleT<UT_Vector2R>;
using ROHandleV3R = GA_ROHandleT<UT_Vector3R>;
using RWHandleV3R = GA_RWHandleT<UT_Vector3R>;

enum Status
{
    SUCCESS = 0,
    INTERRUPTED,
    BAD_PARMS,
    NO_BOUNDARY,
    NUMERICAL_ERROR
};

enum Method
{
    SCP,             	// Spectral Conformal Parameterization (SCP)
    LSCM,               // Least Squares Conformal Mapping (LSCM)
    LINEAR_ABF,         // Angle-Based Flattening (Linearized version)
    PROJECT,		// Projection onto best-fitting plane
};

template <typename T>
struct PtrRange
{
		     	 PtrRange(const T *s, const T *e) :
				 myStart(s), myEnd(e) { }

    SYS_FORCE_INLINE
    const T		*begin() const { return myStart; }

    SYS_FORCE_INLINE
    const T		*end() const { return myEnd; }

    SYS_FORCE_INLINE
    exint 		 size() const { return exint(myEnd - myStart); }

    SYS_FORCE_INLINE
    const T	 	&first() const
			    { UT_ASSERT_P(size() > 0); return *myStart; }
private:
    const T		 *myStart, *myEnd;
};


// A class encapsulation an abstract triangulation of an island. The island
// must be a manifold with or without boundary as each half-edge is allowed
// too have at most one sym(). Triangles will be numbered from 0 to n - 1 if
// there are n of them. The half-edges (which will be different from polygon
// half-edges) will simply be numbered from 0 .. 3n - 1 where half-edges
// 3k, 3k + 1, and 3k + 2 will be the three half-edges of triangle k. Methods
// provide access to real source and destination vertex or point offsets of
// each triangulation half-edge. The triangulation doesn't come with any
// indexing of island points. This is done independently through island point
// maps.

class GU_API Triangulation
{
public:
    using GetPos3Func = std::function<UT_Vector3R(GA_Offset)>;

    Triangulation(const GU_Detail *gdp, const GA_OffsetArray &polys,
		  const GA_EdgeGroup *seams = nullptr,
		  const GetPos3Func& get_pos3 = nullptr,
		  UT_Vector3R *centroid = nullptr);


    SYS_FORCE_INLINE
    int		 	 numTriangles() const { return int(myTris.size()); }

    SYS_FORCE_INLINE
    int		 	 numTriHedges() const { return 3 * numTriangles(); }

    SYS_FORCE_INLINE
    int	 	 	 triHedge(int i, int j = 0) const
			    { return 3 * i + j % 3; }
    SYS_FORCE_INLINE
    int	 	 	 hedgeTri(int i) const { return i / 3; }

    SYS_FORCE_INLINE
    GA_Offset	 	 triVertex(int i, int j = 0) const
			    { return GA_Offset(myTris(i).myVtxs[j % 3]); }
    SYS_FORCE_INLINE
    GA_Offset	 	 triPoint(int i, int j = 0) const
			    { return myGdp->vertexPoint(triVertex(i, j)); }
    SYS_FORCE_INLINE
    int	 	 	 lnext(int i) const
			    { return (i % 3 == 2) ? i - 2 : i + 1; }

    SYS_FORCE_INLINE
    int	 	 	 lprev(int i) const
			    { return (i % 3 == 0) ? i + 2 : i - 1; }

    SYS_FORCE_INLINE
    int	 	 	 onext(int i) const { return sym(lprev(i)); }

    SYS_FORCE_INLINE
    GA_Offset	 	 srcVertex(int i) const
			    { return GA_Offset(myTris(i / 3).myVtxs[i % 3]); }
    SYS_FORCE_INLINE
    GA_Offset	 	 dstVertex(int i) const
			    { return GA_Offset(myTris(i / 3).myVtxs[(i + 1) % 3]); }
    SYS_FORCE_INLINE
    GA_Offset	 	 apxVertex(int i) const
			    { return GA_Offset(myTris(i / 3).myVtxs[(i + 2) % 3]); }
    SYS_FORCE_INLINE
    int	 	 	 sym(int i) const
			    { return myTris(i / 3).mySyms[i % 3]; }
    SYS_FORCE_INLINE
    GA_Offset	 	 srcPoint(int i) const
			    { return myGdp->vertexPoint(srcVertex(i)); }
    SYS_FORCE_INLINE
    GA_Offset	 	 dstPoint(int i) const
			    { return myGdp->vertexPoint(dstVertex(i)); }
    SYS_FORCE_INLINE
    GA_Offset	 	 apxPoint(int i) const
			    { return myGdp->vertexPoint(apxVertex(i)); }

    SYS_FORCE_INLINE
    GA_Offset		 hedgePoly(int i) const
    			    { return myGdp->vertexPrimitive(srcVertex(i)); }

    // Is a half-edge a diagonal of an input polygon (as opposed to
    // corresponding a physical edge.
    SYS_FORCE_INLINE
    bool		 isDiagonal(int i) const
    			    { return sym(i) >= 0
				     && hedgePoly(i) == hedgePoly(sym(i)); }
private:
    struct Triangle
    {
	SYS_FORCE_INLINE
	Triangle(GA_Offset v0, GA_Offset v1, GA_Offset v2) :
		myVtxs {int(v0), int(v1), int(v2)},
		mySyms {-1, -1, -1}
	{ }

	int		 myVtxs[3];
	int		 mySyms[3];
    };

    using TriangleArray = UT_Array<Triangle>;
    TriangleArray	 myTris;

    const GU_Detail	*myGdp;
};

class GU_API Island
{
public:
    explicit Island(const GU_Detail *gdp, int idx = -1) :
	    myGdp(gdp), myIndex(idx)
    { }

    const GU_Detail	*getDetail() const { return myGdp; }

    SYS_FORCE_INLINE
    int			 getIndex() const { return myIndex; }

    const
    GA_OffsetArray	&polys() const { return myPolys; }

    SYS_FORCE_INLINE
    void	         appendPoly(GA_Offset poly)
			    { myPolys.append(poly); }

    const
    Triangulation	&getTriangulation() const
    {
	UT_ASSERT(myTri != nullptr);
	return *myTri;
    }

    void		 triangualte(const GA_EdgeGroup *seams = nullptr)
    {
	myTri.reset(new Triangulation(myGdp, myPolys, seams));
    }

    void		 calcHedgeDirichletData(UT_FprealArray &apx_cotan,
	    			UT_FprealArray &tri_area) const;

    fpreal		 calcArea(ROHandleV3R pos,
			    	UT_Vector3R *centroid = nullptr) const;

    template<typename Func>
    void 		 forEachVertex(Func func) const
    {
	for (GA_Offset poly : myPolys)
	{
	    auto vtxs = myGdp->getPrimitiveVertexList(poly);
	    for (int i = 0, ie = int(vtxs.size()); i < ie; i++)
		func(vtxs(i));
	}
    }

    SYS_FORCE_INLINE
    bool 		 hasPointIndex() const
    			    { return myVertexPointAttr.isValid(); }

    int		 	 numPoints() const
    			    { return myNumPoints; }

    GA_ROHandleI	 vertexPointAttr() const { return myVertexPointAttr; }

    SYS_FORCE_INLINE
    int			 vertexPoint(GA_Offset vtx) const
    			    { return myVertexPointAttr.get(vtx); }

    int		 	 indexPoints(GA_RWHandleI &vtx_pt_attr)
    {
	myVertexPointAttr = vtx_pt_attr;
	myNumPoints = buildPointIndex([&](GA_Offset vtx, int idx)
				      { myVertexPointAttr.set(vtx, idx); });
	return myNumPoints;
    }

    int		 	 indexPoints(UT_IntArray &vtx_island_pt) const
    {
	return buildPointIndex([&](GA_Offset vtx, int idx)
			       { vtx_island_pt(int(vtx)) = idx; });
    }

    class GU_API PointMap
    {
    public:
	explicit 	 PointMap(const Island &island);

	SYS_FORCE_INLINE
	int		 vertexPoint(GA_Offset vtx) const
    			    { return myVtxPointMap.size() > 0
				     ? myVtxPointMap(vtx)
				     : myVertexPointAttr.get(vtx); }

    	SYS_FORCE_INLINE
    	int		 numPoints() const { return myNumPoints; }

	SYS_FORCE_INLINE
    	int		 pointTriHedge(int pt) const
	{
	    if (myPointTriHedgeMap.size() == 0)
		buildPointTriHedgeMap();

	    return myPointTriHedgeMap(pt);
	}

	SYS_FORCE_INLINE
    	int		 vertexTriHedge(GA_Offset vtx) const
	{
	    int i0 = pointTriHedge(vertexPoint(vtx));
	    int i = i0;
	    do
	    {
		if (myTri.srcVertex(i) == vtx && !myTri.isDiagonal(i))
		    return i;
	    } while (i = myTri.onext(i), i >= 0 && i != i0);
	    return -1;
	}

	SYS_FORCE_INLINE
	int  		 numBoundaryPoints() const
	{
	    if (myBoundaryFlag.size() == 0)
		markBoundaryPoints();

	    return myNumBoundaryPoints;
	}

	SYS_FORCE_INLINE
	bool  		 isBoundaryPoint(int i) const
	{
	    if (myBoundaryFlag.size() == 0)
		markBoundaryPoints();
	    return myBoundaryFlag.getBitFast(i);
	}

	SYS_FORCE_INLINE
	int		 numInteriorPoints() const
			    { return myNumPoints - numBoundaryPoints(); }


    	SYS_FORCE_INLINE
    	GA_Offset	 pointVertex(int pt) const
    			    { return myTri.srcVertex(pointTriHedge(pt)); }
    private:

	void 		 buildPointTriHedgeMap() const;
	void		 markBoundaryPoints() const;

	const
	Triangulation 	&myTri;

	GA_ROHandleI 	 myVertexPointAttr;
	UT_IntArray 	 myVtxPointMap;

	int 		 myNumPoints				= 0;
	mutable int 	 myNumBoundaryPoints 			= 0;

	mutable
	UT_BitArray 	 myBoundaryFlag;

	mutable
	UT_IntArray 	 myPointTriHedgeMap;
    };

    class GU_API BoundaryMap
    {
    public:
	explicit 	 BoundaryMap(const Island &island);

	int		 numComponents() const
			    { return int(myCompStarts.size() - 1); }

	using HedgeRange = PtrRange<int>;

	int 		 componentSize(int i) const
			    { return myCompStarts(i + 1) - myCompStarts(i); }

	HedgeRange	 componentHedges(int i) const
	{
	    return { myHedges.data() + myCompStarts(i),
		     myHedges.data() + myCompStarts(i + 1) };
	}

	int		 outerComponent() const
			    { return myOuterComp; }

    private:
	UT_IntArray	 myHedges;
	UT_IntArray	 myCompStarts;
	int		 myOuterComp				= -1;
    };


    class GU_API QuadLayout
    {
    public:
	explicit 	 QuadLayout(const Island &island);

	void		 findNodes();

	struct QuadDegree
	{
	    QuadDegree(int nq, int s) : num_quads(nq), separations(s) { }
	    int	num_quads;
	    int 	separations;
	};

	QuadDegree	 quadDegree(int pt) const;

	SYS_FORCE_INLINE
	bool  		 isNode(int pt) const
			    { return myNodeFlag.getBitFast(pt); }

	SYS_FORCE_INLINE
	bool  		 isOnArc(int pt) const
			    { return myArcFlag.getBitFast(pt); }


	SYS_FORCE_INLINE
	int  		 numLoops() const
			    { return int(myLoopFirstTriHedge.size()); }

	SYS_FORCE_INLINE
	int  		 numArcs() const
			    { return myNumArcs; }

	SYS_FORCE_INLINE
	int  		 numInteriorLoops() const
			    { return numLoops() - numArcs(); }

	SYS_FORCE_INLINE
	bool		 isQuadTriHedge(int i) const
	{
	    auto &&i_poly = myTri.hedgePoly(i);
	    if (!isQuad(i_poly))
		return false;
	    int i_sym = myTri.sym(i);
	    return (i_sym < 0 || myTri.hedgePoly(i_sym) != i_poly);
	}

	void		 findLoops();
	void		 findRectifiablePatches(UT_IntArray &patches) const;

	SYS_FORCE_INLINE
	int 		 loopLength(int j) const
				{ return myLoopLength(j); }

	SYS_FORCE_INLINE
	int 		 loopFirstTriHedge(int j) const
				{ return myLoopFirstTriHedge(j); }

	SYS_FORCE_INLINE
	int 		 loopLastTriHedge(int j) const
				{ return myLoopLastTriHedge(j); }

	SYS_FORCE_INLINE
	int 		 loopSym(int j) const
	{
	    int i = loopFirstTriHedge(j);
	    int i_sym = myTri.sym(i);
	    if (i_sym < 0)
		return -1;

	    return triHedgeLoop(i_sym);
	}

	SYS_FORCE_INLINE
	int			 loopLnext(int j) const
	{
	    int i = loopLastTriHedge(j);
	    int i_next = myTri.lnext(i);
	    while (myTri.isDiagonal(i_next))
		i_next = myTri.lnext(myTri.sym(i_next));
	    return triHedgeLoop(i_next);
	}

	SYS_FORCE_INLINE
	int 		 triHedgeLoopSucc(int i) const
				{ return myTriHedgeLoopSucc(i); }

	SYS_FORCE_INLINE
	int 		 triHedgeLoop(int i) const
				{ return myTriHedgeLoop(i); }

	SYS_FORCE_INLINE
	int		 numPatches() const
			    { return int(myPatchFirstArc.size()); }

	SYS_FORCE_INLINE
	int		 arcPatch(int j) const { return myArcPatch(j); }

	SYS_FORCE_INLINE
	int		 patchArc(int p) const { return myPatchFirstArc(p); }

    private:

	SYS_FORCE_INLINE
	int  		 vertexPoint(GA_Offset v) const
			    { return myPointMap.vertexPoint(v); }

	SYS_FORCE_INLINE
	int  		 srcPoint(int i) const
			    { return vertexPoint(myTri.srcVertex(i)); }

	SYS_FORCE_INLINE
	int  		 dstPoint(int i) const
			    { return vertexPoint(myTri.dstVertex(i)); }

	// Given a boundary tri-hedge, find its successor along the boundary.
	SYS_FORCE_INLINE
	int 		 boundarySucc(int i) const
	{
	    i = myTri.lnext(i);
	    while (myTri.sym(i) >= 0)
		i = myTri.lnext(myTri.sym(i));
	    return i;
	};

	SYS_FORCE_INLINE
	bool 		 isQuad(GA_Offset poly) const
			    { return myGdp->getPrimitiveVertexCount(poly) == 4; }


	SYS_FORCE_INLINE
	bool		 isDstOnBoundary(int i) const
			    { return myPointMap.isBoundaryPoint(dstPoint(i)); }

	// Given a tri hedge i, return the tri hedge j that succeeds it
	// in a path, or -1 if none.
	int 		 quadSucc(int i) const;

	const
	Triangulation 	&myTri;

	const
	PointMap	&myPointMap;

	const
	GU_Detail	*myGdp;

	int 		 myNumKnots 				= 0;
	int 		 myNumArcs 				= 0;

	UT_BitArray 	 myNodeFlag;				// points
	UT_BitArray 	 myArcFlag;				// points

	UT_IntArray 	 myLoopFirstTriHedge;
	UT_IntArray 	 myLoopLastTriHedge;

	UT_IntArray	 myLoopLength;
	UT_IntArray 	 myTriHedgeLoopSucc;
	UT_IntArray 	 myTriHedgeLoop;


	UT_IntArray 	 myArcPatch;
	UT_IntArray	 myPatchFirstArc;
    };

    const PointMap	&getPointMap() const
    {
	if (!myPointMap)
	    myPointMap.reset(new PointMap(*this));
	return *myPointMap;
    }

    const QuadLayout	&getQuadLayout() const
    {
	if (!myQuadLayout)
	    myQuadLayout.reset(new QuadLayout(*this));
	return *myQuadLayout;
    }

    const BoundaryMap	&getBoundaryMap() const
    {
	if (!myBoundaryMap)
	    myBoundaryMap.reset(new BoundaryMap(*this));
	return *myBoundaryMap;
    }

private:

    using SetIdxFunctor = std::function<void(GA_Offset, int)>;
    int		 	 buildPointIndex(SetIdxFunctor set_idx) const;

    using TriUptr = UT_UniquePtr<Triangulation>;
    using PointMapUptr = UT_UniquePtr<PointMap>;
    using QuadLayoutUptr = UT_UniquePtr<QuadLayout>;
    using BoundaryMapUptr = UT_UniquePtr<BoundaryMap>;

    int		 	 myIndex				= -1;
    int			 myNumPoints				= -1;

    GA_OffsetArray	 myPolys;
    TriUptr	 	 myTri;

    const
    GU_Detail 		*myGdp					= nullptr;

    mutable
    PointMapUptr	 myPointMap;

    mutable
    QuadLayoutUptr	 myQuadLayout;

    mutable
    BoundaryMapUptr	 myBoundaryMap;

    GA_RWHandleI	 myVertexPointAttr;
};

/// An IslandBundle is used to store information about a collection (bundle)
/// of *primary* islands determined by partitioning the polygons in a primtiive
/// group 'prims' of a detail 'gdp' using the given group 'seams' of edges.
/// The bundle also supports a "remapping" of the islands into *secondary*
/// islands (or just "islands" after the effect) by recutting them using a
/// second set of seams. Originally this was meant to be a simple "refining"
/// of islands in which each primary island provided access to the secondary
/// ones into which it was divided. Later on support was added to allow the
/// repartitioning potentially remap the entire scene by erasing parts of the
/// original seams that defined the primary islands thereby merging fragments
/// from distinct primary islands into secondary ones. In this setting, each
/// secondary island will be registered as a descendent of *one* of
/// possibly several primary islands it overlaps. The other overlapped primary
/// islands will be aware of having lost part (or all) of their territory
/// to secondary islands held by other primary ones but without access to them.
///
/// The bundle provides methods to iterate over all primary, or current
/// islands. In addition, the bundle provides a mechanism for quickly marking
/// islands dirty and resetting all dirty flags. In particular, the building
/// of secondary islands is optimized to efficiently determine the affected
/// islands and set their dirty flags.

class GU_API IslandBundle
{
public:
    IslandBundle(const GU_Detail *gdp,
		 const GA_PrimitiveGroup *prims,
		 const GA_EdgeGroup *seams,
		 const GEO_DetachedHedgeInterface *hip = nullptr);

		    	~IslandBundle();

    // Repartition the collection of islands along the islet_seams and assign
    // the created islets to (one of) parent islands.
    void		 repartitionIslands(const GA_EdgeGroup *islet_seams);

    // Clear all island dirty flags.
    void		 clearDirtyFlags() { mySerial++; }

    // The total range of possible island indices. Note that some can be vacant.
    int		 	 numIslandIndices() const
			    { return int(myIslandInfos.size()); }

    // Number of primary "islands", those cut by the original set of seams.
    int		 	 numPrimaryIslands() const
			    { return myNumPrimaryIslands; }

    const
    Island		&island(int i) const
			    { return myIslandInfos(i).island(); }

    Island		&island(int i) { return myIslandInfos(i).island(); }

    SYS_FORCE_INLINE
    int		 	 polyPrimaryIsland(GA_Offset poly) const
    			    { return myPolyPrimaryIsland.get(poly); }

    SYS_FORCE_INLINE
    int		 	 polyIsland(GA_Offset poly) const
    {
	int j = polyPrimaryIsland(poly);
	if (j < 0)
	    return -1;

	if (getNext(j) >= 0)
	    return myPolySecondaryIsland.get(poly);

	return j;
    }

    bool		 isPrimary(const Island &island) const
    			    { return island.getIndex() < myNumPrimaryIslands; }

    SYS_FORCE_INLINE
    int			 hedgeIsland(GEO_Hedge h) const
    			    { return vertexPrimaryIsland(myHip->srcVertex(h)); }

    SYS_FORCE_INLINE
    int		 	 vertexPrimaryIsland(GA_Offset vtx) const
    			    { return polyPrimaryIsland(
					myGdp->vertexPrimitive(vtx)); }

    SYS_FORCE_INLINE
    int		 	 vertexIsland(GA_Offset vtx) const
    			    { return polyIsland(myGdp->vertexPrimitive(vtx)); }

    bool		 hasPointIndex() const
    			    { return myIslandVtxPt.isValid(); }

    SYS_FORCE_INLINE
    int			 vtxIslandPt(GA_Offset vtx, int island_idx) const
    			    { return island_idx < myNumPrimaryIslands
				     ? myIslandVtxPt.get(vtx)
				     : myIsletVtxPt.get(vtx); }

    void		 getIslandIndices(UT_IntArray &idxs) const;

    template<typename Func>
    void 		 forEachIslandIndex(Func func, bool parallel = false)
    {
	UT_IntArray idxs(myIslandInfos.size());
	getIslandIndices(idxs);
	if (parallel)
	    UTparallelForEachNumber(idxs.size(),
		[&](const UT_BlockedRange<exint> &r)
		{
		    for (exint j = r.begin(), je = r.end(); j != je; ++j)
			func(idxs(j));
		});
	else
	    for (int i : idxs)
		func(i);
    }

    template<typename Func>
    void 		 forEachIsland(Func func, bool parallel = false)
    {
	forEachIslandIndex([&](int i) { func(island(i)); }, parallel);
    }

    template<typename Func>
    void 		 forEachIslandIndex(Func func,
				bool parallel = false) const
    {
	UT_IntArray idxs(myIslandInfos.size());
	getIslandIndices(idxs);
	if (parallel)
	    UTparallelForEachNumber(idxs.size(),
		[&](const UT_BlockedRange<exint> &r)
		{
		    for (exint j = r.begin(), je = r.end(); j != je; ++j)
			func(idxs(j));
		});

	else
	    for (int i : idxs)
		func(i);
    }

    template<typename Func>
    void 		 forEachIsland(Func func, bool parallel = false) const
    {
	forEachIslandIndex([&](int i) { func(island(i)); }, parallel);
    }

    template<typename Func>
    void 		 forEachDirtyIsland(Func func,
				bool parallel = false) const
    {
	forEachIslandIndex([&](int i)
	   {
	       if (isDirty(i))
		   func(island(i));
	   }, parallel);
    }

    template<typename Func>
    void 		 forEachDirtyIsland(Func func, bool parallel = false)
    {
	forEachIslandIndex([&](int i)
	   {
	       if (isDirty(i))
		   func(island(i));
	   }, parallel);
    }


    template<typename Func>
    void 		 forEachPrimaryIsland(Func func) const
    {
	for (int i = 0; i < myNumPrimaryIslands; i++)
	    func(island(i));
    }

    bool 		 isDirty(const Island &island) const
			    { return isDirty(island.getIndex()); }

    void 		 makeDirty(const Island &island)
			    { makeDirty(island.getIndex()); }

private:

    int		 	 getNext(int i) const
			    { return myIslandInfos(i).getNext(); }

    void		 setNext(int i, int j)
			    { return myIslandInfos(i).setNext(j); }

    bool		 isVacant(int i) const
    			    { return myIslandInfos(i).isVacant(); }

    int		 	 newIsland();
    void		 freeIsland(int i);

    // Add a new islet to island i.
    int		 	 newSecondaryIsland(int i);

    // Destroy all secondary islands registered to primary island i.
    void		 destroyDependentSecondaryIslands(int i);

    void		 breakPrimaryIslands(const UT_IntArray &islands,
				const GA_EdgeGroup *seams);

    void		 markOvertaken(int i) { setNext(i, -2); }
    bool		 isOvertaken(int i) const
    			    { return getNext(i) == -2; }

    // Is a primary island unchanged by secondary cutting?
    bool		 isUndivided(int i) const
    			    { return getNext(i) == -1; }


    void 		 makeDirty(int i)
    {
	myIslandInfos(i).serial(mySerial);
	myIslandInfos(i).dirty(true);
    }

    bool		 isDirty(int i) const
    {
	return myIslandInfos(i).dirty()
	       && myIslandInfos(i).serial() == mySerial;
    }

    struct LabeledEdge
    {
	LabeledEdge(int i, GA_Edge e) :
		island(i), edge(SYSmin(e.p0(), e.p1()),
				SYSmax(e.p0(), e.p1()))
	{ }

	int		 island;
	GA_Edge	 	 edge;
    };

    using LabeledEdgeArray = UT_Array<LabeledEdge>;
    using IslandUptr = UT_UniquePtr<Island>;
    using HedgeInterface = const GEO_DetachedHedgeInterface;
    using HedgeInterfaceUptr = UT_UniquePtr<HedgeInterface>;

    // Dirty flag of the island is only taken seriously if the island's
    // Serial matches the managers. When manager's serial is bumped, all
    // dirty flags are assumed to have been dealt with.

    struct IslandInfo
    {
	void	 	 resetIsland(Island *island_ptr = nullptr)
			    { myIsland.reset(island_ptr); }

	bool		 isVacant() const { return myIsland == nullptr; }

	const
	Island		 &island() const { return *myIsland; }
	Island		 &island() { return *myIsland; }

	int		 serial() const { return mySerial; }
	void	 	 serial(int i) { mySerial = i; }

	void	 	 dirty(bool b) { myDirty = b; }
	bool	 	 dirty() const { return myDirty; }

	bool	 	 hasIslets() const { return myNext >= 0; }

	int	 	 getNext() const { return myNext; }
	void	 	 setNext(int i) { myNext  = i; }

    private:
	bool	 	 myDirty				= false;
	int	 	 mySerial				= 0;

	IslandUptr	 myIsland;
	int 	 	 myNext					= -1;
    };


    using IslandInfoArray = UT_Array<IslandInfo>;


    LabeledEdgeArray 	 myLabeledIsletSeamEdges;

    IslandInfoArray	 myIslandInfos;

    GA_RWHandleI	 myPolyPrimaryIsland;
    GA_RWHandleI	 myPolySecondaryIsland;

    int		 	 myNumPrimaryIslands			= 0;
    int		 	 myNumIslands				= 0;

    UT_IntArray	 	 myFreeIndices;

    const GU_Detail	*myGdp;

    HedgeInterfaceUptr	 myHiup;
    HedgeInterface	*myHip;

    GA_RWHandleI	 myIslandVtxPt, myIsletVtxPt;

    GA_EdgeGroup	*mySeams				= nullptr;

    int		 	 mySerial				= 0;
};

// A ConstraintSet carries all possible constraints (applicable to various
// methods) for a single island. As it stands now, the constraints can be:
//
// 	- Pins: vertex offset plus UV coordinates
//
// 	- Axis constraints: list of vertices which should be aligned in u or v
//		direction.
//
// 	- Line constraints: list of vertices that should end up in a line.
//
// 	- Flags (constraints applied to entire island)

// The constraints are categorized into three types:
// 	Point constraints target a single island point.
//	Group constraints target a group island vertices (tri-hedges).
//	Island constraints target the entire island.

template <typename T>
class ElementConstraintList : public UT_Array<T>
{
public:
    using Base = UT_Array<T>;

    void 		 deleteIndices(const UT_IntArray &idxs)
    {
	if (idxs.size() == 0)
	    return;

	auto move_segment = [&](int s, int e, int &j)
	{
	    for (int i = s; i < e; i++, j++)
		(*this)(j) = (*this)(i);
	};

	int j = idxs(0);
	for (int i = 1, ie = int(idxs.size()); i < ie; i++)
	    move_segment(idxs(i - 1) + 1, idxs(i), j);

	move_segment(idxs.last() + 1, int(Base::size()), j);
	Base::setSize(j);
    }

    bool		 operator!=(const ElementConstraintList<T> &other) const
    			    { return !(Base::operator==(other)); }
};

template <typename T>
struct GroupConstraintList
{
public:
    			 GroupConstraintList() { myFirst.append(0); }

    void		 add(T data = T())
    			    { 	myFirst.append(myFirst.last());
				myData.append(data); }

    void		 appendVertex(int i)
    			    { myElements.append(i); myFirst.last()++; }

    int		 	 size() const
			    { return int(myFirst.size()) - 1; }

    using ElementRange = PtrRange<int>;
#if 0
    struct ElementRange
    {
			 ElementRange(const int *s, const int *e) :
			    	myStart(s), myEnd(e) { }

	SYS_FORCE_INLINE
	const int 	*begin() const { return myStart; }

	SYS_FORCE_INLINE
	const int	*end() const { return myEnd; }

	SYS_FORCE_INLINE
	int 		 size() const { return int(myEnd - myStart); }

	SYS_FORCE_INLINE
	int	 	 first() const
			    { return size() > 0 ? *myStart : -1; }
    private:
	const int 	*myStart, *myEnd;
    };
#endif
    struct GroupHandle
    {
	SYS_FORCE_INLINE
			 GroupHandle(const GroupConstraintList<T> *set,
				int grp_idx) :
			    myList(set), myIndex(grp_idx) { }

	SYS_FORCE_INLINE
	ElementRange	 elements() const
			    { return myList->elements(myIndex); }

	SYS_FORCE_INLINE
	const T		&data() const
			    { return myList->data(myIndex); }

    private:
	using List = GroupConstraintList<T>;
	const List	*myList;
	int		 myIndex;
    };

    SYS_FORCE_INLINE
    GroupHandle	 	 operator()(int i) const
    			    { return GroupHandle(this, i); }

    SYS_FORCE_INLINE
    ElementRange	 elements(int i) const
			    { return { myElements.data() + myFirst(i),
				       myElements.data() + myFirst(i + 1) }; }

    bool 		 operator==(const GroupConstraintList &other) const
    			    { return (myElements == other.myElements
				      && myFirst == other.myFirst
				      && myData == other.myData); }

    bool		 operator!=(const GroupConstraintList &other) const
    			    { return !operator==(other); }

private:

    int		 	 size(int i) const
			    { return myFirst(i + 1) - myFirst(i); }

    SYS_FORCE_INLINE
    const T		&data(int i) const { return myData(i); }


    UT_IntArray	 	 myElements;
    UT_IntArray	 	 myFirst;
    UT_Array<T>		 myData;
};

class GU_API ConstraintSet
{
public:
    enum AlignDir { U_AXIS = 0, V_AXIS };

    bool		 isEmpty() const;
    bool		 isTrivial() const;
    bool		 hasAffineDifferenceWith(const ConstraintSet &other) const;
    bool		 operator==(const ConstraintSet &other) const;
    bool		 operator!=(const ConstraintSet &other) const
    			    { return !(*this == other); }

    struct EmptyInfo
    {
			 EmptyInfo() = default;

	SYS_FORCE_INLINE
	bool		operator==(const EmptyInfo &other) const
			    { return true; }
    };

    struct PinInfo
    {
			 PinInfo() = default;
			 PinInfo(int pt, UT_Vector3R uv, int id = -1,
			 	int ref_pt = -1, UT_Vector3R refuv
			 				= { 0, 0, 0 }) :
			    myPoint(pt), myUV(uv), myId(id),
			    myRefPoint(ref_pt), myRefUV(refuv) { }

	SYS_FORCE_INLINE
	int	 	 point() const { return myPoint; }

	SYS_FORCE_INLINE
	int	 	 refPoint() const { return myRefPoint; }

	SYS_FORCE_INLINE
	fpreal	 	 u() const { return myUV.x(); }

	SYS_FORCE_INLINE
	fpreal	 	 v() const { return myUV.y(); }

	SYS_FORCE_INLINE
	int	 	 id() const { return myId; }

	SYS_FORCE_INLINE
	UT_Vector3R	 uv() const { return myUV; }

	SYS_FORCE_INLINE
	UT_Vector3R	 refuv() const { return myRefUV; }

	SYS_FORCE_INLINE
	bool		 operator==(const PinInfo &pin) const
	{
	    return myUV == pin.myUV && myRefUV == pin.myRefUV
		   && myPoint == pin.myPoint && myRefPoint == pin.myRefPoint;
	}

	int		 myPoint, myRefPoint, myId;
	UT_Vector3R	 myUV, myRefUV;
    };

    struct AngleInfo
    {
			 AngleInfo() = default;
			 AngleInfo(int i_in, int i_out, fpreal angle = M_PI) :
			    myHedgeIn(i_in), myHedgeOut(i_out),
			    myAngle(angle) { }

	SYS_FORCE_INLINE
	int	 	 hedgeIn() const { return myHedgeIn; }

	SYS_FORCE_INLINE
	int	 	 hedgeOut() const { return myHedgeOut; }

	SYS_FORCE_INLINE
	fpreal	 	 angle() const { return myAngle; }

	SYS_FORCE_INLINE
	bool		 operator==(const AngleInfo &other) const
			    { return myHedgeIn == other.myHedgeIn
				     && myHedgeOut == other.myHedgeOut
				     && myAngle == other.myAngle; }

    private:
	int		 myHedgeIn, myHedgeOut;
	fpreal		 myAngle;
    };

    struct AlignInfo
    {
			AlignInfo() = default;
    	explicit	AlignInfo(AlignDir dir) : myDir(dir) { }

	SYS_FORCE_INLINE
	AlignDir	dir() const { return myDir; }

	SYS_FORCE_INLINE
	bool		operator==(const AlignInfo &other) const
			    { return myDir == other.myDir; }

    private:
	AlignDir	myDir;
    };

    using PinSet = ElementConstraintList<PinInfo>;
    using AngleSet = ElementConstraintList<AngleInfo>;
    using AlignedGroupSet = GroupConstraintList<AlignInfo>;
    using StraightGroupSet = GroupConstraintList<EmptyInfo>;

    const PinSet	&pins() const { return myPins; }
    PinSet		&pins() { return myPins; }

    const
    AngleSet		&angles() const { return myAngles; }
    AngleSet		&angles() { return myAngles; }

    const
    AlignedGroupSet	&alignedSets() const { return myAlignGroups; }
    AlignedGroupSet	&alignedSets() { return myAlignGroups; }

    const
    StraightGroupSet	&straightSets() const { return myStraightGroups; }
    StraightGroupSet	&straightSets() { return myStraightGroups; }

private:

    PinSet		 myPins;
    AngleSet		 myAngles;
    AlignedGroupSet	 myAlignGroups;
    StraightGroupSet	 myStraightGroups;
};

class GU_API ConstraintBundle
{
public:
    using PinInfo = ConstraintSet::PinInfo;
    using AlignDir = ConstraintSet::AlignDir;

    explicit ConstraintBundle(const IslandBundle &islands) :
	    myIslands(islands)
    { }

    void		 addPin(GA_Offset vtx, UT_Vector3R uv, int id = -1,
				GA_Offset ref_vtx = GA_INVALID_OFFSET,
				UT_Vector3R refuv = { 0, 0, 0 });

    // Remove duplicate pins, ie all but the first pin associated to each
    // island point. Only non-negative pin ids are returned.
    void		 sanitizePins(UT_IntArray *removed_pin_ids = nullptr);

    bool		 addAlignedGroup(const GA_OffsetArray &vtxs,
					 AlignDir dir);

    bool		 addStraightGroup(const GA_OffsetArray &vtxs);

    ConstraintSet 	&islandConstraints(const Island &island)
    			    { return myIslandConstraints[island.getIndex()]; }

    const
    ConstraintSet 	&islandConstraints(const Island &island) const
    {
	auto it = myIslandConstraints.find(island.getIndex());
	if (it != myIslandConstraints.end())
	    return it->second;

	return theEmptyConstraintSet;
    }

private:
    using ConstraintSetMap = UT_Map<int, ConstraintSet>;

    const
    IslandBundle	&myIslands;
    ConstraintSetMap 	 myIslandConstraints;

    static
    ConstraintSet	 theEmptyConstraintSet;
};

using AngleConstraint = ConstraintSet::AngleInfo;
using AngleConstraintArray = UT_Array<AngleConstraint>;


GU_API Status 	 	 flattenIsland(Method method, const Island &island,
				const ConstraintSet &constraints,
				RWHandleV3R uvh);

GU_API void 	 	 calcAnglesAndAreas(const Island &island,
				UT_FprealArray &opposite_angle_cotan,
				UT_FprealArray &tri_area);

GU_API Status 	 	 flattenSpectral(const Island &island,
				const ConstraintSet &constraints,
				RWHandleV3R uvh);

GU_API Status 	 	 flattenSpectral(const Island &island,
				const ConstraintSet &constraints,
				const UT_FprealArray &opposite_angle_cotan,
				const UT_FprealArray &tri_area,
				RWHandleV3R uvh);

GU_API Status 	 	 flattenLeastSquares(const Island &island,
				const ConstraintSet &constraints,
				RWHandleV3R uvh);

GU_API Status 	 	 flattenLeastSquares(const Island &island,
				const ConstraintSet &constraints,
				const UT_FprealArray &opposite_angle_cotan,
				const UT_FprealArray &tri_area,
				RWHandleV3R uvh);

GU_API Status 	 	 flattenProjection(const Island &island,
				const ConstraintSet &constraints,
				RWHandleV3R uvh);


GU_API Status 	 	 flattenAngleBased(const Island &island,
				const ConstraintSet &constraints,
				RWHandleV3R uvh);

GU_API void 	 	 findIslandOuterBoundary(const Island &island,
				UT_IntArray &outer_bd_tri_hedgefs);


GU_API void	 	 repositionIsland(const Island &island,
				const ConstraintSet &constraints,
				ROHandleV3R &orig_uvh,
				RWHandleV3R &uvh,
				bool use_custom_pins = false);


GU_API void	 	 balanceIsland(const Island &island,
				RWHandleV3R uvh);


GU_API void 		 generateStraighLoopConstraints(const Island &island,
			       const ConstraintSet &constraints,
			       AngleConstraintArray &angle_constraints);

GU_API void		 generateQuadLayoutConstraints(const Island &island,
			    	bool straighten_arcs,
				bool straighten_grids,
				bool rectify_patches,
				ConstraintSet &constraints);


} // namespace GU_Flatten2;


#endif
