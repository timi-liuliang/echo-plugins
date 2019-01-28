/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_StaleMantraFiles.h (FS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __FS_StaleMantraFiles__
#define __FS_StaleMantraFiles__

#include "FS_API.h"

class UT_StringArray;

/// When piping IFDs to mantra, SOHO will spool geometry and possibly other
/// files to $HOUDINI_TEMP_DIR/ifds/storage.  It saves the geometry files to
/// filenames of the form: PID_XXXXX.YY, where PID is the PID of the mantra
/// process.
/// 
/// If mantra terminates before the geometry files have been read, they are
/// left dangling in the temporary drive, and may cause the disk to fill up.
///
/// This class is a convenience class for dealing with these temporary files.
class FS_API FS_StaleMantraFiles
{
public:
    /// Get a list of stale files, which have no process running.
    /// The function will fill up the @c removed array with the full path of
    /// all files that were removed.  The @c failed_files list is a list of
    /// files which we think are stale files, but we were unable to remove.
    ///
    /// The @c dry_run boolean lets you call the function without actually
    /// making changes to the file system.  In this case, all files will be
    /// lised in the @c removed list.
    ///
    /// The file returns @c true if there were stale files detected.
    static bool	cleanup(UT_StringArray &removed,
				UT_StringArray &failed_files,
				UT_StringArray *ignored_files,
				bool dry_run);
};

#endif

