/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP library (C++)
 *
 * COMMENTS:	Value for expressing interest in a node...
 *
 */

#ifndef __OP_Value_h__
#define __OP_Value_h__

#include "OP_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_WorkBuffer.h>

class OP_Node;
class OP_EventHandler;

typedef enum
{
    OP_CHILD_CREATED,	// A new node has been created -- DATA: ptr to child
    OP_CHILD_REORDERED,	// A child's index has been changed 
    			// -- DATA: child ptr

    OP_NODE_DELETED,	// This node was deleted 
    OP_CHILD_DELETED,	// Let the network know about the above.

    OP_CHILD_UNSTOWED,	// These are used by OH to signal that an OH gadget
    OP_NODE_STOWED,	// is stow/unstowed and should have interests added
			// or removed appropriately.
    OP_NODE_HIDDEN,	// Almost the same as a STOWED. UNSTOW is opposite.
    OP_CHILD_REVEALED,	// Almost the same as a UNSTOWED. STOW is opposite.
    OP_CHILD_HIDDEN,	// Let network know about the above.

    OP_CHILD_SWITCHED,	// Change to networks current, display or render node.

    OP_NAME_CHANGED,	// This node's name changed

    // these five need to be propagated to dependents so that we can recook
    OP_INPUT_REWIRED,	// One of this node's inputs was connected to
			// a different node 	-- DATA: input index
    OP_INPUT_CHANGED,	// One of the input nodes has changed
    OP_FLAG_CHANGED,	// Something like the display flag changed


    // of the following three events, each implies the preceeding ones
    OP_PARM_UICHANGED,	// A parameter's appearance changed
			// -- DATA: parm index
    OP_PARM_CHANGED,	// A parameter's value changed -- DATA: parm index
    OP_PARM_ANIMATED,	// One of the node's channels has changed in a drastic
			// way eg keyframe added/removed -- DATA: parm index
    OP_PARM_CHANNELS,	// A channel has been added, deleted, or renamed
			// -- DATA: parm index

    OP_UI_CHANGED,	// for example error status, current, selected
			// -- DATA: OP_UIChangeType 
    OP_UI_CURRENT_CHANGED,	// The current node has been set when emitted
				// from OPgetDirector() -- DATA: node pointer
    OP_UI_EDIT_CURRENT_CHANGED,	// The current node has been set when emitted
				// from OPgetDirector() and is meant to be
				// edited -- DATA: node pointer
    OP_UI_EDIT_NETWORK,		// Our picked network has changed.
				    // This is only emitted from OP_Director 
				    // -- DATA: network node pointer
    OP_UI_MOVED,	// location change
    OP_OUTPUT_CHANGED,	// the output of a node has changed 
    OP_OUTPUT_CHANGED_FOR_COOK_TYPE, // the output of a node has changed as the
				     // node changed from cooking for render to
				     // cooking for display or vice versa.
				     // This change, unlike OP_OUTPUT_CHANGED,
				     // is not propagated to dependents!
				     // -- DATA: 0 = cooked for display
				     // -- DATA: 1 = cooked for render
    OP_GROUPLIST_CHANGED,// The network group list or selection has changed
    OP_INDIRECT_CREATED,// An OP_IndirectInput object was added to the net
    OP_INDIRECT_DELETED,// or deleted from the net.

    OP_COOKING_CHANGED,	// current status of cook state (for performance)

    OP_GROUP_CREATED,   // An OP_Group object was added to the net
    OP_GROUP_DELETED,   // or deleted from the net.
    OP_GROUP_CHANGED,   // An OP_Group was renamed or its membership
		        // changed.

    OP_GEOSELECTION_CHANGED, // The geometry's selection changed

    // These CHGROUP events are only triggered on the OP_Director
    // data is (CH_Group*)
    OP_CHGROUP_CREATED,		// Channel group added (must be first)
    OP_CHGROUP_REMOVED,		// Channel group removed
    OP_CHGROUP_RENAMED,		// Channel group renamed
    OP_CHGROUP_MOVED,		// Channel group changed index
    OP_CHGROUP_SELCHANGED,	// Group selection changed
    OP_CHGROUP_CURCHANGED,	// Current group changed
    OP_CHGROUP_CHANGED,		// Channel group membership has changed
				// (stays last)

    OP_INPUT_RENAMED,		// One of our inputs was renamed

    OP_CHSCOPE_CHANGED,		// Channel scope has changed (only OP_Director)
    OP_CHSCOPEPIN_CHANGED,	// Channel selection has changed
    OP_CHSCOPESELECTION_CHANGED,// Channel selection has changed
				// (only OP_Director)
    OP_CHSCOPE_DISPLAY_CHANGED,	// Channel selection has changed
				// (only OP_Director)
    OP_CHSCOPE_GRAPH_SELECTION_CHANGED,	// Channel selection has changed
				// (only OP_Director)

    OP_CHILD_PICK_CHANGED,	// A selection has changed for our children

    OP_NODE_PREDELETE,		// This node will be deleted 

    OP_NETWORKBOX_CREATED,	// A network box was created within the network
				// -- DATA: OP_NetworkBox*
    OP_NETWORKBOX_DELETED,	// A network box was removed from the network
				// -- DATA: OP_NetworkBox*
    OP_NETWORKBOX_CHANGED,	// The contents of a network box have changed
				// -- DATA: OP_NetworkBox*
    OP_NETWORKBOX_STOWED,	// An OP_NetworkBox has been stowed, similar to
				// OP_NODE_STOWED

    OP_SPAREPARM_MODIFIED,	// A Spare parm was created or deleted.

    OP_MULTIPARM_MODIFIED,	// A Multiparm instance was created or deleted.

    OP_PARM_ENABLE_CHANGED,	// Parm's enable state changed.
				// -- DATA: parm index

    OP_POSTIT_NOTE_CREATED,	// A post it note was created within the network
    OP_POSTIT_NOTE_DELETED,	// A post it note was removed from the network
    OP_POSTIT_NOTE_CHANGED,	// The contents of a post it note have changed
    OP_POSTIT_NOTE_STOWED,	// An OP_PostIt has been stowed, similar to
				// OP_NODE_STOWED
    OP_UI_POSTIT_MOVED,

    OP_UI_REFRESH_EXPOSED,	// Send after an event that could have changed
				// exposed flags in a network. 

    OP_PARM_VISIBLE_CHANGED,	// Parms' visible state changed.
				// -- DATA: parm index

    OP_CUSTOM_DATA_CHANGED,	// Some custom data held on the node changed.
				// -- DATA: node-specific enum indicating
				// exactly which bit of custom data changed
    
    OP_EVENT_TYPE_COUNT	// sentinel

} OP_EventType;

