/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PolyBridge.h (GU Library, C++)
 *
 * COMMENTS:
 *
 */


#ifndef __GU_PolyBridge_h__
#define __GU_PolyBridge_h__

#include "GU_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_Ramp.h>
#include <GA/GA_Types.h>
#include <GA/GA_Edge.h>
#include <GA/GA_ElementGroup.h>
#include <GA/GA_Handle.h>
#include <GA/GA_ElementWrangler.h>
#include <GEO/GEO_Face.h>
#include <GEO/GEO_PolyCounts.h>

class GU_Detail;
class GEO_Face;
class GA_EdgeGroup;
class GA_Attribute;
class UT_StringStream;
class GA_AttributeRefMap;
class GU_Spine;
class GEO_PrimPoly;

/// A GU_Spine encapsulates a "spine curve": an open curve together
/// with a rotation minimizing orthonormal frame at every point. You can
/// create a spine by supplying the two points and the tangents to the curve
/// at the each of them as well as the curve normal at the first point. This
/// will normally create a smooth curve between the two points with the given
/// tangents. The magnitudes of the tangents can be controlled. Furthermore,
/// it is possible to force the spine to move straight along a tangent for a
/// an initial part of the curve and only start to blend into the other
/// direction during a given range. It is also possible to provide an external
/// curve after which to model the spine. The external curve is positioned
/// according to the supplied positioning method and the source and destination
/// directions are blended separately according to their respective blending
/// ranges with this curve. The two resulting curves are then blended using a
/// cubic smooth step function. The main output method is sample() which
/// samples the generated spine to produce arrays of point positions, tangents,
/// and normals. An evaluate spine method is also provided which evaluates
/// the spine curve on a [0-1] parameterization. Note that rotation minimizing
/// frames are only evaluated during sampling and cannot be accessed for
/// random access.

class GU_API GU_Spine
{
public:
    enum Positioning
    {
	AS_IS = 0,
	SRC_TO_DST,
	ALONG_SRC_DIR,
	ALONG_DST_DIR,
	TANGENT_TO_SRC_DIR,
	TANGENT_TO_DST_DIR,
	TRANSLATE_TO_SRC,
	TRANSLATE_TO_DST
    };

    enum SampleMode
    {
	PARAMETER_UNIFORM = 0,
	ARC_LENGTH_UNIFORM,
	CURVATURE_ADAPTIVE
    };

			 GU_Spine(GEO_Face *spine_curve = NULL,
				bool reverse = false,
				fpreal u0 = 0.0, fpreal u1 = 1.0,
				bool straight = false);

			~GU_Spine();

    void		 sample(int size, SampleMode sample_mode,
				UT_Vector3Array &pos,
				UT_Vector3Array &tan,
				UT_Vector3Array &norm,
				UT_FprealArray &uvals,
				bool miter, UT_Vector3Array *expands);

    void		 position(UT_Vector3 src_pos, UT_Vector3 src_tan,
				UT_Vector3 src_norm, UT_Vector3 dst_pos,
				UT_Vector3 dst_tan, Positioning = SRC_TO_DST,
				fpreal axial_rotation = 0.0);

    void		 setStiffness(fpreal src_stiff, fpreal dst_stiff);
    void		 setBlend(fpreal src_blend, fpreal dst_blend);
    void		 setClipRange(fpreal c0, fpreal c1);

    void 		 getClip0(UT_Vector3 &p, UT_Vector3 &t, UT_Vector3 &n);
    void 		 getClip1(UT_Vector3 &p, UT_Vector3 &t, UT_Vector3 &n);

    /// Evaluate the spine curve at parameter value u. The second parameter
    /// du can only be 0 or 1 to evaluate the point position or the tangent
    /// vector.

    UT_Vector3		 evalSpine(fpreal u, int du);

    fpreal		 evalExternalAttribF(GA_ROHandleF ah, fpreal u,
					UT_Array<GA_Offset> &offsets,
					UT_FloatArray &weights,
					GA_AttributeOwner owner);

private:
    void		 setup();
    void		 sampleParameter(SampleMode sample_mode,
				UT_FprealArray &sample_u);

