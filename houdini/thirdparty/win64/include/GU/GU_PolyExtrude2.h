/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 * COMMENTS: Poly Extrude code.
 *
 */
#ifndef __GU_PolyExtrude2_h__
#define __GU_PolyExtrude2_h__

#include "GU_API.h"
#include <UT/UT_Matrix4.h>
#include <UT/UT_Array.h>
#include <UT/UT_Vector3.h>
#include <GA/GA_Types.h>
#include <GEO/GEO_HedgeInterface.h>
#include "GU_PolyBridge.h"

class GA_Group;
class GA_PrimitiveGroup;
class GA_PointGroup;
class GA_PrimitiveGroup;
class GA_ElementWranglerCache;
class GEO_Face;
class GEO_PrimPoly;
class GU_Detail;
class GA_Attribute;
class GEO_DetachedHedgeInterface;
class GU_Decompose;

template<typename T, bool B> class GA_EdgeT; 
using GA_Edge = GA_EdgeT<GA_Offset, false>;


class GU_API GU_PolyExtrude2
{
public:

			 GU_PolyExtrude2(GU_Detail *gdp,
				const GA_PrimitiveGroup *prims,
				const GA_EdgeGroup *split_edges = NULL,
				bool split_all = false);

			 GU_PolyExtrude2(GU_Detail *gdp,
				const GA_EdgeGroup *edges,
				const GA_PointGroup *split_points = NULL,
				bool split_all = false);

			~GU_PolyExtrude2();

    enum ExtrusionMode
    {
	PRIM_OR_EDGE_NORMAL = 0,
	POINT_NORMAL,
    };

    typedef GU_PolyBridge::MorphMethod 		MorphMethod;
    typedef GU_PolyBridge::SpineSampleMethod 	SpineSampleMethod;

    // Performs the extrusion
    void		 extrude(fpreal distance = 0.0);

    // Methods to help placing a handle on the first component
    UT_Vector3		 getXformHandleCenter();
    UT_Matrix4		 getXformHandleXform();
    UT_Vector3		 getDraggerStart();
    UT_Vector3		 getDraggerDir();


    bool		 isTrivial() const;

    void		 setExtrusionMode(ExtrusionMode m)
			    { myExtrusionMode = m; }
    void		 setMorphMethod(MorphMethod m)
			    { myMorphMethod = m; }

    void		 setOutputFront(bool b) { myOutputFront = b; }
    void		 setOutputBack(bool b) { myOutputBack = b; }
    void		 setOutputSide(bool b) { myOutputSide = b; }

    void		 setUseExistingPointNormal(bool b)
    			    { myUseExistingNormal = b; }

    void		 setPointNormalAttrib(GA_ROHandleV3 ptnml)
    			    { mySuppliedPointNormal = ptnml; }

    void		 setFrontGroup(GA_PrimitiveGroup *grp)
			    { myAddFrontPrimGroup = grp; }

    void		 setFrontGroup(GA_EdgeGroup *grp)
			    { myAddFrontEdgeGroup = grp; }

    void		 setBackGroup(GA_PrimitiveGroup *grp)
			    { myAddBackPrimGroup = grp; }

    void		 setBackGroup(GA_EdgeGroup *grp)
			    { myAddBackEdgeGroup = grp; }

    void		 setSideGroup(GA_PrimitiveGroup *grp)
			    { myAddSidePrimGroup = grp; }

    void                 setFrontSeamGroup(GA_EdgeGroup *grp)
                            { myAddFrontSeamGroup = grp; }

    void                 setBackSeamGroup(GA_EdgeGroup *grp)
                            { myAddBackSeamGroup = grp; }

    void		 setSelectionGroup(GA_PrimitiveGroup *grp)
			    { myPrimSelection = grp; }

    void		 setSelectionGroup(GA_EdgeGroup *grp)
			    { myEdgeSelection = grp; }

