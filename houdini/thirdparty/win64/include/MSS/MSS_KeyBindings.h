/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UI_KeyBindings.h ( UI Library, C++)
 *
 * COMMENTS:
 *	Key bindings pertaining to Model SOP States.
 */

#ifndef __MSS_KeyBindings_h__
#define __MSS_KeyBindings_h__

#include <BM/BM_KeyBindings.h>

//
// ------------- Hotkey definitions: ----------------
//

#define MSS_KEY_SOPPATH		"h.pane.gview.state.sop."

// For the Face State:
#define MSS_KEY_CURVE_BEZIER		MSS_KEY_SOPPATH "curve.bezier"
#define MSS_KEY_CURVE_NURBS		MSS_KEY_SOPPATH "curve.nurbs"
#define MSS_KEY_CURVE_POLYGON		MSS_KEY_SOPPATH "curve.polygon"
#define MSS_KEY_CURVE_BREAKPOINTS	MSS_KEY_SOPPATH "curve.breakpoints"
#define MSS_KEY_CURVE_CVS		MSS_KEY_SOPPATH "curve.cvs"
#define MSS_KEY_CURVE_FREEHAND		MSS_KEY_SOPPATH "curve.freehand"
#define MSS_KEY_CURVE_TOGGLEOPEN	MSS_KEY_SOPPATH "curve.toggleopen"
#define MSS_KEY_CURVE_TRANSFLOOR	MSS_KEY_SOPPATH "curve.transfloor"
#define MSS_KEY_CURVE_RELPOINTS		MSS_KEY_SOPPATH "curve.relpoints"
#define MSS_KEY_CURVE_CONVERTREFS	MSS_KEY_SOPPATH "curve.convertrefs"

// For the PolyKnitState:
#define MSS_KEY_POLYKNIT_LOCATE		MSS_KEY_SOPPATH	"polyknit.locate"

// For the PolySplit State:
#define MSS_KEY_POLYSPLIT_LOCATE	MSS_KEY_SOPPATH "polysplit.locate"
#define MSS_KEY_POLYSPLIT_CONSTBIAS	MSS_KEY_SOPPATH "polysplit.constbias"
#define MSS_KEY_POLYSPLIT_MIDPOINT	MSS_KEY_SOPPATH "polysplit.midpoint"
#define MSS_KEY_POLYSPLIT_PERP		MSS_KEY_SOPPATH "polysplit.perp"
#define MSS_KEY_POLYSPLIT_TOGGLEOPEN	MSS_KEY_SOPPATH "polysplit.toggleopen"

// For the PolySplit2 State:
#define MSS_KEY_POLYSPLIT2_SHOWFEEDBACK	MSS_KEY_SOPPATH "polysplit2.showfeedback"
#define MSS_KEY_POLYSPLIT2_SNAPMIDPOINT	MSS_KEY_SOPPATH "polysplit2.snapmidpoint"
#define MSS_KEY_POLYSPLIT2_SNAPPERP	MSS_KEY_SOPPATH "polysplit2.snapperp"
#define MSS_KEY_POLYSPLIT2_SUBDIVTOGGLE MSS_KEY_SOPPATH "polysplit2.subdivtoggle"
#define MSS_KEY_POLYSPLIT2_TOGGLEOPEN	MSS_KEY_SOPPATH "polysplit2.toggleopen"
#define MSS_KEY_POLYSPLIT2_ALLOWFACES	MSS_KEY_SOPPATH "polysplit2.allowfaces"
#define MSS_KEY_POLYSPLIT2_QUADCOMPLETE	MSS_KEY_SOPPATH "polysplit2.togglequadcomp"
#define MSS_KEY_POLYSPLIT2_EDGEPERCENTTOGGLE	MSS_KEY_SOPPATH	"polysplit2.toggleedgepercent"
#define MSS_KEY_POLYSPLIT2_PARALLELLOOPTOGGLE   MSS_KEY_SOPPATH "polysplit2.parallellooptoggle"
#define MSS_KEY_POLYSPLIT2_PARALLELFLIPTOGGLE   MSS_KEY_SOPPATH "polysplit2.parallelfliptoggle"

