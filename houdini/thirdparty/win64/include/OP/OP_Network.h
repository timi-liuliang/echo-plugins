/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP Library (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __OP_Network_h__
#define __OP_Network_h__

#include "OP_API.h"
#include "OP_DotList.h"
#include "OP_GroupList.h"
#include "OP_NetworkBox.h"
#include "OP_NetworkBoxList.h"
#include "OP_Node.h"
#include "OP_Operator.h"
#include "OP_PostIt.h"
#include "OP_PostItNoteList.h"
#include <UT/UT_Array.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Set.h>
#include <UT/UT_String.h>
#include <UT/UT_SymbolTable.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>
#include <set>

class UT_CPIO;
class UT_WorkBuffer;
class UT_StringArray;
class OP_SubnetIndirectInput;
class OP_OperatorTable;
class OP_Director;
class OP_Group;
class OP_UndoLoad;
class OP_ProxyRefNode;
class OP_Dot;
class op_PathSet;
class MOT_Director;

typedef UT_SymbolMap<OP_OperatorTable *> OP_OperatorTableMap;

// The tile type is used to distinguish between the various types of
// nodes that the networks are starting to contain.
enum OP_TileType
{
    OP_TILE_ALL = 0xFF,
    OP_TILE_NODE = 0x01,	// The regular op nodes
    OP_TILE_PARENT = 0x02,	// The parent inputs for subnets
    OP_TILE_GROUP = 0x04,	// The op groups

    // These are for convenience
    OP_TILE_NOT_NODE = (OP_TILE_ALL & ~OP_TILE_NODE),
    OP_TILE_NOT_PARENT = (OP_TILE_ALL & ~OP_TILE_PARENT),
    OP_TILE_NOT_GROUP = (OP_TILE_ALL & ~OP_TILE_GROUP)
};

// If you are looking for the OBJ_TABLE_NAME, SOP_SCRIPT_NAME, and 
// POP_OPTYPE_NAME style #defines that used to be here, they have been moved up
// to OP_Node.h.

enum OP_RenameAction
{
    OP_RENAME_NO_PROPAGATE	    = 0x00, // does no propagation
    OP_RENAME_NORMAL		    = 0x01, // does all propagation
    OP_RENAME_NO_EVENTS		    = 0x02, // does not send op events AND
					    // does not re-export chops
    OP_RENAME_NO_DEPENDENCIES	    = 0x04, // does not notify dependencies

    OP_RENAME_NUM_ACTIONS		    // sentinel
};

struct OP_FixNameParms
{
    OP_FixNameParms()
    {
	replacePrefix	     = 0;
	replaceSuffix	     = 0;
	removeTrailingDigits = 0;
    }

    int		    replacePrefix;
    UT_String	    prefixReplaceStr;
    UT_String	    prefixStr;
    int		    replaceSuffix;
    UT_String	    suffixReplaceStr;
    UT_String	    suffixStr;
    int		    removeTrailingDigits;

private:
    // copy constructors are not implemented yet
    OP_FixNameParms( const OP_FixNameParms &copy );
    OP_FixNameParms &operator =( const OP_FixNameParms &copy );
};

// This class is used to filter OP_Operators from being instantiated
// and shelf tools from appearing in the Tab menu.
class OP_API OP_OperatorFilter
{
    public:
	OP_OperatorFilter()		{ }
	virtual ~OP_OperatorFilter()	{ }

	/// Returns true if an operator type can be created as a child, and 
	/// false otherwise.
	virtual bool allowOperatorAsChild(OP_Operator *)    { return true; }

	/// Returns true if a shelf tool by the given name can appear
	/// in the Tab menu for that (parent) network.
	virtual bool allowTool(const char *tool_name)	    { return true; }
};

typedef void *(OP_Network::*OP_FindItemFunc) (const char *);

class OP_API OP_Network : public OP_Node
{
public:
    static const char 	*getScriptFromTable(const char *table);
    static const char	*getOpTypeFromTable(const char *table);

    // Primary node access methods. Only take OP_Node's into consideration
    virtual int		 getNchildren() const;
    virtual OP_Node	*getChild(const char *name=0, int *hint=0) const;
    virtual OP_Node	*getChild(int index) const;
    virtual int		 getChildIndex(const OP_Node *) const;

    // isNetwork returns if the node can have children.  This is true
    // if the node has an operator table or has any children.
    // This does NOT tell you if it is derived from OP_Network.
    virtual int		 isNetwork() const;
    
    virtual int		 getDandROpsEqual();
    virtual int		 updateDandROpsEqual(int check_inputs = 1);

    // This method a node within the network's UT_ValArray of children
    // dstindex can be any number between 0 and childcount + 1.
    // dstindex refers to the index of the child before which the
    // src node should be moved. Right now, it is only used by OPUI_OpList.
    // As soon as it is used elsewhere, we'll have to add OP_Events for it.
    virtual void	 moveChildIndex(int srcindex, int dstindex);

    // These work the same as getOpType* but return what sort of ops
    // this network usually has as children.  Of course, there's
    // always the chance of heterogenous ops skulking around in there
    // so treat it as a guideline rather than an assertion.
    virtual const char	*getChildType() const;
    virtual OP_OpTypeId	 getChildTypeID() const = 0;

    int 	 	 getTemplateOps(UT_ValArray<OP_Node *> &list);

    // obtains the children that match a given operator name.
    // INPUTS:
    //  op_name - the name of a operator to look for (e.g., "object_merge")
    // OUTPUTS:
    //  list - the list of children that are instances of an operator
    //         that is identified by op_name
    // RETURNS:
    //   number of children in the output list
    int			 getOpsByName(const char *op_name,
				      UT_ValArray<OP_Node *> &list);

    OP_Node		*peekCurrentNodePtr();
    OP_Node		*getCurrentNodePtr();

    void		 stashCurrentNodePtr(const OP_Node *child);
    OP_Node *		 getStashedCurrentNodePtr() const;

    virtual OP_Node	*getDisplayNodePtr();
    virtual OP_Node	*getRenderNodePtr();
    virtual CHOP_Node	*getAudioNodePtr();
    virtual const UT_ValArray<OP_Node *>	&getOutputNodePtrs() const;

    virtual void	 childDeleted(OP_Node *);
    virtual void	 childFlagChange(OP_Node *);

    fpreal		 getChildrenScale() const { return myChildScale; }
    void		 setChildrenScale(fpreal scale);

    // Returns the OP_OperatorFilter object for this network. Used to
    // filter out some OP_Operator types. See OP_OperatorFilter above.
    virtual OP_OperatorFilter	*getOperatorFilter();

    /// Obtains the namespace scope names for this network and for its parents,
    /// all aranged in a stack array, with this network's name first in the
    /// array, and the node hierarchy root (director) appearing last.
    /// By convention, the scope name is the same as the network operator name
    /// (with the table name).
    void		 getNetworkScopeStack(UT_StringArray &scope_stack);

    /// Returns true if the given op can be created as a child of this network.
    bool		 isOperatorAllowedAsChild(OP_Operator *op);

    /// Returns true if the given tool can appear in the Tab menu for this net.
    bool		 isToolAllowed(const char *tool_name);

    /// Adds a new OP_Node of the specified type as a child of this node.
    /// If 'name' is not given, then it will use 'type' suffixed with a unique
    /// numeric suffix. In either case, if the 'name' collides with a
    /// pre-existing node, then the actual node will be created with a unique
    /// name created by numeric suffixing.
    /// @param	exact_type   If true, the operator name parameter 'type',
    ///			is used verbatim to lookup the operator. Otherwise,
    ///			a preferred operator name that matches 'type'
    ///			specification is found first and then is used to lookup
    ///			the operator. The preferred type may be different than
    ///			'type' for version namespace (eg "hda" 
    ///			may match the preferred "hda::2.0" operator), for
    ///			scope namespace (eg "hda" may match
    ///			"Object/outer::hda" inside "outer" subnet), or for
    ///			operator namespace (eg, "userX::hda" may be preferred
    ///			to "hda", if the namespace hierarchy is so configured
    ///			that "userX" namespace trumps the top level (global)
    ///			namespace).
    OP_Node		*createNode(const char *type, const char *name = 0,
				    int notify = 1, int explicitly = 1,
				    int loadcontents = 1,
				    int *aliasedScriptedOp = 0,
				    char *matIconFilename = 0,
				    bool exact_type = false);

    /// Convenience method for requesting en exact type
    OP_Node		*createNodeOfExactType(const char *type, 
					       const char *name = 0)
			 {
			     return createNode(type, name, 1, 1, 1, 0, 0, true);
			 }

    // Create dummy reference proxy nodes for dangling references
    // This should only be called from OP_Node!
    virtual OP_Network	*createProxyRefNode( const char *path );

    // Makes a copy of a single node inside this network. The source node does
    // not need to be inside this network.
    OP_Node		*copyNode(const OP_Node *srcnode,
				const char *toname = NULL,
				bool reference_parameters = false);

    // Makes a copy of any number of network items inside this network. The
    // source items must all be contained in a single network, but it does
    // not need to be this network.
    bool		 copyItems(const OP_NetworkBoxItemList &srcitems,
				OP_NetworkBoxItemList &destitems,
				bool reference_parameters,
				bool relative_references,
				bool connect_outputs_to_multi_inputs);
    
    // Makes a copy of all picked network items into this network. The source
    // network does not need to be this network.
    void		 copyPickedItems(
				const OP_Network *srcnet, 
				OP_ItemTypeMask item_type_mask,
				OP_Group *newnodes = nullptr, 
				const OP_FixNameParms *fix_name_parms=nullptr);

    // Copy 'srcnetbox' and its contents to this network. 'toname' specifies
    // the name you'd like the copy to have. If this name has already been
    // taken in this network, we'll use a slightly altered unique version of
    // it.  'nodes' may only be non-null when 'save_type' is
    // OP_NETWORKBOX_SAVE_SPECIFIED_CONTENTS, in which case it must be a valid
    // ptr array of nodes in 'netbox'.
    OP_NetworkBox	*copyNetworkBox(OP_NetworkBox *srcnetbox,
				OP_NetworkBoxSaveType save_type,
				OP_NodeList *nodes = NULL, 
				const char *toname = NULL,
				bool reference_parameters = false);

    // Copy a single post-it note into this network. The source post-it does
    // not need to be in this network.
    OP_PostIt		*copyPostItNote(OP_PostIt *src_note,
				const char *toname = 0);

    // Makes a copy of all picked nodes into this network. The source network
    // does not need to be this network.
    void		 copyPickedNodes(const OP_Network *srcnet, 
				OP_Group *newnodes=0, 
				const OP_FixNameParms *fix_name_parms=0);

    // Moves the picked nodes from srcnet into this network.
    // If src_nodes_ptr and dest_nodes_ptr are not null, they will be set
    // to the original node pointers and the new node pointers.  (The nodes
    // in src_nodes_ptr will have been deleted.)
    virtual bool	 movePickedNodes(OP_Network *src_net,
				OP_NodeList *src_nodes_ptr=NULL,
				OP_NodeList *dest_nodes_ptr=NULL);

    /// Returns true if it is safe at this time to destroy this node.
    static bool		 canDestroyNode(OP_Node *node);

    int			 destroyNode(OP_Node *node);
    int			 destroyNodes(const OP_NodeList &nodes);

    virtual int		 renameNode(OP_Node *node, const char *name,
				OP_RenameAction action = OP_RENAME_NORMAL);

    /// Obtains the CHOP network to store motion effects. If create is true,
    /// then it will be created if it doesn't exist yet.
    OP_Network		*findOrCreateMotionEffectsNetwork(bool create=true);

    // Delete implicit ancestors. If included is true delete the initial given
    // node as well.  If a state name is passed in, check that the nodes were
    // created by that state. The node is not deleted if any of its outputs do
    // not match the given state. This function will have no effect on nodes 
    // not belonging to this network, or on subnetworks. The deletion will 
    // stop at the first invalid node (if keep_node points to anything, that
    // node is invalid for deletion).
    OP_Node		*deleteImplicitAncestors(OP_Node &node, 
				const char *state = 0, OP_Node *keep_node = 0,
				bool included = true);
    // non-destructive version of deleteImplicitAncestors() that just returns
    // the information.
    OP_Node		*getImplicitAncestors(
				OP_NodeList &implicit_nodes,
				OP_NodeList &border_nodes,
				OP_Node &node, 
				const char *state = 0, OP_Node *keep_node = 0,
				bool included = true);

    // Fetch all channel collections from this node down.
    int		 getAllCollections(UT_ValArray<CH_Collection *> &list,
				   int hide=0);

    /// Build a unique node name or a unique operator name from
    /// the one given. We'll pretend we also have children with the names in
    /// 'fakeChildren' when we do our name-collision checking if 'fakeChildren'
    /// is non-NULL.
    void	 uniqueNodeName(UT_String &name,
				UT_StringSet *fakeChildren = 0);
    
    // This function is used to make some extra names invalid. Returns 0
    // by default. Used by the VOPNET manager to prevent a new VOPNET
    // with a name that matches an existing operator type.
    virtual int	 isInvalidName(const char *name);

    static void	 fixName( const char *name, 
			  const OP_FixNameParms &fix_name_parms, 
			  UT_String &fixed_name );

    // I/O methods - there are also the save & load inherited from the
    //	OP_Node class.
    
    ///Save all picked items of the types specified by 'item_type'. Note that
    ///saving any nodes or netboxes implicitly always saves indirect inputs if 
    ///they're picked (legacy behaviour). If 'item_type' includes netboxes, you 
    ///can specify the save policy for netboxes regarding how much of their 
    ///contents they should be saved along with themselves. This is the only way
    ///for a node to remember which netbox it was in.
    OP_ERROR	 savePickedItems(std::ostream &os,
			const OP_SaveFlags &flags, 
			OP_ItemTypeMask item_type,
			OP_NetworkBoxSaveType save_type =
			    OP_NETWORKBOX_SAVE_ALL_CONTENTS);
    
    ///Save all items of the types specified by 'item_type'. Note that
    ///saving any nodes or netboxes implicitly always saves indirect inputs if 
    ///they're picked or part of a picked netbox (legacy behaviour).
    OP_ERROR	 saveAllItems(std::ostream &os,
			const OP_SaveFlags &flags, 
			OP_ItemTypeMask item_type);
    
    // Saves a single node with the same save options on the child nodes
    // as on the top level node, with the exception of network box membership
    // for children (which should always be saved)
    OP_ERROR	 saveSingle(std::ostream &os, OP_Node *op,
			const OP_SaveFlags &flags,
			const UT_String &name_override = UT_String());
    // Saves a single node with different save options for the child nodes
    // compared to the top level node.
    OP_ERROR	 saveSingle(std::ostream &os, OP_Node *op,
			const OP_SaveFlags &flags,
			const OP_SaveFlags &childflags,
			std::set<int>* renaming_exclude_ids = NULL,
			const UT_String &name_override = UT_String());

    ///Saves a single netbox. 'save_type' lets you specify the save policy 
    ///with regards to how much of its contents the network box saves with it. 
    ///'nodes' may only be non-null when 'save_type' is 
    ///OP_NETWORKBOX_SAVE_SPECIFIED_CONTENTS, in which case it must be a valid
    ///ptr array of nodes in 'netbox'.
    OP_ERROR	 saveSingleNetworkBox(std::ostream &os,
			OP_NetworkBox &netbox,
			const OP_SaveFlags &flags, 
			OP_NetworkBoxSaveType save_type,
			OP_NodeList *nodes = NULL);

    /// Save a single post it note.
    OP_ERROR	 saveSinglePostIt(std::ostream &os, OP_PostIt &pnote,
			const OP_SaveFlags &flags);

    /// Save a single dot.
    OP_ERROR	 saveSingleDot(std::ostream &os, OP_Dot &dot,
			const OP_SaveFlags &flags);

    ///In order to avoid duplicately saving nodes in 'op_list' that have already
    ///been saved by their netboxes, make sure to mark those netboxes and then
    ///pass in 'skip_nodes_in_marked_netboxes' as true.
    OP_ERROR	 saveOpList(std::ostream &os,
			const UT_ValArray<OP_Node *> &op_list,
			const OP_SaveFlags &save_flags, 
			int *nsaved = NULL);

    //Use when saving a mixture of different types of items into 1 file
    OP_ERROR	 saveItemList(std::ostream &os,
			const OP_NetworkBoxItemList &items,
			const OP_SaveFlags &save_flags);

    const char	*getClipboardFile(const char *file_prefix = 0);

    //Note that although we now have an OP_ItemTypeMask parameter available to
    //us, we still need to keep the OP_TileType parameters to indicate saving
    //of groups *boo*
    int		 saveToClipboard(int all, OP_TileType type=OP_TILE_NODE,
			int grouplist=0, 
			OP_ItemTypeMask item_type = OP_ITEMTYPE_NODE,
			const char *file_prefix = 0);
    int		 loadFromClipboard(fpreal x = 0, fpreal y = 0,
			bool turn_off_display = false,
			const char *file_prefix = 0);

    // loadNetwork will load nodes into the current net.  For example,
    // if there file is "foo.sops", you would call obj->loadNetwork().
    // if the file were "foo.obj", you would call obj->getParent()->loadNetwork
    bool	 loadNetwork(UT_IStream &is,
			int merge = 0,
			const char *pattern = NULL,
			int overwrite = 0,
			const OP_FixNameParms *fix_name_parms = NULL,
			UT_StringArray *src_names = NULL,
			UT_StringArray *dest_names = NULL);

    bool	 pasteNetworkForMove(UT_IStream &is,
			UT_StringArray &src_names,
			UT_StringArray &dest_names);

    virtual bool opShouldHandleChange(OP_EventType reason);

    static bool	 isOpWritten(UT_IStream &is);

    virtual void clearUnreferencedInputs();

    ///  Check a stream for collisions for loading...  This will add the
    ///	problem nodes to 'node_list' and the problem network boxes to 
    ///	'netbox_list', and also return the total number of collisions found...  
    ///	It rewinds the stream to the starting point, thus, it can't be used with
    ///	stdin! Remember that a node can collide with a network box, since they
    /// share the same namespace!
    int		 getLoadCollisions(UT_IStream &is, OP_NodeList &node_list, 
				   UT_ValArray<OP_NetworkBox *> &netbox_list,
				   UT_ValArray<OP_PostIt *> &pnote_list,
				   const char *pattern = "*");

    /// Simulates adding items with the names in 'names' to this network, and
    /// stores the resulting names after name-collisions have been addressed 
    /// back into 'names'. This works for anything sharing the node namespace, 
    /// which currently includes nodes and network boxes.
    void	 simulateNameCollisions(UT_StringArray &names);

    // The getSortedList() returns an array of all the child nodes sorted
    // alphabetically.
    const UT_ValArray<OP_Node *>	*getSortedList();

    // getAnyOutputOf will return the first OP found which is has the
    // given OP as an input. Returns null if no such op exists.
    OP_Node	*getAnyOutputOf(OP_Node *op) const;

    // getAnyOP will return any OP in the collection.
    // Null if there are none.
    OP_Node	*getAnyOP() const;

    // Group manipulation methods:
    char	*findUniqueGroupName(const char *basename=0);
    OP_Group	*   findGroup(const char *name) const;
    OP_Group	* createGroup(const char *name=0);
    int		 destroyGroup(const char *name);
    int		 destroyGroup(OP_Group *group);

    int		 getNgroups() const;
    OP_Group	*getGroup(int index) const;
    int		 getGroups(UT_ValArray<OP_Group *> &list, int internal=0);
    int		 getMemberships(const OP_Node *node,
    				UT_ValArray<OP_Group *> &list,
				int internal=0);

    char 	*findUniqueNetworkBoxName(const char *base = NULL);
    char 	*findUniquePostItNoteName(const char *base = NULL);
    char 	*findUniqueDotName(const char *base = NULL);

    /// Pass in NULL as the name if you'd like a default name. Passing in
    /// 'items' will automatically create a network box containing those items
    /// with the netbox positioned properly around them
    OP_NetworkBox	*createNetworkBox(const char *name, 
				OP_NetworkBoxItemList *items = NULL);
    int		 	 getNnetworkBoxes() const;
    // Given a path, return the network box. The path may be relative.
    virtual OP_NetworkBox *findNetworkBox(const char *const_path);
    // Unlike 'findNetworkBox()', these 2 methods only return netboxes 
    // contained directly in this network itself.
    OP_NetworkBox	*getNetworkBox(int index) const;
    OP_NetworkBox	*getNetworkBox(const char *name) const;

    /// Pass in NULL as the name if you'd like a default name. 
    OP_PostIt		*createPostItNote(const char *name);
    int		 	 getNpostItNotes() const;
    // Given a path, return the post it note. The path may be relative.
    virtual OP_PostIt	*findPostItNote(const char *const_path);
    // Unlike 'findPostItNote()', these 2 methods only return postits 
    // contained directly in this network itself.
    OP_PostIt		*getPostItNote(int index) const;
    OP_PostIt		*getPostItNote(const char *name) const;

    // Dots are not meant to be referenced by name.
    OP_DotList		&getDotList()
			 { return myDotList; }
    int			 getNdots() const;
    // Given a path, return the post it note. The path may be relative.
    virtual OP_Dot	*findDot(const char *const_path);
    OP_Dot		*getDot(int index) const;
    OP_Dot		*getDot(const char *name) const;

    /// Return the number of items picked. 'item_type_mask' is a bit field used 
    /// to specify which item types you'd liked to include in the count. Use the
    /// values in OP_ItemType to set the bits in 'item_type_mask'. 
    unsigned int nPicked(OP_ItemTypeMask item_type_mask,
			 bool include_hidden_nodes) const;

    /// Return all picked items of types specified by 'item_type_mask'. 
    /// Note that if only picked nodes are of interest, there is also
    /// OP_Node::getPickedChildren().
    void	 getPickedItems(OP_ItemTypeMask item_type_mask, 
			OP_NetworkBoxItemList &picked, 
			bool include_hidden = false,
			bool recurse_picked_netboxes = false) const;

    /// Return all items that match the type mask.
    void	 getAllItems(OP_ItemTypeMask item_type_mask, 
			OP_NetworkBoxItemList &items) const;

    /// Returns true if the item was successfully deleted
    bool	 destroyItem(OP_NetworkBoxItem *item);
    /// Returns the number of successfully deleted items
    int		 destroyItems(const OP_NetworkBoxItemList &items);

    /// Same as findNode or findNetworkBox, but more generalized so you can 
    /// search through multiple items with just one method. 'item_type_mask' is
    /// used to specify what type of items you want to consider in your search.
    /// Does not support indirect parent input searches.
    virtual OP_NetworkBoxItem *findItem(const char *const_path, 
					OP_ItemTypeMask item_type = 
					OP_ITEMTYPE_ALL);

    /// Look up a direct child item by name, with an option to screen based
    /// on the item type. Like findItem above, but doesn't look into child
    /// networks.
    OP_NetworkBoxItem	*getItem(const char *name,
				OP_ItemTypeMask item_type = OP_ITEMTYPE_ALL);

    /// Returns true if the given node is a child descendent of the same
    /// type with no intervening levels of different type.
    bool	 isSubNode(const OP_Node *node) const;

    /// Try to avoid using this method, it only clears the selection for tiles,
    /// not network boxes (network boxes are considered items but not tiles). 
    /// Can use setAllPickFlags() instead. In fact, since there are no group 
    /// tiles in the worksheet any more, this method has become somewhat useless
    void	 clearSelection(OP_TileType t, int grouplist);

    ///Set the pick flags to 'value' for all items of types specified by 
    ///'item_type'
    void	 setAllPickFlags(OP_ItemTypeMask item_type, int value);
    void	 pickRequest(OP_NetworkBoxItem *item, int shift);
    void	 pickRequest(int shift)
		 { OP_Node::pickRequest(shift); }

    void	 addPicked(const OP_Group *group);	// Pick these nodes
    void	 subPicked(const OP_Group *group);	// Removed from picked

    // There are two versions of getOperatorTable. One that takes an
    // operator type and returns the table for that type. The second
    // returns the operator table associated with this network. This
    // latter assumes that "setOperatorTable" has been called. All
    // OP_Network sub-class constructors should do this:
    //
    //	    setOperatorTable(getOperatorTable(operatorType));
    //
    // If the script_path is NOT null, this method will create the
    // operator table if it doesn't exist.  Otherwise, it will return
    // null for non-existant table names.
    static OP_OperatorTable	*getOperatorTable(
				    const UT_StringRef &op_type,
				    const char *script_path=0);
    OP_OperatorTable		*getOperatorTable() const
						  { return myOperatorTable; }
    virtual const char		*getScriptDirectory();

    // this is overridden so that we can clear the group flags
    virtual void		 clearUndoFlags();

    // Sub-networks use parent inputs to connect open inputs in
    // the net to inputs in the parent. A list of the parent connections
    // is the bookeeping required to manage and resolve these connections.
    bool			 wantsParentInputs() const;

    // Get all picked Parent Inputs
    int			 getNparentInputs() const
			 { return myParentInputs.entries(); }
    virtual OP_SubnetIndirectInput *findParentInput(const char *const_path);
    OP_SubnetIndirectInput *getParentInput(int index) const;
    OP_SubnetIndirectInput *getParentInput(const char *name) const;

    // getIndirectString returns a string that can be used to save
    // parent inputs that refer to our parent input table.
    // These are simply saved as a string index into our table.
    // These can be resolved back using resolveIndirectString.
    int			 getIndirectString(OP_IndirectInput *, UT_String &);
    OP_IndirectInput	*resolveIndirectString(const char *);

    virtual void	 inputConnectChanged(int which);

    void		 getAllConnectedChildren(
				int input_index, UT_Array<OP_Node*> &nodes,
				bool expandSubnets = true) const;
    void		 getAllConnectedChildrenNamed(
				const OP_ConnectorId& input_name,
				UT_Array<OP_Node*> &nodes,
				bool expandSubnets = true) const;

    // The ViewSetting functions are used by ICE to store the name
    // of the COP currently being viewed. Put here because COP_SubNets
    // and ICENET_Nodes both need to store this information.
    virtual void	 setViewSetting(const char *) {};
    virtual const char	*getViewSetting() const { return ""; }

    virtual void	 dumpDependencies(std::ostream &os);

    virtual void	 changeParmTemplate(PRM_Template *new_template);

    int			 getNumChildManagers(OP_OpTypeId type) const;
    void		 addChildManager(OP_OpTypeId type);
    void		 removeChildManager(OP_OpTypeId type);
    virtual bool	 getSaveWithVopnets();
    virtual void	 runDelScript();
    void		 clearDelScripts(int pickedOnly);

    /// This method will unscope all the channels belonging to this op.
    /// If the recurse flag is set it will recurse through the children
    /// (at this level the recurse flag is ignored).
    virtual void	 unscopeChannels(bool recurse);

    /// This method will undisplay all the channels belonging to this op.
    /// If the recurse flag is set it will recurse through the children
    /// (at this level the recurse flag is ignored).
    virtual void	 undisplayChannels(bool recurse);

    /// This method will undisplay all the channels belonging to this op.
    /// If the recurse flag is set it will recurse through the children
    /// (at this level the recurse flag is ignored).
    virtual void	 unpinChannels(bool recurse);

    // Do global expansion on a pattern - get lists of network boxes
    // If expand is given, concatenated with full list of names
    // If list is given, appended with list of nodes.
    virtual void	 globNetworkBoxes(const char *pat, UT_String *expand,
					  UT_ValArray<OP_NetworkBox *> *list=0,
					  OP_GlobContext *context = 0,
					  const char *prefix = " ");

    virtual void	 globPostIts(const char *pat, UT_String *expand,
					  UT_ValArray<OP_PostIt *> *list=0,
					  OP_GlobContext *context = 0,
					  const char *prefix = " ");

    ///Save out all the netboxes in this network to 'os' in separate packets
    OP_ERROR	 saveNetworkBoxes(std::ostream &os, const OP_SaveFlags &flags, 
				  const char *path_prefix);
    OP_ERROR	 savePostIts(std::ostream &os, const OP_SaveFlags &flags, 
				  const char *path_prefix);
    OP_ERROR	 saveDots(std::ostream &os, const OP_SaveFlags &flags, 
				  const char *path_prefix);

    ///Load a network box from 'is'. Can load from both .hip files and the 
    ///clipboard. 'path' is used as the new netbox's name. If 'overwrite' is
    ///specified, the new netbox will overwrite any other netboxes in this
    ///network with the same name, rather than modify its name to be unique.
    ///'created' is used to return a ptr to the new network box.
    bool	 loadNetworkBox(UT_IStream &is, const char *path = "",
		    bool path_contains_network = true,
		    bool path_contains_packet_ext = true,
		    int overwrite = 0, OP_NetworkBox **created = NULL);

    /// Load a post-it note from 'is'. Can load from both .hip files and the 
    /// clipboard. 'path' is used as the new post-it's name. If 'overwrite' is
    /// specified, the new post-it will overwrite any other post-its in this
    /// network with the same name, rather than modify its name to be unique.
    /// 'created' is used to return a ptr to the new post-it.
    bool	 loadPostIt(UT_IStream &is, const char *path = "",
		    bool path_contains_network = true,
		    bool path_contains_packet_ext = true,
		    int overwrite = 0, OP_PostIt **created = NULL);

    /// Load a network dot from 'is'. Can load from both .hip files and the 
    /// clipboard. 'path' is used as the new dot's name. If 'overwrite' is
    /// specified, the new dot will overwrite any other dots in this
    /// network with the same name, rather than modify its name to be unique.
    /// 'created' is used to return a ptr to the new post-it.
    bool	 loadDot(UT_IStream &is, const char *path = "",
		    bool path_contains_network = true,
		    bool path_contains_packet_ext = true,
		    int overwrite = 0, OP_Dot **created = NULL);

    /// Returns the unique id of the representative operator that this network
    /// may have.  The changed_child parameter and the current_rep_id are used
    /// in order to minimize the number of findSubNode() calls that are made.
    int		         getRepresentativeNodeId(OP_Node *changed_child,
						 int current_rep_id);

    // Changes a child node's type from its current type to 'newtype'
    // the keep... parameters will maintain the same name, pararmeter values
    // and network contents if set to true.
    bool		 changeChildNodeType(OP_Node *child,
					     const char *newtype,
					     bool keepname,
					     bool keepparms,
					     bool keepcontents,
					     bool quiet,
					     int *newChildId=NULL);

    // Returns true if the supplied node is specially marked as editable
    // in this locked HDA node.
    bool		 getIsSubNodeEditable(const OP_Node *subnode) const;
    bool		 hasAnyEditableSubNodes() const;

    /// Builds the list of subnodes that are marked as message nodes.
    /// Returns false if there are none (list.entries() == 0 then)
    bool		 getMessageSubNodes(OP_NodeList &list) const;

    // Add any OTL-defined operators used by this network to the specified
    // table.
    virtual void	 getActiveOperatorsDefinedByOTL(
				UT_Set<OP_Operator *> &active_operators) const;

    // Save a packet containing the fallback library paths for the specified
    // operators.
    static void		 saveOTLBackupInformation(std::ostream &os,
					const OP_OperatorList &fallbackops,
					const OP_OperatorList &dummyops);

    OP_SubnetIndirectInput *getNamedParentInput(
				const OP_ConnectorId& name) const;
    bool		 getIsClearingContentsOfEditableSubnodes()
			 { return myIsClearingContentsOfEditableSubnodes; }

    virtual int64	 getMemoryUsage(bool inclusive) const;
    
    static bool 	 warnDeprecatedOpsOnLoad() 
			 { return theWarnDeprecatedOpsOnLoad; }
    static void 	 setWarnDeprecatedOpsOnLoad(bool enable)
			 { theWarnDeprecatedOpsOnLoad = enable; }

    static const char	*theChildTableName;

    void		 setCachedMimeFile(const char *filename);
    const char		*getCachedMimeFile() const;
    void		 clearCachedMimeFile();
    bool		 hasCachedMimeFile() const;

    virtual OP_ERROR	 getErrorSeverity();
    virtual void	 getRawErrors(UT_Array<UT_Error> &errors, bool update);

protected:
    // Protected constructor since networks are only created by Operators.
	     OP_Network(OP_Network *parent, const char *name, OP_Operator *op);
    virtual ~OP_Network();

    virtual void	 clearAndDestroy(void);
	    void	 clearAndDestroyNodes(void);

    virtual int		 propagateModification(OP_Node *by_whom,
					       OP_EventType reason,
					       int parm_index,
					       OP_PropagateData &prop_data);

    // new method for dependencies: use the PRM_TYPE_OP_REF_*
    //	in your parm template to add your dependency.
    // override this to do special cases, make sure you call the base class
    virtual void	 buildOpDependencies();
    virtual void	 notifyRenameDependents( const UT_String &full_from,
						 OP_NodeList &cook_nodes );
    virtual void	 notifyRenameReferences( const UT_String &full_from,
						 OP_NodeList &cook_nodes );
    virtual void	 rebuildOpDependents( bool proxy_only );

    // clone dependencies from the proxy. proxy is no longer valid
    // after the clone!
    virtual void	 cloneFromProxyRefNode( OP_Network *proxy );
    // clone a new proxy node. this node is no longer valid after this!
    virtual OP_Network * cloneToProxyRefNode();
    virtual int		 hasProxyRefNodes() const
			    { return myProxyRefNodes.entries() > 0; }
    void		 removeProxyRefNode( OP_Network *proxy );
    virtual void	 moveProxyRefNodes( OP_Network *dest );

    virtual void	 moveDependencies( OP_Node *from_node );

    // Override of OP_Node::load to handle some network-specific packets.
    virtual bool	 load(UT_IStream &is, const char *ext = "",
					      const char *path = 0);

    /// Called upon entering and exiting loadNetwork() method
    virtual void	 beginLoadingInNetwork();
    virtual void	 endLoadingInNetwork();

    // Called at the end of loadChildOrder()
    virtual void	 finishedLoadingChildOrder();

    // Called when loadNetwork finishes loading this whole network.
    // Do not set `is_child_call`.  That argument is set internally to
    // true when finishedLoadingNetwork() is called on the node's children.
    virtual void	 finishedLoadingNetwork(bool is_child_call=false);

    // This overrides the OP_Node function.
    // syncContents loads the given stream into this operator.
    // The stream should be in .hip file format, with "this" pointing to
    // the root node in the .hip file. It handles collisions gracefully
    // by updating the existing operators instead of deleting and
    // recreating them. It deletes any existing nodes not in the
    // stream, and creates nodes if necessary.
    virtual bool	 syncContents(UT_IStream &is);

    // The following should only be called by the director, who doesn't have
    // a real choice, does he?
    virtual void	 addNode(OP_Node *node, int notify=1, int explicitly=1);

    OP_Node		*findConnectedNode(OP_Node *op) const;

    // These functions ensure we have valid render and display nodes.
    OP_Node		*getPreferredDisplayNodePtr(OP_Node *skipthisnode)const;
    void		 resetDisplayNodePtr(OP_Node *skipthisnode=0);
    OP_Node		*getPreferredRenderNodePtr(OP_Node *skipthisnode) const;
    void		 resetRenderNodePtr(OP_Node *skipthisnode=0);
    OP_Node		*getPreferredAudioNodePtr(OP_Node *skipthisnode) const;
    void		 resetAudioNodePtr(OP_Node *skipthisnode=0);
    virtual CHOP_Node  **getAudioNodeRef();

    // This function calls all of the above, for when we are deleting nodes.
    void		 validateStatusNodePtrs(const OP_NodeList &skipnodes);

    virtual void	 updateExtraFlagPtrs();

    virtual void	 nodeUnlocked();

    virtual bool	 canDestroyNode();
    virtual void	 destroySingleNode(OP_Node *node);

    // This function goes recursively through every node below this one and
    // renames it to some unique but meaningless value. This is used for
    // compiling networks.
    void		 modifyAllChildNodeNames(bool renamepickableobjects,
				const char *prefix, int &id,
				std::set<int>* renaming_exclude_ids = NULL);

    /// This is a helper method to the setChannelScope method.  It traverses
    /// the node's parameters and sets the scope on them according to the
    /// given pattern.
    /// This method is also defined in OP_Parameters, it is overridden here
    /// to provide parsing support for network paths.
    virtual void	 traverseScope(const char *pattern, 
				OP_ScopeOp scope_op,
				const OP_ScopeOptions &scope_opts);


    OP_Node			*myRenderNodePtr;
    OP_Node			*myDisplayNodePtr;
    UT_ValArray<OP_Node *>	 myOutputNodes;

    // Implemented in VOPs. Only the new nodes are to be inserted into the out
    // list, not any of the existing ones.
    virtual void	getAdditionalUndoNodes(const OP_NodeList& orig_list, 
				OP_NodeList& nodes_for_input_undo) { }

    /// Returns true if the child at node can should be saved, false otherwise. 
    virtual bool	getAllowSavingChild(OP_Node *node,
				const OP_SaveFlags &flags)
			{ return true; }

private:
    // Some helper functions for loading and saving.
    OP_ERROR	 saveNetwork(std::ostream &os, const OP_SaveFlags &flags,
			const OP_SaveFlags &childflags,
			const char *pathPrefix, bool vopnets);
    OP_ERROR	 saveChildNodes(std::ostream &os, const OP_SaveFlags &flags,
			const char *pathPrefix, bool vopnets,
			bool forceNoInit = false);
    OP_ERROR	 saveOp(std::ostream &os, OP_Node *node,
			const OP_SaveFlags &flags,
			const OP_SaveFlags &childflags, const char *pathPrefix,
			bool vopnets, bool forceNoInit = false,
			const UT_String &name_override = UT_String());
    OP_ERROR	 savePostIt(std::ostream &os, OP_PostIt *pnote,
			const OP_SaveFlags &flags,
			const char *pathPrefix);
    bool	 loadNodeType(UT_IStream &is, OP_Network *net,
				UT_String &nodeType, int &matches);
    bool	 loadOp(UT_IStream &is, int overwrite, const char *path,
			op_PathSet &noneditable_missing_table);

    // Some special handling for items loaded into a network due to copy/paste
    // or undo/redo. We need to register these items with an OP_UndoLoad, and
    // we may want to pick them.
    void	 beginPickLoaded(bool pick_loaded);
    void	 beginLoadUndo(OP_Node *save_ops[4]);
    void	 itemLoaded(OP_NetworkBoxItem *item);
    void	 endLoadUndo(OP_Node *save_ops[4]);
    void	 endPickLoaded();

    // Some helper functions for syncContents.
    bool	 syncContentsInit(UT_IStream &is,
				OP_Network *net, OP_Node *node,
				UT_String &nodeName, OP_Node *&newnode,
				bool &match_definition_failure,
				const char *packet_node_path);
    // Helper method to clear the netboxes/post-its from any subnodes of the
    // given network whose netboxes may be saved to *this network's contents
    // section without a preceding .init packet for that subnode.
    void	 syncContentsClearNetboxesFromNonInitSubNodes(OP_Network *net);
    // Helper method to clear the contents of any editable subnodes which will
    // be loaded from the hip file or a parent asset's contents.
    void	 clearContentsOfEditableSubNodes();
    // Helper method to determine if an ancestor is an instance of a particular
    // operator currently loading its contents section.
    bool	 isLoadingContentsSection(OP_Operator *op) const;

    void	 setDoMergeFlag( bool flag )	{ myDoMerge = flag; }

    // These functions perform operations that are required after loading
    // a number of new operators. Used by loadNetwork and syncContents.
    void	 resolveNetworkBoxOwnership();
    void	 initSpecialNodes(OP_NodeList &loaded_ops);
    void	 initNodeConnectors(OP_NodeList &loaded_ops);
    void	 sendBulkNotifications(const OP_NodeList &loaded_ops,
				       const OP_NodeList &op_roots,
				       bool builddependencies,
				       bool notify_name_dependents);
    void	 fixPendingOverride();

    int		 savePartialVerify(std::ostream &os);
    void	 createParentInputs();

    // Build a list of item roots from the stream. In English, this means that
    // 'item_roots' will be populated with the paths of all items at the top 
    // level of this load. Initial stream position is unchanged.
    bool	 buildItemRoots(UT_IStream &is, const char *mergePattern,
           			UT_StringArray &item_roots);
    
    // Destroys all child nodes except those that appear in the supplied
    // list (and their parents). Traverses into writable child nodes too.
    // Returns the number of descendents found in nodes.
    void	 destroyAllNodesExcept(const UT_Set<OP_Node *> &nodes);

    // Then after the list is made, we go through and rename the original
    // items so there won't be any collisions on loading
    void	 prepareCollisionNames(const OP_NetworkBoxItemList &items,
				       UT_WorkBuffer &prefix);
    // Then, after the load, we go through and rename the items to their
    //	original status (renaming the collisions along the way)
    void	 fixLoadInputs();		// Resolves references of nodes
    void	 fixLoadNames(const OP_NetworkBoxItemList &items,
			      UT_WorkBuffer &prefix,
			      const OP_FixNameParms *fix_name_parms,
			      UT_StringArray *dest_names);

    int		 groupCount() const;	// Doesn't count internal groups.
    OP_ERROR	 saveGroupList(std::ostream &os, int binary = 0);
    bool	 loadGroupList(UT_IStream &is, const char *path="");
    OP_ERROR	 saveParentInputs(std::ostream &os, int picked= 0);
    bool	 loadParentInputs(UT_IStream &is, const char *path = "");
    OP_ERROR	 saveNetworkData(std::ostream &os, int binary = 0,
			int picked= 0);
    bool	 loadNetworkData(UT_IStream &is, const char *path = "");
    OP_ERROR	 saveChildOrder(std::ostream &os, const OP_SaveFlags &flags);
    bool	 loadChildOrder(UT_IStream &is);
    OP_ERROR	 saveCompiledSection(std::ostream &os, int binary = 0,
			int picked= 0);
    bool	 loadCompiledSection(UT_IStream &is, const char *path = "");

    // these should surround saving of ops within a network. 
    void	 prepareOpSave(std::ostream &os, const OP_SaveFlags &flags,
			       int &nsaved, int &writeTrailer);
    void	 cleanupOpSave(std::ostream &os, const OP_SaveFlags &flags,
			       int &nsaved, int &writeTrailer);

    // A helper function for findUniqueGroupName and findUniqueNetworkBoxName
    // and findUniqueDotName which takes the "does this item exist" function as
    // a parameter
    char 	*findUniqueName(const char *base, OP_FindItemFunc find);

    void	 changeNodeIdFromLoad(OP_Node &node, int new_id);
    void	 invokeFinishLoadingNetwork(OP_NodeList& op_roots);

    bool	 verifyParents();
    static bool	 verifyAllParents();

    void	 setOperatorTable(OP_OperatorTable *table);

    UT_SymbolMap<OP_Node *>		 mySymbols;
    UT_ValArray<OP_Node *>		 myNodes;
    UT_ValArray<OP_Network *>		 myProxyRefNodes;
    UT_ValArray<OP_Node *>		 mySortedList;
    UT_ValArray<OP_SubnetIndirectInput*> myParentInputs;
    OP_GroupList			 myGroups;
    OP_NetworkBoxList			 myNetworkBoxes;
    OP_PostItNoteList			 myPostItNotes;
    OP_DotList				 myDotList;
    OP_OperatorTable			*myOperatorTable;
    OP_Node				*myLastLoadedOpPtr;
    OP_NetworkBox			*myLastLoadedNetworkBox;
    OP_PostIt				*myLastLoadedPostIt;
    OP_Dot				*myLastLoadedDot;
    OP_Node				*myCreatedNode;
    UT_SortedSet<int>			 myEditableSubNodes;
    OP_UndoLoad				*myUndoLoad;
    bool				 myPickLoaded;

    fpreal				 myChildScale;
    int					 myDandRCounter;
    short				 myDandROpsEqual;
    char				 myNameSortDirty;
    char				 myOpLoadedFlag;
    bool				 myDoMerge;
    UT_StringHolder			 mySkipBadNodePath;
    
    int					 myLastInputChanged;
    int					 myChildManagerCount[NUM_MANAGERS];
    int					 myStashedCurrentNodeId;
    bool				 myIsClearingContentsOfEditableSubnodes;
    UT_StringHolder			 myCachedMimeFile;
    
    static bool				 theWarnDeprecatedOpsOnLoad;

    friend class			 OP_Director;
    friend class			 MOT_Director;
};

OP_API const OP_OperatorTableMap &OPgetGlobalOperatorTables();

#endif

