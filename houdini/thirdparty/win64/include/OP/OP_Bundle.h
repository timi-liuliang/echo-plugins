/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Bundle.h ( OP Library, C++)
 *
 * COMMENTS:	A bundle of OPs.  These OPs can span across multiple networks
 *		and are thus heterogeneous (unlike OP_Groups).
 *
 * CAVEATS:
 *	Currently, we store the node membership as an int-array.  In theory,
 *	this can be changed to use a full-path string array.  The interface
 *	should remain the same.
 *
 * TODO:
 *	What we want to do in the long run is to have a mechanism to apply
 *	expressions to "filter" the contents of the bundle.  Basically, there
 *	should be an expression associated with the bundle.  The expression
 *	will define "membership".  There will be a bundle of new expression
 *	functions which work on bundles:
 *	   bundlematch(string pattern);
 *		Glob the pattern.  I'm guessing that the pattern will be
 *		defined on a per-op basis, so that we can handle relative
 *		paths.
 *
 *	   bundleexpand(string bundle_name);
 *		Expand the bundle into a list of op names.
 *
 *	   operator +(bundle, bundle)	- Union
 *	   operator -(bundle, bundle)	- Subtraction
 *	   operator ^(bundle, bundle)	- Intersection
 *
 *	   bundlefilter(bundle, filter_type)
 *		Filter a bundle according to some pre-defined filter type.  For
 *		example "obj/light" (only allow light objects)
 *	Possibly, there will be two expressions, the first is run before the
 *	static list is accumulated.  This allows arbitrary OPs to be added
 *	before the hard-coded OPs are processed.  The second is run after the
 *	fact, in order to trim the list down.  Of course, this may not be the
 *	way it works, this is just a todo.
 */

#ifndef __OP_Bundle__
#define __OP_Bundle__

#include "OP_API.h"
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Notifier.h>
#include "OP_Node.h"
#include "OP_BundleFilter.h"
#include "OP_BundlePattern.h"

class OP_Network;
class OP_Bundle;
class OP_Group;
class UT_WorkBuffer;

// a small class for notifications about bundle events
class OP_API OP_BundleEvent
{
public:

    enum OP_EventType
    {
	BUNDLE_RENAMED,		// bundle was renamed
	BUNDLE_PICKED,		// bundle was picked
	BUNDLE_LOCK_CHANGED,	// lock on the bundle changed
	BUNDLE_FILTER_CHANGED,	// filter of the bundle changed
	OP_ADDED,		// operator was added to the bundle
	OP_DELETED,		// operator was removed from the bundle
	OP_ADDED_OR_DELETED,
	OP_RENAMED		// operator was renamed
    };

    OP_BundleEvent( OP_EventType type, OP_Bundle &bundle ) 
			    : myEventType( type ), myBundle( bundle ) {}
    
    // the event type and originating bundle
    OP_EventType		 myEventType;
    OP_Bundle			&myBundle;
};


class OP_API OP_Bundle {
public:
     OP_Bundle(const char *name, int internal=0);
    ~OP_Bundle();

    /// Obtains the unique name of the bungle.
    const char		*getName() const	{ return myName; }

    /// Determines whether the budnle has been internally created. An internal
    /// bundle is created in C++ code, based on some pattern string obtained 
    /// from a node's parameter. The non-internal bundles are explicitly created
    /// by the user and are all listed in the bundle pane.
    int			 isInternal() const	{ return myInternal; }

    /// Rename the bundle to a new name.
    void		 rename(const char *name);

    /// Processes a new node that has been added to some network. The bundle
    /// may decide to add that node to itself, if it is a pattern bundle.
    bool		 nodeAdded(OP_Node *node);

    /// Processes a node that is about to be deleted from some network. If that
    /// node belongs to the bundle, it will be reomved as a member.
    void		 nodeDeleted(OP_Node *node);

