/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_CookTash.h (OP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OP_COOKTASK_H_INCLUDED__
#define __OP_COOKTASK_H_INCLUDED__

#include "OP_API.h"
#include "OP_Error.h"
#include "OP_Node.h"

#include <UT/UT_Assert.h>
#include <UT/UT_Debug.h>
#include <UT/UT_Thread.h>
#include <UT/UT_Task.h>
#include <UT/UT_TaskState.h>

class OP_Node;

class OP_API OP_CookTask : public UT_Task
{
public:
    OP_CookTask(OP_Context &context, OP_Node &node)
	: myContext(context)
	, myNode(node)
	, myDoneSpawning(false)
    {
	myContext.setCookTask(this);
    }

    static void
    spawnRootTaskAndWait(OP_Context &context, OP_Node &node)
    {
	UT_Task *root;

	root = new (allocate_root()) OP_CookTask(context, node);
	UT_Task::spawnRootAndWait(*root);
    }
    static OP_CookTask *
    createRootTask(OP_Context &context, OP_Node &node)
    {
	return new (allocate_root()) OP_CookTask(context, node);
    }

    void
    spawnChildTask(OP_Context &context, OP_Node &node)
    {
	UT_Task *child;

	UT_ASSERT(!myDoneSpawning);
	child = new (allocate_child()) OP_CookTask(context, node);
	incrementRefCount();
	spawnChild(*child);
    }
    void
    spawnWaitTask(OP_Node &node)
    {
	// Someone is already cooking the given node. Spawn a wait task as our
	// child. When the input is marked as done, the current task will
	// become runnable.
	UT_ASSERT(!myDoneSpawning);
	incrementRefCount();
	node.taskState().addWaitingTask(*this);
    }

    virtual UT_Task *	run();

private:
    OP_Context		myContext;
    OP_Node &		myNode;
    bool		myDoneSpawning;
};

#endif // __OP_COOKTASK_H_INCLUDED__