    UT_Vector3		 evalBaseCurve(fpreal u, int du);
    UT_Vector3		 evalSrcCurve(fpreal u, int du);
    UT_Vector3		 evalDstCurve(fpreal u, int du);
    UT_Vector3		 evalPolyLineCurve(fpreal u, int du);
    void		 getPolyLineWeights(fpreal u, int &i0, int &i1,
				fpreal &w0, fpreal &w1);


    void		 setupPolyLineSpineCurve();

    inline fpreal	 unitToRealDomain(fpreal u_unit);

    inline UT_Vector3	 evalExternalCurve(fpreal u, int du = 0);

    UT_Vector3		 positionCurvePoint(UT_Vector3 x);

    inline int		 getNumCVs()
			    {
				return mySpineCurve ?
				    mySpineCurve->getFastVertexCount() :
				    myNumCVs;
			    }

    inline UT_Vector3	 getCV(int i)
			    {
				return (mySpineCurve) ?
					positionCurvePoint(
					    mySpineCurve->getDetail().getPos3(
						mySpineCurve->getPointOffset(i))) :
					myCVs[i];
			    }

    fpreal		 myClipU0, myClipU1;
    fpreal		 myU0, myU1;

    UT_Vector3		 myClipP0, myClipT0, myClipN0;
    UT_Vector3		 myClipP1, myClipT1, myClipN1;

    UT_Vector3		 mySrcP, mySrcT, mySrcN, myDstP, myDstT;

    fpreal		 mySrcStiffness, myDstStiffness;
    fpreal		 mySrcBlend, myDstBlend;

    Positioning	 	 myPositioninig;
    fpreal		 myAxialRotation;

    bool		 myIsPolyLine: 1;
    bool		 myReverseSpine: 1;
    bool		 myStraight: 1;
    bool		 myTrivial: 1;
    bool		 myInternalSpine: 1;

    GEO_Face		*mySpineCurve;

    fpreal		 myPolyLineLength;
    UT_FprealArray	 myPolyLineBreakpoints;

    int			 myNumCVs;
    UT_Vector3		 myCVs[6];

    fpreal		 myScale;
    UT_Vector3		 myOrigin;
    UT_Vector3		 myTranslate;
    UT_Quaternion	 myCurveToBridgeQ;
    UT_Quaternion	 myAxialRotationQ;


};

fpreal
GU_Spine::unitToRealDomain(fpreal u_unit)
{
    if (u_unit < 0.0f)
	u_unit = 0.0f;
    else if (u_unit > 1.0f)
	u_unit = 1.0f;

    return u_unit * fpreal(myNumCVs - 3);
}

/// GU_PolyBridge encapsulates most of the functionality of the polybridge sop.
/// It can generate a bridge between a source and a destination edge loop each
/// of which can be open or closed.


class GU_API GU_PolyBridge
{
public:

    enum TextureMode
    {
	INTERPOLATE = -1,
	UNIT_SQUARE,
	RECTANGLE,
	PROPORTIONAL
    };

    enum TextureScaling
    {
	FIT_UNIT_SQUARE = 0,
	MATCH_3D,
	MATCH_UV
    };

			 GU_PolyBridge(GU_Detail *gdp,
				 const GA_OffsetArray &src_chain,
				 bool src_chain_closed,
				 const GA_OffsetArray &dst_chain,
				 bool dst_chain_closed,
				 bool straight_bridge = true);

			~GU_PolyBridge();

    typedef std::pair<int, int> IndexPair;
    typedef UT_Array<IndexPair> IndexPairArray;

    inline void		 buildTopology(int divisions,
				int src_divisions = 0,
				int dst_divisions = 0,
				IndexPairArray *pairings = nullptr)
    {
	computeBridge(GA_INVALID_OFFSET, GA_INVALID_OFFSET, divisions,
		      src_divisions, dst_divisions, 0, pairings);
    }

    inline void		 writeGeometry(GA_Offset ptoff0,
				GA_Offset primoff0,
				int num_twists = 0,
				const GU_Detail *tgdp = nullptr,
				const GA_Offset *src_rep_vtx = nullptr,
				const GA_Offset *dst_rep_vtx = nullptr,
				GA_PointWrangler *pt_wrangler = nullptr,
				GA_PrimitiveWrangler *prim_wrangler = nullptr,
				GA_VertexWrangler *vtx_wrangler = nullptr)
    {
	computeBridge(ptoff0, primoff0, -1, -1, -1, num_twists, nullptr, tgdp,
		      src_rep_vtx, dst_rep_vtx, pt_wrangler, prim_wrangler,
		      vtx_wrangler);
    }