    void		 setInset(float inset) { myInset = inset; }
    void		 setDivisions(int div) { myDivisions = div; }
    void		 setPreserveGroups(bool b) { myPreserveGroups = b; }
    void		 setStraightSpine(bool b) { myStraightSpine = b; }
    void		 setTwist(fpreal t) { myTwist = t; }
    void 		 setThicknessScale(fpreal s) { myThicknessScale = s; }
    void 		 setTwistScale(fpreal s) { myTwistScale = s; }
    void		 setThicknessRamp(UT_Ramp *r) { myThicknessRamp = r; }
    void		 setTwistRamp(UT_Ramp *r) { myTwistRamp = r; }
    void		 setDeviationTolerance(fpreal a)
			    { myDeviationToleranceCos = SYScos(a); }
    void		 setAxialRotation(fpreal r)  { myAxialRotation = r; }

    void		 setExtSpineBlend(fpreal s, fpreal d)
			    { myExtSpineSrcBlend = s; myExtSpineDstBlend = d; }

    void		 setXform(const UT_Matrix4 &frontxform,
			       bool is_global = false);

    void		 setExternalSpine(GEO_Face *curve)
			     { myExternalSpine = curve; }


    void		 setThicknessAttrib(GA_Attribute *attrib)
			    { myThicknessAttrib = attrib; }

    void		 setTwistAttrib(GA_Attribute *attrib)
			    { myTwistAttrib = attrib; }

    void		 setSpineRange(fpreal s, fpreal e)
			    { mySpineStart = s; mySpineEnd = e; }

    void		 setReverseSpine(bool b) { myReverseSpine = b; }
    void		 setMagnitudes(fpreal s, fpreal d)
			    { mySrcMagnitude = s; myDstMagnitude = d; }

    void		 setStiffnesses(fpreal s, fpreal d)
			    { mySrcStiffness = s; myDstStiffness = d; }

    void		 addVertexNormals(fpreal cusp_angle_degrees,
				bool cusp_front, bool cusp_back);

    void		 setSpineSampleMethod(SpineSampleMethod m)
			    { mySpineSampleMethod = m; }

    void		 limitInset(bool limit, bool common_limit)
			    { myLimitInset = limit;
				myCommonInsetLimit = common_limit; }

    void		 setLocalXDirAttrib(const char *attrib_name);
    void		 setLocalZDirAttrib(const char *attrib_name);
    void		 setLocalCtrAttrib(const char *attrib_name);
    void		 setLocalDistScaleAttrib(const char *attrib_name);
    void		 setLocalInsetScaleAttrib(const char *attrib_name);
    void		 setLocalTwistScaleAttrib(const char *attrib_name);
    void		 setLocalDivsScaleAttrib(const char *attrib_name);

    using TextureMode = GU_PolyBridge::TextureMode;
    using TextureScaling = GU_PolyBridge::TextureScaling;

    void 		 setSideTexturing(TextureMode mode,
				TextureScaling scaling);

    fpreal		 getCompInsetScale(int k);
    fpreal		 getCompDistScale(int k);
    fpreal		 getCompTwistScale(int k);
    fpreal		 getCompDivsScale(int k);

private:

    void		 initialize();
    void		 computeOpenPolyNormals();

    void		 buildPrimFront();
    void		 buildEdgeFront(bool generate_edge_polys);

    void		 setupGroupsAndAttribs();

    void		 buildPrimExtrusionMaps();
    void		 buildComponentExtrusionMap(int k);

    void		 buildEdgeExtrusionMap();
    void		 calculatePathNormals(bool trivial);

    void		 setFrontPointPositions();
    void		 transformEdgeFront(bool trivial);

    UT_Vector3		 calcBoundaryExtrusion(GEO_Hedge h0, GEO_Hedge h1,
				int comp);

