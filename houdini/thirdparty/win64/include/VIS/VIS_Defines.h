/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VIS_Defines.h (VIS Library, C++)
 *
 * COMMENTS:
 * 	Common definitions used by visualization classes.
 */

#ifndef __VIS_Defines__
#define __VIS_Defines__

#include "VIS_API.h"
#include <UT/UT_Notifier.h>

#include <DEP/DEP_MicroNode.h>
#include <PRM/PRM_ParmList.h>

class VIS_API VIS_Context
{
public:
		 VIS_Context();
    explicit 	 VIS_Context(int key);
		 VIS_Context(const VIS_Context &other);
    int		 getKey() const;
    inline bool	 operator==(VIS_Context other) const;
    inline bool  operator!=(VIS_Context other) const;

    const static VIS_Context Common;
    const static VIS_Context None;
private:
    int myKey;
};

// Default icon for a new visualizer type
#define VIS_TYPE_DEFAULT_ICON		"SHOP_vm_geo_file"

// Name of visualizer types used to load up old user options as visualizers:
#define VIS_GENERIC_NAME		"vis_generic"
#define VIS_MARKER_NAME			"vis_marker"

// Standard visualizer parameter names:
#define VIS_PARM_NAME_ATTRIB		"attrib"
#define VIS_PARM_NAME_GROUP		"group"
#define VIS_PARM_NAME_CLASS		"class"


class VIS_API VIS_TableRef
{
public:
			 VIS_TableRef();
    explicit 		 VIS_TableRef(int i);
			 VIS_TableRef(const VIS_TableRef &src);
    inline bool 	 operator==(const VIS_TableRef other) const;
    inline bool 	 operator!=(const VIS_TableRef other) const;
    inline bool 	 isNode() const { return myId >= 0; }
    inline int 		 getId() const { return myId; }

    static const VIS_TableRef Unassigned;
    static const VIS_TableRef SceneFile;
    static const VIS_TableRef Global;
    static const VIS_TableRef Adhoc;

private:
    int			 myId;
};

struct VIS_API VIS_Event
{
    enum VIS_EventType
    {
	VISUALIZER_ADDED,
	VISUALIZER_DELETED,
	MOUNT_CHANGED,
	VISUALIZER_PARMS_CHANGED,
	VISUALIZER_FLAGS_CHANGED,
	ACTIVE_STATE_CHANGED,
	TABLE_ENABLED_STATE_CHANGED,
	TABLE_DELETED
    };

    VIS_Event(VIS_EventType type, VIS_TableRef owner, int index, int uid) :
	myEventType(type), myOwner(owner), myIndex(index), myUniqueId(uid)
    {}

    VIS_EventType	 myEventType;
    VIS_TableRef	 myOwner;
    int			 myIndex;
    int			 myUniqueId;
};


typedef UT_NotifierImpl<VIS_Event &> VIS_EventNotifier;

