/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_PopNetNode_h__
#define __HOM_PopNetNode_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"

SWIGOUT(%rename(PopNetNode) HOM_PopNetNode;)

class HOM_API HOM_PopNetNode : virtual public HOM_Node
{
public:
    HOM_PopNetNode()
    : HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_PopNetNode(const HOM_PopNetNode &popnet_node)
    : HOM_Node(popnet_node),
      HOM_NetworkMovableItem(popnet_node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_PopNetNode()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    SWIGOUT(%newobject displayNode;)
    virtual HOM_Node *displayNode() = 0;
    SWIGOUT(%newobject renderNode;)
    virtual HOM_Node *renderNode() = 0;
};

#endif
