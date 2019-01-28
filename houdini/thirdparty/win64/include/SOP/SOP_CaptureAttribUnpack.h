/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_CaptureAttribUnpack.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_CaptureAttribUnpack__
#define __SOP_CaptureAttribUnpack__

#include "SOP_API.h"
#include "SOP_Node.h"

class SOP_API SOP_CaptureAttribUnpack : public SOP_Node
{
public:
	     SOP_CaptureAttribUnpack(OP_Network *net, const char *,OP_Operator *entry);
    virtual ~SOP_CaptureAttribUnpack();

    static OP_Node	*myConstructor(OP_Network  *net, const char *name,
				       OP_Operator *entry);
    static PRM_Template *buildTemplates();

protected:
    virtual OP_ERROR cookMySop(OP_Context &context) override;
    const SOP_NodeVerb *cookVerb() const override;
};

#endif

