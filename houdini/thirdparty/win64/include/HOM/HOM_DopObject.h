/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_DopObject_h__
#define __HOM_DopObject_h__

#include "HOM_API.h"
#include "HOM_DopData.h"

class HOM_Matrix4;
class HOM_Geometry;
class HOM_EditableDopGeometryGuard;

SWIGOUT(%rename(DopObject) HOM_DopObject;)

class HOM_API HOM_DopObject : virtual public HOM_DopData
{
public:
    HOM_DopObject()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_DopObject(const HOM_DopObject &dopobject)
    : HOM_DopData(dopobject)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_DopObject()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual std::string name() = 0;
    virtual int objid() = 0;

    virtual bool matches(const char *pattern) = 0;

    SWIGOUT(%newobject transform;)
    SWIGOUT(%kwargs transform;)
    virtual HOM_Matrix4 *transform(bool include_geometry_transform=true) = 0;

    SWIGOUT(%newobject geometry;)
    SWIGOUT(%kwargs geometry;)
    virtual HOM_Geometry *geometry(const char *data_name=NULL) = 0;

    SWIGOUT(%newobject editableGeometry;)
    SWIGOUT(%kwargs editableGeometry;)
    virtual HOM_EditableDopGeometryGuard *editableGeometry(
	    const char *data_name=NULL) = 0;
};

#endif

