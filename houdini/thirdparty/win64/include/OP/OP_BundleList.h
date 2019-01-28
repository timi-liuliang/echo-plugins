/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_BundleList.h ( OP_BundleList Library, C++)
 *
 * COMMENTS:	Container for bundles of objects
 */

#ifndef __OP_BundleList__
#define __OP_BundleList__

#include "OP_API.h"
#include <iosfwd>
#include <UT/UT_ValArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_Notifier.h>
#include "OP_Node.h"

class UT_String;
class PRM_Name;
class OP_Node;
class OP_Network;
class CMD_Args;
class CMD_Manager;
class OP_Bundle;
class OP_Group;
class OP_BundleFilter;


// a small class for notifications about bundle events
class OP_API OP_BundleListEvent
{
public:

    enum OP_BundleListEventType
    {
	BUNDLE_ADDED,		// bundle was created or added to the list 
	BUNDLE_DELETED,		// bundle was removed from the list
	BUNDLE_CHANGED		// a bundle changed
    };

    OP_BundleListEvent( OP_BundleListEventType type, OP_Bundle &bundle ) 
	: myEventType( type ), myBundle( bundle ) {}
    
    // the event type
    OP_BundleListEventType	     myEventType;
    OP_Bundle			    &myBundle;
};


// definition of the bundle list
class OP_API OP_BundleList 
{
public:
     OP_BundleList();
    ~OP_BundleList();

    /// Deletes all bundles (internal or non-internal).
    void	 clear();

    /// Returns the number of non-internal bundles.
    int		 entries() const	{ return myBundles.entries(); }

    /// Returns the i-th non-internal bundle.
    OP_Bundle	*getBundle(int i)	{ return myBundles(i); }

    /// Searches for the bundle by name, and returns it if found. Otherwise
    /// returns NULL.
    OP_Bundle	*getBundle(const char *name);

    /// Obtains the list of bundles whose names match the given pattern.
    void	 getBundlesByPattern( const char * pattern,
				      UT_ValArray<OP_Bundle *> & bundles );

    /// Returns the number of internal bundles.
    int		 internalEntries() const { return myInternal.entries(); }

    /// Returns the i-th internal bundle.
    OP_Bundle	*getInternalBundle(int i)	{ return myInternal(i); }

    /// Creates and returns a new bundle. 
    /// @param	name	    The name of the bundle.
    /// @param	internal    If true, the created bundle is internal. Otherwise,
    ///			    it's non-internal.
    OP_Bundle	*createBundle(const char *name, int internal=0);

    /// Creates a duplicate of a given bundle. The new bundle's name is similar
    /// to the original, but a numerical suffix is appended.
    /// @param	bundle	The bundle to duplicate.
    OP_Bundle	*duplicateBundle(OP_Bundle * bundle);

    /// Deletes the bundle of a given name. Before the bundle is deleted an
    /// event is sent out.
    /// @param	name	The name of the bundle to delete.
    /// 
    /// @return	True if the bundle was found and destroyed.
    int		 destroyBundle(const char *name);
    int		 destroyBundle(int index);

    /// Returns a potential bundle name that has no duplicate among the
    /// existing bundles.
    /// @param	base	The desired name of the bundle. If that name is not
    ///			taken, it will be returned. Otherwise, a numerical
    ///			suffix will be appended.
    char *	 findUniqueBundleName(const char *base = NULL);
    int		 renameBundle(const char *oldname, const char *newname);

    /// Obtains an array of pointers to the non-internal bundles that
    /// the given node is a member of. This will NOT return indices to 
    /// internal bundles.
    /// @returns    The number of bundles returned. 
    int		 getMemberships(const OP_Node *node,
    				UT_ValArray<OP_Bundle *> &memberships) const;

    /// Sets a new bundle filter.
    /// @param	bname	The bundle name.
    /// @param	fname	The filter name.
    /// @return	    The previous filter.
    const OP_BundleFilter	*setBundleFilter(const char *bname,
						 const char *fname);
    /// Returns the filter by the given name, or NULL if not found.
    const OP_BundleFilter	*lookupFilter(const char *name) const;

    /// Obtains a list of existing filters.
    void	 getFilters( UT_ValArray<const OP_BundleFilter *> &list );


