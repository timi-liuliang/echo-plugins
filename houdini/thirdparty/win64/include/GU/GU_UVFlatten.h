/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 */

#ifndef __GU_UVFlatten_h__
#define __GU_UVFlatten_h__

#include "GU_API.h"
#include "GU_Detail.h"
#include "GU_UVUtils.h"

#include <GEO/GEO_HedgeInterface.h>

#include <GA/GA_Edge.h>
#include <GA/GA_EdgeGroup.h>

#include <UT/UT_Map.h>
#include <UT/UT_ParallelUtil.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_TriangleMesh.h>

// #define REPORT_ABF_ANGLES

namespace GU_UVFlatten
{

using namespace GU_UVUtils;

using ROHandleV2R = GA_ROHandleT<UT_Vector2R>;
using RWHandleV2R = GA_RWHandleT<UT_Vector2R>;
using ROHandleV3R = GA_ROHandleT<UT_Vector3R>;
using RWHandleV3R = GA_RWHandleT<UT_Vector3R>;

#if 0
template <typename T>
using GetFunctor = std::function<T(int)>;

template <typename T>
using SetFunctor = std::function<void(int, T)>;
#endif

using TriMesh = UT_TriangleMesh;

enum Status
{
    SUCCESS = 0,
    INTERRUPTED,
    BAD_PARMS,
    BAD_BOUNDARY,
    NUMERICAL_ERROR
};

// Various flattening methods
enum Method
{
    SCP,             		// Spectral Conformal Parameterization (SCP)
    LSCM,               	// Least Squares Conformal Mapping (LSCM)
    LINEAR_ABF,         	// Angle-Based Flattening (Linearized version)
    PROJECT,			// Projection onto best-fitting plane
};

// Various possible ways of splitting polygons with 4 or more vertices
enum Tessellation
{
    TRIANGULATE_ALL = 0,	// Triangulate all polygons
    SPLIT_CONVEX_QUADS		// Split convex quads (new point at centroid),
    				// triangulate all other polygons.
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

// PolySurfaceTriMesh is a an abstract TriMesh built over a subset of polygons
// of a geometry detail. It constructs and maintains the means needed for
// going from the physical space to the abstract one and vice versa.

class GU_API PolySurfaceTriMesh : public TriMesh
{
public:
    explicit 		 PolySurfaceTriMesh(const GU_Detail *gdp,
		    		GA_RWHandleI vtx_pt_attr = GA_RWHandleI(),
    				Tessellation tessellation = TRIANGULATE_ALL);

    SYS_FORCE_INLINE
    const GU_Detail	*getDetail() const { return myGdp; }

    SYS_FORCE_INLINE
    const
    GA_OffsetArray	&polys() const { return myPolys; }

    SYS_FORCE_INLINE
    void	         appendPoly(GA_Offset poly) { myPolys.append(poly); }

    void		 buildMesh(const GEO_DetachedHedgeInterface *hip,
				const GA_EdgeGroup *seams = nullptr,
				UT_IntArray *shared_vtx_idx_array = nullptr);

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
    int			 geoVertexToMeshPoint(GA_Offset vtx) const
    			    { return myGeoVertexMeshPoint.get(vtx); }

    // Map a half-edge of the mesh to a vertex in the detail. This differs
    // from the meshVertexToGeoVertex in that only half-edges that map
    // exactly to source vertex of half-edges in the detail are mapped and
    // other half-edges are mapped to invalid offsets.

    GA_Offset		 meshHedgeToGeoVertex(int h) const;

    int		 	 geoVertexToMeshHedge(GA_Offset vtx) const;

    // Given a mesh point, find the position from the detail.
    UT_Vector3R		 getMeshPointPos3(int pt) const;

    SYS_FORCE_INLINE
    GA_Offset 		 meshVertexToGeoVertex(int v) const
    			    { return myMeshVertexGeoVertex(v); }

