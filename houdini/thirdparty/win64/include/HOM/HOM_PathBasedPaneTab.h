/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_PathBasedPaneTab_h__
#define __HOM_PathBasedPaneTab_h__

#include "HOM_PaneTab.h"
class HOM_Node;

SWIGOUT(%rename(PathBasedPaneTab) HOM_PathBasedPaneTab;)

class HOM_API HOM_PathBasedPaneTab : virtual public HOM_PaneTab
{
public:
    HOM_PathBasedPaneTab()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_PathBasedPaneTab(const HOM_PathBasedPaneTab &pane)
    : HOM_PaneTab(pane)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_PathBasedPaneTab()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    // These methods deal with the pane's network, not the current node
    // inside that network.  The path gadget on the pane will display the
    // network's path.
    SWIGOUT(%newobject pwd;)
    virtual HOM_Node *pwd() = 0;
    virtual void setPwd(HOM_Node &node) = 0;
    virtual void cd(const char *path) = 0;

    // These methods deal with the current node inside the pane.  For example,
    // the current node inside a network editor pane is the one that appears
    // in an (unpinned) parameter pane.
    virtual HOM_Node *currentNode() = 0;
    virtual void setCurrentNode(HOM_Node &node, bool pick_node = true) = 0;
};

#endif
