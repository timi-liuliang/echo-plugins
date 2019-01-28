/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_AgentShapeDeformer_h__
#define __HOM_AgentShapeDeformer_h__

#include "HOM_API.h"
#include "HOM_ElemPtr.h"
#include "HOM_PtrOrNull.h"
#include "HOM_Module.h"

class HOM_AgentRig;
class HOM_AgentShapeBinding;
class HOM_AgentShapeLibrary;

SWIGOUT(%rename(AgentShapeDeformer) HOM_AgentShapeDeformer;)

class HOM_API HOM_AgentShapeDeformer
{
public:
    HOM_AgentShapeDeformer()
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_AgentShapeDeformer()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_AgentShapeDeformer> deformer) const = 0;
    bool operator!=(HOM_PtrOrNull<HOM_AgentShapeDeformer> deformer) const
        { return !operator==(deformer); }

    virtual int __hash__() const = 0;
    virtual std::string __repr__() const = 0;

    virtual std::string name() const = 0;
};

#endif

