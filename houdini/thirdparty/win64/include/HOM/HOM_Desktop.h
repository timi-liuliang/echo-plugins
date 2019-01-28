/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Desktop_h__
#define __HOM_Desktop_h__

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
class HOM_FloatingPanel;

SWIGOUT(%rename(Desktop) HOM_Desktop;)

class HOM_API HOM_Desktop
{
public:
    HOM_Desktop()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Desktop(const HOM_Desktop &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Desktop()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Pane> > panes() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_PaneTab> > paneTabs() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_PaneTab> > currentPaneTabs() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_PaneTab> > floatingPaneTabs() = 0;

    SWIGOUT(%newobject paneTabOfType;)
    SWIGOUT(%kwargs paneTabOfType;)
    virtual HOM_PaneTab *paneTabOfType(HOM_EnumValue &type, int index=0) = 0;

    SWIGOUT(%newobject findPane;)
    virtual HOM_Pane *findPane(int pane_id) = 0;

    SWIGOUT(%newobject findPaneTab;)
    virtual HOM_PaneTab *findPaneTab(const char *name) = 0;

    SWIGOUT(%newobject createFloatingPaneTab;)
    SWIGOUT(%kwargs createFloatingPaneTab;)
    virtual HOM_PaneTab *createFloatingPaneTab(
            HOM_EnumValue &pane_tab_type,
            const std::vector<int> &position = std::vector<int>(),
            const std::vector<int> &size = std::vector<int>(),
	    const char *python_panel_interface=nullptr) = 0;

    // This method is deprecated:
    SWIGOUT(%newobject createFloatingPane;)
    HOM_PaneTab *createFloatingPane(
            HOM_EnumValue &pane_tab_type,
            const std::vector<int> &position = std::vector<int>(),
            const std::vector<int> &size = std::vector<int>())
    { return createFloatingPaneTab(pane_tab_type, position, size); }

    virtual std::vector<HOM_ElemPtr<HOM_FloatingPanel> > floatingPanels() = 0;

    SWIGOUT(%newobject createFloatingPanel;)
    SWIGOUT(%kwargs createFloatingPanel;)
    virtual HOM_FloatingPanel *createFloatingPanel(HOM_EnumValue &pane_tab_type,
		    const std::vector<int> &position = std::vector<int>(),
		    const std::vector<int> &size = std::vector<int>(),
		    const char *python_panel_interface=nullptr) = 0;

    SWIGOUT(%newobject paneUnderCursor;)
    virtual HOM_Pane *paneUnderCursor() = 0;
    SWIGOUT(%newobject paneTabUnderCursor;)
    virtual HOM_PaneTab *paneTabUnderCursor() = 0;

    
    virtual std::string name() = 0;

    SWIGOUT(%newobject shelfDock;)
    virtual HOM_ShelfDock *shelfDock() = 0;

    SWIGOUT(%newobject displaySideHelp;)
    virtual HOM_PaneTab *displaySideHelp(bool show = true) = 0;
    virtual void displayHelp(HOM_NodeType &node_type) = 0;
    virtual void displayHelpPath(const char *help_path) = 0;
    virtual void displayHelpPyPanel(const char *interface_name) = 0;

    virtual void setAsCurrent() = 0;
};

#endif
