/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DAE_OPUtils.h (COLLADA Library, C++)
 *
 * COMMENTS:	OP utility library.
 * 
 */

#ifndef __DAE_OPUtil_h__
#define __DAE_OPUtil_h__

#include <SYS/SYS_Types.h>
#include <UT/UT_String.h>
#include <CH/CH_ExprLanguage.h>
#include <OP/OP_Layout.h>
#include "DAE_API.h"

class CH_Channel;
class OP_SubnetIndirectInput;
class OP_Network;
class OP_Node;

/// Static class.
/// The OP utility library.  This is a wrapper for low-level
/// functionality dealing with objects from the Houdini network.
class DAE_API DAE_OPUtil
{
public:

    /// Passes back a suggested OP label for the given name and id.
    static void				 getOPLabel(const UT_String &name,
						    const UT_String &id,
						    UT_String &label);
    
    /// Uses the path to find a node in the Houdini network.
    /// Returns a pointer to the OP network if the node is a subnet.
    /// Returns NULL otherwise.
    static OP_Network			*findOPNetwork(const char *path);

    /// Creates an OP node of the given type, under the given
    /// parent network.  If successful, returns a pointer to the new OP_Node,
    /// otherwise, it returns NULL.
    static OP_Node			*createOPNode(OP_Network *parent,
						      const char *label,
						      const char *type, 
						      bool runInit=1);
    
    /// Wires the first operator into the second operator.
    /// The connection is made to the input corresponding to 'inputIndex'.
    static void				 wireOPNodes(OP_Node *outputNode, 
						     OP_Node *inputNode, 
						     int inputIndex);

    /// Wires the indirect input node into the operator.
    /// The connection is made to the input corresponding to 'inputIndex'.
    static void				 wireOPNodes(
					     OP_SubnetIndirectInput *outputNode,
					     OP_Node *inputNode,
					     int inputIndex);
    
    /// Organizes the node layout in the given network.
    static void				 organizeOPLayout(OP_Network *network, OP_LayoutDirection layout_dir = OP_LAYOUT_TOP_TO_BOT);

    /// Passes back the OP parameter's value (1 integer).
    static void				 getOPParmValue(OP_Node *node, 
							const char *parmName, 
							int &ivalue,
							int index=0);

    /// Passes back the OP parameter's value (1 float).
    static void				 getOPParmValue(OP_Node *node, 
							const char *parmName, 
							fpreal &fvalue,
							int index=0);

    /// Sets the OP parameter's value (1 integer).
    static void				 setOPParm(OP_Node *node,
						   const char *parmName, 
						   int ivalue,
						   int index=0);

    /// Sets the OP parameter's value (1 float).
    static void				 setOPParm(OP_Node *node,
						   const char *parmName, 
						   fpreal fvalue,
						   int index=0);

    /// Sets the OP parameter's value (1 string).
    static void				 setOPParm(OP_Node *node,
						   const char *parmName, 
						   const char * strvalue,
						   CH_StringMeaning meaning,
						   int index=0);

    /// Sets the OP parameter's value (3 floats).
    static void				 setOPParm(OP_Node *node,
						   const char *parmName, 
						   fpreal fvalue1,
						   fpreal fvalue2, 
						   fpreal fvalue3);
    /// Sets the OP parameter's value (3 strings).
    //
    static void				 setOPParm(OP_Node *node, 
						   const char *parmName, 
						   const char *svalue1, 
						   const char *svalue2, 
						   const char *svalue3,
						   CH_StringMeaning meaning);

    /// Adds a spare float parameter to the given node.
    static void				 addOPSpareFloatParm
					    (OP_Node *node, 
					     const char *token, 
					     const char *label, 
					     fpreal defaultValue,
					     fpreal minValue,
					     fpreal maxValue,
					     bool cook_dependent = false);

    /// Adds a spare string parameter to the given node.
    static void				 addOPSpareStringParm
					    (OP_Node *node, 
					     const char *token, 
					     const char *label, 
					     const char *defaultValue,
					     bool cook_dependent = false);

    /// Adds a spare 3-float parameter to the given node.
    static void				 addOPSpare3FloatParm
					    (OP_Node *node, 
					     const char *token, 
					     const char *label, 
					     fpreal defaultValue1,
					     fpreal defaultValue2,
					     fpreal defaultValue3,
					     bool cook_dependent = false);

    /// Adds a property to the given node.
    static void				 addOPProperty
					    (OP_Node *node,
					     const char *propClass,
					     const char *propName);
    
    /// Sets a keyframe on the given channel using the
    /// passed-in attributes.
    static void				 keyOPChannel(CH_Channel *ch, 
						      fpreal time, 
						      fpreal fvalue,
						      bool hasInSlope=false,
						      bool hasOutSlope=false,
						      bool hasInterp=false,
						      fpreal inSlope=0.0,
						      fpreal outSlope=0.0,
						      const char *interp="");

    /// Returns the channel index that corresponds to the
    /// given symbolic name.  The mapping of names to indices
    /// is specified in "Chapter 4: The COMMON Profile - Common Glossary"
    /// of the COLLADA specification document.
    static int			         getCHIndexByName(const char *name);

    /// Returns the CH key expression that corresponds to the given
    /// symbol name.  The possible names are specified in
    /// "Chapter 3: Schema Reference - <sampler>"
    ///  of the COLLADA specificaion document.
    static const char			*getCHKeyExprByName(const char *name);
    
protected:

    /// Constructor.
    DAE_OPUtil();

    /// Destructor.
    virtual ~DAE_OPUtil();
};


#endif
