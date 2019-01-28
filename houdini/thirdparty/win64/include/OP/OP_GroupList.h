/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP library (C++)
 *
 * COMMENTS:	
 *
 * BUGS:
 *
 */

#ifndef __OP_GroupList_h__
#define __OP_GroupList_h__

#include "OP_API.h"
#include "OP_Error.h"
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class OP_Node;
class OP_Network;
class OP_Group;

class OP_API OP_GroupList : public UT_ValArray<OP_Group *>
{
public:
    typedef UT_ValArray<OP_Group *> Base;

    OP_GroupList(OP_Network *owner);
    // NOTE: We don't need a virtual destructor, because our superclass
    //       has a non-virtual destructor, and this is only destructed
    //       as type OP_GroupList.
    ~OP_GroupList();

    void	 clearAndDestroy();	// Destroy all groups

    OP_Group	*createGroup(const char *name, fpreal x = .0F, fpreal y = .0F);
    OP_Group	  *findGroup(const char *name) const;
    int		 deleteGroup(const char *name);
    int		 deleteGroup(OP_Group *group);
//
//  getMembers will provide an array of pointers to all groups in the list.
//  If the internal flag is zero this will not include internal groups.
//
    int		 getMembers(UT_ValArray<OP_Group *> &list, int internal=0) const;
//
//  getMemberships will provide an array of pointers to the groups that
//  the specified node is a member of. The return value is the number of
//  groups returned. If the internal flag is set the list will include
//  internal groups.
//
    int		 getMemberships(const OP_Node *node,
    				UT_ValArray<OP_Group *> &list,
				int internal=0) const;

//
//  saveCount returns the number of groups that would be saved if you
//  performed a save operation. This does not count internal groups.
//
    int		 saveCount() const;
    void	 saveToClipboard(std::ostream &os, int all=0, int list=0) const;
    bool	 loadFromClipboard(UT_IStream &is, const char *path) const;
    OP_ERROR	 save(std::ostream &os, int binary = 0);
    bool	 load(UT_IStream &is, int merge = 0);

    void	 clearUndoFlags();

    /// Return the amount of memory owned by this OP_GroupList
    int64        getMemoryUsage(bool inclusive) const;

private:
    OP_Network	*myNetwork;
};

#endif
