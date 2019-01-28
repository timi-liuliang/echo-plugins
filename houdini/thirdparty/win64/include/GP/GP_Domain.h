/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Domain pasting Library (C++)
 *
 * COMMENTS:	Paste domain class.
 * 
 */

#ifndef __GP_Domain_h__
#define __GP_Domain_h__

#include "GP_API.h"
#include <iosfwd>
#include <UT/UT_BoundingRect.h>
#include "GP_FrameHierarchy.h"
#include "GP_Node.h"

class UT_IStream;
class GA_SaveMap;
class GA_LoadMap;
class GP_FrameHierarchy;
class GP_DomainDAG;
class GP_NodeList;


class GP_API GP_Domain : public GP_Node
{
public:
    // Class c-tors and d-tor
		 GP_Domain(void);
		 GP_Domain(const UT_BoundingRect &original,
			   const GP_XformHandle &xform,
			   int key = 0);
		 GP_Domain(const GP_Domain &d);
    virtual	~GP_Domain(void);

    // Produce a brand new copy of ourselves. Must free it yourself.
    // Does not copy the frames recursively!
    virtual GP_Node	*copy(void) const;

    // I/O functions returning 0 if OK and -1 otherwise.
    virtual int		 save(std::ostream &os, int binary = 0) const;
    virtual bool	 load(UT_IStream &is);

    virtual bool	 saveJSONFields(UT_JSONWriter &w,
					const GA_SaveMap &map) const;
    virtual void	 initJSONLoad();
    virtual bool	 loadJSONField(UT_JSONParser &p,
				       const GA_LoadMap &map,
				       const char *field_token);

    // Return a decreasing-order list of would-be parents listed in decreasing
    // order in a stack of nodes.
    virtual void	 findParents(GP_NodeList &stack, GP_NodeList &parns)=0;
    virtual void	 findParent (GP_Domain &parent)=0;

    // Spawn a child and link it to us, assign its frames accordingly, etc.
    virtual GP_Domain	*spawn(const UT_BoundingRect &brect,
			       float ubwidth = 0, float vbwidth = 0,
			       int   ubdivs  = 2, int   vbdivs  = 2,
			       int   newkey  = 0) = 0;

    // Anything goes in terms of spline changes, especially if it
    // involves changes of both knots and CVs, as in refinement, unrefinement,
    // degree elevation, or cutting. Return 0 if OK, else -1.
    virtual int		 modifyBasis(int (*apply)(void *tpsurf, void *d),
				     void *data = 0, int standalone = 1) = 0;

    // Query or set the frame hierarchy. Use with care.
    GP_FrameHierarchy	*frames(void) const
			 {
			     return myFrames;
			 }
    void		 frames(GP_FrameHierarchy *f)
			 {
			     if (myFrames && myOwnFrames) delete myFrames;
			     myFrames    = f;
			     myOwnFrames = 0;
			 }

    // The DAG we live in:
    GP_DomainDAG	*dag(void) const;

    // Update node if a transformation has occurred:
    void		 update(void);

    // Return the 4 values stored in the polygon. It first updates itself:
    void		 polygon(GP_Point &p1, GP_Point &p2, GP_Point &p3,
				 GP_Point &p4)
			 {
			     update();
			     p1 = myWorldPoly[0];
			     p2 = myWorldPoly[1];
			     p3 = myWorldPoly[2];
			     p4 = myWorldPoly[3];
			 }

    // If we're pasted, grab the parent domain:
    GP_Domain		*parentDomain(void) const
			 {
			     return (myFrames && myFrames->parent())
				 ? myFrames->parent()->domain() : 0;
			 }

    // Convert p from world to our space and return 0 if OK, -1 if bad.
    // This a a safer method than the GP_Frame counterpart because it
    // checks the result against the original brect (myOriginal) -- needed
    // when the xform is linear because linears never fail.
    int			 fromWorld(GP_Point &p) const;

    // Check if the world polygon contains the given point. If quick is true
    // we do only a bbox test. We assume the polygon is up-to-date.
    int			 contains(const GP_Point &worldp, int quick=0) const;

    // Check if we fully contain the other domain. If quick is true we do only
    // a bbox test. We assume the polygons are up-to-date.
    int			 contains(const GP_Domain &d, int quick = 0) const;

    // Check if we intersect the other domain. If quick is true we do only
    // a bbox test. We assume the polygons are up-to-date.
    int			 intersects(const GP_Domain &d, int quick = 0) const;

    // Intersect our transformed domain with a world line and return the
    // parametric value 't' of the line at the intersection point. If
    // t is not in [0,1], we don't actually intersect.
    float		 intersect(const GP_Point &a, const GP_Point &b) const;

    // Find out if we're in a paste graph, ie. if we have a parent or not:
    int			 isFeature() const { return myFrames->parent() != 0; }

    // Query or set the "relative" pointer:
    GP_Domain		*relative(void) const	{ return myRelative; }
    void		 relative(GP_Domain *d)	{ myRelative = d;    }

    // Get a const handle to the original rectangle, which corresponds to
    // the size of the valid interval, or set the brect and update the 
    // world poly.
    const UT_BoundingRect	&originalBRect() const	{ return myOriginal; }
    void			 originalBRect(const UT_BoundingRect &brect)
				 {
				     myOriginal = brect;
				     updatePolygon();
				 }
    // Replace the original brect but make sure it does not affect the world
    // image (which means we must xform the personalXform and the children)
    void			 safeOriginalBRect(const UT_BoundingRect &);

    // Return the world bounding rectangle. The non-const method will 
    // first update the polygon.
    const UT_BoundingRect	&worldBRect() const	{ return myWorldRect; }
    const UT_BoundingRect	&worldBRect() { update(); return myWorldRect; }


protected:
    // Update the polygon if a transformation has occurred. This assumes the
    // frame is up to date (ie. the frame hierarchy has been updated.
    // Also see update(). The polygon is assumed convex.
    void		 updatePolygon(void);


private:
    GP_FrameHierarchy	*myFrames;	// frame we live in
    UT_BoundingRect	 myOriginal;	// non-transformed domain
    UT_BoundingRect	 myWorldRect;	// bounding rect for poly
    GP_Point		 myWorldPoly[4];// domain boundaries (xformed to world)
    int			 myOwnFrames;	// do we own these frames?

    GP_Domain		*myRelative;	// used in DAG traversals
};

#endif
