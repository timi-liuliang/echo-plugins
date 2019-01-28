/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_crowds_h__
#define __HOM_crowds_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_AgentShapeDeformer.h"
#include "HOM_Vector2.h"
#include "HOM_Vector3.h"

class HOM_AgentDefinition;
class HOM_Geometry;

SWIGOUT(%rename(crowds) HOM_crowds;)

class HOM_API HOM_crowds
{
public:
    virtual ~HOM_crowds() {}

    virtual std::string __repr__() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_AgentDefinition> >
    findAgentDefinitions(HOM_Geometry &geo, const char *group_str = 0) = 0;

    virtual void
    replaceAgentDefinitions(HOM_Geometry &geo,
        const std::map<HOM_AgentDefinition *, HOM_AgentDefinition *> &defn_map,
        const char *group_str = 0) = 0;

    virtual std::map<std::string, hboost::any>
    computeRotationLimits(const HOM_AgentRig &rig,
                          const std::vector<HOM_AgentClip *> &clips,
                          int xform_idx, int parent_xform_idx) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_AgentShapeDeformer> >
    shapeDeformers() = 0;
};

#endif