    /// Processes a list of nodes that have just been added to some network. Any
    /// nodes that match the pattern (if set) will be added to the bundle.
    void		 bulkNodesAdded(const OP_NodeList &list);

    /// Informs the bundle that some unspecified nodes have been added or
    /// deleted. The bundle will mark itself as dirty, if necessary.
    void		 nodeAddedDeleted();

    /// Informs the bundle that some other bundle contents has changed. This
    /// bundle will mark iself as dirty, if necessary.
    /// @param	bundle_name The name of the other bundle that has changed
    ///		(including the @ signg )
    void		 otherBundleChanged(const char *bundle_name);

    /// Calculates and caches the member nodes that match the specified pattern,
    /// and other member values (such as myPatternSubnetInclusion)
    ///
    /// @param	creator	The creator network whose descendent nodes (children and
    ///			grand children, etc) are considered for the bundle 
    ///			membership.
    /// @param relativeto   The node with respect to which the pattern is
    ///			    specified (so that it is possible to resolve
    ///			    relative path patterns such as "../sibbling*"
    /// @param pattern	The pattern which the member member nodes must match.
    void		 expandPattern(const OP_Network *creator,
				       const OP_Node *relativeto,
				       const char *pattern);

    /// Removes all the member nodes (or cached nodes and dirties the bundle).
    void		 clear();

    /// Sets a new node filter for the bundle, dirties the bundle, and sends
    /// out an event.
    /// @param	filter	The new filter that influences the membership. It
    ///			accepts some nodes and rejects other.
    void			 setFilter(const OP_BundleFilter *filter);

    /// Returns the current node filter.
    const OP_BundleFilter	*getFilter()
				 { 
				     return myFilter; 
				 }

    /// Sets the pattern and turn the bundle into a smart bundle, if it is not
    /// already smart. If the pattern is NULL, the bundle will no longer be
    /// smart (it will be converted into a normal bundle).
    void			 setStringPattern( const char * pattern );

    /// Returns the pattern originally set on the bundle.
    const char			*getStringPattern() const
				 {
				     return myPattern ? myPattern->buffer()
						      : NULL;
				 }

    /// Returns the current pattern for nodes. The nodes that match the pattern
    /// are the members of the bundle.
    const OP_BundlePattern	*getBundlePattern() const
				 {
				     return myBundlePattern;
				 }

    /// Mark the bundle pattern as dirty
    void			 setBundlePatternDirty()
				 {
				     if (myBundlePattern)
					 myPatternDirty = true;
				 }

    /// Returns true if the bundle is "smart". That is if it is a non-internal
    /// bundle whose contents is determined by a pattern.
    bool			 isSmart() const
				 {
				     return myPattern && !myInternal;
				 }

    /// The subnet inclusion flag determines whether a pattern includes subnet
    /// contents. This means that if a node does not explicitly a member
    /// of the bundle (ie, does not match the pattern), but its ancestor
    /// does, then that node is also a member.
    void			 setSubnetInclusionFlag(bool onoff);

    /// Obtains the subnet inclusion flag.
    int				 getSubnetInclusionFlag() const
					{ return myPatternSubnetInclusion; }

    /// Adds the node to the bundle and sends out a notification that a node
    /// has been added.
    int			 addOp(OP_Node *op);
    int			 addOp(int unique_id);

    /// Adds the nodes in the list to the bundle and sends out a
    /// notification that nodes have been added. If filter or a pattern is set,
    /// only the nodes that match them will be added.
    /// @return	Returns the number of added nodes .
    int			 addOpList(const OP_NodeList &list);

    /// Removes the node from the bundle and sends out a notification event.
    int			 removeOp(OP_Node *op);
    int			 removeOp(int unique_id);

    /// Processes a node when its type (or representative type) has changed.
    /// The bundle gets marked as dirty if necessary. 
    void		 refilterOp(OP_Node &node);

    /// Returns the touch time, which is an integer that gets incremented
    /// each time the bundle contents changes.
    int			 getTouchTime() const	{ return myTouchTime; }

