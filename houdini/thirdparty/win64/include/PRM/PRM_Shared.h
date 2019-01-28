/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Shared.h (PRM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __PRM_Shared__
#define __PRM_Shared__

#include "PRM_API.h"

// define these globals in the PRM library as a convenience.
// Defining global versions will save memory while enhancing the
// convenience to the user. It also means better standardization
// of parameter names.

// These defines go with the prmGroupTypeNames[], please make sure they are
// in sync.
enum PRM_GroupType
{
    PRM_GROUPTYPE_GUESS         = 0,
    PRM_GROUPTYPE_BREAKPOINTS   = 1,
    PRM_GROUPTYPE_EDGES         = 2,
    PRM_GROUPTYPE_POINTS        = 3,
    PRM_GROUPTYPE_PRIMS         = 4
};

// These defines go with the prmUVGroupTypeNames[], please make sure they are
// in sync.
enum PRM_UVGroupType
{
    PRM_UVGROUPTYPE_GUESS	= 0,
    PRM_UVGROUPTYPE_VERTICES	= 1,
    PRM_UVGROUPTYPE_EDGES	= 2,
    PRM_UVGROUPTYPE_POINTS	= 3,
    PRM_UVGROUPTYPE_PRIMS	= 4
};

class PRM_Name;
class PRM_Range;
class PRM_Default;
class PRM_ChoiceList;

// Common names
PRM_API extern PRM_Name	PRMlmbName;
PRM_API extern PRM_Name	PRMmmbName;
PRM_API extern PRM_Name	PRMrmbName;
PRM_API extern PRM_Name	PRMampName;
PRM_API extern PRM_Name	PRMaffectorName;
PRM_API extern PRM_Name	PRMcapsName;
PRM_API extern PRM_Name	PRMcenterName;
PRM_API extern PRM_Name	PRMconnectivityName;
PRM_API extern PRM_Name	PRMcoordName;
PRM_API extern PRM_Name	PRMcycleName;
PRM_API extern PRM_Name	PRMdiffuseName;
PRM_API extern PRM_Name	PRMdirectionName;
PRM_API extern PRM_Name	PRMdivName;
PRM_API extern PRM_Name	PRMentityName;
PRM_API extern PRM_Name	PRMfeatherName;
PRM_API extern PRM_Name	PRMframeName;
PRM_API extern PRM_Name	PRMfreqName;
PRM_API extern PRM_Name	PRMfullscaleName;
PRM_API extern PRM_Name	PRMuscaleName;
PRM_API extern PRM_Name	PRMgroupName;
PRM_API extern PRM_Name PRMgroupTypeName;
PRM_API extern PRM_Name PRMvexPrecisionName;
PRM_API extern PRM_Name	PRMhandFollowName;
PRM_API extern PRM_Name	PRMhandGlobalXformName;
PRM_API extern PRM_Name	PRMhandPivotName;
PRM_API extern PRM_Name	PRMhandRotName;
PRM_API extern PRM_Name	PRMhandAlignName;
PRM_API extern PRM_Name	PRMangleStepName;
PRM_API extern PRM_Name	PRMangleStepDisplayName;
PRM_API extern PRM_Name	PRMscaleStepName;
PRM_API extern PRM_Name	PRMxlateStepName;
PRM_API extern PRM_Name	PRMmaterialName;
PRM_API extern PRM_Name	PRMmaxAngleName;
PRM_API extern PRM_Name	PRMminAngleName;
PRM_API extern PRM_Name	PRMnmlName;
PRM_API extern PRM_Name	PRMnoiseTypeName;
PRM_API extern PRM_Name	PRMoffsetName;
PRM_API extern PRM_Name	PRMobjectName;
PRM_API extern PRM_Name	PRMorientName;
PRM_API extern PRM_Name	PRMpivotXformParmGroupName;
PRM_API extern PRM_Name	PRMpivotName;
PRM_API extern PRM_Name	PRMpivotXlateLabelName;
PRM_API extern PRM_Name	PRMpivotRotName;
PRM_API extern PRM_Name	PRMposName;
PRM_API extern PRM_Name	PRMradiusName;
PRM_API extern PRM_Name	PRMrollName;		// Rolloff
PRM_API extern PRM_Name	PRMrotName;
PRM_API extern PRM_Name	PRMscaleName;
PRM_API extern PRM_Name	PRMshearName;
PRM_API extern PRM_Name	PRMseedName;
PRM_API extern PRM_Name	PRMsizeName;
PRM_API extern PRM_Name	PRMsopName;
PRM_API extern PRM_Name	PRMsurfaceName;		// Rows,cols,etc Surface Type
PRM_API extern PRM_Name PRMswitcherName;        // dummy used for group switches
PRM_API extern PRM_Name	PRMtextureMapName;
PRM_API extern PRM_Name	PRMtrsName;
PRM_API extern PRM_Name	PRMturbName;
PRM_API extern PRM_Name	PRMuorderName;
PRM_API extern PRM_Name	PRMvorderName;
PRM_API extern PRM_Name	PRMorderName;
PRM_API extern PRM_Name	PRMimperfectName;
PRM_API extern PRM_Name	PRMwidthName;
PRM_API extern PRM_Name	PRMxlateName;
PRM_API extern PRM_Name	PRMxyzName;
PRM_API extern PRM_Name	PRMweightName;
PRM_API extern PRM_Name	PRMkernelName;
PRM_API extern PRM_Name	PRMprimTypeName;
PRM_API extern PRM_Name	PRMangleName;
PRM_API extern PRM_Name	PRMbeginangleName;
PRM_API extern PRM_Name	PRMendangleName;
PRM_API extern PRM_Name	PRMprimitiveName;
PRM_API extern PRM_Name	PRMpolygonName;
PRM_API extern PRM_Name	PRMmeshName;
PRM_API extern PRM_Name	PRMnurbsName;
PRM_API extern PRM_Name	PRMbezierName;
PRM_API extern PRM_Name	PRMprimitiveName;
PRM_API extern PRM_Name	PRMrowsName;
PRM_API extern PRM_Name	PRMcolsName;
PRM_API extern PRM_Name	PRMrowcolName;
PRM_API extern PRM_Name	PRMupoleName;
PRM_API extern PRM_Name	PRMarcAngleName;
PRM_API extern PRM_Name	PRMarcTypeName;
PRM_API extern PRM_Name	PRMclosedName;
PRM_API extern PRM_Name	PRMopenArcName;
PRM_API extern PRM_Name	PRMclosedArcName;
PRM_API extern PRM_Name	PRMslicedArcName;
PRM_API extern PRM_Name	PRMheightName;
PRM_API extern PRM_Name	PRMtopRadName;
PRM_API extern PRM_Name	PRMbottomRadName;
PRM_API extern PRM_Name	PRMtopBotRadName;
PRM_API extern PRM_Name	PRMxyExpName;
PRM_API extern PRM_Name	PRMzExpName;
PRM_API extern PRM_Name	PRMwrapuName;
PRM_API extern PRM_Name	PRMkeepShapeName;
PRM_API extern PRM_Name	PRMinterpuName;
PRM_API extern PRM_Name	PRMwrapvName;
PRM_API extern PRM_Name	PRMinterpvName;
PRM_API extern PRM_Name	PRMforceName;
PRM_API extern PRM_Name	PRMkeepPrimsName;
PRM_API extern PRM_Name	PRMfirstuName;
PRM_API extern PRM_Name	PRMlastuName;
PRM_API extern PRM_Name	PRMfirstvName;
PRM_API extern PRM_Name	PRMlastvName;
PRM_API extern PRM_Name	PRMjoinOpName;
PRM_API extern PRM_Name	PRMblendName;
PRM_API extern PRM_Name	PRMbiasName;
PRM_API extern PRM_Name	PRMtoleranceName;
PRM_API extern PRM_Name	PRMfitToleranceName;
PRM_API extern PRM_Name	PRMrayToleranceName;
PRM_API extern PRM_Name PRMcollinearToleranceName;
PRM_API extern PRM_Name	PRMmultiplicityName;
PRM_API extern PRM_Name	PRMproximityName;

PRM_API extern PRM_Name	PRMvlengthName;
PRM_API extern PRM_Name	PRMleadnmlName;
PRM_API extern PRM_Name	PRMupdatenmlsName;
PRM_API extern PRM_Name	PRMupdatenmlstName;
PRM_API extern PRM_Name	PRMupdatenmlspName;
PRM_API extern PRM_Name	PRMaffectednmlsName;

PRM_API extern PRM_Name	PRMRowsName;
PRM_API extern PRM_Name	PRMColumnsName;
PRM_API extern PRM_Name	PRMRowsAndColumns;
PRM_API extern PRM_Name	PRMTrianglesName;
PRM_API extern PRM_Name	PRMQuadrilateralsName;
PRM_API extern PRM_Name	PRMAlternatingTrianglesName;
PRM_API extern PRM_Name	PRMReverseTrianglesName;

PRM_API extern PRM_Name	PRMNoCapName;
PRM_API extern PRM_Name	PRMCapFacetedName;
PRM_API extern PRM_Name	PRMCapSharedName;
PRM_API extern PRM_Name	PRMCapRoundedName;
PRM_API extern PRM_Name	PRMCapTangentialName;

PRM_API extern PRM_Name	PRMstitchOpName;	// stitching
PRM_API extern PRM_Name	PRMdostitchName;
PRM_API extern PRM_Name	PRMdotangentName;
PRM_API extern PRM_Name	PRMsharpName;
PRM_API extern PRM_Name	PRMfixedName;

PRM_API extern PRM_Name	PRMspanDivsName;
PRM_API extern PRM_Name	PRMuvGapName;
PRM_API extern PRM_Name	PRMsharpCornerName;
PRM_API extern PRM_Name	PRMmapTypeName;
PRM_API extern PRM_Name	PRMuRangeName;
PRM_API extern PRM_Name	PRMvRangeName;
PRM_API extern PRM_Name	PRMuniformName;
PRM_API extern PRM_Name	PRMchordLenName;
PRM_API extern PRM_Name	PRMarcLenName;

PRM_API extern PRM_Name	PRMaccurateName;
PRM_API extern PRM_Name	PRMkeepInName;
PRM_API extern PRM_Name	PRMkeepOutName;
PRM_API extern PRM_Name	PRMextractPtName;
PRM_API extern PRM_Name	PRMkeepOrigName;

PRM_API extern PRM_Name	PRMuName;
PRM_API extern PRM_Name	PRMvName;
PRM_API extern PRM_Name	PRMuWarpName;
PRM_API extern PRM_Name	PRMvWarpName;
PRM_API extern PRM_Name	PRMuUseBiasName;
PRM_API extern PRM_Name	PRMvUseBiasName;
PRM_API extern PRM_Name	PRMuBiasName;
PRM_API extern PRM_Name	PRMvBiasName;
PRM_API extern PRM_Name	PRMuSharpName;
PRM_API extern PRM_Name	PRMvSharpName;
PRM_API extern PRM_Name	PRMdistName;
PRM_API extern PRM_Name	PRMuvSnapName;

PRM_API extern PRM_Name	PRMleftuvName;
PRM_API extern PRM_Name	PRMrightuvName;
PRM_API extern PRM_Name	PRMrightuvendName;
PRM_API extern PRM_Name	PRMlrwidthName;
PRM_API extern PRM_Name	PRMlrscaleName;
PRM_API extern PRM_Name	PRMlroffsetName;
PRM_API extern PRM_Name	PRMloopName;

PRM_API extern PRM_Name	PRMfuCapDivsName;
PRM_API extern PRM_Name	PRMluCapDivsName;
PRM_API extern PRM_Name	PRMfvCapDivsName;
PRM_API extern PRM_Name	PRMlvCapDivsName;
PRM_API extern PRM_Name	PRMfuCapScaleName;
PRM_API extern PRM_Name	PRMfvCapScaleName;
PRM_API extern PRM_Name	PRMluCapScaleName;
PRM_API extern PRM_Name	PRMlvCapScaleName;

PRM_API extern PRM_Name	PRMskinOpsName;
PRM_API extern PRM_Name	PRMincName;
PRM_API extern PRM_Name	PRMoutPolysName;
PRM_API extern PRM_Name	PRMgridSpaceName;
PRM_API extern PRM_Name	PRMgridCountName;
PRM_API extern PRM_Name	PRMgridRulerName;
PRM_API extern PRM_Name	PRMoriginName;
PRM_API extern PRM_Name	PRMnormalName;
PRM_API extern PRM_Name	PRMupVectorName;
PRM_API extern PRM_Name	PRMlockUpName;
PRM_API extern PRM_Name	PRMnextPointName;
PRM_API extern PRM_Name	PRMaddPointName;
PRM_API extern PRM_Name	PRMgroupNameName;
PRM_API extern PRM_Name	PRMgroupToSelName;
PRM_API extern PRM_Name	PRMselToGroupName;
PRM_API extern PRM_Name	PRMaddToSelName;
PRM_API extern PRM_Name	PRMsubFromSelName;
PRM_API extern PRM_Name	PRMxsectGroupName;
PRM_API extern PRM_Name	PRMaddToGrpName;
PRM_API extern PRM_Name	PRMsubFromGrpName;
PRM_API extern PRM_Name	PRMvertexName;

// Subnets
PRM_API extern PRM_Name	PRMinputLabelNames[];

// Curve
PRM_API extern PRM_Name PRMparamName;
PRM_API extern PRM_Name PRMsmoothName;
PRM_API extern PRM_Name PRMnormalizeBasisName;
PRM_API extern PRM_Name	PRMsharpConnName;

// Create groups names
PRM_API extern PRM_Name	PRMgrpGeometryTypeName;
PRM_API extern PRM_Name	PRMgrpOperationName;
PRM_API extern PRM_Name	PRMgrpPatternName;
PRM_API extern PRM_Name	PRMgrpStartEndName;
PRM_API extern PRM_Name	PRMgrpEveryOfName;
PRM_API extern PRM_Name	PRMgrpBoundTypeName;
PRM_API extern PRM_Name	PRMgrpSpreadAngleName;
PRM_API extern PRM_Name	PRMgrpEdgeDepthName;
PRM_API extern PRM_Name	PRMgrpEdgeAngleName;
PRM_API extern PRM_Name	PRMgrpEdgeMinAngleName;
PRM_API extern PRM_Name	PRMgrpEdgeMaxAngleName;
PRM_API extern PRM_Name PRMgrpEdgeAngleBetweenEdgesName;
PRM_API extern PRM_Name PRMgrpEdgeMinLenName;
PRM_API extern PRM_Name PRMgrpEdgeMaxLenName;
PRM_API extern PRM_Name	PRMgrpPointNumberName;
PRM_API extern PRM_Name	PRMgrpUnsharedName;

PRM_API extern PRM_Name	PRMgrpPrefixName;
PRM_API extern PRM_Name	PRMchanPrefixName;
PRM_API extern PRM_Name	PRMgrpOutputName;

PRM_API extern PRM_Name	PRMaxisName;
PRM_API extern PRM_Name	PRMvectorName;

// Material color names
PRM_API extern PRM_Name	PRMmatAmbName;
PRM_API extern PRM_Name	PRMmatDiffName;
PRM_API extern PRM_Name	PRMmatSpecName;
PRM_API extern PRM_Name	PRMmatReflName;
PRM_API extern PRM_Name	PRMmatTransName;
PRM_API extern PRM_Name	PRMmatEmitName;

PRM_API extern PRM_Name	PRMmatRoughName;
PRM_API extern PRM_Name	PRMmatRefractName;
PRM_API extern PRM_Name	PRMmatFresnelName;
PRM_API extern PRM_Name	PRMmatAparaName;
PRM_API extern PRM_Name	PRMmatAperpName;
PRM_API extern PRM_Name	PRMmatArollName;
PRM_API extern PRM_Name	PRMmatShadowName;
PRM_API extern PRM_Name	PRMmatBiasName;

// Extruding
PRM_API extern PRM_Name	PRMinsetName;
PRM_API extern PRM_Name	PRMfrontName;
PRM_API extern PRM_Name	PRMbackName;
PRM_API extern PRM_Name	PRMsideName;
PRM_API extern PRM_Name	PRMdoFuseName;
PRM_API extern PRM_Name	PRMshareFacesName;
PRM_API extern PRM_Name PRMremoveSharedSidesName;
PRM_API extern PRM_Name	PRMconvMeshName;
PRM_API extern PRM_Name	PRMorientPolysName;

// Fix Selected Node Names
PRM_API extern PRM_Name	PRMprefixName;
PRM_API extern PRM_Name	PRMsuffixName;
PRM_API extern PRM_Name	PRMremovePrefixName;
PRM_API extern PRM_Name	PRMprefixCharName;
PRM_API extern PRM_Name	PRMremoveSuffixName;
PRM_API extern PRM_Name	PRMsuffixCharName;

// Particle Render Attributes
PRM_API extern PRM_Name		PRMrpartType;
PRM_API extern PRM_Name		PRMrpartSize;
PRM_API extern PRM_Name		PRMrpartBlurTime;
PRM_API extern PRM_Name		PRMrpartSphereNorm;
PRM_API extern PRM_Default	PRMrpartTypeDefault;
PRM_API extern PRM_Default	PRMrpartSizeDefault;
PRM_API extern PRM_ChoiceList	PRMrpartTypeMenu;

// Soft Selection Names
PRM_API extern PRM_Name		PRMsoftRadiusName;
PRM_API extern PRM_Name		PRMmetricName;
PRM_API extern PRM_Name		PRMsoftTypeName;
PRM_API extern PRM_Name         PRMsoftDistanceMetricsName;
PRM_API extern PRM_Name		PRMsoftIgnorePointConnectivityName;
PRM_API extern PRM_Name		PRMsoftTangentAngleName;
PRM_API extern PRM_Name		PRMsoftVisualizeFalloffName;
PRM_API extern PRM_ChoiceList	PRMsoftVisualizeFalloffMenu;
PRM_API extern PRM_Name         PRMsoftApplyRolloffName;
PRM_API extern PRM_Name         PRMsoftDistanceAttributeName;
PRM_API extern PRM_Name         PRMsoftLeadPointAttributeName;

// Pre-Transform Names
PRM_API extern PRM_Name		PRMprexformParmGroupName;
PRM_API extern PRM_Name		PRMprexformTRSName;
PRM_API extern PRM_Name		PRMprexformXYZName;
PRM_API extern PRM_Name		PRMprexformXlateName;
PRM_API extern PRM_Name		PRMprexformRotName;
PRM_API extern PRM_Name		PRMprexformScaleName;
PRM_API extern PRM_Name		PRMprexformShearName;


// This enum goes with PRMsoftVisualizeFalloffMenu, please make sure it is in
// sync.
enum PRM_SoftVisualizeType
{
    PRM_SOFT_VISUALIZE_NEVER = 0,
    PRM_SOFT_VISUALIZE_ALWAYS = 1,
    PRM_SOFT_VISUALIZE_STATE_ACTIVE = 2
};

// Mirroring
PRM_API extern PRM_Name		PRMmirrorPlaneName;
PRM_API extern PRM_Name		PRMmirrorGroupUseName;
PRM_API extern PRM_ChoiceList	PRMmirrorGroupUseMenu;

// Common defaults
PRM_API extern PRM_Default	PRMposDefaults[];
PRM_API extern PRM_Default	PRMxaxisDefaults[];	// 1,0,0
PRM_API extern PRM_Default	PRMyaxisDefaults[];	// 0,1,0
PRM_API extern PRM_Default	PRMzaxisDefaults[];	// 0,0,1
PRM_API extern PRM_Default	PRMnegoneDefaults[];	// -1
PRM_API extern PRM_Default	PRMpointOneDefaults[];	// 0.1
PRM_API extern PRM_Default	PRMpointFiveDefaults[];	// 0.5
PRM_API extern PRM_Default	PRMzeroDefaults[];	// 0
PRM_API extern PRM_Default	PRMoneDefaults[];	// 1
PRM_API extern PRM_Default	PRMtwoDefaults[];	// 2
PRM_API extern PRM_Default	PRMthreeDefaults[];
PRM_API extern PRM_Default	PRMfourDefaults[];
PRM_API extern PRM_Default	PRMfiveDefaults[];
PRM_API extern PRM_Default	PRMsixDefaults[];
PRM_API extern PRM_Default	PRMsevenDefaults[];
PRM_API extern PRM_Default	PRMeightDefaults[];
PRM_API extern PRM_Default	PRMnineDefaults[];
PRM_API extern PRM_Default	PRMtenDefaults[];	// 10
PRM_API extern PRM_Default	PRM20Defaults[];	// 20
PRM_API extern PRM_Default	PRM45Defaults[];	// 45
PRM_API extern PRM_Default	PRM90Defaults[];	// 90
PRM_API extern PRM_Default	PRM100Defaults[];	// 100
PRM_API extern PRM_Default	PRM180Defaults[];	// 180
PRM_API extern PRM_Default	PRM360Defaults[];	// 360
PRM_API extern PRM_Default	PRMangleDefaults[];	// 0, 360
PRM_API extern PRM_Default	PRMhalfAngleDefaults[];	// 0, 180
PRM_API extern PRM_Default	PRM360over8Default;	// 360/8
PRM_API extern PRM_Default	PRMsurfaceDefault;	// For the surface menu
PRM_API extern PRM_Default	PRMconnectivityDefault; // Connectivity menu
PRM_API extern PRM_Default	PRMtextureMapDefault;	// Default texture map name
PRM_API extern PRM_Default	PRMbumpMapDefault;	// Default bump map name
PRM_API extern PRM_Default	PRMgroupDefault;	// Default new group name
PRM_API extern PRM_Default	PRMvexPrecisionDefault;	// Default vex precision
PRM_API extern PRM_Default      PRMrenameDefault;	// Default rename group name
PRM_API extern PRM_Default      PRMbiasDefault;		// 0.5
PRM_API extern PRM_Default      PRMtoleranceDefault;	// 1
PRM_API extern PRM_Default      PRMfitToleranceDefault;	// 0.001
PRM_API extern PRM_Default      PRMrayToleranceDefault;	// 0.01
PRM_API extern PRM_Default      PRMsharpnessDefault;	// 0
PRM_API extern PRM_Default	PRMuvRangeDefaults[];	// 0.1, 0.9
PRM_API extern PRM_Default	PRMluvDefaults[];	// 1, 0
PRM_API extern PRM_Default	PRMruvDefaults[];	// 0, 0
PRM_API extern PRM_Default	PRMradiusDefault;	// 0.5
PRM_API extern PRM_Default	PRMsoftVisualizeFalloffDefault;
PRM_API extern PRM_Default	PRMmatrix2Defaults[];	// 4 floats (identity)
PRM_API extern PRM_Default	PRMmatrix3Defaults[];	// 9 floats (identity)
PRM_API extern PRM_Default	PRMmatrix4Defaults[];	// 16 floats (identity)

// Subnets
PRM_API extern PRM_Default	PRMinputLabelDefaults[];// input label defaults

// Common choices
PRM_API extern PRM_ChoiceList	PRMtoggleMenu;	// on or off
PRM_API extern PRM_ChoiceList	PRMentityMenuPointsAndPrimitives;	// primitives or points
PRM_API extern PRM_ChoiceList	PRMentityMenuAll;   // all supported groups: primitives, points, and edges
PRM_API extern PRM_ChoiceList	PRMgroupTypeMenu;      // supported groups
PRM_API extern PRM_ChoiceList	PRMUVgroupTypeMenu;      // supported groups
PRM_API extern PRM_ChoiceList	PRMvexPrecisionMenu;      // supported precision
PRM_API extern PRM_ChoiceList	PRMplaneMenu;	// XY, YZ, ZX
PRM_API extern PRM_ChoiceList	PRMaxisMenu;	// X axis, Y axis, Z axis
PRM_API extern PRM_ChoiceList	PRMtrsMenu;	// Translate rotate scale
PRM_API extern PRM_ChoiceList	PRMxyzMenu;	// Rotate X Y Z
PRM_API extern PRM_ChoiceList	PRMrgbalMenu;	// rgb, r, g, b, a, luminance
PRM_API extern PRM_ChoiceList	PRMrgbal2Menu;	// r, g, b, a, luminance
PRM_API extern PRM_ChoiceList	PRMrgbhsvMenu;	// r,g,b, hue, saturation, value
PRM_API extern PRM_ChoiceList	PRMrgbaMenu;	// red, green, blue, alpha
PRM_API extern PRM_ChoiceList	PRMbumpMenu;	// red, green, blue, alpha
PRM_API extern PRM_ChoiceList	PRMnoiseMenu;	// hermite interpolation, sparse convolution
PRM_API extern PRM_ChoiceList	PRMintegrateMenu; // integration type
PRM_API extern PRM_ChoiceList	PRMprimitiveMenu; // prim, poly, mesh, nurb, bez
PRM_API extern PRM_ChoiceList	PRMsurfaceMenu;	  // row, col, rowcol, tri, quad, alt.tri
PRM_API extern PRM_ChoiceList	PRMconnectivityMenu; // row, col, rowcol, tri, quad, alt.tri, inherit
PRM_API extern PRM_ChoiceList	PRMarcTypeMenu;	// closed, open arc, closed arc, sliced arc
PRM_API extern PRM_ChoiceList	PRMcycleMenu;	// how to sweep cross sections (all, each)
PRM_API extern PRM_ChoiceList	PRMcapMenu;	// none, faceted, shared, rounded, tangential
PRM_API extern PRM_ChoiceList   PRMuWrapMenu;	// no wrap, wrap, if 1st xsect
PRM_API extern PRM_ChoiceList   PRMvWrapMenu;	// no wrap, wrap, if 1st xsect
PRM_API extern PRM_ChoiceList   PRMuInterpMenu;	// no interp, interp, if 1st
PRM_API extern PRM_ChoiceList   PRMvInterpMenu;	// no interp, interp, if 1st
PRM_API extern PRM_ChoiceList	PRMlockUpVectMenu; // to nml, wrldx, y, z, none
PRM_API extern PRM_ChoiceList	PRMdirUVMenu;	   // in U or V
PRM_API extern PRM_ChoiceList	PRMprimOpsMenu;		// all/groups/skip
PRM_API extern PRM_ChoiceList	PRMpointOpsMenu;	// all/groups/skip/sep
PRM_API extern PRM_ChoiceList	PRMparamMenu;	// Curve Parameterizations
PRM_API extern PRM_ChoiceList	PRMsoftTypesMenu; // soft fallout functions
PRM_API extern PRM_ChoiceList	PRMsoftDistanceMetricsMenu; // soft distance measurement
PRM_API extern PRM_ChoiceList	PRMmetricMenu; // Metric (for UV sops)
PRM_API extern PRM_ChoiceList	PRMUVLayerMenu; // UV Layer
PRM_API extern PRM_ChoiceList	PRMopFilterTypeMenu;	// Filters (ie. !!OBJ!!)
PRM_API extern PRM_ChoiceList	PRMviewportLODMenu;	// GEO_ViewportLOD
PRM_API extern PRM_ChoiceList	PRMviewportLODMenuWithUnchanged;

PRM_API extern PRM_ChoiceList	PRMkeepPosMenu; // Keep Pos Toggle Menu
PRM_API extern PRM_ChoiceList	PRMchildCompMenu; // Keep Pos Toggle Menu

// Handle alignment
PRM_API extern PRM_ChoiceList	PRMhandleAlignMenu;
// Handle angle step
PRM_API extern PRM_ChoiceList	PRMhandleAngleStepMenu;

// Common Ranges
// If there is a [] around the range it is a UI only range.  If not, it
// is a restricted range.
PRM_API extern PRM_Range 	PRMangleRange;		// [0] to [360]
PRM_API extern PRM_Range 	PRMhalfAngleRange;	// 0 to 180
PRM_API extern PRM_Range	PRMnonNegativeRange;	// 0 to [100]
PRM_API extern PRM_Range	PRMpositiveRange;	// 0.0001 to [1000]
PRM_API extern PRM_Range	PRMnonPositiveRange;	// [-100] to 0
PRM_API extern PRM_Range	PRMdivision3Range;	// 3 to [50]
PRM_API extern PRM_Range	PRMdivision2Range;	// 2 to [50]
PRM_API extern PRM_Range	PRMdivision1Range;	// 1 to [50]
PRM_API extern PRM_Range	PRMdivision0Range;	// 0 to [50]
PRM_API extern PRM_Range	PRMdivisionRange;	// 1 to [100]
PRM_API extern PRM_Range	PRMexpRange;		// 0 to [5]
PRM_API extern PRM_Range	PRMframeRange;		// [1] to [300]
PRM_API extern PRM_Range	PRMrulerRange;		// 0 to [10]
PRM_API extern PRM_Range	PRMlodRange;		// 0.001 to [5]
PRM_API extern PRM_Range	PRMrolloffRange;	// 0 to [10]
PRM_API extern PRM_Range	PRMseedRange;		// [0] to [200]
PRM_API extern PRM_Range	PRMscaleRange;		// [-5] to [5]
PRM_API extern PRM_Range	PRMuscaleRange;		// [0] to [10]
PRM_API extern PRM_Range	PRMstepSizeRange;	// 0.001 to [5]
PRM_API extern PRM_Range	PRMdistRange;		// [-10] to [10]
PRM_API extern PRM_Range	PRMorderRange;		// 2 to 11
PRM_API extern PRM_Range	PRMfrequency10Range;	// 1 to [10]
PRM_API extern PRM_Range	PRMfrequencyRange;	// 1 to [5]
PRM_API extern PRM_Range	PRMmetaWeightRange;	// [-5] to [10]
PRM_API extern PRM_Range	PRMbiasRange;		// 0 to 1
PRM_API extern PRM_Range	PRMtoleranceRange;	// 0 to [1]
PRM_API extern PRM_Range	PRMsharpnessRange;	// 0 to 1
PRM_API extern PRM_Range	PRMunitRange;		// 0 to 1
PRM_API extern PRM_Range	PRMincRange;		// 1 to [20]
PRM_API extern PRM_Range	PRMradiusRange;		// 0 to [1]
PRM_API extern PRM_Range 	PRMsofttoggleRange;	// [0] to [1]
PRM_API extern PRM_Range 	PRMfullunitRange;	// -1 to 1
PRM_API extern PRM_Range 	PRMfullunituiRange;	// [-1] to [1]
PRM_API extern PRM_Range 	PRMfullangleRange;	// [-360] to [360]
#endif