// For the Brush States:
#define MSS_KEY_BRUSH_CIRCLE		MSS_KEY_SOPPATH "brush.circle"
#define MSS_KEY_BRUSH_SQUARE		MSS_KEY_SOPPATH "brush.square"
#define MSS_KEY_BRUSH_BITMAP		MSS_KEY_SOPPATH "brush.bitmap"
#define MSS_KEY_BRUSH_ORIENT		MSS_KEY_SOPPATH "brush.orient"
#define MSS_KEY_BRUSH_ACCUMSTENCIL	MSS_KEY_SOPPATH "brush.accumstencil"
#define MSS_KEY_BRUSH_P_MAIN		MSS_KEY_SOPPATH "brush.p_main"
#define MSS_KEY_BRUSH_P_SMOOTH		MSS_KEY_SOPPATH "brush.p_smooth"
#define MSS_KEY_BRUSH_P_ERASE		MSS_KEY_SOPPATH "brush.p_erase"
#define MSS_KEY_BRUSH_P_LIFT		MSS_KEY_SOPPATH "brush.p_lift"
#define MSS_KEY_BRUSH_P_ROTATE		MSS_KEY_SOPPATH "brush.p_rotate"
#define MSS_KEY_BRUSH_S_MAIN		MSS_KEY_SOPPATH "brush.s_main"
#define MSS_KEY_BRUSH_S_SMOOTH		MSS_KEY_SOPPATH "brush.s_smooth"
#define MSS_KEY_BRUSH_S_ERASE		MSS_KEY_SOPPATH "brush.s_erase"
#define MSS_KEY_BRUSH_S_LIFT		MSS_KEY_SOPPATH "brush.s_lift"
#define MSS_KEY_BRUSH_S_ROTATE		MSS_KEY_SOPPATH "brush.s_rotate"

// For the Sculpt State:
#define MSS_KEY_SCULPT_P_SMOOTHDEFORM	MSS_KEY_SOPPATH "brush.sculpt.p_smdef"
#define MSS_KEY_SCULPT_S_SMOOTHDEFORM	MSS_KEY_SOPPATH "brush.sculpt.s_smdef"

// For the Paint State:
#define MSS_KEY_PAINT_P_EYEDROP		MSS_KEY_SOPPATH "brush.paint.p_eyedrop"
#define MSS_KEY_PAINT_S_EYEDROP		MSS_KEY_SOPPATH "brush.paint.s_eyedrop"

// For the Edit State:
#define MSS_KEY_EDIT_BRUSH		MSS_KEY_SOPPATH "edit.brush"
#define MSS_KEY_EDIT_SLIDE		MSS_KEY_SOPPATH "edit.slide"
#define MSS_KEY_EDIT_FLATTEN		MSS_KEY_SOPPATH "edit.flatten"
#define MSS_KEY_EDIT_CIRCLE		MSS_KEY_SOPPATH "edit.circle"
#define MSS_KEY_EDIT_SPACING		MSS_KEY_SOPPATH "edit.spacing"
#define MSS_KEY_EDIT_STRAIGHTEN		MSS_KEY_SOPPATH "edit.straighten"
#define MSS_KEY_EDIT_SMOOTH		MSS_KEY_SOPPATH "edit.smooth"
#define MSS_KEY_EDIT_TOGGLE_TWEAK	MSS_KEY_SOPPATH "edit.toggle_tweak_mode"

// For the UVPelt Selector:
#define MSS_KEY_UVPELT_VISUALIZE	MSS_KEY_SOPPATH "uvpelt.visualize"

