/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_GalleryEntry.h ( OP Library, C++)
 *
 * COMMENTS:	Class that defines an OP_Gallery entry.
 */

#ifndef __OP_GalleryEntry__
#define __OP_GalleryEntry__

#include "OP_API.h"
#include <UT/UT_Options.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Compression.h>
#include <FS/FS_IndexFile.h>

#define OP_GALLERY_ENTRY_PREFIX	    "gallery::"

class UT_StringArray;
class OP_Gallery;
class OP_Node;
class OP_Network;

class OP_API OP_GalleryEntry
{
public:
    // This simple constructor creates a gallery entry in memory only. It
    // has no connection to the Gallery Manager or any Gallery.
				 OP_GalleryEntry();
    virtual			~OP_GalleryEntry();

    bool			 getEntryIsValid() const;
    bool			 match(const char *namepattern,
				       const char *labelpattern,
				       const char *keywordpattern,
				       const char *category,
				       const char *optable,
				       const char *optype) const;

    const UT_String		&getFilePath() const;
    const UT_String		&getEntryName() const;
    void			 setEntryName(const char *entryname);

    void			 getLabel(UT_String &label) const;
    void			 setLabel(const char *label);

    void			 getDescription(UT_String &description) const;
    void			 setDescription(const char *description);

    void			 getIcon(UT_String &icon) const;
    void			 setIcon(const char *icon);

    bool			 getAllowIconRegeneration() const;
    void			 setAllowIconRegeneration(bool flag);

    void			 getHelpUrl(UT_String &helpurl) const;
    void			 setHelpUrl(const char *helpurl);

    void			 getOTLPath(UT_String &otlpath) const;
    void			 setOTLPath(const char *otlpath);

    void			 getOpTable(UT_String &optable) const;
    void			 setOpTable(const char *optable);

    void			 getOpTypes(UT_String &optypes) const;
    void			 getOpTypes(UT_StringArray &optypes) const;
    void			 setOpTypes(const char *optypes);
    void			 setOpTypes(const UT_StringArray &optypes);

    void			 getKeywords(UT_String &keywords) const;
    void			 getKeywords(UT_StringArray &keywords) const;
    void			 setKeywords(const char *keywords);
    void			 setKeywords(const UT_StringArray &keywords);

    void			 getCategories(UT_String &cats) const;
    void			 getCategories(UT_StringArray &cats) const;
    void			 setCategories(const char *cats);
    void			 setCategories(const UT_StringArray &cats);

    bool			 hasContents() const;
    bool			 applyContents(OP_Node *destnode) const;
    void			 setContents(OP_Node *srcnode);

    bool			 hasParms() const;
    bool			 applyParms(OP_Node *destnode) const;
    void			 getParms(UT_String &script) const;
    void			 setParms(OP_Node *srcnode);
    void			 setParms(const UT_WorkBuffer &data);

    // Applies information other than parms and contents. Right now this
    // means setting the node comment to the gallery entry description.
    bool			 applyOther(OP_Node *destnode) const;

    // Applies the parms and contents from this gallery entry to the
    // supplied node.
    bool			 canApplyToNode(OP_Node *node) const;
    bool			 applyToNode(OP_Node *node) const;

    // Finds the "natural" manager in which it can be instantiated:
    OP_Network			*getNaturalOpManager(void) const;

    // Creates a new node and applies this entry to that new node. The
    // net parameter specifies the network that should be the parent of
    // the new node. If no network is given, the node is created in the
    // "natural" network type for that gallery entry.
    bool			 canCreateChildNode(OP_Network *net=0) const;
    OP_Node			*createChildNode(OP_Network *net=0,
					    bool move_to_good_pos=true) const;

    // Finds the name of the best operator type acceptable to this entry.
    void			 getBestOpType(UT_String &optype, 
					OP_Network *parent_net = NULL) const;

    // Uses the specified node to initialize the entry. All previous
    // information is removed first.
    void			 setInformationFromNode(OP_Node *node);

    // Clears all information from this entry.
    void			 clearAllInformation();

    // Set one gallery entry equal to another. This copies the options, name,
    // and index file contents, but not the gallery pointer.
    const OP_GalleryEntry	&operator=(const OP_GalleryEntry &src);

    // This compare function can be used with the various UT_ValArray
    // sorting functions to sort gallery entries based on the entry name.
    static int			 compareEntries(OP_GalleryEntry * const *a,
						OP_GalleryEntry * const *b);

    /// Returns ture if the gallery entry is in the process of creating
    /// a node, before it even applies the parameters, contents, or other
    /// changes.
    static bool			 isCreatingNode();

protected:
    // This constructor form is used by Gallery objects loading from the
    // gallery file.
				 OP_GalleryEntry(OP_Gallery *owner,
						 const char *entryname,
						 bool newentry);

    void			 clear();
    bool			 getOption(const char *optionname,
					   UT_String &optionvalue) const;
    bool			 getOption(const char *optionname,
					   UT_StringArray &optionvalues,
					   bool spaceisseparator) const;
    bool			 getOptionBool(const char *optionname,
					   bool &optionvalue) const;
    void			 setOption(const char *optionname,
					   const UT_String &optionvalue);
    void			 setOption(const char *optionname,
					   const UT_StringArray &optionvalues);
    void			 setOptionBool(const char *optionname,
					   bool optionvalue);

    void			 saveCompressedSection(const char *section,
        			                       const UT_WorkBuffer &data,
        			                       UT_CompressionType type);
    bool			 loadCompressedSection(const char *section,
						       char *&buffer,
						       int &buflen) const;
    void			 allowEditing();

private:
    void			 createDefault();
    void			 loadFromGallery();

    OP_Gallery			*myGallery;
    FS_IndexFile		*myIndexFile;
    UT_String			 myEntryName;
    UT_Options			 myOptions;
    bool			 myEditableIndexFile;
    static bool			 theCreatingNodeFlag;

    friend class		 OP_Gallery;
};

typedef UT_ValArray<OP_GalleryEntry *> OP_GalleryEntryList;

#endif