    void		 buildBridge(const GA_Offset *src_rep_vtx,
				const GA_Offset *dst_rep_vtx,
				GA_OffsetArray *user_link_grp = 0,
				GA_OffsetArray *auto_link_grp = 0,
				GA_OffsetArray *boundary_link_grp = 0,
				GA_OffsetArray *src_chain_grp = 0,
				GA_OffsetArray *dst_chain_grp = 0,
				int num_twists = 0);


    int			 getNumMeshPolys();
    int			 getNumMeshPolyVtxs();

    GA_Offset		 setupBatchBuild(GA_Offset ptoff0,
			       UT_IntArray &quad_pt_nums,
			       GEO_PolyCounts &quad_size_list);

    void		 fillGroups(GA_Offset ptoff0,
				GA_Offset primoff0,
				GA_OffsetArray *user_link_grp,
				GA_OffsetArray *auto_link_grp,
				GA_OffsetArray *boundary_link_grp,
				GA_OffsetArray *src_chain_grp,
				GA_OffsetArray *dst_chain_grp);
    enum DirectionSign
    {
	DIR_SIGN_AUTO = 0,
	DIR_SIGN_POS,
	DIR_SIGN_NEG
    };

    enum MorphMethod
    {
	LINEAR = 0,
	ROTATING_FRAME,
    };

    typedef GU_Spine::Positioning	SpinePositioning;
    typedef GU_Spine::SampleMode	SpineSampleMethod;

    UT_Vector3		 getSrcCentroid()
			    { setupEnds(); return mySrcCtr; }

    UT_Vector3		 getDstCentroid()
			    { setupEnds(); return myDstCtr; }

    void		 setWarningStream(UT_StringStream *w)
			    { myWarningStream = w;  }

    void		 setThicknessAttrib(GA_Attribute *attrib)
			    { mySpineThickness = attrib; }

    void		 setTwistAttrib(GA_Attribute *attrib)
			    { mySpineTwist = attrib; }

    void		 setMorphMethod(MorphMethod m)
			    { myMorphMethod = m; }

    void 		 setMagnitudes(fpreal s = 1.0, fpreal d = 1.0)
			    { mySrcMagnitude = s; myDstMagnitude = d; }

    void 		 setStiffnesses(fpreal s = 0.0, fpreal d = 0.0)
			    { mySrcStiffness = s; myDstStiffness = d; }

    void		 setAxialRotation(fpreal r)
			    { myAxialRotation = r; }

    void		 setExternalSpine(GEO_Face *curve)
			    { myExternalSpine = curve; }

    void		 setThicknessRamp(UT_Ramp *ramp)
			    { myThicknessRamp = ramp; }

    void		 setTwistRamp(UT_Ramp *ramp)
			    { myTwistRamp = ramp; }

    void		 setSrcDir(UT_Vector3 dir)
			    { mySrcDir = dir; mySrcDirGiven = true; }

    void		 setDstDir(UT_Vector3 dir)
			    { myDstDir = dir; myDstDirGiven = true; }

    void		 setSrcCtr(UT_Vector3 ctr)
			    { mySrcCtr = ctr; mySrcCtrGiven = true; }

    void		 setDstCtr(UT_Vector3 ctr)
			    { myDstCtr = ctr; myDstCtrGiven = true; }

    void		 setAttachToSrc(bool attach)
			    { myAttachToSrc = attach; }

    void		 setAttachToDst(bool attach)
			    { myAttachToDst = attach; }

    void		 setThicknessScale(fpreal s)
			    { myThicknessUnit = s; }

    void		 setTwistRange(fpreal min, fpreal max)
			    { myMinTwist = min; myMaxTwist = max; }

    void		 setReverseSpine(bool r)
			    { myReverseSpine = r; }

    void		 setSrcDirectionSign(DirectionSign t)
			    { mySrcDirSign = t; }

