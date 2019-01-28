/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_Error.h (C++)
 *
 * COMMENTS:
 *		This class is used to store SOP errors or warnings.
 *
 */

#ifndef __SOP_Error__
#define __SOP_Error__

enum SOP_ErrorCodes
{
    //
    //  Cooking errors

    SOP_ERR_INVALID_SRC = 0,    // No source or source has error

    //
    //  File errors
    SOP_ERR_FILEGEO,            // File error on load/save geometry

    //
    //  Some common cooking errors

    SOP_ERR_BADGROUP,           // Invalid group specification
    SOP_ERR_BAD_POINTGROUP,
    SOP_MESSAGE,
    SOP_ERR_ILLEGAL_GROUPNAME,
    SOP_ERR_ILLEGAL_GROUPPREFIX,
    SOP_GROUP_DUPLICATE_NAME,	// SOP_Group error

    // Some messages for the Lsystem sop
    SOP_LSYSTEM_STR_FIRST,	// first line
    SOP_LSYSTEM_STR_OTHERS,	// other lines
    SOP_LSYSTEM_PIC_FILE,	// invalid raster file

    SOP_ERR_ADD_BAD_FACE,
    SOP_ERR_ADD_BAD_POINT,
    SOP_ERR_GRP_NONAME,		

    SOP_LOAD_UNKNOWN_ASCII_FLAG,	//Skipping unknown flag (%s)
    SOP_LOAD_UNKNOWN_BINARY_FLAG,	//Skipping unknown flag

    SOP_BAD_FLAG_FORMAT,		//expecting sopflags = ...

    SOP_NO_NORMALS,			//cannot compute normals

    SOP_BAD_DIVS,			//invalid division/segment size

    SOP_BAD_OBJECT_MERGED,		//Bad object in object_merge
    SOP_BAD_SOP_MERGED,			//Bad sop in object_merge

    SOP_SKELETON_VOLUMES,		// Number of volumes.
    SOP_SKELETON_POINTS,		// Number of points found.
    SOP_SKELETON_POINTS_INVALID,	// Deformation point count changed
    SOP_SKELETON_VOLUMES_INVALID,	// Skeleton circle count changed
    SOP_SKELETON_HIERARCHY_INVALID,	// Ellipse hierarchy changed

    SOP_DEL_DEGENERATE,			// Deletion causes degenerate prims

    SOP_UNMATCH_SOURCE,			// two source have unmatch geometry

    SOP_BASIS_INVALID,			// non-spline type	#26
    SOP_BASIS_SELECTION,		// select one of U or V
    SOP_BASIS_INDEX,			// index exceeds primitives
    SOP_BASIS_BASIS,			// invalid basis
    SOP_BASIS_INCOMPATIBLE,		// incompatible bases, can't copy

    SOP_SWEEP_REF_NPTS, 		// reference path and backbone
    					// does not have same # of points
    SOP_SWEEP_REF_NPRIMS,		// reference path and backbone
    					// does not have same # of primitives

    SOP_ATTRIBUTE_INVALID,		// Invalid attribute name

    SOP_DEFORM_NO_CAPT_ATR,		// no capture attrib. for deformation
    SOP_DEFORM_INVALID_REGION,		// can't find a captured region
    SOP_DEFORM_NUM_REGIONS,		// number of regions found
    SOP_DEFORM_MAX_REGIONS_PERPOINT,	// max regions assigned per point

    SOP_CURVECLAY_NO_PROFILE,		// no profiles on the surface

    SOP_TRIMGRAPH_NO_LOOP,		// no loop can be formed from graph.
    SOP_TRIMGRAPH_TWO_SIDES,		// need exactly two xrsections on side

    SOP_CAPTURE_INVALID_PARENT,		// invalid hierarchy
    SOP_CAPTURE_INVALID_REGION,		// invalid region bone hierarchy
    SOP_CAPTURE_NO_REGIONS,		// no capture regions found in hiearchy
    SOP_CAPTURE_NO_COLOR,		// capture region missing color attrib
    SOP_CAPTURE_SURF_ERR,		// error in weight from surface calc.
    SOP_CAPTURE_NUMPTS_CHANGE,		// num pts changed since capture frame
    SOP_CAPTURE_CAPTFRAME_ERR,		// couldn't cook geo at capt frame
    SOP_CAPTURE_LOAD_ERR,		// override file reading error
    SOP_CAPTURE_FILE_ERR,		// override file not found
    SOP_CAPTURE_NPTS_MSG,		// number of points captured message

