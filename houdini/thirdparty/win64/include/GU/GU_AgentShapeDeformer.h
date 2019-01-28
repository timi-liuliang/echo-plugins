/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_AgentShapeDeformer.h (GU Library, C++)
 *
 */

#ifndef __GU_AgentShapeDeformer__
#define __GU_AgentShapeDeformer__

#include "GU_API.h"
#include "GU_AgentShapeLib.h"

#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_StringHolder.h>

class GA_Range;
class GU_Agent;
class GU_Detail;

class GU_AgentShapeDeformer;
using GU_AgentShapeDeformerPtr = UT_IntrusivePtr<GU_AgentShapeDeformer>;
using GU_AgentShapeDeformerConstPtr =
    UT_IntrusivePtr<const GU_AgentShapeDeformer>;

/// Defines a deformer for an agent shape.
class GU_API GU_AgentShapeDeformer
    : public UT_IntrusiveRefCounter<GU_AgentShapeDeformer>
{
public:
    GU_AgentShapeDeformer(const UT_StringHolder &name);

    virtual ~GU_AgentShapeDeformer() = default;

    /// Unique identifier for the deformer. This is used when serializing
    /// layers to JSON.
    const UT_StringHolder &name() const { return myName; }

    /// Deform @p pt_range in @p gdp, which was unpacked from the given shape.
    virtual void deform(GU_Detail &gdp, const GA_Range &pt_range,
                        const GU_AgentShapeLib::Shape &src_shape,
                        const GU_Agent &src_agent) const = 0;

private:
    UT_StringHolder myName;
};

#endif
