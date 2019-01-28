/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_DirUtil.C (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef _UT_DIRUTIL_H_
#define _UT_DIRUTIL_H_

#include "UT_API.h"
#include "UT_FileStat.h"
#include <SYS/SYS_Deprecated.h>

#include <time.h>

class UT_FileStat;
class UT_String;
class UT_StringArray;
class UT_StringHolder;

UT_API int UTunixFullPathSpecified(const char *name);
UT_API int UTfullPathSpecified(const char *name);
// This, unlike, UTfullPathSpecified, returns false for ./filename
UT_API bool UTisAbsolutePath(const char *filename);
UT_API bool UTisRootPath(const char *filename);

/// UTgetRootPrefixLength() returns the length of the root part of the file
/// path.
///
/// The remainder of the file path after the root part length will have
/// one of the following forms:
///	'/path/to/file'		    => for absolute paths
///	'relative/path/to/file'	    => for relative paths
///	''			    => for root paths
///
/// Some examples of input file paths and output root lengths:
///	'/path/to/file'		    => 0
///	'relative/path/to/file'	    => 0
///	'C:/path/to/file'	    => 2    (Windows only)
///	'C:relative/path/to/file'   => 2    (Windows only)
///	'//path/to/file'	    => 1    (Windows only)
///	'//path/to/file'	    => 0    (Unix only)
///	'file:///path/to/file'	    => 5
///	'file:/path/to/file'	    => 5
///	'file:relative/path/to/file'=> 5
UT_API int UTgetRootPrefixLength(const char *filename);

UT_API void UTmakeAbsoluteFilePath(UT_String &path,
				   const char *basepath = nullptr);
UT_API void UTmakeAbsoluteFilePath(UT_StringHolder &path,
				   const char *basepath = nullptr);
UT_API const UT_StringArray &UTgetAbsolutePathPrefixes();
UT_API void UTaddAbsolutePathPrefix(const char *prefix);

// Please pass in just the filename and not the entire path.
UT_API bool UTisHiddenFile(const char *filename);

/// Returns true if the given path is a readable, _regular_ file, ie. not a
/// directory.
UT_API bool UTisValidRegularFile(const char *path);

/// Returns true if the given path is a readable _directory_.
UT_API bool UTisValidDirectory(const char *path);

/// Returns true if path is a _directory_, without permission checks
UT_API bool UTisDirectory(const char *path);

/// Returns true if path is a _regular_ file, without permission checks
UT_API bool UTisRegularFile(const char *path);

/// Returns true if file path exists, without permission checks.
/// This is optimized for known platforms.
UT_API bool UTfileExists(const char *path);

#ifdef WIN32
    #include <sys/types.h>
    #include <direct.h>
    #include <errno.h>

    // Define dummy value to compare with errno in assertions related
    // to Linux libraries and large file systems. This define now exists on
    // newer versions of Visual Studio. The current value is defined to be the
    // same as the one found in VC10.
    #ifndef EOVERFLOW
	#define EOVERFLOW	132
    #endif

    typedef void* DIR;
    struct dirent {
	char *d_name;
    };
    UT_API DIR *opendir( const char *name );
    UT_API struct dirent *readdir( DIR *dirp );
    UT_API void closedir( DIR *dirp );
    
    /// Get the current working directory
    UT_API char *getUnixCwd( char *buffer, int maxlen );
#else
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <string.h>
    #include <dirent.h>
    #include <unistd.h>
    #include <stdlib.h>

    /// Get the current working directory
    UT_API char *getUnixCwd( char *buffer, int maxlen );
#endif

/// Do the equivalent of a stat on the last file from readdir or opendir.
UT_API int statLastRead(DIR *dirp, const char *path, UT_FileStat &file_stat);

#endif

