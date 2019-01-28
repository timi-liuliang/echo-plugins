/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_DataTree_h__
#define __HOM_DataTree_h__

#include "HOM_PaneTab.h"

SWIGOUT(%rename(DataTree) HOM_DataTree;)

class HOM_API HOM_DataTree : virtual public HOM_PaneTab
{
public:
    HOM_DataTree()
    { HOM_CONSTRUCT_OBJECT(this) }
    // Because the lowermost base classes initialize the virtual bases
    // before any non-virtual bases, the correct thing to do here is
    // explicitly call the constructor for HOM_PaneTab.
    HOM_DataTree(const HOM_DataTree &pane)
    : HOM_PaneTab(pane)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_DataTree()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual std::vector<std::string> treeTypes() = 0;
    virtual std::string treeType() = 0;
    virtual bool setTreeType(const std::string &tree_type) = 0;

    virtual std::string currentPath() = 0;
    virtual void clearCurrentPath() = 0;
    virtual void setCurrentPath(const std::string &path,
				bool multi_select = false,
				int index = -1) = 0;
    virtual void setCurrentPaths(const std::vector<std::string> &paths,
				 bool expanded) = 0;
    virtual void setTreeExpanded(bool expanded) = 0;
    virtual void setCurrentNodeExpanded(bool expanded) = 0;
};

#endif