    SOP_CHANNEL_INVALID_INPUT_CHOP,	// couldn't cook chop
    SOP_CHANNEL_MISSING_RBRACKET,	// missing )
    SOP_CHANNEL_INVALID_ATTRIBUTE,	// couldnt find geo attribute
    SOP_CHANNEL_INVALID_INDEX,		// index out of range for attribute
    SOP_CHANNEL_MISMATCHED_SCOPE,	// num chans and num attrs differ
    SOP_CHANNEL_INVALID_CHANNEL,	// couldnt find channel

    SOP_SOFTLOCK_PTNUM_CHANGE,		// num of points changed since softlock
    SOP_SOFTLOCK_PROMOTED,		// softlock promoted to a hardlock

    SOP_STAMP_TOKEN_IN_USE,		// stamp parameter already used

    SOP_POLYPATCH_TOO_FEW_ROWCOL,	// need more rows, columns or clamping

    SOP_CARVE_INVALID_RANGE,		// need full ranges

    SOP_VEX_ERROR,			// sop vex error

    SOP_SYNTAX_ERROR,			// error parsing points list
    SOP_CURVE_ERROR,			// curve couldn't be drawn
    SOP_POINT_DNE,			// referenced point doesn't exist
    SOP_NEED_MORE_POINTS,		// not enough points specified

    SOP_CAPTURE_NO_CAPTFRAME,		// missing pCaptFrame attribute

    SOP_ERR_BADNODE,			// Node not found or wrong type.

    SOP_ERR_SELFMERGE,			// Cannot use oneself as the source 
					// of an object merge.
    
    SOP_ERR_SURFACEGROUPRANGE,		// Refernce to a non-existent surface 
					// group.

    SOP_ERR_CENTERMISMATCH,		// Mismatch on center points.

    SOP_ERR_CENTERGROUPMISSING,		// Missing center groups.

    SOP_ERR_MISMATCHBACKQUOTE,		// Mismatch backquote in rule %s.

    // SOP_WireDeform errors
    SOP_ERR_MISSING_RADIUS,		// Missing radiusf primitive attribute
    SOP_ERR_RESTDEFORM_MISMATCH,	// Rest and deform geometry mismatch
    SOP_WARN_MISSING_WCAPT_PRIMU,	// Missing pCaptWirePrimU attribute
    SOP_ERR_MISSING_WCAPT,		// Missing pCaptWirePrimU attribute
    SOP_ERR_MISSING_WCAPT_U,		// Missing pCaptWireU attribute
    SOP_ERR_MISSING_WCAPT_V,		// Missing pCaptWireV attribute
    SOP_ERR_MISMATCH_WCAPT_U,		// Mismatched pCaptWireU attribute
    SOP_ERR_MISMATCH_WCAPT_V,		// Mismatched pCaptWireU attribute
    SOP_ERR_MISSING_PRIMITIVE,		// Missing primitive reference %s
    SOP_ERR_NO_DEFORM_EFFECT,		// No deformation.

    // SOP_WireCapture errors
    SOP_WARN_NO_PTS_CAPTURED,		// Warning: No points captured.

    SOP_ERR_INVALID_CREGION,		// Error: Invalid cregion %s

    SOP_ERR_MISMATCH_VERTEX,	// Primitives have mismatching vertex counts
    SOP_ERR_MISMATCH_POINT,	// Reference and Target have different point
    SOP_ERR_MISMATCH_PRIMITIVE,	// Reference and Target have different prim
    SOP_ERR_MISMATCH_POINTLIST,	// Target and Ref point lists must be same len
    SOP_ERR_MISMATCH_CONNECT,	// Connected piece mismatch
    SOP_ERR_CONNECT_FAIL,	// A connected piece didn't match.
    SOP_ERR_MATCH_DUPLICATE,	// Ambiguous matching.

    SOP_ERR_INVALID_ATTRIBUTE_NAME,	// Invalid attribute name %s.	
    SOP_ERR_MISSING_UV,		// Missing uv vertex attribute.

    SOP_UVPELT_PRIMITIVE_INDEX,		// index exceeds primitives
    SOP_UVPELT_BOUNDARY_POINTS,		// no boundary points found

    // SOP_Facet errors
    SOP_WARN_PLANAR_INCOMPLETE,	// Unable to make primitive planar

    SOP_ERR_NOT_METABALL,	// Input %s does not contain metaballs
    SOP_ERR_MISSING_MCAPT,	// Input %s is missing metaCapt point attribute
    SOP_ERR_MISSING_MCAPTFRAME, // Input %s is missing metaCaptFrame attribute
    SOP_ERR_MISSING_MCAPTGROUPS,// Input %s is missing metaCaptGroups attribute
    SOP_WARN_MISSING_COLOR,	// Input %s is missing pt/primitive Cd attribute
    SOP_WARN_CAPTOBJ_INVALID,	// Capture Object %s is invalid or won't cook

