/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PathFileInfo.h ( Utility Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_PathFileInfo__
#define __UT_PathFileInfo__

#include "FS_API.h"
class UT_PathFile;

#include <UT/UT_String.h>

enum UT_PF_CopyReason
{
    UT_PFCR_DUPLICATE,
    UT_PFCR_CREATE_ORIGINAL,
    UT_PFCR_STASH
};

class FS_API UT_PathFileInfo
{
public:
			 UT_PathFileInfo(UT_PathFile *owner);
    virtual		~UT_PathFileInfo();

    UT_PathFile		*getOwner() { return myOwner; }

    // if str is 0, then the myFileFullPath and myFileName are set.
    void		 computeFullPath(UT_String *str);

    // If you know which directory the file is located in then call the
    // first of the next two. Otherwise call the second one.
    void		 deconstructFullPath(int level);
    int			 deconstructFullPath();

    virtual void	 copy(UT_PathFileInfo *src,
			      UT_PF_CopyReason reason);


    void		 setDiskPath(const char *p) { myDiskPath.harden(p); }
    void		 setFullPath(const char *);
    void		 setFileName(const char *);
    void		 setBaseName(const char *);
    void		 setExtension(const char *);
    void		 setDirIndex(int);

    			 // prepareForSave modifies the Dir Index if necessary
			 // until it refers to an appropriate location.
    			 // it returns 0 if no such location was found; 1 ow.
			 // The avoid_* flags determine how the location is
			 // found:
			 //
			 // avoid_overwrite means it won't overwrite any
			 //                       existing files
			 // avoid_hide means it won't be hidden by hide files
			 // avoid_override means it must the topmost file in
			 // the path.
    
    int			 prepareForSave(int avoid_overwrite=0,
					int avoid_hide=1, 
					int avoid_override=1);

    void		 setHideFlag(int o);
    void		 setAppendFlag(int o);
    void		 setNoSaveFlag(int o);
    void		 setFilteredFlag(int o)	{ myFilteredFlag = o; }


    const char		*getDiskPath() { return myDiskPath; }
    const char		*getFullPath();
    const char		*getDirectory();
    const char		*getFileName();
    const char		*getBaseName();
    const char		*getExtension();
    int			 getDirIndex();

    int			 getHideFlag();
    int			 getAppendFlag();
    int			 getNoSaveFlag();
    int			 getFilteredFlag()	{ return myFilteredFlag; }

private:
    // not implemented:
    UT_PathFileInfo	&operator =(const UT_PathFileInfo&);

private:

    void		 updateHintFlag();

    UT_String		 myDiskPath;
    UT_String		 myFullPath;
    UT_String		 myDirectory;
    UT_String		 myFileName;
    UT_String		 myBaseName;
    UT_String		 myExtension;

    int			 myDirIndex;

    unsigned int	 myHideFlag:1,
			 myAppendFlag:1,
			 myNoSaveFlag:1,
			 myFilteredFlag:1,
			 myFullPathOKflag:1,
			 myDirIndexOKflag:1,
			 myFileNameOKflag:1,
			 myBaseNameOKflag:1,
			 myExtensionOKflag:1,
			 myHintOKflag:1;

    UT_PathFile		*myOwner;
};

#endif

