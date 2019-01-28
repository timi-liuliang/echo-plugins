/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DD_Define.h
 *
 * COMMENTS:
 */

#ifndef __DD_Define__
#define __DD_Define__

enum DD_State {
    DD_STATE_START,
    DD_STATE_DRAG,
    DD_STATE_END,
    DD_STATE_CANCEL
};

//
// Some common drag-n-drop attributes
// which are supplied by the sender
//
// Strings are chose over enumerated types
// so custom drag-n-drop objects will have
// a smaller chance of name collision.
//

#define DD_DATA_TEXT			"text"
#define DD_LABEL_TEXT			"Text"

// the data parameter is a (void *)int to index between multiple choices
#define DD_DATA_PARM_PATH		"parm_path"
#define DD_LABEL_PARM_PATH		"Parameter Path"

#define DD_DATA_CHAN_PATH		"chan_path"
#define DD_DATA_FLAG_PATH		"flag_path"

// the node and track are separated by a space since track names may
// have slashes in them
#define DD_DATA_TRACK_PATH		"track_path"

// the data parameter is a (void *)int to index between multiple choices
#define DD_DATA_OP_PATH			"op_path"
#define DD_LABEL_OP_PATH		"Node Path"

#define DD_DATA_ITEM_PATH		"item_path"
#define DD_LABEL_ITEM_PATH		"Nwtwork Item Path"
#define DD_DATA_PERS_HANDLE_PATH	"persistent_handle_path"

// returns the name of a take
#define DD_DATA_TAKE_NAME		"take_name"

// returns a pointer to an OP_GalleryEntry or the gallery entry name
#define DD_DATA_GALLERY_ENTRY		"gallery_entry"
#define DD_DATA_GALLERY_ENTRY_NAME	"gallery_entry_name"
#define DD_LABEL_GALLERY_ENTRY_NAME	"Gallery Entry"

// returns a pointer to SHLF_Tool or the shelf tool name
#define DD_DATA_SHELF_TOOL		"shelf_tool"
#define DD_DATA_SHELF_TOOL_NAME		"shelf_tool_name"
#define DD_LABEL_SHELF_TOOL_NAME	"Shelf Tool"

// Returns the name of the node type
#define DD_DATA_ASSET_GUID		"asset_guid"
#define DD_LABEL_ASSET_GUID		"Asset Guid"
#define DD_DATA_ASSETTOOL_NAME		"asset_tool_name"
#define DD_LABEL_ASSETTOOL_NAME		"Asset Tool"
#define DD_DATA_FOLDER_GUID		"folder_guid"
#define DD_LABEL_FOLDER_GUID		"Folder Guid"

// returns a pointer to OPUI_ToolbarItem
#define DD_DATA_SHELF_TOOL_BUTTON	"shelf_tool_button"

// returns a pointer to the OPUI_Toolbar or the shelf name
#define DD_DATA_SHELF_TOOLBAR		"shelf_toolbar"
#define DD_DATA_SHELF_TOOLBAR_NAME	"shelf_toolbar_name"
#define DD_LABEL_SHELF_TOOLBAR_NAME	"Shelf"

// returns a pointer to FUSE_DesktopNode or the name of the pane tab
#define DD_DATA_DESKTOP_NODE		"desktop_node"
#define DD_DATA_DESKTOP_NODE_NAME	"desktop_node_name"
#define DD_LABEL_DESKTOP_NODE_NAME	"Pane Tab"

#define DD_DATA_DOPDATA_PYTHON		"dopdata_python"
#define DD_LABEL_DOPDATA_PYTHON		"DopData as Python expr"
#define DD_DATA_DOPDATA_HSCRIPT_FLOAT	"dopdata_hscript_float"
#define DD_LABEL_DOPDATA_HSCRIPT_FLOAT	"DopData as Hscript float"
#define DD_DATA_DOPDATA_HSCRIPT_STRING	"dopdata_hscript_string"
#define DD_LABEL_DOPDATA_HSCRIPT_STRING	"dopdata as Hscript string"

// Returns a path to a primitive, which is essentially the SOP path
// ('/obj/foo/mysop') plus the primitive path name (can be partial) separated
// by a colon (e.g: '/obj/foo/mysop:/alembic/foo/prim')
#define DD_DATA_PRIM_PATH		"prim_path"
#define DD_LABEL_PRIM_PATH		"Primitive Path"

// Visualizder
#define DD_DATA_VISUALIZER		"visualizer"
#define DD_LABEL_VISUALIZER		"Visualizer"

// USD Primitives and collections
#define DD_DATA_USDPRIMPATH		"usdprimpath"

//
// Some attributes which receivers can derive given the above
//

//
// parameter
//

#define DD_DATA_CHAN_VALUE	"chan_value"
#define DD_LABEL_CHAN_VALUE	"Current Channel Value"


#define DD_DATA_CHAN_EXPR	"chan_express"
#define DD_LABEL_CHAN_EXPR	"Channel Expression"

#define DD_DATA_CHAN_REF_ABS	"chan_ref_abs"
#define DD_LABEL_CHAN_REF_ABS	"Absolute Channel Reference"

#define DD_DATA_CHAN_REF_REL	"chan_ref_rel"
#define DD_LABEL_CHAN_REF_REL	"Relative Channel Reference"

#define DD_DATA_COPY_CHANNEL	"copy_channel"
#define DD_LABEL_COPY_CHANNEL	"Copy Channel"

#define DD_DATA_CHAN_GROUPS	"chgroups"
#define DD_LABEL_CHAN_GROUPS	"Channel Groups"

#define DD_DATA_SI_TREENODES	"si_tree_nodes"

//
// chop
//
//

#define DD_DATA_TRACK_VALUE	"chop_value"
#define DD_LABEL_TRACK_VALUE	"Current CHOP Value"

#define DD_DATA_TRACK_REF_ABS	"chop_ref_abs"
#define DD_LABEL_TRACK_REF_ABS	"CHOP Reference"

#define DD_DATA_TRACK_REF_REL	"chop_ref_rel"
#define DD_LABEL_TRACK_REF_REL	"Relative CHOP Reference"

#define DD_DATA_TRACK_EXPORT	"chop_export"
#define DD_LABEL_TRACK_EXPORT	"Export CHOP"

#endif