    SOP_ERR_MISSING_MCAPTDATA,	// Input %s is missing metaCaptData attribute
    SOP_ERR_MISSING_MCAPTPATHS, // Input %s is missing metaCaptPaths attribute
    SOP_ERR_MISSING_MCAPTGROUPMAP, // Input %s misisng metaCaptGroupMap attr

    SOP_ERR_PRIM_NUMS_DIFFERENT, // Inputs contain different numbers of primitives.
    SOP_ERR_NOT_OPEN_CURVE,	// Primitives must be open curves.

    // Muscle sliding errors for Capture/Deform meta
    SOP_ERR_MISSING_ANCHORXFORMS,
    SOP_ERR_INVALID_ANCHORPATH,

    SOP_ERR_RENAME,		// Cannot rename group '%s'
    SOP_ERR_RENAME_DUPLICATE,	// Name conflict renaming '%s' to '%s'
    SOP_ERR_CONVERT_DUPLICATE,	// Name conflict converting '%s' to '%s'
    SOP_ERR_UNMATCHED_GROUP_PATTERN, // No groups match pattern '%s'

    // DeformMuscle errors
    SOP_ERR_MISSING_INFLATENORMALS,
    SOP_ERR_MISSING_CENTERINPUT,
    SOP_ERR_METABALL_COUNT_CHANGED, // DeformMuscle and DeformMeta
    SOP_ERR_METABALLS_NO_GROUPS, // Capture Meta - metaballs aren't in groups.
    SOP_ERR_BAD_MUSCLE_NAME, // Invalid muscle name specification.
    SOP_ERR_CANNOT_REMOVE_REGION, // Error removing capture region %s.

    // Attribute transfer errors
    SOP_ERR_MISSMATCH_GROUP_TYPE,	// Specified input group is not a %s.

    // PointCloudIso error
    SOP_ERR_MISSING_POINT_NORMALS,	// Input SOP missing point normals

    // SOPZ_DopImport error
    SOP_ERR_OBJECT_NAME_CONFLICT,	// Duplicate source DOP object names.
    SOP_ERR_NO_DOP_OBJECTS,		// No DOP object names specified.

    SOP_ERR_MDD_MISMATCH_POINT, // MDD file and input have different # points
    SOP_ERR_FILEMDD,		// Please specify an MDD file
    SOP_ERR_MDDCORRUPT,		// MDD file "%s" may be corrupt
    SOP_ERR_MDD_EXTRA_DATA,	// Unknown extra data (%s) found in MDD file 

    SOP_ERR_CAPT_MISMATCH_POINT,    // Input and painted geometry don't have
				    //    matching point counts.

    SOP_ERR_METABALL_THRESHOLDMODE_BAD_WEIGHT, // Bad weight for threshold mode
    SOP_ERR_MANIFOLD_GEOMETRY, // Invalid Geometry

    SOP_ERR_INVALID_REGEX,	// Invalid regular expression in string edit
    SOP_ERR_CREATE_DIRECTORY_FAIL,	// Could not create directory %s

    SOP_ERR_MULTIPLE_OUTPUT,	// Multiple outputs with same index %s

    SOP_ERR_SCENE_STYLESHEET_ERROR,	// Error parsing a scene style sheet.
    SOP_ERR_OBJECT_STYLESHEET_ERROR,	// Error parsing an object style sheet.

    // SOP_PolySplit2 errors
    SOP_ERR_TOO_MANY_CURVES,
    SOP_ERR_WRONG_CURVE_TYPE,
    SOP_ERR_SPLITTING_DISCONNECTED_GEOMETRY,
    SOP_ERR_EDGELOOP_NON_EDGE_POINT,
    SOP_ERR_POINT_ON_FACE,
    SOP_ERR_SPLITTING_NON_EXISTANT_EDGE,

    // SOP_PolyFill errors
    SOP_ERR_NO_UNSHARED_EDGES,
    SOP_WARN_NON_MANIFOLD,
    SOP_WARN_ODD_EDGE_PATCH,
    SOP_ERR_POLYFILL_LOOP_DISTANCE_INVALID,
    SOP_WARN_NO_PLANE_NORMAL,
    SOP_WARN_ONLY_ONE_CORNER,
    SOP_WARN_CONFLICTING_CORNERS,

    // SOP_LidarImport errors
    SOP_ERR_MISSING_POSITION, 
    SOP_WARN_ATTRIBS_NOT_FOUND,
    SOP_ERR_LIDAR_READER_ERROR,
    SOP_WARN_PARMS_NOT_APPLICABLE,

    SOP_WARN_DEFORM_REGION_FAST_INCOMPATIBLE,// region isn't compatible with fast deform

    SOP_ERR_CHOP_NODE_NODE_FOUND,	// Error finding chop node

    SOP_NUM_ERROR_TYPES			// sentinel
};

#endif
