/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_PropagateData.h (OP Library, C++)
 *
 * COMMENTS:	Helper class for OP_Node::propagateModification()
 */

#ifndef __OP_PROPAGATEDATA_H__
#define __OP_PROPAGATEDATA_H__

#include "OP_API.h"
#include "OP_Value.h"
#include <UT/UT_Algorithm.h>
#include <DEP/DEP_MicroNode.h>
#include <UT/UT_Pair.h>

// This helper struct is needed for propagateModification(). It must be
// initialized first with beginPropagateModification().
class OP_API OP_PropagateData : public DEP_PropagateData
{
public:
    OP_PropagateData(OP_EventType reason)
    {
	myExprChange = isExprChangeEvent(reason);
    }

    static bool isExprChangeEvent(OP_EventType reason)
		{
		    return (reason == OP_PARM_ANIMATED
			    || reason == OP_PARM_CHANNELS);
		}
};

#endif // __OP_PROPAGATEDATA_H__
