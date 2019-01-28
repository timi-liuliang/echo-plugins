/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_Operator.h ( DOP Library, C++)
 *
 * COMMENTS:	Special class which has additional knowledge about the operator
 *		being described.
 */

#ifndef __DOP_Operator__
#define __DOP_Operator__

#include "DOP_API.h"
#include "DOP_OperatorInfo.h"
#include <OP/OP_Operator.h>


#define DOP_MAX_NUMINPUTS	10
#define DOP_MAX_NUMOUTPUTS	4



class DOP_API DOP_Operator : public OP_Operator {
public:
    DOP_Operator(const char	   *name,
		  const char	   *english,
		  OP_Constructor    construct,
		  PRM_Template	   *templates,
		  unsigned	    min_sources,
		  unsigned	    max_sources,
		  CH_LocalVariable *variables,
		  unsigned	    flags,
		  unsigned	    num_outputs);

    DOP_Operator(const char	   *name,
		  const char	   *english,
		  OP_Constructor    construct,
		  PRM_Template	   *templates,
		  const char	   *child_table_name,
		  unsigned	    min_sources,
		  unsigned	    max_sources,
		  CH_LocalVariable *variables,
		  unsigned	    flags,
		  unsigned	    num_outputs);

    virtual ~DOP_Operator();

    virtual OP_SpecificData		*getOpSpecificData()
					 { return &myOperatorInfo; }
    virtual const OP_SpecificData	*getOpSpecificData() const
					 { return &myOperatorInfo; }

    virtual void			 getOperatorSpecificInfoText(
						int verbose,
						UT_WorkBuffer &text);

    virtual bool			 wantsInputEditor() const;

    virtual unsigned			 maxOutputs();

private:
    DOP_OperatorInfo			 myOperatorInfo;
};

#endif
