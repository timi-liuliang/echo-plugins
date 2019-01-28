/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Pane_h__
#define __HOM_Pane_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_EnumValue.h"
#include "HOM_PaneTab.h"
class HOM_Desktop;
class HOM_FloatingPanel;

SWIGOUT(%rename(Pane) HOM_Pane;)

class HOM_API HOM_Pane
{
public:
    HOM_Pane()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Pane(const HOM_Pane &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Pane()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject desktop;)
    virtual HOM_Desktop *desktop() = 0;

    virtual int id() = 0;

    virtual bool isMaximized() = 0;

    SWIGOUT(%newobject floatingPanel;)
    virtual HOM_FloatingPanel *floatingPanel() = 0;

    virtual void setIsMaximized( bool on ) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_PaneTab> > tabs() = 0;

    SWIGPYTHONOUT(%newobject createTab;)
    virtual HOM_PaneTab *createTab(HOM_EnumValue &type) = 0;

    SWIGOUT(%newobject currentTab;)
    virtual HOM_PaneTab *currentTab() = 0;

    SWIGOUT(%newobject tabOfType;)
    virtual HOM_PaneTab *tabOfType(HOM_EnumValue &type, int index = 0) = 0;

    SWIGOUT(%newobject splitHorizontally;)
    virtual HOM_Pane *splitHorizontally() = 0;

    SWIGOUT(%newobject splitVertically;)
    virtual HOM_Pane *splitVertically() = 0;

    SWIGOUT(%newobject getSplitParent;)
    virtual HOM_Pane *getSplitParent() = 0;

    SWIGOUT(%newobject getSplitChild;)
    virtual HOM_Pane *getSplitChild(int child) = 0;

    virtual void splitSwap() = 0;

    virtual void splitRotate() = 0;

    virtual bool isSplitMaximized() = 0;

    virtual bool isSplitMinimized() = 0;

    virtual void setIsSplitMaximized(bool on) = 0;

    virtual bool isSplit() = 0;

    virtual void setSplitFraction(double value) = 0;

    virtual double getSplitFraction( ) = 0;

    virtual void setSplitDirection(int dir) = 0;

    virtual int getSplitDirection( ) = 0;
};

#endif
