/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_NetworkBoxItem.h (OP Library, C++)
 *
 * COMMENTS: The base class for anything which can go into a OP_NetworkBox.
 *	     The only requirements for these classes are that they have an
 *	     absolute X and Y position.
 */

#ifndef __OP_NetworkBoxItem__
#define __OP_NetworkBoxItem__

#include "OP_API.h"
#include "OP_ItemId.h"
#include "OP_DataTypes.h"
#include <UT/UT_Color.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_Set.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_WorkBuffer.h>
#include <SYS/SYS_Types.h>

#define OP_NO_OWNER_NETWORK_BOX		"__NO_OWNER_NETWORK_BOX__"
#define OP_PATH_HAS_NETPATH		(true)
#define OP_PATH_HAS_NO_NETPATH		(false)
#define OP_PATH_HAS_PACKETEXT		(true)
#define OP_PATH_HAS_NO_PACKETEXT	(false)

class OP_Network;
class OP_NetworkBox;
class OP_NetworkBoxItem;
class OP_Node;
class UT_String;

typedef UT_Set<const OP_NetworkBoxItem *>	OP_EnclosedItems;	

class OP_API OP_NetworkBoxItem : public UT_NonCopyable
{
public:
			 OP_NetworkBoxItem();
    virtual		~OP_NetworkBoxItem();

    /// Get and set the position of this item. Units are absolute, as opposed
    /// to relative units found in OPUI.
    virtual void	 setXY(fpreal x, fpreal y) = 0;
    virtual fpreal	 getX() const = 0;
    virtual fpreal	 getY() const = 0;
    virtual fpreal	 getW() const = 0;
    virtual fpreal	 getH() const = 0;

    /// Set the node's location with setXY(), but first check if the new
    /// position is valid.  Also update any netboxes containing this node
    /// and send an OP_UI_MOVED event.
    bool		 setXYWithBoundsChecks(fpreal x, fpreal y);
    void		 validateSafeBounds(fpreal &x, fpreal &y);
    void		 validateSafeBounds(fpreal &x, fpreal &y,
				fpreal &w, fpreal &h);

    /// The following methods return the safe extents of the position of items.
    static fpreal	 getMaxSafeX() { return  1e6; }
    static fpreal	 getMinSafeX() { return -1e6; }
    static fpreal	 getMaxSafeY() { return  1e6; }
    static fpreal	 getMinSafeY() { return -1e6; }

    /// Set the node's location to a "good" position, using either the input
    /// or output nodes as the basis for the decision of what "good" means.
    /// The keepcentered flag tells us to move every item in our parent
    /// network _except_ us. The moveconnected flag causes this function to
    /// move items connected to the other side of this item to avoid overlaps
    /// when we move this item.
    void		 moveToGoodPosition(bool relativetoinputs = true,
				bool moveinputs = true,
				bool moveoutputs = true,
				bool moveunconnected = true);

    /// The owner box of an OP_NetworkBoxItem is the OP_NetworkBox which
    /// it's held in.
    /// If a box has no owner, its owner box will be returned as NULL.
    /// Owner box is NULL by default on construction.
    void	 	 setOwnerBox(OP_NetworkBox *box);
    OP_NetworkBox	*getOwnerBox() const;
    bool		 hasOwnerBox(OP_NetworkBoxItem *item) const;

    /// Returns the highest level owner box that is minimized. In other words,
    /// the netbox that contains this item that is visible in a network editor.
    OP_NetworkBox	*getMinimizedOwnerBox() const;

    virtual int          setPicked(int on_off, 
				bool propagate_parent_event = true) = 0;
    virtual int          getPicked() const = 0;

    ///Returns the network that is our parent
    virtual OP_Network	*getParentNetwork() const = 0;

