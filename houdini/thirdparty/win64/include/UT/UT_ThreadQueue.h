/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ThreadQueue.h ( UT Library, C++)
 *
 * COMMENTS:
 *	Modelled after Python's Queue, this is a simple queue
 *	which you can use to communicate to another thread with.
 */

#ifndef __UT_ThreadQueue__
#define __UT_ThreadQueue__

#include "UT_API.h"
#include "UT_Thread.h"
#include "UT_Condition.h"
#include "UT_RingBuffer.h"

///
/// UT_ThreadQueue
///
/// Designed to be templated on a POD.
///
template <typename T>
class UT_ThreadQueue
{
public:
    /// A max cost of -1 will never block on adding items to the queue.
    UT_ThreadQueue(exint maxcost = -1) 
    { 
	myMaxCost = maxcost; 
	myCurrentCost = 0; 
    }
    ~UT_ThreadQueue() {}

    /// Synchronized test for number of entries
    int			entries() const
    {
	UT_AutoLock	l(myLock);

	return myList.entries();
    }

    /// Removes an item, returns false if failed to remove because
    /// the queue was empty.
    bool		remove(T &item)
    {
	UT_AutoLock	l(myLock);

	if (isEmpty())
	    return false;
	item = myList.popFirst();
	exint cost = myCost.popFirst();
	myCurrentCost -= cost;

	// Let people know it may now be empty.
	myCond.triggerAll();
	return true;
    }

    /// Blocks until the item is ready.
    T			waitAndRemove()
    {
	UT_AutoLock	l(myLock);
	T		result;

	while (!remove(result))
	{
	    myCond.waitForTrigger(myLock);
	}
	return result;
    }

    /// Blocks until the queue is empty.
    /// May not still be empty, since someone could add after
    /// you reach empty!
    void		waitForEmpty()
    {
	UT_AutoLock	l(myLock);

	while (!isEmpty())
	{
	    myCond.waitForTrigger(myLock);
	}
    }

    /// Blocks until a queue change occurs, returns the new queue size.
    /// Returns zero immediately if queue is empty.
    int			waitForQueueChange()
    {
	UT_AutoLock	l(myLock);

	if (isEmpty())
	    return 0;

	myCond.waitForTrigger(myLock);
	return myList.entries();
    }

    /// Adds the item to the queue.
    /// Will block if it the total cost would be exceeded by
    /// adding this to the queue.
    /// Zero cost never block, nor does a max cost of -1
    void		append(const T &item, exint cost = 0)
    {
	UT_AutoLock	l(myLock);

	// Check if we'll exceed the cost.
	while (1)
	{
	    // IF we are empty, add even if too expensive!
	    if (cost && myMaxCost >= 0 && (myCurrentCost > 0) && (myCurrentCost + cost > myMaxCost))
	    {
		// Too expensive to add right now, so await a trigger.
		myCond.waitForTrigger(myLock);
	    }
	    else
	    {
		// Enough room for the new item.
		break;
	    }
	}

	// Append
	myList.push(item);
	myCost.push(cost);
	myCurrentCost += cost;

	// Alert anyone waiting to wake up.
	myCond.triggerAll();
    }

protected:
    /// Non-synchronized, do not call outside of the lock.
    bool		isEmpty() const
    {
	return !myList.entries();
    }

private:
    /// Prohibit copying as I am not sure if lock semantics will work.
    UT_ThreadQueue(const UT_ThreadQueue<T> &ref) {}
    UT_ThreadQueue<T> &operator=(const UT_ThreadQueue<T> &ref) {}

    UT_RingBuffer<T>	myList;
    UT_RingBuffer<exint>	myCost;
    exint		myMaxCost;
    exint		myCurrentCost;

    mutable UT_Lock	myLock;
    UT_Condition	myCond;
};

#endif


