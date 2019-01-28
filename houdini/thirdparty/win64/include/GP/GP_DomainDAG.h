/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Domain pasting Library (C++)
 *
 * COMMENTS:	Paste DAG class.
 * 
 */

#ifndef __GP_DomainDAG_h__
#define __GP_DomainDAG_h__

#include "GP_API.h"
#include <limits.h>
#include "GP_Graph.h"
#include "GP_Domain.h"

class GP_NodeList;
class GP_NodeTree;

class GP_API GP_DomainDAG : public GP_Graph
{
public:
    // C-tor with 0 nodes. Can't paste on it.
    GP_DomainDAG(void);

    // The root this c-tor assigns to myRoot cannot be already pasted somewhere
    // else.
    GP_DomainDAG(GP_Domain *root);

    // Shallow copy c-tor: same nodes, but different lists.
    GP_DomainDAG(const GP_DomainDAG&);

    // D-tor that destroys all the nodes (thanks to its base class).
    virtual		~GP_DomainDAG(void);

    // Clear the graph, ie. shrink the domain list to 0 without deleting
    // the domains themselves, or destroy the domains as well:
    virtual void	 clear(void);
    virtual void	 clearAndDestroy(int total = 1);

    // Copy the source data into us. This is not a complete copy because
    // it doesn't resolve the issue of domain surface ownership.
    void		 copyFrom(const GP_DomainDAG &srcdag);

    // Copy from the source. Shallow in the nodes. All out nodes are 
    // destroyed first. Be careful.
    GP_DomainDAG	&operator=(const GP_DomainDAG &src);

    // Check if the domain or the dag would fit into our root domain. "parents"
    // emerges with a list of would-be top container parents.
    int			 canPaste(GP_Domain &d);
    int			 canPaste(GP_Domain &d, GP_NodeList &parents);
    int			 canPaste(GP_DomainDAG &dag)
			 {
			     return dag.myRoot ? canPaste(*dag.myRoot) : 0;
			 }
    int			 canPaste(GP_DomainDAG &dag,GP_NodeList &parents)
			 {
			     return dag.myRoot?canPaste(*dag.myRoot,parents):0;
			 }
    int			 canPaste(GP_DomainDAG &dag, GP_Domain &parent,
				  const UT_BoundingRect &brect);

    // Find out if we're in a paste graph, ie. if we have a parent or not:
    int			 isPasted() const
			 {
			     return (myRoot && myRoot->isFeature());
			 }

    // Paste the dag onto us if it fits, and return its root. "parents" holds
    // the list of would-be to container domains.
    GP_Domain		*paste(GP_DomainDAG &child, int keepshape = 0);
    GP_Domain		*paste(GP_DomainDAG &child, GP_NodeList &parents,
			       int keepshape = 0);

    // Paste the srcdag onto us after checking if the 4 domain world points
    // are in our transformed root domain. Return the root of the srdag or 0.
    GP_Domain		*paste(GP_DomainDAG &srcdag,
			       const GP_Point &b1, const GP_Point &b2,
			       const GP_Point &b3, const GP_Point &b4);

    // Unpaste a domain alone or with all its contained children. If you know
    // what the contained set is, pass it in for speed. If you unpaste the
    // root domain, the whole dag goes.
    GP_Domain		*unpasteAt  (GP_Domain &child);
    GP_DomainDAG	*unpasteFrom(GP_Domain &child, GP_NodeTree *ctree=0);

    // Delete a domain alone or with all its contained children. If you know
    // what the contained set is, pass it in for speed. If you delete the root
    // domain all the kids will be unpasted. Return 0 upon success, or -1.
    int			 destroyAt  (GP_Domain &child);
    int			 destroyFrom(GP_Domain &child, GP_NodeTree *ctree=0);

    // Replace a domain alone or with all its contained children with a new
    // DAG. If you know the domain's contained set, pass it in for speed.
    // The method returns dag's root or 0.
    GP_Domain		*replaceAt  (GP_Domain &child, GP_DomainDAG &dag);
    GP_Domain		*replaceFrom(GP_Domain &child, GP_DomainDAG &dag,
				     GP_NodeTree *ctree = 0);

