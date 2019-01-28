/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 */

#ifndef __VOP_Collect_h__
#define __VOP_Collect_h__

#include "VOP_API.h"
#include "VOP_VariableInput.h"

class UT_StringRef;

class VOP_API VOP_Collect : public VOP_VariableInput
{
protected:
    VOP_Collect(OP_Network *parent, const char *name, OP_Operator *entry);
    virtual ~VOP_Collect();

public:
    static OP_Operator		*createOperator();
    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		 myTemplateList[];

    /// Get nodes that effectively are fed into the collect node. These
    /// include regular (eg, output) nodes, but also regular nodes indirectly
    /// connected through a chain of nulls and switches.
    void	getCollectedNodes(VOP_NodeList &outputs);

    /// Get the types of shaders wired into this collect node.
    void	getCollectedShaderTypes(VOP_ShaderTypeList &types) const;
    
    /// Collect VOP is virtually equivalent to a vo material.
    virtual VOP_Type	getShaderType() const;

protected:
    /// Inherited from VOP_Node.
    virtual void	 initMaterialFlag();

    /// Generate the node errors.
    /// Return true if an error was generated and false otherwise.
    virtual bool	 generateErrorsSubclass();

    /// @{
    /// API from VOP_VariableInput
    virtual const char	*getVariableOperator() const;
    virtual const char	*getVariableOutputName() const;
    virtual const char	*getVariableInputName() const;
    virtual VOP_Type	*getAllowedTypeCombinations() const;
    /// @}

    /// @{ Intercepting input manipulation to notify the parent.
    virtual void	 moveInput(int srcidx, int dstidx, 
				bool forcesubnet = false);
    virtual OP_ERROR	 setInput(unsigned idx, OP_Node *op,
				unsigned outputIdx = 0);
    virtual OP_ERROR	 setInputReference(unsigned idx, const char *label,
				int keeppos, unsigned outputIdx = 0);
    virtual OP_ERROR	 setNamedInput(const OP_ConnectorId& input_id,
				OP_Node *op, 
				const OP_ConnectorId* output_name = nullptr);
    virtual OP_ERROR	 setNamedInputReference(const OP_ConnectorId& input_id, 
				const char *label, int keeppos, 
				const OP_ConnectorId* output_name = nullptr);
    /// @}

    /// @{ In case the collect VOP is assigned as a material.
    virtual VOP_Node	*findShader(VOP_Type shader_type, fpreal now);
    virtual void	 findAllShaders( VOP_NodeList &shader_nodes,
					VOP_ShaderTypeList &shader_types );
    virtual VOP_Node	*getProcedural(VOP_Type type);
    virtual bool	 getParameterOrProperty(
				    const UT_StringRef &name, fpreal now,
				    OP_Node *&op, PRM_Parm *&parm,
				    bool create_missing_multiparms,
				    PRM_ParmList *obsolete = 0);
    virtual bool	 getParameterOrPropertyByChannel(
				    const UT_StringRef &chname,
				    fpreal now, OP_Node *&op, PRM_Parm *&parm,
				    int &vector_index,
				    PRM_ParmList *obsolete = 0);
    /// @}

    /// Returns true if there is more than one input of the same type.
    bool		 hasInputTypeConflicts();

private:
    /// Informs the parent of connector changes.
    void		 notifyParentOfInputChange();
};

#endif
