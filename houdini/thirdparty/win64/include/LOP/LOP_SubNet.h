/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	LOP Library (C++)
 *
 */

#ifndef __LOP_SubNet_h__
#define __LOP_SubNet_h__

#include "LOP_API.h"
#include "LOP_Node.h"

class LOP_API LOP_SubNet : public LOP_Node
{
public:
    // Standard OP_Network stuff:
    static const char	*theChildTableName;
    static OP_Node	*myConstructor(OP_Network *net, const char *name,
				OP_Operator *entry);
    static PRM_Template	 myTemplateList[];

    virtual unsigned	 getNumVisibleOutputs() const override;

protected:
	     LOP_SubNet(OP_Network *parent, const char *name,
		       OP_Operator *entry);
    virtual ~LOP_SubNet();

    virtual void	 childDeleted(OP_Node *child) override;
    virtual void	 childFlagChange(OP_Node *child) override;

    virtual OP_ERROR	 cookMyLop(OP_Context &context) override;
};

#endif
