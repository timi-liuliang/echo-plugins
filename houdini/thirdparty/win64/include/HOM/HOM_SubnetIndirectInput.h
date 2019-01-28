/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_SubnetIndirectInput_h__
#define __HOM_SubnetIndirectInput_h__

#include "HOM_API.h"
#include "HOM_IndirectInput.h"
#include "HOM_PtrOrNull.h"

class HOM_NodeConnection;

SWIGOUT(%rename(SubnetIndirectInput) HOM_SubnetIndirectInput;)

class HOM_API HOM_SubnetIndirectInput : virtual public HOM_IndirectInput
{
public:
    HOM_SubnetIndirectInput()
    : HOM_IndirectInput(HOM_networkItemType::SubnetIndirectInput),
      HOM_NetworkMovableItem(HOM_networkItemType::SubnetIndirectInput)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_SubnetIndirectInput(const HOM_SubnetIndirectInput &input)
    : HOM_IndirectInput(input),
      HOM_NetworkMovableItem(input)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_SubnetIndirectInput()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual bool operator==(HOM_PtrOrNull<HOM_SubnetIndirectInput> input) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_SubnetIndirectInput> input) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_NodeConnection> > inputConnections() = 0;
    virtual int number() = 0;
};

#endif
