/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_FileSystem.h ( SI Library, C++)
 *
 * COMMENTS:	This provides a "filesystem" interface that's used by
 *		SI_FileChooser.[Ch]  This is used both by the Unix/NT file
 *		system and the Houdini OP "filesystem"
 *
 *		This class does not ever include "." or ".." in the directory
 *		list.
 */

#ifndef __FS_FileSystem__
#define __FS_FileSystem__

#include "FS_API.h"
#include <sys/types.h>
#include <UT/UT_Array.h>
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include "FS_Info.h"

typedef int	(*FS_FileFilter)(const char *filename, void *data);

class FS_API FS_FileSystem {
public:
	     FS_FileSystem(const char *start_path="");
	    ~FS_FileSystem();

    // When over-riding the changePath() method, make sure to harden the myPath
    // member data.

    int			 changePath(const char *newpath);
    const char		*getPath(char *buffer=0, int len=0);
    void		 setPattern(const char *p) { myPattern.harden(p); }

    int			 canCreateDir(const char *path);
    int			 createDir(const char *path);

    int			 canViewFile() const;
    void		 viewFile(const char *path);

    // The pollDirectory() call should be called to load up the
    // directories, files, and respective stats. This is NOT done automatically
    // when changePath() is called.
    int			 pollDirectory();
    void		 forceReload()
			 {
			     // Cause a reload of directory next poll
			     myLastReadTime = 0;
			 }

    // Both of these methods (stat/access) return 1 if successful, 0 on fail.
    // This is not like the unix system calls stat()/access() which return 0 on
    // success, -1 on fail.
    int			 statFile(const char *name, FS_Stat *stats=0);
    int			 accessFile(const char *name, int access_mode);

    void		 setFileFilter(FS_FileFilter filter, void *data);
    void		 setDirectoryFilter(FS_FileFilter filter, void *data);

    UT_StringArray 	 &getDirectories()
				 { return myDirectories; }
    UT_StringArray 	 &getFiles()
				 { return myFilenames; }
    //UT_ValArray<char *> &getHiddenFiles()     { return myFilenames; }
    UT_Array<FS_Stat> &getFileStats()	{ return myFileStats; }
    UT_Array<FS_Stat> &getDirStats ()	{ return myDirStats;  }

    void        	setHidden (bool hidden) { myIgnoreHidden = hidden; }
    // By default, isRoot() checks the current path to see if it's "/"
    int			 isRoot(const char *otherpath = 0);

protected:
    // Stat should return the last modification date of a directory.  This
    // allows us to determine whether the directory has been modified
    // externally or not.
    time_t		 statDirectory();

    // The readContents() method should return 1 if the directory was read
    // successfully, 0 if there was an error.
    // As a note, the file/directory filters will always be set.  By default,
    // they will return the filename passed in.
    int			 readContents();

    /// It would be nice to sort these as we read them in but then the
    /// fileStats will not match up. Therefore, we append entries now and
    /// sort them later.
    void		 addDirectory(const char *name, const FS_Stat &fstat);
    void		 addFilename(const char *name, const FS_Stat &fstat);

    UT_String		 myPath, myLastReadPath;

    bool        	 myIgnoreHidden;

    FS_FileFilter	 myFilter;
    void		*myFilterData;

    FS_FileFilter	 myDirFilter;
    void		*myDirFilterData;

private:
    void		 clearLists();

    time_t		 myLastReadTime;
    UT_StringArray	 myDirectories;
    UT_StringArray	 myFilenames;
    UT_String		 myPattern;

    UT_Array<FS_Stat>	myFileStats;
    UT_Array<FS_Stat> 	myDirStats;
};

#endif
