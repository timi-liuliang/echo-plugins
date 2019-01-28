/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_Language.h ( VOP Library, C++)
 *
 * COMMENTS:	This is an abstraction for different languages for VOPs.
 *		This class is basically a mapper from the parameters to types
 *		and other stuff.
 */

#ifndef __VOP_Types__
#define __VOP_Types__

#include "VOP_API.h"
#include <VEX/VEX_VexTypes.h>
#include <VEX/VEX_RslTypes.h>
#include <VEX/VEX_OslTypes.h>

template <typename T> class UT_Array;


typedef enum {
    // VEX types
    VOP_TYPE_UNDEF = VEX_TYPE_UNDEF,
    VOP_TYPE_VECTOR4 = VEX_TYPE_VECTOR4,
    VOP_TYPE_VECTOR  = VEX_TYPE_VECTOR,
    VOP_TYPE_FLOAT   = VEX_TYPE_FLOAT,
    VOP_TYPE_INTEGER = VEX_TYPE_INTEGER,
    VOP_TYPE_STRING  = VEX_TYPE_STRING,
    VOP_TYPE_MATRIX3 = VEX_TYPE_MATRIX3,
    VOP_TYPE_MATRIX4 = VEX_TYPE_MATRIX4,
    VOP_TYPE_BSDF    = VEX_TYPE_BSDF,
    VOP_TYPE_VECTOR2 = VEX_TYPE_VECTOR2,
    VOP_TYPE_MATRIX2 = VEX_TYPE_MATRIX2,

    // Structs are considered custom types.
    VOP_TYPE_CUSTOM = VEX_MAX_TYPES,
    VOP_TYPE_STRUCT = VOP_TYPE_CUSTOM,

    VOP_TYPE_VECTOR4_ARRAY,
    VOP_TYPE_VECTOR_ARRAY,
    VOP_TYPE_VECTOR2_ARRAY,
    VOP_TYPE_FLOAT_ARRAY,
    VOP_TYPE_INTEGER_ARRAY,
    VOP_TYPE_STRING_ARRAY,
    VOP_TYPE_MATRIX2_ARRAY,
    VOP_TYPE_MATRIX3_ARRAY,
    VOP_TYPE_MATRIX4_ARRAY,

    // RSL types
    VOP_TYPE_POINT,	// RSL point
    VOP_TYPE_NORMAL,	// RSL normal
    VOP_TYPE_COLOR,	// RSL color
    VOP_TYPE_POINT_ARRAY,
    VOP_TYPE_NORMAL_ARRAY,
    VOP_TYPE_COLOR_ARRAY,

    VOP_TYPE_ERROR,	// An error type (different than undefined type)
    VOP_TYPE_VOID,	// Mainly for function return type.
    VOP_TYPE_METHOD,	// Member method of a class.

    // Shader types
    VOP_SHADER_START,		// Marker for start of shader types

    VOP_SURFACE_SHADER,
    VOP_SURFACE_SHADOW_SHADER,
    VOP_DISPLACEMENT_SHADER,
    VOP_GEOMETRY_SHADER,
    VOP_INTERIOR_SHADER,
    VOP_LIGHT_SHADER,
    VOP_LIGHT_SHADOW_SHADER,
    VOP_LIGHT_FILTER_SHADER,
    VOP_ATMOSPHERE_SHADER,
    VOP_LENS_SHADER,
    VOP_OUTPUT_SHADER,
    VOP_BACKGROUND_SHADER,
    VOP_PHOTON_SHADER,
    // VOP_EMITTER_SHADER,
    // VOP_PHOTONVOLUME_SHADER,
    // VOP_ENVIRONMENT_SHADER,
    // VOP_CONTOUR_SHADER,
    // VOP_CONTOUR_STORE_SHADER,
    // VOP_CONTOUR_CONTRAST_SHADER,
    // VOP_TEXTURE_SHADER,
    VOP_IMAGE3D_SHADER = VOP_PHOTON_SHADER + 8,
    VOP_POP_SHADER, 
    VOP_SOP_SHADER, 
    VOP_CHOP_SHADER, 
    VOP_COP2_SHADER, 
    VOP_CVEX_SHADER,
    VOP_GENERIC_SHADER,		// Generic shader with generic context.
    VOP_BSDF_SHADER,		// Generic BxDF shader (for surface and volume)
    VOP_COSHADER_SHADER,
    VOP_COSHADER_ARRAY,
    VOP_PROPERTIES_SHADER,
    VOP_MUTABLE_SHADER,		// For switch & select which inherit input type
    VOP_MATERIAL_SHADER,	// Material: collection of shader vopnets
    VOP_VOP_MATERIAL_SHADER,	// Material: collecion of shaders in one vopnet
    VOP_SHADER_CLASS_SHADER,	// Material: class with shader methods in vopnet
    VOP_STRUCT_DEF_SHADER,	// Vop network defines a vop struct 
    VOP_INTEGRATOR_SHADER,	// BxDF integrator for lighting calculation

    VOP_SHADER_END,		// Marker for end of shader types

    VOP_MAX_TYPES,		// Sentinal, before the uniform types

    VOP_TYPE_UNIFORM_MODIFIER = 0x1000,
    VOP_TYPE_UFLOAT	= (VOP_TYPE_UNIFORM_MODIFIER | VOP_TYPE_FLOAT),
    VOP_TYPE_UVECTOR	= (VOP_TYPE_UNIFORM_MODIFIER | VOP_TYPE_VECTOR),
    VOP_TYPE_UPOINT	= (VOP_TYPE_UNIFORM_MODIFIER | VOP_TYPE_POINT),
    VOP_TYPE_UNORMAL	= (VOP_TYPE_UNIFORM_MODIFIER | VOP_TYPE_NORMAL),
    VOP_TYPE_UCOLOR	= (VOP_TYPE_UNIFORM_MODIFIER | VOP_TYPE_COLOR),
    VOP_TYPE_UMATRIX4	= (VOP_TYPE_UNIFORM_MODIFIER | VOP_TYPE_MATRIX4),
} VOP_Type;

