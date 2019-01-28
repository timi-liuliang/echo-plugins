/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_Node.h ( SHOP Library, C++)
 *
 * COMMENTS:	Shader OPs.  These are nodes which collect all shading
 *		information for nodes into a nice compact encapsulation.
 */

#ifndef __SHOP_NodeTypes__
#define __SHOP_NodeTypes__

#include "SHOP_API.h"
#include <VOP/VOP_Types.h>

typedef enum {
    SHOP_INVALID	= VOP_TYPE_UNDEF,

    // Shaders applied to geometry
    SHOP_SURFACE	= VOP_SURFACE_SHADER,
    SHOP_SURFACE_SHADOW	= VOP_SURFACE_SHADOW_SHADER,
    SHOP_DISPLACEMENT	= VOP_DISPLACEMENT_SHADER,
    SHOP_GEOMETRY	= VOP_GEOMETRY_SHADER,	// Procedural geometry
    SHOP_INTERIOR	= VOP_INTERIOR_SHADER,	// Volume interior shader

    // Shaders for computing illumination
    SHOP_LIGHT		= VOP_LIGHT_SHADER,		// Illumination
    SHOP_LIGHT_SHADOW	= VOP_LIGHT_SHADOW_SHADER,	// Shadow occlusion
    SHOP_LIGHT_FILTER	= VOP_LIGHT_FILTER_SHADER,	// Filter (eg gobo)

    // Shaders for atmospherics
    SHOP_ATMOSPHERE	= VOP_ATMOSPHERE_SHADER,	// General atmosphere

    // Typically associated with a camera
    SHOP_LENS		= VOP_LENS_SHADER,		// Modify camera rays
    SHOP_OUTPUT		= VOP_OUTPUT_SHADER,		// Output shader
    SHOP_BACKGROUND	= VOP_BACKGROUND_SHADER,	// Background shader

    // Special shaders
    SHOP_PHOTON		= VOP_PHOTON_SHADER,

    SHOP_IMAGE3D	= VOP_IMAGE3D_SHADER,		// 3D image generator
    SHOP_CVEX		= VOP_CVEX_SHADER,		// CVEX SHOP
    SHOP_GENERIC	= VOP_GENERIC_SHADER,		// Generic SHOP shader.
    SHOP_BSDF		= VOP_BSDF_SHADER,		// Generic BxDF shader.

    SHOP_COSHADER	= VOP_COSHADER_SHADER,		// RSL co-shader
    SHOP_COSHADER_ARRAY = VOP_COSHADER_ARRAY,		// RSL co-shader array

    SHOP_PROPERTIES	= VOP_PROPERTIES_SHADER,	// Render Properties
    SHOP_MUTABLE	= VOP_MUTABLE_SHADER,

    SHOP_MATERIAL	= VOP_MATERIAL_SHADER,		// material shopnet
    SHOP_VOP_MATERIAL   = VOP_VOP_MATERIAL_SHADER,	// material vopnet
    SHOP_SHADER_CLASS	= VOP_SHADER_CLASS_SHADER,	// shader class material
    SHOP_VOP_STRUCT	= VOP_STRUCT_DEF_SHADER,	// defines a vop struct
    SHOP_INTEGRATOR	= VOP_INTEGRATOR_SHADER,	// lighting clalculator

    SHOP_MAX_TYPES
} SHOP_TYPE;


/// @{
/// Convert between VOP and SHOP types
static inline VOP_Type  SHOPconvertToVopType(SHOP_TYPE type) 
                        { return (VOP_Type) type; }
static inline SHOP_TYPE	SHOPconvertFromVopType(VOP_Type type)
			{ return (SHOP_TYPE)type; }
/// @}

#endif

