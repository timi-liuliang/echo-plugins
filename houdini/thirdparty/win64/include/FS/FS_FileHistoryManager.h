/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_FileHistoryManager.h (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __FS_FileHistoryManager__
#define __FS_FileHistoryManager__

#include "FS_API.h"
#include "FS_FileHistory.h"

// IMPORTANT: Must be kept in sync with FS_FileHistoryTypeNames
enum FS_FileHistoryType{
    FS_FILEHISTORY_HIP = 0,
    FS_FILEHISTORY_PIC,
    FS_FILEHISTORY_GEO,
    FS_FILEHISTORY_RAMP,
    FS_FILEHISTORY_CAPT,
    FS_FILEHISTORY_CLIP,
    FS_FILEHISTORY_PAINT,
    FS_FILEHISTORY_LUT,
    FS_FILEHISTORY_CMD,
    FS_FILEHISTORY_MIDI,
    FS_FILEHISTORY_TXT,
    FS_FILEHISTORY_I3D,
    FS_FILEHISTORY_CHAN,
    FS_FILEHISTORY_FONT,
    FS_FILEHISTORY_OTL,
    FS_FILEHISTORY_MISC,
    FS_FILEHISTORY_SIM,
    FS_FILEHISTORY_SIMDATA,
    FS_FILEHISTORY_DAE,
    FS_FILEHISTORY_DS,
    FS_FILEHISTORY_FBX,
    FS_FILEHISTORY_GAL,
    FS_FILEHISTORY_MOV,
    FS_FILEHISTORY_ICON,
    FS_FILEHISTORY_ALEMBIC,
    FS_FILEHISTORY_GLTF,
    FS_FILEHISTORY_LIGHTRIG,
    FS_FILEHISTORY_PRJ,
    FS_FILEHISTORY_PSD,
    FS_FILEHISTORY_DIRECTORY,
    FS_MAX_HISTORY_TYPES
};

class	FS_FileHistory;

class FS_API FS_FileHistoryManager
{
public:

    static FS_FileHistoryManager 	*getFileHistoryManager();
    ~FS_FileHistoryManager();

    void	 saveFileHistoryList();
    void	 saveDirHistoryList();
    
    void	 loadFileHistoryList();
    void	 loadDirHistoryList();


    // Queries the files for a menu.
    int		 numFilesInList(FS_FileHistoryType filetype) const;
    const char	*getFilenameFromList(int index, FS_FileHistoryType filetype) const;

    // Queries the directories for a menu.
    int		 numDirsInList(FS_FileHistoryType filetype) const;
    const char	*getDirFromList(int index, FS_FileHistoryType filetype) const;
    
    void	 getFormattedFilenameFromList(int index,
			FS_FileHistoryType filetype, UT_String &path) const;
    void	 getFormattedDirFromList(int index,
			FS_FileHistoryType filetype, UT_String &path) const;

    // This will add to both the file and directory history
    void	 addFileToHistoryList(const char *filename, 
		    FS_FileHistoryType filetype);
    void	 removeFileFromHistoryList(const char *filename,
		    FS_FileHistoryType filetype);

protected:
    FS_FileHistoryManager();
    
    void	 saveHistoryList(const char *filename, bool filelist);
    void	 loadHistoryList(const char *filename, bool filelist);
private:
    FS_FileHistory	 *getHistoryList(FS_FileHistoryType filetype);
    const FS_FileHistory *getHistoryList(FS_FileHistoryType filetype) const;
    FS_FileHistory	 *getDirHistoryList(FS_FileHistoryType filetype);
    const FS_FileHistory *getDirHistoryList(FS_FileHistoryType filetype) const;
    
    UT_ValArray<FS_FileHistory *>	myHistoryList;    
    UT_ValArray<FS_FileHistory *>	myDirHistoryList;    
};

#define FSgetFileHistoryManager FS_FileHistoryManager::getFileHistoryManager

#endif
