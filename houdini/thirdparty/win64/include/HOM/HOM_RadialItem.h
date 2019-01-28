/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_RadialItem_h__
#define __HOM_RadialItem_h__

#include "HOM_API.h"
#include "HOM_Errors.h"
#include "HOM_EnumValue.h"
#include "HOM_Module.h"
#include <string>

SWIGOUT(%rename(RadialItem) HOM_RadialItem;)

class HOM_API HOM_RadialItem
{
public:
    HOM_RadialItem()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_RadialItem(const HOM_RadialItem &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_RadialItem()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual void destroy() = 0;

    virtual HOM_EnumValue type() = 0;
};

#endif
