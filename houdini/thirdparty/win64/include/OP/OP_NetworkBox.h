/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_NetworkBox.h (OP Library, C++)
 *
 * COMMENTS:	Defines network boxes!
 * 
 */

#ifndef __OP_NetworkBox__
#define __OP_NetworkBox__

#include "OP_API.h"
#include "OP_NetworkBoxFlags.h"
#include "OP_NetworkBoxItem.h"
#include "OP_Node.h"

#include <UT/UT_Assert.h>
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>


class OP_Network;
class OP_Stat;
class UT_AutoUndoBlock;

#define OP_NETWORKBOX_DEFAULT_W		2.5F
#define OP_NETWORKBOX_DEFAULT_H		2.5F
#define OP_NETWORKBOX_MINIMUM_W		0.3F
#define OP_NETWORKBOX_MINIMUM_H		0.3F
#define OP_NETWORKBOX_MINIMIZED_W	2.5F

/// When saving a network box to the clipboard, you can use this enumeration
/// to specify which of its contents to automatically save with it
enum OP_NetworkBoxSaveType
{
    OP_NETWORKBOX_SAVE_NO_CONTENTS,	    // Don't save any items in the box
    OP_NETWORKBOX_SAVE_ALL_CONTENTS,	    // Save all items in the box
    OP_NETWORKBOX_SAVE_PICKED_CONTENTS,	    // Save only the picked items
    OP_NETWORKBOX_SAVE_SPEC_CONTENTS  	    // Save only the specified items. 
					    // Applies only to nodes (cannot
					    // save an indirect unless you save
					    // its parent subnet entirely).
					    // Supported only by certain
					    // methods which require a node ptr
					    // array passed in
};

class OP_API OP_NetworkBox : public OP_NetworkBoxItem
{
public:
    /// Create a network box with the specified name. If a duplicate name is 
    /// passed in, it will be altered to make it unique in the network.
	    OP_NetworkBox(const UT_String &name, OP_Network *net);
    virtual ~OP_NetworkBox();

    /// Set the coordinates for this box (lower left corner) in absolute units.
    /// NOTE: If myResizing is set, then setXY does NOT move the box, but
    /// effectively resizes it by moving the lower/left corner of the box 
    /// without moving its contents, and this may result in a change in myW 
    /// and/or myH.
    virtual void	 setXY(fpreal x, fpreal y);
    
    /// Same as setXY(), but here we can force the box to take on these X/Y 
    /// coordinates, even if our contents don't fit us any more.  The
    /// propagate_parent_event tells this method whether or not it should send
    /// and event after moving the network box -- this should normally be set
    /// to true.
	    void	 setXY(fpreal x, fpreal y, bool force,
			       bool propagate_parent_event);

    /// Get the position of this box. Units are absolute, as opposed
    /// to relative units found in OPUI.
    virtual fpreal	 getX() const;
    virtual fpreal	 getY() const;

    /// Get and set the width and height of this box. Units are absolute
    /// as opposed to the relative units found in OPUI.
    /// If the width or height specified is smaller than that which will
    /// contain all of the items, the width or height respectively will only
    /// be adjusted to the minimum size required to contain the items. To avoid
    /// this automatic size check, set 'force' to true.
    void		 setW(fpreal w, bool force = false);
    void		 setH(fpreal h, bool force = false);

    /// Set the network box's width like setW(), but first check if the new
    /// width is valid.  This method wraps the resizing operation
    /// with calls to setResizing(true) and setResizing(false).
    bool		 setWWithBoundsChecks(fpreal w);

    /// Set the network box's width like setH(), but first check if the new
    /// height is valid.  This method wraps the resizing operation
    /// with calls to setResizing(true) and setResizing(false).
    bool		 setHWithBoundsChecks(fpreal h);
    
    /// Gives you the w/h of the netbox as you see it, meaning if the netbox is
    /// minimized, you get the minimized size.
    virtual fpreal	 getW()  const;
    virtual fpreal	 getH()  const;

