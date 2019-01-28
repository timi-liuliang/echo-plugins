/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PolyBevel.h (GU Library, C++)
 *
 * COMMENTS:
 *
 */


#ifndef __GU_PolyBevel_h__
#define __GU_PolyBevel_h__

#include "GU_API.h"
#include "GU_Detail.h"
#include <GEO/GEO_Hedge.h>
#include <GA/GA_Handle.h>
#include <GA/GA_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_StringStream.h>
#include <SYS/SYS_Inline.h>

#include <math.h>

class Profile;
class GU_Decompose;
class GU_Insetter;
class GU_PolyBevelParms;

class GEO_DetachedHedgeInterface;
class GEO_Face;
class GEO_PolyCounts;
class GEO_PrimPoly;

class GA_Attribute;
class GA_EdgeGroup;
class GA_PointGroup;
class GA_PointWrangler;
class GA_PrimitiveGroup;
class GA_PrimitiveWrangler;
class GA_VertexWrangler;

#define EXPLICIT_ELEMENT_INDEX	1


class GU_API GU_PolyBevel
{
public:
    enum FilletShape
    {
	NONE = 0,
	SOLID,
	CREASE,
	CHAMFER,
	ROUND,
	PROFILE
    };

    enum OffsetMode
    {
	FIXED_DISTANCE = 0,
	FIXED_WIDTH,
	RELATIVE_DISTANCE
    };

    enum ProfileSampling
    {
	UNIFORM = 0,
	CURVATURE_SENSITVE,
	CONTROL_POINTS
    };

			 GU_PolyBevel(GU_Detail *gdp,
				const GU_Detail *rest_gdp = nullptr,
				const GEO_DetachedHedgeInterface *h = nullptr);

			~GU_PolyBevel();

    void		 bevelEdges(fpreal offset,
				const GA_EdgeGroup *edges,
				OffsetMode mode,
				FilletShape shape,
				int divs,
				const GU_PolyBevelParms *parms);

    void		 bevelPoints(fpreal offset,
				const GA_PointGroup *points,
				FilletShape shape,
				int divs,
				const GU_PolyBevelParms *parms);

    void		 saveMeshBoundaryEdgesToDetailArray(const char *name);
    void		 saveBevelPointsToDetailArray(const char *name);
    void		 saveMeshInteriorEdgesToDetailArray(const char *name);
    void		 saveLimitPtCoordsToDetailArray(const char *attrib_name);

    const
    UT_Fpreal32Array	&getLimitPtCoords() { return myLimitPtCoords; }

    const std::string	 getWarningMessage()
    			    { return myWarnings.str().toStdString(); }

    struct 		 PatchSet;
    struct 		 UVPatchSet;
    class 		 Positioner;

#if EXPLICIT_ELEMENT_INDEX
    struct ElementIndex
    {
	explicit	 ElementIndex() {}
	explicit	 ElementIndex(int index) : myIndex(index) {}

	explicit
	operator  	 int() const { return myIndex; }

	bool	 	 operator==(const ElementIndex &other) const
			    { return myIndex == other.myIndex; }

	bool	 	 operator!=(const ElementIndex &other) const
			    { return myIndex != other.myIndex; }

    private:
	int		 myIndex;
    };

    struct Patch : public ElementIndex
    {
	using ElementIndex::ElementIndex;
    };

    struct Mesh: public ElementIndex
    {
	using ElementIndex::ElementIndex;
    };

#else

    typedef int		 ElementIndex;
    typedef ElementIndex Patch;
    typedef ElementIndex Mesh;
#endif

private:

    struct SplitRecord;
    struct SplitDraft;
    struct FanEdge;
    struct VertexFan;
    struct VertexFanHandle;
    struct VertexFanChain;
    struct SliceInfo;
    struct EdgeOffset;

    void		 bevel(fpreal offset, FilletShape shape, int divs);

    void		 buildRimTopology(FilletShape shape);

    void		 buildFilletTopology(FilletShape shape,
				int divisions,
				GA_PrimitiveGroup *edge_prims,
				GA_PrimitiveGroup *corner_prims);

