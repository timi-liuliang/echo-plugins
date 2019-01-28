/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_BidirectionalTree.h (UT Library, C++)
 *
 * COMMENTS:	A simple templatized intrusive n-ary tree class using
 *		doubly-linked parent and sibling pointers. It supports O(1)
 *		addition and removal from any tree node.  Tree nodes are
 *		automatically removed from the tree when deleted.
 *
 *		For a lighter singly-linked version, see UT_Tree.
 *
 *		*** To be extended as required by users of this class. ***
 *
 * USAGE:	Declare your tree node class like so:
 *
 *		    class MyType : public UT_BidirectionalTree<MyType>
 *
 */

#ifndef __UT_BIDIRECTIONALTREE_H__
#define __UT_BIDIRECTIONALTREE_H__

#include "UT_API.h"
#include <stddef.h>
#include "UT_Assert.h"

template <typename T>
class UT_BidirectionalTree
{
public:
    typedef UT_BidirectionalTree<T>  NodeType;

    UT_BidirectionalTree()
	: myParent(NULL)
	, myChildren(NULL)
	, myPrevSibling(NULL)
	, myNextSibling(NULL)
    {
    }
    virtual ~UT_BidirectionalTree()
    {
	unlinkAll();
    }

    /// Unlink this node from the tree completely.
    void unlinkAll()
    {
	setParent(NULL);
	removeAllChildren();
    }

    /// Set the parent of this node. If it already belongs to a tree, it will
    /// first be removed from it.
    void setParent(NodeType *parent)
    {
	if (myParent == parent)
	    return;

	if (myParent)
	{
	    myParent->removeChild(this);
	    UT_ASSERT(myParent == NULL);
	    UT_ASSERT(myPrevSibling == NULL);
	    UT_ASSERT(myNextSibling == NULL);
	}
	if (parent)
	{
	    parent->addChildToHead(this);
	    UT_ASSERT(myParent == parent);
	}
    }

    /// Add a child node to the head of the children list. O(1)
    void addChildToHead(NodeType *child)
    {
	UT_ASSERT(child->myParent == NULL);
	UT_ASSERT(child->myPrevSibling == NULL);
	UT_ASSERT(child->myNextSibling == NULL);

	child->myParent = this;
	child->setNextSibling(myChildren);
	myChildren = child;
    }

    /// Remove the given child node.
    void removeChild(NodeType *child)
    {
	NodeType *  old_prev;
	NodeType *  old_next;

	UT_ASSERT(child->myParent == this);

	if (child == myChildren)
	    myChildren = child->myNextSibling;

	child->myParent = NULL;
	old_prev = child->setPrevSibling(NULL);
	old_next = child->setNextSibling(NULL);
	if (old_prev)
	    (void) old_prev->setNextSibling(old_next);
	else if (old_next)
	    (void) old_next->setPrevSibling(old_prev);

	UT_ASSERT(myChildren == NULL || myChildren->myPrevSibling == NULL);
    }

    /// Remove all children from the tree.
    void removeAllChildren()
    {
	while (myChildren)
	    removeChild(myChildren);
    }

    /// Iterators
    // @{
    T *getParent() const
    {
	UT_ASSERT(myParent || (!myPrevSibling && !myNextSibling));
	return (T *)myParent;
    }
    T *getFirstChild() const
    {
	return (T *)myChildren;
    }
    T *getPrevSibling() const
    {
	return (T *)myPrevSibling;
    }
    T *getNextSibling() const
    {
	return (T *)myNextSibling;
    }
    // @}

    /// Some utility methods
    // @{
    bool    hasParent() const	{ return (myParent != NULL); }
    bool    hasChildren() const	{ return (myChildren != NULL); }
    bool    hasSiblings() const	{ return (myPrevSibling || myNextSibling); }
    bool    isInTree() const	{ bool yesno = hasParent() || hasChildren();
				  UT_ASSERT(yesno || !hasSiblings());
				  return yesno; }
    // @}

private:
    NodeType *setNextSibling(NodeType *next)
    {
	NodeType *old = myNextSibling;

	if (myNextSibling)
	    myNextSibling->myPrevSibling = NULL;
	myNextSibling = next;
	if (myNextSibling)
	    myNextSibling->myPrevSibling = this;

	return old;
    }
    NodeType *setPrevSibling(NodeType *prev)
    {
	NodeType *old = myPrevSibling;

	if (myPrevSibling)
	    myPrevSibling->myNextSibling = NULL;
	myPrevSibling = prev;
	if (myPrevSibling)
	    myPrevSibling->myNextSibling = this;

	return old;
    }

private:
    NodeType *	myParent;
    NodeType *	myChildren;
    NodeType *	myPrevSibling;
    NodeType *	myNextSibling;
};

#endif // __UT_BIDIRECTIONALTREE_H__
