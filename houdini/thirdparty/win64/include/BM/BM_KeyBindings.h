/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BM_KeyBindings.h ( UI Library, C++)
 *
 * COMMENTS:
 *	Key bindings pertaining to states.
 */

#ifndef __BM_KeyBindings_h__
#define __BM_KeyBindings_h__

#include <UI/UI_KeyBindings.h>

#define BM_KEY_VIEWERPATH	"h.pane.gview."
#define BM_KEY_HANDLEPATH	BM_KEY_VIEWERPATH "handle."
#define BM_KEY_STATEPATH	BM_KEY_VIEWERPATH "state."
#define BM_KEY_MODELPATH	BM_KEY_VIEWERPATH "model."
#define BM_KEY_SELECTMODEPATH	BM_KEY_VIEWERPATH "selmode."


#define BM_KEY_ACCEPT		BM_KEY_VIEWERPATH "accept"
#define BM_KEY_DELETE		BM_KEY_VIEWERPATH "delete"
#define BM_KEY_EDITTOOLBAR	BM_KEY_VIEWERPATH "edittoolbar"
#define BM_KEY_UP		BM_KEY_VIEWERPATH "up"
#define BM_KEY_DOWN		BM_KEY_VIEWERPATH "down"
#define BM_KEY_LEFT		BM_KEY_VIEWERPATH "left"
#define BM_KEY_RIGHT		BM_KEY_VIEWERPATH "right"
#define BM_KEY_TOP		BM_KEY_VIEWERPATH "top"
#define BM_KEY_BOTTOM		BM_KEY_VIEWERPATH "bottom"

#define BM_KEY_EDITOP		BM_KEY_VIEWERPATH "editop"
#define BM_KEY_OPPARMS		BM_KEY_VIEWERPATH "operation_parameters"
#define BM_KEY_TOGGLEHUDDISPLAY BM_KEY_VIEWERPATH "togglehuddisplay"
#define BM_KEY_HIDEPERSISTENTS	BM_KEY_VIEWERPATH "hidepersistents"
#define BM_KEY_SHOWPERSISTENTS	BM_KEY_VIEWERPATH "showpersistents"
#define BM_KEY_MAKEPERSISTENTS	BM_KEY_VIEWERPATH "makepersistents"
#define BM_KEY_DELETEPERSISTENTS	BM_KEY_VIEWERPATH "deletepersistents"
#define BM_KEY_KEYFRAMEHANDLES		BM_KEY_VIEWERPATH "keyframehandles"
#define BM_KEY_SCOPEHANDLES		BM_KEY_VIEWERPATH "scopehandles"
#define BM_KEY_EXPORTALLPARMS		BM_KEY_VIEWERPATH "export_all_parms"
#define BM_KEY_EXPORTALLHANDLES		BM_KEY_VIEWERPATH "export_all_handles"

#define BM_KEY_OPERATOR_MENU		BM_KEY_VIEWERPATH "operator_menu"
#define BM_KEY_OPERATOR_MENU_BRANCH	BM_KEY_VIEWERPATH "operator_menu_branch"
#define BM_KEY_REPEAT_CURRENT		BM_KEY_VIEWERPATH "repeat_current"
#define BM_KEY_REPEAT_CURRENT_BRANCH	BM_KEY_VIEWERPATH "repeat_current_branch"
#define BM_KEY_REPEAT_PREVIOUS		BM_KEY_VIEWERPATH "repeat_previous"
#define BM_KEY_REPEAT_PREVIOUS_BRANCH	BM_KEY_VIEWERPATH "repeat_previous_branch"

#define BM_KEY_TOGGLE_PERSISTENT	BM_KEY_HANDLEPATH "persistent"
#define BM_KEY_TOGGLE_DISPLAY		BM_KEY_HANDLEPATH "display"
#define BM_KEY_BOUND_OPPARMS		BM_KEY_HANDLEPATH "bound_opparms"
#define BM_KEY_EXPORT_PARMS		BM_KEY_HANDLEPATH "export_parms"
#define BM_KEY_EXPORT_HANDLE		BM_KEY_HANDLEPATH "export_handle"
#define BM_KEY_HANDLE_PARMS		BM_KEY_HANDLEPATH "handle_parms"
#define BM_KEY_ANIM_OPPARMS		BM_KEY_HANDLEPATH "anim_opparms"
#define BM_KEY_NEXT_KEYFRAME		BM_KEY_HANDLEPATH "step_to_next_key"
#define BM_KEY_PREVIOUS_KEYFRAME	BM_KEY_HANDLEPATH "step_to_prev_key"
#define BM_KEY_SET_KEY_FRAME		BM_KEY_HANDLEPATH "set_keyframe"
#define BM_KEY_REMOVE_KEY_FRAME		BM_KEY_HANDLEPATH "remove_keyframe"
#define BM_KEY_SCOPE_APPEND_CHANNELS	\
				    BM_KEY_HANDLEPATH "scope_append_channels"
#define BM_KEY_SCOPE_CHANNELS		BM_KEY_HANDLEPATH "scope_channels"
#define BM_KEY_DEL_CHANNELS		BM_KEY_HANDLEPATH "delete_channels"
#define BM_KEY_LOCK_PARMS		BM_KEY_HANDLEPATH "lock_parms"
#define BM_KEY_TO_DEFAULTS		BM_KEY_HANDLEPATH "revert_defaults"
#define BM_KEY_TO_FACTORY_DEFAULTS	\
				    BM_KEY_HANDLEPATH "revert_factory_defaults"
#define BM_KEY_OVERWRITE_DEFAULTS	BM_KEY_HANDLEPATH "overwrite_defaults"

#define BM_KEY_SELECTTOOL		BM_KEY_STATEPATH "new_select"
#define BM_KEY_RENDERREGIONTOOL		BM_KEY_STATEPATH "new_renderregion"

// Selection short-cuts. These go immediately into selection mode.

#define BM_KEY_SELTYPE_OBJECT		BM_KEY_SELECTMODEPATH "object"
#define BM_KEY_SELTYPE_POINTS		BM_KEY_MODELPATH "seltypepoints"
#define BM_KEY_SELTYPE_EDGES		BM_KEY_MODELPATH "seltypeedges"
#define BM_KEY_SELTYPE_PRIMS		BM_KEY_MODELPATH "seltypeprims"
#define BM_KEY_SELTYPE_VERTICES		BM_KEY_MODELPATH "seltypevertices"
#define BM_KEY_SELTYPE_BREAKPOINTS	BM_KEY_MODELPATH "seltypebreakpoints"
#define BM_KEY_SELTYPE_GROUPS		BM_KEY_MODELPATH "seltypegroups"


#endif
