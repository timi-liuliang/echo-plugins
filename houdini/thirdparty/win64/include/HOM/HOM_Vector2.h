/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Vector2_h__
#define __HOM_Vector2_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Matrix2.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"
#include <UT/UT_Vector2.h>
#include <vector>
#include <stdexcept>

SWIGOUT(%rename(Vector2) HOM_Vector2;)

class HOM_API HOM_Vector2
{
public:
    HOM_Vector2()
    : myVector2(0.0, 0.0)
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_Vector2(double x, double y)
    : myVector2(x, y)
    { HOM_CONSTRUCT_OBJECT(this) }

    SWIGOUT(%ignore HOM_Vector2(const UT_Vector2D&);)
    HOM_Vector2(const UT_Vector2D &vector2)
    : myVector2(vector2)
    { HOM_CONSTRUCT_OBJECT(this) }

    SWIGOUT(%ignore HOM_Vector2(const UT_Vector2F&);)
    HOM_Vector2(const UT_Vector2F &vector2)
    : myVector2(vector2)
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_Vector2(const std::vector<double> &sequence)
    {
        HOM_CONSTRUCT_OBJECT(this)
        setTo(sequence);
    }

    ~HOM_Vector2()
    { HOM_DESTRUCT_OBJECT(this) }

    bool operator==(HOM_PtrOrNull<HOM_Vector2> vector2)
    { return vector2.myPointer && myVector2 == vector2.myPointer->myVector2; }

    bool operator!=(HOM_PtrOrNull<HOM_Vector2> vector2)
    { return !operator==(vector2); }

    bool isAlmostEqual(HOM_Vector2 &vector2, double tolerance=0.00001);

    // This method is deprecated.
    bool almostEqual(HOM_Vector2 &vector2, double tolerance=0.00001)
    { return isAlmostEqual(vector2, tolerance); }

    int __hash__();
    std::string __str__();
    std::string __repr__();

    double __getitem__(int index);
    void __setitem__(int index, double value);
    int __len__()
    { return 2; }

    // operator[] exists only to make this class easier to use from C++,
    // and isn't wrapped by swig.
    SWIGOUT(%ignore operator[];)
    double &operator[](int index);

    void setTo(const std::vector<double> &sequence);

    SWIGOUT(%ignore operator=;)
    HOM_Vector2 &operator=(const HOM_Vector2& vector2)
    {
	myVector2 = vector2.myVector2;
	return *this;
    }

    HOM_Vector2 __add__(HOM_Vector2 &vector2)
    { return HOM_Vector2(myVector2 + vector2.myVector2); }
    HOM_Vector2 __sub__(HOM_Vector2 &vector2)
    { return HOM_Vector2(myVector2 - vector2.myVector2); }
    HOM_Vector2 __neg__()
    { return HOM_Vector2(-myVector2); }
    HOM_Vector2 __mul__(double scalar)
    { return HOM_Vector2(myVector2 * scalar); }
    HOM_Vector2 __rmul__(double scalar)
    { return HOM_Vector2(myVector2 * scalar); }
    HOM_Vector2 __mul__(const HOM_Matrix2 &matrix2)
    { return HOM_Vector2(myVector2 * matrix2.myMatrix2); }
    HOM_Vector2 __div__(double scalar)
    { return HOM_Vector2(myVector2 / scalar); }

    HOM_Vector2 normalized();
    double length()
    { return myVector2.length(); }
    double lengthSquared()
    { return myVector2.length2(); }

    double distanceTo(HOM_Vector2 &vector2);

    double dot(HOM_Vector2 &vector2)
    { return myVector2.dot(vector2.myVector2); }

    double x()
    { return myVector2.x(); }
    double y()
    { return myVector2.y(); }

    void *_asVoidPointer()
    { return &myVector2; }

    SWIGOUT(%ignore myVector2;)
    UT_Vector2D myVector2;
};

#endif