    void		 registerRimHedge(GA_Offset vtx, UT_Vector3 vel,
				uint flags);

    UT_Vector3		 offsetVector(const EdgeOffset &o0,
				const EdgeOffset &o1);

    UT_Vector3		 divergentOffsetVector(const EdgeOffset &o0,
				const EdgeOffset &o1);

    bool		 canOffsetAlongFold(const EdgeOffset &e0,
				const EdgeOffset &e1,
				const VertexFanHandle &fan,
				UT_Vector3 &v);

    fpreal		 offsetRatio(const EdgeOffset &e0,
				const EdgeOffset &e1,
				const VertexFanHandle &fan,
				UT_Vector3 &v);

    void		 draftPointBevelRimVertexSplits(SplitDraft &draft,
				FilletShape shape);

    void		 draftEdgeBevelRimVertexSplits(SplitDraft &draft,
				GU_Decompose &euler_paths);

    void		 splitPointBevelRimVertices(const SplitDraft &draft);
    void		 splitEdgeBevelRimVertices(const SplitDraft &draft,
				GU_Decompose &euler_paths);



    void		 deleteRedundantHubs();

#if 0
    void		 assignRimHedgeMates();
#endif

    void		 offsetFilletBoundary(fpreal inset_amount);

    void		 rebuildHedgeInterface(GA_PrimitiveGroup *prims = 0);


    enum EdgeError
    {
	EDGE_ERROR_NONE = 0,
	EDGE_ERROR_DEGENERATE,
	EDGE_ERROR_BOUNDARY,
	EDGE_ERROR_NONMANIFOLD,
	EDGE_ERROR_BADMANIFOLD,
	EDGE_ERROR_TOO_SHORT,
	EDGE_ERROR_TOO_SHARP,
	EDGE_ERROR_NONMANIFOLD_ENDPOINT
    };

    SYS_FORCE_INLINE
    void 		 addHedgeToBevelHedges(GEO_Hedge h,
				bool ignore_flat = false,
				fpreal flat_cos = 0.9999,
    				bool doing_all = false);

    SYS_FORCE_INLINE
    EdgeError 		 verifyHedgeForBevelling(GEO_Hedge h,
				bool ignore_flat = false,
				fpreal flat_cos = 0.9999);

    SYS_FORCE_INLINE
    void 		 addPointToBevelPoints(GA_Offset pt);


#if 0
    // We'll be making a mesh of patches, arcs, and peaks.
    GEO_Hedge	 	 mate(GEO_Hedge h);
#endif

    SYS_FORCE_INLINE int numRimComponents() const
    {
	return myRimCompStarts.entries() - 1;
    }

    SYS_FORCE_INLINE int rimCompSize(int k) const
    {
	return myRimCompStarts(k + 1) - myRimCompStarts(k);
    }

    // Index of the i-th edge in the k-th rim component
    SYS_FORCE_INLINE int rimCompHedgeIndex(int k, int i) const
    {
	return myRimCompStarts(k) + i;
    }

    SYS_FORCE_INLINE int mateIndex(int i) const
    {
	return myRimHedgeMate(i);
    }

    SYS_FORCE_INLINE int rimCompHedgeMateIndex(int k, int i) const
    {
	return mateIndex(rimCompHedgeIndex(k, i));
    }

    // The i-th edge in the k-th rim component
    SYS_FORCE_INLINE
    GEO_Hedge 		 rimCompHedge(int k, int i) const
    {
	return myRimHedges(rimCompHedgeIndex(k, i));
    }

    // The i-th edge in the k-th rim component
    SYS_FORCE_INLINE
    GEO_Hedge 		 rimCompHedgeMate(int k, int i) const
    {
	return myRimHedges(rimCompHedgeMateIndex(k, i));
    }

    SYS_FORCE_INLINE int numRimHedges() const
    {
	return myRimHedges.entries();
    }

    SYS_FORCE_INLINE
    GEO_Hedge 		 rimHedge(int i) const
    {
	return myRimHedges(i);
    }

