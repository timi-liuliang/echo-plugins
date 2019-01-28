/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_VopNode_h__
#define __HOM_VopNode_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"

// Swig ignores #include by default so we use %include instead.
#ifdef SWIG
%include <UT/UT_Experimental.h>
#else
#include <UT/UT_Experimental.h>
#endif

SWIGOUT(%rename(VopNode) HOM_VopNode;)

class HOM_API HOM_VopNode : virtual public HOM_Node
{
public:
    HOM_VopNode()
    : HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_VopNode(const HOM_VopNode &vop_node)
    : HOM_Node(vop_node),
      HOM_NetworkMovableItem(vop_node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_VopNode()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual HOM_VopNode* insertParmGenerator(
	    const char* input_name, HOM_EnumValue &parm_gen_type, 
	    bool reference_input_defaults) = 0;
    virtual std::map<std::string, HOM_ElemPtr<HOM_Node> > 
	insertParmGeneratorsForAllInputs(
	    HOM_EnumValue &parm_gen_type,
	    bool reference_input_defaults) = 0;

    virtual HOM_VopNode* parmDefinerNode() = 0;

    // Deletes a chain of nodes wired into a particular input if and only if 
    // they are wired only into the specified input on this node. Returns true
    // if nodes were deleted, false otherwise.
    virtual bool deleteIndependentInputNodes(int input_index) = 0;

    virtual bool dependsOnlyOn(HOM_VopNode * other_node, int max_connections,
	    bool recurse) = 0;

    virtual void setIsInputVisible(const char* input_name, bool is_visible) = 0;
    virtual bool isInputVisible(const char* input_name) = 0;

    virtual std::vector<std::string> inputDataTypes() = 0;
    virtual std::vector<std::string> outputDataTypes() = 0;
    virtual std::vector<std::string> inputParmTypes() = 0;
    virtual bool isInputCompatible(int idx, HOM_VopNode &other, int other_idx,
	    bool allow_conversions = false) = 0;

    virtual std::string code() = 0;

    virtual std::string outerCode() = 0;

    // Flags.
    virtual bool isBypassed() = 0;
    virtual void bypass(bool on) = 0;
    virtual bool isDebugFlagSet() = 0;
    virtual void setDebugFlag(bool on) = 0;
    virtual bool isDetailLowFlagSet() = 0;
    virtual void setDetailLowFlag(bool on) = 0;
    virtual bool isDetailMediumFlagSet() = 0;
    virtual void setDetailMediumFlag(bool on) = 0;
    virtual bool isDetailHighFlagSet() = 0;
    virtual void setDetailHighFlag(bool on) = 0;
    virtual bool isCompressFlagSet() = 0;
    virtual void setCompressFlag(bool on) = 0;
    virtual bool isMaterialFlagSet() = 0;
    virtual void setMaterialFlag(bool on) = 0;

    // Parameter groups for the network editor UI.
    virtual void setInputGroupExpanded(const char *group, bool expanded) = 0;
    virtual bool isInputGroupExpanded(const char *group) = 0;
    virtual std::vector<int> inputsInGroup(const char *group) = 0;
    virtual std::vector<std::string> inputGroupNames() = 0;
    virtual std::vector<int> inputIndexOrder() = 0;

    // Get the shader string for the render_type. Also, for the multi_type by
    // default interpret it as a surface (invalid shader type would be better).
    virtual std::string shaderString(const char *render_type=NULL,
	    HOM_EnumValue &interpret_type = HOM_shaderType::Surface,
	    bool as_encapsulated = false) = 0;

    // Returns the code for the given shader. If the VOP node is a subnet
    // returns the encapsulated shader code, otherwise returns the shader code
    // from the VOP network, or the SHOP.
    virtual std::string shaderCode(
	    HOM_EnumValue &shader_type = HOM_shaderType::Surface)= 0;
    
    SWIGOUT(%kwargs shaderName;)
    virtual std::string shaderName(
	    bool as_otl_path=true, const char *shader_type_name=NULL)= 0;

    virtual HOM_EnumValue& shaderType() = 0;

    virtual std::string shaderLanguageName() = 0;

    virtual std::string currentSignatureName() = 0;
    
    // When a vop is used as a shader node, some parameters may
    // be overriden by a layered shader (coshader), or a parameter may
    // specify an oppath for an explicit coshader.
    // These methods return info about such coshaders.
    virtual std::vector<HOM_ElemPtr<HOM_Node> > coshaderNodes(
	    const char *parm_name) = 0;
    virtual std::vector<std::string> coshaderNodesOutputNames(
	    const char *parm_name) = 0;

    // Return a list of extra bindings from this specific node, in
    // particular Snippet VOPs triggering bindings via @ references.
    virtual std::vector<std::string> extraBindings() = 0;
};

#endif

