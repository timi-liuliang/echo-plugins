/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP library (C++)
 *
 * COMMENTS:	UVFlatten SOP
 */


#ifndef __SOP_UVFlatten3_h__
#define __SOP_UVFlatten3_h__

#include "SOP_API.h"
#include "SOP_Node.h"

#include "SOP_Node.h"

class DEP_MicroNode;

class SOP_API SOP_UVFlatten3 : public SOP_Node
{
public:
			 SOP_UVFlatten3(OP_Network *net, const char *name,
				 OP_Operator *op);

    virtual int		 isRefInput(unsigned int i) const override;

    static PRM_Template  *buildTemplates();

    const SOP_NodeVerb   *cookVerb() const override final;    

    static OP_Node	*myConstructor(OP_Network*,const char *, OP_Operator *);

    virtual OP_ERROR     cookInputGroups(OP_Context &context, int alone = 0) override;

    virtual void getDescriptiveParmName(UT_String &name) const override final
    {
        name = "uvattrib";
    }

protected:
    virtual const char	*inputLabel(unsigned idx) const override;

    // Method to cook geometry for the SOP
    virtual OP_ERROR	 cookMySop(OP_Context &context) override;

};

#endif
