/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Undo.h ( Utility Library, C++)
 *
 * COMMENTS:
 *		This is a virtual base class defining an undoable operation
 *
 *		Note: The destructor deletes the next undo as well.
 */

#ifndef __UT_Undo__
#define __UT_Undo__

#include "UT_API.h"

#include <SYS/SYS_Types.h>

class UT_String;

class UT_API UT_Undo
{
public:
    			 UT_Undo();
    virtual		~UT_Undo();

    virtual void	 undo() = 0;
    virtual void	 redo() = 0;

    // If isValid() is false, then UT_UndoManager is able to free the undo.
    virtual bool	 isValid()	{ return true; }

    // needToUndo/Redo return true if undoing/redoing this UT_Undo would
    // actually change something, or false otherwise. For example, false would
    // be returned if you try to undo a geometry selection after closing the
    // SOP viewer you performed that selection in. (Without the SOP viewer up,
    // you wouldn't see the selection being undone so the undo didn't actually
    // do anything as far as the user is concerned). Basically, returning
    // false is a hint to the UT_UndoManager that it should skip this undo and
    // go on to undo the next thing on its stack (although calling undo/redo
    // even if needTo returns false is still allowed)
    virtual bool	 needToUndo()	{ return isValid(); }
    virtual bool	 needToRedo()	{ return isValid(); }

    // isUndoBlock returns 1 if and only if this object is a UT_UndoBlock
    // object.
    virtual int		 isUndoBlock() const { return 0; }

    // In the case of DM-based undos, the UT_Undo constructor will
    // set the memory usage size initially to the size of the object
    // itself, and as each undo object in the class hierarchy gets 
    // gets initialized, it will increment this size using 
    // addToMemoryUsage() for additional memory that must be accounted for 
    // (data pointed to by members, etc.)
    virtual int64	 getMemoryUsage()		{ return myMemUsage; }
    void		 addToMemoryUsage(int64 k)	{ myMemUsage += k; }
    void		 setMemoryUsage(int64 k)	{ myMemUsage = k; }

    void		 setNextUndo(UT_Undo *nundo)	{ myNextUndo = nundo; }
    UT_Undo		*getNextUndo()			{ return myNextUndo; }

protected:
    // generate a unique filename in the HOUDINI_TMP_DIR.  Note that only
    // the first five chars of prefix are used, the rest are ignored.
    void		 generateTmpFilename(const char *prefix,
					     UT_String &filename);

private:
    int64		 myMemUsage;

    UT_Undo		*myNextUndo;
};

#endif

