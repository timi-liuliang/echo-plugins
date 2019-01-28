/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Domain pasting Library (C++)
 *
 * COMMENTS:	Specialized frame collection. Based on a tree structure.
 * 
 */

#ifndef __GP_FrameHierarchy_h__
#define __GP_FrameHierarchy_h__

#include "GP_API.h"
#include <iosfwd>
#include "GP_CorrectionFrame.h"

class UT_IStream;
class GA_SaveMap;
class GA_LoadMap;
class GP_Domain;
class GP_DomainDAG;
class GP_NodeTree;

class GP_API GP_FrameHierarchyFlags
{
public:
    GP_FrameHierarchyFlags()
    {
	       	dirty		= 0;
		modified	= 0;
    }
    unsigned	dirty		:1, // xform changed
		modified	:1; // visited by update method
};

class GP_API GP_FrameHierarchy
{
  public:
    // Class c-tors and d-tor.
		 GP_FrameHierarchy(void);
		 GP_FrameHierarchy(const GP_XformHandle &rootxform);
		 GP_FrameHierarchy(const GP_FrameHierarchy &h); // shallow!
    virtual	~GP_FrameHierarchy(void);

    // Insert the root node, which will replace the existing root. Return 0 if
    // successful and -1 otherwise.
    int			 insertRoot(GP_CorrectionFrame &root);

    // Insert child into this hierarchy and return 0 if OK, -1 if failure.
    // The child must not belong to another hierarchy at this point. Checking
    // for duplicates is OFF by default.
    int			 insert(GP_FrameHierarchy &child, int check = 0,
							  int wcorrection = 1);

    // Remove child, update the hierarchy the child's world xform. The child
    // becomes stand-alone. The method returns the child if OK and 0 otherwise.
    GP_FrameHierarchy	*remove(GP_FrameHierarchy &child);

    // Same as removal, only the child's anchor is not updated. Useful for
    // imminent re-attachment.
    GP_FrameHierarchy	*detach(GP_FrameHierarchy &child);

    // Remove child w/o updating the child's state -- just cut him off.
    // The method returns the child if OK and 0 otherwise.
    GP_FrameHierarchy	*discard(GP_FrameHierarchy &child);

    // Isolate ourselves by removing ourselves from the tree of hierarchies
    // and letting our parent and all of our children know. We also reset
    // myDAG to zero.
    void		 isolate(void);

    // Move child to the given hierarchy, making all the necessary updates.
    // Return 0 if OK and -1 otherwise. The child should be ours initially.
    int			 move(GP_FrameHierarchy &child, GP_FrameHierarchy &to)
			 {
			     return detach(child) ? to.insert(child) : -1;
			 }

    // Update this collection starting at the given frame or at the root
    // if the frame is nil. We assume the frame is in the collection.
    void		 update(const GP_Frame * = 0)
			 {
			     if (myAnchor->myFlags.dirty)
				 myAnchor->traverseUpdateWorld(0);
			 }

    // Hierarchy traversal method (depth-first):
    void		 apply(void (f)(GP_FrameHierarchy*,void*), void *data);

    // I/O functions returning 0 if OK and -1 otherwise. They operate ONLY
    // on the root.
    int			 save(std::ostream &os, int binary = 0) const;
    bool		 load(UT_IStream &is);

    bool		 save(UT_JSONWriter &w, const GA_SaveMap &map) const;
    bool		 load(UT_JSONParser &p, const GA_LoadMap &map);

    // Copy just our root from the source's root:
    void		 copyFrom(const GP_FrameHierarchy &src);

    // Copy the links of the frames stored in destnodes. Returns a pointer to
    // the root frame in destnodes.
    GP_FrameHierarchy	*copyLinks(GP_NodeTree &destnodes) const;

    // Search the whole hierarchy (not just immediate children), or just the
    // children:
    int			 contains(const GP_FrameHierarchy &descendent) const;
    int			 hasChild(const GP_FrameHierarchy &child     ) const
			 {
			     int		 f = 0;
			     GP_FrameHierarchy	*h = myChildren;

			     while (h && !(f = (h==&child))) h = h->mySibling;
			     return f;
			 }

    // Find out if we have any children or a parent:
    int			 hasParent  () const	{ return myParent   ? 1 : 0; }
    int			 hasChildren() const	{ return myChildren ? 1 : 0; }

    // Access some of the internal hierarchies we know about:
    GP_FrameHierarchy 	*anchor  (void) const	{ return myAnchor;   }
    GP_FrameHierarchy	*parent  (void) const	{ return myParent;   }
    GP_FrameHierarchy	*sibling (void) const	{ return mySibling;  }
    GP_FrameHierarchy	*children(void) const	{ return myChildren; }

    // Const reference to the root.
    const GP_CorrectionFrame	&root() const	{ return myRoot;     }

    // Query or set the "modified" flag. If true, it means that either us
    // or our ancestors had changed at one point in time. It does not mean
    // we're still dirty. Use the dirty() method for that.
    int			 modified(void) const	{ return myFlags.modified; }
    void		 modified(int m)	{ myFlags.modified = m;    }

    // Query or set the domain information stored in this node:
    GP_Domain		*domain(void) const	{ return myDomain;   }
    void		 domain(GP_Domain *d)	{ myDomain = d;      }

    // Query or set the domain DAG information stored in this node.
    // This is to be used only by the anchor!
    GP_DomainDAG	*dag(void) const	{ return myDAG;   }
    void		 dag(GP_DomainDAG *d)	{ myDAG = d;      }

    // Some frame's transformation has changed. Let us know.
    int 		 isDirty(void) const	{ return myFlags.dirty;    }
    void		   dirty(void)
			   {
			       myFlags.dirty = 1;
			       myAnchor->myFlags.dirty = 1; // very important!
			   }


protected:
private:
    GP_CorrectionFrame	 myRoot;	// root node. kids are hierarchies

    GP_FrameHierarchy	*myAnchor;	// root-most hierar. we're in. never 0.
    GP_FrameHierarchy	*myParent;	// parent "node"
    GP_FrameHierarchy	*mySibling;	// sibling "node"
    GP_FrameHierarchy	*myChildren;	// hierarchy of children

    GP_Domain		*myDomain;	// domain associated with frame
    GP_DomainDAG	*myDAG;		// this ptr is set only for the ANCHOR!

    GP_FrameHierarchyFlags	myFlags;


    // Special internal c-tor.
    GP_FrameHierarchy(const GP_FrameHierarchy  &hierarchy,
                      const GP_CorrectionFrame &root);

    // Traverse the hierarchy from us to all the descendents and update
    // everyone's world transforms, then clear everyone's dirty flags.
    void		 traverseUpdateWorld(int history = 0);

    // Link an immediate child to us. Really meant ot be used internally.
    void		 attach(GP_FrameHierarchy &child)
			 {
			     child.myParent = this;
			     child.mySibling = myChildren;
			     myChildren = &child;
			 }

    // Set the anchor to us and all our descendents:
    void		 anchor(GP_FrameHierarchy *a)
			 { 
			     myAnchor = a; 
			     GP_FrameHierarchy *h = myChildren; 
			     while (h)
			     {
				 h->anchor(a); 
				 h = h->mySibling; 
			     }
			 }

    // Tell a frame if it's in this list or not:
    void link  (GP_Frame &frame)	{ frame.hierarchy(this); }
    void unlink(GP_Frame &frame)	{ frame.hierarchy( 0  ); }
};

#endif
