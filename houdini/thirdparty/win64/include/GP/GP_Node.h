/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Domain pasting Library (C++)
 *
 * COMMENTS:	Paste node class.
 * 
 */

#ifndef __GP_Node_h__
#define __GP_Node_h__

#include "GP_API.h"
#include "GP_NodeTree.h"


class GP_API GP_Node
{
public:
    // Class c-tors and d-tor
		 GP_Node(int akey = 0)		{ myKey = akey; }
		 GP_Node(const GP_Node &node)	{ myKey = node.myKey; }
    virtual	~GP_Node(void); // clear node lists, but in/out nodes stay

    // Produce a brand new copy of ourselves. Must free it yourself.
    // Does not copy our in and out nodes!
    virtual GP_Node	*copy(void) const;

    // Copy the in nodes and out-nodes from a pool of already built nodes:
    // Return 0 if OK and -1 otherwise. Use with discretion.
    virtual int 	 copyInNodes (const GP_Node &src, 
				      const GP_NodeTree &destpool);
    virtual int 	 copyOutNodes(const GP_Node &src, 
				      const GP_NodeTree &destpool);

    // Create a a brand new object of the same type as us just using the
    // default c-tor.
    virtual GP_Node	*newSpecies(void) const;

    // Remove the in or out edges and return the node address if found.
    void		 removeInEdges (void);
    void		 removeOutEdges(void);

    // Return the descendents or 0 if none. Must free the object yourself.
    GP_NodeTree		*descendents(void) const;

    // Return the in/out degree, ie the number of nodes in each list:
    int			 inDegree (void) const	{ return myInNodes.entries ();}
    int			 outDegree(void) const	{ return myOutNodes.entries();}

    // Query or set the key:
    int			 key(void) const	{ return myKey; }
    void		 key(int k)		{ myKey = k;    }

    // Query or set the in and out node collections:
          GP_NodeTree	&inNodes (void)      	{ return myInNodes;  }
    const GP_NodeTree	&inNodes (void) const	{ return myInNodes;  }
          GP_NodeTree	&outNodes(void)      	{ return myOutNodes; }
    const GP_NodeTree	&outNodes(void) const	{ return myOutNodes; }

private:
    GP_NodeTree	 myInNodes;
    GP_NodeTree	 myOutNodes;
    int		 myKey;
};

#endif
