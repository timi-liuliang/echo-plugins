 /*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __VOP_Error__
#define __VOP_Error__

enum VOP_ErrorCodes
{
    VOP_MESSAGE = 0,

    VOP_BAD_INPUT,		// input type doesn't match output type.
    VOP_SUBNET_BAD_VOP,		// bad child vop %s.
    VOP_TIMEDEPENDENT_CHANNEL,	// cannot have channels that depend on time.
    VOP_PARAMETER_NAME_CONFLICT,// Parameter name '%s' is a reserved word.
    VOP_INVALID_CONDITIONAL,	// invalid conditional string in a parm VOP.
    VOP_BAD_STORAGE,		// Invalid uniform/varying storage
    VOP_INVALID_SPLINE,		// Invalid spline type specified for language
    VOP_RECURSION,		// Recursive loop in VOP Network
    VOP_BAD_VOP,		// bad vop %s.
    VOP_PROBLEMS_COMPILING,	// Errors or Warnings during VEX compile.
    VOP_NOT_PARAMETER_VOP,	// '%s' input not a Parameter VOP
    VOP_INPUT_TYPE_COLLISION,	// more than one input of the same type
    VOP_STRUCT_DEF_CONFLICT,	// definition conflicts with inputs or outputs
    VOP_NO_STRUCT_INPUT,	// Input wire type is not a struct.
    VOP_ANY_STRUCT_INPUT,	// Input wire type is an unresolved struct.
    VOP_NO_STRUCT_DEF,		// Cannot obtain struct type definition.
    VOP_NO_STRUCT_MEMBER,	// Input struct doesn't have member %s to unpack
    VOP_BAD_STRUCT_MEMBER_TYPE,	// Incompatible type of struct member '%s'.
    VOP_NO_SWITCH_INPUTS,	// No switch inputs
    VOP_PARAMETER_NAME_GLOBAL,	// Parameter name '%s' is a global variable.
};

#endif
