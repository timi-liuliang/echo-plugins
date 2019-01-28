/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_ErrorMicroNode.h (OP Library, C++)
 *
 * COMMENTS:	DEP_MicroNode subclass representing the generated errors
 *		stored in the VOP code generator.
 *
 */

#ifndef __VOP_ERRORMICRONODE_H_INCLUDED__
#define __VOP_ERRORMICRONODE_H_INCLUDED__

#include "VOP_API.h"
#include <DEP/DEP_MicroNode.h>

class VOP_CodeGenerator;

class VOP_API VOP_ErrorMicroNode : public DEP_MicroNode
{
public:
    explicit		    VOP_ErrorMicroNode(VOP_CodeGenerator &code_gen);
    virtual		    ~VOP_ErrorMicroNode();

    virtual const char *    className() const
				{ return "VOP_ErrorMicroNode"; }

    virtual void	    getInputs(DEP_MicroNodeList &inputs) const;
    virtual void	    clearInputs();
    virtual void	    getOutputs(DEP_MicroNodeList &outputs) const;

private:

    VOP_CodeGenerator &	    myCodeGen;

};

#endif // __VOP_ERRORMICRONODE_H_INCLUDED__
