/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ShopNode_h__
#define __HOM_ShopNode_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_Node.h"

SWIGOUT(%rename(ShopNode) HOM_ShopNode;)

class HOM_API HOM_ShopNode : virtual public HOM_Node
{
public:
    HOM_ShopNode()
    : HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_ShopNode(const HOM_ShopNode &shop_node)
    : HOM_Node(shop_node),
      HOM_NetworkMovableItem(shop_node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ShopNode()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    // Get the shader string for the render_type. Also, for the multi_type by
    // default interpret it as a surface (invalid shader type would be better).
    virtual std::string shaderString(const char *render_type = NULL,
	    HOM_EnumValue &interpret_type = HOM_shaderType::Surface) = 0;

    // Returns the code for the given shader. 
    virtual std::string shaderCode(
	    HOM_EnumValue &shader_type = HOM_shaderType::Surface)= 0;
    
    SWIGOUT(%kwargs shaderName;)
    virtual std::string shaderName(
	    bool as_otl_path=true, const char *shader_type_name=NULL) = 0;
    
    virtual std::vector<std::string> supportedRenderers() = 0;

    SWIGOUT(%newobject definingVopNetNode;)
    virtual HOM_Node *definingVopNetNode() = 0;

    virtual HOM_EnumValue& shaderType() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Node> > coshaderNodes(
	    const char *parm_name) = 0;
};

#endif
