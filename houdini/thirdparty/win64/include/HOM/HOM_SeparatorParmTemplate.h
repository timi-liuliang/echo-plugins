/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_SeparatorParmTemplate_h__
#define __HOM_SeparatorParmTemplate_h__

#include "HOM_API.h"
#include "HOM_ParmTemplate.h"

SWIGOUT(%feature("notabstract") HOM_SeparatorParmTemplate;)
SWIGOUT(%rename(SeparatorParmTemplate) HOM_SeparatorParmTemplate;)

class HOM_API HOM_SeparatorParmTemplate : virtual public HOM_ParmTemplate
{
public:
#ifdef SWIG
%extend {
    %kwargs HOM_SeparatorParmTemplate;
    HOM_SeparatorParmTemplate(
	    const char *name,
	    bool is_hidden = false,
	    const std::map<std::string, std::string> &tags =
		HOM_StdMapStringString(),
	    const std::map<HOM_EnumValue, std::string> &conditionals =
		HOM_StdMapEnumString()
 			    )
    {
	return HOM().newSeparatorParmTemplate(name, is_hidden, tags,
					      conditionals);
    }
};
#else
    HOM_SeparatorParmTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_SeparatorParmTemplate(const HOM_SeparatorParmTemplate &parm_template)
    : HOM_ParmTemplate(parm_template)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_SeparatorParmTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig think we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__();)
};

#endif
