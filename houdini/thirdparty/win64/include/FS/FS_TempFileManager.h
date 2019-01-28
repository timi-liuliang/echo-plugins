/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_TempFileManager.h ( FS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __FS_TempFileManager__
#define __FS_TempFileManager__

#include "FS_API.h"

class UT_StringArray;

/// This class provides an interface to query information about temporary files
/// maintained by the "temp:" handler.
class FS_API FS_TempFileManager
{
public:
    /// Return the number of temporary file handles
    static int	entries();

    /// Close & remove all temporary files.  This may not actually cause the
    /// file to be deleted.  Since there may be streams which still reference
    /// the file, the file will stay in existence until all references are
    /// closed.  However, this will cause all files to be deleted from the temp
    /// file cache.
    /// The function returns the number of files deleted.
    static int	deleteAllFiles();

    /// Close and remove a single temporary file.  Please see @c
    /// deleteTempFiles() for details.
    static bool	deleteFile(const char *name);

    /// Return the list of all temporary files
    static void	getFileList(UT_StringArray &files);
};

#endif
