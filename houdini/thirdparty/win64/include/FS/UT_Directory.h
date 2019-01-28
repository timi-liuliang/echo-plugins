/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS library (C++)
 *
 * COMMENTS:	UT_Directory creates a hash list for a directory
 *		This class makes no distinctions between the file types
 *		it leaves this to the user of the class.
 *
 *		Originally taken from UT_Directory
 */

#ifndef __UT_Directory_h__
#define __UT_Directory_h__

#include "FS_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringSet.h>

class FS_API UT_Directory {
public:
    /// Constructs an object corresponding to a directory specified by the
    /// path parameter, and fills the member symbol table with entries
    /// corresponding to files matched by a given pattern.
    /// If includeDots is true, the hidden files (that start with a dot '.')
    /// are also added to the member symbol table.
     UT_Directory(const char *path, const char *pattern=0, int includeDots=1);
    ~UT_Directory();

    /// Returns 1 if the file is found in the directory.
    int		findFile(const char *filename) const;


    /// @{
    /// Returns a list of files matching the pattern passed in.  The result
    ///	contains each matching file separated by a space (or a list of files).
    int		getFiles(const char *pattern, UT_String &result) const;
    int		getFiles(const char *pattern, UT_StringList &list) const;
    int		getFiles(const char *pattern, UT_StringArray &array) const;
    /// @}

    /// Returns the number of files in the member symbol table.
    int		entries() const;

    /// Traverse all the files and call the 'func' callback on the
    /// file names that match a given pattern.
    void	traverseFiles(int (*func)(const char *filename, void *data),
			      void *data = 0, const char *pattern = 0) const;

private:
    UT_StringSet	 myFiles;
};

#endif
