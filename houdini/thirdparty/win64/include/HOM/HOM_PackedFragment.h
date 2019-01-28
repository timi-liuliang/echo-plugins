/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_PackedFragment_h__
#define __HOM_PackedFragment_h__

#include "HOM_API.h"
#include "HOM_PackedPrim.h"
#include "HOM_Geometry.h"

SWIGOUT(%rename(PackedFragment) HOM_PackedFragment;)

class HOM_API HOM_PackedFragment : virtual public HOM_PackedPrim
{
public:
    HOM_PackedFragment()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_PackedFragment(const HOM_PackedFragment &prim)
    : HOM_Prim(prim), HOM_PackedPrim(prim)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_PackedFragment()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual void setEmbeddedGeometry(HOM_Geometry &geometry,
				     const char *attrib, const char *name) = 0;
};

#endif
