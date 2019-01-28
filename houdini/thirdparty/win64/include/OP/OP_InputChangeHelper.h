/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_InputChangeHelper.h (OP Library, C++)
 *
 * COMMENTS:	Helper class for keeping track of whether node inputs have
 *		changed.
 *
 * HOW TO USE THIS CLASS
 * =====================
 *
 * SETUP:
 *	- Override OP_Node::inputConnectChanged() to call
 *	  OP_InputChangeHelper::inputConnectChanged() as well. Don't forget to
 *	  delegate to the base class.
 *	- In the OP_Node::deleteCookedData() and OP_Node::bypassMe() overrides,
 *	  call OP_InputChangeHelper::resetChangedSourceFlags(). Any other calls
 *	  to delete the node's data should also call resetChangedSourceFlags().
 * 
 * Now, when you want to mark an input as being used, call useInput(). To query
 * if an input is dirty without clearing it, use checkChangedSourceFlags().
 */

#ifndef __OP_INPUTCHANGEHELPER_H__
#define __OP_INPUTCHANGEHELPER_H__

#include "OP_API.h"
#include <UT/UT_Assert.h>
#include <UT/UT_IntArray.h>
#include "OP_Node.h"

class OP_API OP_InputChangeHelper
{
public:

    /// Mark all inputs as dirty. This is the default.
    void resetChangedSourceFlags()
    {
	 myDuplicateCounts.setCapacity(0);
    }

    /// Mark the given input as dirty.
    /// NOTE: Users of this class should override
    ///	      OP_Node::inputConnectChanged() to call this.
    void inputConnectChanged(const OP_Node &node, int which_input)
    {
	if (which_input >= 0)
	    setDuplicateCount(which_input, -1, node.maxInputs());
	else
	    resetChangedSourceFlags();
    }

    /// Use the given input on the node. This updates source flags to mark the
    /// input as now clean, returning whether the input changes.
    /// If force is true, then we treat the input as always dirty.
    /// Returns false if input has errors and we'll add an input error in that
    /// case.
    bool useInput(const OP_Node &node, unsigned idx, bool &changed, bool force)
    {
	// checkChangedSourceFlags() returns if input has an error, "changed"
	// is updated.
	if (!checkChangedSourceFlags(node, idx, changed))
	    return false;

	if (force || changed)
	{
	    OP_Node *	src = node.getInput(idx);

	    setDuplicateCount(idx, src->getCookCount(), node.maxInputs());

	    changed = true; // we may have entered with (force == true)
	}
	// else not forced, and not changed

	return true;
    }

    /// Similar to useInput() except it doesn't affect the flags. It's used to
    /// simply query whether the given input has changed. Returns false if
    /// there's errors on the input. "changed" is updated.
    bool checkChangedSourceFlags(const OP_Node &node, unsigned idx,
				 bool &changed)
    {
	OP_Node	*   src;

	src = node.getInput(idx);
	if (!src || src->error() >= UT_ERROR_ABORT)
	{
	    changed = true;
	    return false;
	}

	changed = (src->getCookCount() != getDuplicateCount(idx));
	return true;
    }

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myDuplicateCounts.getMemoryUsage(false);
        return mem;
    }

private:
    int getDuplicateCount(int idx)
    {
	if (idx >= 0 && idx < myDuplicateCounts.entries())
	    return myDuplicateCounts(idx);
	return -1;
    }
    void setDuplicateCount(int idx, int count, int max_inputs)
    {
	if (idx >= 0 && idx < max_inputs)
	{
	    while (idx >= myDuplicateCounts.entries())
		myDuplicateCounts.append(-1);
	    myDuplicateCounts(idx) = count;
	}
	else    // clear all duplicate counts
	{
	    UT_ASSERT(!"Untested code path");
	    myDuplicateCounts.entries(0);
	}
    }

private:
    UT_IntArray	    myDuplicateCounts;
};

#endif // __OP_INPUTCHANGEHELPER_H__