    /// Return the full maximized size of the netbox, regardless of whether the
    /// netbox is currently maximized or minimized
    fpreal		 getMaximizedW() const;
    fpreal	 	 getMaximizedH() const;

    /// Using the netbox's current X,Y, width, and height data, 
    /// resize the netbox if necessary to fit its contents. Will ONLY resize if 
    /// the box is too small. WILL add a small buffer border around its contents
    /// for prettiness.
    void		 resizeToFit();
    
    /// Given an area defined by the absolute coordinates (xmin, ymin) - 
    /// (xmax, ymax), resize the netbox if necessary to ensure that the area 
    /// can fit inside this box. Will ONLY resize if the box is too small. Will
    /// NOT add a buffer border around its contents for prettiness.
    void		 resizeToFit(fpreal xmin, fpreal ymin, fpreal xmax,
				     fpreal ymax);

    /// The name which is used for identification and in displays of this
    /// box. setName() returns true if the name change was successful, and 
    /// alters the name slightly if necessary to make it unique within this 
    /// network.
    bool		 	 setName(const UT_String &name);
    virtual const UT_String	&getItemName() const
				 { return myName; };
    virtual bool		 setItemName(const UT_String &name)
				 { return setName(name); }

    /// The comment that is used to describe the network box.
    void		 	 setComment(const UT_StringHolder &comment);
    const UT_StringHolder	&getComment() const { return myComment; };

    /// Items which are contained in this box can be OP_Nodes,
    /// OP_IndirectInputs or OP_NetworkBoxes. They are all encapsulated
    /// in the OP_NetworkBoxItem class.
    int			 getNitems() const;
    int			 getNexposedItems() const;
    OP_NetworkBoxItem	*getItem(int idx) const;

    /// Collect items base on their type, possibly recursively.
    void		 getItems(OP_ItemTypeMask item_mask,
				OP_NetworkBoxItemList &list,
				bool include_hidden = true,
				bool recursive = true,
				bool picked_only = false) const;
    void		 getNodes(OP_NodeList &list,
				bool include_hidden = true,
				bool recursive = true,
				bool picked_only = false) const;

    // Like getItems(), except that it only counts the number of them
    exint		 countItems(
				OP_ItemTypeMask item_mask,
				bool include_hidden = true,
				bool recursive = true,
				bool picked_only = false) const;

    // Traverse the items returned by getItems()
    template <typename F>
    void		traverseItems(
				OP_ItemTypeMask item_mask,
				bool include_hidden,
				bool recursive,
				bool picked_only,
				const F &functor) const;

    /// Add an item to this box. If the item was previously in another netbox,
    /// we remove it from there before we add it here. If 'resize_to_fit' is
    /// set to true, we'll resize if necessary to fit our contents after the 
    /// item is added - this option should always be on, except when the undo
    /// mechanism requires some extra control to restore states properly
    bool		 addItem(OP_NetworkBoxItem *item, 
				 bool propagate_parent_event = true,
				 bool resize_to_fit = true); 
    bool		 addItems(OP_NetworkBoxItemList &items);

    /// Remove the item from this box. The item's owner box is set to NULL.
    bool		 removeItem(OP_NetworkBoxItem *item,
				 bool propagate_parent_event = true,
				 bool do_reparent = false,
				 bool save_undo = true);
    bool		 removeItems(OP_NetworkBoxItemList &items);
    bool		 removeAllItems(bool do_reparent = false);

    /// Sets the network boxes contents to be exactly the items in 'items' (i.e.
    /// removes any previous contents not in 'items'). Set 'snap_to_bounds' to 
    /// true if you'd like us to make the netbox's dimensions snap to form a
    /// nice snug fit around its new contents.
    bool		 setContents(OP_NetworkBoxItemList &items,
				     bool snap_to_bounds); 
    
    /// Returns true if this box is currently picked
    virtual int		 getPicked() const;

    /// Set whether this box is picked; returns true if the picked
    /// status has changed. Setting 'propagate_parent_event' to false stops
    /// this method from notifying the network of this change, useful when we're
    /// picking a lot of items at once, it's inefficient to trigger a callback
    /// for each pick separately, can just do one update at the end
    virtual int 	 setPicked(int on_off, 
				   bool propagate_parent_event = true);

