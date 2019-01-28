/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_DotList.h (OP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OP_DotList__
#define __OP_DotList__

#include "OP_API.h"
#include "OP_Error.h"
#include "OP_SaveFlags.h"
#include <UT/UT_Array.h>
#include <iosfwd>

class UT_IStream;
class OP_Network;
class OP_Input;
class OP_Dot;

class OP_API OP_DotList
{
    using		 DotArray = UT_Array<OP_Dot *>;

public:
			 OP_DotList(OP_Network &net);
			~OP_DotList();

    exint		 entries() const
			 { return myDots.entries(); }
    OP_Dot		*operator()(exint idx) const
			 { return (idx >= 0 && idx < entries())
				? myDots(idx)
				: nullptr; }
    exint		 find(OP_Dot *dot) const
			 { return myDots.find(dot); }

    using		 iterator = DotArray::iterator;
    using		 const_iterator = DotArray::const_iterator;

    iterator		 begin() { return myDots.begin(); }
    iterator		 end() { return myDots.end(); }
    const_iterator	 begin() const { return myDots.begin(); }
    const_iterator	 end() const { return myDots.end(); }

    OP_Dot		*findDot(const char *name) const;
    OP_Dot		*createDot(bool save_undo = true);
    bool		 deleteDot(OP_Dot &dot_to_delete,
				bool save_undo = true);
    void		 clearAndDestroy();

    /// Save a single dot into a CPIO packet.
    OP_ERROR		 save(std::ostream &os,
				OP_Dot &dot,
				const OP_SaveFlags &flags,
				const char *path_prefix) const;

    /// Load a network box from 'is'. Can load from both .hip files and the 
    /// clipboard. 'path' is used as the new netbox's name (only the portion of 
    /// path after the last slash). If 'overwrite' is specified, the new netbox 
    /// will overwrite any other netboxes in this network with the same name, 
    /// rather than modify its name to be unique. 'created' is used to return a 
    /// ptr to the new network box.
    bool		 load(UT_IStream &is, const char *path,
				bool path_contains_network,
				bool path_contains_packet_ext,
				int overwrite = 0, OP_Dot **created = NULL);

    /// Return the amount of memory owned by this OP_DotList
    int64		 getMemoryUsage(bool inclusive) const;

private:
    bool		 deleteDot(int idx, bool save_undo = true);

    DotArray		 myDots;
    OP_Network		&myNetwork;
};

#endif
