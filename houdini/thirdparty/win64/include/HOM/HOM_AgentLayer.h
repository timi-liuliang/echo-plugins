/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_AgentLayer_h__
#define __HOM_AgentLayer_h__

#include "HOM_API.h"
#include "HOM_ElemPtr.h"
#include "HOM_PtrOrNull.h"
#include "HOM_Module.h"

class HOM_AgentRig;
class HOM_AgentShapeBinding;
class HOM_AgentShapeLibrary;

SWIGOUT(%rename(AgentLayer) HOM_AgentLayer;)
// Force SWIG to output an __init__ method even though it determines this
// class is abstract.
SWIGOUT(%feature("notabstract") HOM_AgentLayer;)

class HOM_API HOM_AgentLayer
{
public:
#ifdef SWIG
%extend
{
    HOM_AgentLayer(const char *name, const HOM_AgentRig &rig,
                   const HOM_AgentShapeLibrary &shapelib,
                   const std::vector<HOM_AgentShapeBinding *> &shape_bindings,
                   const HOM_AgentLayer *source_layer = 0)
    {
        return HOM().newAgentLayer(name, rig, shapelib, shape_bindings,
                                   source_layer);
    }

    HOM_AgentLayer(const char *path, const HOM_AgentRig &rig,
                   const HOM_AgentShapeLibrary &shapelib,
                   bool keep_external_ref = true)
    {
        return HOM().newAgentLayer(path, rig, shapelib, keep_external_ref);
    }
}
#else
    HOM_AgentLayer()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_AgentLayer()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_AgentLayer> layer) const = 0;
    bool operator!=(HOM_PtrOrNull<HOM_AgentLayer> layer) const
        { return !operator==(layer); }

    virtual int __hash__() const = 0;
    virtual std::string __repr__() const = 0;

    virtual std::string asJSON() const = 0;

    virtual std::string name() const = 0;

    SWIGOUT(%kwargs fileName;)
    virtual std::string fileName(bool expanded = false) const = 0;

    virtual std::vector<HOM_ElemPtr<HOM_AgentShapeBinding> > bindings() const = 0;

    virtual std::vector<HOM_ElemPtr<HOM_AgentShapeBinding> >
    bindings(int transform) const = 0;

    virtual std::vector<HOM_ElemPtr<HOM_AgentShapeBinding> > staticBindings() const = 0;

    virtual std::vector<HOM_ElemPtr<HOM_AgentShapeBinding> > deformingBindings() const = 0;
};

#endif
