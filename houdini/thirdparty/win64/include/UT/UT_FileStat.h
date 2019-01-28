/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FileStat.h (UT Library, C++)
 *
 * COMMENTS:	Cross-platform method of obtaining file statistics
 */

#ifndef __UT_FILESTAT_H_INCLUDED__
#define __UT_FILESTAT_H_INCLUDED__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include <time.h>

class UT_API UT_FileStat
{
public:
    enum FileType
    {
	REGULAR,
	EXECUTABLE,
	DIRECTORY,
	SHORTCUT,	// only on Windows
	SOCKET,
	CHAR_DEVICE,
	BLOCK_DEVICE,
	SPECIAL
    };

    // The permission type stores read/write/execute permissions for the file.
    // We define it as 'int' to match UTaccess.
    typedef int	PermissionType;

    UT_FileStat(FileType ftype = REGULAR,
	    int64 fsize = 0,
	    time_t f_atime = 0,
	    time_t f_ctime = 0,
	    time_t f_mtime = 0,
	    PermissionType f_permissions = 0777)
	: myFileType(ftype)
	, mySize(fsize)
	, myAccessTime(f_atime)
	, myCreateTime(f_ctime)
	, myModTime(f_mtime)
	, myPermissions(f_permissions)
    {
    }

    void
    fixInvalidTimestamps()
    {
	// On Windows, the _findfirst/_findnext functions silently convert 0
	// FILETIME values to -1 if something bad happens. Things can then fail
	// down the line because of this. So clamp these to 0. For an example,
	// see bug #63178.
	if (myAccessTime < 0)
	    myAccessTime = 0;
	if (myCreateTime < 0)
	    myCreateTime = 0;
	if (myModTime < 0)
	    myModTime = 0;
    }

    bool isFile() const
	{ return myFileType == REGULAR || myFileType == EXECUTABLE; }

    FileType		myFileType;
    int64		mySize;
    time_t		myAccessTime;
    time_t		myCreateTime;
    time_t		myModTime;
    PermissionType	myPermissions;
};

/// Obtain file statistics.  Returns 0 on success, else error code.
/// The UT_FileStat structure is only written-to when returning 0.
/// @{
UT_API int UTfileStat(const char *path, UT_FileStat *file_stat);
UT_API int UTfileFStat(int file_descriptor, UT_FileStat *file_stat);
/// @}

#endif // __UT_FILESTAT_H_INCLUDED__
