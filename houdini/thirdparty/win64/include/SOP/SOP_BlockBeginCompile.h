/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_BlockBeginCompile.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_BlockBeginCompile__
#define __SOP_BlockBeginCompile__

#include "SOP_API.h"
#include "SOP_BlockBegin.h"

class SOP_API SOP_BlockBeginCompile : public SOP_BlockBegin
{
public:
	     SOP_BlockBeginCompile(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_BlockBeginCompile();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		 myTemplateList[];

    virtual void	 FORPATH(UT_String &str, fpreal t) const
			 { evalString(str, "blockpath", 0, t); }
    virtual void	 NAME(UT_String &str, fpreal t) const
			 { evalString(str, "name", 0, t); }

    static int			 buildBeginBlockStatic(void *op, int idx, 
					fpreal t, const PRM_Template *);

    virtual void	 getDescriptiveParmName(UT_String &name) const
			 { name = "name"; }
protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context);

    virtual const char	*inputLabel(unsigned idx) const;

private:
};


#endif


