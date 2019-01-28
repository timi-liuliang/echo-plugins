/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_RampParmTemplate_h__
#define __HOM_RampParmTemplate_h__

#include "HOM_API.h"
#include "HOM_ParmTemplate.h"

SWIGOUT(%feature("notabstract") HOM_RampParmTemplate;)
SWIGOUT(%rename(RampParmTemplate) HOM_RampParmTemplate;)

class HOM_API HOM_RampParmTemplate : virtual public HOM_ParmTemplate
{
public:
#ifdef SWIG
%extend {
    %kwargs HOM_RampParmTemplate;
    HOM_RampParmTemplate(
	    const char *name,
	    const char *label,
	    HOM_EnumValue &ramp_parm_type,
	    int default_value = 2,
	    HOM_EnumValue *default_basis = NULL,
	    bool show_controls = true,
	    HOM_EnumValue *color_type = NULL,
	    const char *disable_when = NULL,
	    bool is_hidden = false,
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
	return HOM().newRampParmTemplate(
	    name, label, ramp_parm_type, default_value, default_basis,
	    show_controls, color_type, disable_when, is_hidden, help,
	    script_callback, script_callback_language, tags, conditionals,
	    default_expression, default_expression_language);
    }
};
#else
    HOM_RampParmTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_RampParmTemplate(const HOM_RampParmTemplate &parm_template)
    : HOM_ParmTemplate(parm_template)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_RampParmTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig think we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__();)

    virtual int defaultValue() = 0;
    virtual void setDefaultValue(int default_value) = 0;
    virtual std::string defaultExpression() = 0;
    virtual void setDefaultExpression(
	const std::string &default_expression) = 0;
    virtual HOM_EnumValue &defaultExpressionLanguage() = 0;
    virtual void setDefaultExpressionLanguage(
	const HOM_EnumValue &default_expression_language) = 0;

    virtual HOM_EnumValue &parmType() = 0;
    virtual void setParmType(HOM_EnumValue &ramp_parm_type) = 0;

    virtual HOM_EnumValue &defaultBasis() = 0;
    virtual void setDefaultBasis(HOM_EnumValue &default_basis) = 0;

    virtual HOM_EnumValue &colorType() = 0;
    virtual void setColorType(HOM_EnumValue &color_type) = 0;

    virtual bool showsControls() = 0;
    virtual void setShowsControls(bool on) = 0;
};

#endif
