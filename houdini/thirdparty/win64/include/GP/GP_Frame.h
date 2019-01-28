/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Domain pasting Library (C++)
 *
 * COMMENTS:	Transformation frame for pasted domains.
 * 
 */

#ifndef __GP_Frame_h__
#define __GP_Frame_h__

#include "GP_API.h"
#include <iosfwd>
#include "GP_Xform.h"

class GP_FrameHierarchy;


class GP_API GP_Frame
{
    friend class GP_FrameHierarchy;

public:
    // Class c-tors and d-tor.
		 GP_Frame(void);
		 GP_Frame(const GP_Frame &frame);
		 GP_Frame(const GP_XformHandle &handle);
    virtual	~GP_Frame(void);

    // Deep copy from source frame.
    virtual void copyFrom(const GP_Frame &src);

    // I/O functions returning 0 if OK and -1 otherwise.
    virtual int	 save(std::ostream &os, int binary = 0) const;
    virtual bool	 load(UT_IStream &is);

    // Assignment operator: 
    GP_Frame	&operator=(const GP_Frame &frame);

    // Take a point assumed to be in our frame and convert it to other
    // frames. Return 0 if successful, else -1:
    int 	 toParent(GP_Point &p) const;
    int 	 toWorld (GP_Point &p) const;
    int 	 toFrame (GP_Point &p, const GP_Frame &to) const;

    // Take a matrix of points assumed to be in our frame and convert it to
    // other frames. Return 0 if successful, else -1:
    int 	 toParent(GP_PointMatrix &matx) const;
    int 	 toWorld (GP_PointMatrix &matx) const;
    int 	 toFrame (GP_PointMatrix &matx,
				      const GP_Frame &to) const;

    // Take a point or matrix expressed in another frame and convert it to us.
    // Return 0 if successful, else -1:
    int 	 fromParent(GP_Point &p) const;
    int 	 fromWorld (GP_Point &p) const;
    int 	 fromParent(GP_PointMatrix &matx) const;
    int 	 fromWorld (GP_PointMatrix &matx) const;

    // Add a translation by delta to this frame's transform:
    void	 translate(float dx, float dy);

    // Query or set the dirty flag when the world xform is bad:
    int		 isDirty(void) const	{ return myDirtyWorld; }
    void	   dirty(void);

    // Change the transformation:
    virtual void changeXform (const GP_XformHandle &handle);
    virtual void composeXform(const GP_XformHandle &handle);

    // Query (but not change) the personal and world transforms:
    const GP_XformHandle	&personalXform() const { return myXform;      }
    const GP_XformHandle	&worldXform   () const { return myWorldXform; }

    // Find out what hierarchy we're in, if any:
    GP_FrameHierarchy		*hierarchy(void) const	{ return myHierarchy;}

    // Return our parent frame in the hierarchy, or nil.
    const GP_Frame		*parent(void) const;


protected:
    //  Constructor meant to be used only by friends:
    GP_Frame(const GP_XformHandle &xform, const GP_XformHandle &world,
	     int dirtyworld, GP_FrameHierarchy *frames);

    // Set the parent and world transforms:
    void	 personalXform(const GP_XformHandle &handle)
		 {
		     myXform = handle;

		     if (myHierarchy)
		     {
			 dirty(); // will need to recompute the world xform
		     }
		     else
		     {
			 myWorldXform = handle;
			 myDirtyWorld = 0;
		     }
		 }
    void	 worldXform (const GP_XformHandle &handle)
		 {
		     myWorldXform = handle; // will share the xform
		     myDirtyWorld = 0;
		 }
    void	 worldXform (const GP_Xform &xform)
		 {
		     myWorldXform = xform; // copies the xform deeply
		     myDirtyWorld = 0;
		 }

    // Set the hierarchy pointer. We live in it.
    void	 hierarchy(GP_FrameHierarchy *h = 0)
		 {
		     if (h != myHierarchy)
		     {
			 myHierarchy  = h;
			 myDirtyWorld = 1; // just us
		     }
		 }

    // Only friends can use these ones:
    GP_XformHandle	&personalXform(void) { return myXform;	    }
    GP_XformHandle	&worldXform   (void) { return myWorldXform; }

private:
    GP_XformHandle	 myXform;
    GP_XformHandle	 myWorldXform;
    int			 myDirtyWorld;
    GP_FrameHierarchy	*myHierarchy;
};

#endif
