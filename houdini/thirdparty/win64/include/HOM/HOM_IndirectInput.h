/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_IndirectInput_h__
#define __HOM_IndirectInput_h__

#include "HOM_API.h"
#include "HOM_NetworkMovableItem.h"
#include "HOM_PtrOrNull.h"

class HOM_NodeConnection;

SWIGOUT(%rename(IndirectInput) HOM_IndirectInput;)

class HOM_API HOM_IndirectInput : virtual public HOM_NetworkMovableItem
{
public:
    HOM_IndirectInput(HOM_EnumValue &network_item_type)
    : HOM_NetworkMovableItem(network_item_type)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_IndirectInput(const HOM_IndirectInput &input)
    : HOM_NetworkMovableItem(input)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_IndirectInput()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::vector<HOM_ElemPtr<HOM_Node> > outputs() = 0;
    virtual std::vector<HOM_ElemPtr<HOM_NodeConnection> > outputConnections() = 0;
    SWIGOUT(%newobject input;)
    virtual HOM_Node *input() = 0;
    virtual int inputOutputIndex() = 0;

    SWIGOUT(%newobject inputItem;)
    virtual HOM_NetworkMovableItem *inputItem() = 0;
    virtual int inputItemOutputIndex() = 0;

    virtual void setColorDefault() = 0;
    virtual void setUseColorFromOutput(bool use_output_color) = 0;
    virtual bool useColorFromOutput() = 0;
    virtual void setSymbolicColorName(const char *name) = 0;
    virtual std::string symbolicColorName() = 0;
};

#endif
