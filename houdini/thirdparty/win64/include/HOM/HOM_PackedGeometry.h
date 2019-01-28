/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_PackedGeometry_h__
#define __HOM_PackedGeometry_h__

#include "HOM_API.h"
#include "HOM_PackedPrim.h"
#include "HOM_Geometry.h"

SWIGOUT(%rename(PackedGeometry) HOM_PackedGeometry;)

class HOM_API HOM_PackedGeometry : virtual public HOM_PackedPrim
{
public:
    HOM_PackedGeometry()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_PackedGeometry(const HOM_PackedGeometry &prim)
    : HOM_Prim(prim), HOM_PackedPrim(prim)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_PackedGeometry()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    SWIGOUT(%newobject getEmbeddedGeometry;)
    virtual HOM_Geometry * getEmbeddedGeometry() = 0;

    virtual void setEmbeddedGeometry(HOM_Geometry &geometry) = 0;
};

#endif
