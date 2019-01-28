/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Direct manipulation library (C++)
 *
 * COMMENTS:
 *	Definitions for DM.
 *
 */

#ifndef __DM_Defines_H__
#define __DM_Defines_H__

#include <UT/UT_Array.h>
#include <GU/GU_SelectType.h>
#include <BM/BM_KeyBindings.h>
#include <RE/RE_Types.h>

//------------------------------ Drawing ----------------------------------

// Draw all the points or primitives, or just the selected:
#define DM_DRAW_SELECTED	0x01
#define DM_DRAW_NONSELECTED	0x02
#define DM_DRAW_ALL		(DM_DRAW_SELECTED | DM_DRAW_NONSELECTED)

//------------------------------ Picking ----------------------------------

#define DM_PICKED_NOTHING	0
#define DM_PICKED_POINT		1
#define DM_PICKED_EDGE		2
#define DM_PICKED_NORMAL	3
#define DM_PICKED_PRIMITIVE	4

//---------------------------- Picking radii ------------------------------

#define DM_VIEWPORT_MAX_PICK_DIST		RE_MAX_PICK_DIST 
#define DM_VIEWPORT_SNAP_NEAR_PICK_DIST		RE_MAX_PICK_DIST 
#define DM_VIEWPORT_HANDLE_PICK_DIST		5
#define DM_VIEWPORT_HANDLE_PICK_OVERLAP_DIST	5
#define DM_VIEWPORT_GEO_PICK_DIST		5
#define DM_VIEWPORT_GEO_PICK_OVERLAP_DIST	5
#define DM_VIEWPORT_SNAP_PICK_DIST		10
#define DM_VIEWPORT_SNAP_PICK_OVERLAP_DIST	10
#define DM_VIEWPORT_PEANUT_SNAP_PICK_DIST		10
#define DM_VIEWPORT_PEANUT_SNAP_PICK_OVERLAP_DIST	7

//---------------------------- Selection Styles ---------------------------

enum DM_SelectionStyle
{
    DM_STYLE_UNKNOWN = -1,
    DM_STYLE_BOX_PICK = 0,
    DM_STYLE_LASSO_PICK,
    DM_STYLE_BRUSH_PICK,
    DM_STYLE_LASER_PICK,
    DM_STYLE_NUM_ENTRIES    // sentinel
};

//---------------------------- Handle Pick Ids -------------------------------
#define DM_X_AXIS		'X'
#define DM_Y_AXIS		'Y'
#define DM_Z_AXIS		'Z'
#define DM_x_AXIS		'x'
#define DM_y_AXIS		'y'
#define DM_z_AXIS		'z'

//----------------------------- Miscellaneous --------------------------------

#define	DM_ROTATE_BY		45	// degrees to rotate by

enum {
    DM_OBJ_GEO 			= 0x00000001,
    DM_OBJ_CAM			= 0x00000002,
    DM_OBJ_LIGHT		= 0x00000004,
    DM_OBJ_BONE			= 0x00000008,
    DM_OBJ_NULL			= 0x00000010,
    DM_OBJ_BLEND		= 0x00000020,
    DM_OBJ_OTHER		= 0x00000040,
    DM_OBJ_ALL			= 0x0000007F
};

enum DM_SelectionFacing {
    DM_SEL_ALLOW_FRONT_AND_BACK = 0,
    DM_SEL_ALLOW_FRONT,
    DM_SEL_ALLOW_BACK,
    DM_SEL_ALLOW_NUM
};

enum {
    DM_SEL_CONNECTIVITY_VIEWPORT = 0,
    DM_SEL_CONNECTIVITY_3D,
    DM_SEL_CONNECTIVITY_UV,
    DM_SEL_CONNECTIVITY_NUM_ENTRIES
};

enum {
    DM_CLIPPLANE_TOP	= 0,
    DM_CLIPPLANE_BOTTOM,
    DM_CLIPPLANE_LEFT,
    DM_CLIPPLANE_RIGHT,
    DM_CLIPPLANE_FRONT,
    DM_CLIPPLANE_BACK,
    DM_NUM_CLIPPLANES
};

