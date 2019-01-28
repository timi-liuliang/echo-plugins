/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_AgentLinearSkinDeformer.h (GU Library, C++)
 *
 */

#ifndef __GU_AgentLinearSkinDeformer__
#define __GU_AgentLinearSkinDeformer__

#include "GU_API.h"
#include "GU_AgentShapeDeformer.h"

/// The default deformer for agents, which uses GU_LinearSkinDeformer.
class GU_API GU_AgentLinearSkinDeformer : public GU_AgentShapeDeformer
{
public:
    enum class Method
    {
        Linear,
        DualQuat
    };

    GU_AgentLinearSkinDeformer(Method method = Method::Linear);

    virtual void deform(GU_Detail &gdp, const GA_Range &pt_range,
                        const GU_AgentShapeLib::Shape &src_shape,
                        const GU_Agent &src_agent) const override;

private:
    Method myMethod;
};

#endif
