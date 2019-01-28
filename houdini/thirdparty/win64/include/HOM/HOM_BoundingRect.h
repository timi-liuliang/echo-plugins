/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_BoundingRect_h__
#define __HOM_BoundingRect_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_Vector2.h"
#include "HOM_PtrOrNull.h"
#include <UT/UT_BoundingRect.h>
#include <vector>

class HOM_Matrix4;

SWIGOUT(%rename(BoundingRect) HOM_BoundingRect;)

class HOM_API HOM_BoundingRect
{
public:
    HOM_BoundingRect()
    { HOM_CONSTRUCT_OBJECT(this); myBoundingRect.initBounds(); }

    HOM_BoundingRect(double x1, double y1, double x2, double y2)
	: myBoundingRect(SYSmin(x1, x2),
			 SYSmin(y1, y2),
			 SYSmax(x1, x2),
			 SYSmax(y1, y2))
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_BoundingRect(const HOM_Vector2 &pos1, const HOM_Vector2 &pos2)
	: myBoundingRect(SYSmin(pos1.myVector2.x(), pos2.myVector2.x()),
			 SYSmin(pos1.myVector2.y(), pos2.myVector2.y()),
			 SYSmax(pos1.myVector2.x(), pos2.myVector2.x()),
			 SYSmax(pos1.myVector2.y(), pos2.myVector2.y()))
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_BoundingRect(const HOM_BoundingRect &src)
	: myBoundingRect(src.myBoundingRect)
    { HOM_CONSTRUCT_OBJECT(this) }

    SWIGOUT(%ignore HOM_BoundingRect(const UT_BoundingRectD &);)
    HOM_BoundingRect(const UT_BoundingRectD &rect)
	: myBoundingRect(rect)
    { HOM_CONSTRUCT_OBJECT(this) }

    SWIGOUT(%ignore HOM_BoundingRect(const UT_BoundingRectF &);)
    HOM_BoundingRect(const UT_BoundingRectF &rect)
	: myBoundingRect(rect.xmin(), rect.ymin(), rect.xmax(), rect.ymax())
    { HOM_CONSTRUCT_OBJECT(this) }

    ~HOM_BoundingRect()
    { HOM_DESTRUCT_OBJECT(this) }

    bool operator==(HOM_PtrOrNull<HOM_BoundingRect> rect)
    { return rect.myPointer && myBoundingRect==rect.myPointer->myBoundingRect; }

    bool operator!=(HOM_PtrOrNull<HOM_BoundingRect> rect)
    { return !operator==(rect); }

    bool isAlmostEqual(HOM_BoundingRect &rect, double tolerance=0.00001);

    int __hash__();
    std::string __str__();
    std::string __repr__();

    bool isValid()
    { return myBoundingRect.isValid(); }

    void setTo(const std::vector<double> &tuple);
    void translate(const std::vector<double> &tuple);
    void scale(const std::vector<double> &tuple);
    void expand(const std::vector<double> &tuple);

    HOM_Vector2 min()
    { return HOM_Vector2(myBoundingRect.xmin(), myBoundingRect.ymin()); }
    HOM_Vector2 max()
    { return HOM_Vector2(myBoundingRect.xmax(), myBoundingRect.ymax()); }
    HOM_Vector2 size()
    { return HOM_Vector2(myBoundingRect.sizeX(), myBoundingRect.sizeY()); }
    HOM_Vector2	center()
    { return HOM_Vector2(myBoundingRect.centerX(), myBoundingRect.centerY()); }
    HOM_Vector2	getOffsetToAvoid(HOM_BoundingRect &bounds,
	HOM_Vector2 *direction = NULL);

    void enlargeToContain(const std::vector<double> &point);
    void enlargeToContain(const HOM_BoundingRect &rect)
    { myBoundingRect.enlargeBounds(rect.myBoundingRect); }
    void intersect(const HOM_BoundingRect &rect)
    { myBoundingRect.intersectBounds(rect.myBoundingRect); }

    bool intersects(const std::vector<double> &point0,
	    const std::vector<double> &point1,
	    const std::vector<double> &point2);
    bool intersects(const std::vector<double> &point0,
	    const std::vector<double> &point1);
    bool intersects(const HOM_BoundingRect &rect)
    { return myBoundingRect.intersects(rect.myBoundingRect); }

    bool contains(const std::vector<double> &point);
    bool contains(const HOM_BoundingRect &rect)
    { return rect.myBoundingRect.isInside(myBoundingRect); }
    HOM_Vector2 closestPoint(const std::vector<double> &point);

    SWIGOUT(%ignore operator=;)
    HOM_BoundingRect &operator=(const HOM_BoundingRect& rect)
    {
	myBoundingRect = rect.myBoundingRect;
	return *this;
    }

    void *_asVoidPointer()
    { return &myBoundingRect; }

    SWIGOUT(%ignore myBoundingRect;)
    UT_BoundingRectD myBoundingRect;
};

#endif