    void		 setDstDirectionSign(DirectionSign t)
			    { myDstDirSign = t; }

    void		 setClipRange(fpreal s, fpreal e)
			    { myClipStart = s; myClipEnd = e; }

    void		 setGenerateSpine(bool b)
			    { myDoGenerateSpine = b; }

    void		 setSpineGroup(GA_PointGroup *grp)
			    { mySpinePointGroup = grp; }

    void		 setGenerateMesh(bool b)
			    { myDoGenerateMesh = b; }

    void		 setMeshGroup(GA_PrimitiveGroup *grp)
			    { myMeshPrimGroup = grp; }

    void 		 setTangentAttrib(GA_Attribute *attrib)
			    { myTangentAttrib = attrib; }

    void 		 setNormalAttrib(GA_Attribute *attrib)
			    { myNormalAttrib = attrib; }

    void 		 setBinormalAttrib(GA_Attribute *attrib)
			    { myBinormalAttrib = attrib; }

    void		 pairByEdgeCount(bool b)
			    { myPairByEdgeCount = b; }

    void		 setPairingShift(int s)
			    { myPairingShift = s; }

    void		 setExtSpineRange(fpreal s, fpreal e)
			    { mySpineStart = s; mySpineEnd = e; }

    void 		 setExtSpineBlend(fpreal s = 0.0, fpreal d = 0.0)
			    { mySrcBlend = s; myDstBlend = d; }

    void		 setExtSpinePositioning(SpinePositioning p)
			    { mySpinePlacement = p; }

    void		 setSpineSampleMethod(SpineSampleMethod m)
			    { mySpineSampleMethod = m; }

    void		 setMiterJoints(bool b)
			    { myMiterJoints = b; }

    void		 setScaleInvariant(bool b)
			    { myScaleInvariant = b; }

    void		 setPreTwist(fpreal t)
			    { myPreTwist = t; }

    void		 setSrcEdgeGroup(GA_EdgeGroup *grp)
			    { mySrcEdgeGroup = grp; }

    void		 setDstEdgeGroup(GA_EdgeGroup *grp)
			    { myDstEdgeGroup = grp; }

    void		 setChainRefs(int src_ref, int dst_ref)
			    { mySrcRef = src_ref; myDstRef = dst_ref; }

    void		 setSrcFrameUpDir(UT_Vector3 dir)
			    { mySrcFrameUpDir = dir;
				mySrcFrameUpDirGiven = true; }

    void		 setDstFrameUpDir(UT_Vector3 dir)
			    { myDstFrameUpDir = dir;
				myDstFrameUpDirGiven = true; }

    int			 getNumSrcLoopPts() const
			    { return mySrcLoop.entries(); }

    int			 getNumDstLoopPts() const
			    { return myDstLoop.entries(); }

    void		 setWranglePoint(bool w) { myWranglePt = w; }
    void		 setWranglePrimitive(bool w) { myWranglePrim = w; }
    void		 setWrangleVertex(bool w) { myWrangleVtx = w; }


    void		 setCollectedLinks(bool user_links, bool auto_links,
				bool boundary_links);

    void		 fillWrangleDetail(GA_Offset base_offset,
				GA_Offset *src_vtx_rep,
				GA_Offset *dst_vtx_rep,
				const GA_Offset *supplied_src_vtx_rep,
				const GA_Offset *supplied_dst_vtx_rep,
				GA_PrimitiveWrangler *prim_wrangler,
				GA_VertexWrangler *vtx_wrangler,
    				UT_Array<GA_RWHandleV3> &uvs,
				TextureMode uv_style = INTERPOLATE,
    				TextureScaling uv_scaling = FIT_UNIT_SQUARE);

private:

    void		 computeBridge(GA_Offset ptoff0,
				GA_Offset primoff0,
				int divisions,
				int src_divisions = 0,
				int dst_divisions = 0,
				int num_twists = 0,
				IndexPairArray *pairings = nullptr,
				const GU_Detail *tgdp = nullptr,
				const GA_Offset *src_rep_vtx = nullptr,
				const GA_Offset *dst_rep_vtx = nullptr,
				GA_PointWrangler *pt_wrangler = nullptr,
				GA_PrimitiveWrangler *prim_wrangler = nullptr,
				GA_VertexWrangler *vtx_wrangler = nullptr);

