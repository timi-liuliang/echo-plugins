/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_UndoWorkerFinderTable.h
 *
 * COMMENTS:
 *	This class maintains a table of undo worker finders.  See
 *	UT_UndoWorkerFinder.h for comments on how this class works.
 */

#ifndef __UT_UndoWorkerFinderTable_h__
#define __UT_UndoWorkerFinderTable_h__

#include "UT_API.h"
#include "UT_Map.h"
class UT_UndoWorkerFinderBase;

class UT_API UT_UndoWorkerFinderTable
{
public:
    static UT_UndoWorkerFinderTable	&instance();

    void	 addWorkerFinder(UT_UndoWorkerFinderBase &worker_finder);
    void	 removeWorkerFinder(UT_UndoWorkerFinderBase &worker_finder);
    void	*findOpaqueWorker(int undo_worker_id);

protected:
    UT_UndoWorkerFinderTable();
    ~UT_UndoWorkerFinderTable();

    static UT_UndoWorkerFinderTable	*ourInstance;

    UT_Map<int, UT_UndoWorkerFinderBase *> myTable;
    int					 myNextUniqueId;
};

#endif
