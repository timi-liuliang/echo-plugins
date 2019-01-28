/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_ScriptImports.h ( PRM Library, C++)
 *
 * COMMENTS:	This class identifies a block of parameters in an associated
 *              template list and their import source.
 */

#ifndef __PRM_ScriptImports_h__
#define __PRM_ScriptImports_h__

#include "PRM_API.h"
#include <UT/UT_String.h>
#include <UT/UT_Array.h>

class PRM_API PRM_ScriptImportBlock
{
public:
			 PRM_ScriptImportBlock()
			     : myLabel(UT_String::ALWAYS_DEEP),
			       mySource(UT_String::ALWAYS_DEEP),
			       myToken(UT_String::ALWAYS_DEEP),
			       myMask(UT_String::ALWAYS_DEEP)
			 {
			     init(-1, 0, 0, 0, 0, 0, false);
			 }

			 PRM_ScriptImportBlock(int offset, int size,
					       const char *src)
			     : myLabel(UT_String::ALWAYS_DEEP),
			       mySource(UT_String::ALWAYS_DEEP),
			       myToken(UT_String::ALWAYS_DEEP),
			       myMask(UT_String::ALWAYS_DEEP)
			 {
			     init(offset, size, src, 0, 0, 0, false);
			 }

     void		 init(int offset, int size,
			      const char *src,
			      const char *token,
			      const char *mask,
			      const char *label,
			      bool enable)
			 {
			     myOffset = offset;
			     mySize = size;
			     mySource = src;
			     myToken = token;
			     myMask = mask;
			     myLabel = label;
			     myEnable = enable;

			     myNestLevel = -1;
			     mySwitcherIndex = -1;
			     myFolder = -1;
			 }

     int		 getOffset() const { return myOffset; }

     int		 getNestLevel() const { return myNestLevel; }
     void		 setNestLevel(int level) { myNestLevel = level; }

     int		 getSwitcherIndex() const { return mySwitcherIndex; }
     void		 setSwitcherIndex(int i) { mySwitcherIndex = i; }
     int		 getFolder() const { return myFolder; }
     void		 setFolder(int i) { myFolder = i; }

     int		 getSize() const { return mySize; }

     const UT_String	&getLabel() const { return myLabel; }
     const UT_String	&getSource() const { return mySource; }
     const UT_String	&getToken() const { return myToken; }
     const UT_String	&getMask() const { return myMask; }
     bool		 getEnable() const { return myEnable; }

protected:
    // The offset in the template list of the first parameter that belongs to
    // this block.
    int		 myOffset;

    // The nest level indicates how deep this block is embedded in switchers
    // and other nested blocks.  This is necessary to support import blocks
    // within import blocks.
    int		 myNestLevel;

    // The offset and nest level are not enough to uniquely determine which
    // folder (or even which switcher) an import block correctly belongs to
    // once empty folders enter the picture.  Thus we need to keep track of
    // the switcher index and folder number to which the block belongs.
    int		 mySwitcherIndex;
    int		 myFolder;

    // The number of parameters embedded in this block.  Any embedded switchers
    // are treated as a single entity.
    int		 mySize;

    // The remaining members represent actual data associated with the import
    // block not used to locate it in a template list.
    UT_String	 myLabel;
    UT_String	 mySource;
    UT_String	 myToken;
    UT_String	 myMask;
    bool	 myEnable;
};

typedef UT_Array<PRM_ScriptImportBlock>	PRM_ScriptImportList;

class PRM_API PRM_ScriptImports {
public:
		 PRM_ScriptImports();
		~PRM_ScriptImports();

    void			 appendBlock(int offset, int size,
					     const char *source,
					     const char *token,
					     const char *mask,
					     const char *label,
					     bool enable);

    // All nest levels of the import blocks take the nest level of the
    // given index in the template list as the 0 level.
    int				 getNestBaseIndex() const
				 { return myNestBaseIndex; }
    void			 setNestBaseIndex(int index)
				 { myNestBaseIndex = index; }

    // Go through the import blocks in our list and find the one with a
    // matching label. Returns null if no matching block is found.
    const PRM_ScriptImportBlock	*findBlockWithLabel(const char *label) const;

    PRM_ScriptImportList	 myBlocks;
    int				 myNestBaseIndex;
};

#endif