    SYS_FORCE_INLINE
    UT_Vector3		 rimHedgeSrcVel(int i) const
    {
	return myRimHedgeSrcVel(i);
    }

    SYS_FORCE_INLINE
    uint32	 	 rimHedgeFlags(int i, uint32 mask = ~(uint32(0))) const
    {
	return (mask & myRimHedgeFlags(i));
    }

    SYS_FORCE_INLINE
    Patch 		 rimHedgePatch(int i) const
    {
	return myRimHedgePatch(i);
    }

    SYS_FORCE_INLINE
    Patch 		 rimCompHedgePatch(int k, int j) const
    {
	return myRimHedgePatch(rimCompHedgeIndex(k, j));
    }

    SYS_FORCE_INLINE
    Patch 		 rimCompHedgeMatePatch(int k, int j) const
    {
	return myRimHedgePatch(rimCompHedgeMateIndex(k, j));
    }

    UT_Vector3		 rotateHedge(GEO_Hedge h_ref, fpreal angle,
				fpreal base_angle, fpreal weight = 1.0) const;



    void		 dumpHedge(GEO_Hedge h) const;
    void		 dumpRimComponents();
    void		 dumpPatches();

    bool		 hasTargetedTangent(int side, Patch pach);
    bool		 needsMesh(int side, FilletShape shape, Patch ptch);

    int			 calcSlices(VertexFanHandle fan,
				int span_start, int span_end,
				int num_slices,
				fpreal start_angle, fpreal slice_angle,
				fpreal wt,
				SliceInfo *sinfo);

    int			 calcSplitLocations(VertexFanHandle fan,
				int span_start, int span_end,
				int num_slices,
				fpreal start_angle, fpreal slice_angle,
				fpreal wt,
				SliceInfo *sinfo);

    bool		 isBisectable(VertexFanHandle fan) const;

    bool		 sliceFlatFanSpan(VertexFanHandle fan,
				SplitDraft &draft,
				int span_start, int span_end,
				fpreal outer_slice_angle,
				fpreal inner_slice_angle,
				fpreal wt);

    void		 sliceClosedFanSpan(VertexFanHandle fan,
				SplitDraft &draft,
				int span_start, int span_end,
				fpreal outer_slice_angle,
				fpreal inner_slice_angle,
				fpreal wt);

    void		 closedFanSplit(VertexFanHandle fan,
				SplitDraft &draft,
				bool on_open_poly);

    void		 fullCircleSplit(VertexFanHandle fan,
				SplitDraft &draft);

    void		 unisplit(const EdgeOffset &e0, const EdgeOffset &e1,
				VertexFanHandle fan, SplitDraft &draft,
				UT_Vector3 vel, fpreal os0, fpreal os1);

    bool		 isFanBisector(const EdgeOffset &e0,
                                const EdgeOffset &e1, const UT_Vector3 &vf,
                                fpreal &a0, fpreal &a1);

    int			 mirrorFoldVector(const EdgeOffset &e0,
				const EdgeOffset &e1,
				const UT_Vector3 &vf, UT_Vector3 &vm);

    void		 bisplitFold(const EdgeOffset &e0, const EdgeOffset &e1,
				VertexFanHandle fan, SplitDraft &draft,
				fpreal os0, fpreal os1);

    void		 bisplitFlat(const EdgeOffset &e0, const EdgeOffset &e1,
				VertexFanHandle fan, SplitDraft &draft,
				fpreal os0, fpreal os1);


    void		 multisplit(const EdgeOffset &e0, const EdgeOffset &e1,
				VertexFanHandle fan, SplitDraft &draft,
				fpreal os0, fpreal os1);


    int			 findNextFold(VertexFanHandle fan, int idx0,
				fpreal *angle_sum = nullptr);

    int			 rewireToNextFold(VertexFanHandle fan,
				SplitDraft &draft,
				GEO_Hedge base_hedge, int idx0,
				bool rewire_first, bool rewire_last,
				fpreal bisect_angle = 0.0,
				UT_Vector3 bisector
                                          = UT_Vector3(0.0f, 0.0f, 0.0f),
				uint32 flags = 0, fpreal span_angle_sum = -1.0);

