/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 */

#ifndef __VOP_OutputVar_h__
#define __VOP_OutputVar_h__

#include "VOP_API.h"
#include "VOP_Node.h"

class VOP_API VOP_OutputVar : public VOP_Node
{
public:
    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		 myTemplateList[];

    virtual void		 initializeNode();
    virtual void		 initMaterialFlag();

    virtual bool		 isOutputVopNode() const;
    virtual void		 getCode(UT_String &codestr,
                		         const VOP_CodeGenContext &context);

    virtual const VOP_OutputVar *castToOutputVar() const { return this; }
    virtual VOP_OutputVar *castToOutputVar() { return this; }

    virtual void		 ensureParametersAreValid();

    virtual const char		*inputLabel(unsigned idx) const;

    virtual unsigned		 maxInputs() const;

    virtual bool		 forceCodeGenerationOfInputs(
					const VOP_CodeGenContext &context,
					bool check_shader_context) const;
    virtual void		 getContextsForCodeGeneration(
					UT_StringArray & context_names ) const;

    void			 setOUTPUTCODELAST(bool flag);
    bool			 OUTPUTCODELAST();

    /// Gets the concrete context type this node belongs to.
    virtual VOP_ContextType	 getSpecificShaderContextType() const;

    /// Obtains the list of global variables that make up the inputs.
    const VOP_GlobalVarList	&getGlobalVarList() const
				 { return myGlobalVarList; }

    virtual VOP_Node *		 findSimpleInputFromOutput(int output_index);
    virtual void		 findSimpleInputCandidatesFromOutput( 
					    int output_index,
					    UT_IntArray & input_indices );

    virtual bool	 isVopMethod() const;
    virtual bool	 isVopMethodPublic() const;
    virtual void	 getVopFunctionName(UT_String &function_name) const;
    virtual void	 getVopFunctionReturnInfo(VOP_FunctionArgInfo &i) const;
    virtual void	 getVopFunctionArgInfos(
				UT_Array<VOP_FunctionArgInfo> &arg_infos);

    /// Sets the node to use the given shader context type.
    void		 setSHADERTYPE(VOP_Type shader_type);

protected:
    VOP_OutputVar(OP_Network *parent, const char *name, OP_Operator *entry);
    virtual ~VOP_OutputVar();

    virtual bool	 updateParmsFlags();
    
    virtual void	 preOpChanged(OP_EventType reason, void *data);
    virtual void	 getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		 getInputFromNameSubclass(const UT_String &in) const;
    virtual void	 getInputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						int idx);
    virtual void	 getAllowedInputTypesSubclass(unsigned idx, 
						VOP_VopTypeArray &voptypes);
    void		 refreshGlobalVariables();

    virtual unsigned	 getNumVisibleOutputs() const;
    virtual void	 getOutputNameSubclass(UT_String &out, int idx) const;
    virtual void	 getOutputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						   int idx);

private:
    virtual void	 loadFinished();
    int			 getLatestVopVersion() const;
    bool		 usingShaderType() const;
    VOP_ContextType	 SHADERCONTEXTTYPE() const;
    VOP_Type		 SHADERTYPE() const;

    VOP_GlobalVarList		 myGlobalVarList;
    VOP_ContextType		 myShaderContextType;
    VOP_Type			 myShaderType;
    bool			 myIsSearchingForInputs;
};

#endif
