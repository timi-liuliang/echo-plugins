/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_UndoManager.h ( Utility Library, C++)
 *
 * COMMENTS:
 *		This is the class that manages undo and redo.
 *		There is one global undo manager, but you can make
 *		your own instance if you want (for example the
 *		modeler could handle its undos separately)
 *
 *		NOTE: It is not designed to be subclassed. If you
 *		      do subclass it, you're going to have to
 *		      make the destructor virtual, etc.
 *
 *              Example usage:
 *
 *		void f()
 *		{
 *		    UT_AutoUndoBlock	u("Some undo", ANYLEVEL);
 *
 *		    if (UTwillAcceptUndoAddition())
 *			UTaddToUndoBlock(new MY_UndoSubclass);
 *
 *		    // do stuff
 *		}
 *
 *		First note that we must surround the UTaddToUndoBlock() code
 *		by creating an undoblock which is accomplished by
 *		UT_AutoUndoBlock. This class begins an undoblock and ends when
 *		it goes out of scope. The ANYLEVEL is additional information
 *		saying that this undoblock can be invoked at any time. If it's
 *		known that this should always be the topmost undoblock (eg.
 *		action is invoked from the UI), then you should indicated this
 *		by TOPLEVEL.
 *
 *		Second, the call to UTaddToUndoBlock() must be preceded by a
 *		check to UTwillAcceptUndoAddition(). This check ensures that
 *		undos won't be created while performing the undo/redo action
 *		themselves as well as when undos are explicitly disabled.
 *
 *		Finally, notice that the undo object is created and ownership
 *		is given to UTaddtoUndoBlock() so that UT_UndoManager can free
 *		it when the undo memory limit is reached.
 */

#ifndef __UT_UndoManager__
#define __UT_UndoManager__

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_StringHolder.h"
#include "UT_ValArray.h"
#include <SYS/SYS_Types.h>

class UT_Undo;
class UT_UndoBlock;
struct utClearerStruct;
struct utUndoInterestStruct;

enum UT_UndoBlockType
{
    TOPLEVEL,
    ANYLEVEL
};

class UT_API UT_UndoManager
{
public:
			 UT_UndoManager();
			~UT_UndoManager();

    static void		 disableUndoCreation();
    static void		 enableUndoCreation();

    // When calling beginUndoBlock if you can determine that your code
    // path will only be executed from outside any other undo block (eg
    // on the mouse-down of a mouse-drag instantiated operation) then
    // pass in UT_UNDOBLOCK_TOPLEVEL as the blocktype to beginUndoBlock.
    // Otherwise pass in UT_UNDOBLOCK_GENERIC.
    int			 beginUndoBlock(const char *name,
					UT_UndoBlockType blocktype);

    // Instead of beginning a new undo block, this will append to the previous
    // undo block if the given name matches. This only happens if we currently
    // don't have any undoblocks open. If we do, then we will just use the
    // current one just like beginUndoBlock(). The returned level must be
    // passed to a corresponding endUndoBlock() call.
    int			 beginLastUndoBlock(const char *name,
					    UT_UndoBlockType blocktype);

    // The level that's passed in to endUndoBlock should the level that
    // was returned by the matching call to beginUndoBlock. If the code
    // that called beginUndoBlock is too far from the call to endUndoBlock
    // then you can pass in -1 (ie you don't know) but that means that
    // some of the undo manager error detection won't be able to operate.
    void		 endUndoBlock(int level);

    // This value should always be checked before adding to an undo block.
    int			 willAcceptUndoAddition();
    // This will return a non zero value if we are performing an undo or redo.
    int			 performingUndoRedo();

    // The block level will be > 0 when within an undo block.
    int			 blockLevel() const { return myBlockLevel; }

    // This returns true if any undos are being done, zero if undos
    // are disabled.
    int			 isUndoEnabled();

    // Add an undo to the current undoblock, taken ownership of the pointer.
    // You *must* precede with this with a call to willAcceptUndoAddition().
    // See the top of this file for how to use this.
    void		 addToUndoBlock(UT_Undo *undo);

    // Add an undo to the undo block that last ended. Takes ownership of undo
    void		 addToLastUndoBlock(UT_Undo *undo);

    // Suspend/Resume allowing toplevel undoblocks. All calls to suspend must
    // be matched with a corresponding call to resume.
    void		 suspendTopLevelUndoBlock();
    void		 resumeTopLevelUndoBlock();
    bool		 isTopLevelUndoSuspended() const;

    // Use these functions when you want to tell the undo manager
    // that any undos that are being added (outside your control)
    // should be ignored. Just frame the code section in question
    // with disallowUndos() and allowUndos() in that order.
    void		 disallowUndos()	{ disableUndoCreation(); }
    void		 allowUndos()		{ enableUndoCreation(); }

