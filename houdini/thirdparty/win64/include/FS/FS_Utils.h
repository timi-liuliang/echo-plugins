/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_Utils.h ( FS Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __FS_Utils__
#define __FS_Utils__

#include "FS_API.h"

class UT_StringArray;

extern "C" {
    SYS_VISIBILITY_EXPORT extern void installFSHelpers();
}

/// Install the FS helpers
FS_API void	FSloadCustomHelpers();

/// Return a list of the DSO files which have FS helpers defined (i.e. files
/// which have installFSHelpers())
FS_API const UT_StringArray	&FSgetDSOList();

/// Called internally by helpers to register the files
FS_API void			 FScheckDSORegister();

#endif
