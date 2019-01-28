/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FileUtil.h ( UT Library, C++)
 *
 * COMMENTS:	This verb class provides a wide assortment of
 *	file utility functions.
 */

#ifndef __UT_FileUtil__
#define __UT_FileUtil__

#include "UT_API.h"
#include <iosfwd>
#include <stdio.h>
#include <SYS/SYS_Types.h>
#include "UT_NTUtil.h"		// for mode_t

class UT_String;
class UT_WorkBuffer;

// Here are flags used the by the file utility functions:
// If this is set, the showPrompt will be called before any destructive
// action.
#define UT_FILEUTIL_PROMPT	1
// If this is set, the original directory will be included in the
// sweep.
#define UT_FILEUTIL_INCLUSIVE	2

// The standard return code is 0 for success, negative for failure.
// An appropriate error is passed to the error method which can
// be overridden to do something more interesting than dumping to
// the console.
class UT_API UT_FileUtil
{
public:
    enum UT_RemoveDepth { IF_EMPTY, ALL_CONTENTS };


    UT_FileUtil()		{}
    virtual ~UT_FileUtil()	{}
    
    /// Returns the file size of that file in the given path. Returns -1 on
    /// error.
    static int64	getFileSize(const char *path);

    /// Returns the file modification time of the file in the given path.
    /// Returns -1 on error.
    static exint	getFileModTime(const char *path);

    /// Returns the latest file modification time recusively of all files
    /// in the given path including the path itself.  If the given path 
    /// can be a regular file.
    /// Returns -1 if no timestamps can be retrieved.
    static exint	getRecursiveDirMaxModTime(const char *path);

    /// Return the maximum number of file descriptors that are available
    /// for this process.
    static int		getMaxFileDescriptors();

    /// Follow the specified symbolic link to the contents, recursing if the
    /// contents are themselves another symbolic link.  This method will fail
    /// if the specified path is not to a symbolic link, or if the link does
    /// not point to an existing file when allow_dangling_link is false.
    static int		resolveSymbolicLinks(const char *path,
					     UT_WorkBuffer &contents,
					     bool allow_dangling_link = false);

    // Copies a file on disk from the source to the dest.
    // This is only good for smallish files as we allocate a file size
    // memory buffer.
    static int		copyFile(const char *srcname, const char *dstname);
    // Recursively copy a directory
    static int		copyDir(const char *srcname, const char *dstname);
    // Copies a file from a disk into a stream.  This is good for
    // building a .cpio archive.
    static int 		copyFileToStream(const char *srcname, std::ostream &os);

    // Move a file, returns 0 on success.
    static int		moveFile(const char *srcname, const char *dstname);
    // Delete a file
    static int		removeFile(const char *fname);

    // Delete a directory
    static int		removeDir(const char *dname,
				  UT_RemoveDepth depth = IF_EMPTY);

    /// Make a single directory.  All leading path components must exist.
    /// The function returns true if the path already exists or if the path was
    /// made.
    /// @param path @n The path to create
    /// @param mode @n The unix file mode
    /// @param ignore_umask @n
    ///   By default, the users umask will be used in conjunction with the
    ///   mode.  This parameter will @b force the mode to be the given value.
    static bool		makeDir(const char *path, mode_t mode=0777,
				bool ignore_umask = false);

    /// Make a directory and all the parent directories needed
    /// @param path @n The path to create
    /// @param mode @n The unix file mode
    /// @param ignore_umask @n
    ///   By default, the users umask will be used in conjunction with the
    ///   mode.  This parameter will @b force the mode to be the given value.
    static bool		makeDirs(const char *path, mode_t mode=0777,
				bool ignore_umask = false);

    /// Locks a file for exclusive access. Will not return until the file lock
    /// is achiveved or an error occurs. Equivalent to calling writeLockFile().
    /// Note: On Linux, if any file descriptor referring to this file is
    /// closed, the lock will be automatically be closed. Be very careful!
    /// Returns true if the file lock succeeded, and false otherwise.
    static bool		lockFile(int fd);

    /// Unlocks a file that was locked for exclusive access with lockFile, 
    /// above.
    /// Returns true if the unlock succeeded, and false otherwise.
    static bool		unlockFile(int fd);

