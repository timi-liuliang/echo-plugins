/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_AgentShape_h__
#define __HOM_AgentShape_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"

SWIGOUT(%rename(AgentShape) HOM_AgentShape;)

class HOM_API HOM_AgentShape
{
public:
    HOM_AgentShape()
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_AgentShape()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_AgentShape> lib) const = 0;
    bool operator!=(HOM_PtrOrNull<HOM_AgentShape> lib) const
        { return !operator==(lib); }

    virtual int __hash__() const = 0;
    virtual std::string __repr__() const = 0;

    virtual std::string name() const = 0;
    virtual int uniqueId() const = 0;

    SWIGOUT(%newobject geometry;)
    virtual HOM_Geometry *geometry() const = 0;
};

#endif
