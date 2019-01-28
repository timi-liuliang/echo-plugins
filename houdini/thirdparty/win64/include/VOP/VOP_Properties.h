/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_Properties.h (VOP  Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VOP_Properties__
#define __VOP_Properties__

#include "VOP_API.h"
#include "VOP_Node.h"
#include "VOP_OperatorInfo.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_SymbolTable.h>

#define VOP_PROPERTIES_NODE_NAME	"properties"

class VOP_API VOP_Properties : public VOP_Node 
{
public:
    /// Static factory function that generates VOP_Properties objects.
    static OP_Node *		myConstructor( OP_Network * net,
					       const char * name,
					       OP_Operator * entry);

    /// The list of parameters for VOP_Properties nodes.
    static PRM_Template		myTemplateList[];

    virtual unsigned		 orderedInputs() const;
    virtual unsigned		 getNumVisibleInputs() const;
    virtual const char		*inputLabel(unsigned idx) const;

    virtual unsigned		 getNumVisibleOutputs() const;
    virtual const char		*outputLabel(unsigned idx) const;


    /// Whenever the spare parameteres change, we need to update
    /// our input list.
    virtual void		 opChanged(OP_EventType reason, void *data=0);

protected:
    /// Constructor.
				 VOP_Properties( OP_Network * parent, 
						 const char * name, 
						 OP_Operator * entry );
    /// Destructor.
    virtual			~VOP_Properties();

    virtual bool		 updateParmsFlags();
    
    /// Update the internal map of parameter inputs
    /// based on the node's spare parameters.
    void			 updateParmInputs();

    /// Return the VOP data input type that should be used 
    /// for the given parameter template.
    VOP_Type			 getInputTypeByParmTemplate(
				    const PRM_Template *tplate) const;

    virtual void		 getInputNameSubclass(
				    UT_String &in, int idx) const;
    virtual int			 getInputFromNameSubclass(
				    const UT_String &in) const;
    virtual void		 getInputTypeInfoSubclass(
				    VOP_TypeInfo &type_info, int idx);

    virtual void		 getOutputNameSubclass(
				    UT_String &out, int idx) const;
    virtual void		 getOutputTypeInfoSubclass(
				    VOP_TypeInfo &type_info, int idx);

    virtual bool		 getRequiresInputParameters() const;

private:
    
    /// This structure contains the parameter inputs that were
    /// derived from the node's spare parameters.
    UT_ValArray<VOP_InOutInfo *>     myParmInputs;
};

#endif

