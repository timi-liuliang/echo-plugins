/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_IntParmTemplate_h__
#define __HOM_IntParmTemplate_h__

#include "HOM_API.h"
#include "HOM_ParmTemplate.h"

SWIGOUT(%feature("notabstract") HOM_IntParmTemplate;)
SWIGOUT(%rename(IntParmTemplate) HOM_IntParmTemplate;)

class HOM_API HOM_IntParmTemplate : virtual public HOM_ParmTemplate
{
public:
#ifdef SWIG
%extend {
    %kwargs HOM_IntParmTemplate;
    HOM_IntParmTemplate(
	    const char *name,
	    const char *label,
	    int num_components,
	    const std::vector<int> &default_value = std::vector<int>(),
	    int min = 0, int max = 10,
	    bool min_is_strict = false, bool max_is_strict = false,
	    HOM_EnumValue &naming_scheme = HOM_parmNamingScheme::XYZW,
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
		std::vector<HOM_EnumValue *>(),
	    bool menu_use_token = false)
    {
	return HOM().newIntParmTemplate(
	    name, label, num_components, default_value,
	    min, max, min_is_strict, max_is_strict, naming_scheme,
	    menu_items, menu_labels, icon_names, item_generator_script,
	    item_generator_script_language, menu_type, menu_use_token, disable_when, is_hidden,
	    is_label_hidden, join_with_next, help, script_callback,
	    script_callback_language, tags, conditionals, default_expression,
	    default_expression_language);
    }
};
#else
    HOM_IntParmTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_IntParmTemplate(const HOM_IntParmTemplate &parm_template)
    : HOM_ParmTemplate(parm_template)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_IntParmTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig think we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__();)

    virtual int minValue() = 0;
    virtual void setMinValue(int min_value) = 0;
    virtual int maxValue() = 0;
    virtual void setMaxValue(int max_value) = 0;
    virtual bool minIsStrict() = 0;
    virtual void setMinIsStrict(bool on) = 0;
    virtual bool maxIsStrict() = 0;
    virtual void setMaxIsStrict(bool on) = 0;

    virtual std::vector<int> defaultValue() = 0;
    virtual void setDefaultValue(const std::vector<int> &default_value) = 0;
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

    virtual bool menuUseToken() = 0;
    virtual void setMenuUseToken(bool menuusetoken) = 0;
};

#endif
