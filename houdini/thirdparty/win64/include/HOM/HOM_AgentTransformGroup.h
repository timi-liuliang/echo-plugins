/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_AgentTransformGroup_h__
#define __HOM_AgentTransformGroup_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"

class HOM_AgentRig;

SWIGOUT(%rename(AgentTransformGroup) HOM_AgentTransformGroup;)
// Force SWIG to output an __init__ method even though it determines this
// class is abstract.
SWIGOUT(%feature("notabstract") HOM_AgentTransformGroup;)

class HOM_API HOM_AgentTransformGroup
{
public:
#ifdef SWIG
%extend
{
    HOM_AgentTransformGroup(const char *path, const HOM_AgentRig &rig,
                            bool keep_external_ref = true)
    { return HOM().newAgentTransformGroup(path, rig, keep_external_ref); }

    HOM_AgentTransformGroup(
        const char *name, const std::vector<int> &transforms,
        const HOM_AgentRig &rig,
        const std::vector<double> &weights = std::vector<double>())
    { return HOM().newAgentTransformGroup(name, transforms, rig, weights); }
}
#else
    HOM_AgentTransformGroup()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_AgentTransformGroup()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool
    operator==(HOM_PtrOrNull<HOM_AgentTransformGroup> group) const = 0;

    bool operator!=(HOM_PtrOrNull<HOM_AgentTransformGroup> group) const
        { return !operator==(group); }

    virtual int __hash__() const = 0;
    virtual std::string __repr__() const = 0;

    virtual std::string asJSON() const = 0;

    virtual std::string name() const = 0;
    SWIGOUT(%kwargs fileName;)
    virtual std::string fileName(bool expanded = false) const = 0;

    virtual std::vector<int> transformIndices() const = 0;
    virtual std::vector<double> weights() const = 0;
};

#endif