    SYS_FORCE_INLINE
    GA_Offset		 meshPointToGeoVertex(int pt) const
    			    { return meshVertexToGeoVertex(pointVertex(pt)); }

    Tessellation	 getTessellation() const { return myTessellation; }

    fpreal		 surfaceArea() const { return mySurfaceArea; }
    fpreal		 polyArea(exint poly_idx) const
    			    { return myPolyArea(poly_idx); }

protected:
    int			 numPrimaryPoints() const { return myNumPrimaryPoints; }

private:
    using MeshUptr = UT_UniquePtr<TriMesh>;
    using AttribUptr = UT_UniquePtr<GA_Attribute>;

    const GU_Detail 	*myGdp					= nullptr;

    Tessellation	 myTessellation;

    GA_RWHandleI	 myGeoVertexMeshPoint;
    AttribUptr		 myGeoVertexMeshPointAttrib;

    GA_OffsetArray	 myPolys;
    UT_FprealArray	 myPolyArea;
    GA_OffsetArray	 myMeshVertexGeoVertex;

    // Number of points in the mesh that arise (and are mapped back) to actual
    // geometry points. The points with index myNumPrimaryPoints and higher
    // are expected to be auxiliary points.

    int			 myNumPrimaryPoints			= 0;

    UT_Vector3RArray	 myAuxPointPos;
    fpreal		 mySurfaceArea				= 0.0;
};

class ConstraintSet;

// Islands encapsulate the needed abstractions for flattening a subset of
// polygons in a geometry detail. It takes care of building the triangle mesh
// required by flattening algorithms and the mapping between the physical
// geometry and the virtual mesh. Once created, polygons can be added to an
// island by calling appendPoly(). Once all island polygons are appended,
// a call to buildMesh() is needed to setup the internal structures before
// calling any of the flattenX() or related methods.

class GU_API Island : public PolySurfaceTriMesh
{
public:
    explicit 		 Island(const GU_Detail *gdp, int idx = -1,
		    		GA_RWHandleI vtx_pt_attr = GA_RWHandleI(),
    				Tessellation tessellation = TRIANGULATE_ALL) :
    			    PolySurfaceTriMesh(gdp, vtx_pt_attr, tessellation),
	    		    myIndex(idx) { }

    SYS_FORCE_INLINE
    int			 getIndex() const { return myIndex; }

    Status		 flatten(Method method,
				const ConstraintSet &constraints,
				RWHandleV3R uvh) const;

    Status		 flattenLSCM(const ConstraintSet &constraints,
	    			RWHandleV3R uvh) const;

    Status		 flattenSCP(const ConstraintSet &constraints,
	    			RWHandleV3R uvh) const;

    Status		 flattenLinearABF(const ConstraintSet &constraints,
	    			RWHandleV3R uvh) const;

    Status		 flattenProjection(RWHandleV3R uvh) const;

    fpreal		 calcArea(ROHandleV3R uvh,
    				UT_Vector3R *center = nullptr) const;

    void		 balance(RWHandleV3R uvh) const;

    void	 	 reposition(const ConstraintSet &constraints,
				ROHandleV3R &orig_uvh,
				RWHandleV3R &uvh,
				fpreal area_scale = 0.0,
				bool use_custom_pins = false) const;

    bool		 verifyFinite(ROHandleV3R &uvh) const;

private:
    int		 	 myIndex				= -1;
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
		 		Tessellation tessellation,
		 		const GEO_DetachedHedgeInterface *hip
		 		    	= nullptr);

		    	~IslandBundle();

    // Repartition the collection of islands along the islet_seams and assign
    // the created islets to (one of) parent islands.
    void		 repartitionIslands(const GA_EdgeGroup *new_seams);

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
    void 		 forEachIsland(Func func)
    {
	forEachIslandIndex([&](int i) { func(island(i)); }, false);
    }

