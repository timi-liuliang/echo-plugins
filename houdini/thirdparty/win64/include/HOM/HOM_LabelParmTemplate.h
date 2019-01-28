/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_LabelParmTemplate_h__
#define __HOM_LabelParmTemplate_h__

#include "HOM_API.h"
#include "HOM_ParmTemplate.h"

SWIGOUT(%feature("notabstract") HOM_LabelParmTemplate;)
SWIGOUT(%rename(LabelParmTemplate) HOM_LabelParmTemplate;)

class HOM_API HOM_LabelParmTemplate : virtual public HOM_ParmTemplate
{
public:
#ifdef SWIG
%extend {
    %kwargs HOM_LabelParmTemplate;
    HOM_LabelParmTemplate(
	    const char *name,
	    const char *label,
	    const std::vector<std::string> &column_labels =
		std::vector<std::string>(),
	    bool is_hidden = false,
	    bool is_label_hidden = false,
	    bool join_with_next = false,
	    const char *help = NULL,
	    const std::map<std::string, std::string> &tags =
		HOM_StdMapStringString(),
	    const std::map<HOM_EnumValue, std::string> &conditionals =
		HOM_StdMapEnumString() )
    {
	return HOM().newLabelParmTemplate(name, label, column_labels,
	    is_hidden, is_label_hidden, join_with_next, help, tags,
	    conditionals);
    }
};
#else
    HOM_LabelParmTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_LabelParmTemplate(const HOM_LabelParmTemplate &parm_template)
    : HOM_ParmTemplate(parm_template)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_LabelParmTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig think we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__();)

    virtual std::vector<std::string> columnLabels() = 0;
    virtual void setColumnLabels(const std::vector<std::string> &column_labels) = 0;
};

#endif
