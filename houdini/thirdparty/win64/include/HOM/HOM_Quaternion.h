/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Quaternion_h__
#define __HOM_Quaternion_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_Vector3.h"
#include "HOM_PtrOrNull.h"
#include <UT/UT_Quaternion.h>
#include <vector>
#include <stdexcept>

class HOM_Matrix3;
class HOM_Matrix4;

SWIGOUT(%rename(Quaternion) HOM_Quaternion;)

class HOM_API HOM_Quaternion
{
public:
    HOM_Quaternion()
    : myQuaternion(0.0, 0.0, 0.0, 1.0)
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_Quaternion(const std::vector<double> &sequence)
    {
        HOM_CONSTRUCT_OBJECT(this)
        setTo(sequence);
    }

    HOM_Quaternion(double x, double y, double z, double w)
    : myQuaternion(x, y, z, w)
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_Quaternion(double angle_in_deg, const std::vector<double> &axis)
    {
	setToAngleAxis(angle_in_deg, axis);
	HOM_CONSTRUCT_OBJECT(this)
    }

    HOM_Quaternion(HOM_Matrix3 &matrix3);
    HOM_Quaternion(HOM_Matrix4 &matrix4);

    SWIGOUT(%ignore HOM_Quaternion(const UT_QuaternionD&);)
    HOM_Quaternion(const UT_QuaternionD &quaternion)
    : myQuaternion(quaternion)
    { HOM_CONSTRUCT_OBJECT(this) }

    ~HOM_Quaternion()
    { HOM_DESTRUCT_OBJECT(this) }

    bool operator==(HOM_PtrOrNull<HOM_Quaternion> quaternion)
    { return quaternion.myPointer &&
	myQuaternion == quaternion.myPointer->myQuaternion; }

    bool operator!=(HOM_PtrOrNull<HOM_Quaternion> quaternion)
    { return !operator==(quaternion); }

    bool isAlmostEqual(HOM_Quaternion &quaternion, double tolerance=0.00001);

    // This method is deprecated.
    bool almostEqual(HOM_Quaternion &quaternion, double tolerance=0.00001)
    { return isAlmostEqual(quaternion, tolerance); }

    int __hash__();
    std::string __str__();
    std::string __repr__();

    double __getitem__(int index);
    void __setitem__(int index, double value);
    int __len__()
    { return 4; }

    void setTo(const std::vector<double> &tuple);

    void setToRotationMatrix(HOM_Matrix3 &matrix3);
    void setToRotationMatrix(HOM_Matrix4 &matrix4);

    void setToAngleAxis(double angle_in_deg, const std::vector<double> &axis);

    void setToVectors(const HOM_Vector3 &v1, const HOM_Vector3 &v2);

    SWIGOUT(%kwargs setToEulerRotates;)
    void setToEulerRotates(
	    const std::vector<double> &angles_in_deg,
	    const char *rotate_order="xyz");

    SWIGOUT(%ignore operator=;)
    HOM_Quaternion &operator=(const HOM_Quaternion& quaternion)
    {
	myQuaternion = quaternion.myQuaternion;
	return *this;
    }

    HOM_Quaternion conjugate();
    HOM_Quaternion inverse();
    double dot(HOM_Quaternion &quaternion)
    { return ::dot(myQuaternion, quaternion.myQuaternion); }

    HOM_Quaternion __add__(HOM_Quaternion &quaternion)
    { return HOM_Quaternion(myQuaternion + quaternion.myQuaternion); }
    HOM_Quaternion __sub__(HOM_Quaternion &quaternion)
    { return HOM_Quaternion(myQuaternion - quaternion.myQuaternion); }
    HOM_Quaternion __mul__(double scalar)
    { return HOM_Quaternion(myQuaternion * scalar); }
    HOM_Quaternion __mul__(HOM_Quaternion &quaternion);

    HOM_Quaternion slerp(HOM_Quaternion &other, double fraction);


    HOM_Quaternion normalized();

    double length()
    { return myQuaternion.length(); }


    SWIGOUT(%newobject extractRotationMatrix3;)
    HOM_Matrix3 *extractRotationMatrix3();

    std::pair<double, HOM_ElemPtr<HOM_Vector3> > extractAngleAxis();

    HOM_Vector3 extractEulerRotates(const char *rotate_order="xyz");

    HOM_Vector3 rotate(const HOM_Vector3 &v);

    void *_asVoidPointer()
    { return &myQuaternion; }


    SWIGOUT(%ignore myQuaternion;)
    UT_QuaternionD myQuaternion;
};

#endif
