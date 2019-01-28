/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_VopNodeType_h__
#define __HOM_VopNodeType_h__

#include "HOM_NodeType.h"

SWIGOUT(%rename(VopNodeType) HOM_VopNodeType;)

class HOM_API HOM_VopNodeType : virtual public HOM_NodeType
{
public:
    HOM_VopNodeType()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_VopNodeType(const HOM_VopNodeType &vop_nodetype)
    : HOM_NodeType(vop_nodetype)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_VopNodeType()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual HOM_EnumValue &shaderType() = 0;
    virtual std::string renderMask() = 0;

    virtual std::map<std::string, std::string> inputTags(
	    const char *input_name) = 0;
    virtual std::map<std::string, std::string> outputTags(
	    const char *output_name) = 0;
};

#endif
