/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_Operator.h ( VOP Library, C++)
 *
 * COMMENTS:	Special class which has additional knowledge about the operator
 *		being described.
 */

#ifndef __VOP_Operator__
#define __VOP_Operator__

#include "VOP_API.h"
#include <OP/OP_Operator.h>
#include "VOP_OperatorInfo.h"

class VOP_API VOP_Operator : public OP_Operator {
public:
    VOP_Operator(const char	   *name,
		  const char	   *english,
		  OP_Constructor    construct,
		  PRM_Template	   *templates,
		  const char	   *child_table_name,
		  unsigned	    min_sources,
		  unsigned	    max_sources,
		  const char	   *vopnetMask,
		  CH_LocalVariable *variables = 0,
		  unsigned	    flags = 0,
		  unsigned	    num_outputs = 1);

    virtual ~VOP_Operator();

    virtual OP_SpecificData		*getOpSpecificData()
					 { return &myOperatorInfo; }
    virtual const OP_SpecificData	*getOpSpecificData() const
					 { return &myOperatorInfo; }

    virtual unsigned	 maxOutputs();

    virtual bool	 wantsInputEditor() const;
    virtual bool	 wantsParmOrderEditor() const;
    virtual bool	 wantsOutputNameEditor() const;
    virtual bool	 wantsParentInputs() const;

private:
    VOP_OperatorInfo	 myOperatorInfo;
    unsigned		 myNumOutputs;
};

#endif
