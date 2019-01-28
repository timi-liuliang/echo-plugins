/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_FileHistory.h (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __FS_FileHistory__
#define __FS_FileHistory__

#include "FS_API.h"
#include<UT/UT_ValArray.h>
#include<UT/UT_String.h>
#include <iosfwd>

class FS_API FS_FileHistory
{
public:
     FS_FileHistory(const int maxFiles, const char *type, bool resolve);
    ~FS_FileHistory();
    
    int		 numFilesinMRU() const;
    const char	*getFilenamefromMRU(int index) const;
    void	 addFiletoMRU(const char *filename);
    void	 removeFilefromMRU(const char *filename);
    void	 getFormattedFilenamefromMRU(int index, UT_String &path) const;
    void	 saveFilestoMRU(std::ostream &os);
    bool	 loadFilesfromMRU(UT_IStream &is);
    const char 	*getTypeNameofMRU();

private:
    UT_ValArray<UT_String *>	 myMRUList;
    int				 myMRUListHead;
    int				 myListMax;
    UT_String			 myTypeName;
    bool			 myResolvePath;
};

#endif
    
