/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_MenuParmTemplate_h__
#define __HOM_MenuParmTemplate_h__

#include "HOM_API.h"
#include "HOM_ParmTemplate.h"
#include "HOM_EnumModules.h"

SWIGOUT(%feature("notabstract") HOM_MenuParmTemplate;)
SWIGOUT(%rename(MenuParmTemplate) HOM_MenuParmTemplate;)

class HOM_API HOM_MenuParmTemplate : virtual public HOM_ParmTemplate
{
public:
#ifdef SWIG
%extend {
    %kwargs HOM_MenuParmTemplate;
    HOM_MenuParmTemplate(
	    const char *name,
	    const char *label,
	    const std::vector<std::string> &menu_items,
	    const std::vector<std::string> &menu_labels =
		std::vector<std::string>(),
	    int default_value = 0,
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
	    const std::string &default_expression = "",
	    const HOM_EnumValue &default_expression_language = 
		HOM_scriptLanguage::Hscript,
	    bool store_default_value_as_string=false,
	    bool menu_use_token = false)
    {
	return HOM().newMenuParmTemplate(
	    name, label, menu_items, menu_labels, default_value, icon_names,
            item_generator_script, item_generator_script_language,
	    menu_type, menu_use_token, disable_when, is_hidden, is_label_hidden,
	    join_with_next, help, script_callback, script_callback_language,
	    tags, conditionals, default_expression, 
	    default_expression_language, store_default_value_as_string);
    }
};
#else
    HOM_MenuParmTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_MenuParmTemplate(const HOM_MenuParmTemplate &parm_template)
    : HOM_ParmTemplate(parm_template)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_MenuParmTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig think we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__();)

    virtual int defaultValue() = 0;
    virtual std::string defaultValueAsString() = 0;
    virtual void setDefaultValue(int default_value) = 0;
    virtual std::string defaultExpression() = 0;
    virtual void setDefaultExpression(
	const std::string &default_expression) = 0;
    virtual HOM_EnumValue &defaultExpressionLanguage() = 0;
    virtual void setDefaultExpressionLanguage(
	const HOM_EnumValue &default_expression_language) = 0;

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

    virtual bool menuUseToken() = 0;
    virtual void setMenuUseToken(bool menuusetoken) = 0;
};

#endif
