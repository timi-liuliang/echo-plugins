/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Domain pasting Library (C++)
 *
 * COMMENTS:	Paste graph class.
 * 
 */

#ifndef __GP_Graph_h__
#define __GP_Graph_h__

#include "GP_API.h"
#include "GP_NodeTree.h"

class GP_API GP_Graph
{
public:
    // Class c-tor and d-tor
		 GP_Graph(void) {}
		 GP_Graph(GP_NodeTree &cnodes);		// shallow
		 GP_Graph(const GP_Graph &graph);	// shallow
    virtual	~GP_Graph(void); // clears and *destroys* the nodes

    // Clear the graph, ie. shrink the node list to 0 without deleting
    // the nodes themselves, or destroy the nodes as well:
    virtual void	 clear(void);
    virtual void	 clearAndDestroy(int total = 1);

    // Clear and destroy ourselves, then copy the data from the source. If
    // we encounter the target in the source while copying, return its copy.
    // The method copies each node's in and out nodes as well. We first
    // clear and destroy ourselves.
    GP_Node		*copyFrom(const GP_Graph &src, const GP_Node * = 0);

    // Copy the source into us. Shallow at the node level. All our
    // nodes get destroyed.
    GP_Graph		&operator=(const GP_Graph &src);

    // Add another guy to us. This is just an alias:
    GP_Graph		&operator+=(const GP_Graph &src)
			 {
			     merge(src);
			     return *this;
			 }

    // Merge the graph into us. The nodes will be shared but the node
    // structures won't. You may want to clear() the src graph after
    // this to make sure it doesn't delete my nodes too when it dies:
    void		 merge(const GP_Graph &src);

    // Remove a node from the graph and return its address if found. All its 
    // links are severed.
    GP_Node		*remove(GP_Node &node);

    // Method to change the key of a node while preserving the sanity of the
    // node tree. Return the changed node or 0 if not found.
    GP_Node		*rekey(GP_Node &node, int newkey);

    // The public gets a const handle to the nodes:
    const GP_NodeTree	&nodes(void) const	{ return myNodes; }

    // See how many nodes are in the graph:
    int			 entries(void) const	{ return myNodes.entries(); }


protected:
    // Grab the nodes:
    GP_NodeTree		&nodes(void)		{ return myNodes; }

private:
    GP_NodeTree	 myNodes;
};

#endif
