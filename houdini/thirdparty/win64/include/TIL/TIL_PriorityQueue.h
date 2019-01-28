/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_HoldingQueue.h	(TIL Library, C++)
 *
 * COMMENTS:
 *	A 'queue' with multiple queues inside, each of which bucket a
 *      priority range.
 */
#ifndef TIL_PRIORITY_QUEUE_H
#define TIL_PRIORITY_QUEUE_H

#include "TIL_API.h"
#include "TIL_HoldingQueue.h"

typedef TIL_HoldingNode TIL_PNode;


class TIL_API TIL_PriorityQueue
{
public:

    // sorts nodes into 'nbuckets' queues, each of which cover
    // consequetive priority ranges of 'bucket_range'
    // if any bucket other than the lowest overflows the 'max_per_bucket',
    // it will be passed down to a lower bucket.
    
		 TIL_PriorityQueue(unsigned int nbuckets,
				   unsigned int bucket_range,
				   unsigned int max_entries);
		~TIL_PriorityQueue();


    void	add(TIL_PNode *node, unsigned int priority);
    void	remove(TIL_PNode *node);

    // returns the last node in the lowest priority non-empty queue
    TIL_PNode  *pop();

    bool	isEmpty() const;

    void	empty(bool delete_nodes = true);

    // both of these only set the values; they only affect funture additions.
    void	 setPriorityRange(unsigned int bucket_range);
    unsigned int getPriorityRange() const { return myBucketPriority; }

    // sets the max # of elements assumed to be in the cache, so that the
    // bucket sizes can be assigned appropriately.
    void	 setMaxSize(unsigned int size);
    unsigned int getMaxBucketSize(int bn) const { return myMaxBucketSize[bn]; }

    unsigned int getNumBuckets() const { return myNumBuckets; }
    unsigned int getBucketSize(int bucket) const;
    
    int		 traverse(int (*tfunc)(TIL_PNode *, void *), void *data);
    
private:
    TIL_HoldingQueue	*myBuckets;
    unsigned int	*myMaxBucketSize;
    unsigned int	 myNumBuckets;
    unsigned int	 myBucketPriority;
};

#endif
