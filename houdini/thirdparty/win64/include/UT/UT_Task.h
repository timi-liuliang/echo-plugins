/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Task.h (UT Library, C++)
 *
 * COMMENTS:	Wrapper for tbb::task
 */

#ifndef __UT_TASK_H_INCLUDED__
#define __UT_TASK_H_INCLUDED__

#include "UT_API.h"
#include "UT_TaskScope.h"
#include <tbb/task.h>
#include <tbb/task_group.h>

class UT_TaskList;

class UT_API UT_Task : private tbb::task
{
    typedef tbb::task	me;

public:
    UT_Task()
	: myParentScope(NULL)
    {
    }

    void setParentScope(const UT_TaskScope *parent)
    {
	myParentScope = parent;
    }

    void setRefCount(int count)
    {
	me::set_ref_count(count);
    }
    void incrementRefCount()
    {
	me::increment_ref_count();
    }
    int decrementRefCount()
    { 
	return me::decrement_ref_count();
    }

    void recycleAsContinuation()
    {
	me::recycle_as_continuation();
    }
    void recycleAsSafeContinuation()
    {
	me::recycle_as_safe_continuation();
    }

    static void spawnRootAndWait(UT_Task &task)
    {
	task.setParentScope(UT_TaskScope::getCurrent());
	me::spawn_root_and_wait(task);
    }
    void	enqueue()
    {
	setParentScope(UT_TaskScope::getCurrent());
	me::enqueue(*this);
    }
    void spawnChild(UT_Task &task)
    {
	task.setParentScope(myParentScope);
	me::spawn(task);
    }

    /// Should be overriden by derived classes.
    /// Returns the next task to run, NULL if none.
    virtual UT_Task *	run() = 0;

    // Methods imported from tbb::task
    using me::allocate_root;
    using me::allocate_child;

    void cancelGroupExecution()
    {
	me::cancel_group_execution();
    }
    bool isCancelled() const
    {
	return me::is_cancelled();
    }

private:

    // Wrapper so that we maintain the UT_TaskScope hierarchy for UT_TaskLock
    virtual UT_Task *	execute()
			{
			    UT_TaskScope scope(myParentScope);
			    return run();
			}

    friend class	UT_TaskList;

private:

    const UT_TaskScope *    myParentScope;
};


class UT_API UT_EmptyTask : public UT_Task
{
public:
    UT_Task *run()
    {
	return NULL;
    }
};

class UT_API UT_TaskList : private tbb::task_list
{
    typedef tbb::task_list me;
public:

    /// Returns true when number of tasks is non-zero
    bool	isEmpty() const
		{
		    return me::empty();
		}

    /// Removes all tasks
    void	clear()
		{
		    myList.clear();
		    me::clear();
		}

    /// Append a task
    void	append(UT_Task &task)
		{
		    myList.append(&task);
		    me::push_back(task);
		}

    /// Spawn this task list as root tasks and wait
    void	spawnRootAndWait()
		{
		    const UT_TaskScope *parent = UT_TaskScope::getCurrent();
		    for (int i = 0; i < myList.entries(); i++)
			myList(i)->setParentScope(parent);

		    tbb::task::spawn_root_and_wait(*this);
		}

    /// Run this task list in serial
    void	runSerial()
		{
		    for (int i = 0; i < myList.entries(); i++)
			myList(i)->run();
		}

private:
    UT_ValArray<UT_Task *>  myList;
};

class UT_API UT_TaskGroupContext : private tbb::task_group_context
{
    typedef tbb::task_group_context me;

public:

    enum KindType
    {
        ISOLATED = me::isolated,
        BOUND = me::bound
    };

		UT_TaskGroupContext(KindType relation_to_parent = BOUND)
		    : task_group_context((kind_type)relation_to_parent)
		{
		}

    void	reset()
		{
		    me::reset();
		}

    bool	cancelGroupExecution()
		{
		    return me::cancel_group_execution();
		}

    bool	isGroupExecutionCancelled() const
		{
		    return me::is_group_execution_cancelled();
		}
};

class UT_API UT_TaskGroup : private tbb::task_group
{
    typedef tbb::task_group	me;

public:
    UT_TaskGroup()
	: tbb::task_group()
    {
    }
    ~UT_TaskGroup()
    {
    }

    template <typename F>
    void	run(const F &f)
		{
		    me::run(f);
		}

    template <typename F>
    void	run(tbb::task_handle<F> &f)
		{
		    me::run(f);
		}

    template <typename F>
    void	runAndWait(const F &f)
		{
		    me::run_and_wait(f);
		}

    template <typename F>
    void	runAndWait(tbb::task_handle<F> &f)
		{
		    me::run_and_wait(f);
		}

    // American English
    bool	isCanceling()	{ return me::is_canceling(); }
    // British English
    bool	isCancelling()	{ return me::is_canceling(); }

    tbb::task_group_status	wait()		{ return me::wait(); }
    void			cancel()	{ me::cancel(); }
};

#endif // __UT_TASK_H_INCLUDED__
