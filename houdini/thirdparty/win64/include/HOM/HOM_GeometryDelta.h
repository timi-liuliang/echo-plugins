/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 * Geometry delta interface.
 */

#ifndef __HOM_GeometryDelta_h__
#define __HOM_GeometryDelta_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include <string>
class HOM_SopNode;

SWIGOUT(%rename(GeometryDelta) HOM_GeometryDelta;)

class HOM_API HOM_GeometryDelta
{
public:
    HOM_GeometryDelta()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_GeometryDelta(const HOM_GeometryDelta &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_GeometryDelta()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual void copyGeoPointAttributes(HOM_SopNode &src) = 0;
};

#endif