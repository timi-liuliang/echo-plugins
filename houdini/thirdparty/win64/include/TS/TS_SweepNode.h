/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TS_SweepNode.h ( TS Library, C++)
 *
 * COMMENTS:	Classes to help with ray-intersection.  In preparing for
 *		ray-intersection, each node has the opportunity to build a
 *		sweep list.
 *
 * Pre-Process:
 *   The sweep root finder will ask the expression to build a sweep root list.
 *   This will go through all it's children and:
 *	a)  Test each node for intersection with the ray (i.e. ray-prep)
 *	b)  Add the primitive to a sorted sweep list for the expression
 *
 * Root Finding:
 *   - The sweep root finder needs to find the closest starting point for the
 *     ray-intersection processing.  So, it asks the expression.
 *   - The expression takes it's first inactive sweep node and asks it for it's
 *     closest node (this potentially recurses through additional expressions)
 *   - The sweeper then tells the expression to cull any active sweep nodes and
 *     add any nodes it needs.
 *
 * The sweep nodes are stored in a non-circular list (i.e. terminated by nulls
 * at both ends).
 *
 */

#ifndef __TS_SweepNode__
#define __TS_SweepNode__

#include "TS_API.h"
class TS_MetaExpression;

class TS_API TS_SweepNode
{
public:
    // Before calling any of these functions with a non-negative thread
    // number, the thread pools MUST be initialized with a call to
    // setupThreads
    static TS_SweepNode	*allocNode(TS_MetaExpression *prim, float t0, float t1,
				   int threadNum = -1);
    static void		 freeList(TS_SweepNode *node, TS_SweepNode *last=0,
				  int threadNum = -1);
    static void		 freeNode(TS_SweepNode *node, int threadNum = -1);
    static void		 destroyMemory();

    // Initializes multiple sweep node object pools to be used
    // by multiple threads
    static void		 setupThreads(int numThreads);

    TS_MetaExpression	*getPrim() const	{ return myPrim; }
    float		 getT0() const		{ return myT0; }
    float		 getT1() const		{ return myT1; }

    TS_SweepNode	*getNext()	{ return myNext; }

    // Both of these methods return the new head
    TS_SweepNode	*linkAtHead(TS_SweepNode *head);
    TS_SweepNode	*unlink(TS_SweepNode *head);

    // This function is used when we get the sorted list of sweep nodes.  We
    // can quickly build the links without doing it one by one.
    void		 setLinks(TS_SweepNode *prev, TS_SweepNode *next)
			 {
			     myPrev = prev;
			     myNext = next;
			 }

private:
    TS_SweepNode	*myPrev, *myNext;
    TS_MetaExpression	*myPrim;
    float		 myT0, myT1;
};

#endif