    /// Returns a flag that indicates if the bundle tries to automatically add
    /// newly created nodes to itself.
    int			 isLocked() const	{ return myLockedFlag; }
    void		 setLocked(bool onoff);

    /// Returns the number of member nodes.
    int			 entries();

    /// Returns the i-th member of the bundle. The order is arbitrary, but the
    /// index should not exceed the number of total entries.
    OP_Node		*getNode(int idx);

    /// Returns the root node at which the search begins when matching the
    /// pattern. Only the ancestors (children, grandchildren - that is nodes
    /// contained in some way by the root) are considered when matching the 
    /// pattern.
    const OP_Node	*getPatternNode() const;

    /// Returns the node used to resolve relative paths in the pattern.
    const OP_Node	*getRelativeNode() const;

    /// Sorts the member nodes alphanumerically by node path.
    void		 sortByPath();

    /// Sorts the member nodes by numerical value of the node pointer.
    void		 sortByPointer();

    /// Changes (ie, increases or decreases) the reference count by the given
    /// amount. When the count decreases to zero, the bundle list (ie, the owner
    /// of all the bundles) will delete the bundle.
    void		 bumpRefCount(int dir)	{ myRefCount += dir; }

    /// Returns the current reference count.
    int			 getRefCount() const	{ return myRefCount; }

    /// Determines whether or not a node is contained in the bundle.  If the
    /// check_representative flag is true, then the node's parents will be
    /// checked for containment inside the bundle.
    int			 contains(const OP_Node *op, bool check_representative);
    int			 contains(int unique_id, bool check_representative);

    /// Builds a string that specifies all the members of the bundle.
    void		 buildString(UT_WorkBuffer &buffer, OP_Node *cwd = 0, 
				    char delim = ' ');

    /// Obtains all the bundle member ids.
    void		 getMembers(UT_IntArray &list);

    /// Obtains all the bundle members as a node list.
    void		 getMembers(UT_ValArray<OP_Node *> &list);

    /// Obtains the union of the member nodes from all the given bundles.
    /// The nodes in the result list are unique.
    static void		 getAllMembers( const UT_ValArray<OP_Bundle*> &bundles,
					UT_ValArray<OP_Node *> & nodes );

    /// For undo mechanism only.  Don't go mucking with the array please
    /// @private
    const UT_IntArray	&undoGetMembers() const	{ return myNodes; }
    void		 undoSetMembers(UT_IntArray &nodes);

    /// For undo mechanism only. Don't emit events whenever you feel like
    /// (otherwise it would be private)
    /// @private
    void		 emitEvent( OP_BundleEvent::OP_EventType type );

    /// Adds a node interest to the bundle. If the bundle changes, it will 
    /// alert all the nodes that expressed interest in it by calling
    /// bundleChanged() on it. This is a separate notification mechanism
    /// from passing the OP_BundleEvent via myEventNotifier.
    /// @param	add_id	The node id of the interested node.
    void		 addInterest(int add_id);

    /// Removes the node interest from the bundle. 
    /// @param	remove_id   The node id that is no longer interested in bundle.
    void		 removeInterest(int remove_id);

    /// Adds a parameter interest to the bundle. If the bundle changes,
    /// it will alert all the parameter channels by calling parmChanged()
    /// on the node with parm id. This is a separate notification mechanism
    /// from passing the OP_BundleEvent via myEventNotifier.
    /// @param	add_id	The node id of the interested node.
    /// @param	parm_id	The id (index) of the referencing parameter
    void		 addParmInterest(int node_id, int parm_id);

    /// Removes the parameter interest from the bundle.
    /// @param	add_id	The node id of the interested node.
    /// @param	parm_id	The id (index) of the referencing parameter
    void		 removeParmInterest(int node_id, int parm_id);

    /// Processes the given group after it has changed. If any bundles 
    /// reference this group, the will be marked as dirty.
    static void		 notifyOpsOfGroupChange(OP_Group *group);
    
