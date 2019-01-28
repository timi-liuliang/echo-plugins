/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_HDADefinition_h__
#define __HOM_HDADefinition_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"
#include "HOM_EnumModules.h"
#include <UT/UT_Options.h>
#include <string>
#include <vector>
class HOM_NodeType;
class HOM_HDASection;
class HOM_NodeTypeCategory;
class HOM_HDAOptions;
class HOM_ParmTemplate;

SWIGOUT(%rename(HDADefinition) HOM_HDADefinition;)

class HOM_API HOM_HDADefinition
{
public:
    HOM_HDADefinition()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_HDADefinition(const HOM_HDADefinition &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_HDADefinition()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_HDADefinition> hda_definition) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_HDADefinition> hda_definition) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject nodeType;)
    virtual HOM_NodeType *nodeType() = 0;

    virtual HOM_NodeTypeCategory &nodeTypeCategory() = 0;
    virtual std::string nodeTypeName() = 0;

    virtual std::string libraryFilePath() = 0;

    virtual std::string embeddedHelp() = 0;

    // installed() is deprecated in favour of isInstalled().
    virtual bool isInstalled() = 0;
    bool installed()
    { return isInstalled(); }

    // current() is deprecated in favour of isCurrent().
    virtual bool isCurrent() = 0;
    bool current()
    { return isCurrent(); }

    // preferred() is deprecated in favour of isPreferred().
    virtual bool isPreferred() = 0;
    bool preferred()
    { return isPreferred(); }

    // setPreferred() is deprecated in favour of setIsPreferred().
    virtual void setIsPreferred(bool preferred) = 0;
    void setPreferred(bool preferred)
    { setIsPreferred(preferred); }

    virtual std::map<std::string, HOM_ElemPtr<HOM_HDASection> > sections() = 0;

    virtual bool hasSection(const char *name) = 0;
    SWIGOUT(%newobject addSection;)
    SWIGOUT(%kwargs addSection;)
    virtual HOM_HDASection *addSection(
	    const char *name, const std::string &contents = "",
	    HOM_EnumValue &compression_type=HOM_compressionType::NoCompression) = 0;
    virtual void removeSection(const char *name) = 0;

    virtual std::string description() = 0;
    virtual void setDescription(const char *label) = 0;
    virtual std::string icon() = 0;
    virtual void setIcon(const char *icon) = 0;
    virtual int minNumInputs() = 0;
    virtual void setMinNumInputs(int min_num_inputs) = 0;
    virtual int maxNumInputs() = 0;
    virtual void setMaxNumInputs(int max_num_inputs) = 0;

    virtual std::string extraInfo() = 0;
    virtual void setExtraInfo(const char *extra_info) = 0;

    virtual std::string userInfo() = 0;
    virtual void setUserInfo(const char *extra_info) = 0;

    virtual bool hideDefaultParameters() = 0;
    virtual std::string representativeNodePath() = 0;

    virtual std::string comment() = 0;
    virtual void setComment(const char *comment) = 0;

    virtual std::string version() = 0;
    virtual void setVersion(const char *version) = 0;

    virtual int modificationTime() = 0;
    virtual void setModificationTime(int time=-1) = 0;

    SWIGOUT(%newobject options;)
    virtual HOM_HDAOptions *options() = 0;
    virtual void setOptions(HOM_HDAOptions &options) = 0;

    virtual void updateFromNode(HOM_Node &node) = 0;

    SWIGOUT(%kwargs save;)
    virtual void save(
	    const std::string &file_name,
	    HOM_Node *template_node = NULL,
	    HOM_HDAOptions *options = NULL,
	    bool compile_contents = false,
	    bool black_box = false,
	    bool create_backup = true) = 0;

    SWIGOUT(%kwargs copyToHDAFile;)
    virtual void copyToHDAFile(
	    const std::string &file_name,
	    const char *new_name = NULL,
	    const char *new_menu_name = NULL) = 0;

    virtual void destroy() = 0;

    SWIGOUT(%newobject parmTemplateGroup;)
    virtual HOM_ParmTemplateGroup *parmTemplateGroup() = 0;

    SWIGOUT(%kwargs setParmTemplateGroup;)
    virtual void setParmTemplateGroup(
            HOM_ParmTemplateGroup &parm_template_group,
	    bool rename_conflicting_parms=false,
	    bool create_backup=true) = 0;

    virtual void setExtraFileOption(const char *name, HOM_UTOptionAny value,
				    HOM_EnumValue &type_hint 
					= HOM_fieldType::NoSuchField) = 0;
    virtual void removeExtraFileOption(const char *name) = 0;

    // compileCodeSection() deprecated. Compiled VEX assembly is no longer
    // stored in HDAs or OTLs. This function does nothing.
    virtual void compileCodeSection(const char *source_section,
				    const char *destination_section) = 0;

    // This is the function that does the real work of fetching the contents of
    // the ExtraFileOptions section as a UT_Options object.  This function is
    // never called directly from Python, and is instead invoked using from
    // extraFileOptions().
    SWIGOUT(%ignore getRawExtraFileOptions;)
    virtual void getRawExtraFileOptions(UT_Options &options) = 0;

#ifdef SWIG
#ifdef SWIGPYTHON
%extend
{
    InterpreterObject extraFileOptions()
    {
        UT_Options options;
        self->getRawExtraFileOptions(options);
        return HOMoptionsToInterpreterObject(options);
    }
}
#endif
#endif
};

#endif
