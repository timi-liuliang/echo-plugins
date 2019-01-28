/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GP_NodeTree.h (C++)
 *
 * COMMENTS: paste nodes stored in a splay tree structure
 *
 *
 */

#ifndef __GP_NodeTree_h__
#define __GP_NodeTree_h__


#include "GP_API.h"
#include <UT/UT_SplayTree.h>

class GP_Node;

class GP_API GP_NodeTree : public UT_SplayTree
{
public:
    // Constructors and destructor:
		 GP_NodeTree(void);
		 GP_NodeTree(const GP_NodeTree &tree); // shallow
    virtual 	~GP_NodeTree(void); // does a clear(), not a clearAndDestroy()

    // Clear and DESTROY ourselves, then copy the data from the source. If
    // we encounter the target in the source while copying, return its copy.
    GP_Node		*copyFrom(const GP_NodeTree &src, const GP_Node * = 0);

    // Another copy method that looks for the nodes to insert in a pool of
    // destination nodes rather than copying the source nodes. Return 0 if
    // all source nodes were found in the destination, else -1.
    // We first clear ourselves.
    int			 copyFrom(const GP_NodeTree &src,
				  const GP_NodeTree &destpool);

    // Insert a node into the structure. Return 0 if the node is already
    // in the tree. Otherwise return 1.
    int			 insert(const GP_Node &d)
			 {
			     return UT_SplayTree::add(&d);
			 }
    int			 append(const GP_Node &d)
			 {
			     return UT_SplayTree::add(&d);
			 }

    // Remove a node from the structure and return its address if found.
    const GP_Node	*remove(void) // removes root
			 {
			     return (const GP_Node*)UT_SplayTree::remove();
			 }
    const GP_Node	*remove(int key);
    const GP_Node	*remove(const GP_Node &d)
			 {
			     return (const GP_Node*)UT_SplayTree::remove(&d);
			 }

    // Search for a given node and return it if found. The search is by
    // key, so the address might differ.
    const GP_Node	*find(int key) const;
    const GP_Node	*find(const GP_Node &d) const
			 {
			     return (const GP_Node*)UT_SplayTree::find(&d);
			 }

    // Inquire about a given node:
    int			 contains(int key) const;
    int			 contains(const GP_Node &d) const
			 {
			     return (UT_SplayTree::find(&d) != 0);
			 }

    // Clear the structure, or clear and destroy its GP_Node contents too:
    void		 clear(void)	{ UT_SplayTree::clear(); }
    void		 clearAndDestroy(void);

private:
    // Nothing.
};

#endif