    // Similar to paste, only if the child or its contained DAG has bridges,
    // the bridges will be raised before pasting, then lowered again. If 
    // you know child's contained nodes, pass them in for speed. We return 
    // dag's root or 0.
    GP_Domain		*overlayAt  (GP_Domain &child, GP_DomainDAG &dag,
				     int keepshape = 0);
    GP_Domain		*overlayAt  (GP_Domain &child, GP_DomainDAG &srcdag,
				     const UT_BoundingRect &brect);
    GP_Domain		*overlayAt  (GP_Domain &child, GP_DomainDAG &srcdag,
				     const GP_Point &b1, const GP_Point &b2,
				     const GP_Point &b3, const GP_Point &b4);
    GP_Domain		*overlayFrom(GP_Domain &child, GP_DomainDAG &dag,
				     GP_NodeTree *ctree=0, int keepshape = 0);
    GP_Domain		*overlayFrom(GP_Domain &child, GP_DomainDAG &dag,
				     const GP_Point &b1, const GP_Point &b2,
				     const GP_Point &b3, const GP_Point &b4,
				     GP_NodeTree *ctree=0);

    // Remove the child. This is an alias for  unpasteAt() and doesn't need
    // to be redefined in the derived classes.
    GP_Domain		*removeAt(GP_Domain &child)
			 {
			     return GP_DomainDAG::unpasteAt(child);
			 }

    // Anything goes in terms of spline surface changes, especially if it
    // involves changes of both knots and CVs, as in refinement, unrefinement,
    // degree elevation, or cutting. If "standalone" is 1, we bring the
    // child back to its unpasted shape before calling (*apply)().
    // Return 0 if OK, else -1.
    int			 modifyBasis(GP_Domain &child,
				     int (*apply)(void *tpsurf, void *d),
				     void *data = 0,
				     int   standalone = 0);

    // Add a layer by spawning a domain definitely contained within the child.
    // Return that domain if everything is successful, else 0. The child
    // in the domain area defined by brect, and the ub/vb stuff are belt
    // reinforcements.
    GP_Domain		*spawn(GP_Domain &child, const UT_BoundingRect &brect,
			       float ubwidth = 0, float vbwidth = 0,
			       int   ubdivs  = 2, int   vbdivs  = 2);

    // See if we contain this world domain point. A quick check does only a 
    // bbox test:
    int			 contains(const GP_Point &worldp, int quick = 0) const
			 {
			     return myRoot ? myRoot->contains(worldp,quick):0;
			 }

    // See if d is pasted on us by checking its anchor against our root:
    int			 contains(const GP_Domain &d) const
			 {
			     return myRoot
				 ?  myRoot->frames()==d.frames()->anchor() : 0;
			 }

    // Method to change the key of a domain while preserving the sanity of the
    // graph. Return the changed node or 0 if not found. The fast method does
    // not remove and reinsert the domain.
    GP_Domain		*keyUp(GP_Domain &domain)
			 {
			     return (GP_Domain*)rekey(domain, myLabel++);
			 }
    void		 keyUpFast(GP_Domain &domain)
			 {
			     domain.key(myLabel++);
			 }

    // Return top-most domain that worldp lies inside, or 0.
    GP_Domain		*topDomain(const GP_Point &worldp) const;

    // Change the root transformation given a handle or a domain. This will set
    // everything dirty. The domain isn't const because it might need to be
    // updated.
    void		 changeRootXform(const GP_XformHandle &handle);
    void		 changeRootXform(GP_Domain &d);

    // Assuming "child" is the root of a newly pasted DAG, link it to its
    // parent domain(s) (top containers) in our DAG. The methods make sure 
    // everything is (re)pasted and updated accordingly. The bases are
    // already set at this point by findParents().
    virtual void	 link(GP_Domain &child, GP_NodeList &parents,
			      int keepshape = 0);
    virtual void	 link(GP_Domain &child, GP_Domain   &parent,
			      int withcorrection = 1);