namespace VIS_Flags
{

// Visualizer flag bits
enum
{
    CREATES_PRIMITIVES_BIT = 0,
    FILTERS_PRIMITIVES_BIT,
    SETS_UP_DECORATIONS_BIT,
    RENDERS_DECORATIONS_BIT,
    SETS_UP_RENDER_BIT,
    NEEDS_REFINE_ON_ACTIVATION_BIT,
    NEEDS_REFINE_ON_DEACTIVATION_BIT,
    HAS_ATTRIBUTE_PARM_BIT,
    HAS_GROUP_PARM_BIT,
    EXCLUSIVE_ACTION_PER_TYPE_BIT,
    COMMON_CONTEXT_ONLY_BIT,
    NEEDS_UPDATE_BIT,
    NEEDS_REFINE_BIT,
    NEEDS_REFINE_ON_GEO_CHANGE_BIT,
    COLLECTS_PRIMITIVES_BIT,
    //----------------
    NUM_VISUALIZER_FLAGS
};

// Visualizer flags
const uint32  CreatesPrimitives		= 1 << CREATES_PRIMITIVES_BIT;
const uint32  FiltersPrimitives		= 1 << FILTERS_PRIMITIVES_BIT;
const uint32  SetsUpDecorations		= 1 << SETS_UP_DECORATIONS_BIT;
const uint32  RendersDecorations	= 1 << RENDERS_DECORATIONS_BIT;
const uint32  SetsUpRender		= 1 << SETS_UP_RENDER_BIT;
const uint32  NeedsRefineOnActivation	= 1 << NEEDS_REFINE_ON_ACTIVATION_BIT;
const uint32  NeedsRefineOnDeactivation	= 1 << NEEDS_REFINE_ON_DEACTIVATION_BIT;
const uint32  HasAttributeParm		= 1 << HAS_ATTRIBUTE_PARM_BIT;
const uint32  HasGroupParm		= 1 << HAS_GROUP_PARM_BIT;
const uint32  ExclusiveActionPerType	= 1 << EXCLUSIVE_ACTION_PER_TYPE_BIT;
const uint32  CommonContextOnly		= 1 << COMMON_CONTEXT_ONLY_BIT;
const uint32  NeedsUpdate		= 1 << NEEDS_UPDATE_BIT;
const uint32  NeedsRefine		= 1 << NEEDS_REFINE_BIT;
const uint32  NeedsRefineOnGeoChange	= 1 << NEEDS_REFINE_ON_GEO_CHANGE_BIT;
const uint32  CollectsPrimitives	= 1 << COLLECTS_PRIMITIVES_BIT;

// Mount flag bits
enum
{
    ACT_ON_UNSELECTED_BIT = 0,
    ACT_ON_SELECTED_BIT,
    ACT_ON_GHOSTED_BIT,
    ACT_ON_DISPLAYED_BIT,
    ACT_ON_CURRENT_BIT,
    ACT_ON_TEMPLATED_BIT,
    SHOW_IN_TOOLBAR,
    NO_MOVE,
    NO_DELETE,
    NO_SAVE,
    NO_TYPE_CHANGE,
    //----------------
    NUM_MOUNT_FLAGS
};

// Mount flags
const uint32  ActOnUnselected		= 1 << ACT_ON_UNSELECTED_BIT;
const uint32  ActOnSelected		= 1 << ACT_ON_SELECTED_BIT;
const uint32  ActOnGhosted		= 1 << ACT_ON_GHOSTED_BIT;
const uint32  ActOnDisplayed		= 1 << ACT_ON_DISPLAYED_BIT;
const uint32  ActOnCurrent		= 1 << ACT_ON_CURRENT_BIT;
const uint32  ActOnTemplated		= 1 << ACT_ON_TEMPLATED_BIT;
const uint32  ShowInToolbar		= 1 << SHOW_IN_TOOLBAR;
const uint32  NoMove			= 1 << NO_MOVE;
const uint32  NoDelete			= 1 << NO_DELETE;
const uint32  NoSave			= 1 << NO_SAVE;
const uint32  NoTypeChange		= 1 << NO_TYPE_CHANGE;
} // namespace VIS_Flags

enum VIS_VisualizerScope
{
    NODE_ONLY				= 0,
    DESCENDENTS_SAME_NETWORK_TYPE,
    DESCENDENTS_ALL,
    GLOBAL,

    NUM_SCOPES				// count sentinel
};


// Helper single input micronode subclass with the ability to arm and disarm

class VIS_MicroNode : public DEP_MicroNode
{
public:
			 VIS_MicroNode() : myArmed(false), myInput(NULL) {}
    void 	 	 setInput(DEP_MicroNode *input) { myInput = input; }
    void	 	 arm(bool armed);
    bool	 	 isArmed() { return myArmed; }
    DEP_MicroNode 	*getInput() { return myInput; }
private:
    DEP_MicroNode	*myInput;
    bool 	 	 myArmed;
};


inline bool VIS_TableRef::operator==(VIS_TableRef other) const
{
    return myId == other.myId;
}
inline bool VIS_TableRef::operator!=(VIS_TableRef other) const
{
    return myId != other.myId;
}


inline bool
VIS_Context::operator==(VIS_Context other) const
{
    return myKey == other.myKey;
}

inline bool
VIS_Context::operator!=(VIS_Context other) const
{
    return myKey != other.myKey;
}



#endif
