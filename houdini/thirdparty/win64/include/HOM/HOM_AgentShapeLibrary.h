/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_AgentShapeLibrary_h__
#define __HOM_AgentShapeLibrary_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"

class HOM_AgentShape;
class HOM_Geometry;

SWIGOUT(%rename(AgentShapeLibrary) HOM_AgentShapeLibrary;)
// Force SWIG to output an __init__ method even though it determines this
// class is abstract.
SWIGOUT(%feature("notabstract") HOM_AgentShapeLibrary;)

class HOM_API HOM_AgentShapeLibrary
{
public:
#ifdef SWIG
%extend
{
    HOM_AgentShapeLibrary()
    { return HOM().newAgentShapeLibrary(); }

    HOM_AgentShapeLibrary(const char *path, bool keep_external_ref = true)
    { return HOM().newAgentShapeLibrary(path, keep_external_ref); }
}
#else
    HOM_AgentShapeLibrary()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_AgentShapeLibrary()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_AgentShapeLibrary> lib) const = 0;
    bool operator!=(HOM_PtrOrNull<HOM_AgentShapeLibrary> lib) const
        { return !operator==(lib); }

    virtual int __hash__() const = 0;
    virtual std::string __repr__() const = 0;

    SWIGOUT(%newobject freeze;)
    virtual HOM_AgentShapeLibrary *
    freeze(bool keep_external_ref = false) const = 0;

    virtual std::string name() const = 0;

    SWIGOUT(%kwargs fileName;)
    virtual std::string fileName(bool expanded = false) const = 0;

    SWIGOUT(%newobject findShape;)
    virtual HOM_AgentShape *findShape(const std::string &name) const = 0;

    virtual std::vector<HOM_ElemPtr<HOM_AgentShape> > shapes() const = 0;

    SWIGOUT(%newobject data;)
    virtual HOM_Geometry *data() const = 0;

    SWIGOUT(%newobject addShape;)
    virtual HOM_AgentShape *addShape(const std::string &name,
                                     HOM_Geometry &geometry) const = 0;
};

#endif
