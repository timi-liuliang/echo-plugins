/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_HoldingQueue.C	(TIL Library, C++)
 *
 * COMMENTS:
 *	Queue with constant time head addition, tail removal and
 *	internal removal.
 */
#ifndef TIL_HOLDING_QUEUE_H
#define TIL_HOLDING_QUEUE_H

#include "TIL_API.h"
class TIL_HoldingQueue;

class TIL_API TIL_HoldingNode
{
public:
    TIL_HoldingNode() : myPrev(0), myNext(0), myQueue(0) { }

    TIL_HoldingNode	*&prev() { return myPrev; }
    TIL_HoldingNode	*&next() { return myNext; }
    
    TIL_HoldingNode	*prev() const { return myPrev; }
    TIL_HoldingNode	*next() const { return myNext; }

    void		setQueue(TIL_HoldingQueue *q, bool update_queues=true);
    TIL_HoldingQueue	*queue() const { return myQueue; }
    
private:
    TIL_HoldingNode  *myPrev;
    TIL_HoldingNode  *myNext;
    TIL_HoldingQueue *myQueue;
};


class TIL_API TIL_HoldingQueue
{
public:
     TIL_HoldingQueue() : myHead(0), myTail(0), myEntries(0) {} 
    ~TIL_HoldingQueue();

    // adds to head.
    void		add(TIL_HoldingNode *newtile);

    // removes an arbitrary tile.
    void		remove(TIL_HoldingNode *tile);

    // returns the last node off at the tail & removes it from the queue.
    TIL_HoldingNode *	pop();
    
    // queries.
    TIL_HoldingNode *	head() { return myHead; }
    bool		isEmpty() const { return myHead == 0; }
    unsigned int	entries() const { return myEntries; }

    // clears out the queue and deletes the holding nodes if delete_nodes is
    // true.
    void		empty(bool delete_nodes = true);

    int			traverse(int (*tfunc)(TIL_HoldingNode *, void *),
				 void *data);
private:
    TIL_HoldingNode *myHead;
    TIL_HoldingNode *myTail;
    unsigned int     myEntries;
};


#endif

