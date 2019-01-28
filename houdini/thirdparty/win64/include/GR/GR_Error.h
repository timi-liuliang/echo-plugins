/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_Error.h (C++)
 *
 * COMMENTS:
 *		This is a list of all GR errors or warnings.
 *
*/

#ifndef GR_Error_h
#define GR_Error_h

enum GR_ErrorCode
{
    GR_NO_MESSAGE		= 0,
    
    // Legacy messages
    GR_SAVE_DEFVIEW_FAILED	= 1,	// Unable to save default ViewState: %s
    GR_SAVE_DEFVIEW_SUCCESS	= 2,	// View options saved to: %s
    GR_SAVE_OPT_DEFNS_FAILED	= 3,	// Unable to save user options: %s
    GR_SAVE_OPT_DEFNS_SUCCESS	= 4,	// User options saved to: %s
    GR_HYDRA_MESSAGE		= 5,	// Hydra: %s
    GR_GENERIC_MESSAGE		= 6,	// %s

    // Messages
    GR_MESSAGE_INSTANCE_LIMIT_HIT = 100, // point instance poly limit exceeded
    GR_MESSAGE_MATERIAL_LIMIT_HIT = 101, // single obj material limit exceeded

    // Warnings
    GR_WARNING_MSS_PARTIAL	= 200,  // couldn't completely apply MSS
    GR_WARNING_NO_UVS		= 201,  // UVs were not found and required
    GR_WARNING_SUBD_REDUCED	= 202,  // Subdivision level was reduced
    GR_WARNING_VOLUME_REDUCED   = 203,  // Volume resolution was reduced
    GR_WARNING_NO_VERTEX_UVS	= 204,  // No vertex UVs to display
    GR_WARNING_NO_POINT_UVS	= 205,  // No point UVs to display

    // Errors
    GR_ERROR_MISSING_TEXTURE	= 300,	// Texture not found
    GR_ERROR_SOURCE_OP_COOK_FAIL= 301,	// Source OP didn't cook (errored)

    // Really bad errors
    GR_FATAL_SHADER_COMPILE_FAILED = 400, // GLSL shader compile/link error
    
};

#endif