    /// Sets the picked (selected) flag to on/off.
    int		 	 setPicked(int on_off);

    /// Returns the current pick (selected) flag.
    int			 getPicked() const	{ return myPickedFlag; }

    /// Returns an object that emmits events originating from the bundle
    /// when something about the bundle changes.
    UT_NotifierImpl<OP_BundleEvent&>
			&getEventNotifier() { return myEventNotifier; }
    
    /// Converts the normal bundle to a smart bundle by using the members
    /// of the bundle to constuct a pattern that will match all of the 
    /// current members and only the current members.
   void 		 convertToSmartBundle(UT_String* finalpattern);


public: // convenience methods that operate on all members of a bundle 
    /// Sets the flag on on all the members of this bundle.
    void		 setOpsFlag(char flagchar, int onoff,
				    bool propagate_to_ancestors = false);

    /// sets op visibility. This is smarter than just truning the display
    /// flag. It also adds to the visible children parameter of the ancestors
    /// so that the bundle nodes become visibile.
    void		 setOpsVisibility( bool onoff );

    /// Syncs (if sync_flag == true) and unsyncs (if sync_flag == false )
    /// the HDA definitions of the nodes contained in this bundle.
    void		 syncOpOTLs( bool sync_flag, 
				     bool propagate_to_ancestors = false);

private:
    /// Tests a single pattern to see if it the pattern catches anything that
    /// is not in the bundle.
    bool		testPattern(UT_String pattern, const OP_Node* node);

    /// The bundle is defined by the asterisk pattern, so we can do faster
    /// checks which don't require building the node list.
    bool		checkAsteriskPattern(int unique_id) const;

    /// Finds the longest common prefix and the longest common suffix of an 
    /// array of patterns and then combines then with single level wildcard
    /// separators.
    /// Ex.:	    paths = ("/obj/box_object1/section1/obj1", 
    ///			     "/obj/sphere_object2/section2/obj1")
    ///		    will result in:
    ///		    prefix = /obj/
    ///		    suffix = /obj1
    ///		    resultpattern = /obj/%/obj1
    ///
    void		combineIxes(UT_StringArray& paths, 
				    UT_String* resultpattern, 
				    UT_String* prefix, UT_String* suffix);

    /// Constructs a pattern that will match  all of the nodes at one specific
    /// depth
    void		 constructSinglePattern(UT_String* resultpattern,
						UT_StringArray& paths);

    /// This function will determine whether each level of the regex is 
    /// is independent of the subsequent levels, if they are independent, they can
    /// be isolated and handled separately.
    UT_String		 checkLevelIndependence(
			    UT_Array<UT_StringArray>* pathsbylevel,
			    UT_String prefix, UT_String suffix);

    /// This function takes independent chunks of the regex and shortens them 
    /// as possible.
    UT_String		shortenIndependents(
			    UT_Array<UT_StringArray>& pathsbylevel,
			    UT_String prefix, UT_String suffix);

    /// Constructs an array where the the members of the Bundle are separated
    /// by the depth of their full path. 
    /// Ex.: If the bundle contains ("/obj", "/obj/box1", "/obj/box2")
    ///	    The resulting array would be:
    ///			pathsbylength(0) = empty
    ///			pathsbylength(1) = ("/obj")
    ///			pathsbylength(2) = ("/obj/box1", "/obj/box2")
    void		getPathsByLength(
				UT_Array<UT_StringArray>* pathsbylength);

    /// Precondition: all strings in paths must be the same depth
    /// Will populate a list of paths by level. The nth entry of the ref array
    /// contains a list of all of the substrings at the n'th level of depth from
    /// each element of paths.
    void		getPathsByLevel(UT_StringArray& paths,
				UT_Array<UT_StringArray>* pathsbylevel,
				UT_String prefix, UT_String suffix);