// For the UVEdit State:
#define MSS_KEY_UVEDIT_TOGGLE_SEW	MSS_KEY_SOPPATH "uvedit.toggle_sew"
#define MSS_KEY_UVEDIT_SMOOTH		MSS_KEY_SOPPATH "uvedit.smooth"
#define MSS_KEY_UVEDIT_TOGGLE_TWEAK	MSS_KEY_SOPPATH "uvedit.toggle_tweak_mode"

// For the UVFlatten State:
#define MSS_KEY_UVFLATTEN_TOGGLE_MODE	MSS_KEY_SOPPATH "uvflatten.mode"
#define MSS_KEY_UVFLATTEN_FLATTEN_U	MSS_KEY_SOPPATH "uvflatten.flatten_u"
#define MSS_KEY_UVFLATTEN_FLATTEN_V	MSS_KEY_SOPPATH "uvflatten.flatten_v"

// For the UVFlatten 2.0 State:
#define MSS_KEY_UVFLATTEN2_CUT_MODE	MSS_KEY_SOPPATH "uvflatten2.cutmode"
#define MSS_KEY_UVFLATTEN2_SEW_MODE	MSS_KEY_SOPPATH "uvflatten2.sewmode"
#define MSS_KEY_UVFLATTEN2_PIN_MODE	MSS_KEY_SOPPATH "uvflatten2.pinmode"
#define MSS_KEY_UVFLATTEN2_ALIGN_MODE	MSS_KEY_SOPPATH "uvflatten2.alignmode"
#define MSS_KEY_UVFLATTEN2_STRAIGHTEN_MODE	MSS_KEY_SOPPATH "uvflatten2.straightenmode"

#define MSS_KEY_UVFLATTEN2_ALIGN_PINS	MSS_KEY_SOPPATH "uvflatten2.align_pins"
#define MSS_KEY_UVFLATTEN2_ALIGN_PINS_U	MSS_KEY_SOPPATH "uvflatten2.align_pins_u"
#define MSS_KEY_UVFLATTEN2_ALIGN_PINS_V	MSS_KEY_SOPPATH "uvflatten2.align_pins_v"

#define MSS_KEY_UVFLATTEN2_CLEAR_CONSTRAINTS	MSS_KEY_SOPPATH "uvflatten2.clear_constraints"
#define MSS_KEY_UVFLATTEN2_RESET_LAYOUT		MSS_KEY_SOPPATH "uvflatten2.reset_layout"
#define MSS_KEY_UVFLATTEN2_IMPORT_SEAMS		MSS_KEY_SOPPATH "uvflatten2.import_seams"

// For the UVFlatten 3.0 State:
#define MSS_KEY_UVFLATTEN3_CUT_MODE	MSS_KEY_SOPPATH "uvflatten3.cutmode"
#define MSS_KEY_UVFLATTEN3_SEW_MODE	MSS_KEY_SOPPATH "uvflatten3.sewmode"
#define MSS_KEY_UVFLATTEN3_PIN_MODE	MSS_KEY_SOPPATH "uvflatten3.pinmode"
#define MSS_KEY_UVFLATTEN3_ALIGN_MODE	MSS_KEY_SOPPATH "uvflatten3.alignmode"
#define MSS_KEY_UVFLATTEN3_RECTIFY_MODE	MSS_KEY_SOPPATH "uvflatten3.rectifymode"
#define MSS_KEY_UVFLATTEN3_STRAIGHTEN_MODE	MSS_KEY_SOPPATH "uvflatten3.straightenmode"

#define MSS_KEY_UVFLATTEN3_ALIGN_PINS	MSS_KEY_SOPPATH "uvflatten3.align_pins"
#define MSS_KEY_UVFLATTEN3_ALIGN_PINS_U	MSS_KEY_SOPPATH "uvflatten3.align_pins_u"
#define MSS_KEY_UVFLATTEN3_ALIGN_PINS_V	MSS_KEY_SOPPATH "uvflatten3.align_pins_v"

