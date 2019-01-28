/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 *	    Note: This is more of an internal class, not  an actual node. It needs to inherit 
 *	    from VOP_Node to generate code properly, though. What happens is during code 
 *	    generation, copies of it are inserted into the flattened node list, and (before
 *	    they are inserted) are told which two nodes and which input/output they go between.
 *	    Thus, these are essentially virtual nodes that get created only at code generation 
 *	    time and destroyed immediately afterwards.
 *	    
 *	    During code generation, they pretend they really are connected between the two nodes,
 *	    and generate proper auto-conversion code. Auto convert also contains severa static 
 *	    functions that determine whether two types can be auto-converted between.
 *
 */

#ifndef __VOP_AutoConvert_h__
#define __VOP_AutoConvert_h__

#include "VOP_API.h"
#include "VOP_Node.h"
#include <string>

// #define DEBUG_DISABLE_AUTOCONVERT

class VOP_API VOP_AutoConvert : public VOP_Node
{
public:
    VOP_AutoConvert(OP_Network *net);
    virtual ~VOP_AutoConvert();

    /// Returns true if intype can be converted to outtype.
    static bool		canAutoconvert(VOP_Type intype, VOP_Type outtype,
				       const VOP_Language *language);
    static bool		canAutoconvert(const VOP_TypeInfo &intype, 
				       const VOP_TypeInfo &outtype,
				       const VOP_Language *language);

    // Since this "node" is meant to be used on its own, we override this
    // to let VEX code be generated properly.
    virtual void	getPathWithSubnet(UT_String &str) const;
    virtual void	getOuterCode(UT_String &codestr,
                	             const VOP_CodeGenContext &context);
    virtual void	getCode(UT_String &codestr,
                	        const VOP_CodeGenContext &context);

    virtual OP_Network		*getParentNetwork()  const;

    virtual OP_Input	*getInputReference(unsigned idx, bool grow);
    virtual OP_Input	*getInputReferenceConst(unsigned idx) const;

    void		hookupBetween(VOP_Node* input_node, VOP_Node* output_node, 
				    int input_idx, bool tell_nodes);

    virtual bool	 isConnected(int inputidx, bool recursive);
    virtual unsigned	 getNumVisibleInputs() const;
    virtual unsigned		 getNumVisibleOutputs() const;
    virtual const char		*inputLabel(unsigned idx) const;
    virtual const char		*outputLabel(unsigned idx) const;

    static void initConversions(void);

    static const char*	getAutoConvertOpName(const VOP_TypeInfo &from_type, 
					     const VOP_TypeInfo &to_type);
    static const char*	getAutoConvertOpName(VOP_Type from_type, VOP_Type to_type);
    static VOP_Type	generateCodeFor(VOP_Node *input_node, 
				VOP_Node *output_node, 
				int input_idx, UT_String &codestr,
				bool current_types_only);

    virtual bool	 isAutoConvertNode() const
			    { return true; }

protected:

    virtual void	 getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		 getInputFromNameSubclass(const UT_String &in) const;
    virtual void	 getInputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						  int idx);

    virtual void	 getOutputNameSubclass(UT_String &out, int idx) const;
    virtual void	 getOutputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						   int idx);

    static void		 getConversionBetween(VOP_Type intype, VOP_Type outtype,
				std::string& out, const VOP_Language *language);

private:


    // Since autoconversion nodes are only created when generating
    // code, we can just keep the pointers. Also, we can't use setInput(), etc.
    // because that will modify the actual node graph, and disconnect previous nodes.
    // We don't want to do that.
    VOP_Node *myInput, *myOutput;
    int myInputIdx; // input index on the node we output to

    // From-to-type.
    static std::string theConversionTable[VOP_MAX_TYPES][VOP_MAX_TYPES];
    static bool	  theDidInitConversions;
    static std::string theConversionNodeNameTable[VOP_MAX_TYPES][VOP_MAX_TYPES];

    VOP_TypeInfo    myConvertedOutputTypeInfo;
};

#endif
