/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NetworkMovableItem_h__
#define __HOM_NetworkMovableItem_h__

#include "HOM_API.h"
#include "HOM_NetworkItem.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"

class HOM_Color;
class HOM_Node;
class HOM_NetworkBox;
class HOM_Vector2;

SWIGOUT(%rename(NetworkMovableItem) HOM_NetworkMovableItem;)

class HOM_API HOM_NetworkMovableItem : public HOM_NetworkItem
{
public:
    HOM_NetworkMovableItem(HOM_EnumValue &network_item_type)
	: HOM_NetworkItem(network_item_type)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkMovableItem(const HOM_NetworkMovableItem &src)
	: HOM_NetworkItem(src)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_NetworkMovableItem()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual int __hash__() = 0;

    virtual std::string name() = 0;
    SWIGOUT(%kwargs setName;)
    virtual void setName(const char* name, bool unique_name = false) = 0;
    virtual int digitsInName() = 0;

    virtual std::string path() = 0;
    virtual std::string relativePathTo(HOM_NetworkMovableItem &base_item) = 0;

    virtual bool isSelected() = 0;
    SWIGOUT(%kwargs setSelected;)
    virtual void setSelected(bool on, bool clear_all_selected = false,
	    bool show_asset_if_selected = false) = 0;

    virtual HOM_Color color() = 0;
    virtual void setColor(HOM_Color &color) = 0;

    virtual HOM_Vector2 size() = 0;
    virtual HOM_Vector2 position() = 0;
    virtual void setPosition(const std::vector<double> &position) = 0;
    virtual void move(const std::vector<double> &amount) = 0;

    SWIGOUT(%newobject parent;)
    virtual HOM_Node *parent() = 0;
    SWIGOUT(%newobject parentNetworkBox;)
    virtual HOM_NetworkBox *parentNetworkBox() = 0;

    virtual int64 sessionId() = 0;
};

#endif