    int 		 rewireFanSpanToNextFold(VertexFanHandle fan,
				SplitDraft &draft,
				GEO_Hedge base_hedge, int idx0,
				bool rewire_first, bool rewire_last);

    SYS_FORCE_INLINE
    bool		 isUVSeam(GEO_Hedge h);

    int		 	 scanVertexFan(GEO_Hedge h0, GEO_Hedge h1,
				VertexFanChain &fan_chain,
				bool choose_ends = false);

    void		 dumpVertexFan(VertexFanHandle fan);

    void		 draftVertexFanSplit(const VertexFan &fan,
				SplitDraft &draft);

    void		 draftSplitPointFan(VertexFanHandle fan,
				SplitDraft &draft);
    void		 draftSplitVertexFan(const EdgeOffset &e0,
				const EdgeOffset &e1,
				VertexFanHandle fan,
				SplitDraft &draft, bool force_split,
				UT_Vector3 offset_vector,
				fpreal os0, fpreal os1);

    inline int		 numHubs() const
			    { return myHubs.entries(); }

    inline int		 hubIndex(GA_Offset pt) const
			    { return myHubIndex.get(pt); }

    inline GA_Offset	 hub(GA_Offset pt) const
			    { return myHubs(hubIndex(pt)); }

    inline int		 hubDegree(GA_Offset pt) const
			    { return myHubDegrees(hubIndex(pt)); }

    inline GA_Offset	 restPointOffset(GA_Offset pt) const
			    { return GA_Offset(myRestPtOff.get(pt)); }

    inline UT_Vector3	 hubPos(GA_Offset pt) const
			    { return myRestGdp->getPos3(restPointOffset(pt)); }

    void		 setupPatchAxes(FilletShape shape,
				int divisions,
				GA_Offset &baseoffset);

    // void		 dumpPatch(Patch ptch);
    void		 dumpPatchAxes(int divs);

    void		 meshPatchInterior(Patch ptch, FilletShape shape,
                                GA_Offset &base_pt, GA_Offset &base_prim,
				GEO_PolyCounts &poly_size_list,
				UT_IntArray &poly_pt_nums,
				GA_OffsetArray *edge_prim_offs,
				GA_OffsetArray *corner_prim_offs);

    void		 meshPatchJoint(Patch lbp, Patch rbp,
				GA_Offset &base_pt, GA_Offset &base_prim,
				GEO_PolyCounts &poly_size_list,
				UT_IntArray &poly_pt_nums,
				GA_OffsetArray *edge_prim_offs,
				GA_OffsetArray *corner_prim_offs);

    void		 meshPatchEndTriangle(Patch ptch, int side,
				GA_Offset &base_pt, GA_Offset &base_prim,
				GEO_PolyCounts &poly_size_list,
				UT_IntArray &poly_pt_nums,
				GA_OffsetArray *edge_prim_offs,
				GA_OffsetArray *corner_prim_offs);

    void		 setMeshCornerPositions(int divs);

    void		 setupEdgePatchAxes(Patch bp,
				bool even_divs, bool mesh_left,
				bool mesh_right, GA_Offset &base_pt);

    void		 setupVertexPatchAxes(Patch bp, bool even_divs,
				bool mesh_left, bool mesh_right,
				GA_Offset &base_pt);

    GA_Offset		 axisEndOnLeftBoundary(Patch bp);
    GA_Offset		 axisEndOnRightBoundary(Patch bp);

    void 		 meshEdgePatchMidDiv(Patch ptch, GA_Offset &base_prim,
				GEO_PolyCounts &poly_size_list,
				UT_IntArray &poly_pt_nums,
				GA_OffsetArray *edge_prim_offs,
				GA_OffsetArray *corner_prim_offs);

    void 		 generateHubCapPrim(GA_Offset hub, FilletShape shape,
                                Patch ptch, GA_Offset &base_prim,
				GEO_PolyCounts &poly_size_list,
				UT_IntArray &poly_pt_nums,
				GA_OffsetArray *edge_prim_offs,
				GA_OffsetArray *corner_prim_offs);

