/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_BoundingSphere.h ( Utility Library, C++)
 *
 * COMMENTS:
 *		This class implements a bounding sphere (with unity scale).
 *		It's not very precise. It's mainly meant for a one time
 *		calculation of a bounding sphere. If you try combining bounding
 *		spheres, you get rather gross results compared to your initial
 *		points.
 */

#ifndef __UT_BoundingSphere__
#define __UT_BoundingSphere__

#include "UT_API.h"
#include "UT_Vector3.h"
#include "UT_Vector4.h"
#include "UT_Vector3Array.h"
#include "UT_BoundingBox.h"


class UT_API UT_BoundingSphere
{
public:
			 UT_BoundingSphere();
			~UT_BoundingSphere();

    bool		 isInside(const UT_Vector3R &pt) const;

    fpreal		 getRadius() const	{ return myRadius; }
    const UT_Vector3R	&getCenter() const	{ return myCenter; }

    void		 initialize(bool requires_update = false);
    void		 computeBound();
    fpreal		 computeRadius(const UT_Vector3R &center) const;

    void		 addPoint(const UT_Vector3R &pt);
    void		 addBox(const UT_BoundingBox &box);
    void		 addSphere(const UT_BoundingSphere &bsphere);

    void		 transform(const UT_Matrix4R &xform);

private:
    // NOTE: Currently, the default copy constructor and assignment operators
    //       are sufficient.
    UT_Vector3R		 myCenter;
    fpreal		 myRadius;
    fpreal		 myRadiusSquared;
    UT_Vector3R		 myMins[3];
    UT_Vector3R		 myMaxs[3];
    bool		 myDoDynamic;
    UT_Vector3RArray	 myPointArray;
};
#endif