    /// A convenience method to set the lock on the member nodes of a bundle,
    /// within an undo bloc.
    void		     bundleSetLock( OP_Bundle *bundle, bool lock );

    /// A convenience method to set a filter on a bundle, within an undo block.
    /// @param	fname	The name of the new filter.
    /// @return	    The old filter.
    const OP_BundleFilter   *bundleSetFilter(OP_Bundle* bundle, 
					     const char *fname); 

    /// A convenience method to set a filter on a bundle, within an undo block.
    /// @return	    The old filter.
    const OP_BundleFilter   *bundleSetFilter(OP_Bundle* bundle, 
					     const OP_BundleFilter *filter);

    /// A convenience method to set a pattern on a bundle, within an undo block.
    /// If a bundle was a normal bundle, it is turned into a pattern bundle.
    void		     bundleSetPattern(OP_Bundle* bundle,
					      const char * pattern);

    /// A convenience method to remove all nodes from the bundle, within an undo
    /// block, and to send out a notification.
    void		     bundleClear( OP_Bundle *bundle );

    /// A convenience method to add a node to the bundle, within an undo
    /// block, and to send out a notification.
    /// @return	    The number of nodes added.
    int			     bundleAddOp( OP_Bundle *bundle, OP_Node* node );

    /// A convenience method to add nodes to the bundle, within an undo
    /// block, and to send out a notification.
    /// @return	    The number of nodes added.
    int			     bundleAddOps(OP_Bundle *bundle,
					  const OP_NodeList &nodes );

    /// A convenience method to remove a node from the bundle, within an undo
    /// block, and to send out a notification.
    /// @return	    The number of nodes removed.
    int			     bundleRemoveOp( OP_Bundle *bundle, OP_Node* node );

    /// A convenience method to remove nodes from the bundle, within an undo
    /// block, and to send out a notification.
    /// @return	    The number of nodes removed.
    int			     bundleRemoveOps(OP_Bundle *bundle, 
					     const OP_NodeList &nodes );
    
    /// A notifier called by OP_Network when a node has been created. Adds the
    /// node to any matching pattern bundles.
    void	 nodeAdded(OP_Node *node);

    /// A notifier called by OP_Network when a node has been deleted. Removes
    /// the node from any bundles that contain it.
    void	 nodeDeleted(OP_Node *node);

    /// A notifier called by OP_Network when a node has been renamed. Removes
    /// the node from any pattern bundles that the node no longer matches, and
    /// adds the node to any pattern bundles that the node now matches.
    void	 nodeRenamed(OP_Node *node);

    /// A notifier called when the node type (or its representative type) has 
    /// changed. It reapplies the filters to the bundles to see if the node 
    /// now passes the type tests.
    void	 refilterOp(OP_Node &node);

    /// Called when director is finished loading. It adds the nodes to any
    /// matching bundles.
    // NB: if loading is nested (as it sometimes happens with nested OTLs)
    //     and we are still loading when this method is invoked,
    //     nodes will be added to a pending list. When the bulkAddNodes is
    //     invoked later (since it is invoked always after endLoading()),
    //     and when we are no longer loading, then we process both nodes and 
    //     pending nodes. If nodes is NULL then it tries to flush pending unless
    //     director is still loading.
    void	 bulkAddNodes(const OP_NodeList *nodes);

    /// Returns the integer indicating the list version number. Each time
    /// the list changes (a bundle is added, removed, etc) the integer is
    /// incremented. This can be used to avoid any recomputations, if nothing 
    /// has changed.
    int		 getTouchTime() const	{ return myTouchTime; }

