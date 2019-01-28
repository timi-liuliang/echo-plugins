/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_RadialScriptItem_h__
#define __HOM_RadialScriptItem_h__

#include "HOM_RadialItem.h"

SWIGOUT(%rename(RadialScriptItem) HOM_RadialScriptItem;)

class HOM_API HOM_RadialScriptItem : public HOM_RadialItem
{
public:
    HOM_RadialScriptItem()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_RadialScriptItem(const HOM_RadialScriptItem &item)
    : HOM_RadialItem(item)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_RadialScriptItem()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual const char *label() = 0;

    virtual void setLabel(const char *s) = 0;

    virtual const char *icon() = 0;

    virtual void setIcon(const char *s) = 0;

    virtual const char *check() = 0;

    virtual void setCheck(const char *s) = 0;

    virtual const char *script() = 0;

    virtual void setScript(const char *s) = 0;
};

#endif
