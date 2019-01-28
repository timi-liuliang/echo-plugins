/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __DOP_Error__
#define __DOP_Error__

#include <UT/UT_Error.h>

enum DOP_ErrorCode
{
    DOP_MESSAGE = 0,
    DOP_BADINPUTTYPE,		// Connection to input is the wrong type
    DOP_INVALIDINPUT,		// Input is invalid.
    DOP_INVALIDOUTPUT,		// Output connected to our input is invalid.
    DOP_FILEFAILEDREAD,		// Failed readin file "%s".
    DOP_BADOPTIONVALUE,		// The option value didn't match the option type
    DOP_CANTCREATERELATIONSHIP,	// Couldn't create a needed SIM_Relationship
    DOP_NODATANAME,		// No data name specified
    DOP_NODATA,			// Couldn't find specified data %s
    DOP_NOOBJECT,		// Couldn't find specified object %s
    DOP_FILEFAILEDWRITE,	// Failure writing to file "%s".
    DOP_CREATEDIRECTORYFAIL,	// Could not create directory '%s'.
};

#endif