    // Assuming child is pasted on us, update the pasted image of all of
    // surf's descendents in pasted order. surf is not updated.
    // Return 0 upon success, else -1.
    virtual int		 updateDescendents(GP_Domain &child,
					   int including_child = 0);

    // Some vertices (or all) have changed, so reverse engineer the
    // displacements from the CVs of the spline surface. In other words,
    // given the local frames and the GEO_Points, build the displacements for
    // the child node, then update its descendents. Return -1 if not our child.
    virtual int		 updateDisplacements(GP_Domain &child, 
					     int keepframes = 1);

    // The public can only look at the root but not change it because things
    // are tricky in the hierarchy:
    const GP_Domain	*root(void) const	{ return myRoot; }


protected:
    // Let us know when something has changed in the root domain.
    virtual void	 changedRoot(void);

    // Tell the dag it's obsolete. This will happen when the dag has been
    // pasted onto some other dag, and should be deleted. This method clears
    // the nodes. Derived classes will probably "delete this" inside it,
    // so don't use the dag thereafter.
    virtual void	 obsolete(void);

    // Build a sorted list of nodes representing the contained DAG rooted at
    // base, base included. You must free it yourself.
    GP_NodeTree		*containedDomains(const GP_Domain& base) const;

    // root must be stand-alone. Don't do it if we have a root already. Use
    // modifyAt() instead. The method returns root or 0 if failure.
    GP_Domain		*insertRoot(GP_Domain &root);

    // Just adds this one to the node, and bumps up our label count.
    void		 insertDomain(GP_Domain &domain);

    // Relabel the whole DAG from a given value and update myLabel.
    void		 relabel(int start = INT_MIN);

    // Use this method after loading to make sure all the proper links are
    // set. ONLY after loading. Return 0 is OK and -1 otherwise.
    int			  pasteInPlace(GP_Domain &child);

    // Derived classes get a full look at the root with a non-const handle:
    GP_Domain		*root(void)		{ return myRoot; }


private:
    GP_Domain		*myRoot;	// root domain
    int			 myLabel;	// current label for nodes


    // Private c-tor that assigns "root" to its root and grabs the nodes
    // from the node tree. The structures will not be the same, but the
    // nodes will be shared:
    GP_DomainDAG(GP_Domain *root, GP_NodeTree &cnodes);

    // Given a domain or its immediate parents, find the root of the shortest
    // contained DAG that would contain the domain or its given parents.
    GP_Domain		*minContainment(GP_Domain &domain);
    GP_Domain		*minContainment(GP_NodeList &parents);

public:	// raiseBridges() is called from within a static callback...
    // Assuming base is in our graph, raise the bridges recursively.
    void		 raiseBridges(GP_Domain &d,
				      GP_NodeTree *bridges);

    // Return a list of domains d would touch if pasted here. We assume d
    // would fit in the root domain. Unlike canPaste(), this method ignores
    // nodes currently not linked up. "parents" contains domains in decreasing
    // key order.
    GP_NodeList		&underDomains(GP_Domain &d, GP_NodeList &parents);
private:

    // This method assumes the child is pasted onto this dag, and removes
    // its frame links, the node from the graph, and sets its dag pointer
    // to nil. If this is the root, it sets myRoot to zero.
    void		 isolate(GP_Domain &child);

    // Detach the high bridges (ie. here the immediate children) of the node.
    // The method returns an ordered list of detached nodes. These nodes will
    // only have links to the contained DAG they are the roots of. You must
    // free this list yourself.
    GP_NodeTree		*raiseBridgesAt(GP_Domain &node);

    // Raise bridges from a base node or from a list of contained nodes. 
    // The resulting nodes will only have links to the contained DAG they are
    // the roots of. You must free this list yourself.
    GP_NodeTree		*raiseBridgesFrom(GP_Domain &base);
    GP_NodeTree		*raiseBridgesFrom(GP_NodeTree &nodes);

    // Relink the bridges by pasting them again in the order in which they 
    // had been pasted originally.
    void		 lowerBridges(GP_NodeTree &bridges, int relabel = 0);
};

#endif
