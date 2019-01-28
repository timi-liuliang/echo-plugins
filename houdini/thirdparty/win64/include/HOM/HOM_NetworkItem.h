/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NetworkItem_h__
#define __HOM_NetworkItem_h__

#include "HOM_API.h"
#include "HOM_EnumModules.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"

SWIGOUT(%rename(NetworkItem) HOM_NetworkItem;)

class HOM_API HOM_NetworkItem
{
public:
    HOM_NetworkItem(HOM_EnumValue &network_item_type)
	: myType(network_item_type)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkItem(const HOM_NetworkItem &src)
	: myType(src.myType)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_NetworkItem()
    { HOM_DESTRUCT_OBJECT(this) }

    HOM_EnumValue &networkItemType()
    { return myType; }

private:
    HOM_EnumValue	&myType;
};

#endif