    void 		 generateLinks(UT_FprealArray &srcs,
				int src_first, int src_last,
			        UT_FprealArray &dst_breakpoitns,
				int dst_first, int dst_last,
				IndexPairArray &links,
				bool do_full_circle);

    fpreal		 unitize(UT_FprealArray &breakpoints,
				int i, int first = 0, int last = -1) const;

    UT_Vector3		 evaluateExternalSpine(fpreal t, int du = 0);

    UT_Vector3		 getLinkPointPosition(int i, fpreal t);


    UT_Vector3		 getLinkPointPositionLinear(int i, fpreal t);
    GU_Spine		*sampleSpine(int divisions);

    /// All of these methods return true if their results differ from the
    /// cached previous ones.
    bool		 setupEnds();
    bool 		 setupDirs();

    bool		 calcDefaultPairingRefs();
    void		 calcPositionsInEndFrames();
    void		 buildLinks(IndexPairArray *forced_pairs);

    /// Given a gramgement of a chain 'cycle', interpreted cyclically, with
    /// two reference indices 'start_cycle_ref' and 'end_cycle_ref'
    /// (respectively the indices at which the fragment starts and ends, and a
    /// second chain 'path', interpreted linearly, with two references indices
    /// 'path_prefix_end_ref' and 'path_suffix_begin_ref, with the former
    /// always being less than or equal to the latter, return the index
    /// 'idx' withing the cycle fragment which breaks it best in the sense that
    /// the ratio between the lengths "from cycle_start_ref to idx" and
    /// "from idx to cycle_end_ref" is as close as possible to
    /// ratio between length of the prefix of the path that ends at index
    /// path_prefix_end_ref and the suffix of the path that starts at
    /// index path_suffix_begin_ref. Note that start_cycle_ref may be equal
    /// to end_cycle_ref indicating that we want to divide the entire cycle.
    /// It is also possible for the path_pref_end_ref and path_suffix_begin_ref
    /// to be equal.

    int			 breakCycleRange(GA_OffsetArray &cycle,
				int start_cycle_ref, int end_cycle_ref,
				GA_OffsetArray &path,
				int path_prefix_end_ref,
				int path_suffix_start_ref);


    /// Given a set of pre-specified pairings between source and destination
    /// chain points, we must *sort* them out before generating the lines
    /// between consecutive pairs. When one or both links are closed (circular)
    /// this sorting becomes non-trivial. We deal with this by using the first
    /// pre-specified pair (s0, d0) of source and destination indices as a
    /// references. If both chains are closed, then we use s0 and d0
    /// respectively as the starting indices for the two circular lists. If
    /// one of the chains is open, then we pick the starting index on the
    /// closed chain so that all

    void		 findChainBaseIndices(IndexPairArray *forced_pairs,
				int &src_base_idx,
				int &dst_base_idx);

    void		 monotonizePairs(IndexPairArray &pairs,
				IndexPairArray &dropped_pairs,
				bool circular_first = false,
				bool circular_second = false);

    UT_Vector3		 calcLoopCentroid(GA_OffsetArray &loop, bool closed);
    UT_Vector3		 calcLoopNormalAndRadius(GA_OffsetArray &loop,
				bool closed, UT_Vector3 center,
				fpreal &radius, fpreal &length);

    int			 findLoopFarthestPoint(GA_OffsetArray &loop,
				UT_Vector3 from_pos,
				fpreal max_dist = -1.0,
				UT_Vector3 halfspace_normal
				    = UT_Vector3(0.0, 0.0, 0.0),
				fpreal topple_bump_factor = 0.01);

    GA_Offset		 appendPt(GA_Offset &baseoff,
				GA_Offset ptoff = GA_INVALID_OFFSET);

    void		 addLinksToEdgeGroup(const IndexPairArray &pairs,
				GA_EdgeGroup *grp);

    int			 findTwoAxisExtremePoint(const GA_OffsetArray &pts,
				UT_Vector3 ctr, UT_Vector3 tie_breaker,
				UT_Vector3 x_dir, UT_Vector3 y_dir,
				fpreal tol);