    template<typename Func>
    void 		 parallelForEachIsland(Func func)
    {
	forEachIslandIndex([&](int i) { func(island(i)); }, true);
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
    void 		 forEachIsland(Func func) const
    {
	forEachIslandIndex([&](int i) { func(island(i)); }, false);
    }

    template<typename Func>
    void 		 parallelForEachIsland(Func func) const
    {
	forEachIslandIndex([&](int i) { func(island(i)); }, true);
    }

    template<typename Func>
    void 		 forEachDirtyIsland(Func func) const
    {
	forEachIslandIndex([&](int i)
	   {
	       if (isDirty(i))
		   func(island(i));
	   }, false);
    }

    template<typename Func>
    void 		 parallelForEachDirtyIsland(Func func) const
    {
	forEachIslandIndex([&](int i)
	   {
	       if (isDirty(i))
		   func(island(i));
	   }, true);
    }

    template<typename Func>
    void 		 forEachDirtyIsland(Func func)
    {
	forEachIslandIndex([&](int i)
	   {
	       if (isDirty(i))
		   func(island(i));
	   }, false);
    }

    template<typename Func>
    void 		 parallelForEachDirtyIsland(Func func)
    {
	forEachIslandIndex([&](int i)
	   {
	       if (isDirty(i))
		   func(island(i));
	   }, true);
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

    GA_RWHandleI	&getVertexPointAttrib(bool primary);

    int		 	 getNext(int i) const
			    { return myIslandInfos(i).getNext(); }

    void		 setNext(int i, int j)
			    { return myIslandInfos(i).setNext(j); }

    bool		 isVacant(int i) const
    			    { return myIslandInfos(i).isVacant(); }

    int		 	 newIsland(bool primary = true);
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

    void		 generateLabeledSeams(const GA_EdgeGroup *seams,
				LabeledEdgeArray &labeled_seams) const;


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
    Tessellation	 myTessellation;

    const GU_Detail	*myGdp;

    HedgeInterfaceUptr	 myHiup;
    HedgeInterface	*myHip;

    GA_RWHandleI	 myIslandVtxPt, myIsletVtxPt;

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

    // Returns true if the flattening result under this constraint set
    // is going to be interpretable as a repositioning of those in the
    // "other" set.
    bool		 isRepositioningOf(const ConstraintSet &other) const;

    bool		 operator==(const ConstraintSet &other) const;
    bool		 operator!=(const ConstraintSet &other) const
    			    { return !(*this == other); }

    struct BaseInfo
    {
			 BaseInfo() = default;
			 BaseInfo(int id) : myId(id) { }

	SYS_FORCE_INLINE
	bool		operator==(const BaseInfo &other) const
			    { return true; }

    	SYS_FORCE_INLINE
    	int		 id() const { return myId; }

	int		 myId					= -1;
    };

    struct PinInfo : public BaseInfo
    {
			 PinInfo() = default;
			 PinInfo(int pt, UT_Vector3R uv, int id = -1,
			 	int ref_pt = -1, UT_Vector3R refuv
			 				= { 0, 0, 0 }) :
			    BaseInfo(id), myPoint(pt), myUV(uv),
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

    struct AlignInfo : public BaseInfo
    {
			AlignInfo() = default;
    	explicit	AlignInfo(AlignDir dir, int id = -1) :
    			    BaseInfo(id), myDir(dir) { }

	SYS_FORCE_INLINE
	AlignDir	dir() const { return myDir; }

	SYS_FORCE_INLINE
	bool		operator==(const AlignInfo &other) const
			    { return myDir == other.myDir; }

    private:
	AlignDir	myDir;
    };

    using StraightenInfo = BaseInfo;
    using PinSet = ElementConstraintList<PinInfo>;
    using AngleSet = ElementConstraintList<AngleInfo>;
    using AlignGroupSet = GroupConstraintList<AlignInfo>;
    using StraightenGroupSet = GroupConstraintList<StraightenInfo>;

    const PinSet	&pins() const { return myPins; }
    PinSet		&pins() { return myPins; }

    const
    AngleSet		&angles() const { return myAngles; }
    AngleSet		&angles() { return myAngles; }

    const
    AlignGroupSet	&alignedSets() const { return myAlignGroups; }
    AlignGroupSet	&alignedSets() { return myAlignGroups; }

    const
    StraightenGroupSet	&straightSets() const { return myStraightGroups; }
    StraightenGroupSet	&straightSets() { return myStraightGroups; }

private:

    PinSet		 myPins;
    AngleSet		 myAngles;
    AlignGroupSet	 myAlignGroups;
    StraightenGroupSet	 myStraightGroups;
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

    bool		 addAlignGroup(const GA_OffsetArray &vtxs,
				AlignDir dir, int id = -1);

    bool		 addStraightenGroup(const GA_OffsetArray &vtxs,
				int id = -1);

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

    // static
    ConstraintSet	 theEmptyConstraintSet;
};

using AngleConstraint = ConstraintSet::AngleInfo;
using AngleConstraintArray = UT_Array<AngleConstraint>;

// Flatten the mesh by projecting it onto its best fitting plane.

GU_API Status 	 	 flattenProjection(const TriMesh &mesh,
				const GetFunctor<UT_Vector3R> &get_pt_pos,
				const SetFunctor<UT_Vector2R> &set_pt_uv);

// Least Squares Conformal Map flattening of mesh. The first two pins in
// constraint set are used to flatten the island. If less than two pins are
// given, the two flattening pins are picked automatically.

GU_API Status 	 	 flattenLSCM(const TriMesh &mesh,
				const ConstraintSet &constraints,
				const GetFunctor<UT_Vector3R> &get_pt_pos,
				int outer_boundary_comp,
				const SetFunctor<UT_Vector2R> &set_pt_uv);

// Least Squares Conformal Map flattening of mesh using explicitly-specified
// points to pin and supplied conformal energy data.

GU_API Status 	 	 flattenLSCM(const TriMesh &mesh,
				const ConstraintSet &constraints,
				const GetFunctor<fpreal> &hedge_apx_cotan,
				const GetFunctor<fpreal> &hedge_tri_area,
				int pin0_pt, int pin1_pt,
				const SetFunctor<UT_Vector2R> &set_pt_uv);

GU_API Status 	 	 flattenSCP(const TriMesh &mesh,
				const ConstraintSet &constraints,
				const GetFunctor<UT_Vector3R> &get_pt_pos,
				int outer_boundary_comp,
				const SetFunctor<UT_Vector2R> &set_pt_uv);

GU_API Status 	 	 flattenSCP(const TriMesh &mesh,
				const ConstraintSet &constraints,
				const GetFunctor<fpreal> &hedge_apx_cotan,
				const GetFunctor<fpreal> &hedge_tri_area,
				int outer_boundary_comp,
				const SetFunctor<UT_Vector2R> &set_pt_uv);

GU_API Status 	 	 flattenLinearABF(const TriMesh &mesh,
				const ConstraintSet &constraints,
				const GetFunctor<UT_Vector3R> &get_pt_pos,
				int outer_boundary_comp,
				const SetFunctor<fpreal> &set_vtx_angle);

GU_API void 		 generateStraighLoopConstraints(const TriMesh &mesh,
			       const ConstraintSet &constraints,
			       AngleConstraintArray &angle_constraints);

GU_API void 		 calcConformalEnergyData(const TriMesh &mesh,
				const GetFunctor<UT_Vector3R> &get_pt_pos,
				UT_FprealArray &hedge_apx_cotan,
				UT_FprealArray &hedge_tri_area);


GU_API int		 longestBoundaryComponent(const TriMesh &mesh,
				const GetFunctor<UT_Vector3R> &get_pt_pos);

GU_API void		 pickLSCMDefaultPins(const TriMesh &mesh,
				const GetFunctor<UT_Vector3R> &get_pt_pos,
				int outer_boundary_comp,
				int &pin0_pt, int &pin1_pt);

} // namespace GU_UVFlatten;


#endif
