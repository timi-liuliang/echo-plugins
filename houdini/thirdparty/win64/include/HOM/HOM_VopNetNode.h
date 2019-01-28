/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_VopNetNode_h__
#define __HOM_VopNetNode_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"
class HOM_VexContext;

SWIGOUT(%rename(VopNetNode) HOM_VopNetNode;)

class HOM_API HOM_VopNetNode : virtual public HOM_Node
{
public:
    HOM_VopNetNode()
    : HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_VopNetNode(const HOM_VopNetNode &vopnet_node)
    : HOM_Node(vopnet_node),
      HOM_NetworkMovableItem(vopnet_node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_VopNetNode()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    SWIGOUT(%newobject definedType;)
    virtual HOM_NodeType *definedType() = 0;

    SWIGOUT(%newobject vexContext;)
    virtual HOM_VexContext *vexContext() = 0;

    virtual HOM_EnumValue *shaderType() = 0;
};

#endif
