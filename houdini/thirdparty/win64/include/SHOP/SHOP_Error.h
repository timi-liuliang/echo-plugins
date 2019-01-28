/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_Error.h ( SHOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SHOP_Error__
#define __SHOP_Error__

enum SHOP_ErrorCode
{
    SHOP_ERR_NO_ERROR		= 0,	// No error
    SHOP_ERR_INPUT_MISMATCH	= 1,	// Inputs of OP don't match
    SHOP_ERR_INVALID_SRC	= 2,	// Invalid source specified

    SHOP_ERR_MAX_ERRORS
};

#endif
