/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_ScriptNode.h ( VOP Library, C++)
 *
 * COMMENTS:	This is the meat & potatoes.  Here we have an operator which is
 *		defined by a script.
 */

#ifndef __VOP_ScriptNode__
#define __VOP_ScriptNode__

#include "VOP_API.h"
#include "VOP_Node.h"

class VOP_ScriptOperator;

class VOP_API VOP_ScriptNode : public VOP_Node {
public:
	     VOP_ScriptNode(OP_Network *dad, const char *name, OP_Operator *op);
    virtual ~VOP_ScriptNode();

    static OP_Node	*myConstructor(OP_Network *net, const char *name,
				       OP_Operator *op);
    static PRM_Template	 myMultiInputSetTemplateList[];

    virtual void	 initMaterialFlag();
    virtual void	 getModulesToImport(UT_StringArray &modules,
				const VOP_CodeGenContext &context);
    virtual void	 getOuterCode(UT_String &codestr,
				const VOP_CodeGenContext &context);
    virtual void	 getCode(UT_String &codestr,
                	         const VOP_CodeGenContext &context);
    virtual bool	 getParmConstantString(const char *parmname,
						const VOP_TypeInfo &type_info,
						UT_String &str,
						bool expand_string,
						const VOP_Language *language=0);

    void		 ensureParametersAreValid();

    virtual const char	*inputLabel(unsigned idx) const;
    virtual const char	*outputLabel(unsigned idx) const;

    virtual int		 getSignatureCount() const;
    virtual void	 getSignatureName(UT_String &name, int index) const;
    virtual void	 setCurrentSignature(const char *name);
    virtual bool	 getCurrentSignatureName(UT_String &name);

    virtual void	 getAllowedInputTypeInfos(unsigned idx,
					      VOP_VopTypeInfoArray &voptypes);

    virtual bool	 isShader() const;
    virtual bool	 isShader(VOP_Type shader_type) const;
    virtual bool	 isCachedShader() const;
    virtual bool	 getCachedShaderCode(std::ostream &os,
                	                    VOP_ContextType context_type) const;
    virtual bool	 forceCodeGenerationOfInputs(
					    const VOP_CodeGenContext &context,
					    bool check_shader_context) const;

    virtual void	 getParmNameFromInput(UT_String &parmname,
					      int inputidx) const;

    static void		 buildInputSetMenu(void *, PRM_Name *, int,
					   const PRM_SpareData *,
					   const PRM_Parm *);
    int			 getCurrentInputSet() const;

    /// Updates the opeartor information about inputs and outputs based on the
    /// current state of the script node.
    /// If 'keep_info' is true, most of the op_info is preserved (eg, input
    /// order, input visibility flags, etc), and this method updates the data 
    /// that can be specifed only by the node itself (and not op type 
    /// properties editor). Otherwise, if false, node inptus will dictate
    /// the saved order and flags.
    void		 updateInputsAndOutputsInfo(VOP_OperatorInfo *op_info,
				bool keep_info = false);
    virtual bool	 getIsInputDefinedByParmChild(int idx) const;
    virtual bool	 getIsOutputDefinedByParmChild(int idx) const;

    virtual void	 opChanged(OP_EventType reason, void *data=0);

protected:
    void		 updateCurrentInputSet();

    virtual bool	 updateParmsFlags();
    
    unsigned		 getNumScriptOpInputs() const;
    const UT_StringHolder &getScriptOpInputName(int idx) const;
    int			 findScriptOpInput(const char *input_name) const;
    virtual void	 getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		 getInputFromNameSubclass(const UT_String &in) const;
    virtual void	 getInputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						  int idx);
    virtual bool	 getIsInputVisibleDefaultSubclass(int idx);
    virtual void	 getAllowedInputTypeInfosSubclass(unsigned idx,
					    VOP_VopTypeInfoArray &voptypes);

    unsigned		 getNumScriptOutputs() const;
    int			 findScriptOpOutput(const UT_StringRef &output_name) const;
    virtual void	 getOutputNameSubclass(UT_String &out, int idx) const;
    virtual void	 getOutputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						   int idx);

    /// Returns true if output of a given index has an input by the same name.
    virtual bool	 hasCorrespondingInput(int output_idx) const;

    virtual void	 getVopFunctionName(UT_String &function_name) const;

private:
    bool		 getBestMatchingParameter(UT_WorkBuffer &parm,
					const char *input, int inputidx) const;
    bool		 getBestMatchingInput(int &input_index, 
					const UT_String &input_name) const;
    virtual bool	 shouldOutputNodeVariableDeclarations() const;

private:
    VOP_ScriptOperator	*myScript;
    int			 myCurrentInputSet;
    UT_IntArray		 myValidInputSets;
    bool		 myUpdatingInputSetFlag;
};

#endif
