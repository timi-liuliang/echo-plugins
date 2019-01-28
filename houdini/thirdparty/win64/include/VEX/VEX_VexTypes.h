/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VEX_VexTypes.h ( VEX Library, C++)
 *
 * COMMENTS:	Data types for VEX variables
 */

#ifndef __VEX_VexTypes__
#define __VEX_VexTypes__

#include "VEX_API.h"
#include <UT/UT_Array.h>

/// VEX variable types.
enum VEX_Type
{
    VEX_TYPE_UNDEF,		// Unknown type

    VEX_TYPE_VECTOR4,		// Four scalars
    VEX_TYPE_VECTOR,		// Three scalars
    VEX_TYPE_FLOAT,		// A single scalar value
    VEX_TYPE_INTEGER,		// A single integer value
    VEX_TYPE_STRING,		// A character string
    VEX_TYPE_MATRIX3,		// A 3x3 (rotation) matrix
    VEX_TYPE_MATRIX4,		// A 4x4 (transform) matrix
    VEX_TYPE_BSDF,		// A BSDF
    VEX_TYPE_VECTOR2,		// Two scalars
    VEX_TYPE_MATRIX2,		// A 2x2 matrix.

    VEX_MAX_TYPES		// Sentinel

};

enum VEX_ContextTypeEnum
{
    VEX_SURFACE_CONTEXT_ENUM,		// Surface shading context
    VEX_DISPLACE_CONTEXT_ENUM,		// Displacement shading context
    VEX_LIGHT_CONTEXT_ENUM,		// Light shading context
    VEX_SHADOW_CONTEXT_ENUM,		// Shadow shading context
    VEX_FOG_CONTEXT_ENUM,		// Fog shading context
    VEX_CHOP_CONTEXT_ENUM,		// CHOP operator context
    VEX_POP_CONTEXT_ENUM,		// POP operator context
    VEX_SOP_CONTEXT_ENUM,		// SOP operator context
    VEX_COP2_CONTEXT_ENUM,		// COP operator context
    VEX_I3D_CONTEXT_ENUM,		// Image3D context
    VEX_CVEX_CONTEXT_ENUM,		// CVEX context

    VEX_MAX_CONTEXT_ENUM
};
SYS_STATIC_ASSERT(VEX_MAX_CONTEXT_ENUM <= 32);

/// VEX is used in various contexts.  Each context has its own set of functions
/// and variables available.  The enum::VEX_ContextType can be used to limit
/// the VEX operator to a specific set of contexts.  Contexts can be or'ed
/// together.
#define ENUM_TO_MASK(TOKEN)	TOKEN = (1 << TOKEN##_ENUM)
enum VEX_ContextType
{
    VEX_INVALID_CONTEXT = 0,
    ENUM_TO_MASK(VEX_SURFACE_CONTEXT),
    ENUM_TO_MASK(VEX_DISPLACE_CONTEXT),
    ENUM_TO_MASK(VEX_LIGHT_CONTEXT),
    ENUM_TO_MASK(VEX_SHADOW_CONTEXT),
    ENUM_TO_MASK(VEX_FOG_CONTEXT),
    ENUM_TO_MASK(VEX_CHOP_CONTEXT),
    ENUM_TO_MASK(VEX_POP_CONTEXT),
    ENUM_TO_MASK(VEX_SOP_CONTEXT),
    ENUM_TO_MASK(VEX_COP2_CONTEXT),
    ENUM_TO_MASK(VEX_I3D_CONTEXT),
    ENUM_TO_MASK(VEX_CVEX_CONTEXT)
};
#undef CONTEXT_MASK

using VEX_ContextTypeList = UT_Array<VEX_ContextType>;

/// @def VEX_SHADING_CONTEXT Shading Contexts for mantra
#define VEX_SHADING_CONTEXT	(VEX_SURFACE_CONTEXT | \
				 VEX_DISPLACE_CONTEXT | \
				 VEX_LIGHT_CONTEXT | \
				 VEX_SHADOW_CONTEXT | \
				 VEX_FOG_CONTEXT)

/// @def VEX_OP_CONTEXT OP Contexts
#define VEX_OP_CONTEXT		(VEX_CHOP_CONTEXT | \
				 VEX_POP_CONTEXT | \
				 VEX_SOP_CONTEXT | \
				 VEX_COP2_CONTEXT)

/// @def VEX_ALL_CONTEXT All contexts (shading, operator, i3d and CVEX)
#define VEX_ALL_CONTEXT		(VEX_SHADING_CONTEXT | \
				 VEX_OP_CONTEXT | \
				 VEX_I3D_CONTEXT | \
				 VEX_CVEX_CONTEXT)

