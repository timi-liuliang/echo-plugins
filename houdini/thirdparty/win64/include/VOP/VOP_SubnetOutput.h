/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 */

#ifndef __VOP_SubnetOutput_h__
#define __VOP_SubnetOutput_h__

#include "VOP_API.h"
#include <UT/UT_StringArray.h>
#include "VOP_Node.h"

class VOP_API VOP_SubnetOutput : public VOP_Node, public VOP_OutputNameEditorSource
{
public:
    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		 myTemplateList[];

    virtual void		 getCode(UT_String &codestr,
                		         const VOP_CodeGenContext &context);

    virtual const char		*inputLabel(unsigned idx) const;

    virtual unsigned		 getNumVisibleInputs() const;

    virtual bool		 forceCodeGenerationOfInputs(
					const VOP_CodeGenContext &context,
					bool check_shader_context) const;

    // For suboutput VOPs, moving an input means we should also rewire the
    // outputs from our containing subnet. We also watch for setInput calls
    // to rearrange our parent's outputs when inputs are connected or
    // disconnected from us.
    virtual void		 moveInput(int srcidx, int dstidx,
					   bool forcesubnet = false);
    // Do not call this method yourself. It performs the core work
    // necessary to move inputs on this subnet output node, but does not
    // fully account for the synchonizing it with the rest of the
    // nodes.
    void			 moveInputInternal(int srcidx, int dstidx, 
					   bool forcesubnet);
    virtual OP_ERROR		 setInput(unsigned idx, OP_Node *op,
					  unsigned outputIdx = 0);
    virtual OP_ERROR		 setInputReference(unsigned idx,
						   const char *label,
						   int keeppos,
						   unsigned outputIdx = 0);

    virtual OP_ERROR	 setNamedInput(const OP_ConnectorId& input_name, OP_Node *op,
	const OP_ConnectorId* output_name = NULL);
    virtual OP_ERROR	 setNamedInputReference(const OP_ConnectorId& input_name, 
	const char *label, int, const OP_ConnectorId* output_name = NULL);


    // Returns a pointer used by OPUI_OutputNameEditor to get and set data.
    virtual VOP_OutputNameEditorSource	*getOutputNameEditorSource()
				 { return this; }

    // Provide access to the output name and label parameter functions so
    // the OPUI_OutputNameEditor can deal with them appropriately.
    virtual int			 getNumInputsFromParent() const;
    virtual void		 NAMEFROMPARM(UT_String &str, int idx) const;
    virtual void		 LABELFROMPARM(UT_String &str, int idx) const;
    virtual void		 setNAMEFROMPARM(UT_String &str, int idx);
    virtual void		 setLABELFROMPARM(UT_String &str, int idx);

    void			 beginWiring();
    void			 endWiring();
    void			 clearCachedInputNames();

protected:
    VOP_SubnetOutput(OP_Network *parent, const char *name, OP_Operator *entry);
    virtual ~VOP_SubnetOutput();

    virtual void	 preOpChanged(OP_EventType reason, void *data);

    virtual void	 getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		 getInputFromNameSubclass(const UT_String &in) const;
    virtual void	 getInputTypeInfoSubclass(VOP_TypeInfo &type_info, 
					    int idx);
    virtual void	 getAllowedInputTypeInfosSubclass(unsigned idx,
					    VOP_VopTypeInfoArray &voptypes);

private:
    void		 adjustParentForInputChange(int idx, bool inputnull);
    void		 getInternalInputName(UT_String &in, int idx) const;
    void		 findSafeBase(UT_String &in) const;
    int			 mapToParentInputIndex(int index) const;

    UT_StringArray	 myInputNames;
    int			 myWiringDepth;
};

#endif
