/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_BoundingCone.h (UT Library, C++)
 *
 * COMMENTS:	A cone centered at the origin with a given direction and
 *		half-angle.
 */

#ifndef __UT_BoundingCone_h__
#define __UT_BoundingCone_h__

#include "UT_API.h"
#include "UT_Vector3.h"

class UT_API UT_BoundingCone {
public:
    UT_BoundingCone()
    {
	myDir = 0;
	myAngle = 0;
    }

    // Cone angles are measured from the center to the edge of the cone.
    UT_BoundingCone(const UT_Vector3 &dir, float angle)
	: myDir(dir)
	, myAngle(angle)
    {
	myDir.normalize();
    }

    const UT_Vector3	&getDir() const		{ return myDir; }
    float		 getAngle() const	{ return myAngle; }

    // Initialize the direction vector and set the angle to 0
    void		 initBounds(const UT_Vector3 &dir);

    // Enlarge the cone so that it encloses the given direction vector.
    void		 enlargeBounds(const UT_Vector3 &dir);

    // Tests for any intersection between two cones
    bool		 intersects(const UT_BoundingCone &cone) const;

    // Find the intersection of this bounding cone with another cone and
    // store the result in this.  The result will be a conservative bound,
    // since the exact intersection may not be a cone.  If the cones are
    // disjoint, the angle for this cone will be set to 0 and this method
    // will return false.
    bool		 computeIntersection(const UT_BoundingCone &cone);

    void		 dump();

private:
    UT_Vector3	myDir;
    float	myAngle;
};

#endif
