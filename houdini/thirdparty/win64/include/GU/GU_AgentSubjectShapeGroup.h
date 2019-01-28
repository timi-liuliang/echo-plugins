/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_AgentSubjectShapeGroup.h ( GSTY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_AgentSubjectShapeGroup
#define __GU_AgentSubjectShapeGroup

#include "GU_API.h"
#include "GU_AgentLayer.h"
#include "GU_AgentSubjectShape.h"
#include <STY/STY_SubjectGroup.h>

class GU_PrimPacked;

class GU_API GU_AgentSubjectShapeGroup : public STY_SubjectGroup
{
public:
    // The 'agent_prim' arg is optional since it's used only for a convenience
    // shortcut to have shape implicitly provide script binding data from agent.
    // Otherwise we would have to employ preBinding keyword explicitly.
    GU_AgentSubjectShapeGroup(const GU_AgentLayer::ShapeArray& shapes,
	    const GU_PrimPacked *agent_prim = NULL);

    virtual int			 getNumSubjects() const;
    virtual const STY_Subject	&getSubject(int idx) const;

private:
    UT_Array<GU_AgentSubjectShape>	 mySubjects;
};

#endif

