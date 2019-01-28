/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Input.h (C++)
 *
 * COMMENTS:
 *
 */

#ifndef _OP_Input_h_
#define _OP_Input_h_

#include "OP_API.h"
#include "OP_ConnectorId.h"
#include <UT/UT_Defines.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_Array.h>
#include <iosfwd>
#include <string>

class OP_Node;
class OP_Network;
class OP_IndirectInput;
class OP_NetworkBoxItem;
class UT_JSONParser;
class UT_JSONWriter;
class UT_OptionEntry;
class UT_Options;

#ifdef UT_DEBUG
//#define OP_DEBUG_TRACE_NAME_RESOLUTION
//#define OP_DEBUG_CHECK_INPUT_CONSISTENCY
#endif

class OP_API OP_Input
{
public:
     OP_Input(OP_NetworkBoxItem *owner, OP_ConnectorId new_id);
    ~OP_Input();

    int			 save(std::ostream &os, int bin,
				const char* input_name);
    bool		 load(UT_IStream &is, const char *path,
				std::string *input_name_out);

    // Returns the node that we are an input to.
    OP_NetworkBoxItem	*getOwnerItem() const
			 { return myOwner; }

    // Convert our input string to an actual pointer again
    void		 resolveReference();
    const char		*getNodeReference() const
			 { return myNodeReference.isstring()
				    ? myNodeReference.buffer() : ""; }

    //  Get the Node that this input refers to.  May return NULL
    OP_Node		*getNode();
    unsigned		 getNodeOutputIndex();
    OP_ConnectorId	 getNodeOutputName(void);

    // Dirty the cached output index of the input node.
    void		 dirtyCachedOutputIndex();

    // Check if there is a Node at the other end of the connection.
    // Indirect inputs don't count unless they are actually connected
    // to a node.
    bool		 isConnectedToNode() const;
    // Check if either a node or indirect input (with or without a node
    // connected indirectly) is connected.
    bool		 isConnected() const
			 { return isIndirect() || isConnectedToNode(); }

    bool		 isIndirect() const
			 { return myIndirectFlag; }
    OP_IndirectInput	*getIndirect();

    // Returns the subnet indirect input if there is one, otherwise return
    // the input node.
    OP_NetworkBoxItem	*getInputItem();
    unsigned		 getInputItemOutputIndex();

    int64		 getMemoryUsage(bool inclusive) const;

    void		 setIsVisible(bool value)
			 { myVisibleFlag = value; }
    bool		 getIsVisible() const
			 { return myVisibleFlag; }

    // Use this instead of the assignment operator, which is private (and
    // should not even be called by friend classes).
    void		 copyFrom(OP_Input* other);

    void		 getId(int &id);
    void		 setId(int id);
    OP_ConnectorId	 getId() const
			 { return myId; }

    void		 clearIndirectInput();

    bool		 getPicked() const;
    bool		 getOnPickedPath(bool through_pinned_dots = true) const;
    int			 setPicked(bool picked,
				bool send_change_event = true,
				bool save_undo = true);

private:
    //  Wire the input
    void		 setInput(OP_Node *node, unsigned outputIdx);
    void		 setNamedInput(OP_Node *node,
				const OP_ConnectorId* output_name);
    void		 setInputReference(const char *label,
				unsigned outputIdx);
    void		 setNamedInputReference(const char *label,
				const OP_ConnectorId* output_name);

    //  Take care of the input's forward link when attaching inputs
    void		 setMyNode(OP_Node *input_node, unsigned outputIdx);
    void		 setMyNodeNamed(OP_Node *input_node,
				const OP_ConnectorId* outputName, 
				bool allow_soft_fail = false);

    void		 resolveLegacyOutputIndex(OP_Node* using_node);

    void		 setIndirectInput(OP_IndirectInput *input);

    // Utility functions used by getOnPickedPath.
    bool		 inputPathPicked(bool through_pinned_dots) const;
    bool		 outputPathPicked(bool through_pinned_dots) const;

    // User-specifiable names that can be set on a per-input basis.
    void		 getUserEditableDataKeys(UT_StringArray &keys) const;
    const UT_OptionEntry *getUserEditableData(const UT_StringRef &key) const;
    void		 setUserEditableData(const UT_StringHolder &key,
				UT_OptionEntry *value);
    void		 setUserEditableDataDefault(const UT_Options &options);
    bool		 saveUserEditableData(UT_JSONWriter &w) const;
    bool		 loadUserEditableData(UT_JSONParser &p);

    // Only we are allowed to call our assignment operator.
    OP_Input		&operator=(const OP_Input &other) = default;

    OP_NetworkBoxItem		*myOwner;
    OP_Node			*myNode;
    int				 myUnresolvedLegacyOutputIndex;
    OP_ConnectorId		 myNodeOutputName;

    OP_IndirectInput		*myIndirectInput;
    UT_String			 myNodeReference;
    UT_Options			*myUserEditableData;

    OP_ConnectorId		 myId;

    // Temporary, for transition purposes only. OP_Node::getOutputFromName() is
    // currently very slow for nodes with, say, 2048 outputs. So we cache this
    // for now, but we should really get rid of indices altogether.
    int				 myCachedNodeOutputIdx;

    // Flag indicating we are in the middle of resolving our reference.
    bool			 myResolvingReference;
    // Flag indicating this is an indirect input.
    bool			 myIndirectFlag;
    // Controls the visibility of this input (in VOPs only).
    bool			 myVisibleFlag;
    // Holds whether or not this connection is selected.
    bool			 myPicked;

    friend class		 OP_Node;
    friend class		 OP_Dot;
};

#endif