    GU_Detail		*buildWrangleDetail(GA_OffsetArray &src_vtx_rep,
				GA_OffsetArray &dst_vtx_rep,
				const GA_Offset *supplied_src_vtx_rep = nullptr,
				const GA_Offset *supplied_dst_vtx_rep = nullptr);

    void		 calcTextureRange(UT_Array<GA_ROHandleV3> &uvs,
				TextureMode uv_style, TextureScaling uv_scaling,
				UT_FprealArray &ranges);


    void 		 setWrangleDetailTextureCoords(GA_Offset base_offset,
				GA_OffsetArray &loop, bool loop_closed,
				int ref_idx, fpreal u, fpreal v_min,
			    	fpreal v_max, fpreal loop_len,
				GA_RWHandleV3 &uvh);

    SYS_FORCE_INLINE
    fpreal		 spineLength() const { return mySpineArcLength.last(); }
    enum LinkType
    {
	LINK_TYPE_NORMAL = 0,
	LINK_TYPE_USER,
	LINK_TYPE_AUTO,
	LINK_TYPE_BOUNDARY
    };

    void		 dumpChains();
    void		 dumpLinks(IndexPairArray *links,
				int src_base_idx = 0,
				int dst_base_idx = 0,
				UT_Array<LinkType> *types = NULL);

    class FirstThenSecond
    {
	public:
	inline bool operator()(const IndexPair &a, const IndexPair &b) const
	{
	    if (a.first == b.first)
		return a.second < b.second;
	    else
		return a.first < b.first;
	}
    };


    class SecondThenFirst
    {
	public:
	inline bool operator()(const IndexPair &a, const IndexPair &b) const
	{
	    if (a.second == b.second)
		return a.first < b.first;
	    else
		return a.second < b.second;
	}
    };

    bool		 myEndsAreSetUp: 1;
    bool		 myDirsAreSetUp: 1;
    bool		 myExternalSpineIsSetUp: 1;
    bool		 myInternalSpineIsSetUp: 1;
    bool		 myHaveDefaultPairingRefs: 1;
    bool		 myReverseSpine: 1;
    bool		 myMiterJoints: 1;

    bool		 mySrcDirGiven: 1;
    bool		 myDstDirGiven: 1;
    bool		 mySrcCtrGiven: 1;
    bool		 myDstCtrGiven: 1;

    bool		 mySrcFrameUpDirGiven: 1;
    bool		 myDstFrameUpDirGiven: 1;

    bool 		 myDoGenerateSpine: 1;
    bool		 myDoGenerateMesh: 1;

    bool		 myPairByEdgeCount: 1;

    bool		 mySrcClosed: 1;
    bool		 myDstClosed: 1;
    bool		 myAttachToSrc: 1;
    bool		 myAttachToDst: 1;

    bool		 myWranglePt: 1;
    bool		 myWrangleVtx: 1;
    bool		 myWranglePrim: 1;

    bool		 myCollectUserLinks: 1;
    bool		 myCollectAutoLinks: 1;
    bool		 myCollectBoundaryLinks: 1;

    bool		 mySpineIsStraight: 1;
    bool		 myScaleInvariant: 1;

    int 		 myPairingShift;
    int			 mySrcRef, myDstRef;

    GU_Spine		*mySpine;

    GA_PrimitiveGroup	*myMeshPrimGroup;
    GA_PointGroup	*mySpinePointGroup;

    GA_RWHandleV3	 myTangentAttrib;
    GA_RWHandleV3	 myNormalAttrib;
    GA_RWHandleV3	 myBinormalAttrib;

    GA_OffsetArray	 mySrcLoop, myDstLoop;

    fpreal		 mySrcRadius, myDstRadius;
    fpreal		 mySrcMagnitude, myDstMagnitude;
    fpreal		 mySrcStiffness, myDstStiffness;
    fpreal		 myClipStart, myClipEnd;
    fpreal		 mySpineStart, mySpineEnd;
    fpreal		 mySrcLength = 0, myDstLength = 0;
    UT_FprealArray	 mySpineArcLength;

    UT_Vector3		 mySrcDir, myDstDir;
    UT_Vector3 		 mySrcCtr, myDstCtr;
    UT_Vector3		 mySrcLoopNormal, myDstLoopNormal;
    UT_Vector3 		 mySrcFrameUpDir, myDstFrameUpDir;

