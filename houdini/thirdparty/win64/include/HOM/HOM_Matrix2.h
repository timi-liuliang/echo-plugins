/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Matrix2_h__
#define __HOM_Matrix2_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_Vector3.h"
#include "HOM_PtrOrNull.h"
#include <UT/UT_Matrix2.h>
#include <vector>

SWIGOUT(%rename(Matrix2) HOM_Matrix2;)

class HOM_API HOM_Matrix2
{
public:
    HOM_Matrix2()
    {
	HOM_CONSTRUCT_OBJECT(this)
	myMatrix2.zero();
    }
    HOM_Matrix2(double diagonal_value)
    {
	HOM_CONSTRUCT_OBJECT(this)
	myMatrix2 = diagonal_value;
    }
    HOM_Matrix2(const std::vector<double>&values)
    {
	HOM_CONSTRUCT_OBJECT(this);
	setTo(values);
    }
    HOM_Matrix2(const std::vector<std::vector<double> > &values)
    {
	HOM_CONSTRUCT_OBJECT(this);
	setTo(values);
    }

    SWIGOUT(%ignore HOM_Matrix2(const UT_DMatrix2&);)
    HOM_Matrix2(const UT_DMatrix2 &matrix2)
    : myMatrix2(matrix2)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Matrix2(const UT_Matrix2 &matrix2)
    : myMatrix2(matrix2)
    { HOM_CONSTRUCT_OBJECT(this) }

    ~HOM_Matrix2()
    { HOM_DESTRUCT_OBJECT(this) }

    bool operator==(HOM_PtrOrNull<HOM_Matrix2> matrix2)
    { return matrix2.myPointer && myMatrix2 == matrix2.myPointer->myMatrix2; }

    bool operator!=(HOM_PtrOrNull<HOM_Matrix2> matrix2)
    { return !operator==(matrix2); }

    bool isAlmostEqual(HOM_Matrix2 &matrix2, double tolerance=0.00001);

    int __hash__();
    std::string __str__();
    std::string __repr__();

    SWIGOUT(%ignore operator=;)
    HOM_Matrix2 &operator=(const std::vector<std::vector<double> > &tuple);
    HOM_Matrix2 &operator=(const HOM_Matrix2& matrix2)
    {
	myMatrix2 = matrix2.myMatrix2;
	return *this;
    }

    double at(int row, int col);

    std::vector<double> asTuple();
    std::vector<std::vector<double> > asTupleOfTuples();

    void setAt(int row, int col, double value);
    void setTo(const std::vector<double> &tuple);
    void setTo(const std::vector<std::vector<double> > &tuple);

    void setToIdentity()
    { myMatrix2.identity(); }

    void setToZero()
    { myMatrix2.zero(); }

    HOM_Matrix2 __add__(HOM_Matrix2 &matrix2)
    { return HOM_Matrix2(myMatrix2 + matrix2.myMatrix2); }
    HOM_Matrix2 __sub__(HOM_Matrix2 &matrix2)
    { return HOM_Matrix2(myMatrix2 - matrix2.myMatrix2); }
    HOM_Matrix2 __mul__(HOM_Matrix2 &matrix2)
    { return HOM_Matrix2(myMatrix2 * matrix2.myMatrix2); }
    HOM_Matrix2 __mul__(double scalar)
    { return HOM_Matrix2(myMatrix2 * scalar); }
    HOM_Matrix2 preMult(HOM_Matrix2 &matrix2)
    { return HOM_Matrix2(matrix2.myMatrix2 * myMatrix2); }

    HOM_Matrix2 transposed();
    HOM_Matrix2 inverted();
    double determinant()
    { return myMatrix2.determinant(); }

    void *_asVoidPointer()
    { return &myMatrix2; }


    SWIGOUT(%ignore myMatrix2;)
    UT_DMatrix2 myMatrix2;
};

#endif
