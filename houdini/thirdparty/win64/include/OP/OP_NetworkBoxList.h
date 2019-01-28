/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_NetworkBoxList.h (OP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OP_NetworkBoxList__
#define __OP_NetworkBoxList__

#include "OP_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_ValArray.h>
#include <iosfwd>

#include "OP_Error.h"
#include "OP_Node.h"

class OP_Network;
class OP_NetworkBox;
class OP_NetworkBoxItem;
class OP_SaveFlags;

class OP_API OP_NetworkBoxList : public UT_ValArray<OP_NetworkBox *>
{
public:
    OP_NetworkBoxList(OP_Network *net);
    // NOTE: We don't need a virtual destructor, because our superclass
    //       has a non-virtual destructor, and this is only destructed
    //       as type OP_NetworkBoxList.
    ~OP_NetworkBoxList();

    /// Remove all network boxes from this list
    void	   clearAndDestroy();

    /// Create a network box with the specified name and insert it into this 
    /// OP_NetworkBoxList at the next available index. Returns a pointer to the
    /// OP_NetworkBox created or NULL if creation was unsuccessful. Pass in NULL
    /// as the name if you'd like a default name. If a duplicate name is passed 
    /// in, it will be altered to make it unique. Passing in 'items' will 
    /// automatically create a network box containing those items with the 
    /// netbox positioned properly around them. 
    OP_NetworkBox *createNetworkBox(const char *name, 
				    OP_NetworkBoxItemList *items = nullptr);

    /// Find and return a given network box by name, or NULL if it's not found
    OP_NetworkBox *findNetworkBox(const char *name) const;

    /// Find the index of a given network box by name. Returns -1 if not found
    int		   findNetworkBoxIndex(const char *name) const;

    /// Find the index of a given network box by ptr. Returns -1 if not found
    int		   findNetworkBoxIndex(OP_NetworkBox *netbox) const;

    /// Remove a network box by ptr. Returns true if it was successfully
    /// removed; false if the box wasn't found
    bool 	   deleteNetworkBox(OP_NetworkBox *netbox);

    /// Remove a network box by index. Returns true if it was successfully
    /// removed; false if the index is invalid
    bool 	   deleteNetworkBox(int index);

    /// Save the entire list to the ostream os. 'path_prefix' should be the path
    /// you want to save the netboxes as being under.
    OP_ERROR	   save(std::ostream &os, const OP_SaveFlags &flags,
		        const char *path_prefix) const;

    ///Load a network box from 'is'. Can load from both .hip files and the 
    ///clipboard. 'path' is used as the new netbox's name (only the portion of 
    ///path after the last slash). If 'overwrite' is specified, the new netbox 
    ///will overwrite any other netboxes in this network with the same name, 
    ///rather than modify its name to be unique. 'created' is used to return a 
    ///ptr to the new network box.
    bool	   load(UT_IStream &is, const char *path, 
			bool path_contains_network,
			bool path_contains_packet_ext,
		        int overwrite = 0, OP_NetworkBox **created = NULL);

    ///Saves all netboxes in the provided list to the clipboard. Returns the 
    ///total # of items saved.
    int		   saveListToClipboard(std::ostream &os,
			const OP_SaveFlags &flags,
			const UT_ValArray<OP_NetworkBox *> 
			&netboxes) const;

    ///Save 'netbox' to 'os'. 'path_prefix' should be the path you want to save 
    ///the netbox as being under.
    OP_ERROR	   saveSingleNetworkBox(std::ostream &os,
			OP_NetworkBox &netbox, 
			const OP_SaveFlags &flags,
			const char *path_prefix) const;

    /// Return the amount of memory owned by this OP_NetworkBoxList
    int64        getMemoryUsage(bool inclusive) const;

private:
    
    OP_Network	  *myNetwork;
};

#endif