    /// Get whether this box is currently minimized
    bool		 getMinimized() const;

    /// Set whether this box is currently minimized
    /// If update_position is true, then the box is moved to accomodate the
    /// minimized status.  If propagate_parent_event is true, an event is sent
    /// to the network about our minimized state.
    bool		 setMinimized(bool isminimized,
				      bool update_position,
				      bool propagate_parent_event);

    /// A very general purpose method used to temporarily mark this network box 
    /// as being involved in an operation. Very useful if e.g. we have a set of 
    /// nodes and network boxes to apply an operation to. In this case we want 
    /// to mark each netbox as we apply our op to it and its contents, in case 
    /// any of the nodes we're passed happen to be in one of those netboxes. In
    /// this way we can quickly check if a node's owning box has been marked, 
    /// and avoid duplicately applying the operation to the node again. REMEMBER
    /// to UNMARK all netboxes you marked once you're done with your operation!
    void 		 setMarked(bool flag);
    bool		 getMarked();

    /// Calculate the smallest bounds we're willing to accept as our size
    /// given our contents. By default, 'incl_buff_border' is set to add a small
    /// buffer border to make the layout look pretty. NOTE: If we contain no
    /// items, we return an inverted bounding box, see implementation for
    /// further details.
    void		 getItemBounds(fpreal &x1, fpreal &y1, fpreal &x2,
			               fpreal &y2, bool incl_buff_border = true)
				       const;

    /// Automatically alters myX, myY, myW, myH in order to snap the size of
    /// the netbox to the minimum bounding box enclosing its contents. Will
    /// ALWAYS resize the box to the minimum bounding box size, unlike
    /// resizeToFit(). Set 'incl_buff_border' to true if you want a small
    /// buffer border to make the layout look pretty.
    void		 snapToBounds(bool incl_buff_border = true);
    
    /// Save the attributes of this network box to the ostream 
    int		 	 save(std::ostream &os, const OP_SaveFlags &flags);

    /// Load the contents of the stream into the attributes of this network 
    /// box; if binary is nonzero, load in binary mode.
    /// Loading doesn't send the OP_NETWORKBOX_CREATED message to the network;
    /// the caller is responsible for doing that.
    bool 	 	 load(UT_IStream &is);
    /// Depending on the order in which netboxes are saved, we may load them
    /// in an order where an owned netbox is loaded before its owner. In this
    /// case, we need to run through all the netboxes after loading them to
    /// set up any ownerships that fall into this category.
    void		 resolveLoadedOwnerName();

    /// Returns my item type (OP_ITEMTYPE_NETWORKBOX)
    virtual OP_ItemType	 getItemType() const;

    const OP_Stat       &getStat() const        { return myStats; }
    OP_Stat             &getStat()              { return myStats; }
   
    /// Return the network i'm in
    virtual OP_Network	*getParentNetwork() const;
    
    /// Sets 'cmd' to be a string containing our values for the flags specified 
    /// by 'flags'. If 'flags' == NULL, then we output values for all of our 
    /// flags.
    void	 	 getSaveFlagsString(UT_String &cmd,
					    const char *flags,
					    bool save_to_hip) const;

    /// Used by opscript, this outputs the sequence of hscript commands
    /// necessary to recreate this network box. Does not script the box's
    /// contents. 'dogeneral' is set when opscript is generating a macro.
    int			 saveCommand(std::ostream &os, const char *name, 
				     int dogeneral) const;

    /// Each netbox has a unique id. This is used primarily for undos, as we
    /// also keep a list of netboxes in order of id, so lookup by id becomes
    /// quite quick.
    static OP_NetworkBox	*lookupNetworkBox(int unique_id);
    int			 	 getUniqueId() const { return myUniqueId; }
    virtual int64		 getItemUniqueId() const { return myUniqueId; } 

    /// Control the default color used on new network boxes.
    static const UT_Color	&defaultColor();
    static void			 setDefaultColor(const UT_Color *color);

