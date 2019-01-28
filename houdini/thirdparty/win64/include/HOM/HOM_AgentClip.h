/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_AgentClip_h__
#define __HOM_AgentClip_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"

class HOM_AgentRig;
class HOM_ChopNode;
class HOM_Matrix4;

SWIGOUT(%rename(AgentClip) HOM_AgentClip;)
// Force SWIG to output an __init__ method even though it determines this
// class is abstract.
SWIGOUT(%feature("notabstract") HOM_AgentClip;)

class HOM_API HOM_AgentClip
{
public:
#ifdef SWIG
%extend
{
    HOM_AgentClip(const char *name, const char *path, const HOM_AgentRig &rig,
                  bool keep_external_ref = true)
    { return HOM().newAgentClip(name, path, rig, keep_external_ref); }

    HOM_AgentClip(const char *name, HOM_ChopNode &chop, const HOM_AgentRig &rig,
                  double frame = HOM_AgentClip::currentFrame())
    { return HOM().newAgentClip(name, chop, rig, frame); }

    HOM_AgentClip(const HOM_AgentClip &clip, const HOM_AgentRig &rig,
                  const char *name = nullptr)
    { return HOM().newAgentClip(clip, rig, name); }
}
#else
    HOM_AgentClip()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_AgentClip()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_AgentClip> clip) const = 0;
    bool operator!=(HOM_PtrOrNull<HOM_AgentClip> clip) const
        { return !operator==(clip); }

    virtual int __hash__() const = 0;
    virtual std::string __repr__() const = 0;

    virtual std::string name() const = 0;
    SWIGOUT(%kwargs fileName;)
    virtual std::string fileName(bool expanded = false) const = 0;
    virtual double length() const = 0;
    virtual double sampleRate() const = 0;

    SWIGOUT(%newobject sampleLocal;)
    virtual HOM_Matrix4 *sampleLocal(double time, int transform) const = 0;
    SWIGOUT(%newobject sampleWorld;)
    virtual HOM_Matrix4 *sampleWorld(double time, int transform) const = 0;

    virtual double sample(double time, const char *channel) const = 0;
    virtual std::vector<std::string> channelNames() const = 0;

    virtual HOM_BinaryString data(bool binary, bool worldspace = false) const = 0;

    // Works around Swig errors when directly using HOM().frame() as the value
    // of a default argument - it seems to think there are nested classes.
    // We also don't want to add a time dependency here.
    SWIGOUT(%ignore currentFrame;)
    static double currentFrame()
    {
        return HOM().timeToFrame(HOM().getTimeWithoutAddingDependency());
    }
};

#endif
