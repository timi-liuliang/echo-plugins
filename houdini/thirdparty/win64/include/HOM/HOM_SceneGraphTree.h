/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_SceneGraphTree_h__
#define __HOM_SceneGraphTree_h__

#include "HOM_PathBasedPaneTab.h"
#include "HOM_EnumModules.h"

SWIGOUT(%rename(SceneGraphTree) HOM_SceneGraphTree;)

class HOM_API HOM_SceneGraphTree : virtual public HOM_PathBasedPaneTab
{
public:
    HOM_SceneGraphTree()
    { HOM_CONSTRUCT_OBJECT(this) }

    // Because the lowermost base classes initialize the virtual bases
    // before any non-virtual bases, the correct thing to do here is
    // explicitly call the constructor for HOM_PaneTab.
    HOM_SceneGraphTree(const HOM_SceneGraphTree &pane)
    : HOM_PaneTab(pane), HOM_PathBasedPaneTab(pane)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_SceneGraphTree()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)
};

#endif
