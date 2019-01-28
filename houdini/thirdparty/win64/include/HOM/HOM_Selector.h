/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Selector_h__
#define __HOM_Selector_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Module.h"
#include "HOM_Errors.h"
#include <vector>
#include <string>

class HOM_EnumValue;
class HOM_NodeType;

SWIGOUT(%rename(Selector) HOM_Selector;)

class HOM_API HOM_Selector
{
public:
    HOM_Selector()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Selector(const HOM_Selector &element)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Selector()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    // These functions identify the selector, and the node type it is
    // bound to.
    virtual std::string name() = 0;
    SWIGOUT(%newobject nodeType;)
    virtual HOM_NodeType *nodeType() = 0;

    // These functions relate to the selector type. The geometry types
    // that can be selected, and the group type parm values that correspond
    // to those geometry types are both defined by the selector type.
    virtual std::string selectorType() = 0;

    virtual std::vector<HOM_EnumValue *> geometryTypes() = 0;

    virtual std::vector<int> groupTypeParmValues() = 0;
    virtual bool ordered() = 0;

    // These functions access information about the selector configuration.
    // These are all options that can be controlled when creating the
    // selector.
    virtual std::string prompt() = 0;
    virtual std::vector<HOM_EnumValue *> primitiveTypes() = 0;
    virtual std::string groupParmName() = 0;
    virtual std::string groupTypeParmName() = 0;
    virtual int inputIndex() = 0;
    virtual bool inputRequired() = 0;
    virtual bool allowDragging() = 0;
    virtual bool emptyStringSelectsAll() = 0;

    virtual std::string extraInfo() = 0;

    // Unbinds the selector. Use NodeType::addSelector to make a new one.
    virtual void destroy() = 0;
};

#endif
