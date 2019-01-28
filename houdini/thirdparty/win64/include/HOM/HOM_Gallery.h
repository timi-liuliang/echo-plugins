/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Gallery_h__
#define __HOM_Gallery_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Module.h"
#include "HOM_Errors.h"
#include "HOM_ElemPtr.h"
#include <vector>

class HOM_NodeType;
class HOM_GalleryEntry;

SWIGOUT(%rename(Gallery) HOM_Gallery;)

class HOM_API HOM_Gallery
{
public:
    HOM_Gallery()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Gallery(const HOM_Gallery &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Gallery()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    SWIGOUT(%kwargs galleryEntries;)
    virtual std::vector<HOM_ElemPtr<HOM_GalleryEntry> > galleryEntries(
	    const char *name_pattern = NULL,
	    const char *label_pattern = NULL,
	    const char *keyword_pattern = NULL,
	    const char *category = NULL,
	    HOM_NodeType *node_type = NULL) = 0;

    SWIGOUT(%newobject createEntry;)
    virtual HOM_GalleryEntry *createEntry(const char *entry_name,
	    HOM_Node *node = NULL) = 0;

    virtual void deleteEntry(const char *entry_name) = 0;
};

#endif

