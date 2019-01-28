/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ToggleParmTemplate_h__
#define __HOM_ToggleParmTemplate_h__

#include "HOM_API.h"
#include "HOM_ParmTemplate.h"

SWIGOUT(%feature("notabstract") HOM_ToggleParmTemplate;)
SWIGOUT(%rename(ToggleParmTemplate) HOM_ToggleParmTemplate;)

class HOM_API HOM_ToggleParmTemplate : virtual public HOM_ParmTemplate
{
public:
#ifdef SWIG
%extend {
    %kwargs HOM_ToggleParmTemplate;
    HOM_ToggleParmTemplate(
	    const char *name,
	    const char *label,
	    bool default_value = false,
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
		HOM_scriptLanguage::Hscript)
    {
	return HOM().newToggleParmTemplate(
	    name, label, default_value, disable_when, is_hidden,
	    is_label_hidden, join_with_next, help, script_callback,
	    script_callback_language, tags, conditionals,
	    default_expression, default_expression_language);
    }
};
#else
    HOM_ToggleParmTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_ToggleParmTemplate(const HOM_ToggleParmTemplate &parm_template)
    : HOM_ParmTemplate(parm_template)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_ToggleParmTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig think we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__();)

    virtual bool defaultValue() = 0;
    virtual void setDefaultValue(bool default_value) = 0;
    virtual std::string defaultExpression() = 0;
    virtual void setDefaultExpression(
	const std::string &default_expression) = 0;
    virtual HOM_EnumValue &defaultExpressionLanguage() = 0;
    virtual void setDefaultExpressionLanguage(
	const HOM_EnumValue &default_expression_language) = 0;

};

#endif
