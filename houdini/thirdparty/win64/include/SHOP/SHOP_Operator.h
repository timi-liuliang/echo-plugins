/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_Operator.h ( SHOP Library, C++)
 *
 * COMMENTS:	Special class which has additional knowledge about the operator
 *		being described.
 */

#ifndef __SHOP_Operator__
#define __SHOP_Operator__

#include "SHOP_API.h"
#include <OP/OP_Operator.h>
#include "SHOP_NodeTypes.h"
#include "SHOP_OperatorInfo.h"

class SHOP_API SHOP_Operator : public OP_Operator {
public:
    SHOP_Operator(const char	   *name,
		  const char	   *english,
		  OP_Constructor    construct,
		  PRM_Template	   *templates,
		  const char	   *child_table_name,
		  unsigned	    min_sources,
		  unsigned	    max_sources = 9999,
		  CH_LocalVariable *variables = 0,
		  unsigned	    flags = 0,
		  int		    auto_add_clerk=SHOP_AUTOADD_ANY);

    SHOP_Operator(const char	  *name,
		  const char	  *english,
		  OP_Constructor   construct,
		  OP_TemplatePair *template_pair,
		  const char	   *child_table_name,
		  unsigned	   min_sources,
		  unsigned	   max_sources = 9999,
		  OP_VariablePair *variable_pair = 0,
		  unsigned	   flags = 0,
		  int		   auto_add_clerk=SHOP_AUTOADD_ANY);

    virtual ~SHOP_Operator();

    void		 useShaderIconName();

    virtual int		 compareOperator(const OP_Operator *other,
					 int method) const;

    virtual unsigned	 maxOutputs();
    virtual bool	 wantsInputEditor() const;

    // This method will return a pointer to the SHOP_OperatorInfo class
    virtual OP_SpecificData		*getOpSpecificData()
					 { return &myOperatorInfo; }
    virtual const OP_SpecificData	*getOpSpecificData() const
					 { return &myOperatorInfo; }

private:
    SHOP_OperatorInfo	myOperatorInfo;
};

#endif
