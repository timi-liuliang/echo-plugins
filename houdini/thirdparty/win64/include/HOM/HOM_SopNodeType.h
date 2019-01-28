/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_SopNodeType_h__
#define __HOM_SopNodeType_h__

#include "HOM_NodeType.h"

class HOM_Selector;

SWIGOUT(%rename(SopNodeType) HOM_SopNodeType;)

class HOM_API HOM_SopNodeType : virtual public HOM_NodeType
{
public:
    HOM_SopNodeType()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_SopNodeType(const HOM_SopNodeType &sop_nodetype)
    : HOM_NodeType(sop_nodetype)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_SopNodeType()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual std::vector<HOM_ElemPtr<HOM_Selector> > selectors(
	    std::vector<int> selector_indices = std::vector<int>()) = 0;

    SWIGOUT(%newobject addSelector;)
    SWIGOUT(%kwargs addSelector;)
    virtual HOM_Selector *addSelector(const char *name,
	    const char *selector_type,
	    const char *prompt = "Select components",
	    std::vector<HOM_EnumValue *> primitive_types =
		std::vector<HOM_EnumValue *>(),
	    const char *group_parm_name = NULL,
	    const char *group_type_parm_name = NULL,
	    int input_index = 0,
	    bool input_required = true,
	    bool allow_dragging = false,
	    bool empty_string_selects_all = true) = 0;
};

#endif
