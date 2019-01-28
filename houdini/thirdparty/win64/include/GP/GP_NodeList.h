/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GP_NodeList.h (C++)
 *
 * COMMENTS: paste nodes stored in a linked list structure
 *
 *
 */

#ifndef __GP_NodeList_h__
#define __GP_NodeList_h__

#include "GP_API.h"
#include <UT/UT_LinkList.h>

class GP_NodeTree;
class GP_Node;
class gp_Node;

class GP_API GP_NodeList : public UT_LinkList
{
public:
    // Class c-tor and d-tor.
		 GP_NodeList(void) { myCrt = 0; }
		 GP_NodeList(const GP_NodeList &list); // shallow
    virtual	~GP_NodeList(void); // does a clear(), not a clearAndDestroy()

    // Insert a node into the structure at the beginning or at the end.
    // No duplicate checking is done.
    void		 insert(const GP_Node &d);
    void		 append(const GP_Node &d);

    // Remove a node from the structure and return its address if found.
    const GP_Node	*remove(void); // removes last
    const GP_Node	*remove(int key);
    const GP_Node	*remove(const GP_Node &d);

    // Search for a given node and return it if found. The search is by
    // key, so the address might differ.
    const GP_Node	*find(int key) const;
    const GP_Node	*find(const GP_Node &d) const;

    // Inquire about a given node:
    int			 contains(int key) const;
    int			 contains(const GP_Node &d) const
			 {
			     return (find(d) != 0);
			 }

    // Clear the structure, or clear and destroy its GP_Node contents too:
    void		 clear(void)	{ UT_LinkList::clear(); }
    void		 clearAndDestroy(void);

    // Iterate through the list.
    GP_Node		*iterateInit(void);
    GP_Node		*iterateNext(void);

    // Convert the given tree to a *stack*, meaning that tree elements
    // are *inserted* rather than appended.
    GP_NodeList		&operator=(const GP_NodeTree &tree);

    // Simple assignment operator that first clears us, then copies the
    // nodes from source in a shallow manner (but our structure is unique).
    GP_NodeList		&operator=(const GP_NodeList &list);

private:
    gp_Node		*myCrt;
};

#endif
