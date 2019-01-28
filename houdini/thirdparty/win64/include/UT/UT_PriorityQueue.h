/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This is a class template that implements a resizable array of 
 *      arbitrary objects. The template parameter represents the type
 *	of object, and is called utPtr. You can instantiate this class
 *	with any object or pointer, such as:
 *		UT_ValArray<GeoPoint*>		pObj;
 *	The utCompare class should be a class with a very simple constructor
 *	which provides a single method "isLess" which compares two utPtr's.
 */

#ifndef __UT_PriorityQueue_h__
#define __UT_PriorityQueue_h__

#include "UT_Array.h"

/// NOTE: UT_PriorityQueue and std::priority_queue are *max* heaps, so the
///       comparison direction must be *opposite* that for sorting an array.
template <class utPtr, class utCompare=std::less<utPtr>,
			bool need_changed_position=false>
class UT_PriorityQueue
{
public:
    /// Trivial constructor and destructor:
    explicit UT_PriorityQueue(unsigned int sz=0,
                              const utCompare &compare = utCompare())
	: myArray(sz)
        , comparator(compare)
    { }
    virtual ~UT_PriorityQueue() {}

    /// Copy constructor that uses operator '=' to assign each of a's Things
    /// to this array.
    UT_PriorityQueue(const UT_PriorityQueue<utPtr, utCompare,
                                            need_changed_position> &a)
        : myArray(a.myArray)
        , comparator(a.comparator)
    { }

    unsigned int	insert(utPtr t)
		        {
			    unsigned int	temp;
			    temp = myArray.append(t);
			    if (need_changed_position)
				changedPosition(myArray[temp], temp);
			    return bubbleUp(temp);
			}
    unsigned int	append(utPtr t)
			{
			    return myArray.append(t);
			}


    /// calls remove before destroying
    void		destroy(unsigned int entry)
		        {
			    utPtr ptr = myArray[entry];
			    remove(entry);
			    delete ptr;
			}

    void		remove(unsigned int entry)
		        {
			    if (myArray.size() > 1)
			    {
				myArray[entry] = myArray.last();
				if (need_changed_position)
				    changedPosition(myArray[entry], entry);
				myArray.setSize(myArray.size()-1);
				bubbleDown(entry);
			    }
			    else
			    {
				myArray.setSize(0);
			    }
			}

    void		clear()
			{
			    myArray.clear();
			}

    exint		size() const	{ return myArray.size(); }
    exint		entries() const	{ return myArray.size(); }

    int64		getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myArray.getMemoryUsage(false);
        return mem;
    }

    bool		isEmpty() const	{ return myArray.isEmpty(); }

    // make sure is queue is not empty
    const utPtr		&head() const { return myArray[0]; };

    // In almost all cases, you only want to pull from the head of the
    // queue.  But, to make the remove(unsigned int entry) sensible,
    // you must be able to query.
    const utPtr		&getEntry(int idx) const { return myArray[idx]; };
		    

    // . allows nodes to keep track of where they are in the queue
    // .. this way nodes can be placed in two queues and delete from both
    // .. and allows deleting with just a node pointer
    virtual void	changedPosition(utPtr /*a*/,
					unsigned int /*newposition*/ ) const
			{ }

    unsigned int	bubbleDown(unsigned int entry)
			{
			    for ( ;; )
			    {
				unsigned int	l = leftChild(entry);
				unsigned int	r = l+1;
				unsigned int	newplace;

				if (nodeExists(r))	// both children exist
				{
				    if (comparator(myArray[entry], myArray[l]))
				    {
					if (comparator(myArray[l], myArray[r]))
					    newplace = r;
					else
					    newplace = l;
				    }
				    else if (comparator(myArray[entry],
							 myArray[r]))
				    {
					newplace = r;
				    }
				    else break;

				}
				else if(nodeExists(l)) // just left child
				{
				    if (comparator(myArray[entry], myArray[l]))
					newplace = l;
				    else break;
				}
				else	// no children
				{
				    break;
				}

				swap(entry, newplace);
				entry = newplace;
			    }
			    return entry;
			}

    unsigned int	bubbleUp(unsigned int entry)
			{
			    while (entry)	// while not root
			    {
				int	nextentry = parent(entry);
				if (!comparator(myArray[entry],
						 myArray[nextentry]))
				{
				    swap(entry, nextentry);
				    entry = nextentry;
				}
				else
				{
				    break;
				}
			    }
			    return entry;
			}

private:
    static unsigned int	parent(unsigned int node)
			{
			    return (node-1)>>1;
			}

    static unsigned int	leftChild(unsigned int node)
			{
			    return (node<<1) + 1;
			}

    static unsigned int	rightChild(unsigned int node)
			{
			    return (node<<1) + 2;
			}

    bool		nodeExists(unsigned int node)
			{
			    return node < myArray.size();
			}

    void		swap(unsigned int a, unsigned int b)
			{
			    utPtr		temp = myArray[a];
			    myArray[a] = myArray[b];
			    myArray[b] = temp;

			    if (need_changed_position)
			    {
				changedPosition(myArray[a], a);
				changedPosition(myArray[b], b);
			    }
			}

protected:
    // In almost all cases, you probably want to extract the queue in sorted
    // order.  However, some sub-classes may not require this, so we make this
    // protected so that the sub-classes can access the queue without the
    // sorting requirement (see IMG3D_Photon).
    UT_Array<utPtr>	myArray;
    const utCompare	comparator;
};

/// Based on ut_KDPQueueType (UT_KDTree.C)
/// This class is a priority queue ordered so that entries are always pulled
/// off in increasing order of a float parameter.
template <typename T>
class UT_MinQueue
{
private:
    class QEntry
    {
    public:
	QEntry() {}
	QEntry(T node, float d)
	    : myNode(node)
	    , myDistance(d) {}

	T	 myNode;
	float	 myDistance;

	bool operator<(const QEntry &b) const
	{
	    return myDistance > b.myDistance;
	}
    };

    class QCompare {
    public:
	bool operator()(const QEntry &a, const QEntry &b) const
	{ return a < b; }
    };

public:
    // Add an entry to the queue
    void	insert(T node, float d)
		{
		    myQueue.insert(QEntry(node, d));
		}

    // Pull of the min entry
    bool	pullOff(T &node, float &d)
		{
		    if (myQueue.size())
		    {
			node = myQueue.getEntry(0).myNode;
			d = myQueue.getEntry(0).myDistance;
			myQueue.remove(0);
			return true;
		    }
		    return false;
		}

    void	clear()
		{ myQueue.clear(); }

private:
    UT_PriorityQueue<QEntry, QCompare, false>	myQueue;
};

#endif