    /// Clear undo flags on all network boxes.
    static void			 clearAllPendingUndoFlags();

    /// ALWAYS call this with 'resizing' == true before you do any resizing 
    /// operations (e.g. setW()), and call it after you're done resizing with 
    /// 'resizing' == false.  You should only do this once for each group of 
    /// resizing operations on the same netbox. 
    ///
    /// This method: 
    /// 1) ensures we get a proper undo block around all the resize ops, and 
    /// 2) allows you to use setXY() to stretch the box to the left (otherwise,
    ///    setXY() just moves the box)
    void		 setResizing(bool resizing);
    bool		 getResizing();

    /// Accessors for the auto fit flag on the network box.
    void		 setAutoFit(bool autofit);
    bool		 getAutoFit();

    /// Returns the amount of memory owned by this OP_NetworkBox
    int64		 getMemoryUsage(bool inclusive) const;

private:
    /// Given a delta-x and delta-y, move all our items the same amount
    void		 moveItems(fpreal dx, fpreal dy);

    /// Callback for the network's events
    static void		 netChanged(OP_Node *caller, void *callee, 
				    OP_EventType type, void *data);

    void		 saveForUndoAddItem(OP_NetworkBoxItem *item);
    void		 saveForUndoLayout();
    void		 saveForUndoMinimize();
    void		 saveForUndoRemoveItem(OP_NetworkBoxItem *item);
    void		 saveForUndoRename();
    void		 saveForUndoComment();
    void		 saveForUndoResize();
    
    void         	 clearUndoFlags();
    void         	 setAnyUndoFlag();
    bool	 	 getAnyUndoPending()    const;

    /// This should only be called by our loading mechanism while we load, right
    /// after we get created!
    void		 setUniqueId(int id);
    
    OP_Network		*myNetwork;
    UT_String	 	 myName; 
    UT_StringHolder	 myLoadedOwnerName;
    UT_StringHolder	 myComment;
    
    /// ALWAYS set these through accessors to ensure proper undo creation
    fpreal 	 	 myX, myY;
    fpreal	 	 myW, myH; 

    // CAUTION: Do not play with the ordering of these items, they should match
    // 	        in order with the items in OPUI_NetworkBox for safe and
    // 	        efficient synchronization between OP and OPUI levels
    OP_NetworkBoxItemList myItems;

    /// Keeps info like timestamps for last modification time, creation time,
    /// etc
    OP_Stat	 	 myStats;

    /// Encapsulates all our flags
    OP_NetworkBoxFlags	 myFlags;
   
    UT_AutoUndoBlock	*myUndoResizeBlock;
    
    // Note that although netboxes share their namespace with nodes, they do not
    // share their "ID space".
    int		 	 myUniqueId;

    bool		 myAnyUndoFlagSet;

    /// The height of the drag bar. This is initialized for us by the OPUI
    /// network boxes.
    static fpreal	 theDragbarHeight;
};

///////////////////////////////////////////////////////////////////////////////
//
// Method Implementations
//

template <typename F>
void
OP_NetworkBox::traverseItems(
	OP_ItemTypeMask item_mask,
	bool include_hidden,
	bool recursive,
	bool picked_only,
	const F &functor) const
{
    for (exint i = 0, n = myItems.size(); i < n; ++i)
    {
	OP_NetworkBoxItem &item = *myItems(i);

	if ((item.getItemType() & item_mask) != 0)
	{
	    if (include_hidden ||
		item.getItemType() != OP_ITEMTYPE_NODE ||
		(UTverify_cast<const OP_Node *>(myItems(i)))->getExpose())
	    {
		if (!picked_only || item.getPicked())
		    functor(item);
	    }
	}
	if (recursive && myItems(i)->getItemType() == OP_ITEMTYPE_NETWORKBOX)
	{
	    const OP_NetworkBox
		&box = *UTverify_cast<const OP_NetworkBox *>(&item);
	    box.traverseItems(item_mask, include_hidden, recursive, picked_only,
			      functor);
	}
    }
}

#endif
