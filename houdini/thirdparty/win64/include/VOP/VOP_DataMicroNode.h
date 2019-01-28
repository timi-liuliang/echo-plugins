/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_DataMicroNode.h (OP Library, C++)
 *
 * COMMENTS:	OP_DataMicroNode subclass representing a VOP_Node's output data.
 */

#ifndef __VOP_DATAMICRONODE_H_INCLUDED__
#define __VOP_DATAMICRONODE_H_INCLUDED__

#include "VOP_API.h"
#include <OP/OP_DataMicroNode.h>

class OP_Node;

class VOP_API VOP_DataMicroNode : public OP_DataMicroNode
{
public:
    explicit		    VOP_DataMicroNode(OP_Node &node);
    virtual		    ~VOP_DataMicroNode();

    virtual const char *    className() const
				{ return "VOP_DataMicroNode"; }

    virtual void	    getOutputs(DEP_MicroNodeList &inputs) const;
};

#endif // __VOP_DATAMICRONODE_H_INCLUDED__
