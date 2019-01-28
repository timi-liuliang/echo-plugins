/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Gallery.h ( OP Library, C++)
 *
 * COMMENTS:	Class associated with a gallery file that manages the
 * 		contained OP_GalleryEntrys.
 */

#ifndef __OP_Gallery__
#define __OP_Gallery__

#include "OP_API.h"
#include <FS/FS_IndexFile.h>
#include "OP_GalleryEntry.h"

class UT_StringArray;
class OP_GalleryManager;

class OP_API OP_Gallery
{
public:
    int				 getNumGalleryEntries() const;
    OP_GalleryEntry		*getGalleryEntry(int index) const;
    OP_GalleryEntry		*findGalleryEntry(const char *entryname) const;
    OP_GalleryEntry		*newGalleryEntry(const char *entryname);
    void			 removeGalleryEntry(const char *entryname);
    void			 removeGalleryEntry(int index);
    void			 getMatchingEntries(OP_GalleryEntryList &found,
						    OP_Node *node) const;
    void			 getMatchingEntries(OP_GalleryEntryList &found,
						    const char *namepattern,
						    const char *labelpattern,
						    const char *keywordpattern,
						    const char *category,
						    const char *optable,
						    const char *optype) const;

    /// Returns the array of all keywords used by gallery entries
    /// that match the requirement of the optable.
    void			 getKeywords(UT_StringArray & keywords,
					     const char *optable = NULL);

    const UT_String		&getFilePath() const;
    bool			 getFileIsValid() const;
    bool			 getFileIsWritable() const;

    bool			 refresh(bool force);
    bool			 save();

protected:
				 OP_Gallery(OP_GalleryManager *owner,
					    const char *filepath);
    virtual			~OP_Gallery();

    void			 clear();

private:
    static bool			 createBackupFile(const char *filename);

    OP_GalleryManager		*myGalleryManager;
    FS_IndexFile		*myIndexFile;
    OP_GalleryEntryList		 myGalleryEntries;
    UT_String			 myFilePath;

    friend class		 OP_GalleryManager;
};

typedef UT_ValArray<OP_Gallery *>	 OP_GalleryList;

#endif

