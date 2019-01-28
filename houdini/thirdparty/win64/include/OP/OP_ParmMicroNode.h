/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_ParmMicroNode.h (OP Library, C++)
 *
 * COMMENTS:	PRM_ParmMicroNode subclass representing an PRM_Parm which
 *		sends out OP_PARM_CHANGED events.
 */

#ifndef __OP_PARMMICRONODE_H_INCLUDED__
#define __OP_PARMMICRONODE_H_INCLUDED__

#include "OP_API.h"
#include <PRM/PRM_ParmMicroNode.h>
#include <UT/UT_Assert.h>
#include <SYS/SYS_Types.h>

class OP_Node;

class OP_API OP_ParmMicroNode : public PRM_ParmMicroNode
{
public:
			    OP_ParmMicroNode()
				: PRM_ParmMicroNode()
				{ }
			    OP_ParmMicroNode(PRM_Parm &parm, int sub_index)
				: PRM_ParmMicroNode(parm, sub_index)
				{ }

    virtual const char *    className() const
				{ return "OP_ParmMicroNode"; }

    virtual void	    getOutputs(DEP_MicroNodeList &outputs) const;

    virtual void	    dump(
				std::ostream &os,
				const char *prefix = 0,
				int indent_level = 0) const;

    OP_Node *		    ownerNodePtr() const;

    virtual void		becameDirty(DEP_MicroNode &src, const DEP_PropagateData &data);
private:

};

#endif // __OP_PARMMICRONODE_H_INCLUDED__
