/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SplayTree.h (C++)
 *
 * COMMENTS:
 *
 *	This code implements a generic c++ class of splay tress.
 *	It is based on C code written by David Brower, daveb@rtech.uucp 1/88 
 *	who based it on pascal code written by Douglas W. Jones who
 *	based it on algorithms originally taken from "Self Adjusting Binary 
 *	Trees" 	by DD Sleator and RE Tarjan (ACM SIGAC TBoston, Apr 1983).
 *
 */

#ifndef __UT_SplayTree_h__
#define __UT_SplayTree_h__


#include "UT_API.h"
class utSplayNode;

class UT_API UT_SplayTree {

public:

	// Constructors and destructor:

	explicit UT_SplayTree(int (*fcomp)(const void *, const void *));
	UT_SplayTree(const UT_SplayTree &tree); // shallow
	virtual		~UT_SplayTree(void);

	// Modification methods

	int		 add(const void *, unsigned maxDepth = 0);

	// remove node from tree, deallocate it and return its data
	// 0 if not found. If passed 0, it removes the root.
	const void	*remove(const void * = 0);

	// delete every node from the tree
	void		 clear(void);

	// Add a tree to us or remove it:
	UT_SplayTree	&operator+=(const UT_SplayTree &tree);
	UT_SplayTree	&operator-=(const UT_SplayTree &tree);

	// Assign another tree to us:
	UT_SplayTree	&operator=(const UT_SplayTree &tree);

	// query methods

	// apply the function to each node in the tree
	// terminate if a non-zero result is obtained
	// Return the result returned from the last node
	// if no nodes, return 0

	int		 traverse(int (*func)(const void *));
	int		 traverse(int (*func)(const void *,void *),void *data);
	int		 traverseBackward(int (*func)(const void *));
	int		 traverseBackward(int (*func)(const void *,void *),
					  void *data);

	// return the node containing the data, 0 if not found
	const void	*find(const void *data) const;

	int		 entries(void) const {return count; }

private:

	// insert a node into the tree. If maxdepth > 0, we do a
	// splay if we find a branch that exceeds this maximum depth
	void		 insertNode(utSplayNode *, unsigned maxDepth);
	void		 deleteNode(utSplayNode *);
	utSplayNode	*findNode(const void *data);
	utSplayNode	*removeHead(utSplayNode **);
	void		 splay(utSplayNode *);
	void		 splayToDepth(unsigned);
	int		 splayToDepth(utSplayNode *, unsigned, unsigned);

	// member data
	int		(*myFcomp)(const void *, const void *);
	utSplayNode	 *myRoot;	// root of binary tree
	int		  count;	// size of tree
	int		  myNumAdded;   // Number added since last splayToDepth
};

#endif