typedef	void	(*OP_EventMethod)(OP_Node *caller, void *callee,
				  OP_EventType type, void *data);

OP_API extern const char *OPeventToString(OP_EventType type);

// UTformat support.
static inline size_t
format(char *buffer, size_t buffer_size, const OP_EventType &v)
{
    UT_WorkBuffer	eventtext;
    eventtext.sprintf("%s (%d)", OPeventToString(v), (int)(v));
    if (!buffer)
	return eventtext.length();
    else
    {
	size_t len = std::min(size_t(eventtext.length()), buffer_size);
	::memcpy(buffer, eventtext.buffer(), len);
	return len;
    }
}
class OP_API OP_Value
{
public:
	     OP_Value(OP_Node *owner);
	    ~OP_Value();

    int		 hasOpInterest	(void *data, OP_EventMethod eventHandler) const;
    void	 addOpInterest	(void *data, OP_EventMethod eventHandler);
    void	 removeOpInterest(void *data, OP_EventMethod eventHandler);
    void	 changed	(OP_EventType type, void *data=0) const;

    unsigned	 numInterests() const;

    int64 getMemoryUsage(bool inclusive) const;

private:
    UT_ValArray<OP_EventHandler *>	 myHandlers;
    OP_Node				*myOwner;
};

#endif
