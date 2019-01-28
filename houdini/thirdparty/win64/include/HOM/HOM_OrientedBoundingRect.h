/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_OrientedBoundingRect_h__
#define __HOM_OrientedBoundingRect_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Matrix2.h"
#include "HOM_Vector2.h"

#include <UT/UT_OBBox2D.h>

SWIGOUT(%rename(OrientedBoundingRect) HOM_OrientedBoundingRect;)

class HOM_API HOM_OrientedBoundingRect
{
public:
    HOM_OrientedBoundingRect(const std::vector<HOM_Vector2> &points);

    ~HOM_OrientedBoundingRect()
    { HOM_DESTRUCT_OBJECT(this) }

    bool operator==(HOM_PtrOrNull<HOM_OrientedBoundingRect> brect)
    {
        return brect.myPointer &&
               myBoundingBox == brect.myPointer->myBoundingBox;
    }

    bool operator!=(HOM_PtrOrNull<HOM_OrientedBoundingRect> brect)
    { return !operator==(brect); }

    bool isAlmostEqual(HOM_OrientedBoundingRect &brect,
                       double tolerance=0.00001);

    int __hash__();
    std::string __str__();
    std::string __repr__();

    HOM_Vector2 sizevec()
    { return myBoundingBox.getRadii(); }

    HOM_Vector2	center()
    { return myBoundingBox.getCenter(); }

    HOM_Matrix2 orientation()
    { return myBoundingBox.getBasis(); }

    SWIGOUT(%ignore myBoundingBox;)
    UT_OBBox2DD myBoundingBox;
};

#endif
