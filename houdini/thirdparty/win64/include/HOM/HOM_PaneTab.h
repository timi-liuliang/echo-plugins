/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_PaneTab_h__
#define __HOM_PaneTab_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_EnumValue.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"


class HOM_FloatingPanel;
class HOM_Pane;


SWIGOUT(%rename(PaneTab) HOM_PaneTab;)

class HOM_API HOM_PaneTab
{
public:
    HOM_PaneTab()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_PaneTab(const HOM_PaneTab &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_PaneTab()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_PaneTab> pane_tab) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_PaneTab> pane_tab) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject pane;)
    virtual HOM_Pane *pane() = 0;

    SWIGOUT(%newobject floatingPanel;)
    virtual HOM_FloatingPanel *floatingPanel() = 0;

    virtual std::string name() = 0;
    virtual void setName(const char *name) = 0;

    virtual bool isCurrentTab() = 0;
    virtual void setIsCurrentTab() = 0;

    virtual bool isFloating() = 0;

    SWIGOUT(%newobject clone;)
    virtual HOM_PaneTab *clone() = 0;

    virtual void close() = 0;

    virtual HOM_EnumValue &type() = 0;
    SWIGOUT(%newobject setType;)
    virtual HOM_PaneTab *setType(HOM_EnumValue &type) = 0;

    virtual HOM_EnumValue &linkGroup() = 0;
    virtual void setLinkGroup(HOM_EnumValue &group) = 0;
    virtual bool isPin() = 0;
    virtual void setPin(bool pin) = 0;

    virtual std::vector<std::string> _getValueNames() = 0;
    virtual std::string _getValue(const char *name) = 0;
    virtual bool _setValue(const char *name, const char* value) = 0;
    virtual bool _pressButton(const char *name) = 0;

    virtual std::vector<int> size() = 0;
    virtual std::vector<int> contentSize() = 0;
};

#endif
