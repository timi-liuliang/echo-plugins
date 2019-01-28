/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_GalleryManager.h ( OP Library, C++)
 *
 * COMMENTS:	Class to manage OP_Gallerys.
 */

#ifndef __OP_GalleryManager__
#define __OP_GalleryManager__

#include "OP_API.h"
#include <UT/UT_Notifier.h>
#include <UT/UT_SymbolTable.h>
#include "OP_Gallery.h"

class UT_StringArray;
class OP_GalleryCategoryOrder;

typedef enum {
    OP_GALLERY_ADDED,
    OP_GALLERY_REMOVED,
    OP_GALLERY_REFRESHED,
    OP_GALLERY_CONFIGCHANGE
} OP_GalleryChangeType;


typedef UT_NotifierImpl<OP_GalleryChangeType> OP_GalleryNotifier;

class OP_API OP_GalleryManager
{
public:
    int				 getNumGalleries() const;
    OP_Gallery			*getGallery(int index) const;
    OP_Gallery			*findGallery(const char *path) const;
    OP_Gallery			*installGallery(const char *path,
				    bool create, bool addtohistory);
    bool			 removeGallery(const char *path);
    void			 removeGallery(int index);
    void			 getMatchingEntries(
				    OP_GalleryEntryList &found,
				    OP_Node *node) const;
    void			 getMatchingEntries(
				    OP_GalleryEntryList &found,
				    const char *namepattern,
				    const char *labelpattern,
				    const char *keywordpattern,
				    const char *category,
				    const char *optable,
				    const char *optype) const;

    /// Returns the array of all keywords used by entries in all galleries
    /// that match the requirement of the optable.
    void			 getKeywords(UT_StringArray & keywords,
					     const char *optable = NULL);

    /// Adds an extra category even if no entry subscribes to it.
    /// 
    /// @param	optable	    The context in which extra category should appear.
    /// @param	category    The category name to add.
    void			 addExtraCategory( const char *optable,
						   const char *category );

    /// Gets a list of extra categories added for the given optable.
    const UT_StringArray *	 getExtraCategories( const char *optable )const;

    void			 refreshAll(bool force);
    void			 purgeInvalidGalleries();
    void			 sendEvent(OP_GalleryChangeType changetype);
    OP_GalleryNotifier		&getEventNotifier();

    void			 getInstalledGalleryPaths(
				    UT_StringArray &gallerypaths,
				    bool includestandard,
				    bool includehda) const;

    void			 getCommonGalleryDirs(UT_StringArray & paths);

    // Controls whether galleries should create backup files when they
    // are saved.
    void			 setCreateBackups(bool createbackups);
    bool			 getCreateBackups() const;

    // Returns whether or not we are allowed to save the preferences file.
    // This will be false if there is a nosave file in the HOUDINI_PATH.
    bool			 canSavePreferences() const;

    // Obtains the default gallery path.
    const UT_String		&getDefaultGallery();

    /// Gets the sort order of categories (when parent name is not a string). 
    /// Optionally the parent category may be provided in the form of a string
    /// encoding subcategories separated with a slash. When the parent category
    /// name is given, returns the order of sub-categories, if parent is found,
    /// otherwise ther returned array will be empty.
    const UT_StringArray       *getCategorySortOrder(
				    const char *parent_category = NULL);

    // Returns the global gallery manager.
    static OP_GalleryManager	&getManager();

protected:
				 OP_GalleryManager();
    virtual			~OP_GalleryManager();

    void			 clear();
    void			 scanGalleriesPath();

    static void			 fullyQualifyGalleryPath(UT_String &path);

private:
    void			 loadPreferences();
    void			 savePreferences();
    void			 loadUiPreferences();
    void			 saveUiPreferences();

    OP_GalleryList		 myGalleries;
    UT_SymbolMap<int>		 myGalleriesTable;
    OP_GalleryNotifier		 myNotifier;
    UT_String			 myDefaultGallery;
    UT_SymbolMap<UT_StringArray *> myExtraCategories;
    bool			 myCreateBackups;
    OP_GalleryCategoryOrder	*myCategoryOrder;
};

OP_API extern OP_GalleryManager	&OPgetGalleryManager();

#endif

