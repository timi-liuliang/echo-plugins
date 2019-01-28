/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PathFile.h (Utility Library, C++)
 *
 * COMMENTS:
 *		This class is used to look for files using searchpaths. 
 *		It has mechanisms for patternmatching, finding only the
 *		file that's highest in the searchpath, or combining them
 *		all, and so on. The way it searches for files is also
 *		special. Check out the .C file for more docs on that.
 */

#ifndef __UT_PathFile__
#define __UT_PathFile__

#include "FS_API.h"
#include <UT/UT_Defines.h>
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>

class UT_WorkBuffer;
class UT_PathSearch;
class UT_PathFileInfo;
class FS_Reader;

#define UT_NOSAVE_HINT ".nosave"
#define UT_HIDE_HINT ".hide"
#define UT_APPEND_HINT ".append"

typedef int (*fs_path_compare_func_t)(UT_PathFileInfo *const*, UT_PathFileInfo *const*);

class FS_API UT_PathFile
{
public:
			 UT_PathFile();
    virtual		~UT_PathFile();


    // You should call these three at least once before using this class
    void		 setSearchPath(int path_id, const char *pattern);

    // Then you should call this method (or some subclass' equivalent)
    void		 applyStandardFilter();


    const char		*getSearchPattern()	{ return mySearchPattern; }

    const char		*getDefaultDir()	{ return myDefaultDir; }
    int			 getDefaultDirIndex()	{ return myDefaultDirIndex; }
    int			 getHomeDirIndex()	{ return myHomeIndex; }


    // This is the simplest method of using this class, it just streams you
    // all the lines in all the files that match your search.
    bool		 getLine(UT_WorkBuffer &wbuf);

    // Some query methods for getting info about the current line of input
    int			 getCurrentLineNumber()	{ return myCurrentLineNumber; }
    const char		*getCurrentFileName();


    // Query methods for getting at the directory list.
    // NOTE: getDir() indexes from the end of the list!
    int			 getDirCount() const;
    const char		*getDir(int i) const;

    // if you set the filtered flag to zero, then you'll get all the matching
    // files in the search path. ie hidden ones, overridden ones, etc
    int			 getFileInfoCount(int filtered=1) const;
    UT_PathFileInfo	*getFileInfo(int i, int filtered=1) const;
    void		 appendFileInfo(UT_PathFileInfo *info)
			    { myFileInfos.append(info); }

    // note: no deleting done
    void		 clearFileInfoList() { myFileInfos.entries(0); }
    

    void		 sortFileInfos(int (*compare)(UT_PathFileInfo *const*t1,
						      UT_PathFileInfo *const*t2));

    virtual UT_PathFileInfo *createFileInfo();
    UT_PathFileInfo	*appendFileInfo(const char *fullpath, int level);
    void		 removeFileInfo(int i);	// NOT filtered
    void		 removeFileInfo(UT_PathFileInfo *);
    void		 removeFileInfo(const char *fullpath);

protected:
    // note: this is quite different from clearFileInfoList above
    void		 clearFileList();
    void		 computeFileList();

    void		 addMatchingFiles(char *fullpattern, int level);
    void		 removeMatchingFiles(char *fullpattern, int level);

    unsigned int	 myBusyFlag:1,
			 myFilesFoundFlag:1;
private:
    void		 computeDefaultDir();
    const UT_PathSearch	*myPathSearch;
    UT_String		 mySearchPattern;
    UT_String		 myDefaultDir;
    int			 myDefaultDirIndex;
    int			 myHomeIndex;
    int			 myCurrentFileIndex;
    int			 myCurrentLineNumber;

    UT_ValArray<UT_PathFileInfo *>	 myFileInfos;

    FS_Reader		*myFile;
    friend int		 utAddMatchingFiles(const char *file, void *data);
    friend int		 utRemoveMatchingFiles(const char *file, void *data);
};

FS_API extern bool UTfindPreferenceFile( int path_id,
					 const char *filename,
					 UT_String &result,
					 bool &no_save);


#endif
