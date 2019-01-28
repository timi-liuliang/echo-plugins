/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NetworkAnimValue_h__
#define __HOM_NetworkAnimValue_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Vector2.h"
#include "HOM_Vector3.h"
#include "HOM_Vector4.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_Vector4.h>
#include <string>

SWIGOUT(%rename(NetworkAnimValue) HOM_NetworkAnimValue;)

class HOM_API HOM_NetworkAnimValue
{
public:
    HOM_NetworkAnimValue()
	: myLength(0),
	  myDuration(0.0),
	  myValue1(0.0, 0.0, 0.0, 0.0),
	  myValue2(0.0, 0.0, 0.0, 0.0)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkAnimValue(double v)
	: myLength(1),
	  myDuration(0.0),
	  myValue1(v, 0.0, 0.0, 0.0),
	  myValue2(v, 0.0, 0.0, 0.0)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkAnimValue(HOM_Vector2 &v)
	: myLength(2),
	  myDuration(0.0),
	  myValue1(v.x(), v.y(), 0.0, 0.0),
	  myValue2(v.x(), v.y(), 0.0, 0.0)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkAnimValue(HOM_Vector3 &v)
	: myLength(3),
	  myDuration(0.0),
	  myValue1(v.x(), v.y(), v.z(), 0.0),
	  myValue2(v.x(), v.y(), v.z(), 0.0)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkAnimValue(HOM_Vector4 &v)
	: myLength(4),
	  myDuration(0.0),
	  myValue1(v.x(), v.y(), v.z(), v.w()),
	  myValue2(v.x(), v.y(), v.z(), v.w())
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkAnimValue(double duration, double v1, double v2)
	: myLength(1),
	  myDuration(duration),
	  myValue1(v1, 0.0, 0.0, 0.0),
	  myValue2(v2, 0.0, 0.0, 0.0)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkAnimValue(double duration, HOM_Vector2 &v1, HOM_Vector2 &v2)
	: myLength(2),
	  myDuration(duration),
	  myValue1(v1.x(), v1.y(), 0.0, 0.0),
	  myValue2(v2.x(), v2.y(), 0.0, 0.0)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkAnimValue(double duration, HOM_Vector3 &v1, HOM_Vector3 &v2)
	: myLength(3),
	  myDuration(duration),
	  myValue1(v1.x(), v1.y(), v1.z(), 0.0),
	  myValue2(v2.x(), v2.y(), v2.z(), 0.0)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkAnimValue(double duration, HOM_Vector4 &v1, HOM_Vector4 &v2)
	: myLength(4),
	  myDuration(duration),
	  myValue1(v1.x(), v1.y(), v1.z(), v1.w()),
	  myValue2(v2.x(), v2.y(), v2.z(), v2.w())
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkAnimValue(const HOM_NetworkAnimValue &v)
	: myLength(v.myLength),
	  myDuration(v.myDuration),
	  myValue1(v.myValue1),
	  myValue2(v.myValue2)
    { HOM_CONSTRUCT_OBJECT(this) }
    ~HOM_NetworkAnimValue()
    { HOM_DESTRUCT_OBJECT(this) }

    std::string __repr__();
    bool operator==(HOM_PtrOrNull<HOM_NetworkAnimValue> item);
    bool operator!=(HOM_PtrOrNull<HOM_NetworkAnimValue> item);

    SWIGOUT(%ignore myLength;)
    int			 myLength;
    SWIGOUT(%ignore myDuration;)
    fpreal		 myDuration;
    SWIGOUT(%ignore myValue1;)
    UT_Vector4D		 myValue1;
    SWIGOUT(%ignore myValue2;)
    UT_Vector4D		 myValue2;
};

#endif
