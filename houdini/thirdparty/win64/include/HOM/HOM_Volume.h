/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Volume_h__
#define __HOM_Volume_h__

#include "HOM_API.h"
#include "HOM_BinaryString.h"
#include "HOM_Prim.h"

SWIGOUT(%rename(Volume) HOM_Volume;)

class HOM_API HOM_Volume : virtual public HOM_Prim
{
public:
    HOM_Volume()
    { HOM_CONSTRUCT_OBJECT(this) }

    // Because the lowermost base classes initialize the virtual bases
    // before any non-virtual bases, the correct thing to do here is
    // explicitly call the constructor for HOM_Prim.
    HOM_Volume(const HOM_Volume &volume)
    : HOM_Prim(volume)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_Volume()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual double sample(const std::vector<double> &position) = 0;

    SWIGOUT(%newobject gradient;)
    virtual HOM_Vector3 *gradient(const std::vector<double> &position) = 0;

    virtual double voxel(const std::vector<int> &index) = 0;

    virtual void setVoxel(const std::vector<int> &index, double value) = 0;

    virtual std::vector<float> allVoxels() = 0;

    virtual HOM_BinaryString allVoxelsAsString() = 0;

    virtual void setAllVoxels(const std::vector<float> &values) = 0;

    SWIGOUT(%ignore opaqueSetAllVoxelsFromString);
    virtual void opaqueSetAllVoxelsFromString(const char *values, int64 length) = 0;

#ifdef SWIG
%extend
{
    void setAllVoxelsFromString(InterpreterObject values)
    {
	HOM_PyBuffer buffer(values);
	self->opaqueSetAllVoxelsFromString(
	    (const char *)buffer.myData, buffer.myLength);
    }
}
#endif

    virtual std::vector<float> voxelSlice(const char *plane, int index) = 0;

    virtual HOM_BinaryString voxelSliceAsString(const char *plane, int index) = 0;

    virtual void setVoxelSlice(
	    const std::vector<float> &values, const char *plane, int index) = 0;

    SWIGOUT(%ignore opaqueSetVoxelSliceFromString);
    virtual void opaqueSetVoxelSliceFromString(
	    const char *values, int64 length, const char *plane, int index) = 0;

#ifdef SWIG
%extend
{
    void setVoxelSliceFromString(
	    InterpreterObject values, const char *plane, int index)
    {
	HOM_PyBuffer buffer(values);
	self->opaqueSetVoxelSliceFromString(
	    (const char *)buffer.myData, buffer.myLength, plane, index);
    }
}
#endif

    virtual std::vector<int> resolution() = 0;

    SWIGOUT(%newobject indexToPos;)
    virtual HOM_Vector3 *indexToPos(const std::vector<int> &index) = 0;

    virtual std::vector<int> posToIndex(const std::vector<double> &position) = 0;

    virtual bool isValidIndex(const std::vector<int> &index) = 0;

    virtual bool isSDF() = 0;

    virtual bool isHeightField() = 0;

    virtual double volumeMax() = 0;

    virtual double volumeMin() = 0;

    virtual double volumeAverage() = 0;

    SWIGOUT(%newobject transform;)
    virtual HOM_Matrix3 *transform() = 0;

    virtual void setTransform(HOM_Matrix4 &transform) = 0;

    virtual double taperX() = 0;
    virtual double taperY() = 0;

    SWIGOUT(%newobject vertex;)
    virtual HOM_Vertex *vertex(int index) = 0;
};

#endif
