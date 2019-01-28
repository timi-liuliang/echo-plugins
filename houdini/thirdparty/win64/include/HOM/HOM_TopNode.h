/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_TopNode_h__
#define __HOM_TopNode_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"
#include "HOM_IterableList.h"
#include "HOM_PtrOrNull.h"

class HOM_Point;
class TOP_Node;

SWIGOUT(%rename(TopNode) HOM_TopNode;)

class HOM_API HOM_TopNode : virtual public HOM_Node
{
public:
    HOM_TopNode()
    : HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_TopNode(const HOM_TopNode &pop_node)
    : HOM_Node(pop_node),
      HOM_NetworkMovableItem(pop_node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_TopNode()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual bool isBypassed() = 0;
    virtual void bypass(bool on) = 0;
    virtual bool isDisplayFlagSet() = 0;
    virtual void setDisplayFlag(bool on) = 0;
    virtual bool isRenderFlagSet() = 0;
    virtual void setRenderFlag(bool on) = 0;

    SWIGOUT(%newobject displayNode;)
    virtual HOM_Node *displayNode() = 0;
    SWIGOUT(%newobject renderNode;)
    virtual HOM_Node *renderNode() = 0;

    virtual void executeGraph(bool filter_roots,
			      bool block=false,
			      bool generate_only=false) = 0;
    virtual void generateRootTasks() = 0;
    virtual void dirtyAllTasks() = 0;
    virtual void dirtyTasks() = 0;
    virtual std::string graphCommands() = 0;
    virtual std::string taskGraphCommands() = 0;

    virtual void setSelectedWorkItem(int idx) = 0;
    virtual int  getSelectedWorkItem() = 0;
    virtual std::string getWorkItemName(int idx) = 0;
    virtual std::string getPDGGraphContextName() = 0;
    virtual std::string getPDGNodeName() = 0;

    virtual void addPDGFilter(int idx) = 0;
    virtual void removePDGFilter(int idx) = 0;
    virtual bool isPDGFilter(int idx) = 0;
    virtual void enablePDGFilter(bool filter_on) = 0;

    virtual bool isProcessor() = 0;
    virtual bool isPartitioner() = 0;
    virtual bool isMapper() = 0;
    virtual bool isFilterOn() = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Node> > getFilterNodes() = 0;

    virtual std::vector<std::string> outputDataTypes() = 0;
    virtual std::vector<std::string> inputDataTypes() = 0;

};

#endif
