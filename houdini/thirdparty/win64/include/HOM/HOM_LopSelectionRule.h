/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_LopSelectionRule_h__
#define __HOM_LopSelectionRule_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"

class HOM_LopNode;

SWIGOUT(%rename(LopSelectionRule) HOM_LopSelectionRule;)

SWIGOUT(%feature("notabstract") HOM_LopSelectionRule;)
class HOM_API HOM_LopSelectionRule
{
public:
#ifdef SWIG
%extend
{
    HOM_LopSelectionRule()
    { return HOM().newLopSelectionRule(); }
}
#else
    HOM_LopSelectionRule()
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_LopSelectionRule()
    { HOM_DESTRUCT_OBJECT(this) }
#endif

    virtual bool operator==(HOM_PtrOrNull<HOM_LopSelectionRule> options) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_LopSelectionRule> options) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    virtual std::vector<std::string> expandedPaths(
	    HOM_LopNode *lopnode) = 0;
    virtual std::vector<std::string> collectionAwarePaths(
	    HOM_LopNode *lopnode) = 0;

    virtual std::string pathPattern() = 0;
    virtual void setPathPattern(const char *pattern) = 0;
};

#endif
