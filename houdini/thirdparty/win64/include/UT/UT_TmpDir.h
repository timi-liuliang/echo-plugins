/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TmpDir.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_TmpDir_H__
#define __UT_TmpDir_H__

#include "UT_API.h"
#include <stdio.h>


#include <SYS/SYS_StaticInit.h>
SYSdeclareStaticObject( UT_API, theTmpDirLock );


class UT_String;


/// Return the directory to create temp files within. This differs depending on
/// the OS and the HOUDINI_TEMP_DIR environment variable.
UT_API const char	*UTgetTmpDir();

/// Returns a path to a non-existent filename within UTgetTmpDir().
/// This is equivalent to UTgetTmpNameInDir(UTgetTmpDir(), prefix, filename).
/// If the function fails, filename is NULL.
/// @note This doesn't guarentee that the filename will still be non-existent
/// by the time it is opened.
UT_API bool		 UTgetTmpName(
				UT_String &filename,
				const char *prefix=0,
				bool use_guid=false);

/// Returns a path to a non-existent filename within the given directory.
/// @note This doesn't guarentee that the filename will still be non-existent
/// by the time it is opened.
UT_API bool		 UTgetTmpNameInDir(
				UT_String &filename,
				const char *parent_dir,
				const char *prefix=0,
				bool use_guid=false);

/// Returns the user directory. On Windows, this returns the /Users/<name> (ie.
/// the equivalent to %HOMEPATH%). For other platforms, it is the value of the
/// HOME environment variable.
UT_API const char	*UTgetPersonalDir();

/// Returns the path to the platform specific user desktop directory.
UT_API const char	*UTgetDesktopDir();

/// This should only really be used by UT_EnvControl as the default for
/// HOUDINI_TEMP_DIR when it's not set by the user. Use UTgetTmpDir()
/// everywhere else so that the user may override the location.
UT_API const char	*UTgetDefaultTmpDir();

/// Return a temporary file that will be deleted when it is closed.  On some
/// operating systems, the file will be cached in memory if it does not exceed
/// a threshold size.
UT_API FILE		*UTgetTmpFile();

#endif // __UT_TmpDir_H__
