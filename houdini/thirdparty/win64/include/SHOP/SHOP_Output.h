/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_Output.h ( SHOP Library, C++)
 *
 * COMMENTS:	Output nodes for SHOPs.  This probably doesn't have to be
 *	sub-classed off of SHOP_VariableInput since the number of inputs is
 *	fixed.  However, since it hides its unconnected inputs, it does a lot
 *	of the things that the variable input needs to do when the inputs
 *	change.
 */

#ifndef __SHOP_Output__
#define __SHOP_Output__

#include "SHOP_API.h"
#include "SHOP_VariableInput.h"

class SHOP_API SHOP_Output : public SHOP_VariableInput
{
public:
    SHOP_Output(OP_Network *dad, const char *name,
		OP_Operator *entry, SHOP_TYPE shader_type=SHOP_MUTABLE)
	: SHOP_VariableInput(dad, name, entry, shader_type)
    {
    }
    virtual ~SHOP_Output();

    // Number of outputs in the output SHOP
    static const int	 theMaxInputSize;

    static OP_Node	*myConstructor(OP_Network *net,
				       const char *name,
				       OP_Operator *entry);
    static PRM_Template	 myTemplateList[];

    virtual SHOP_Node	*findShader(SHOP_TYPE type, fpreal now,
				const UT_Options *options);
    virtual int		 collectShaders(UT_ValArray<SHOP_Node *>&list,
				    SHOP_TYPE shop_type, fpreal now,
				    const UT_Options *options);

    virtual unsigned	 getNumVisibleInputs() const;
    virtual unsigned	 maxInputs() const;
    virtual void	 getInputName(UT_String &name, int idx) const;
    virtual SHOP_TYPE	 getInputType(int idx) const;
    virtual SHOP_TYPE	 getNamedInputType(
				const OP_ConnectorId& input_name) const;
    virtual OP_ERROR	 setInput(unsigned idx, OP_Node *op,
				unsigned outputIdx=0);

    virtual OP_ERROR	 setNamedInput(
				const OP_ConnectorId& input_name, OP_Node *op,
				const OP_ConnectorId* output_name = NULL);

    virtual int		 getInputTileLOD() const;
    virtual bool	 isInputVisible(int idx) const;

    static int		 getMaterialOutputCount();

protected:
    virtual void	 updateVisibleInputsAndOutputs();
    virtual void	 preOpChanged(OP_EventType Reason, void *data);
    SHOP_Node *		 getInputShop(SHOP_TYPE type);
};

#endif