    /// Sets 'str' to be the path from 'parent' to this item. Pass in NULL for 
    /// 'parent' if you'd like the full path returned.
    void                 getPathFromParent(UT_String &str, 
				const OP_Node *parent) const
			 {
			     UT_WorkBuffer	tmp;
			     getPathFromParent(tmp, parent);
			     tmp.copyIntoString(str);
			 }
    void		 getPathFromParent(UT_WorkBuffer &str,
				const OP_Node *parent) const;
    /// Returns the path of this item relative to 'base_item'
    void		 getRelativePathTo(const OP_NetworkBoxItem *base_item,
				UT_String &result) const;

    /// Accessors for color used in the network view.
    virtual UT_Color	 getColor() const
			 { return myColor; }
    virtual bool	 setColor(const UT_Color &color);
 
    /// Our children should implement this and return what type of item they are
    virtual OP_ItemType	 	 getItemType() const = 0;

    /// Basically the same as OP_Node::getName(), but abstracted to a higher 
    /// level so we can call it on all items.
    virtual const UT_String	&getItemName() const = 0;
    virtual bool		 setItemName(const UT_String &name) = 0;

    /// Functions to get hip-file-unique ids for any item type.
    virtual int64		 getItemUniqueId() const = 0;
    static OP_NetworkBoxItem	*lookupItem(const OP_ItemType &type, int64 id);
    static OP_NetworkBoxItem	*lookupItem(const OP_ItemId &id);

    /// Basically the same as OP_Node::getExpose(), but abstracted to a higher 
    /// level so we can call it on all items.
    virtual bool		 getItemExpose() const
				 { return true; }
    virtual void		 setItemExpose(bool expose)
				 { }

    /// getCreator is similar to getParentNetwork but it climbs the tree to
    /// find the network that originally created this network type. This 
    /// means that if you're down in a sub-network you climb right up to the
    /// parent of all the sub-nets. For SOPs this will return the object, 
    /// for COPs the icenet, for TOPs the Shader, for CHOPs the chopnet,
    /// for POPs the popnet. If the parent tree is truncated for some reason,
    /// we return whatever node is at the top.
    OP_Network			*getCreator() const;

    /// Get the first parent node which has a VOP_CodeGenerator (i.e. has
    /// getVopCodeGenerator() defined.
    OP_Network			*getCodeCreator() const;

    /// Fills an array with all network box items connected to this item.
    void			 getConnectedItems(
					OP_NetworkBoxItemList &connected,
					bool inputs, bool recursive,
					bool include_this_item) const;

    /// Returns the amount of memory owned by this OP_NetworkBoxItem
    /// (JUST this class; not subclasses)
    int64			 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

protected:
    /// Allow subclasses direct access to myColor to allow them to edit this
    /// color without risk of generating undo blocks or change events.
    UT_Color			 myColor;

private:
    class ItemCycle;

    /// Returns all the node and indirect input items contained by this
    /// network box, and any network boxes contained by this network box.
    void		 getContainedItems(OP_NetworkBoxItemList &items) const;

    /// Fills an array with all network box items connected to this item.
    /// Deals with network boxes in a way that prevents issues of items
    /// being double-moved.
    void		 getConnectedItemsForMove(
				OP_NetworkBoxItemList &items,
				bool inputs, bool recursive) const;

    /// Helper functions for the above two functions.
    static void		 addItemToTable(const OP_NetworkBoxItem *item,
				UT_Set<const OP_NetworkBoxItem *> &connected,
				OP_NetworkBoxItemList *connectedlist,
				bool inputs, bool recursive,
				ItemCycle &cycle);
    static void		 getConnectedItems(const OP_NetworkBoxItem *item,
				UT_Set<const OP_NetworkBoxItem *> &connected,
				OP_NetworkBoxItemList *connectedlist,
				bool inputs, bool recursive,
				ItemCycle &cycle);
    static void		 getContainedItems(const OP_NetworkBox *box,
				OP_NetworkBoxItemList &items);

    OP_NetworkBox		*myOwnerBox;
};

#endif

