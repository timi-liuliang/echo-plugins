/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_PostItNoteList.h (OP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OP_PostItNoteList__
#define __OP_PostItNoteList__

#include "OP_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_ValArray.h>

#include "OP_Error.h"
#include "OP_Node.h"

class OP_Network;
class OP_PostIt;
class OP_PostItNoteItem;
class OP_SaveFlags;

class OP_API OP_PostItNoteList : public UT_ValArray<OP_PostIt *>
{
public:
    typedef UT_ValArray<OP_PostIt *> Base;

    OP_PostItNoteList(OP_Network *net);
    // NOTE: We don't need a virtual destructor, because our superclass
    //       has a non-virtual destructor, and this is only destructed
    //       as type OP_PostItNoteList.
    ~OP_PostItNoteList();

    /// Remove all network boxes from this list
    void	   clearAndDestroy();

    /// Create a network box with the specified name and insert it into this 
    /// OP_PostItNoteList at the next available index. Returns a pointer to the
    /// OP_PostIt created or NULL if creation was unsuccessful. Pass in NULL
    /// as the name if you'd like a default name. If a duplicate name is passed 
    /// in, it will be altered to make it unique. Passing in 'items' will 
    /// automatically create a network box containing those items with the 
    /// netbox positioned properly around them. 
    OP_PostIt *createPostItNote(const char *name);

    /// Find and return a given network box by name, or NULL if it's not found
    OP_PostIt *findPostItNote(const char *name) const;

    /// Find the index of a given network box by name. Returns -1 if not found
    int		   findPostItNoteIndex(const char *name) const;

    /// Find the index of a given network box by ptr. Returns -1 if not found
    int		   findPostItNoteIndex(OP_PostIt *netbox) const;

    /// Remove a network box by ptr. Returns true if it was successfully
    /// removed; false if the box wasn't found
    bool 	   deletePostItNote(OP_PostIt *netbox);

    /// Remove a network box by index. Returns true if it was successfully
    /// removed; false if the index is invalid
    bool 	   deletePostItNote(int index);

    /// Load a network box from 'is'. Can load from both .hip files and the 
    /// clipboard. 'path' is used as the new netbox's name (only the portion of 
    /// path after the last slash). If 'overwrite' is specified, the new netbox 
    /// will overwrite any other netboxes in this network with the same name, 
    /// rather than modify its name to be unique. 'created' is used to return a 
    /// ptr to the new network box.
    bool	   load(UT_IStream &is, const char *path,
			bool path_contains_network,
			bool path_contains_packet_ext,
		        int overwrite = 0, OP_PostIt **created = NULL);

    /// Return the amount of memory owned by this OP_PostItNoteList
    int64        getMemoryUsage(bool inclusive) const;

private:
    OP_Network	*myNetwork;
};

#endif
