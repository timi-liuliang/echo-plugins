/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NodeType_h__
#define __HOM_NodeType_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_EnumValue.h"
#include "HOM_Color.h"
#include "HOM_Module.h"
#include "HOM_ParmTemplate.h"
#include "HOM_PtrOrNull.h"
#include <string>

class HOM_HDADefinition;
class HOM_HDAModule;
class HOM_NodeTypeCategory;
class HOM_ParmTemplateGroup;

SWIGOUT(%rename(NodeType) HOM_NodeType;)

class HOM_API HOM_NodeType
{
public:
    HOM_NodeType()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NodeType(const HOM_NodeType &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_NodeType()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_NodeType> node_type) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_NodeType> node_type) = 0;

    virtual int __hash__() = 0;

    virtual std::string __repr__() = 0;

    virtual HOM_NodeTypeCategory &category() = 0;

    virtual std::string name() = 0;
    virtual std::vector<std::string> nameComponents() = 0;
    virtual std::string description() = 0;
    virtual std::string nameWithCategory() = 0;
    virtual std::vector<std::string> namespaceOrder() = 0;

    virtual HOM_EnumValue &source() = 0;
    virtual std::string sourcePath() = 0;

    SWIGOUT(%newobject sourceNetwork;)
    virtual HOM_Node *sourceNetwork() = 0;

    SWIGOUT(%newobject definition;)
    virtual HOM_HDADefinition *definition() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_HDADefinition> >
	allInstalledDefinitions() = 0;

    virtual std::string icon() = 0;
    virtual int minNumInputs() = 0;
    virtual int maxNumInputs() = 0;
    virtual int maxNumOutputs() = 0;

    // generatorFlag() is deprecated in favour of isGenerator().
    virtual bool isGenerator() = 0;
    bool generatorFlag()
    { return isGenerator(); }

    // managerFlag() is deprecated in favour of isManager().
    SWIGOUT(%kwargs isManager);
    virtual bool isManager(bool include_management_types=true) = 0;
    bool managerFlag()
    { return isManager(); }

    // unorderedInputsFlag() is deprecated in favour of hasUnorderedInputs().
    virtual bool hasUnorderedInputs() = 0;
    bool unorderedInputsFlag()
    { return hasUnorderedInputs(); }

    virtual bool hasEditableInputData() = 0;

    virtual std::vector<std::string> aliases() = 0;
    virtual void addAlias(const char *alias) = 0;
    virtual void removeAlias(const char *alias) = 0;

    virtual bool hidden() = 0;
    virtual void setHidden(bool hidden) = 0;

    virtual bool deprecated() = 0;
    virtual std::map<std::string, hboost::any> deprecationInfo() = 0;
    
    virtual std::vector<HOM_ElemPtr<HOM_Node> > instances() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_ParmTemplate> > parmTemplates() = 0;

    // This method was implemented for the experimental edit optype parms
    // workflow that is currently disabled.
    SWIGOUT(%ignore reloadParmTemplates;)
    virtual void reloadParmTemplates() = 0;

    SWIGOUT(%newobject parmTemplateGroup;)
    virtual HOM_ParmTemplateGroup *parmTemplateGroup() = 0;

    // This method was implemented for the experimental edit optype parms
    // workflow that is currently disabled.
    SWIGOUT(%ignore baseParmTemplateGroup;)
    SWIGOUT(%newobject baseParmTemplateGroup;)
    virtual HOM_ParmTemplateGroup *baseParmTemplateGroup() = 0;

    virtual void uninstallFromPath(const char *path) = 0;

    SWIGOUT(%newobject hdaModule;)
    virtual HOM_HDAModule *hdaModule() = 0;

    virtual bool hasPermanentUserDefaults() = 0;

    virtual bool isReadable() = 0;
    virtual bool isWritable() = 0;
    virtual bool areContentsViewable() = 0;
    
    virtual std::vector<std::string> containedNodeTypes() = 0;

    virtual HOM_NodeTypeCategory *childTypeCategory() = 0;

    virtual std::string helpUrl() = 0;
    virtual std::string defaultHelpUrl() = 0;

    virtual std::string embeddedHelp() = 0;

    virtual std::string defaultShape() = 0;
    virtual void setDefaultShape(const char *shape) = 0;
    virtual HOM_Color defaultColor() = 0;
    virtual void setDefaultColor(HOM_Color *color) = 0;

    virtual void *_asVoidPointer() = 0;

    // We need a quick way to get the OP_OpTypeId from C++ code so we know
    // which subclass of HOM_NodeType we should cast to, so that's why
    // opTypeIdAsInt() exists.
    SWIGOUT(%ignore opTypeIdAsInt;)
    virtual int opTypeIdAsInt() = 0;
};

#endif