#define MSS_KEY_UVFLATTEN3_CLEAR_CONSTRAINTS	MSS_KEY_SOPPATH "uvflatten3.clear_constraints"
#define MSS_KEY_UVFLATTEN3_RESET_LAYOUT		MSS_KEY_SOPPATH "uvflatten3.reset_layout"
#define MSS_KEY_UVFLATTEN3_IMPORT_SEAMS		MSS_KEY_SOPPATH "uvflatten3.import_seams"
#define MSS_KEY_UVFLATTEN3_QUICKSELECT		MSS_KEY_SOPPATH "uvflatten3.quick_select"

// For the PolyExtrude 2.0 State:
#define MSS_KEY_EXTRUDE2_TOGGLE_XFORM	MSS_KEY_SOPPATH "polyextrude2.toggle_xform"
#define MSS_KEY_EXTRUDE2_TOGGLE_MODE	MSS_KEY_SOPPATH "polyextrude2.toggle_mode"

// For the AgentCollisionLayer State:
#define MSS_KEY_AGENTCOLLISIONLAYER_TOGGLE_MODE	MSS_KEY_SOPPATH "agentcollisionlayer.mode"

// For the Agent Configure Joints State:
#define MSS_KEY_AGENTCONFIGUREJOINTS_INITLIMITS  MSS_KEY_SOPPATH "agentconfigurejoints.initlimits"

// For the TopoBuild State:
#define MSS_KEY_TOPOBUILD_BUILD		MSS_KEY_SOPPATH "topobuild.build"
#define MSS_KEY_TOPOBUILD_SLIDE		MSS_KEY_SOPPATH "topobuild.slide"
#define MSS_KEY_TOPOBUILD_SPLIT		MSS_KEY_SOPPATH "topobuild.split"
#define MSS_KEY_TOPOBUILD_BRUSH		MSS_KEY_SOPPATH "topobuild.brush"
#define MSS_KEY_TOPOBUILD_SMOOTH	MSS_KEY_SOPPATH "topobuild.smooth"
#define MSS_KEY_TOPOBUILD_SKIN		MSS_KEY_SOPPATH "topobuild.skin"
#define MSS_KEY_TOPOBUILD_COLLAPSE	MSS_KEY_SOPPATH "topobuild.collapse"
#define MSS_KEY_TOPOBUILD_CIRCLE	MSS_KEY_SOPPATH "topobuild.circle"
#define MSS_KEY_TOPOBUILD_SPACING	MSS_KEY_SOPPATH "topobuild.spacing"
#define MSS_KEY_TOPOBUILD_STRAIGHTEN	MSS_KEY_SOPPATH "topobuild.straighten"
#define MSS_KEY_TOPOBUILD_SKINALIGN	MSS_KEY_SOPPATH "topobuild.skinalign"
#define MSS_KEY_TOPOBUILD_SKINTRANSLATE	MSS_KEY_SOPPATH "topobuild.skintranslate"
#define MSS_KEY_TOPOBUILD_DISABLEPROJECTION	MSS_KEY_SOPPATH "topobuild.disableprojection"
#define MSS_KEY_TOPOBUILD_FILL		MSS_KEY_SOPPATH "topobuild.fill"
#define MSS_KEY_TOPOBUILD_BRIDGE	MSS_KEY_SOPPATH "topobuild.bridge"
#define MSS_KEY_TOPOBUILD_BRIDGE_CONNECTED	MSS_KEY_SOPPATH "topobuild.bridgeconnected"
#define MSS_KEY_TOPOBUILD_HIDE		MSS_KEY_SOPPATH "topobuild.hide"
#define MSS_KEY_TOPOBUILD_UNHIDE	MSS_KEY_SOPPATH "topobuild.unhide"
#define MSS_KEY_TOPOBUILD_INVERTHIDDEN	MSS_KEY_SOPPATH "topobuild.inverthidden"
#endif
