/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_Info.h ( FS Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __FS_Info__
#define __FS_Info__

#include "FS_API.h"
#include <UT/UT_Assert.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_SysClone.h>
#include <SYS/SYS_Types.h>


class FS_Info;
class FS_InfoHelper;
class FS_IndexFile;


/// @brief Make a directory and all the parent directories needed.
///
/// @note Currently, this function only accepts native file paths.
///
/// @param dir_info	@n The directory path to create
/// @param mode		@n The unix file mode
/// @param ignore_umask	@n By default, the users umask will be used in
///			conjunction with the mode. This parameter will @b force
///			the mode to be the given value.
///
/// @return Returns false only if a directory creation attempt failed.
FS_API bool FSmakeDirs(
		FS_Info &dir_info,
		mode_t mode = 0777,
		bool ignore_umask = false);


/// Read, write and execute permissions for files.
enum FS_FileAccessMode
{
    FS_READ	= 0x01,
    FS_WRITE	= 0x02,
    FS_EXECUTE	= 0x04
};

/// Helper class for encapsulating file information
class FS_API FS_Stat
{
public:
    FS_Stat(time_t f_mtime = 0, int64 fsize = 0, bool is_dir = false, bool is_shortcut = false)
	: myModTime(f_mtime), mySize(fsize), myDirectory(is_dir), myShortcut(is_shortcut)
    {
    }
    FS_Stat(const FS_Stat &src) = default;

    // This method is used to maintain the stat information when a
    // sequence of files is being collapsed into one entry.
    void updateSequenceStat(const FS_Stat &entry)
    {
	// The modification time for a sequence is the most current
	// of the modification times of its entries.
	if (myModTime < entry.myModTime)
	    myModTime = entry.myModTime;

	// Size for a sequence should be the sum of all files
	mySize += entry.mySize;
    }

    FS_Stat	&operator=(const FS_Stat &s) = default;
    bool operator==(const FS_Stat &s)
    {
	return (myModTime == s.myModTime
		&& mySize == s.mySize
		&& myDirectory == s.myDirectory
		&& myShortcut == s.myShortcut);
    }

    time_t	modTime() const		{ return myModTime; }
    int64	size() const		{ return mySize; }
    bool	isDirectory() const	{ return myDirectory; }
    bool	isShortcut() const	{ return myShortcut; }

    time_t	myModTime;
    int64 	mySize;
    bool	myDirectory:1,
		myShortcut:1;	// only on Windows
};


/// Class for retrieving file information
class FS_API FS_Info
{
public:
	     /// Use this constructor to create a new info object. The
	     /// specified file is not actually opened or accessed until one of
	     /// the information gathering methods is called.
	     FS_Info(const char *source);

    virtual ~FS_Info();

    /// Return the path parsed from the source
    const UT_StringHolder	&path() const	    { return myPath; }

    /// Return the section parsed from the source
    const UT_StringHolder	&section() const    { return mySection; }

			 /// This function returns whether or not the current
			 /// user has rights to access the file in the given
			 /// mode. The mode parameter can be any of FS_READ,
			 /// FS_WRITE, or FS_EXECUTE. You can also or these
			 /// values together to check multiple access types
			 /// simultaneously. If the mode value is 0, the
			 /// function returns @c true if the file exists.
    bool		 hasAccess(int mode = 0) const;

			 /// Returns if the path is a directory
    bool		 getIsDirectory() const;

			 /// Returns the modification time of the file. The
			 /// returned value can be cast directly to a time_t
			 /// value, as defined in the standard C library.
    int			 getModTime() const;

			 /// Returns the latest modification time of the
			 /// directory and all of its contents recursively.
			 /// On a file it works like getModTime().
    int			 getRecursiveMaxModTime() const;

			 /// Returns the file size
    int64		 getFileDataSize() const;

			 /// Returns the file extension
    UT_String		 getExtension() const;

			 /// Does bulk load of files and directories from the
			 /// source. If the source specified in the constructor
			 /// is a directory rather than an individual file,
			 /// this method will return a listing of all files and
			 /// directories contained in that directory.
    bool		 getContents(UT_StringArray &contents,
				     UT_StringArray *dirs = 0,
				     UT_Array<FS_Stat> *stats = 0,
				     UT_Array<FS_Stat> *dir_stats = 0
				     );

			 /// Treats the source as a path glob pattern and
			 /// expands it to the available files. Note that
			 /// backslash escaping with [] is not supported.
    bool		 glob(UT_StringArray &result, bool only_readable=false);

    bool		 exists() const;
    bool		 fileExists() const;
    bool		 canReadFile() const;
    bool		 canWriteFile() const;
    bool		 canWriteDirectory() const;
    bool		 canDeleteFile() const;
    static char		 getNextSepChar(const char *source);
    static char		 getPrevSepChar(const char *source);
    static bool		 customNavigatePath(UT_String &path, const char *file);
    static bool		 getPathOnDisk(UT_String &path, const char *file);

    static bool		 isNativePath(const char *path);

    static bool		 getContentsFromIndexFile(FS_IndexFile &index,
						  UT_StringArray &contents,
						  UT_StringArray *dirs);
    static bool		 getContentsFromIndexFile(
					FS_IndexFile &index,
					UT_StringArray &contents,
					UT_StringArray *dirs,
					UT_Array<FS_Stat> *stats,
					UT_Array<FS_Stat> *dir_stats);
    static bool		 getContentsFromDiskPath(const char *path,
					UT_StringArray &contents,
					UT_StringArray *dirs,
					UT_Array<FS_Stat> *stats = NULL,
					UT_Array<FS_Stat> *dir_stats = NULL);

    static bool		 statFile(const char *source,
				  const char *filename,
				  FS_Stat *fstat);

    // Functions for adding and removing helpers.
    static void		 addInfoHelper(FS_InfoHelper *helper);
    static void		 removeInfoHelper(FS_InfoHelper *helper);

private:
    static UT_ValArray<FS_InfoHelper *>		&getHelpers();

    UT_StringHolder	myPath;
    UT_StringHolder	mySection;
};


/// This class provides a plug-in method for adding a custom "file system".
/// @see FS_ReaderHelper, FS_WriterHelper
class FS_API FS_InfoHelper
{
public:
	     FS_InfoHelper()
	     { FS_Info::addInfoHelper(this); }
    virtual ~FS_InfoHelper()
	     { FS_Info::removeInfoHelper(this); }

    /// Determine whether this helper can process the filename.
    virtual bool	 canHandle(const char *source) = 0;

    /// @param source Filename for access check
    /// @param mode The read/write/execute status @see @ref FS_FileAccessMode
    virtual bool	 hasAccess(const char *source, int mode) = 0;

    /// Return whether the filename is a directory
    virtual bool	 getIsDirectory(const char *source) = 0;

    /// Get the modification timestamp (returns time_t)
    virtual int		 getModTime(const char *source) = 0;

    /// Get the file size in bytes
    virtual int64	 getSize (const char *source) = 0;

    /// Read the contents of a directory
    /// @param source - The directory to read
    /// @param contents - The list of files in the directory
    /// @param dirs - The list of directories in the directory
    /// If @c dirs is NULL, then all directories should be included in the @c
    /// contents.
    virtual bool	 getContents(const char *source,
				     UT_StringArray &contents,
				     UT_StringArray *dirs) = 0;

    virtual UT_String	 getExtension(const char *source)
			 {
			    UT_String ext(UT_String::ALWAYS_DEEP);
			    ext = UT_String(source).fileExtension();
			    return ext;
			 }

    /// Next separator character
    virtual char	 getNextSepChar(const char * /*source*/)
			 { return '/'; }
    /// Previous separator character
    virtual char	 getPrevSepChar(const char * /*source*/)
			 { return '/'; }
    /// Constructs a new path and returns true, if navigating a file path 
    /// requires something else than appending getNextSepChar() and 
    /// concatenating file. But if concatenating sep char and file is 
    /// sufficient, does nothing and returns false.
    virtual bool	 customNavigatePath(UT_String &path, const char *file)
			 { return false; }

    virtual bool	 getPathOnDisk(UT_String &path, const char *file)
			 { return false; }
};

#endif