    UT_Vector3		 calcInteriorExtrusion(GA_Offset pt, int comp);
    UT_Vector3		 calcInteriorExtrusion(GA_Offset pt, int comp,
				const GEO_DetachedHedgeInterface *fhip = 0);


    UT_Vector3		 calcBoundaryInset(GA_Offset pt0, GA_Offset pt1,
				GA_Offset pt2, GA_Offset prim0,
				GA_Offset prim1);

    UT_Vector3		 calcPathNormal(GA_Offset p0, GA_Offset p1,
				GA_Offset p2, int comp, UT_Vector3 *en0 = NULL,
				UT_Vector3 *en1 = NULL);

    UT_Vector3		 calcHedgeNormal(GEO_Hedge h, int &degree,
				UT_Vector3 *binormal = 0);

    void		 transformPrimFront(bool trivial);
    void		 insetPrimFront();

    GU_PolyBridge	*bridgeLoops(int comp, GA_OffsetArray &src_chain,
				GA_OffsetArray &dst_chain,
				bool closed);

    void		 bridgePrimComponents(int batch_size = -1);
    void		 bridgeEdgeComponents(int batch_size = -1);

    void                 buildPrimSeams();
    void                 buildEdgeSeams();

    void		 calcPrimLocalFrames();
    void		 calcPrimCompLocalFrame(int k);

    void		 calcEdgeLocalFrames();
    void		 calcEdgeCompLocalFrame(int k);

    void		 calcCenterAndNormal(int component_id,
				UT_Vector3 &center,
				UT_Vector3 &normal,
				bool add_extrusion = false,
				bool collect_from_attributes = false);

    UT_Vector3		 farthestBoundaryEdgePerp(int k);
    UT_Vector3		 longestBoundaryEdgeDir(int k);

    void		 updateAffectedNormals();

    UT_Vector3		 getMinAreaBoxXDirection(int comp);

    void		 fillOutputGroups();
    void		 cleanup();
    fpreal		 getRescaleFactor() const;

    void		 buildHandleSpaceChangMat();

    void		 saveEdgeGroupToDetailArray(const GA_EdgeGroup *grp,
				const char *attrib_name);

    void		 rescaleFront();

    void		 buildXformFromExternalSpine();

    bool		 isFrontXformTrivial() const;

    inline int		 numCompFrontPts(int k)
			    { return (myCompFrontFirstPt(k + 1) -
					myCompFrontFirstPt(k)); }

    inline GA_Offset	 compFrontPt(int k, int i)
			    { return myFrontPts(myCompFrontFirstPt(k) + i); }

    inline int		 numCompFrontPrims(int k)
			    { return (myCompFrontFirstPrim(k + 1) -
					myCompFrontFirstPrim(k)); }

    inline GA_Offset	 compFrontPrim(int k, int i)
			    { return myFrontPrims(myCompFrontFirstPrim(k) + i); }


    // NB: only used with edge extrusion
    inline int		 frontPtIdx(int k, int i)
			    { return (myCompFrontFirstPt(k) + i); }

    inline UT_Vector3	&frontPtExtrusion(int k, int i)
			    { return myFrontPtExtrusion(frontPtIdx(k, i)); }

    inline UT_Vector3	&frontPtPathNormal(int k, int i)
			    { return myFrontPtPathNormal(frontPtIdx(k, i)); }
    // ---

    // index of the the ith boundary of the kth component among all boundaries
    inline int		 frontBdIdx(int k, int j)
			    { return (myCompFrontFirstBd(k) + j); }

    // index of the ith vertex of the jth boundary
    inline int		 frontBdVtxIdx(int j, int i)
			    { return (myFrontBdFirstVtx(j) + i); }

    // The ith vertex of the jth front boundary
    inline GA_Offset	&frontBdVtx(int i)
			    { return myFrontBdVtxs(i); }

    // The ith vertex of the jth front boundary
    inline GA_Offset	&frontBdVtx(int j, int i)
			    { return myFrontBdVtxs(frontBdVtxIdx(j, i)); }

