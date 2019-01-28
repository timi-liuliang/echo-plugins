/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_UndoWorkerFinder.h
 *
 * COMMENTS:
 *	This class is contained within an object that does the work to
 *	execute an undo (the undo worker).  When the worker is constructed,
 *	an object of this class is also constructed, and the object adds
 *	itself to the table of undo workers.  When the worker is
 *	destructed, this object is also destructed, and this class removes
 *	itself from the table of undo worker finders.
 *
 *	Use this class as follows:
 *
 *	- If the class that executes the undo/redo's is called Worker, add a
 *	  member variable of type UT_UndoWorkerFinder<Worker> and pass in
 *	  a reference to the Worker class in the worker finder's constructor.
 *	- Create undos that are a subclass of UT_DelegatedUndo<Worker> and
 *	  pass the a reference to the undo worker finder to the constructor
 *	  of the delegated undo.
 *	- The delegated undo's undoWorker() method will return a pointer
 *	  to the Worker object, or null if it has been deleted.  The
 *	  undo()/redo() method will not be called if the undo worker was
 *	  deleted, so undoWorker() won't return null in these methods.
 */

#ifndef __UT_UndoWorkerFinder_h__
#define __UT_UndoWorkerFinder_h__

#include "UT_API.h"
// This base class treats the undo worker as a void pointer.
class UT_API UT_UndoWorkerFinderBase
{
public:
    UT_UndoWorkerFinderBase(void *opaque_undo_worker);
    ~UT_UndoWorkerFinderBase();

    int		 undoWorkerId() const { return myUndoWorkerId; }

    // Normally, the destructor will invalidate itself, but you can invoke this
    // earlier if you want.
    void	 invalidate();

private:
    // The UT_UndoWorkerFinderTable class may set our undo worker id.
    friend class UT_UndoWorkerFinderTable;

    void	*myOpaqueUndoWorker;
    int		 myUndoWorkerId;
};


// This templated class is a convenience class.  Its methods are all inlined.
template <class UndoWorker>
class UT_UndoWorkerFinder : public UT_UndoWorkerFinderBase
{
public:
    UT_UndoWorkerFinder(UndoWorker &undo_worker)
	: UT_UndoWorkerFinderBase(&undo_worker) {}

private:
};


#endif
