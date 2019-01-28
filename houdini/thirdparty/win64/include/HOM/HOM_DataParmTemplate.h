/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_DataParmTemplate_h__
#define __HOM_DataParmTemplate_h__

#include "HOM_API.h"
#include "HOM_ParmTemplate.h"

SWIGOUT(%feature("notabstract") HOM_DataParmTemplate;)
SWIGOUT(%rename(DataParmTemplate) HOM_DataParmTemplate;)

class HOM_API HOM_DataParmTemplate : virtual public HOM_ParmTemplate
{
public:
#ifdef SWIG
%extend {
    %kwargs HOM_DataParmTemplate;
    HOM_DataParmTemplate(
	    const char *name,
	    const char *label,
	    int num_components,
	    HOM_EnumValue &look = HOM_parmLook::Regular,
	    HOM_EnumValue &naming_scheme = HOM_parmNamingScheme::XYZW,
	    HOM_EnumValue &data_parm_type = HOM_dataParmType::Geometry,
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
	return HOM().newDataParmTemplate(
	    name, label, num_components, look, naming_scheme, data_parm_type,
	    disable_when, is_hidden, is_label_hidden, join_with_next, help,
	    script_callback, script_callback_language, tags, conditionals,
	    default_expression, default_expression_language);
    }
};
#else
    HOM_DataParmTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_DataParmTemplate(const HOM_DataParmTemplate &parm_template)
    : HOM_ParmTemplate(parm_template)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_DataParmTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig think we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__();)

    virtual HOM_EnumValue &dataParmType() = 0;
    virtual void setDataParmType(HOM_EnumValue &data_type) = 0;

    virtual std::vector<std::string> defaultExpression() = 0;
    virtual void setDefaultExpression(
	const std::vector<std::string> &default_expression) = 0;
    virtual std::vector<HOM_EnumValue *> defaultExpressionLanguage() = 0;
    virtual void setDefaultExpressionLanguage(
	const std::vector<HOM_EnumValue *> &default_expression_language) = 0;
};

#endif
