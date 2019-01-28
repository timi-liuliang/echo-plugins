/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_AgentSubjectShape.h ( GSTY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_AgentSubjectShape__
#define __GU_AgentSubjectShape__

#include "GU_API.h"
#include "GU_AgentLayer.h"
#include <STY/STY_SubjectNamed.h>

class GU_PrimPacked;

class GU_API GU_AgentSubjectShape : public STY_SubjectNamed
{
public:
    // The 'agent_prim' arg is optional since it's used only for a convenience
    // shortcut to have shape implicitly provide script binding data from agent.
    // Otherwise we would have to employ preBinding keyword explicitly.
    GU_AgentSubjectShape(const GU_AgentLayer::ShapeBinding& shape,
	    const GU_PrimPacked *agent_prim = NULL);

    virtual STY_TargetMatchStatus   
	matchesStyleTarget(const STY_TargetHandle &target) const;

    virtual STY_BindingDataSourceHandle
		    getOverrideBindingDataSource(
			    const UT_StringHolder &data_name) const;

protected:
    virtual UT_StringHolder getSubjectName() const
    {
	return myShape->shapeName();
    }

    virtual int64	 getSubjectID() const
    {
	return myShape->shapeId();
    }

private:
    const GU_AgentLayer::ShapeBinding	*myShape;
    const GU_PrimPacked			*myAgentPrim; 
};

#endif

