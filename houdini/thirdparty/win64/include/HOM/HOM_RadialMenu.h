/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_RadialMenu_h__
#define __HOM_RadialMenu_h__

#include "HOM_RadialSubmenu.h"

SWIGOUT(%rename(RadialMenu) HOM_RadialMenu;)

class HOM_API HOM_RadialMenu : virtual public HOM_RadialSubmenu
{
public:
    HOM_RadialMenu()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_RadialMenu(const HOM_RadialMenu &item)
    : HOM_RadialItem(item), HOM_RadialSubmenu(item)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_RadialMenu()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string name() = 0;

    virtual const char *categories() = 0;

    virtual void setCategories(const std::string &s) = 0;

    virtual const char *sourceFile() = 0;

    virtual void save(const std::string &s) = 0;
};

#endif
