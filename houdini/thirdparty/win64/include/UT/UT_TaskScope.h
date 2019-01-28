/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TaskScope.h (UT Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __UT_TASKSCOPE_H_INCLUDED__
#define __UT_TASKSCOPE_H_INCLUDED__

#include "UT_API.h"
#include "UT_Debug.h"
#include "UT_NonCopyable.h"
#include "UT_SmallObject.h"
#include "UT_Thread.h"
#include "UT_ThreadSpecificValue.h"
#include "UT_PerformanceThread.h"

// Set the following line to #if 1 to enable debug message output
#if 0
    #define UT_TASKSCOPE_DBG(ZZ)	UT_DBGOUT(ZZ)
#else
    #define UT_TASKSCOPE_DBG(ZZ)
#endif

// Invalid Task ID
#define UT_INVALID_TASK_ID   0

// Forward declarations
class UT_TaskScope;
template <typename T> class UT_ValArray;

/// Array of UT_TaskScope pointers
typedef UT_ValArray<const UT_TaskScope *>   UT_TaskScopePtrArray;

/// Scope object which defines a thread task, maintaining a parent-child
/// hierarchy.
class UT_API UT_TaskScope : public UT_SmallObject<UT_TaskScope>, UT_NonCopyable
{
public:
    explicit UT_TaskScope(const UT_TaskScope *parent, int thread)
	: myParent(parent)
    {
	myThreadId = thread;
	myOldTaskScope = theTaskScope.getValueForThread(myThreadId);
	theTaskScope.getValueForThread(myThreadId) = this;
	UT_TASKSCOPE_DBG(("Task %p parent %p thread %d",
			  this, parent, myThreadId));

	// Notify the performance monitor that a threaded task has started.
	// It will return -1 if we don't need to be recording thread stats.
	myPerfMonEventId = UTperformanceStartTaskScope(this);
    }
    explicit UT_TaskScope(const UT_TaskScope *parent)
	: myParent(parent)
    {
	myThreadId = SYSgetSTID();
	myOldTaskScope = theTaskScope.getValueForThread(myThreadId);
	theTaskScope.getValueForThread(myThreadId) = this;
	UT_TASKSCOPE_DBG(("Task %p parent %p thread %d",
			  this, parent, myThreadId));

	// Notify the performance monitor that a threaded task has started.
	// It will return -1 if we don't need to be recording thread stats.
	myPerfMonEventId = UTperformanceStartTaskScope(this);
    }
    ~UT_TaskScope()
    {
	// Notify the performance monitor that a threaded task has ended.
	if (myPerfMonEventId >= 0)
	    UTperformanceStopTaskScope(this, myPerfMonEventId);

	theTaskScope.getValueForThread(SYSgetSTID()) = myOldTaskScope;
	UT_TASKSCOPE_DBG(("Task %p parent %p ended", this, myParent));
    }

    /// Return the parent of this task scope
    const UT_TaskScope *	getParent() const { return myParent; }

    /// Return the id of the assigned thread.
    int				getThreadId() const { return myThreadId; }

    /// Obtain the current task scope from thread local storage.
    /// @note The return value is NULL if there is no current task scope.
    // @{
    static const UT_TaskScope *	getCurrent(int thread)
    {
	return theTaskScope.getValueForThread(thread);
    }
    static const UT_TaskScope *	getCurrent()
    {
	return getCurrent(SYSgetSTID());
    }
    // @}

    /// Obtain the current task scope from thread local storage, creating one
    /// if there is none.
    // @{
    static const UT_TaskScope & getOrCreateCurrent(int thread)
    {
	const UT_TaskScope * &task = theTaskScope.getValueForThread(thread);

	if (task == NULL)
	    task = new UT_TaskScope(NULL, thread);
	return *task;
    }
    static const UT_TaskScope & getOrCreateCurrent()
    {
	return getOrCreateCurrent(SYSgetSTID());
    }
    // @}

    /// Obtain the root task scope from thread local storage, creating one
    /// if there is none.
    // @{
    static const UT_TaskScope & getOrCreateRoot(int thread)
    {
	const UT_TaskScope * &task = theTaskScope.getValueForThread(thread);

	if (task == NULL)
	    task = new UT_TaskScope(NULL, thread);

	const UT_TaskScope *root = task;

	while (root->getParent())
	    root = root->getParent();
	return *root;
    }
    static const UT_TaskScope & getOrCreateRoot()
    {
	return getOrCreateRoot(SYSgetSTID());
    }
    // @}

    /// Test if the given task scope is an ancestor of ours
    bool isAncestor(const UT_TaskScope &parent) const
    {
	for (const UT_TaskScope *task = getParent();
	     task != NULL; task = task->getParent())
	{
	    if (task == &parent)
		return true;
	}
	return false;
    }

private:
    const UT_TaskScope *    myParent;
    const UT_TaskScope *    myOldTaskScope;
    int			    myThreadId;
    int			    myPerfMonEventId;

    typedef UT_ThreadSpecificValue<const UT_TaskScope *> ut_TaskScopeTLS;
    static ut_TaskScopeTLS  theTaskScope;
};

#endif // __UT_TASKSCOPE_H_INCLUDED__