    /// Converts the smart bundle to a normal bundle by removing
    /// its defining pattern (if any).  The member list in the bundle
    /// remains the same.  A bundle with no defining pattern is already
    /// a normal bundle.
    void		 convertToNormalBundle();

    /// Checks whether the specified node matches both the
    /// filter and the pattern of the bundle.
    bool		isNodeValid( OP_Node *node,
				     const OP_Node *relativeto ) const;
    /// Removes duplicate nodes in the member list.
    void		removeDuplicates();

    /// Iterates through the nodes (and parameters) that registered the
    /// interest in the bundle and notifies them of the bundle change.
    void		notifyOpsOfChange( OP_BundleEvent::OP_EventType type );

    /// Resets the notification flag, so any changes to the bundle will
    /// trigger event notifications, because the bundle is being evaluated.
    void		bundleEvaluated()	{ myNotifiedFlag = 0; }

    /// Dirties the bundle to indicate that something about the bundle has
    /// changed, and the bundle needs re-evaluation.
    void		touch();

    /// Re-evaluate the bundle membership based on the pattern, which basically
    /// searched for (and caches) all the nodes that match the pattern.
    void		buildPattern();

    /// Refreshes the bundle membership nodes, if necessary.
    void		refreshPattern() {
			    if (myPatternDirty && myPattern)
				buildPattern();
			}
    /// Refreshes the bundle membership nodes if necessary and removes any
    /// duplicate nodes tha may have been doubly-added to the bundle.
    void		refresh()   { refreshPattern(); removeDuplicates(); }

    /// Marks the sort order as dirty.
    void		dirtyAllSorts()
				    { myPathSortDirty = myPtrSortDirty = 1; }

    /// Returns true if this bundle contains a node that is a parent of the
    /// node given by unique_id and also matches the filter.  In other words,
    /// returns true if this bundle contains unique_id implicitly by containing
    /// its parent subnet which has a representative matching the filter.
    bool		containsAsRepresentative(int unique_id);


private:
    /// Unique bundle name.
    char			*myName;

    /// Pattern string that defines the bundle. 
    /// If null, the bundle is not defined by pattern.
    UT_String			*myPattern;	    

    /// Pattern object capable of matching the nodes; it is computed based
    /// on the myPattern member and then cached here. If this member is
    /// null, the bundle still may be a pattern bundle that has not been
    /// evaluated (expanded) yet.
    OP_BundlePattern		*myBundlePattern;   

    const OP_BundleFilter	*myFilter;	    // filter for member nodes
    UT_IntArray			 myNodes;	    // member nodes
    UT_IntArray			 myInterests;	    // nodes interested in bndle
    UT_IntArray			 myParmInterests;   // parms interested in bndle
    UT_IntArray			 myNodeInterests;   // .. nodes that own parms
    int				 myRefCount;	    // bundle reference count
    int				 myPatternNode;	    // root of node search tree
    int				 myRelativeNode;    // reolves relative paths
    int				 myTouchTime;	    // increments at each change
    int				 myRemoveDups;	    // duplication removal flag

    // object that notifies about bundle events
    UT_NotifierImpl<OP_BundleEvent&>    myEventNotifier;

    uint	 myInternal:1,		// Internal bundle or user defined
		 myExpanding:1,		// Recursion flag check
		 myPatternDirty:1,	// Pattern expansion is out of date
		 myPatternSubnetInclusion:1, // Whether, unless explicitly
					     // excluded, subnet contents are
					     // included with the subnet.
		 // locked bundle does not add any newly created nodes that 
		 // match the filter; the unlocked bundle does.
		 myLockedFlag:1,	// Bundle is locked
		 myPathSortDirty:1,	// Is path sort dirty
		 myPtrSortDirty:1,	// Is pointer sort dirty
		 myNotifiedFlag:1,	// I've already notified objects
		 myPickedFlag:1,	// bundle is picked (selected) 
		 myBulkAdding:1;	// Performing bulk addition
};

#endif