    UT_Vector3Array	 mySrcLocalPos, myDstLocalPos;

    UT_IntArray		 myLinkSrcs, myLinkDsts;
    UT_Array<LinkType>	 myLinkTypes;

    MorphMethod		 myMorphMethod;

    UT_Ramp		*myThicknessRamp;
    fpreal		 myThicknessUnit;

    UT_Ramp		*myTwistRamp;
    fpreal		 myMaxTwist;
    fpreal 		 myMinTwist;

    GEO_Face 		*myExternalSpine;

    SpinePositioning	 mySpinePlacement;
    UT_Vector3 		 myCurveOrigin;
    UT_Vector3		 mySpineTranslate;
    fpreal 		 myCurveToBridgeScale;
    UT_Quaternion 	 myCurveToBridgeQ;
    fpreal		 myAxialRotation;
    UT_Quaternion 	 myAxialRotationQ;

    DirectionSign	 mySrcDirSign, myDstDirSign;

    UT_StringStream	*myWarningStream;

    UT_Vector3Array 	 mySpineP, mySpineT, mySpineN, myExpands;
    UT_FprealArray	 mySpineU;

    UT_Vector3 	 	 myStartClipP, myStartClipT, myStartClipN;
    UT_Vector3 		 myEndClipP, myEndClipT, myEndClipN;

    GU_Detail 		*myGdp;

    GA_RWHandleF	 mySpineThickness;
    GA_RWHandleF	 mySpineTwist;

    fpreal		 myPreTwist;
    fpreal		 myLastExternalSpinePos;
    fpreal 		 mySrcBlend, myDstBlend;

    GA_EdgeGroup	*mySrcEdgeGroup, *myDstEdgeGroup;

    SpineSampleMethod	 mySpineSampleMethod;

    int			 myNumQuads;
    int			 myNumNewPts;

    GA_OffsetArray	 myLocalPtOffset;
    UT_IntArray		 myQuadPtNums;
    GEO_PolyCounts 	 myQuadSizeList;

    GA_OffsetArray	 mySrcQuads, myDstQuads;
    GA_OffsetArray	 mySpinePts;

    GA_OffsetArray	 myUserLinkEdges;
    GA_OffsetArray	 myAutoLinkEdges;
    GA_OffsetArray	 myBoundaryLinkEdges;

    GA_Offset		 myInputLastPrimOffset;

    int			 myDivs, mySrcDivs, myDstDivs;

};

GU_API void		 GUbatchBuildBridges(GU_Detail *gdp,
				GU_PolyBridge **bridges,
				int num_bridges,
				const GA_Offset *src_rep_vtxs = nullptr,
				const GA_Offset *dst_rep_vtxs = nullptr,
				GA_PrimitiveGroup *mesh_prims = nullptr,
				GA_OffsetArray *user_link_grp = nullptr,
				GA_OffsetArray *auto_link_grp = nullptr,
				GA_OffsetArray *boundary_link_grp = nullptr,
				GA_OffsetArray *src_chain_grp = nullptr,
				GA_OffsetArray *dst_chain_grp = nullptr,
				int num_twists = 0,
				bool wrangle_pt_attribs = true,
				bool wrangle_prim_attribs = true,
				bool wrangle_vtx_attribs = true,
				GU_PolyBridge::TextureMode uv_style
					= GU_PolyBridge::INTERPOLATE,
				GU_PolyBridge::TextureScaling uv_scaling
					= GU_PolyBridge::FIT_UNIT_SQUARE);

/// This is a very useful tool to find the Minimum Rotation Frame for a
/// movement from x0 to x1 along a curve with tangent and normal t0 and n0
/// at x0 and tangent t1 and x1. It is based on the double-reflection method
/// of Wang, Juttler, Zheng, and Liu described in a paper titled
/// "Computation of Rotation Minimizing Frames", from ACM Transactions on
/// Graphics 2008.

GU_API UT_Vector3	 guRMFSlideFrame(UT_Vector3 x0, UT_Vector3 t0,
				UT_Vector3 n0, UT_Vector3 x1, UT_Vector3 t1);


#endif
