/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP library (C++)
 *
 * COMMENTS:	UVLayout SOP
 */


#ifndef __SOP_UVLayout2_h__
#define __SOP_UVLayout2_h__

#include "SOP_API.h"
#include "SOP_Node.h"

class SOP_API SOP_UVLayout2 : public SOP_Node
{
public:
			 SOP_UVLayout2(OP_Network *net, const char *name,
				      OP_Operator *op);

    virtual int		 isRefInput(unsigned int i) const override;

    virtual OP_ERROR	 cookInputGroups(OP_Context &context,
					int alone = 0) override;

    static PRM_Template	*buildTemplates();
    const SOP_NodeVerb	*cookVerb() const override final;

    static OP_Node	*myConstructor(OP_Network*, const char *,
				OP_Operator *);

    virtual void getDescriptiveParmName(UT_String &name) const override final
    {
        name = "uvattr";
    }

protected:
    virtual const char  *inputLabel(unsigned idx) const override;
    void		 getDescriptiveName(UT_String &name) const override;

    virtual OP_ERROR	 cookMySop(OP_Context &context) override;
};

#endif
