/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_LopNode_h__
#define __HOM_LopNode_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"

class HOM_LopSelectionRule;
class HOM_LopSessionEditor;

SWIGOUT(%rename(LopNode) HOM_LopNode;)

class HOM_API HOM_LopNode : virtual public HOM_Node
{
public:
    HOM_LopNode()
    : HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_LopNode(const HOM_LopNode &pop_node)
    : HOM_Node(pop_node),
      HOM_NetworkMovableItem(pop_node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_LopNode()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual bool isBypassed() = 0;
    virtual void bypass(bool on) = 0;
    virtual bool isDisplayFlagSet() = 0;
    virtual void setDisplayFlag(bool on) = 0;
    virtual bool isDebugFlagSet() = 0;
    virtual void setDebugFlag(bool on) = 0;

    SWIGOUT(%newobject displayNode;)
    virtual HOM_Node *displayNode() = 0;

#ifdef SWIG
%extend
{
    InterpreterObject editableLayer()
    { return HOMincRef((InterpreterObject)self->opaqueEditableLayer()); }
    SWIGOUT(%kwargs activeLayer;)
    InterpreterObject activeLayer(int output_index = 0)
    { return HOMincRef((InterpreterObject)self->
	     opaqueActiveLayer(output_index)); }
    InterpreterObject editableStage()
    { return HOMincRef((InterpreterObject)self->
	     opaqueEditableStage()); }
    SWIGOUT(%kwargs stage;)
    InterpreterObject stage(int output_index = 0,
	    bool apply_network_overrides = true)
    { return HOMincRef((InterpreterObject)self->
	     opaqueStage(output_index, apply_network_overrides)); }
    SWIGOUT(%kwargs sourceLayer;)
    InterpreterObject sourceLayer(int output_index = 0, int layer_index = 0)
    { return HOMincRef((InterpreterObject)self->
	     opaqueSourceLayer(output_index, layer_index));}
}
#endif

    SWIGOUT(%ignore opaqueEditableLayer;)
    virtual void *opaqueEditableLayer() = 0;
    SWIGOUT(%ignore opaqueActiveLayer;)
    virtual void *opaqueActiveLayer(int output_index) = 0;
    SWIGOUT(%ignore opaqueEditableStage;)
    virtual void *opaqueEditableStage() = 0;
    SWIGOUT(%ignore opaqueStage;)
    virtual void *opaqueStage(int output_index,
	    bool apply_network_overrides) = 0;
    SWIGOUT(%ignore opaqueSourceLayer;)
    virtual void *opaqueSourceLayer(int output_index, int layerindex) = 0;

    SWIGOUT(%kwargs sourceLayerCount;)
    virtual int sourceLayerCount(int output_index = 0) = 0;
    virtual std::vector<std::string> lastModifiedPrims() = 0;
    virtual void setLastModifiedPrims(
	    const std::vector<std::string> &primpaths) = 0;
    virtual std::vector<std::string> inputPrims(int inputidx) = 0;

    SWIGOUT(%newobject network;)
    virtual HOM_Node *network() = 0;

    SWIGOUT(%newobject networkSessionEditor;)
    virtual HOM_LopSessionEditor *networkSessionEditor(
	    int output_index = 0, int layer_index = 0) = 0;
    virtual std::vector<std::string> networkSelection() = 0;
    virtual std::string networkSelectionLayerId() = 0;
    virtual std::string networkSelectionCurrentPrim() = 0;
    virtual void setNetworkSelection(
	    const std::vector<std::string> &selection,
	    const char *layerid = nullptr,
	    const char *currentprim = nullptr) = 0;

    virtual std::map<std::string, HOM_ElemPtr<HOM_LopSelectionRule> >
	    networkSelectionRules() = 0;
    virtual void networkClearSelectionRules() = 0;
    virtual void networkSetSelectionRule(const char *name,
	    HOM_LopSelectionRule *rule) = 0;
};

#endif
