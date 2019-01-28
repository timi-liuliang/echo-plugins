/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __SOP_Circle_h__
#define __SOP_Circle_h__

#include "SOP_API.h"
#include <SYS/SYS_Math.h>
#include "SOP_Node.h"

class SOP_API SOP_Circle : public SOP_Node
{
public:
	     SOP_Circle(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_Circle();

    static OP_Node	*myConstructor(OP_Network *net,
				const char *name, OP_Operator *entry);
    static PRM_Template	*buildTemplates();
    const SOP_NodeVerb	*cookVerb() const override;

protected:
    virtual OP_ERROR	cookMySop(OP_Context &context) override;
};

#endif