    void		 meshMiddleDivisions(FilletShape shape,
				GA_Offset &base_prim,
				GEO_PolyCounts &poly_size_list,
				UT_IntArray &poly_pt_nums,
				GA_OffsetArray *edge_prim_offs,
				GA_OffsetArray *corner_prim_offs);

    void		 assignEdgePatchMates();
    void		 generatePatches(int divs, FilletShape shape);

    void		 generateMeshes(FilletShape shape,
                                int divs,
				GA_Offset &base_pt,
				GA_Offset &base_prim,
				GEO_PolyCounts &poly_size_list,
				UT_IntArray &poly_pt_nums,
				GA_OffsetArray *edge_prim_offs,
				GA_OffsetArray *corner_prim_offs);

    void		 stitchFilletGapsToRim(int divisions);


    UT_Vector3		 tangentTowardsMate(int side, Patch ptch);
    UT_Vector3		 insetDirection(int side, Patch ptch);

    UT_Vector3		 vectorOverFold(UT_Vector3 v0, UT_Vector3 n0,
				UT_Vector3 n1);

    void		 updateAffectedNormals(const GA_PrimitiveGroup *,
				const GA_PrimitiveGroup *);

    void		 applyPointScale();

    fpreal		 locateUVTriangle(const UT_Vector3 &x, int prim_idx,
				GA_Offset &va, GA_Offset &vb, GA_Offset &vc,
				fpreal &ga, fpreal &gb, fpreal &gc);

    UT_Vector3		 calcRimVertexUV(GA_Offset vtx);
    UT_Vector3		 barycentricVertexUV(GA_Offset vtx, GA_Offset vta,
				GA_Offset vtb, GA_Offset vtc);

    void		 findRimTriangleVtxs(GA_Offset vtx,
				GA_Offset &vtx_prev, GA_Offset &vtx_next,
				UT_Vector3 &rest_pos, UT_Vector3 &rest_pos_prev,
				UT_Vector3 &rest_pos_next);

    UT_Vector3		 radialRimVertexUV(GA_Offset vtx);

    UVPatchSet 		*constructUVPatchSet();

    void		 calcEdgePatchInternalUVs(Patch ptch,
				UVPatchSet *uvpatches);

    void		 calcVertexPatchInternalUVs(Patch ptch,
				UVPatchSet *uvpatches);

    void		 calcVertexPatchMidDivUVs(Patch ptch,
				UVPatchSet *uvpatches);

    GA_Offset 		 hubPrimPoint(GA_Offset hub, Patch ptch, int side);

    GA_Offset 		 hubPrimAdjacentVertex(GA_Offset hub, Patch ptch,
				int side);



    void		 calcHubCapPrimUVs(int i);

    GA_Offset		 edgePatchDivVertex(int side, Patch ptch, int i,
				UVPatchSet *uvpatches = nullptr);

    void		 vertexPatchDivMeshVtxs(int side, Patch ptch, int div,
				GA_Offset &lv, GA_Offset &uv);

    void		 calcMeshUVs(Mesh mesh,
				UVPatchSet *uvpatches);

    void		 calcEdgePatchMidDivUVs(Patch ptch,
				UVPatchSet *uvpatches);

    void		 calcPatchAxisEndUVs(Patch ptch,
				UVPatchSet *uvpatches);

    void		 offsetRimVertexUVs(UVPatchSet *uvpatches);
    void		 updateAffectedUVs(UVPatchSet *uvpatches,
				FilletShape shape);

    void		 wrangleAttributes(FilletShape shape);

    void		 copyPointAttributes(GA_Offset pt, Patch start_ptch,
				GA_PointWrangler *ptw);

    void		 copyMeshPointAttributes(Mesh m, GA_Offset pt,
				GA_PointWrangler *ptw);

    void		 copyPatchAxisPointAttributes(int side, Patch ptch,
				GA_Offset pt, GA_PointWrangler *ptw);

