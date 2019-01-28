/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NodeBundle_h__
#define __HOM_NodeBundle_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_EnumModules.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"
#include <hboost/any.hpp>
#include <string>
#include <vector>

class HOM_Node;

SWIGOUT(%rename(Bundle) HOM_NodeBundle;)

class HOM_API HOM_NodeBundle
{
public:
    HOM_NodeBundle()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NodeBundle(const HOM_Node &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_NodeBundle()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_NodeBundle> bundle) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_NodeBundle> bundle) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    virtual void destroy() = 0;

    virtual std::string name() = 0;
    virtual void setName(const char* name) = 0;

    virtual HOM_EnumValue &findBestFilter() = 0;
    virtual HOM_EnumValue &filter() = 0;
    virtual void setFilter(HOM_EnumValue &filter) = 0;

    SWIGOUT(%ignore boostAnyPattern;)
    virtual hboost::any boostAnyPattern() = 0;

#ifdef SWIG
%extend
{
    InterpreterObject pattern()
    {
	return HOMboostAnyToInterpreterObject(self->boostAnyPattern());
    }
}
#endif

    virtual void setPattern(const char *pattern_or_none) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Node> > nodes() = 0;

    virtual bool containsNode(HOM_Node &node) = 0;
    virtual void addNode(HOM_Node &node) = 0;
    virtual void removeNode(HOM_Node &base_node) = 0;
    virtual void clear() = 0;

    virtual bool isSelected() = 0;

    SWIGOUT(%kwargs setSelected;)
    virtual void setSelected(bool on, bool clear_all_selected=false) = 0;

    virtual void convertToNormalBundle() = 0;
    virtual void convertToSmartBundle() = 0;
};

#endif
