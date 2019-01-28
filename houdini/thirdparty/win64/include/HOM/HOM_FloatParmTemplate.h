/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_FloatParmTemplate_h__
#define __HOM_FloatParmTemplate_h__

#include "HOM_API.h"
#include "HOM_ParmTemplate.h"

SWIGOUT(%feature("notabstract") HOM_FloatParmTemplate;)
SWIGOUT(%rename(FloatParmTemplate) HOM_FloatParmTemplate;)

class HOM_API HOM_FloatParmTemplate : virtual public HOM_ParmTemplate
{
public:
#ifdef SWIG
%extend {
    %kwargs HOM_FloatParmTemplate;
    HOM_FloatParmTemplate(
	    const char *name,
	    const char *label,
	    int num_components,
	    const std::vector<double> &default_value = std::vector<double>(),
	    double min = 0.0, double max = 10.0,
	    bool min_is_strict = false, bool max_is_strict = false,
	    HOM_EnumValue &look = HOM_parmLook::Regular,
	    HOM_EnumValue &naming_scheme = HOM_parmNamingScheme::XYZW,
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
	return HOM().newFloatParmTemplate(
	    name, label, num_components, default_value,
	    min, max, min_is_strict, max_is_strict, look, naming_scheme,
	    disable_when, is_hidden, is_label_hidden, join_with_next, help,
	    script_callback, script_callback_language, tags, conditionals,
	    default_expression, default_expression_language);
    }
};
#else
    HOM_FloatParmTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_FloatParmTemplate(const HOM_FloatParmTemplate &parm_template)
    : HOM_ParmTemplate(parm_template)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_FloatParmTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig think we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__();)

    virtual double minValue() = 0;
    virtual void setMinValue(double min_value) = 0;
    virtual double maxValue() = 0;
    virtual void setMaxValue(double max_value) = 0;
    virtual bool minIsStrict() = 0;
    virtual void setMinIsStrict(bool on) = 0;
    virtual bool maxIsStrict() = 0;
    virtual void setMaxIsStrict(bool on) = 0;

    virtual std::vector<double> defaultValue() = 0;
    virtual void setDefaultValue(const std::vector<double> &default_value) = 0;
    virtual std::vector<std::string> defaultExpression() = 0;
    virtual void setDefaultExpression(
	const std::vector<std::string> &default_expression) = 0;
    virtual std::vector<HOM_EnumValue *> defaultExpressionLanguage() = 0;
    virtual void setDefaultExpressionLanguage(
	const std::vector<HOM_EnumValue *> &default_expression_language) = 0;

};

#endif
