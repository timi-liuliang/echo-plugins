/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_UndoBlock.h ( Utility Library, C++)
 *
 * COMMENTS:
 *		This class is used by UT_UndoManager to organize groups
 *		of operations into a singe undoable block. It is basically
 *		a stack of UT_Undo's with an associated name (for the undo
 *		menu entry)
 */

#ifndef __UT_UndoBlock__
#define __UT_UndoBlock__

#include "UT_API.h"
#include "UT_String.h"
#include "UT_Undo.h"
#include "UT_ValArray.h"
#include <SYS/SYS_Types.h>


class UT_API UT_UndoBlock : public UT_Undo
{
public:
			 explicit UT_UndoBlock(const char *name, UT_UndoBlock *next = NULL);
			~UT_UndoBlock();

    int			 isEmpty() const		{ return !myUndoHead; }
    void		 addUndo(UT_Undo *);
    void		 setNestedBlockName(int level, const char *name);
    const char		*getNestedBlockName(int level) const
			 {
			    if (level < myNestedNames.entries())
				return myNestedNames(level);
			    return "";
			 }

    virtual void	 undo();
    virtual void	 redo();
    virtual int          isUndoBlock() const { return 1; }

    // needToUndo/Redo return true if any of the UT_Undos in the block return
    // true for the corresponding method
    virtual bool	 needToUndo();
    virtual bool	 needToRedo();

    /// Free all invalid undos/redos. Returns true if any pruning was done.
    bool		 pruneInvalidUndos();

    virtual int64	 getMemoryUsage();

    void		 clearContents();

    UT_UndoBlock	*getNextBlock()			{ return myNextBlock; }
    const UT_UndoBlock	*getNextBlock() const		{ return myNextBlock; }
    void		 setNextBlock(UT_UndoBlock *b)	{ myNextBlock = b; }

    // This is firefighting. The undo strategy we've employed is errorprone,
    // and this method attempts to correct frequent programmer errors.
    // Essentially, when a too-many-begins error occurs, this method is
    // called, and it lops out any undos that don't belong in this block,
    // returning them. The caller is responsible for taking ownership of
    // these returned undos.
    UT_UndoBlock	*repairErrors();
    void		 debugPrint(int indent = 1) const;

    // A suspend id of 0 means we're NOT suspended (the default)
    exint		 getSuspendId() const { return mySuspendId; }
    void		 setSuspendId(exint id) { mySuspendId = id; }

private:
    UT_Undo		*myUndoHead;
    UT_UndoBlock	*myNextBlock;
    UT_ValArray<char *>	 myNestedNames;
    int64		 myMemoryUsage;
    exint		 mySuspendId;
};

#endif

