/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_galleries_h__
#define __HOM_galleries_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_ElemPtr.h"
#include <vector>

class HOM_Node;
class HOM_NodeType;
class HOM_Gallery;
class HOM_GalleryEntry;

SWIGOUT(%rename(galleries) HOM_galleries;)

class HOM_API HOM_galleries
{
public:
    virtual ~HOM_galleries()
    {}

    virtual std::string __repr__() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Gallery> > galleries() = 0;

    SWIGOUT(%kwargs galleryEntries;)
    virtual std::vector<HOM_ElemPtr<HOM_GalleryEntry> > galleryEntries(
	    const char *name_pattern = NULL,
	    const char *label_pattern = NULL,
	    const char *keyword_pattern = NULL,
	    const char *category = NULL,
	    HOM_NodeType *node_type = NULL) = 0;

    SWIGOUT(%newobject installGallery;)
    virtual HOM_Gallery *installGallery(const char *gallery_path) = 0;

    virtual bool removeGallery(const char *gallery_path) = 0;

    SWIGOUT(%newobject createGalleryEntry;)
    virtual HOM_GalleryEntry *createGalleryEntry(const char *gallery_path,
	    const char *entry_name,
	    HOM_Node *node = NULL) = 0;
};

#endif

