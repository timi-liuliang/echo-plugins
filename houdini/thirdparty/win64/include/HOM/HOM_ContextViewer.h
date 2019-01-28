/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ContextViewer_h__
#define __HOM_ContextViewer_h__

#include "HOM_PathBasedPaneTab.h"

class HOM_CompositorViewer;
class HOM_SceneViewer;

SWIGOUT(%rename(ContextViewer) HOM_ContextViewer;)

class HOM_API HOM_ContextViewer : virtual public HOM_PathBasedPaneTab
{
public:
    HOM_ContextViewer()
    { HOM_CONSTRUCT_OBJECT(this) }

    // Because the lowermost base classes initialize the virtual bases
    // before any non-virtual bases, the correct thing to do here is
    // explicitly call the constructor for HOM_PaneTab.
    HOM_ContextViewer(const HOM_ContextViewer &pane)
    : HOM_PaneTab(pane), HOM_PathBasedPaneTab(pane)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_ContextViewer()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    SWIGOUT(%newobject sceneViewer;)
    virtual HOM_SceneViewer *sceneViewer() = 0;

    SWIGOUT(%newobject compositorViewer;)
    virtual HOM_CompositorViewer *compositorViewer() = 0;
};

#endif
