/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Vector4_h__
#define __HOM_Vector4_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_Vector3.h"
#include "HOM_Matrix4.h"
#include "HOM_PtrOrNull.h"
#include <UT/UT_Vector4.h>
#include <vector>
#include <stdexcept>
class HOM_Vector4;

SWIGOUT(%rename(Vector4) HOM_Vector4;)

class HOM_API HOM_Vector4
{
public:
    HOM_Vector4()
    : myVector4(0.0, 0.0, 0.0, 0.0)
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_Vector4(double x, double y, double z, double w)
    : myVector4(x, y, z, w)
    { HOM_CONSTRUCT_OBJECT(this) }

    SWIGOUT(%ignore HOM_Vector4(const UT_Vector4D&);)
    HOM_Vector4(const UT_Vector4D &vector4)
    : myVector4(vector4)
    { HOM_CONSTRUCT_OBJECT(this) }

    SWIGOUT(%ignore HOM_Vector4(const UT_Vector4F&);)
    HOM_Vector4(const UT_Vector4F &vector4)
    : myVector4(vector4)
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_Vector4(const std::vector<double> &sequence)
    {
        HOM_CONSTRUCT_OBJECT(this)
        setTo(sequence);
    }

    HOM_Vector4(HOM_Vector3 &vector3);

    ~HOM_Vector4()
    { HOM_DESTRUCT_OBJECT(this) }

    bool operator==(HOM_PtrOrNull<HOM_Vector4> vector4)
    { return vector4.myPointer && myVector4 == vector4.myPointer->myVector4; }

    bool operator!=(HOM_PtrOrNull<HOM_Vector4> vector4)
    { return !operator==(vector4); }

    bool isAlmostEqual(HOM_Vector4 &vector4, double tolerance=0.00001);

    // This method is deprecated.
    bool almostEqual(HOM_Vector4 &vector4, double tolerance=0.00001)
    { return isAlmostEqual(vector4, tolerance); }

    int __hash__();
    std::string __str__();
    std::string __repr__();

    double __getitem__(int index);
    void __setitem__(int index, double value);
    int __len__()
    { return 4; }

    // operator[] exists only to make this class easier to use from C++,
    // and isn't wrapped by swig.
    SWIGOUT(%ignore operator[];)
    double &operator[](int index);

    void setTo(const std::vector<double> &sequence);

    SWIGOUT(%ignore operator=;)
    HOM_Vector4 &operator=(const HOM_Vector4& vector4)
    {
	myVector4 = vector4.myVector4;
	return *this;
    }

    HOM_Vector4 __add__(HOM_Vector4 &vector4)
    { return HOM_Vector4(myVector4 + vector4.myVector4); }
    HOM_Vector4 __sub__(HOM_Vector4 &vector4)
    { return HOM_Vector4(myVector4 - vector4.myVector4); }
    HOM_Vector4 __mul__(double scalar)
    { return HOM_Vector4(myVector4 * scalar); }
    HOM_Vector4 __rmul__(double scalar)
    { return HOM_Vector4(myVector4 * scalar); }
    HOM_Vector4 __mul__(HOM_Matrix4 &matrix4);
    HOM_Vector4 __div__(double scalar)
    { return HOM_Vector4(myVector4 / scalar); }

    HOM_Vector4 normalized();
    double length()
    { return myVector4.length(); }
    double lengthSquared()
    { return myVector4.length2(); }

    double dot(HOM_Vector4 &vector4)
    { return myVector4.dot(vector4.myVector4); }

    HOM_Vector4 ocio_transform(const std::string &src_space,
				const std::string &dest_space) const;

    double x()
    { return myVector4.x(); }
    double y()
    { return myVector4.y(); }
    double z()
    { return myVector4.z(); }
    double w()
    { return myVector4.w(); }

    void *_asVoidPointer()
    { return &myVector4; }

    SWIGOUT(%ignore myVector4;)
    UT_Vector4D myVector4;
};

#endif