    void		 lerpAttribsOnEdgePatch(Patch ptch,
				GA_PrimitiveWrangler *pw,
				GA_VertexWrangler *vw,
				GA_PrimitiveWrangler *pwx,
				GA_VertexWrangler *vwx,
				UT_IntArray &contribs,
				GA_PrimitiveWrangler *pws,
				GA_VertexWrangler *vws);

    void		 lerpAttribsOnVertexPatch(Patch ptch,
				GA_PrimitiveWrangler *pw,
				GA_VertexWrangler *vw,
				GA_PrimitiveWrangler *pwi,
				GA_VertexWrangler *vwi,
				GA_PrimitiveWrangler *pws,
				GA_VertexWrangler *vws);

    void		 lerpAttribsOnMidDivs(Patch ptch,
				GA_PrimitiveWrangler *pw,
				GA_VertexWrangler *vw,
				GA_PrimitiveWrangler *pws,
				GA_VertexWrangler *vws );

    void		 copyHubCapPrimAttributes(int i,
				GA_PrimitiveWrangler *pwi,
				GA_VertexWrangler *vw,
				GA_PrimitiveWrangler *pws,
				GA_VertexWrangler *vws);

    void		 lerpAttribsOnMesh(Mesh mesh,
				GA_PrimitiveWrangler *pw,
				GA_VertexWrangler *vw,
				GA_PrimitiveWrangler *pwi,
				GA_VertexWrangler *vwi,
				GA_PrimitiveWrangler *pws,
				GA_VertexWrangler *vws);

    template <typename T>
    void		 applyPositioner(T& positioner);

    void		 positionFilletPoints(FilletShape shape,
				const Profile *profile);

    void		 addVisBoundaryEdge(GA_Offset p0, GA_Offset p1);
    void		 addVisInteriorEdge(GA_Offset p0, GA_Offset p1);

    SYS_FORCE_INLINE
    bool		 haveWeightedEdges() const;

    SYS_FORCE_INLINE
    fpreal		 hedgeWeight(GEO_Hedge h) const;

    SYS_FORCE_INLINE
    UT_Vector3		 hedgeVector(GEO_Hedge h) const;

    SYS_FORCE_INLINE
    UT_Vector3		 hedgeNormal(GEO_Hedge h) const;

    SYS_FORCE_INLINE
    int			 rimPrimID(GA_Offset primoff) const;

    SYS_FORCE_INLINE
    int			 hedgePrimID(GEO_Hedge h) const;

    SYS_FORCE_INLINE
    bool		 isHedgeOnBoundaryCap(GEO_Hedge h) const;

    SYS_FORCE_INLINE
    GA_Offset 		 primFirstTriangle(int prim_idx) const
			    { return myRimPolyFirstTri(prim_idx); }

    SYS_FORCE_INLINE
    GA_Offset 		 triangleToNextTriangle(GA_Offset tri) const
			    { return myRimPolyNextTri(tri); }

    SYS_FORCE_INLINE
    GEO_PrimPoly 	*getPoly(GA_Offset primoff)
			    { return (GEO_PrimPoly *)
				myGdp->getPrimitive(primoff); }


    void		 prepareHubs();
    void		 constructRimPolyTriLists(int num_primitives);

    GA_PrimitiveGroup	*capBoundaryBevelPoints();

    void		 processRimPrims();
    void		 prepareRimPrims(const GA_EdgeGroup *edges);
    void                 prepareRimPrims(GA_PointGroup *points,
				bool bevel_open_poly_pts);

    void		 prepareBevelEdges(const GA_EdgeGroup *edges,
				bool ignore_flat, fpreal flat_normal_diff);

    void		 prepareBevelPoints(const GA_PointGroup *points);
    void		 recordEdgesToDissolve();
    void		 consolidateIncidentPrims();
    void		 restoreOriginalPrimitiveIndices();

    void		 markDegeneratePatches(FilletShape shape);
    void		 trivialBevelEdges();
    void		 reopenInputOpenPolys();
    void		 cleanup();

    typedef UT_Array<Patch> PatchArray;

    struct OpenPoly
    {
        GA_Index         myPoly;
        GA_Index         myFirstPt;
    };
    
