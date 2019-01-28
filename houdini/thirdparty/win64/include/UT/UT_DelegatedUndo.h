/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_DelegatedUndo.h
 *
 * COMMENTS:
 *	This class is the base class for undo objects that delegate their
 *	work to someone else (the undo worker).  It uses UT_UndoWorkerFinder's
 *	to see if the undo worker has been deleted.
 *
 *	See the comments in UT_UndoWorkerFinder.h for instructions on how
 *	to use this class.
 */

#ifndef __UT_DelegatedUndo_h__
#define __UT_DelegatedUndo_h__

#include "UT_API.h"
#include "UT_Undo.h"
#include "UT_UndoWorkerFinder.h"

// Undo classes do not need to work with UT_DelegatedUndoBase directly.
class UT_API UT_DelegatedUndoBase : public UT_Undo
{
public:
    UT_DelegatedUndoBase(int undo_worker_id);
    virtual ~UT_DelegatedUndoBase() { }

    // We can't undo or redo anything if our undo worker has been deleted.
    virtual bool isValid()	{ return opaqueUndoWorker() != 0; }

protected:
    void	*opaqueUndoWorker();

private:
    int		 myUndoWorkerId;
};


// This templated class is a convenience class to avoid casting.
template <class UndoWorker>
class UT_DelegatedUndo : public UT_DelegatedUndoBase
{
public:
    UT_DelegatedUndo(const UT_UndoWorkerFinder<UndoWorker> &undo_worker_finder)
	: UT_DelegatedUndoBase(undo_worker_finder.undoWorkerId()) {}

protected:
    // A null pointer is returned if the undo work has been deleted.
    // The undo() and redo() methods will not be called when undoWorker()
    // returns null.
    UndoWorker	*undoWorker() { return (UndoWorker *)opaqueUndoWorker(); }
};


#endif
