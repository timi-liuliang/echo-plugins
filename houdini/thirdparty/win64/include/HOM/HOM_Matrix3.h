/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Matrix3_h__
#define __HOM_Matrix3_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_Vector3.h"
#include "HOM_PtrOrNull.h"
#include <UT/UT_DMatrix3.h>
#include <vector>

SWIGOUT(%rename(Matrix3) HOM_Matrix3;)

class HOM_API HOM_Matrix3
{
public:
    HOM_Matrix3()
    {
	HOM_CONSTRUCT_OBJECT(this)
	myMatrix3.zero();
    }
    HOM_Matrix3(double diagonal_value)
    {
	HOM_CONSTRUCT_OBJECT(this)
	myMatrix3 = diagonal_value;
    }
    HOM_Matrix3(const std::vector<double>&values)
    {
	HOM_CONSTRUCT_OBJECT(this);
	setTo(values);
    }
    HOM_Matrix3(const std::vector<std::vector<double> > &values)
    {
	HOM_CONSTRUCT_OBJECT(this);
	setTo(values);
    }

    SWIGOUT(%ignore HOM_Matrix3(const UT_DMatrix3&);)
    HOM_Matrix3(const UT_DMatrix3 &matrix3)
    : myMatrix3(matrix3)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Matrix3(const UT_Matrix3 &matrix3)
    : myMatrix3(matrix3)
    { HOM_CONSTRUCT_OBJECT(this) }

    ~HOM_Matrix3()
    { HOM_DESTRUCT_OBJECT(this) }

    bool operator==(HOM_PtrOrNull<HOM_Matrix3> matrix3)
    { return matrix3.myPointer && myMatrix3 == matrix3.myPointer->myMatrix3; }

    bool operator!=(HOM_PtrOrNull<HOM_Matrix3> matrix3)
    { return !operator==(matrix3); }

    bool isAlmostEqual(HOM_Matrix3 &matrix3, double tolerance=0.00001);

    int __hash__();
    std::string __str__();
    std::string __repr__();

    SWIGOUT(%ignore operator=;)
    HOM_Matrix3 &operator=(const std::vector<std::vector<double> > &tuple);
    HOM_Matrix3 &operator=(const HOM_Matrix3& matrix3)
    {
	myMatrix3 = matrix3.myMatrix3;
	return *this;
    }

    double at(int row, int col);

    std::vector<double> asTuple();
    std::vector<std::vector<double> > asTupleOfTuples();

    void setAt(int row, int col, double value);
    void setTo(const std::vector<double> &tuple);
    void setTo(const std::vector<std::vector<double> > &tuple);

    void setToIdentity()
    { myMatrix3.identity(); }

    void setToZero()
    { myMatrix3.zero(); }

    HOM_Matrix3 __add__(HOM_Matrix3 &matrix3)
    { return HOM_Matrix3(myMatrix3 + matrix3.myMatrix3); }
    HOM_Matrix3 __sub__(HOM_Matrix3 &matrix3)
    { return HOM_Matrix3(myMatrix3 - matrix3.myMatrix3); }
    HOM_Matrix3 __mul__(HOM_Matrix3 &matrix3)
    { return HOM_Matrix3(myMatrix3 * matrix3.myMatrix3); }
    HOM_Matrix3 __mul__(double scalar)
    { return HOM_Matrix3(myMatrix3 * scalar); }
    HOM_Matrix3 preMult(HOM_Matrix3 &matrix3)
    { return HOM_Matrix3(matrix3.myMatrix3 * myMatrix3); }

    HOM_Matrix3 transposed();
    HOM_Matrix3 inverted();
    double determinant()
    { return myMatrix3.determinant(); }

    SWIGOUT(%kwargs extractRotates;)
    HOM_Vector3 extractRotates(const char *rotate_order="xyz");

    void *_asVoidPointer()
    { return &myMatrix3; }


    SWIGOUT(%ignore myMatrix3;)
    UT_DMatrix3 myMatrix3;
};

#endif
