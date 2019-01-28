/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_AgentDefinition_h__
#define __HOM_AgentDefinition_h__

#include "HOM_API.h"
#include "HOM_IterableList.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"

class HOM_AgentClip;
class HOM_AgentLayer;
class HOM_AgentRig;
class HOM_AgentShapeLibrary;
class HOM_AgentTransformGroup;

SWIGOUT(%rename(AgentDefinition) HOM_AgentDefinition;)
// Force SWIG to output an __init__ method even though it determines this
// class is abstract.
SWIGOUT(%feature("notabstract") HOM_AgentDefinition;)

class HOM_API HOM_AgentDefinition
{
public:
#ifdef SWIG
%extend
{
    HOM_AgentDefinition(const HOM_AgentRig &rig,
                        const HOM_AgentShapeLibrary &shapelib)
    { return HOM().newAgentDefinition(rig, shapelib); }
}
#else
    HOM_AgentDefinition()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_AgentDefinition()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_AgentDefinition> defn) const = 0;
    bool operator!=(HOM_PtrOrNull<HOM_AgentDefinition> defn) const
        { return !operator==(defn); }

    virtual int __hash__() const = 0;
    virtual std::string __repr__() const = 0;

    SWIGOUT(%newobject freeze;)
    virtual HOM_AgentDefinition *
    freeze(HOM_AgentShapeLibrary *new_shapelib = 0) const = 0;

    SWIGOUT(%newobject rig;)
    virtual HOM_AgentRig *rig() const = 0;

    SWIGOUT(%newobject shapeLibrary;)
    virtual HOM_AgentShapeLibrary *shapeLibrary() const = 0;

    SWIGOUT(%newobject layers;)
    virtual HOM_IterableList<HOM_AgentLayer> *layers() const = 0;

    SWIGOUT(%newobject findLayer;)
    virtual HOM_AgentLayer * findLayer(const char *name) const = 0;

    SWIGOUT(%newobject clips;)
    virtual HOM_IterableList<HOM_AgentClip> *clips() const = 0;

    SWIGOUT(%newobject findClip;)
    virtual HOM_AgentClip * findClip(const char *name) const = 0;

    SWIGOUT(%newobject transformGroups;)
    virtual HOM_IterableList<HOM_AgentTransformGroup> *
    transformGroups() const = 0;

    SWIGOUT(%newobject findTransformGroup;)
    virtual HOM_AgentTransformGroup *
    findTransformGroup(const char *name) const = 0;

    virtual void addClip(const HOM_AgentClip &clip) = 0;

    virtual void addLayer(const HOM_AgentLayer &layer) = 0;

    virtual void addTransformGroup(const HOM_AgentTransformGroup &group) = 0;
};

#endif
