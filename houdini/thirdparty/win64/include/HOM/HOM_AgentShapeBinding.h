/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_AgentShapeBinding_h__
#define __HOM_AgentShapeBinding_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"
#include "HOM_Module.h"

class HOM_AgentShape;
class HOM_AgentShapeDeformer;

SWIGOUT(%rename(AgentShapeBinding) HOM_AgentShapeBinding;)
// Force SWIG to output an __init__ method even though it determines this
// class is abstract.
SWIGOUT(%feature("notabstract") HOM_AgentShapeBinding;)

class HOM_API HOM_AgentShapeBinding
{
public:
#ifdef SWIG
%extend
{
    HOM_AgentShapeBinding(int transform, const HOM_AgentShape &shape,
                          bool deforming, double bounds_scale = 1.0)
    {
        return HOM().newAgentShapeBinding(transform, shape, deforming,
                                          bounds_scale);
    }

    HOM_AgentShapeBinding(int transform, const HOM_AgentShape &shape,
                          const HOM_AgentShapeDeformer &deformer,
                          double bounds_scale = 1.0)
    {
        return HOM().newAgentShapeBinding(transform, shape, deformer,
                                          bounds_scale);
    }
}
#else
    HOM_AgentShapeBinding()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_AgentShapeBinding()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool
    operator==(HOM_PtrOrNull<HOM_AgentShapeBinding> binding) const = 0;
    bool operator!=(HOM_PtrOrNull<HOM_AgentShapeBinding> binding) const
        { return !operator==(binding); }

    virtual int __hash__() const = 0;
    virtual std::string __repr__() const = 0;

    virtual std::string shapeName() const = 0;
    virtual int shapeId() const = 0;
    virtual int transformId() const = 0;
    virtual bool isDeforming() const = 0;
    virtual double boundsScale() const = 0;

    SWIGOUT(%newobject shape;)
    virtual HOM_AgentShape *shape() const = 0;

    SWIGOUT(%newobject deformer;)
    virtual HOM_AgentShapeDeformer *deformer() const = 0;
};

#endif
