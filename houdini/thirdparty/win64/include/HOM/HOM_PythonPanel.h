/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_PythonPanel_h__
#define __HOM_PythonPanel_h__

#include "HOM_PathBasedPaneTab.h"

class HOM_PythonPanelInterface;

SWIGOUT(%rename(PythonPanel) HOM_PythonPanel;)

class HOM_API HOM_PythonPanel : virtual public HOM_PathBasedPaneTab
{
public:
    HOM_PythonPanel()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_PythonPanel(const HOM_PythonPanel &pane)
    : HOM_PaneTab(pane), HOM_PathBasedPaneTab(pane)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_PythonPanel()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual HOM_PythonPanelInterface* activeInterface() = 0;
    virtual void setActiveInterface(HOM_PythonPanelInterface *interface) = 0;
    virtual void showToolbar(bool show) = 0;
    virtual bool isToolbarShown() = 0;
    virtual void expandToolbar(bool expand) = 0;
    virtual bool isToolbarExpanded() = 0;

    virtual void setLabel(const char* label) = 0;

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    SWIGOUT(%ignore _setOpaqueActiveInterfaceRootWidget;)
    virtual void _setOpaqueActiveInterfaceRootWidget(void *root_widget) = 0;
    SWIGOUT(%ignore _opaqueActiveInterfaceRootWidget;)
    virtual void * _opaqueActiveInterfaceRootWidget() = 0;


#ifdef SWIG
%extend
{
    // Stash the active interface's root widget.
    // This method is intended to be called internally by FUSE_PythonPanel.
    // It is not meant to be exposed to the user.
    virtual void __setActiveInterfaceRootWidget__(InterpreterObject root_widget)
    {
	self->_setOpaqueActiveInterfaceRootWidget(root_widget);
    }

    // Return the active interface's root widget.
    virtual InterpreterObject activeInterfaceRootWidget()
    {
	return HOMincRef((InterpreterObject)self->_opaqueActiveInterfaceRootWidget());
    }
}
#endif

};
#endif
