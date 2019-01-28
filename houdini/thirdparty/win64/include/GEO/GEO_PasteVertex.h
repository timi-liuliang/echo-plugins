/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CV expressed in a diffuse coordinate system.
 *
 * COMMENTS:
 */


#ifndef __GEO_PasteVertex_h__
#define __GEO_PasteVertex_h__

#include "GEO_API.h"
#include <GA/GA_Types.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Matrix3.h>
#include <GP/GP_Xform.h>


class GEO_Detail;
class GEO_PasteSurf;


class GEO_API GEO_PasteVertex
{
public:
     // C-tor and destructor.
     GEO_PasteVertex(void) {}
    ~GEO_PasteVertex(void) {}

    // Update the world value based on the current frame or vice-versa,
    // assuming everything else is unchanged:
    void		 updateWorldImage  (void);
    void		 updateDisplacement(void);

    // Update the frame origin and transformation by evaluating the base
    // surface at the given greville location. This method does not update
    // the world image as well. We assume worldgrev(row,col) is valid and
    // the base surface exists.
    void		 updateFrame(const GP_PointMatrix &worldgrev,
				     int row, int col);

    // Update the frame origin and transformation by evaluating the base
    // surface at the given greville location. This method does not update
    // the world image as well. We assume worldgrev(row,col) is valid and
    // the base surface exists.
    // Different from updateFrame() because it computes the tangent vectors
    // directly from the base surface, along its u and v flow lines.
    void		 updateIsoFrame(const GP_PointMatrix &gworld,
					int r, int c);

    // Set the internal data. Certain updates might be required thereafter.
    // See the update methods in this class.
    void		 setDisplacement(const UT_Vector3 &v)
			 {
			     myDisplace = v;
			 }
    void		 setFrame(const UT_Vector3 orig)
			 {
			     myFrameOrig = orig;
			     myFrameMatx.identity();
			     myFrameInvm.identity();
			     myInverseOK = 1;
			 }
    void		 setFrame(const UT_Vector3 orig,const UT_Matrix3 &matx)
			 {
			     myFrameOrig = orig;
			     myFrameMatx = matx;
			     myInverseOK = 0;
			 }

    void		 translateOrigin(const UT_Vector3 delta)
			 {
			     myFrameOrig += delta;
			 }

    void		 elevateDisplacement(float delta)
			 {
			     myDisplace.z() += delta;
			 }
    void		 flipDisplacement()
			 {
			     myDisplace.z() = -myDisplace.z();
			 }

    const UT_Vector3	&displacement(void) const	{ return myDisplace; }

    // Get and set the external information: base surface and geo point:
    GEO_PasteSurf	*baseSurface(void) const	{ return myBaseSurf; }
    void		 baseSurface(GEO_PasteSurf *s)	{ myBaseSurf = s;    }

    GA_Offset		 getPointOffset() const
			 {
			     return myPointOffset;
			 }
    void		 point(GA_Offset pt_offset, bool update = true)
			 {
			     myPointOffset = pt_offset;
			     if (update) updateDisplacement();
			 }

    // Equality operator tests pointer equality of point and base surf.
    int			 operator==(const GEO_PasteVertex &vtx) const;

    // Assignment operator overrides point and base surf pointers.
    GEO_PasteVertex	&operator=(const GEO_PasteVertex &vtx);


private:
    GEO_Detail		&detail();

private:
    UT_Vector3		 myDisplace;	// the displacement from the greville

    UT_Vector3		 myFrameOrig;	// origin in pasted space
    UT_Matrix3		 myFrameMatx;	// local frame on base surface
    UT_Matrix3		 myFrameInvm;	// inverse matrix
    char		 myInverseOK;	// 1 if inverse matrix is valid

    GEO_PasteSurf	*myBaseSurf;	// base surface for the pasted CV
    GA_Offset		 myPointOffset;	// image in object (world) space

    // Take two domain points in world coordinates and return the second one
    // in the space of the domain. Return 0 if OK, else -1.
    int 		 computeIncrement(const GP_Point &gcrt,
					  GP_Point &gnext,
					  GP_Point &gsave) const;
};

#endif

