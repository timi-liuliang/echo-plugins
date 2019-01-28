/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_AutoLockInputs.h ( OP Library, C++)
 *
 * COMMENTS:	Lock and automatically unlock inputs when cooking an operator.
 */

#ifndef __OP_AutoLockInputs__
#define __OP_AutoLockInputs__

#include "OP_API.h"
#include "OP_Error.h"
#include <UT/UT_BitArray.h>
#include <SYS/SYS_Types.h>

class OP_Context;
class OP_Node;

class OP_API OP_AutoLockInputs
{
public:
    /// Constructor accepting a node.
    /// NOTE: This does *NOT* lock the inputs!  Call lock().
    /// Must call setNode() before locking if NULL.
    OP_AutoLockInputs(OP_Node *node = NULL)
    {
        myNode = node;
    }

    /// Destructor unlocks any locked inputs.
    ~OP_AutoLockInputs()
    {
        unlock();
    }

    /// Unlocks any locked inputs and sets the node.
    void setNode(OP_Node *node)
    {
        unlock();
        myNode = node;
        if (myLockedInputs.size() > 0)
            myLockedInputs.setSize(0);
    }

    /// Locks all inputs
    OP_ERROR lock(OP_Context &context);

    /// Locks a single input
    OP_ERROR lockInput(exint input, OP_Context &context);

    /// Returns the node associated with this auto-unlocker.
    OP_Node *getLockedNode() const
    { return myNode; }

    /// Returns a bit array of successfully locked inputs.
    const UT_BitArray &getLockedInputs() const
    { return myLockedInputs; }

    /// Explicitly unlock all locked inputs.
    /// NOTE: Only call this directly if you need to unlock early!
    void unlock();

    /// Explicitly unlock the specified locked input.
    /// NOTE: Only call this directly if you need to unlock early!
    void unlockInput(exint input);

    /// Explicilty marks the given input as unlocked.  This is used
    /// if another function outside of the auto-lock has explicilty
    /// unlocked that input.
    void markInputUnlocked(exint input);

private:
    OP_Node     *myNode;
    UT_BitArray  myLockedInputs;
};

#endif