// This enum must be kept in sync with the selection_mode.val radio strip 
// in $FUSE/ViewExtra.ui!
enum DM_SelectMode
{
    DM_SELECTMODE_INVALID = -1,
    DM_SELECTMODE_OBJECT = 0,
    DM_SELECTMODE_GEOMETRY,
    DM_SELECTMODE_DYNAMICS,
    DM_SELECTMODE_SCENE_GRAPH,
    DM_NUM_SELECTMODES	// sentinel
};

enum DM_DopSelectMode
{
    DM_DOPSELECTMODE_DETAILS = 0,
    DM_DOPSELECTMODE_POINTS,
    DM_DOPSELECTMODE_POLYGONS,
    DM_NUM_DOPSELECTMODES	// sentinel
};

// This enum must be kept in sync with the inspect.mode.val radio strip 
// in $FUSE/ViewExtra.ui!
enum DM_InspectMode
{
    DM_INSPECTMODE_OBJECTS = 0,
    DM_INSPECTMODE_PRIMITIVES,
    DM_INSPECTMODE_PIXELS,
    DM_NUM_INSPECTMODES	// sentinel
};

// This enum must be kept in sync with the infoTextMode.menu menu strip 
// in DM_DisplayOption.ui!
enum DM_InfoTextMode
{
    DM_INFOTEXT_OFF = 0,
    DM_INFOTEXT_SELECTED_ONLY,
    DM_INFOTEXT_ALWAYS
};

enum DM_GeoSortMode
{
    DM_GEO_UNSORTED = 0,
    DM_GEO_SORTED_NEAR_FIRST,
    DM_GEO_SORTED_FAR_FIRST,
};

/// mode types for snapping
enum DM_SnapMode
{
    DM_SNAP_MODE_NONE = -1,
    DM_SNAP_MODE_GRID = 0,
    DM_SNAP_MODE_PRIM,
    DM_SNAP_MODE_POINT,
    DM_SNAP_MODE_MULTI,
    DM_SNAP_MODE_LAST // sentinel
};

/// Enumerates options that can change and whose changes are broadcast
enum DM_SnapshotOptionType
{
    DM_SNAPSHOT_HIDE_AT_SOURCE,	 // option of hiding snapshot at source frame
    DM_SNAPSHOT_ALPHA_VALUE,	 // the value of alpha blending
    DM_SNAPSHOT_VISIBLE_OBJECTS, // specification visible objects in snapshot 
    DM_SNAPSHOT_HIDE_HANDLES	 // option of hiding the handles in snapshots
};

/// The set of modifier keys used for selections assigned on Shift, Ctrl,
/// and Ctrl+Shift, respectively.
enum DM_SelectionModifierKeys
{
    DM_SMK_TOGGLE_REMOVE_ADD,	// Houdini and Maya
    DM_SMK_ADD_TOGGLE_REMOVE,	// Softimage
};

enum class DM_DrawablePrimitive : short
{
    Sphere
};

#define DM_PRIMARY_BUTTON	UI_LEFT_BUTTON
#define DM_SECONDARY_BUTTON	UI_MIDDLE_BUTTON
#define DM_TERTIARY_BUTTON	UI_RIGHT_BUTTON

#define DM_SELECTDRAG_BUTTON	UI_LEFT_BUTTON
#define DM_SAFESELECT_BUTTON	UI_MIDDLE_BUTTON
#define DM_FINISH_BUTTON	UI_RIGHT_BUTTON


#define DM_VIEW_MEMORY_NAME	"View"
#define DM_SNAPSHOT_MEMORY_NAME	"Snapshot"

#define DM_LABEL_HOFFSET	0.15
#define DM_LABEL_VOFFSET	0.05

class DM_ViewportMBlurDOFParms
{
public:
    DM_ViewportMBlurDOFParms()
	: mblur_frames(1), shutter(0.5), fps(24.0), frame_bias(0),
	  dof_quality(10), focal_length(10.0), aperture(41.4214), fstop(22.0),
	  beauty_only(false) {}
    int mblur_frames;
    fpreal shutter;
    fpreal fps;
    int frame_bias;
    int dof_quality;
    fpreal focal_length;
    fpreal aperture;
    fpreal fstop;
    bool beauty_only;
};

#endif