enum VEX_CodeGenFlagsEnum
{
    /// Don't write out #pragmas
    VEX_CG_OMIT_PRAGMAS_ENUM,

    /// Add line hints for determining the generator node of each code section.
    VEX_CG_ADD_NODE_LINE_HINTS_ENUM,

    /// Don't add extra comments to the code (header, origin markers, etc.)
    VEX_CG_OMIT_COMMENTS_ENUM,

    /// Generate shader calls for encapsulated shaders. The encapsulated shaders
    /// are not a part of the resulting code from VEX_VexResolver::getVflCode.
    /// Instead, call VEX_VexResolver::getDependencies to get the list of
    /// shaders that will be called.
    /// If this flag is not used, the encapsulated code will be embedded in
    /// the resulting code and a direct function call used instead.
    VEX_CG_CALL_ENCAPSULATED_SHADERS_ENUM,

    /// Don't output empty boilerplate code if the real code generation failed.
    VEX_CG_NO_EMPTY_CODE_ENUM,

    /// Don't add any header comment to the code.
    VEX_CG_OMIT_HEADER_COMMENT_ENUM,

    /// Don't add any shader #defines to the code.
    VEX_CG_OMIT_SHADER_DEFINES_ENUM,
    
    /// Ignore any errors in the VOP network when generating the code.
    VEX_CG_IGNORE_ERRORS_ENUM,
    
    /// Don't check if the imported shader actually exists.
    VEX_CG_NO_SHADER_IMPORT_CHECK_ENUM,

    VEX_CG_MAX_ENUMS
};
SYS_STATIC_ASSERT(VEX_CG_MAX_ENUMS <= 32);

/// VEX code generation flags when calling VEX_VexResolver::getVflCode
enum VEX_CodeGenFlags
{
    VEX_CG_DEFAULT = 0,

    ENUM_TO_MASK(VEX_CG_OMIT_PRAGMAS),
    ENUM_TO_MASK(VEX_CG_ADD_NODE_LINE_HINTS),
    ENUM_TO_MASK(VEX_CG_OMIT_COMMENTS),
    ENUM_TO_MASK(VEX_CG_CALL_ENCAPSULATED_SHADERS),
    ENUM_TO_MASK(VEX_CG_NO_EMPTY_CODE),
    ENUM_TO_MASK(VEX_CG_OMIT_HEADER_COMMENT),
    ENUM_TO_MASK(VEX_CG_OMIT_SHADER_DEFINES),
    ENUM_TO_MASK(VEX_CG_IGNORE_ERRORS),
    ENUM_TO_MASK(VEX_CG_NO_SHADER_IMPORT_CHECK),
};

/// Convenience operator to merge two VEX_CodeGenFlags enum values into one.
static inline VEX_CodeGenFlags
operator|(VEX_CodeGenFlags a, VEX_CodeGenFlags b)
{
    return VEX_CodeGenFlags(uint(a)|uint(b));
}

enum VEX_SpecializeFlagsEnum
{
    VEX_SPECIALIZE_SPECIALIZING_ENUM,		// Specializing code
    VEX_SPECIALIZE_SHADOW_ENUM,			// Specialize for shadow shader
    VEX_SPECIALIZE_DISABLE_JIT_ENUM,		// Disable JIT
    VEX_SPECIALIZE_DISABLE_JUMP_THREADING_ENUM,	// Disable jump threading
    VEX_SPECIALIZE_ENABLE_LOOP_UNSWITCH_ENUM,	// Enable loop unswitching
    VEX_SPECIALIZE_LPE_ENUM,			// Specialize for LPE

    VEX_SPECIALIZE_MAX_ENUMS
};
SYS_STATIC_ASSERT(VEX_SPECIALIZE_MAX_ENUMS <= 32);

enum VEX_SpecializeFlags
{
    VEX_SPECIALIZE_NONE = 0,
    ENUM_TO_MASK(VEX_SPECIALIZE_SPECIALIZING),
    ENUM_TO_MASK(VEX_SPECIALIZE_SHADOW),
    ENUM_TO_MASK(VEX_SPECIALIZE_DISABLE_JIT),
    ENUM_TO_MASK(VEX_SPECIALIZE_DISABLE_JUMP_THREADING),
    ENUM_TO_MASK(VEX_SPECIALIZE_ENABLE_LOOP_UNSWITCH),
    ENUM_TO_MASK(VEX_SPECIALIZE_LPE),
};
/// Convenience operator to merge two VEX_SpecializeFlags enum values into one.
static inline VEX_SpecializeFlags
operator|(VEX_SpecializeFlags a, VEX_SpecializeFlags b)
{
    return VEX_SpecializeFlags(uint(a)|uint(b));
}


#undef ENUM_TO_MASK

#endif

