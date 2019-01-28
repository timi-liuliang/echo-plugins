/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Matrix4_h__
#define __HOM_Matrix4_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_Vector3.h"
#include "HOM_Matrix3.h"
#include "HOM_PtrOrNull.h"
#include <UT/UT_DMatrix4.h>
#include <vector>
#include <stdexcept>

SWIGOUT(%rename(Matrix4) HOM_Matrix4;)

class HOM_API HOM_Matrix4
{
public:
    HOM_Matrix4()
    {
	HOM_CONSTRUCT_OBJECT(this)
	myMatrix4.zero();
    }
    HOM_Matrix4(double diagonal_value)
    {
	HOM_CONSTRUCT_OBJECT(this)
	myMatrix4 = diagonal_value;
    }
    HOM_Matrix4(const std::vector<double> &values)
    {
	HOM_CONSTRUCT_OBJECT(this);
	setTo(values);
    }
    HOM_Matrix4(const std::vector<std::vector<double> > &values)
    {
	HOM_CONSTRUCT_OBJECT(this);
	setTo(values);
    }
    HOM_Matrix4(HOM_Matrix3 &matrix3)
    {
	HOM_CONSTRUCT_OBJECT(this);
	myMatrix4 = matrix3.myMatrix3;
    }

    SWIGOUT(%ignore HOM_Matrix4(const UT_DMatrix4&);)
    HOM_Matrix4(const UT_DMatrix4 &matrix4)
    : myMatrix4(matrix4)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Matrix4(const UT_Matrix4 &matrix4)
    : myMatrix4(matrix4)
    { HOM_CONSTRUCT_OBJECT(this) }

    ~HOM_Matrix4()
    { HOM_DESTRUCT_OBJECT(this) }

    bool operator==(HOM_PtrOrNull<HOM_Matrix4> matrix4)
    { return matrix4.myPointer && myMatrix4 == matrix4.myPointer->myMatrix4; }

    bool operator!=(HOM_PtrOrNull<HOM_Matrix4> matrix4)
    { return !operator==(matrix4); }

    bool isAlmostEqual(HOM_Matrix4 &matrix4, double tolerance=0.00001);

    int __hash__();
    std::string __str__();
    std::string __repr__();

    SWIGOUT(%ignore operator=;)
    HOM_Matrix4 &operator=(const std::vector<std::vector<double> > &sequence);
    HOM_Matrix4 &operator=(const HOM_Matrix4& matrix4)
    {
	myMatrix4 = matrix4.myMatrix4;
	return *this;
    }

    double at(int row, int col);

    std::vector<double> asTuple();
    std::vector<std::vector<double> > asTupleOfTuples();

    void setAt(int row, int col, double value);
    void setTo(const std::vector<double> &sequence);
    void setTo(const std::vector<std::vector<double> > &sequence);

    void setToIdentity()
    { myMatrix4.identity(); }

    void setToZero()
    { myMatrix4.zero(); }

    HOM_Matrix4 __add__(HOM_Matrix4 &matrix4)
    { return HOM_Matrix4(myMatrix4 + matrix4.myMatrix4); }
    HOM_Matrix4 __sub__(HOM_Matrix4 &matrix4)
    { return HOM_Matrix4(myMatrix4 - matrix4.myMatrix4); }
    HOM_Matrix4 __mul__(HOM_Matrix4 &matrix4)
    { return HOM_Matrix4(myMatrix4 * matrix4.myMatrix4); }
    HOM_Matrix4 __mul__(double scalar)
    { return HOM_Matrix4(myMatrix4 * scalar); }
    HOM_Matrix4 preMult(HOM_Matrix4 &matrix4)
    { return HOM_Matrix4(matrix4.myMatrix4 * myMatrix4); }

    HOM_Matrix4 transposed();
    HOM_Matrix4 inverted();
    double determinant()
    { return myMatrix4.determinant(); }

    SWIGOUT(%kwargs explode;)
    std::map<std::string, HOM_Vector3> explode(
	    const char *transform_order="srt",
	    const char *rotate_order="xyz",
	    const HOM_Vector3 &pivot=HOM_Vector3(),
	    const HOM_Vector3 &pivot_rotate=HOM_Vector3());

    SWIGOUT(%kwargs extractTranslates;)
    HOM_Vector3 extractTranslates(const char *transform_order="srt",
				  const HOM_Vector3 &pivot_rotate=HOM_Vector3());

    SWIGOUT(%kwargs extractRotates;)
    HOM_Vector3 extractRotates(
	    const char *transform_order="srt",
	    const char *rotate_order="xyz",
	    const HOM_Vector3 &pivot=HOM_Vector3(),
	    const HOM_Vector3 &pivot_rotate=HOM_Vector3());

    SWIGOUT(%kwargs extractScales;)
    HOM_Vector3 extractScales(
	    const char *transform_order="srt",
	    const HOM_Vector3 &pivot=HOM_Vector3(),
	    const HOM_Vector3 &pivot_rotate=HOM_Vector3());

    SWIGOUT(%kwargs extractShears;)
    HOM_Vector3 extractShears(
	    const char *transform_order="srt",
	    const HOM_Vector3 &pivot=HOM_Vector3(),
	    const HOM_Vector3 &pivot_rotate=HOM_Vector3());

    SWIGOUT(%newobject extractRotationMatrix3;)
    HOM_Matrix3 *extractRotationMatrix3();

    void *_asVoidPointer()
    { return &myMatrix4; }


    SWIGOUT(%ignore myMatrix4;)
    UT_DMatrix4 myMatrix4;
};

#endif
