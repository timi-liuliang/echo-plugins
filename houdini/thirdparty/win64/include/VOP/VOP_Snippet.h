/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 */

#ifndef __VOP_Snippet_h__
#define __VOP_Snippet_h__

#include "VOP_API.h"
#include "VOP_Node.h"

class VOP_API VOP_Snippet : public VOP_Node
{
public:
    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		*buildTemplates();


    static UT_StringHolder	 buildOuterCode(
				    const char *returntype,
				    const char *funcname, 
				    const char *boundprefix,
				    const char *snippet, 
				    const char *inputlist, 
				    const char *exportlist, 
				    bool toplevel,
				    bool strictvariables, 
				    bool vexstrict, 
				    bool linehints, 
				    const char *linefile, 
				    const VOP_Language *language, 
				    const VOP_GlobalVarMap *globalvariables);

    virtual void		 getOuterCode(UT_String &codestr,
                		              const VOP_CodeGenContext &context);

    virtual void		 getCode(UT_String &codestr,
                		         const VOP_CodeGenContext &context);

    virtual bool		 definesShaderExtraParameters() const
				 { return true; }
    virtual int			 getShaderExtraParameters(
					 UT_Array<VOP_Type> &types,
					 UT_IntArray &doexport,
					 UT_StringArray &names,
					 UT_StringArray &def,
					 VOP_Type shader_type = VOP_TYPE_UNDEF )
						const;

    virtual const char		*inputLabel(unsigned idx) const;
    virtual const char		*outputLabel(unsigned idx) const;

    virtual unsigned		 getNumVisibleInputs() const;
    virtual unsigned		 getNumVisibleOutputs() const;

    virtual bool		 forceCodeGenerationOfInputs(
					const VOP_CodeGenContext &context,
					bool check_shader_context) const;

    virtual void	 postOpChanged(OP_EventType reason, void *data);

    /// The following are used to rewire our inputs so things
    /// remain kosher.
    virtual void		 moveInput(int srcidx, int dstidx,
					   bool forcesubnet = false);

    virtual OP_ERROR		 setInput(unsigned idx, OP_Node *op,
					  unsigned outputidx = 0);
    virtual OP_ERROR		 setInputReference(unsigned idx, 
						   const char *label,
						   int keeppos,
						   unsigned outputidx = 0);

    virtual OP_ERROR	 setNamedInput(const OP_ConnectorId&  input_name, OP_Node *op,
	const OP_ConnectorId* output_name = NULL);
    virtual OP_ERROR	 setNamedInputReference(const OP_ConnectorId&  input_name, 
			const char *label, int, const OP_ConnectorId* output_name = NULL);


    virtual VOP_Node *		 findSimpleInputFromOutput(int index);
    virtual void		 findSimpleInputCandidatesFromOutput( 
					    int output_index,
					    UT_IntArray & input_indices );

protected:
    VOP_Snippet(OP_Network *parent, const char *name, OP_Operator *entry);
    virtual ~VOP_Snippet();

    virtual bool	 updateParmsFlags();
    virtual void	 getDescriptiveParmName(UT_String &name) const
			 { name = "code"; }
    
    virtual void	 preOpChanged(OP_EventType reason, void *data);

    virtual void	 getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		 getInputFromNameSubclass(const UT_String &in) const;
    virtual void	 getInputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						  int idx);

    virtual void	 getOutputNameSubclass(UT_String &out, int idx) const;
    virtual void	 getOutputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						   int idx);

    void		 adjustOutputParmsForInputChange(int idx,
					    bool inputnull);
    void		 adjustOutputParmsForNamedInputChange(
					    const OP_ConnectorId& input_name,
					    bool inputnull);
    void		 swapInputs(int srcidx, int dstidx);

    virtual void	 addDiagnosticInfo(const VCC_DiagnosticInfo &diag);

    int			 getShaderExtraParametersHelper(
					 UT_Array<VOP_Type> &types,
					 UT_IntArray &doexport,
					 UT_StringArray &names,
					 UT_StringArray &raw_names,
					 UT_StringArray &def,
					 VOP_Type shader_type = VOP_TYPE_UNDEF )
						const;
private:
    void		 getBaseOutputName(UT_String &out, int idx) const;
    int			 NUMPARMS() const;
    bool		 VEX_STRICT() const;
    bool		 FORCECODEGENERATION() const;
    bool		 STRICTVARIABLES() const;
    void		 OUTPUTNAME(UT_String &str, int idx, bool expand) const;
    CH_StringMeaning	 OUTPUTNAMEmeaning(int idx) const;
    void		 setOutputParm(UT_String &str, CH_StringMeaning meaning,
				       int idx);
    void		 OUTPUTLABEL(UT_String &str, int idx) const;
    void		 EXPORTLIST(UT_String &str) const;
    void		 getFunctionName(UT_String &fname) const;

    void		 evaluateCode(UT_String &codestr) const;
    void		 cppStripCode(UT_String &codestr) const;
    bool		 adjustColumnForBindings(const char *l, int &col) const;

    bool		 mySearchingForInputs;
};

#endif
