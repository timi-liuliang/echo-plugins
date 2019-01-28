/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ViewerState_h__
#define __HOM_ViewerState_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Module.h"
#include "HOM_Errors.h"
#include <vector>
#include <string>

class HOM_NodeType;
class HOM_NodeTypeCategory;
class HOM_ParmTemplateGroup;

SWIGOUT(%rename(ViewerState) HOM_ViewerState;)

class HOM_API HOM_ViewerState
{
public:
    HOM_ViewerState()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_ViewerState(const HOM_ViewerState &element)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ViewerState()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    // These functions identify the state, and the node type and node
    // type category it is associated with.
    virtual std::string name() = 0;
    virtual std::string description() = 0;
    virtual std::string icon() = 0;
    virtual std::vector<HOM_NodeTypeCategory *> categories() = 0;

    SWIGOUT(%newobject nodeType;)
    virtual HOM_NodeType *nodeType() = 0;

    virtual bool isHidden() = 0;

    virtual std::string hotkey() = 0;
    virtual HOM_ParmTemplateGroup * parmTemplates() = 0;
};

#endif

