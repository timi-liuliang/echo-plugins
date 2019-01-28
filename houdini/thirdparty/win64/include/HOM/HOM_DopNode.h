/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_DopNode_h__
#define __HOM_DopNode_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"
#include <vector>

class HOM_DopData;
class HOM_DopNetNode;
class HOM_DopSimulation;

SWIGOUT(%rename(DopNode) HOM_DopNode;)

class HOM_API HOM_DopNode : virtual public HOM_Node
{
public:
    HOM_DopNode()
    : HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_DopNode(const HOM_DopNode &dop_node)
    : HOM_Node(dop_node),
      HOM_NetworkMovableItem(dop_node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_DopNode()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    SWIGOUT(%newobject dopNetNode;)
    virtual HOM_Node *dopNetNode() = 0;
    SWIGOUT(%newobject simulation;)
    virtual HOM_DopSimulation *simulation() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_DopData> > processedObjects() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_DopData> > createdObjects() = 0;

    virtual bool isBypassed() = 0;
    virtual void bypass(bool on) = 0;
    virtual bool isDisplayFlagSet() = 0;
    virtual void setDisplayFlag(bool on) = 0;
    virtual bool isTemplateFlagSet() = 0;
    virtual void setTemplateFlag(bool on) = 0;

    SWIGOUT(%newobject displayNode;)
    virtual HOM_Node *displayNode() = 0;
    SWIGOUT(%newobject renderNode;)
    virtual HOM_Node *renderNode() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_DopData> > objectsToProcess() = 0;

    SWIGOUT(%newobject pythonSolverData;)
    virtual HOM_DopData *pythonSolverData() = 0;
};

#endif
