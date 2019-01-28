/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 */

#ifndef __VOP_Null_h__
#define __VOP_Null_h__

#include "VOP_API.h"
#include "VOP_Node.h"

class VOP_API VOP_Null : public VOP_Node
{
public:
    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		 myTemplateList[];

    virtual void		 getCode(UT_String &codestr,
                		         const VOP_CodeGenContext &context);

    virtual const char		*inputLabel(unsigned idx) const;
    virtual const char		*outputLabel(unsigned idx) const;

    virtual unsigned		 getNumVisibleInputs() const;
    virtual unsigned		 getNumVisibleOutputs() const;
    virtual unsigned		 orderedInputs() const;

    virtual bool		 forceCodeGenerationOfInputs(
					const VOP_CodeGenContext &context,
					bool check_shader_context) const;


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
    VOP_Null(OP_Network *parent, const char *name, OP_Operator *entry);
    virtual ~VOP_Null();

    virtual void	 postOpChanged(OP_EventType reason, void *data);

    virtual void	 getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		 getInputFromNameSubclass(const UT_String &in) const;
    virtual void	 getInputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						  int idx);

    virtual void	 getOutputNameSubclass(UT_String &out, int idx) const;
    virtual void	 getOutputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						   int idx);
    virtual UT_IntArray	    getShaderLayerExportsInputsToCopy(int out_idx,
				    VOP_Type shader_type);
    virtual UT_StringHolder getShaderLayerExportsToAdd(int out_idx,
				    VOP_Type shader_type);

    void		 adjustOutputParmsForInputChange(int idx,
					    bool inputnull);
    void		 adjustOutputParmsForNamedInputChange(
					    const OP_ConnectorId& input_name,
					    bool inputnull);
    void		 swapInputs(int srcidx, int dstidx);

    virtual void	 getTentativeOutputName(UT_String &out, int idx) const;

    virtual int		 NUMPARMS() const;
    virtual void	 OUTPUTNAME(UT_String &str, int idx, bool expand) const;
    virtual CH_StringMeaning	 OUTPUTNAMEmeaning(int idx) const;
    virtual void	 setOutputParm(UT_String &str, CH_StringMeaning meaning,
				       int idx);
    virtual void	 OUTPUTLABEL(UT_String &str, int idx) const;

private:

    bool		 mySearchingForInputs;
};

#endif
