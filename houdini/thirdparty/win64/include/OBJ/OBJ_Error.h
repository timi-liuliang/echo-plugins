/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_Error.h ( Object Library, C++)
 *
 * COMMENTS:
 *		Error codes for object nodes.
 */

#ifndef __OBJ_Error__
#define __OBJ_Error__

enum OBJ_Error
{
    OBJ_ERR_INVALID_CHAIN_END = 0,	// Chain end is invalid
    OBJ_ERR_CANT_KEEP_POS,		// Keep Position when Parenting failed
    OBJ_ERR_INVALID_SHOP,		// Invalid SHOP specified
    OBJ_ERR_CANT_TRANSFER_PRETRANSFORM,	// Can't tranfer pre-transform
    OBJ_ERR_SHEARS_NOT_TRANSFERRED,	// Shears not transferred
    OBJ_ERR_SAVE_INTRINSIC_FAILED,	// Failed to save intrinsic data
    OBJ_ERR_INVALID_PATH,		// Invalid path
    OBJ_ERR_INVALID_SOLVER,		// Invalid bone kinematic solver
    OBJ_ERR_MISSING_TRACK,		// Missing kinematic chop track
    OBJ_ERR_SOLVER_ERROR,		// chop solver has error cooking
    OBJ_ERR_CANT_FIND_OBJ,		// Could not find object %s
    OBJ_ERR_INVALID_POINT_GROUP,	// Invalid point group %s
    OBJ_ERR_NO_POINTS,			// No points available to construct rive
    OBJ_ERR_NO_ATTRIBUTE,		// Can't find the attribute %s
    OBJ_ERR_IPT_DEPRECATE,		// $IPT is deprecated
    OBJ_ERR_SHORTROT_TOO_MANY_INPUTS,	// Too many inputs for Shortest Path (Linear)

    OBJ_ERR_NUM_ERRORS			// sentinel
};
#endif

