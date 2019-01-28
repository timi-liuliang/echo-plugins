/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_AgentRig_h__
#define __HOM_AgentRig_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"

SWIGOUT(%rename(AgentRig) HOM_AgentRig;)
// Force SWIG to output an __init__ method even though it determines this
// class is abstract.
SWIGOUT(%feature("notabstract") HOM_AgentRig;)

class HOM_API HOM_AgentRig
{
public:
#ifdef SWIG
%extend
{
    HOM_AgentRig(const char *path, bool keep_external_ref = true)
    { return HOM().newAgentRig(path, keep_external_ref); }

    HOM_AgentRig(const char *name,
                 const std::vector<std::string> &transform_names,
                 const std::vector<std::vector<int> > &hierarchy)
    { return HOM().newAgentRig(name, transform_names, hierarchy); }
}
#else
    HOM_AgentRig()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_AgentRig()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_AgentRig> rig) const = 0;
    bool operator!=(HOM_PtrOrNull<HOM_AgentRig> rig) const
        { return !operator==(rig); }

    virtual int __hash__() const = 0;
    virtual std::string __repr__() const = 0;

    virtual std::string asJSON() const = 0;

    virtual std::string name() const = 0;

    SWIGOUT(%kwargs fileName;)
    virtual std::string fileName(bool expanded = false) const = 0;

    virtual int transformCount() = 0;
    virtual std::string transformName(int i) = 0;
    virtual int findTransform(const std::string &transform_name) = 0;

    virtual int parentIndex(int transform) = 0;
    virtual std::vector<int> childIndices(int transform) = 0;
};

#endif
