/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP library (C++)
 *
 * COMMENTS:	Data types for cooked data
 *
 */

#ifndef __OP_DataTypes_h__
#define __OP_DataTypes_h__

#include <SYS/SYS_Deprecated.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_UniquePtr.h>

class OP_NetworkBoxItem;
class OP_Node;
class OP_DataBlock;

typedef UT_ValArray<OP_NetworkBoxItem *> OP_NetworkBoxItemList;
typedef UT_ValArray<OP_Node *> OP_NodeList;
typedef UT_IntrusivePtr<OP_DataBlock> OP_DataBlockPtr;

enum OP_DataType 
{
    OP_NO_DATA,			// No cooked data associated with the node
    OP_GEOMETRY_DATA,		// Cooked data in geometry format
    OP_TRANSFORM_DATA,		// Cooked data as a transform
    OP_OTHER_DATA,		// Generic catch all
    OP_CHANNEL_DATA,		// Channel collection output
    OP_SHADER_DATA,		// Shader data
    OP_SIM_DATA,		// Dynamics data
    OP_RASTER_DATA,		// Raster data
    OP_POP_DATA,		// Particle data
    OP_SCENE_DATA,		// Scene description
    OP_NUM_DATA_TYPES		// sentinel
};
// For backwards
typedef SYS_DEPRECATED(13.0) OP_DataType OP_DATA_TYPE;

enum OP_InterestType
{
    OP_INTEREST_NONE	= 0x00,	// I'm not interested
    OP_INTEREST_PARM	= 0x01,	// interested in a parameter changes (rare)
    OP_INTEREST_DATA	= 0x02,	// interested in op data changes (usual)
    OP_INTEREST_FLAG	= 0x04, // interested in flag changes (e.g.display flg)

    // OP_INTEREST_NAME should NOT be used with addExtraInput().
    // It should only be used with addOpReference() or addOpNameReference().
    OP_INTEREST_NAME	= 0x08, // interested in OP name changes

    OP_INTEREST_INPUT	= 0x10,	// interested when inputs change
    OP_INTEREST_OUTPUT	= 0x20,	// interested when outputs change
    OP_INTEREST_INPUTREF= 0x40,	// used for resolving input references

    // OP_INTEREST_NAME should NOT be used with addExtraInput().
    // It should only be used with addOpReference() or addOpNameReference().
				// interested in name & data:
    OP_INTEREST_NAMEDATA = OP_INTEREST_NAME|OP_INTEREST_DATA,
    OP_INTEREST_NAME_INPUT = OP_INTEREST_NAME|OP_INTEREST_INPUT,
    OP_INTEREST_NAME_OUTPUT = OP_INTEREST_NAME|OP_INTEREST_OUTPUT,

    // The following acts as a sentinal
    OP_INTEREST_ALL	= 0xFFFFFFFF	// Use this to match all interests
};

/// An enumeration that indicates to the user, what aspect of a OP_Node's UI 
/// representation changed, that caused the OP_UI_CHANGED event for that node  
/// to be sent.
enum OP_UIChangeType
{
    /// The change type wasn't set, so it could be any of them.
    OP_UICHANGE_ANY		= 0,	
    /// Errors/warnings got set/cleared.
    OP_UICHANGE_ERROR_STATE,
    /// The pick/selection state changed
    OP_UICHANGE_PICK,
    /// The node's color changed
    OP_UICHANGE_COLOR,
    /// The node's delete script changed
    OP_UICHANGE_DELETE_SCRIPT,
    /// The node's comment changed
    OP_UICHANGE_COMMENT,
    /// The node's lock flag
    OP_UICHANGE_LOCK_FLAG,
    /// The node's compress (thumbnail) flag changed
    OP_UICHANGE_COMPRESS_FLAG,
    /// The node's OTL definition match state changed 
    OP_UICHANGE_OTL_MATCH_STATE,
    /// The active input changed
    OP_UICHANGE_ACTIVE_INPUT,
    /// The input/output connections have changed
    OP_UICHANGE_CONNECTIONS,
    /// The expression language changed
    OP_UICHANGE_EXPR_LANGUAGE,
    
    /// A network box owned by this network changed
    OP_UICHANGE_NETWORKBOX,
    /// A post-it owned by this network changed
    OP_UICHANGE_POSTIT,
    /// A dot owned by this network changed
    OP_UICHANGE_DOT,
    
    OP_UICHANGE_OPTYPE_OBJ	= 1 << 8,
    
    OP_UICHANGE_OPTYPE_SOP	= 2 << 8,
    /// \sa COP2_UIChangeType
    OP_UICHANGE_OPTYPE_COP2	= 9 << 8,
    /// \sa VOP_UIChangeType
    OP_UICHANGE_OPTYPE_VOP	= 11 << 8,
};

#endif