    // The strict parameter restricts the behaviour to only affect the
    // top undo block, rather than searching through the stack for the
    // first block which will have an effect.
    // The redoable flag determines whether or not the block about to be
    // undone is added to the redo stack
    void		 undo(bool strict = false, bool redoable = true);
    void		 redo(bool strict = false);

    // Get the name of the next undo operation. This will force a prune.
    const char		*getUndoOperationName();

    // Get the name of the next redo operation. This will force a prune.
    const char		*getRedoOperationName();

    // Get the list of undo names.
    // Note that the list contains pointers to within the undoblocks themselves
    // and will become invalid upon any stack changes.
    void		 getUndoNames(UT_StringList &names, bool prune=true);

    // Get the list of redo names. This will force a prune.
    // Note that the list contains pointers to within the undoblocks themselves
    // and will become invalid upon any stack changes.
    void		 getRedoNames(UT_StringList &names, bool prune=true);

    /// Free all invalid undos/redos
    void		 pruneInvalidUndos();

    int64		 getMemoryUsage();
    int64		 getMemoryUsageLimit()	{ return myMemLimit; }
    void		 setMemoryUsageLimit(int64 bytes);

    // clearUndos will wipe away all the undo and redos
    void		 clearUndos();

    // clearUndoFlags is used to clear any flags that have been set
    // for optimization during undo blocks
    void		 clearUndoFlags();
    void		 installUndoFlagClearer(void (*function)(void *data),
	    				        void *data);
    void		 removeUndoFlagClearer(void (*function)(void *data),
	    				        void *data);

    // Install or clear a callback to be called whenever the undo/redo
    // stack changes.  Currently this is intended for the use of the
    // undo manager UI, and so there can only be one interest.
    void		 setUndoInterest(void (*function)(void *data),
					 void *data);

    const UT_UndoBlock	*getUndoStack() const { return myUndoBlockStack; }
    const UT_UndoBlock	*getRedoStack() const { return myRedoBlockStack; }

private:
    void		 checkMemoryUsage();
    void		 pruneUndoBlockStack();
    void		 pruneRedoBlockStack();

    UT_UndoBlock	*myUndoBlockStack;
    UT_UndoBlock	*myRedoBlockStack;

    int			 myBlockLevel;
    int64		 myMemLimit;
    int64		 myNetMemory;

    UT_ValArray<utClearerStruct *>	myClearers;

    utUndoInterestStruct		*myInterest;

    struct SuspendItem
    {
	exint		    myId;
	int		    myBlockLevel;
	UT_StringHolder	    myName;
    };
    UT_Array<SuspendItem>   mySuspendStack;
    exint		    myLastSuspendId;

    char		 myBusyFlag;
    char		 myEnableBugNotify;
};

UT_API UT_UndoManager *UTgetUndoManager();

// These following functions are provided for convenience.
// By default they affect the global Undo manager, but you
// can override that behaviour.
UT_API int	 UTbeginUndoBlock(const char *name,
				  UT_UndoBlockType blocktype,
				  UT_UndoManager *man = 0);
UT_API int	 UTwillAcceptUndoAddition(UT_UndoManager *man = 0);
UT_API void	 UTaddToUndoBlock(UT_Undo *, UT_UndoManager *man = 0);
UT_API void	 UTendUndoBlock(int level, UT_UndoManager *man = 0);
UT_API int	 UTperformingUndoRedo(UT_UndoManager *man = 0);

// Use this name for your undo block if you want it to adopt the name
// of the next undo block that gets added.
#define UT_UNDOBLOCK_ADOPT_NAME "Multiple Undo Operation"

class UT_API UT_AutoUndoBlock
{
public:
    UT_AutoUndoBlock(const char *name, UT_UndoBlockType blocktype)
    {
	myStackLevel = UTbeginUndoBlock(name, blocktype);
    }
    ~UT_AutoUndoBlock()
    {
	UTendUndoBlock(myStackLevel);
    }

private:
    int myStackLevel;
};

// Create local instances of this class to disallow undos on construction and
// allow them again when it goes out of scope.
class UT_API UT_AutoDisableUndos
{
public:
    UT_AutoDisableUndos()
    {
	UT_UndoManager *undo_manager = UTgetUndoManager();
	if (undo_manager)
	    undo_manager->disallowUndos();
    }

    ~UT_AutoDisableUndos()
    {
	UT_UndoManager *undo_manager = UTgetUndoManager();
	if (undo_manager)
	    undo_manager->allowUndos();
    }
};

#endif
