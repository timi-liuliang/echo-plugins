/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_RadialSubmenu_h__
#define __HOM_RadialSubmenu_h__

#include "HOM_RadialItem.h"

class HOM_RadialScriptItem;

SWIGOUT(%rename(RadialSubmenu) HOM_RadialSubmenu;)

class HOM_API HOM_RadialSubmenu : virtual public HOM_RadialItem
{
public:
    HOM_RadialSubmenu()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_RadialSubmenu(const HOM_RadialSubmenu &item)
    : HOM_RadialItem(item)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_RadialSubmenu()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual const char *label() = 0;

    virtual void setLabel(const char *s) = 0;

    SWIGOUT(%newobject item;)
    virtual HOM_RadialItem *item(HOM_EnumValue &location) = 0;

    virtual std::map<HOM_EnumValue, HOM_ElemPtr<HOM_RadialItem> > items() = 0;

    SWIGOUT(%newobject createSubmenu;)
    SWIGOUT(%kwargs createSubmenu;)
    virtual HOM_RadialSubmenu *createSubmenu(HOM_EnumValue &location,
					     const char *label=nullptr) = 0;

    SWIGOUT(%newobject createScriptItem;)
    SWIGOUT(%kwargs createScriptItem;)
    virtual HOM_RadialScriptItem *
		createScriptItem(HOM_EnumValue &location,
				 const char *label=nullptr,
				 const char *icon=nullptr,
				 const char *check=nullptr,
				 const char *script=nullptr) = 0;
};

#endif