    // ith vertex of the jth boundary of the kth component
    inline GA_Offset	&compFrontBdVtx(int k, int j, int i)
			    { return frontBdVtx(frontBdIdx(k, j), i); }

    // total number of boundary components of the front
    inline int		 numFrontBds()
				{ return myCompFrontFirstBd.entries() - 1; }

    // number of boundaries of the kth front component
    inline int		 numCompFrontBds(int k)
			    { return (myCompFrontFirstBd(k + 1)
					- myCompFrontFirstBd(k)); }

    inline int		 numFrontBdVtxs()
			    { return myFrontBdVtxs.entries(); }

    // number of vertices in the jth front boundary
    inline int		 numFrontBdVtxs(int j)
			    { return (myFrontBdFirstVtx(j + 1)
					- myFrontBdFirstVtx(j)); }

    // number of vertices in the jth boundary of the kth front component
    inline int		 numCompFrontBdVtxs(int k, int j)
			    { return numFrontBdVtxs(frontBdIdx(k, j)); }

    // used only in primitive extrusion
    inline GA_Offset	 frontVtxToBackVtx(GA_Offset v)
			    { return GA_Offset(myFrontToBackVtxMap.get(v)); }

    // used only in edge extrusion
    inline GA_Offset	 frontPtToBackPt(GA_Offset pt)
			    { return GA_Offset(myFrontToBackPtMap.get(pt)); }


    inline int		 bdComp(int j) { return myBdComp(j); }

    // used in edge extrusion only
    inline int		 isFrontCompClosed(int k)
			    { return myFrontCompClosed.getBitFast(k); }


    typedef GU_PolyBridge::IndexPairArray 	IndexPairArray;

    GU_Detail		*myGdp;

    GA_PrimitiveGroup 	*myPrims;

    const
    GA_EdgeGroup 	*myEdges;

    GU_Decompose	*myDecompose;

    GA_RWHandleV3	 myPrimNormal;
    GA_RWHandleV3	 myPointNormal;

    UT_Vector3Array	 myBackCompCtr;		// Center used for back
    UT_Vector3Array	 myBackCompBary;	// Barycenter of the back

    UT_Vector3Array	 myCompXDir, myCompYDir, myCompZDir;

    UT_Vector3Array	 myFrontCompCtr;	// Calculated front center
    UT_Vector3Array	 myExtrudedCompCtr;	// Centroid of extruded comp
    UT_Vector3Array	 myFrontCompZDir, myFrontCompXDir;

    UT_FprealArray	 myCompDivsScale;
    UT_FprealArray	 myCompZScale;
    UT_FprealArray	 myCompInsetScale;
    UT_FprealArray	 myCompTwistScale;


    UT_Matrix4		 myHandleSpaceChangeMat;

    fpreal		 myDeviationToleranceCos;
    fpreal		 mySrcMagnitude, myDstMagnitude;
    fpreal		 mySrcStiffness, myDstStiffness;
    fpreal		 myExtSpineSrcBlend, myExtSpineDstBlend;

    UT_Vector3		 myDraggerStart, myDraggerDir;

    GA_PointGroup	*myFrontBoundaryPtGroup;
    GA_PrimitiveGroup	*myFrontPrimGroup;
    GA_PrimitiveGroup	*myBackPrimGroup;
    GA_PrimitiveGroup	*mySidePrimGroup;
    GA_EdgeGroup	*myFrontEdgeGroup;
    GA_EdgeGroup	*myBackEdgeGroup;

    GA_PrimitiveGroup	*myAddFrontPrimGroup;
    GA_PrimitiveGroup	*myAddBackPrimGroup;
    GA_PrimitiveGroup	*myAddSidePrimGroup;
    GA_EdgeGroup	*myAddFrontEdgeGroup;
    GA_EdgeGroup	*myAddBackEdgeGroup;
    GA_EdgeGroup	*myAddFrontSeamGroup;
    GA_EdgeGroup	*myAddBackSeamGroup;

