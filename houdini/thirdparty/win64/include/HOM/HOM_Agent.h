/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Agent_h__
#define __HOM_Agent_h__

#include "HOM_API.h"
#include "HOM_ElemPtr.h"
#include "HOM_PackedPrim.h"

class HOM_AgentClip;
class HOM_AgentDefinition;
class HOM_AgentLayer;
class HOM_AgentRig;

SWIGOUT(%rename(Agent) HOM_Agent;)

class HOM_API HOM_Agent : virtual public HOM_PackedPrim
{
public:
    HOM_Agent()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_Agent(const HOM_Agent &prim)
    : HOM_Prim(prim), HOM_PackedPrim(prim)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_Agent()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    SWIGOUT(%newobject definition;)
    virtual HOM_AgentDefinition *definition() = 0;

    virtual void setDefinition(const HOM_AgentDefinition &definition) = 0;

    SWIGOUT(%newobject currentLayer;)
    virtual HOM_AgentLayer *currentLayer() = 0;

    virtual void setCurrentLayer(const HOM_AgentLayer &layer) = 0;

    SWIGOUT(%newobject collisionLayer;)
    virtual HOM_AgentLayer *collisionLayer() = 0;

    virtual void setCollisionLayer(const HOM_AgentLayer &layer) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_AgentClip> > clips() = 0;

    virtual void setClips(const std::vector<HOM_AgentClip *> &clips) = 0;

    virtual std::vector<double> clipTimes() = 0;

    virtual void setClipTimes(const std::vector<double> &times) = 0;

    virtual std::vector<double> clipWeights() = 0;

    virtual void setClipWeights(const std::vector<double> &weights) = 0;

    SWIGOUT(%newobject localTransform;)
    virtual HOM_Matrix4 *localTransform(int i) = 0;

    virtual void setLocalTransform(const HOM_Matrix4 &xform, int i) = 0;

    SWIGOUT(%newobject worldTransform;)
    virtual HOM_Matrix4 *worldTransform(int i) = 0;

    virtual void setWorldTransform(const HOM_Matrix4 &xform, int i) = 0;
};

#endif