    UT_Array<OpenPoly>   myOpenPolys;

    const GU_Detail	*myRestGdp;
    GU_Detail		*myGdp;

    PatchSet		*myPatches;

    GA_RWHandleV3	 myPrimNormal;

    UT_BitArray		 myRedundantHubs;
    GA_OffsetArray	 myHubs;
    GA_OffsetArray	 myHubCapPrims;
    GA_RWHandleI	 myHubIndex;
    GA_RWHandleI	 myRestPtOff;
    UT_IntArray		 myHubDegrees;
    UT_Array<Patch>	 myHubFirstPatch;

    GA_RWHandleF	 myHedgeWeight;

    GA_PointGroup	*myBevelPoints;
    GA_VertexGroup	*myBevelHedges;		// accepted input half-edges
    GA_PrimitiveGroup	*myRimPrims;
    GA_PointGroup	*myBoundaryPts;

    GA_VertexGroup	*myBridgeHedges;

    GA_PrimitiveGroup	*myBoundaryCapPrims;

    UT_IntArray		 myRimCompStarts;

    // parallel arrays for rim hedges
    UT_Array<GEO_Hedge>	 myRimHedges;		// rim hedges by index
    UT_Vector3Array	 myRimHedgeSrcVel;	// velocity of ith rim hedge
    PatchArray	 	 myRimHedgePatch;	// patch of ith rim hedge
    UT_IntArray		 myRimHedgeMate;	// mate of ith rim hedge
    UT_Int32Array	 myRimHedgeFlags;	// flags of ith rim hedge


    OffsetMode		 myOffsetMode;
    bool		 myOwnHedgeInterface;

    GA_OffsetArray	 myVisBevelPoints;
    GA_OffsetArray	 myVisBoundaryEdges;
    GA_OffsetArray	 myVisInteriorEdges;
    GA_OffsetArray	 myVisRejectedEdges;

    UT_Fpreal32Array	 myLimitPtCoords;

    fpreal		 myRadialInc;

    GA_RWHandleV3	 myUV;
    GA_ROHandleF	 myOffsetScale;

    GA_RWHandleI	 myRimPrimID;

    UT_IntArray		 myRimPolyClass;
    GA_OffsetArray	 myRimPolyFirstTri;
    GA_OffsetArray 	 myRimPolyNextTri;

    GA_Offset		 myCapPoint;

    UT_StringStream	 myWarnings;

    const GU_PolyBevelParms		*myParms;
    const GEO_DetachedHedgeInterface	*myHip;
};


class GU_API GU_PolyBevelParms
{
public:
    typedef GU_PolyBevel::ProfileSampling 	ProfileSampling;
    typedef GU_PolyBevel::OffsetMode 		OffsetMode;
    typedef GU_PolyBevel::FilletShape 		FilletShape;

    GU_PolyBevelParms(bool ignore_flat = false,
	    fpreal flat_angle = 0.03490658503989,	// 2 degrees
	    bool allow_splits = true,
	    bool split_single_folds = false,
	    bool split_flat_edges = false,
	    fpreal radial_inc = M_PI_2,
	    GA_Attribute *offscale = nullptr,
	    GA_Attribute *pscale = nullptr,
	    GA_PrimitiveGroup *edge_prims = nullptr,
	    GA_PrimitiveGroup *corner_prims = nullptr,
	    bool crease_edges = false,
	    fpreal round_convexity = 1.0,
	    const UT_Ramp *profile_ramp = nullptr,
	    const GEO_Face *profile_curve = nullptr,
	    bool symmetrize_profile = false,
	    fpreal profile_scale = 1.0,
	    ProfileSampling profile_sampling = GU_PolyBevel::UNIFORM) :
	myIgnoreFlat(ignore_flat),
	myAllowSplits(allow_splits),
	mySplitSingleFolds(split_single_folds),
	mySplitFlatEdges(split_flat_edges),
	myFlatAngle(flat_angle),
	myRadialInc(radial_inc),
	myOffsetScale(offscale),
	myPointScale(pscale),
	myEdgePrims(edge_prims),
	myCornerPrims(corner_prims),
	myRoundness(round_convexity),
	myProfileRamp(profile_ramp),
	myProfileCurve(profile_curve),
	myProfileScale(profile_scale),
	myProfileSampling(profile_sampling),
	mySymmetrizeProfile(symmetrize_profile),
	myReverseProfile(symmetrize_profile)
    { }

