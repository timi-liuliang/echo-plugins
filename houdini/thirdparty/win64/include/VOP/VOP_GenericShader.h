/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	( VOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VOP_GenericShader__
#define __VOP_GenericShader__

#include "VOP_API.h"
#include "VOP_Node.h"

class VOP_API VOP_GenericShader : public VOP_Node
{
public:
    /// Operator factory.
    static OP_Operator		*createOperator();

    void			SHADERNAME( UT_String &name ) const;
    void			setSHADERNAME( const char *name ); 

protected:
    /// @{ Standard constructor and destructor for vop nodes.
    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    VOP_GenericShader(OP_Network *parent, const char *nm, OP_Operator *op);
    virtual ~VOP_GenericShader();
    /// @}
   

    /// @{ Virtuals related to shader nodes.
    virtual bool		    isShader() const;
    virtual bool		    isShader(VOP_Type shader_type) const;
    virtual const PRM_Template *    getShaderParmTemplates();
    virtual void		    getVopFunctionName(
					UT_String &function_name) const;
    virtual void		    opChanged(OP_EventType t, void *data=0);
    virtual bool		    updateParmsFlags();
    /// @}

    /// @{ Standard vop virtuals for inputs.
    virtual unsigned	getNumVisibleInputs() const;
    virtual unsigned	orderedInputs() const;
    virtual void	getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		getInputFromNameSubclass(const UT_String &in) const;
    virtual void	getInputTypeInfoSubclass(VOP_TypeInfo &type_info, 
					    int idx);
    /// @}
   
    /// @{ Standard vop virtuals for outputs.
    virtual unsigned	getNumVisibleOutputs() const;
    virtual void	getOutputNameSubclass(UT_String &out, int idx) const;
    virtual void	getOutputTypeInfoSubclass(VOP_TypeInfo &type_info,
						   int idx);
    /// @}

    /// @{ Other vop virtuals
    virtual void	getCode(UT_String &codestr,
                	        const VOP_CodeGenContext &context);
    /// @}

private:
    // Info about inputs and outputs.
    class InOutData;
    const InOutData *	getInOutData() const;

private:
    // Info about inputs and outputs.
    mutable UT_UniquePtr<InOutData>	myInOutData;
};

#endif

