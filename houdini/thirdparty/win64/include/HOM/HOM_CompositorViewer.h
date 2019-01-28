/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_CompositorViewer_h__
#define __HOM_CompositorViewer_h__

#include "HOM_PathBasedPaneTab.h"
#include "HOM_EnumModules.h"

SWIGOUT(%rename(CompositorViewer) HOM_CompositorViewer;)

class HOM_API HOM_CompositorViewer : virtual public HOM_PathBasedPaneTab
{
public:
    HOM_CompositorViewer()
    { HOM_CONSTRUCT_OBJECT(this) }

    // Because the lowermost base classes initialize the virtual bases
    // before any non-virtual bases, the correct thing to do here is
    // explicitly call the constructor for HOM_PaneTab.
    HOM_CompositorViewer(const HOM_CompositorViewer &pane)
    : HOM_PaneTab(pane), HOM_PathBasedPaneTab(pane)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_CompositorViewer()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual std::string currentState() = 0;

    virtual void enterViewState(bool wait_for_exit = false) = 0;

    virtual void setCurrentState(const char *state, bool wait_for_exit = false) = 0;
};

#endif
