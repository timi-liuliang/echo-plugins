/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ButtonParmTemplate_h__
#define __HOM_ButtonParmTemplate_h__

#include "HOM_API.h"
#include "HOM_ParmTemplate.h"

SWIGOUT(%feature("notabstract") HOM_ButtonParmTemplate;)
SWIGOUT(%rename(ButtonParmTemplate) HOM_ButtonParmTemplate;)

class HOM_API HOM_ButtonParmTemplate : virtual public HOM_ParmTemplate
{
public:
#ifdef SWIG
%extend {
    %kwargs HOM_ButtonParmTemplate;
    HOM_ButtonParmTemplate(
	    const char *name,
	    const char *label,
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
		HOM_StdMapEnumString() )
    {
	return HOM().newButtonParmTemplate(name, label, disable_when,
	    is_hidden, is_label_hidden, join_with_next, help, script_callback,
	    script_callback_language, tags, conditionals);
    }
};
#else
    HOM_ButtonParmTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_ButtonParmTemplate(const HOM_ButtonParmTemplate &parm_template)
    : HOM_ParmTemplate(parm_template)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_ButtonParmTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig think we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__();)
};

#endif
