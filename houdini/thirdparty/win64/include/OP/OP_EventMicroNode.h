/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_EventMicroNode.h (OP Library, C++)
 *
 * COMMENTS:	DEP_MicroNode subclass representing an OP_Node's output data.
 */

#ifndef __OP_EVENTMICRONODE_H_INCLUDED__
#define __OP_EVENTMICRONODE_H_INCLUDED__

#include "OP_API.h"
#include "OP_Value.h"
#include <DEP/DEP_MicroNode.h>

class OP_Node;

class OP_API OP_EventMicroNode : public DEP_MicroNode
{
public:
			    OP_EventMicroNode(
				OP_Node &node, OP_EventType type)
				: myNode(node),
				  myType(type)
				{}

    virtual const char *    className() const
				{ return "OP_EventMicroNode"; }

    OP_Node &		    ownerNode() const
				{ return myNode; }

    OP_EventType	    eventType() const
				{ return myType; }

    virtual void		becameDirty(DEP_MicroNode &src, const DEP_PropagateData &data);
private:
    OP_Node &		    myNode;
    OP_EventType	    myType;
};

#endif // __OP_EVENTMICRONODE_H_INCLUDED__