    /// Locks a file for write access. Will not return until the file lock
    /// is achiveved or an error occurs.
    /// Note: On Linux, if any file descriptor referring to this file is
    /// closed, the lock will be automatically be closed. Be very careful!
    /// Returns true if the file lock succeeded, and false otherwise.
    static bool		writeLockFile(int fd);

    /// Locks a file for read access. Will not return until the file lock
    /// is achiveved or an error occurs.
    /// Note: On Linux, if any file descriptor referring to this file is
    /// closed, the lock will be automatically be closed. Be very careful!
    /// Returns true if the file lock succeeded, and false otherwise.
    static bool		readLockFile(int fd);

    // This navigates up the given path the specified number of levels.
    // It adds a \0 at the correct slash, leaving path to be the
    // proper path name.
    // eg:  upDirectory("foo/bar.vex", 1) = "foo"
    //      upDriectory("foo/bar/", 1) = "foo"
    static void 	upDirectory(char *path, int levels);

    // This scans the (ascii) file fname for any line which starts
    // with the prefix.  If readonly is not true, it writes out a new
    // version of the file lacking those lines.  The return is the
    // number of lines that would be removed, or negative if error.
    static int 		removeLinesFromFile(const char *fname, 
				const char *prefix, int readonly = 0);

    // This is of the form
    // removeOverrideFiles("d:/hsite/houdini5",
    //		       "subnet/Sop", 
    //		       "foo.ds", "Dialog Script", UT_FILEUTIL_PROMPT);
    // If the stripinfo is specified, it will do a removeLinesFromFile
    // with stripinfo rather than delete.
    int 		removeOverrideFiles(const char *newbasepath, 
				    const char *relpath, const char *fname, 
				    const char *english, int flags,
				    const char *stripinfo = 0);

    /// Parses the file in search of the given XML element and outputs 
    /// its contents into the string argument. Returns true uppon success
    /// or false on failure.
    static bool		readXMLElementFromFile( const char * xml_file_path,
				    const char * element_name,
				    UT_String  & element_data );

    // These are UI methods you can override to gain different behaviours
    // for your instance:
    // Severity is the same as UI_Manager::UI_ErrorType.
    virtual void	showError(const char *error, int severity = 2);
    // 0 means Cancel, 1 OK.
    virtual int		showPrompt(const char *prompt);
};

/// A class that allows you to create and lock a file for exclusive access.
/// When instances of this class go out of scope, the locked file is
/// automatically unlocked and closed.
class UT_API UT_AutoFileLock
{
public:
    /// Open and lock a file for exclusive reading and writing. 
    UT_AutoFileLock(const char *filename)
    {
	// Try opening it as if it exists first, then try creating it.
	myFile = fopen(filename, "rb+");
	if (!myFile)
	    myFile = fopen(filename, "wb+");

	if (myFile)
	    myLockSucceeded = UT_FileUtil::lockFile(fileno(myFile));
	else
	    myLockSucceeded = false;
    }
    /// Open and lock a file for read-only (exclusive=false),
    /// or for read-write (exclusive=true). If filename does not exist, then
    /// it is opened as read-write.
    UT_AutoFileLock(const char *filename, bool exclusive)
    {
	// Try opening it as if it exists first, then try creating it.
	myFile = fopen(filename, exclusive ? "rb+" : "rb");
	if (myFile)
	{
	    if (exclusive)
		myLockSucceeded = UT_FileUtil::writeLockFile(fileno(myFile));
	    else
		myLockSucceeded = UT_FileUtil::readLockFile(fileno(myFile));
	}
	else
	{
	    myFile = fopen(filename, "wb+");
	    if (myFile)
		myLockSucceeded = UT_FileUtil::writeLockFile(fileno(myFile));
	    else
		myLockSucceeded = false;
	}
    }

    ~UT_AutoFileLock()
    {
	if (myFile)
	{
	    if (myLockSucceeded)
		UT_FileUtil::unlockFile(fileno(myFile));

	    fclose(myFile);
	}
    }

    /// Get the file pointer for this locked file.
    FILE *getFile() const
    {
	return myFile;
    }

    /// Return whether the file is actually locked.
    bool isLocked() const
    {
	return myLockSucceeded;
    }

private:
    FILE *myFile;
    bool myLockSucceeded;
};

#endif