    inline bool		 ignoreFlat() const { return myIgnoreFlat; }
    void		 ignoreFlat(bool b) { myIgnoreFlat = b; }
    
    inline bool		 commonLimit() const { return myCommonLimit; }
    void		 commonLimit(bool b) { myCommonLimit = b; }

    inline bool 	 allowSplits() const { return myAllowSplits; }
    void		 allowSplits(bool b) { myAllowSplits = b; }

    inline bool 	 splitSingleFolds() const { return mySplitSingleFolds; }
    void		 splitSingleFolds(bool b) { mySplitSingleFolds = b; }

    inline bool 	 splitFlatEdges() const { return mySplitFlatEdges; }
    void		 splitFlatEdges(bool b) { mySplitFlatEdges = b; }

    inline bool 	 symmetrizeProfile() const
                            { return mySymmetrizeProfile; }
    void		 symmetrizeProfile(bool b)
                            { mySymmetrizeProfile = b; }

    inline bool 	 reverseProfile() const { return myReverseProfile; }
    void		 reverseProfile(bool b) { myReverseProfile = b; }

    inline fpreal 	 flatAngle() const { return myFlatAngle; }
    void		 flatAngle(fpreal f) { myFlatAngle = f; }

    inline fpreal 	 radialInc() const { return myRadialInc; }
    void		 radialInc(fpreal f) { myRadialInc = f; }

    GA_Attribute 	*offsetScale() const { return myOffsetScale; }
    void		 offsetScale(GA_Attribute *a) { myOffsetScale = a; }

    GA_Attribute 	*pointScale() const { return myPointScale; }
    void		 pointScale(GA_Attribute *a) { myPointScale = a; }

    GA_PrimitiveGroup 	*edgePrims() const { return myEdgePrims; }
    void		 edgePrims(GA_PrimitiveGroup *g) { myEdgePrims = g; }

    GA_PrimitiveGroup 	*cornerPrims() const { return myCornerPrims; }
    void		 cornerPrims(GA_PrimitiveGroup *grp)
                            { myCornerPrims = grp; }

    fpreal 		 roundness() const { return myRoundness; }
    void		 roundness(fpreal f) { myRoundness = f; }

    const UT_Ramp 	*profileRamp() const { return myProfileRamp; }
    void		 profileRamp(const UT_Ramp *r) { myProfileRamp = r; }

    const GEO_Face 	*profileCurve() const { return myProfileCurve; }
    void		 profileCurve(const GEO_Face *c) { myProfileCurve = c; }


    fpreal 		 profileScale() const { return myProfileScale; }
    void		 profileScale(fpreal f) { myProfileScale = f; }

    ProfileSampling 	 profileSampling() const { return myProfileSampling; }
    void	 	 profileSampling(ProfileSampling s)
                            { myProfileSampling = s; }

private:
    bool		 myIgnoreFlat;
    bool 		 myAllowSplits;
    bool 		 mySplitSingleFolds;
    bool		 mySplitFlatEdges;
    bool		 mySymmetrizeProfile;
    bool		 myReverseProfile;
    bool		 myCommonLimit;
    fpreal 		 myFlatAngle;
    fpreal 		 myRadialInc;
    GA_Attribute 	*myOffsetScale;
    GA_Attribute 	*myPointScale;
    GA_PrimitiveGroup 	*myEdgePrims;
    GA_PrimitiveGroup 	*myCornerPrims;
    fpreal 		 myRoundness;
    const UT_Ramp 	*myProfileRamp;
    const GEO_Face	*myProfileCurve;
    fpreal 		 myProfileScale;
    ProfileSampling 	 myProfileSampling;

};

#endif
