/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_BoundingBox_h__
#define __HOM_BoundingBox_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_Vector3.h"
#include "HOM_PtrOrNull.h"
#include <UT/UT_BoundingBox.h>
#include <vector>

class HOM_Matrix4;

SWIGOUT(%rename(BoundingBox) HOM_BoundingBox;)

class HOM_API HOM_BoundingBox
{
public:
    HOM_BoundingBox()
	: myBoundingBox(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_BoundingBox(
            double xmin, double ymin, double zmin,
            double xmax, double ymax, double zmax)
	: myBoundingBox(xmin, ymin, zmin, xmax, ymax, zmax)
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_BoundingBox(const HOM_BoundingBox &src)
	: myBoundingBox(src.myBoundingBox)
    { HOM_CONSTRUCT_OBJECT(this) }

    SWIGOUT(%ignore HOM_BoundingBox(const UT_BoundingBoxD &);)
    HOM_BoundingBox(const UT_BoundingBoxD &bbox)
	: myBoundingBox(bbox)
    { HOM_CONSTRUCT_OBJECT(this) }

    ~HOM_BoundingBox()
    { HOM_DESTRUCT_OBJECT(this) }

    bool operator==(HOM_PtrOrNull<HOM_BoundingBox> bbox)
    { return bbox.myPointer && myBoundingBox == bbox.myPointer->myBoundingBox; }

    bool operator!=(HOM_PtrOrNull<HOM_BoundingBox> bbox)
    { return !operator==(bbox); }

    bool isAlmostEqual(HOM_BoundingBox &bbox, double tolerance=0.00001);

    // This method is deprecated.
    bool almostEqual(HOM_BoundingBox &bbox, double tolerance=0.00001)
    { return isAlmostEqual(bbox, tolerance); }

    int __hash__();
    std::string __str__();
    std::string __repr__();

    HOM_BoundingBox __mul__(const HOM_Matrix4 &matrix4);

    bool isValid()
    { return myBoundingBox.isValid(); }

    void setTo(const std::vector<double> &tuple);

    HOM_Vector3 minvec()
    { return myBoundingBox.minvec(); }
    HOM_Vector3 maxvec()
    { return myBoundingBox.maxvec(); }
    HOM_Vector3 sizevec()
    { return myBoundingBox.size(); }

    HOM_Vector3	center()
    { return myBoundingBox.center(); }

    void enlargeToContain(const std::vector<double> &point);
    void enlargeToContain(const HOM_BoundingBox &bbox)
    { myBoundingBox.enlargeBounds(bbox.myBoundingBox); }

    bool contains(const std::vector<double> &point);

    SWIGOUT(%ignore operator=;)
    HOM_BoundingBox &operator=(const HOM_BoundingBox& bbox)
    {
	myBoundingBox = bbox.myBoundingBox;
	return *this;
    }

    void *_asVoidPointer()
    { return &myBoundingBox; }


    SWIGOUT(%ignore myBoundingBox;)
    UT_BoundingBoxD myBoundingBox;
};

#endif
