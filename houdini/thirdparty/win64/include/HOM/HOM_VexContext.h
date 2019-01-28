/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_VexContext_h__
#define __HOM_VexContext_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include <string>
#include <map>

SWIGOUT(%rename(VexContext) HOM_VexContext;)

class HOM_API HOM_VexContext
{
public:
    HOM_VexContext()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_VexContext(const HOM_VexContext &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_VexContext()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual std::string name() = 0;

    virtual HOM_NodeTypeCategory *nodeTypeCategory() = 0;

    virtual HOM_EnumValue *shaderType() = 0;

    virtual std::map<std::string, std::string> pathsToLoadedVexFunctions() = 0;
};

#endif
