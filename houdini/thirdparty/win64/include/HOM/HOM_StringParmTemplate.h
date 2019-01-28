/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_StringParmTemplate_h__
#define __HOM_StringParmTemplate_h__

#include "HOM_API.h"
#include "HOM_ParmTemplate.h"

SWIGOUT(%feature("notabstract") HOM_StringParmTemplate;)
SWIGOUT(%rename(StringParmTemplate) HOM_StringParmTemplate;)

class HOM_API HOM_StringParmTemplate : virtual public HOM_ParmTemplate
{
public:
#ifdef SWIG
%extend {
    %kwargs HOM_StringParmTemplate;
    HOM_StringParmTemplate(
	    const char *name,
	    const char *label,
	    int num_components,
	    const std::vector<std::string> &default_value =
		std::vector<std::string>(),
	    HOM_EnumValue &naming_scheme = HOM_parmNamingScheme::Base1,
	    HOM_EnumValue &string_type = HOM_stringParmType::Regular,
	    HOM_EnumValue &file_type = HOM_fileType::Any,
	    const std::vector<std::string> &menu_items =
		std::vector<std::string>(),
	    const std::vector<std::string> &menu_labels =
		std::vector<std::string>(),
	    const std::vector<std::string> &icon_names =
		std::vector<std::string>(),
	    const std::string &item_generator_script = "",
	    HOM_EnumValue *item_generator_script_language = NULL,
	    HOM_EnumValue &menu_type = HOM_menuType::Normal,
	    const char *disable_when = NULL,
	    bool is_hidden = false,
	    bool is_label_hidden = false,
	    bool join_with_next = false,
	    const char *help = NULL,
	    const char *script_callback = NULL,
	    HOM_EnumValue &script_callback_language =
		HOM_scriptLanguage::Hscript,
	    const std::map<std::string, std::string> &tags =
		HOM_StdMapStringString(),
	    const std::map<HOM_EnumValue, std::string> &conditionals =
		HOM_StdMapEnumString(),
	    const std::vector<std::string> &default_expression = 
		std::vector<std::string>(),
	    const std::vector<HOM_EnumValue *> &default_expression_language = 
		std::vector<HOM_EnumValue *>())
    {
	return HOM().newStringParmTemplate(
	    name, label, num_components, default_value,
	    naming_scheme, string_type, file_type, menu_items, menu_labels,
	    icon_names, item_generator_script, item_generator_script_language,
	    menu_type, disable_when, is_hidden, is_label_hidden, join_with_next,
	    help, script_callback, script_callback_language, tags, conditionals,
	    default_expression, default_expression_language);
    }
};
#else
    HOM_StringParmTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_StringParmTemplate(const HOM_StringParmTemplate &parm_template)
    : HOM_ParmTemplate(parm_template)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_StringParmTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig think we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__();)

    virtual HOM_EnumValue &stringType() = 0;
    virtual void setStringType(HOM_EnumValue &string_type) = 0;

    virtual HOM_EnumValue &fileType() = 0;
    virtual void setFileType(HOM_EnumValue &file_type) = 0;

    virtual std::vector<std::string> defaultValue() = 0;
    virtual void setDefaultValue(
	    const std::vector<std::string> &default_value) = 0;
    virtual std::vector<std::string> defaultExpression() = 0;
    virtual void setDefaultExpression(
	const std::vector<std::string> &default_expression) = 0;
    virtual std::vector<HOM_EnumValue *> defaultExpressionLanguage() = 0;
    virtual void setDefaultExpressionLanguage(
	const std::vector<HOM_EnumValue *> &default_expression_language) = 0;

    virtual std::vector<std::string> menuItems() = 0;
    virtual void setMenuItems(const std::vector<std::string> &menu_items) = 0;

    virtual std::vector<std::string> menuLabels() = 0;
    virtual void setMenuLabels(const std::vector<std::string> &menu_labels) = 0;

    virtual std::vector<std::string> iconNames() = 0;
    virtual void setIconNames(const std::vector<std::string> &icon_names) = 0;

    virtual std::string itemGeneratorScript() = 0;
    virtual void setItemGeneratorScript(const char *item_generator_script) = 0;

    virtual HOM_EnumValue &itemGeneratorScriptLanguage() = 0;
    virtual void setItemGeneratorScriptLanguage(HOM_EnumValue &language) = 0;

    virtual HOM_EnumValue &menuType() = 0;
    virtual void setMenuType(HOM_EnumValue &menu_type) = 0;
};

#endif
