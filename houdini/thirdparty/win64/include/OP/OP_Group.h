/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP library (C++)
 *
 * COMMENTS:	A group is defined by a name and an offset into the
 *		individual group bit maps which are stored with each node.
 *		Groups should only be created via an OP_GroupList which
 *		maintains the bitmap offsets. Currently we only support
 *		one group list per network.
 *
 * BUGS:
 *
 */

#ifndef __OP_Group_h__
#define __OP_Group_h__

#include "OP_API.h"
#include <UT/UT_Color.h>
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class OP_Network;
class OP_Node;
class OP_GroupList;

class OP_API OP_Group
{
public:
    static char	 	getGroupChar()		{ return '@'; }

    const UT_String	&getName() const	{ return myName; }
    void		 setName(const char *n);

    int		  	 isInternal() const;

    // these methods used for the UI of group tiles
    // The picked flag is different between lists and worksheets, so use
    // the extra "list" parameter to distinguish between the two.
    int		 	 setPicked(int on_off, int list = 0);
    int			 getPicked(int list = 0) const;
    void		 setXY(fpreal x, fpreal y);
    fpreal		 getX() const { return myPosX; }
    fpreal		 getY() const { return myPosY; }


    // The next two methods should be used only by OP_UndoGroup
    short		 getFlags() { return myFlags; }
    void		 setFlags(short f) { myFlags = f; }

    const UT_Color	&getColor(void) const	     { return myColor; }
    void		 setColor(const UT_Color &col) { myColor = col; }

    void	  	 saveGroupForUndo();
    int			 saveContents(std::ostream &os, int binary=0);
    bool		 loadContents(UT_IStream &is);
    void	  	 clearUndoFlags()	{ mySavedForUndoFlag = 0; }

    // these flags operate on all the members of this group
    void		 setFlag(char flagchar, int onoff);
    void		 setDisplay(int onoff);
    void		 setSelectable(int onoff);
    void		 setTemplate(int onoff);
    void		 setExposed(int onoff);
    void		 setSelected(int onoff);
    void		 setExport(int onoff);
    void		 setThumbnail(int onoff);

//
//  clear will remove all members from the group.
//  It returns the number of nodes that were removed.
//
    int		 	 clear(int send_change=1);
//
//  The pattern methods allow addition and subtraction of members based
//  on a pattern string match by name. They each return the number of
//  elements added or subtracted from the group.
//
    int		 addPattern(const char *pat);
    int		 subPattern(const char *pat);
    int		 setPattern(const char *pat) { clear(); return addPattern(pat); }
//
//  addMember and subMember will add and remove an individual node from
//  the group. They return one upon success and zero upon failure (i.e.
//  the node is null or not a member of the groups network.) If the
//  send_change flag is set an OP_GROUP_CHANGED event is generated.
//  
    int		 addMember(OP_Node *node, int send_change=1);
    int		 subMember(OP_Node *node, int send_change=1);
//
//  These methods add and remove the currently picked nodes in the network
//  to or from the group. They return the number of nodes affected. If
//  'or_in_picked_netbox' is set to true, then a node is also considered if it's
//  unpicked but in a picked netbox.
//
    int		 addPickedNodes(bool or_in_picked_netbox = false);
    int		 subPickedNodes(bool or_in_picked_netbox = false);

//  These methods are for getting picked roots and such
//
    int		 subNonPickedRootNodes();

//
//  isMember returns a boolean value indicating whether the given node
//  is contained in this group or not. The "contains" method is simply
//  an alias for "isMember" and is provided here to be consistent with
//  the GB_Group class.
//
    int		 isMember(const OP_Node *node) const;
    int		 contains(const OP_Node *node) const
		 { return isMember(node); }
//
//  getPattern returns a string pattern that matches all the elements of
//  this group. Currently no collapsing is done. i.e. It returns a list
//  of all the member names or "*" if the entire net is in the group.
//  The return value is the number of elements in the group.
//
    int		 getPattern(UT_String &str) const;
//
//  getMembers will provide an array of pointers to the group members.
//  The return value is the number of elements in the group.
//
    int		 getMembers(UT_ValArray<OP_Node *> &list, int avoid_dups=0) const;
    int		 countMembers() const;

    OP_Network	*getNetwork()	{ return myNetwork; }

//
//  sendGroupChange should be used instead of simply sending an
//  OP_GROUP_CHANGED event.
//
    void	 sendGroupChange();

    /// Return the amount of memory owned by this OP_Group
    int64        getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myName.getMemoryUsage(false);
        return mem;
    }

private:
    friend class OP_GroupList;
//
//   Private constructor avoids misuse. OP_GroupList is a friend of ours.
//
     OP_Group(const char *name, OP_Network *net);
    ~OP_Group();

//
//  The group index is the offset into the group bitmaps that this group
//  is using. This value should only be set by the OP_GroupList class.
//

    void	 setGroupIndex(exint index) { myGroupIndex = index; }
    exint	 getGroupIndex() const	    { return myGroupIndex; }
//
//  The initialize method is used to set all the bits in each node for
//  this group to an initial state. This is done when new groups are
//  created.
//
    int		 initialize(int state=0);

    UT_String			 myName;

    UT_Color			 myColor;		// For UI
    fpreal			 myPosX;	
    fpreal			 myPosY;

    OP_Network			*myNetwork;

    exint			 myGroupIndex;

    char			 mySavedForUndoFlag;
    short			 myFlags;
};

#endif
