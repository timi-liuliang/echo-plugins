/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Quadric_h__
#define __HOM_Quadric_h__

#include "HOM_API.h"
#include "HOM_Prim.h"

SWIGOUT(%rename(Quadric) HOM_Quadric;)

class HOM_API HOM_Quadric : virtual public HOM_Prim
{
public:
    HOM_Quadric()
    { HOM_CONSTRUCT_OBJECT(this) }

    // Because the lowermost base classes initialize the virtual bases
    // before any non-virtual bases, the correct thing to do here is
    // explicitly call the constructor for HOM_Prim.
    HOM_Quadric(const HOM_Quadric &quadric)
    : HOM_Prim(quadric)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_Quadric()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    SWIGOUT(%newobject transform;)
    virtual HOM_Matrix3 *transform() = 0;

    // This method is deprecated.
    SWIGOUT(%newobject getTransform;)
    virtual HOM_Matrix3 *getTransform()
    { return transform(); }
    
    SWIGOUT(%newobject vertex;)
    virtual HOM_Vertex *vertex(int index) = 0;
};

#endif