    /// Returns an internal bundle that has the specified pattern. 
    /// If the bundle already exists (as specified by the name parameter) 
    /// and the method parameters correspond to the bundle attributes, then it
    /// is reused and returned. Otherwise, a new bundle is created, configured, 
    /// and returned; in this case, if there is any old bundle matching the
    /// given name, its reference count is decreased. This is handy for
    /// streamlined requests for internal pattern bundles, where this method
    /// can be invoked with the same UT_String object and different pattern, and
    /// the old bundles are seamlessly deleted and new ones created.
    /// 
    /// This method will get a pattern style (i.e. internal bundle) based on the
    /// creator and the pattern specified.  The "style" parameter should be a
    /// unique string for the type of pattern (i.e. "lightmask").
    /// To maintain proper reference counting, the method takes a reference to a
    /// pointer.  This allows the list to maintain the list of referenced
    /// patterns.  The subnet_inclusion argument determines whether subnet
    /// contents are included unless explicitly excluded.
    /// @param	prev_bundle	The name of the bundle created before based on a
    ///	    pattern (perhaps the same or perhaps different) used for the same
    ///	    purpose as the bundle obtained by this method. If, a bundle given by
    ///	    this parameter exists, and it is a of a different pattern, then its
    ///	    reference count will be decremented (and bundle deleted if it
    ///	    reaches zero). Upon return of this method, this parameter will
    ///	    contain the name of the new bundle. If prev_bundle exists, and it
    ///	    exhibits the same properties as specified in the method invocation,
    ///	    then that bundle is reused, the prev_bundle parameter is unchanged,
    ///	    and no reference count is bumped or decreased (ie, just reusing
    ///	    the bundle). This parameter can be an empty string (eg, when called
    ///	    for the very first time).
    /// @param	creator	    The node at which the search for matching nodes
    ///	    begins. It's like a root node in the node parenthood hierarchy. Only
    ///	    nodes contained by the creator are considered for the bundle
    ///	    membership; that is only children, grandchildreen, etc of the root
    ///	    node are considered.
    ///	@param	relativeto  The node with respect to which the relative node
    ///	    paths are specified in the pattern.
    ///	@param	pattern	    The node name or path pattern. Any nodes that match
    ///	    the patterns will be included in the bundle.
    ///	@param	filter	    The name of the OP_BundleFilter filter that needs to
    ///	    be applied to the bundle. Only nodes passing the filter test
    ///	    (usually the node type test) will be included in the bundle.
    ///	@param	label	    An arbitrary label that is included in the
    ///	    construction of the name for the new bundle.  May be used for
    ///	    differentiating between bundles with the same pattern and filter.
    ///	@param	subnet_inclusion    If true, a node will be included in the
    ///	    bundle if any of its ancestors (parent, grandparent, etc) matches
    ///	    the patter, even if the node itself does not match the pattern.
    ///	@param addinterest  If true, the relativeto node will request an
    ///	    interest in the bundle, so it is dirtied an recooked whenever
    ///	    the contents of the returned internal pattern bundle changes. 
    ///	    This is because it is usually the relativeto nodes that invoke this
    ///	    method during their cook path (and parameter evaluation), to gather
    ///	    any other nodes (defined by the pattern), process them, and update
    ///	    own cooked values according to them; since the cooked values are
    ///	    dependent on the gathered nodes, and the gathered nodes
    ///	    depend on the pattern, then the cooked node needs to be marked as
    ///	    dirty.
    OP_Bundle	*getPattern(UT_String &prev_bundle,
			    const OP_Network *creator,
			    const OP_Node *relativeto,
			    const char *pattern,
			    const char *filter = 0,
			    const char *label = 0,
			    bool subnet_inclusion = true,
			    bool addinterest=1);

    /// Decreases the reference count of the bundle, and deletes it when it
    /// reaches zero. Used for discarding unwanted bundles that will no longer
    /// be used.
    void	 deReferenceBundle(const UT_StringRef &prev_bundle);

    /// Sorts the list of bundles alphabetically. This is useful after
    /// appending, renaming, or removing any bundles, to mainatin the 
    /// alphanumeric order.
    void	 sortBundles();

    /// Attempts to simplify the given pattern using this bundle list.
    /// @param	root	The node with respect to which relative paths are
    ///	    specified in the pattern.
    void	 simplifyPattern( const OP_Node *root, UT_String &pattern );

    /// Attempts to simplify the given pattern in a slightly safer manner than
    /// the other method.  The three main differences are:
    /// it supports groups, it doesn't care about group/bundle contents 
    /// (hence a bit faster), and it ignores unrecognized tokens instead 
    /// of removing them.
    /// @param	root	The node with respect to which relative paths are
    ///	    specified in the pattern.
    static void	 simplifyPatternSafely(const OP_Node *root, UT_String &pattern);


    /// Implements the hscript command that lists the bundles.
    void	 cmdLs(CMD_Args &args);

