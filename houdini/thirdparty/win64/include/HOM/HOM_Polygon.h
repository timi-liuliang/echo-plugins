/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Polygon_h__
#define __HOM_Polygon_h__

#include "HOM_API.h"
#include "HOM_Face.h"

SWIGOUT(%rename(Polygon) HOM_Polygon;)

class HOM_API HOM_Polygon : virtual public HOM_Face
{
public:
    HOM_Polygon()
    { HOM_CONSTRUCT_OBJECT(this) }

    // Because the lowermost base classes initialize the virtual bases
    // before any non-virtual bases, the correct thing to do here is
    // explicitly call the constructor for HOM_Prim.
    HOM_Polygon(const HOM_Polygon &polygon)
    : HOM_Prim(polygon), HOM_Face(polygon)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_Polygon()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)
};

#endif