    GA_PrimitiveGroup	*myPrimSelection;
    GA_EdgeGroup	*myEdgeSelection;

    ExtrusionMode	 myExtrusionMode;

    UT_Matrix4		 myFrontXform;
    fpreal		 myDistance;

    exint		 myNumFrontPts;
    exint		 myNumBackVtxs;

    MorphMethod		 myMorphMethod;

    bool		 myOutputFront: 1;
    bool		 myOutputBack: 1;
    bool		 myReverseBack: 1;
    bool		 myOutputSide: 1;
    bool		 myStraightSpine: 1;
    bool		 myPreserveGroups: 1;
    bool		 myReverseSpine: 1;
    bool		 myUseExistingNormal: 1;

    bool		 myAddVertexNormal: 1;
    bool		 myCuspFront: 1;
    bool		 myCuspBack: 1;

    bool		 myWranglePt: 1;
    bool		 myWranglePrim: 1;
    bool		 myWrangleVtx: 1;

    bool		 myXformIsTrivial: 1;
    bool		 myTrivialNormal: 1;

    bool		 myLimitInset: 1;
    bool 		 myCommonInsetLimit: 1;

    fpreal		 myCuspAngleDegrees;

    int			 myDivisions;
    fpreal		 myTwist;
    fpreal 		 myInset;
    fpreal		 myThicknessScale;
    fpreal		 myTwistScale;
    UT_Ramp		*myTwistRamp;
    UT_Ramp		*myThicknessRamp;

    GEO_Face 		*myExternalSpine;
    GA_Attribute	*myThicknessAttrib;
    GA_Attribute	*myTwistAttrib;

    fpreal		 mySpineStart;
    fpreal		 mySpineEnd;
    fpreal		 myAxialRotation;

    GA_ROHandleI	 myFrontPtIdx;
    GA_ROHandleI	 myFrontToBackVtxMap;
    GA_ROHandleI	 myFrontToBackPtMap;

    GA_ROHandleV3	 myLocalXDirAttr;
    GA_ROHandleV3	 myLocalZDirAttr;
    GA_ROHandleV3	 myLocalCtrAttr;
    GA_ROHandleF	 myLocalZScaleAttr;
    GA_ROHandleF	 myLocalInsetScaleAttr;
    GA_ROHandleF	 myLocalTwistScaleAttr;
    GA_ROHandleF	 myLocalDivsScaleAttr;

    GA_ROHandleV3	 mySuppliedPointNormal;

    int			 myNumComponents;
    int			 myNumCompBoundaries;

    SpineSampleMethod	 mySpineSampleMethod;

    UT_Vector3Array	 myFrontPtPos;
    UT_Vector3Array	 myFrontPtExtrusion;
    UT_Vector3Array	 myFrontPtInset;
    UT_Vector3Array	 myFrontPtPathNormal;

    bool		 myXformIsGlobal;

    GA_OffsetArray	 myFrontPts;
    UT_IntArray		 myCompFrontFirstPt;
    UT_BitArray		 myFrontCompClosed;	// edge extrusion use only

    GA_OffsetArray	 myFrontPrims;
    UT_IntArray		 myCompFrontFirstPrim;

    GA_OffsetArray	 myFrontBdVtxs;
    UT_IntArray		 myFrontBdFirstVtx;
    UT_IntArray		 myCompFrontFirstBd;
    UT_IntArray		 myBdComp;

    TextureMode		 mySideTextureMode = GU_PolyBridge::INTERPOLATE;
    TextureScaling	 mySideTextureScaling = GU_PolyBridge::FIT_UNIT_SQUARE;

    IndexPairArray	 myBridgePairing;
    const GEO_DetachedHedgeInterface 	*myHip;
};

#endif
