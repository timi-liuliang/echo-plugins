/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This class implements a doubly linked list that maintains
 *	a replacement list of nodes.
 *
 */

#ifndef __UT_LinkList_h__
#define __UT_LinkList_h__

#include "UT_API.h"
#include "UT_Swap.h"
#include <iosfwd>

class UT_API UT_LinkNode {
public:
    // Constructor and destructor:
    UT_LinkNode(void)	{ predNode = succNode = 0; }
    virtual ~UT_LinkNode(void);

    // Query/set methods:
    UT_LinkNode	       *&prev() { return predNode; }
    UT_LinkNode	       *&next() { return succNode; }
    const UT_LinkNode	*prev() const { return predNode; }
    const UT_LinkNode	*next() const { return succNode; }

    // I/O friends:
    friend std::ostream	&operator<<(std::ostream &os, const UT_LinkNode &n)
    {
	n.outTo(os);
	return os;
    }

protected:
    // Query the name of this class:
    virtual const char	*className(void) const;

    // Our own I/O methods, so we can redefine them in derived classes:
    virtual void outTo (std::ostream &os) const;

private:
    // Pointers to predecessor and successor respectively:
    UT_LinkNode	*predNode;
    UT_LinkNode	*succNode;
};


class UT_API UT_LinkList {
public:
    // Trivial class constructor and destructor. The destructor 
    // deallocates every node in the active and replacement lists.
    UT_LinkList(void)		{ first=last=stashHead=0; count=0; }
    virtual	~UT_LinkList(void);

    void swap( UT_LinkList &other );

    // Reverse the order of the active nodes:
    virtual void reverse(void);

    // Add nodes to the list, or delete them: 
    // append() appends a node, insert() inserts it at index 'where';
    // remove() deletes the node from the list and leaves it intact;
    // destroy() deletes the node from the list and deallocates it.
    // If the node is not in the list, or if nodeIdx is invalid, 
    // both remove() and destroy() return 0. If the node is 0,
    // remove() and destroy() will act upon the last element in the 
    // list. None of the functions below checks whether the node
    // is or is not in the list at the time it is deleted or added.
    // To check for existence, see inList().
    void	 append (UT_LinkNode *node);
    void	 insert (UT_LinkNode *node, int where);
    // node is the node to insert, before and after are the node already
    // in the list that you wish to insert before/after.
    void	 insertBefore(UT_LinkNode *node, UT_LinkNode *before);
    void	 insertAfter(UT_LinkNode *node, UT_LinkNode *after);

    UT_LinkNode	*remove (UT_LinkNode *node = 0);
    UT_LinkNode	*remove (int nodeIdx);
    void	 destroy(UT_LinkNode *node = 0);
    void	 destroy(int nodeIdx);

    // Take a node from the active list and stash it into the 
    // replacement list. recall() performs the inverse procedure.
    // If the node parameter is 0, stash and recall will act upon
    // the last elemenent added to the respective list.
    UT_LinkNode	*stash (UT_LinkNode	*node = 0);
    UT_LinkNode	*stash (int		 nodeIdx);
    UT_LinkNode	*recall(UT_LinkNode	*node = 0);
    UT_LinkNode	*recall(UT_LinkNode	*node, int nodeIdx);

    // Return the index of the node in the list or vice-versa.
    // The first method returns -1 if not found; the 2nd method 
    // returns 0 if unsuccessful.
    int		 find(UT_LinkNode *node) const;
    UT_LinkNode	*find(int nodeIdx) const;

    // Check if the node is in the active list: return 1 if true, o if
    // false.
    int		 inList(UT_LinkNode *node) const;


    // Return the predecessor or the successor of a node, or the 
    // first or last node in the list respectively.
    UT_LinkNode	*prev(UT_LinkNode *node) const	{ return node->prev(); }
    UT_LinkNode	*next(UT_LinkNode *node) const	{ return node->next(); }
    UT_LinkNode	*head() const			{ return first; }
    UT_LinkNode	*tail() const			{ return last;  }

    // Some iteration functions, in case you are not using the separate 
    // iterator class:
    UT_LinkNode	*iterateInit() const { return first; }
    UT_LinkNode	*iterateNext(UT_LinkNode *curr) const
		 {
		    return curr ? curr->next() : first;
		 }
    UT_LinkNode	*iteratePrev(UT_LinkNode *curr) const
		 {
		    return curr ? curr->prev() : last;
		 }
    UT_LinkNode	*iterateFastNext(UT_LinkNode *curr) const {return curr->next();}
    UT_LinkNode	*iterateFastPrev(UT_LinkNode *curr) const {return curr->prev();}

    // Clear the linked list: the list of stashed nodes, the active 
    // list, or both.
    void	 clearStashed();
    void	 clearActive();
    void	 clear()	{ clearActive(); clearStashed(); }

    // Return the length of the list or just find out whether it's empty.
    int		 length() const		{ return count; }
    int		 isEmpty() const	{ return count == 0; }

    // Apply a user-defined function to each element of the linked 
    // list, as long as the function returns zero. If applyFct returns
    // 0, apply() stops traversing the list and returns the current
    // node; otherwise, apply() returns 0.
    UT_LinkNode	*apply(int (*applyFct)(UT_LinkNode *n, void *d), void *d);

    // Copy the other list's data into us. The source gets cleared.
    UT_LinkList	&operator=(UT_LinkList &src);

    // Append the other list's data to ours.  The source gets cleared.
    UT_LinkList &operator+=(UT_LinkList &src);

    // I/O friends:
    friend std::ostream	&operator<<(std::ostream &os, const UT_LinkList &v)
    {
	v.outTo(os);
	return os;
    }

protected:
    // Current node of iteration, in case you are not using the iterator class.
    // Remove a node from the replacement list. Remove the last
    // one inserted if 0.
    UT_LinkNode		*unStash(UT_LinkNode *node = 0);

    // Query the stash head:
    UT_LinkNode		*firstStash() const	{ return stashHead; }

    // Query the name of this class:
    virtual const char	*className(void) const;

    // Our own I/O methods, so we can redefine them in derived classes:
    virtual void	 outTo (std::ostream &os) const;

    // Head of the stash list.
    UT_LinkNode	*stashHead;

    // Just reset the state of the link list
    // Not, this will cause memory leaks unless used with caution,
    // ie. list sorting
    void	 reset()	{ first = last = 0; count = 0; }


private:

    // DATA

    // First and last nodes in the active list.
    UT_LinkNode	*first;
    UT_LinkNode	*last;

    // Number of nodes in the active list.
    int		 count;
};

UT_SWAPPER_CLASS( UT_LinkList )

#endif
