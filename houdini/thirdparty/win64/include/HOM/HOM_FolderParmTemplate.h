/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_FolderParmTemplate_h__
#define __HOM_FolderParmTemplate_h__

#include "HOM_API.h"
#include "HOM_ParmTemplate.h"

SWIGOUT(%feature("notabstract") HOM_FolderParmTemplate;)
SWIGOUT(%rename(FolderParmTemplate) HOM_FolderParmTemplate;)

class HOM_API HOM_FolderParmTemplate : virtual public HOM_ParmTemplate
{
public:
#ifdef SWIG
%extend {
    %kwargs HOM_FolderParmTemplate;
    HOM_FolderParmTemplate(
	    const char *name,
	    const char *label,
            const std::vector<HOM_ParmTemplate *> &parm_templates =
                std::vector<HOM_ParmTemplate *>(),
	    HOM_EnumValue &folder_type = HOM_folderType::Tabs,
	    int default_value = 0,
	    bool is_hidden = false,
	    bool ends_tab_group = false,
	    const char *script_callback = NULL,
	    HOM_EnumValue &script_callback_language =
		HOM_scriptLanguage::Hscript,
	    const std::map<std::string, std::string> &tags =
		HOM_StdMapStringString(),
	    const std::map<HOM_EnumValue, std::string> &conditionals =
		HOM_StdMapEnumString() )
    {
	return HOM().newFolderParmTemplate(
	    name, label, parm_templates, folder_type, default_value, is_hidden,
            ends_tab_group, script_callback, script_callback_language, tags,
	    conditionals);
    }
};
#else
    HOM_FolderParmTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_FolderParmTemplate(const HOM_FolderParmTemplate &parm_template)
    : HOM_ParmTemplate(parm_template)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_FolderParmTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig think we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__();)

    // This method is deprecated in favour of folderType().
    HOM_EnumValue &folderStyle()
    { return folderType(); }

    virtual HOM_EnumValue &folderType() = 0;
    virtual void setFolderType(HOM_EnumValue &folder_type) = 0;

    virtual bool isActualFolder() = 0;

    virtual int defaultValue() = 0;
    virtual void setDefaultValue(int default_value) = 0;

    virtual bool endsTabGroup() = 0;
    virtual void setEndsTabGroup(bool on) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_ParmTemplate> > parmTemplates() = 0;
    virtual void setParmTemplates(
	    const std::vector<HOM_ParmTemplate *> &parm_templates) = 0;
    virtual void addParmTemplate(HOM_ParmTemplate &parm_template) = 0;
};

#endif
