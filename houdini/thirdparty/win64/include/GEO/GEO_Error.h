/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_Error.h (C++)
 *
 * COMMENTS:
 *		This class is used to store GEO errors or warnings.
 *
*/

#ifndef __GEO_Error__
#define __GEO_Error__

enum GEO_ErrorCodes
{
    GEO_ERR_BAD_GROUP		= 0,    // Bad group 
    GEO_ERR_BAD_PATTERN		= 1,    // Bad group pattern
    GEO_ERR_DUPLICATE_NAME	= 2,	// Group name exists already
    GEO_ERR_NO_SPACES		= 3	// Spaces in the group name 
};

#endif
