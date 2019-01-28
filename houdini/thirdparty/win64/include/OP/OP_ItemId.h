/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_ItemId.h (OP Library, C++)
 *
 * COMMENTS: Simple encapsulation of the two values needed to uniquely
 *	     identify any OP_NetworkBoxItem in a Houdini session.
 */

#ifndef __OP_ItemId__
#define __OP_ItemId__

#include "OP_API.h"
#include <UT/UT_Array.h>

// These constants come in handy when a client is storing OP_Node ids and 
// then polling for a node using OP_Node::lookupNode(id), and they need to 
// specify that their stored id is currently invalid,  i.e. if the
// corresponding node was deleted. This can obviously also be used for
// other things
#define OP_INVALID_ITEM_ID	-999
#define OP_INVALID_NODE_ID 	OP_INVALID_ITEM_ID

class OP_NetworkBoxItem;

enum OP_ItemType
{
    OP_ITEMTYPE_NODE 		= 0x00000001,	// OP_Node
    OP_ITEMTYPE_INDIRECT	= 0x00000002,	// OP_SubnetIndirectInput
    OP_ITEMTYPE_NETWORKBOX	= 0x00000004,	// OP_NetworkBox
    OP_ITEMTYPE_POSTIT_NOTE	= 0x00000008,	// OP_PostIt
    OP_ITEMTYPE_DOT		= 0x00000010,	// OP_Dot

    OP_ITEMTYPE_INVALID		= 0x00000000,
    OP_ITEMTYPE_ALL		= 0xFFFFFFFF,
    OP_ITEMTYPE_ALL_BUT_INDIRECT= 0xFFFFFFFD
};

/// This mask is used as a type to encompass all possible combinations of 
/// OP_ItemType. 
typedef unsigned int 		OP_ItemTypeMask;

class OP_API OP_ItemId
{
public:
			 OP_ItemId();
			 OP_ItemId(OP_ItemType item_type, int64 item_id);
			 OP_ItemId(const OP_NetworkBoxItem &item);
			 OP_ItemId(const OP_NetworkBoxItem *item);

    bool		 operator==(const OP_ItemId &other) const
			 { return myItemType == other.myItemType &&
				  myItemId == other.myItemId; }
    bool		 operator!=(const OP_ItemId &other) const
			 { return myItemType != other.myItemType ||
				  myItemId != other.myItemId; }

    bool		 isValid() const
			 { return myItemType != OP_ITEMTYPE_INVALID &&
				  myItemId != OP_INVALID_ITEM_ID; }

    OP_ItemType		 myItemType;
    int64		 myItemId;
};

inline uint64 hash_value(const OP_ItemId &id)
{ return SYSwang_inthash64(id.myItemType) ^ SYSwang_inthash64(id.myItemId); }

typedef UT_Array<OP_ItemId>	OP_ItemIdList;	

#endif

