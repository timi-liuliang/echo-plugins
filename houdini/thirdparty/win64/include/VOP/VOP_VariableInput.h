/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 */

#ifndef __VOP_VariableInput_h__
#define __VOP_VariableInput_h__

#include "VOP_API.h"
#include "VOP_Node.h"

#define VOP_UNIFORM_VARYING_COMBO(vtype, utype)	\
		vtype, vtype, \
		vtype, utype, \
		utype, utype,

#define VOP_UNIFORM_VARYING_COMBO2(vtype1, utype1, vtype2, utype2)	\
		vtype1, vtype2, \
		vtype1, utype2, \
		utype1, utype2,

class VOP_API VOP_VariableInput : public VOP_Node
{
public:
    virtual void		 getCode(UT_String &codestr,
                		         const VOP_CodeGenContext &context);
    virtual void		 ensureParametersAreValid();

    virtual bool		 forceInputsToBool() const { return false; }

    virtual const char		*inputLabel(unsigned idx) const;
    virtual const char		*outputLabel(unsigned idx) const;

    virtual unsigned		 getNumVisibleInputs() const;
    virtual unsigned		 orderedInputs() const;

    // Get details that will vary from operator to operator.
    virtual const char		*getVariableOperator() const = 0;
    virtual const char		*getVariableOutputName() const = 0;
    virtual const char		*getVariableInputName() const
				 { return "input"; }

    // Return a list of VEX data types that expresses all the allowed
    // data type combinations for this operator. The table should
    // contain sets of LHS, then RHS, then Result data types.
    virtual VOP_Type		*getAllowedTypeCombinations() const = 0;

    /// Return the result type of the operation given the type of the first operant.
    virtual VOP_Type		 getResultType(VOP_Type operand_type) const
				 { return operand_type; }

    virtual OP_ERROR	 setNamedInput(const OP_ConnectorId& input_name, OP_Node *op,
	const OP_ConnectorId* output_name = NULL);
    virtual OP_ERROR	 setNamedInputReference(const OP_ConnectorId& input_name, 
	const char *label, int, const OP_ConnectorId* output_name = NULL);

protected:
    VOP_VariableInput(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~VOP_VariableInput();

    // Code to generate before and after the list:
    virtual void	 getPreCode(UT_String &codestr) const;
    virtual void	 getPostCode(UT_String &codestr) const;

    // Where do our variable inputs start? Index 0 by default.
    virtual int		 getVariableOffset(void) const;

    // Where do our VEX function arguments start? Index 0 by default
    virtual int		 getVexArgOffset(void) const;

    virtual void	 getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		 getInputFromNameSubclass(const UT_String &in) const;
    virtual void	 getInputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						  int idx);

    virtual void	 getOutputNameSubclass(UT_String &out, int idx) const;
    virtual void	 getOutputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						   int idx);

    int			 nConnectedVariableInputs() const;

    virtual void	 getAllowedInputTypesSubclass(unsigned idx,
				    VOP_VopTypeArray &voptypes);

    // These are semi-private.  Only the RSL subtract operator really needs to
    // fchange them.
    virtual void	 computeAllowedTypes(int send_event=1);
    void		 updateAllowedTypes() const;

    VOP_Type		 myAllowedInputTypes[VOP_MAX_TYPES];
    int			 myNumAllowedInputTypes;
    VOP_Type		 myOutputType;
    mutable bool	 myInputInfoDirtyFlag;
};

#endif