#define VOP_BASE_TYPE(type)	((VOP_Type)(type&(VOP_TYPE_UNIFORM_MODIFIER-1)))
#define VOP_GET_MODIFIER(type)	((VOP_Type)(type&VOP_TYPE_UNIFORM_MODIFIER))
#define VOP_INVALID_TYPE(type)	((type)==VOP_TYPE_UNDEF||(type)==VOP_TYPE_ERROR)
#define VOP_VALID_TYPE(type)	(!VOP_INVALID_TYPE(type))

/// Returns true if the type is some kind of an array, and false otherwise.
VOP_API bool		    VOPisArrayType( VOP_Type type );

/// @{ Returns the element type of an array and 
/// an array type based on array element type, respectively.
VOP_API VOP_Type	    VOPgetElementTypeFromArray( VOP_Type array_type );
VOP_API VOP_Type	    VOPgetArrayTypeFromElement( VOP_Type element_type );
/// @}


/// Obtains the shader type based on its name.
/// A shader node has a single shader type, which can correspond to a shading
/// context in a given languate, such as surface or displacement. But, 
/// it can be also a material or a class that has no such correspondence.
VOP_API void		    VOPgetShaderTypes(UT_Array<VOP_Type> &types);
VOP_API const char *	    VOPgetShaderTypeName(VOP_Type type);
VOP_API const char *	    VOPgetShaderTypeLabel(VOP_Type type);
VOP_API VOP_Type	    VOPgetShaderType(const char *name);

/// A generic shading context type that can be interpreted as VEX_ContextType,
/// RSL_ContextType, or OSL_ContextType. Different languages support different
/// shading contexts, although they share some common ones (eg, surface).
/// Node's shader type may allow a single context (eg, surface shader contains
/// VOPs that are valid only in the surface context), but it may allow 
/// several context (eg, material allows surface and displacement variables 
/// and fuctions).
typedef int VOP_ContextType;
#define VOP_CONTEXT_TYPE_INVALID    0

/// A definition of the context type list.
typedef UT_Array<VOP_ContextType>	VOP_ContextTypeList;
typedef UT_Array<VOP_Type>		VOP_ShaderTypeList;

/// Defines the known languages. Each of the languages has own set of context
/// types.
typedef enum {
    VOP_LANGUAGE_INVALID,
    VOP_LANGUAGE_VEX,
    VOP_LANGUAGE_RSL,
    VOP_LANGUAGE_OSL
} VOP_LanguageType;


// TODO: remove these conversion functions
/// @{ Utility function to convert between lowel level contexts and shader type.
VOP_API VEX_ContextType	    VOPconvertToVexContextType( VOP_Type shader_type );
VOP_API RSL_ContextType	    VOPconvertToRslContextType( VOP_Type shader_type );
VOP_API OSL_ContextType	    VOPconvertToOslContextType( VOP_Type shader_type );
VOP_API VOP_Type	    VOPconvertToShaderType( VEX_ContextType type ); 
VOP_API VOP_Type	    VOPconvertToShaderType( RSL_ContextType type);
VOP_API VOP_Type	    VOPconvertToShaderType( OSL_ContextType type);
/// @}

// TODO: move this to language class
/// Converts the 'shader_type' based on a language to a VEX or RSL 
/// context type encoded into the returned value of VOP_ContextType.
VOP_API VOP_ContextType	    VOPconvertToContextType( VOP_Type shader_type,
						     VOP_LanguageType language);

// TODO: move this to language class
/// Converts the context type to a 'shader type'.
VOP_API VOP_Type    	    VOPconvertToShaderType(  VOP_ContextType type,
						     VOP_LanguageType language);

// TODO: move these to language class
/// @{ Utility function to find out the context type based on the token
/// name (plus the language type), and vice versa.
VOP_API VOP_ContextType	    VOPconvertToContextType( const char * context_name,
						     VOP_LanguageType language);
VOP_API const char *	    VOPgetContextTypeName( VOP_ContextType context_type,
						     VOP_LanguageType language);
VOP_API const char *	    VOPgetContextTypeLabel(VOP_ContextType context_type,
						     VOP_LanguageType language);
/// @}

// TODO: remove these conversion functions
/// @{ Utility functions to turn lower level context types into VOP_ContextType.
VOP_API VOP_ContextType	    VOPconvertToContextType( VEX_ContextType type );
VOP_API VOP_ContextType	    VOPconvertToContextType( RSL_ContextType type );
VOP_API VOP_ContextType	    VOPconvertToContextType( OSL_ContextType type );
VOP_API VEX_ContextType	    VOPconvertToVexContextType( VOP_ContextType type );
VOP_API RSL_ContextType	    VOPconvertToRslContextType( VOP_ContextType type );
VOP_API OSL_ContextType	    VOPconvertToOslContextType( VOP_ContextType type );
/// @}

VOP_API bool	isValidPassThroughType (VOP_Type type);

#endif
