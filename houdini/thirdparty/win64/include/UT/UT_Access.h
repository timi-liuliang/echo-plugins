/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Access.h (UT Library, C++)
 *
 * COMMENTS:	Provides cross-platform version of access(), UTaccess().
 */

#ifndef __UT_ACCESS_H_INCLUDED__
#define __UT_ACCESS_H_INCLUDED__

#include "UT_API.h"

#ifdef _WIN32

//
// Windows
//

#include <iosfwd>

#define F_OK 0
#define X_OK 1
#define W_OK 2
#define R_OK 4

/// Platform-independent wrapper for the POSIX access() function. Returns 0 if
/// we have the requested permissions.
UT_API int UTaccess(const char *path, int mode);

/// Method for debugging UTaccess() problems with verbose into set to out.
UT_API int UTaccessDebug(const char *path, int mode, std::ostream &out);

#else

//
// Non-Windows
//

#include <unistd.h>

/// Platform-independent wrapper for the POSIX access() function. Returns 0 if
/// we have the requested permissions.
static inline int
UTaccess(const char *path, int mode)
{
    return access(path, mode);
}

#endif // _WIN32

/// Sets the timeout for how old UTaccess() cached results are deemed valid.
UT_API int UTaccessSetCacheExpiry(int num_seconds);

/// Temporarily disable use of the UTaccess() cache.
/// This currently only has an effect on Windows for testing W_OK of network
/// paths.
class UT_DisableAccessCacheScope
{
public:
    inline UT_DisableAccessCacheScope()
	: myOldExpiry(UTaccessSetCacheExpiry(0))
    {
    }

    inline ~UT_DisableAccessCacheScope()
    {
	(void) UTaccessSetCacheExpiry(myOldExpiry);
    }

private:
    int	    myOldExpiry;
};

#endif // __UT_ACCESS_H_INCLUDED__
