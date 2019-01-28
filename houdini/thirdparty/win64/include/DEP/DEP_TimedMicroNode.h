/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DEP_TimedMicroNode.h (DEP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DEP_TIMEDMICRONODE_H_INCLUDED__
#define __DEP_TIMEDMICRONODE_H_INCLUDED__

#include "DEP_API.h"
#include "DEP_MicroNode.h"
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <float.h>

/// MicroNode that caches the last time it was updated to determine if requires
/// an update.
class DEP_API DEP_TimedMicroNode : public DEP_MicroNode
{
public:
    DEP_TimedMicroNode()
	: myLastUpdateTime(DBL_MAX)
    {
    }

    virtual const char *
    className() const
    {
	return "DEP_TimedMicroNode";
    }

    virtual bool
    requiresUpdate(fpreal t) const
    {
	if (DEP_MicroNode::requiresUpdate(t))
	    return true;
	return (isTimeDependent() && !isSameTime(t, myLastUpdateTime));
    }

    virtual void
    update(fpreal t)
    {
	myLastUpdateTime = t;
	if (!isTimeDependent())
	    inheritTimeDependentFromExplicitInputs();
	DEP_MicroNode::update(t);
    }

private:
    fpreal	myLastUpdateTime;
};

#endif // __DEP_TIMEDMICRONODE_H_INCLUDED__
