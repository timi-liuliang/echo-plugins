/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_FloatingPanel_h__
#define __HOM_FloatingPanel_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_EnumValue.h"
#include <string>
#include <vector>

class HOM_ShelfDock;
class HOM_Pane;
class HOM_PaneTab;
class HOM_Vector2;

SWIGOUT(%rename(FloatingPanel) HOM_FloatingPanel;)

class HOM_API HOM_FloatingPanel
{
public:
    HOM_FloatingPanel()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_FloatingPanel(const HOM_FloatingPanel &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_FloatingPanel()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Pane> > panes() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_PaneTab> > paneTabs() = 0;

    SWIGOUT(%newobject paneTabOfType;)
    virtual HOM_PaneTab *paneTabOfType(HOM_EnumValue &type, int index = 0) = 0;

    SWIGOUT(%newobject findPaneTab;)
    virtual HOM_PaneTab *findPaneTab(const char *name) = 0;

    virtual std::string name() = 0;

    virtual void setName(const char *name) = 0;

    virtual bool containsPlaybar() = 0;
    virtual void setContainsPlaybar(bool on) = 0;

    virtual bool containsShelf() = 0;
    virtual void setContainsShelf(bool on) = 0;

    virtual bool containsStatusBar() = 0;
    virtual void setContainsStatusBar(bool on) = 0;

    virtual bool containsMenuBar() = 0;
    virtual void setContainsMenuBar(bool on) = 0;

    virtual bool isAttachedToDesktop() = 0;
    virtual void attachToDesktop(bool on) = 0;

    virtual bool isFullscreen() = 0;
    virtual void setIsFullscreen(bool on) = 0;

    SWIGOUT(%newobject position;)
    virtual HOM_Vector2 *position() = 0;
    virtual void setPosition(const std::vector<int> &position) 
	= 0;
    SWIGOUT(%newobject size;)
    virtual HOM_Vector2 *size() = 0;
    virtual void setSize(const std::vector<int> &size) 
	= 0;

    virtual void close() = 0;
};

#endif
