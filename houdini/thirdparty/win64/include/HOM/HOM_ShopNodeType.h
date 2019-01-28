/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ShopNodeType_h__
#define __HOM_ShopNodeType_h__

#include "HOM_NodeType.h"

SWIGOUT(%rename(ShopNodeType) HOM_ShopNodeType;)

class HOM_API HOM_ShopNodeType : virtual public HOM_NodeType
{
public:
    HOM_ShopNodeType()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_ShopNodeType(const HOM_ShopNodeType &shop_nodetype)
    : HOM_NodeType(shop_nodetype)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ShopNodeType()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual HOM_EnumValue &shaderType() = 0;
    virtual std::string renderMask() = 0;
};

#endif
