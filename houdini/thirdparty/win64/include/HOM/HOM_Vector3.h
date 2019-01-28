/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Vector3_h__
#define __HOM_Vector3_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"
#include <UT/UT_Vector3.h>
#include <vector>
#include <stdexcept>
class HOM_Matrix4;
class HOM_Vector4;
class HOM_Matrix3;

SWIGOUT(%rename(Vector3) HOM_Vector3;)

class HOM_API HOM_Vector3
{
public:
    HOM_Vector3()
    : myVector3(0.0, 0.0, 0.0)
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_Vector3(double x, double y, double z)
    : myVector3(x, y, z)
    { HOM_CONSTRUCT_OBJECT(this) }

    SWIGOUT(%ignore HOM_Vector3(const UT_Vector3D&);)
    HOM_Vector3(const UT_Vector3D &vector3)
    : myVector3(vector3)
    { HOM_CONSTRUCT_OBJECT(this) }

    SWIGOUT(%ignore HOM_Vector3(const UT_Vector3F&);)
    HOM_Vector3(const UT_Vector3F &vector3)
    : myVector3(vector3)
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_Vector3(const std::vector<double> &sequence)
    {
        HOM_CONSTRUCT_OBJECT(this)
        setTo(sequence);
    }

    HOM_Vector3(HOM_Vector4 &vector4);

    ~HOM_Vector3()
    { HOM_DESTRUCT_OBJECT(this) }

    bool operator==(HOM_PtrOrNull<HOM_Vector3> vector3)
    { return vector3.myPointer && myVector3 == vector3.myPointer->myVector3; }

    bool operator!=(HOM_PtrOrNull<HOM_Vector3> vector3)
    { return !operator==(vector3); }

    bool isAlmostEqual(HOM_Vector3 &vector3, double tolerance=0.00001);

    // This method is deprecated.
    bool almostEqual(HOM_Vector3 &vector3, double tolerance=0.00001)
    { return isAlmostEqual(vector3, tolerance); }

    int __hash__();
    std::string __str__();
    std::string __repr__();

    double __getitem__(int index);
    void __setitem__(int index, double value);
    int __len__()
    { return 3; }

    // operator[] exists only to make this class easier to use from C++,
    // and isn't wrapped by swig.
    SWIGOUT(%ignore operator[];)
    double &operator[](int index);

    void setTo(const std::vector<double> &sequence);

    SWIGOUT(%ignore operator=;)
    HOM_Vector3 &operator=(const HOM_Vector3& vector3)
    {
	myVector3 = vector3.myVector3;
	return *this;
    }

    HOM_Vector3 __add__(HOM_Vector3 &vector3)
    { return HOM_Vector3(myVector3 + vector3.myVector3); }
    HOM_Vector3 __sub__(HOM_Vector3 &vector3)
    { return HOM_Vector3(myVector3 - vector3.myVector3); }
    HOM_Vector3 __neg__()
    { return HOM_Vector3(-myVector3); }
    HOM_Vector3 __mul__(double scalar)
    { return HOM_Vector3(myVector3 * scalar); }
    HOM_Vector3 __rmul__(double scalar)
    { return HOM_Vector3(myVector3 * scalar); }
    HOM_Vector3 __mul__(HOM_Matrix4 &matrix4);
    HOM_Vector3 __div__(double scalar)
    { return HOM_Vector3(myVector3 / scalar); }

    HOM_Vector3 normalized();
    HOM_Vector3 multiplyAsDir(HOM_Matrix4 &matrix4);
    double length()
    { return myVector3.length(); }
    double lengthSquared()
    { return myVector3.length2(); }


    SWIGOUT(%newobject matrixToRotateTo;)
    HOM_Matrix4 *matrixToRotateTo(HOM_Vector3 &vector3);

    SWIGOUT(%kwargs smoothRotation;)
    HOM_Vector3 smoothRotation(const HOM_Vector3 &reference,
                               const char *rotate_order = "xyz");

    double distanceTo(HOM_Vector3 &vector3);
    double angleTo(HOM_Vector3 &vector3);

    double dot(HOM_Vector3 &vector3)
    { return myVector3.dot(vector3.myVector3); }
    HOM_Vector3 cross(HOM_Vector3 &vector3)
    { return ::cross(myVector3, vector3.myVector3); }

    HOM_Vector3 ocio_transform(const std::string &src_space,
				const std::string &dest_space) const;

    double x()
    { return myVector3.x(); }
    double y()
    { return myVector3.y(); }
    double z()
    { return myVector3.z(); }

    void *_asVoidPointer()
    { return &myVector3; }


    SWIGOUT(%ignore myVector3;)
    UT_Vector3D myVector3;
};

#endif
