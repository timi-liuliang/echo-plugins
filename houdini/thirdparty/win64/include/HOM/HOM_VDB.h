/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_VDB_h__
#define __HOM_VDB_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Prim.h"
#include <UT/UT_VDBUtils.h>


class HOM_BoundingBox;
class HOM_Vector3;


SWIGOUT(%rename(VDB) HOM_VDB;)


class HOM_API HOM_VDB : virtual public HOM_Prim
{
public:
    HOM_VDB()
    { HOM_CONSTRUCT_OBJECT(this) }

    // Because the lowermost base classes initialize the virtual bases
    // before any non-virtual bases, the correct thing to do here is
    // explicitly call the constructor for HOM_Prim.
    HOM_VDB(const HOM_VDB &volume)
    : HOM_Prim(volume)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_VDB()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual double sample(const std::vector<double> &position) = 0;

    SWIGOUT(%newobject gradient;)
    virtual HOM_Vector3 *gradient(const std::vector<double> &position) = 0;

    virtual double voxel(const std::vector<int> &index) = 0;

    virtual std::vector<int> resolution() = 0;

    SWIGOUT(%newobject indexToPos;)
    virtual HOM_Vector3 *indexToPos(const std::vector<int> &index) = 0;

    virtual std::vector<int> posToIndex(const std::vector<double> &position) = 0;

    virtual bool isSDF() = 0;

    SWIGOUT(%newobject transform;)
    virtual HOM_Matrix3 *transform() = 0;

    virtual double taper() = 0;

    SWIGOUT(%newobject vertex;)
    virtual HOM_Vertex *vertex(int index) = 0;

    // 
    // VDB-specific accessors
    //

    virtual bool isEmpty() = 0;

    virtual int activeVoxelCount() = 0;

    virtual HOM_BoundingBox activeVoxelBoundingBox() = 0;

    virtual HOM_Vector3 voxelSize() = 0;

    virtual HOM_EnumValue &dataType() = 0;

    virtual std::vector<bool> voxelRangeAsBool(HOM_BoundingBox &range) = 0;

    virtual std::vector<double> voxelRangeAsFloat(HOM_BoundingBox &range) = 0;

    virtual std::vector<int> voxelRangeAsInt(HOM_BoundingBox &range) = 0;

    virtual std::vector<HOM_Vector3> voxelRangeAsVector3(HOM_BoundingBox &range) = 0;

#ifdef SWIG
%extend
{
    InterpreterObject voxelRange(HOM_BoundingBox &range)
    {
	const HOM_EnumValue &data_type = self->dataType();
	UT_ASSERT(data_type.enumClass() == typeid(HOM_vdbData));
	switch (data_type.id())
	{
	case HOM_vdbData::Boolean_Id:
	    return swig::from(self->voxelRangeAsBool(range));
	case HOM_vdbData::Float_Id:
	    return swig::from(self->voxelRangeAsFloat(range));
	case HOM_vdbData::Int_Id:
	    return swig::from(self->voxelRangeAsInt(range));
	case HOM_vdbData::Vector3_Id:
	    return swig::from(self->voxelRangeAsVector3(range));
	}

	UT_ASSERT(!"Unknown vdb data type");
	return SWIG_Py_Void();
    }
}
#endif

};

#endif
