/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_PopNode_h__
#define __HOM_PopNode_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"

class HOM_Point;

SWIGOUT(%rename(PopNode) HOM_PopNode;)

class HOM_API HOM_PopNode : virtual public HOM_Node
{
public:
    HOM_PopNode()
    : HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_PopNode(const HOM_PopNode &pop_node)
    : HOM_Node(pop_node),
      HOM_NetworkMovableItem(pop_node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_PopNode()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    SWIGOUT(%newobject curPoint;)
    virtual HOM_Point *curPoint() = 0;

    virtual bool isBypassed() = 0;
    virtual void bypass(bool on) = 0;
    virtual bool isTemplateFlagSet() = 0;
    virtual void setTemplateFlag(bool on) = 0;
    virtual bool isDisplayFlagSet() = 0;
    virtual void setDisplayFlag(bool on) = 0;
    virtual bool isRenderFlagSet() = 0;
    virtual void setRenderFlag(bool on) = 0;

    SWIGOUT(%newobject displayNode;)
    virtual HOM_Node *displayNode() = 0;
    SWIGOUT(%newobject renderNode;)
    virtual HOM_Node *renderNode() = 0;
};

#endif
