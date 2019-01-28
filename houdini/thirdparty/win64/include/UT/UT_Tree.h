/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Tree.h (C++, Utility Library)
 *
 * COMMENTS:
 *
 *	A simple templatized n-ary tree implementation using single pointers.
 *	To be extended as required by users of this class. (For example,
 *      you can't as yet remove nodes from it.)
 *
 *	For slightly enhanced version of this, see UT_BidirectionalTree.
 *
 *	- You must declare your tree class like so:
 *	    class MyTreeType : public UT_Tree<MyTreeType>
 *	- The parent node will free its own children upon destruction so don't
 *	  do things like adding pointers to items on the stack.
 */

#ifndef __UT_Tree_h__
#define __UT_Tree_h__

#include <stdlib.h>


template <class T> class UT_Tree 
{
public:
    UT_Tree()
    {
	myLeft = NULL;
	myRight = NULL;
    }
    virtual ~UT_Tree()
    {
	delete myLeft;
	delete myRight;
    }

    //
    // Methods for using this as an n-ary tree
    //
    void addSibling( UT_Tree<T> *new_sibling )
    {
	if( myRight == NULL )
	    myRight = new_sibling;
	else
	    myRight->addSibling( new_sibling );
    }
    void addChild( UT_Tree<T> *new_child )
    {
	if( myLeft == NULL )
	    myLeft = new_child;
	else
	    myLeft->addSibling( new_child );
    }
    T *getFirstChild()	{ return (T *)myLeft; }
    T *getNextSibling()	{ return (T *)myRight; }

    //
    // Methods for using this as a binary tree
    //
    void setLeft( UT_Tree<T> *new_child )   { myLeft = new_child; }
    void setRight( UT_Tree<T> *new_child )  { myRight = new_child; }
    T *getLeft()			    { return (T *)myLeft; }
    T *getRight()			    { return (T *)myRight; }

private:
    UT_Tree( const UT_Tree<T> &copy );	// not implemented yet
    UT_Tree<T> &operator =( const UT_Tree<T> &copy );// not implemented yet

private: 
    UT_Tree<T> *    myLeft;
    UT_Tree<T> *    myRight;
};


#endif // __UT_Tree_h__
