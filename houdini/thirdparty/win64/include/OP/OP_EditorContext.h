/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_EditorContext.h ( OP Library, C++)
 *
 * COMMENTS:	Data structure holding information about a network editor that
 *		may affect the way nodes are represented in that pane. One such
 *		use is to track which nodes contributed to the current/display
 *		node's cooked data.
 */

#ifndef __OP_EditorContext__
#define __OP_EditorContext__

#include "OP_API.h"
#include <UT/UT_ArraySet.h>

class OP_API OP_EditorContext
{
public:
			 OP_EditorContext();
			~OP_EditorContext();

    void		 clearCookSet()
			 { myCookSet.clear(); }
    void		 addNodeToCookSet(int id)
			 { myCookSet.insert(id); }
    bool		 cookSetContainsNode(int id) const
			 { return myCookSet.contains(id); }

private:
    UT_ArraySet<int>	 myCookSet;
};

#endif