    /// Implements the hscript command that adds a new bundle.
    void	 cmdAdd(CMD_Args &args);

    /// Implements the hscript command that removes an old bundle.
    void	 cmdRm(CMD_Args &args);

    /// Implements the hscript command that manipulates bundle contents by
    /// adding, removing, or replacing the member nodes.
    void	 cmdOp(CMD_Args &args);

    /// Implements the hscript command that renames a bundle.
    void	 cmdName(CMD_Args &args);

    /// Saves the bundle list to the hip file.
    int		 saveToHip(std::ostream &os);

    /// Generates a menu contents that consists of the names of the available
    /// bundles.
    /// @param	menu	An output parameter that will contain new names
    ///	    representing the menu entries.
    /// @param	max	The maximum number of entries that are allowed to be
    ///	    added.
    /// @param	filter	If not NULL, the name of the filter that the bundles
    ///	    must have, if the are to be added to the menu.
    void	 buildBundleMenu(PRM_Name *menu, int max,
				 const char *filter=0,
				 int include_at_sign=1,
				 int put_in_default_message=1);

    /// Standard function that installs the bundle hscript commands.
    static void	 installCommands(CMD_Manager *cmd);

    /// Invoked when the group has changed. It dirties any bundles whose
    /// contents depends on that group.
    void	 notifyOpsOfGroupChange(OP_Group *group);

    /// Returns an obiect that emmits events that originated from the
    /// bundle list.
    UT_NotifierImpl< OP_BundleListEvent& >
			&getEventNotifier() { return myEventNotifier; }
 
    /// Prints out all the available bundle filters.
    void		 dumpFilters(std::ostream &os);

private:
    /// Alerts all other bundles when a bundle has changed.
    void		 notifyBundleChange(const char *name);

    /// Emits an event notifying that a bundle is about to change.
    void		 notifyBundleDeletion(OP_Bundle *bundle);

    /// Incremeants the touch time (when a bundle is added or removed, and the
    /// list changes).
    void		 touch();

    /// Obtains nodes that match the pattern (relative paths are specified
    /// with respect to the root).
    bool		 getSortedPatternTokenNodes(
			    const OP_Node *root,
			    const char *pattern_token,
			    OP_NodeList &nodes );

    /// Notifies bundles of an added node and recures on node's descendents.
    void		 nodeAddedRecurse(OP_Bundle *bundle, OP_Node *node);

    /// Collects the nodes to be removed from a bundle when a given node
    /// is about to be deleted. These are the descendants of the node.
    void		 nodeDeletedRecurse(OP_Bundle *bundle, OP_Node *node,
					    OP_NodeList & to_remove);


private:
    /// The list of non-internal bundles.
    UT_ValArray<OP_Bundle *>	myBundles;

    /// The list of internal bundles.
    UT_ValArray<OP_Bundle *>	myInternal;

    /// Look-up table that maps a bundle name to the bundle pointer.
    UT_SymbolMap<OP_Bundle *>	myNames;

    /// Flag that indicates if the list needs to be re-sorted.
    int				mySortDirty;

    /// The version integer that is incremented each time the bundle list
    /// changes (eg, when a bundle is added or removed).
    int				myTouchTime;

    /// ID's of nodes to add once director complets loading.
    UT_IntArray			myPendingBulkAdded; 

    /// Object that nodifies about bundle list events.
    UT_NotifierImpl<OP_BundleListEvent&>    myEventNotifier;
};


// helper class for saving/restoring node bundle memberships
class op_NodeBundle;
class OP_API OP_NodeBundleMemberships
{
public:
    OP_NodeBundleMemberships();
    OP_NodeBundleMemberships( OP_BundleList *bundle_list );
    ~OP_NodeBundleMemberships();

    /// Saves all node bundle memberships including descendents.
    void saveMemberships( OP_Node *node );

    /// Restore previously saved node bundle memberships.
    void restoreMemberships( OP_Node *node );

    /// Return number of bytes used in RAM.
    int64	getMemoryUsage() const;

    /// Returns the number of bundles.
    int	getNumBundles() const;

private: // methods
    void saveChildMemberships( OP_Node *node, OP_Node *root );

private: // data
    UT_ValArray<op_NodeBundle *>    myNodeBundles;
    OP_BundleList *		    myBundleList;
};

#endif
