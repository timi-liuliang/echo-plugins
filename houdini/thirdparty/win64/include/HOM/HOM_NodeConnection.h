/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NodeConnection_h__
#define __HOM_NodeConnection_h__

#include "HOM_API.h"
#include "HOM_NetworkItem.h"
#include "HOM_Module.h"
#include "HOM_EnumModules.h"
#include "HOM_Errors.h"
#include <string>

class HOM_Node;
class HOM_NetworkMovableItem;
class HOM_SubnetIndirectInput;

SWIGOUT(%rename(NodeConnection) HOM_NodeConnection;)

class HOM_API HOM_NodeConnection : public HOM_NetworkItem
{
public:
    HOM_NodeConnection()
	: HOM_NetworkItem(HOM_networkItemType::Connection)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NodeConnection(const HOM_NodeConnection &src)
	: HOM_NetworkItem(src)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_NodeConnection()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    virtual bool operator==(HOM_PtrOrNull<HOM_NodeConnection> connection) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_NodeConnection> connection) = 0;

    SWIGOUT(%newobject outputNode;)
    virtual HOM_Node *outputNode() = 0;
    virtual int inputIndex() = 0;
    SWIGOUT(%newobject outputItem;)
    virtual HOM_NetworkMovableItem *outputItem() = 0;

    SWIGOUT(%newobject inputNode;)
    virtual HOM_Node *inputNode() = 0;
    virtual int outputIndex() = 0;
    SWIGOUT(%newobject subnetIndirectInput;)
    virtual HOM_SubnetIndirectInput *subnetIndirectInput() = 0;
    SWIGOUT(%newobject inputItem;)
    virtual HOM_NetworkMovableItem *inputItem() = 0;
    virtual int inputItemOutputIndex() = 0;

    virtual bool isSelected() = 0;
    SWIGOUT(%kwargs setSelected;)
    virtual void setSelected(bool selected, bool clear_all_selected=false) = 0;

    virtual std::string inputName() = 0;
    virtual std::string inputLabel() = 0;
    virtual std::string inputDataType() = 0;
    virtual std::string outputName() = 0;
    virtual std::string outputLabel() =0;
    virtual std::string outputDataType() = 0;
};

#endif

