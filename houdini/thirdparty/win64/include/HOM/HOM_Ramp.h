/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Ramp_h__
#define __HOM_Ramp_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_EnumModules.h"
#include "HOM_Module.h"
#include <UT/UT_Ramp.h>


SWIGOUT(%rename(Ramp) HOM_Ramp;)

class HOM_API HOM_Ramp
{
public:
    HOM_Ramp()
	: myIsColor(false)
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_Ramp(std::vector<HOM_EnumValue *> basis,
	     const std::vector<double> &keys,
	     const std::vector<double> &values);

    HOM_Ramp(std::vector<HOM_EnumValue *> basis,
	     const std::vector<double> &keys,
	     const std::vector<std::vector<double> > &values);

    ~HOM_Ramp()
    { HOM_DESTRUCT_OBJECT(this) }


    SWIGOUT(%ignore operator=;)
    HOM_Ramp &operator=(const HOM_Ramp& ramp)
    {
	if (this != &ramp)
	    myRamp = ramp.myRamp;
	return *this;
    }

    std::string __repr__();

    bool isColor()
    { return myIsColor; }

    HOM_EnumValue &colorType();
    void setColorType(HOM_EnumValue &enum_value);


    std::vector<HOM_EnumValue *> basis();

    std::vector<double> keys();

    void *_asVoidPointer()
    { return &myRamp; }


    SWIGOUT(%ignore getUTRamp;)
    UT_Ramp &getUTRamp()
    { return myRamp; }

    SWIGOUT(%ignore setIsColor;)
    void setIsColor(bool is_color)
    { myIsColor = is_color; }

    SWIGOUT(%ignore rampLookup;)
    hboost::any rampLookup(double pos);

    SWIGOUT(%ignore rampValues;)
    hboost::any rampValues();

    // Internal converters
    SWIGOUT(%ignore splineBasisForEnumValue;)
    static UT_SPLINE_BASIS splineBasisForEnumValue(
	HOM_EnumValue &enum_value);
    SWIGOUT(%ignore splineBasisToEnumValue;)
    static HOM_EnumValue & splineBasisToEnumValue(UT_SPLINE_BASIS basis);

#ifdef SWIG
%extend
{
    InterpreterObject lookup(double pos)
    {
	return HOMboostAnyToInterpreterObject(self->rampLookup(pos));
    }

    InterpreterObject values()
    {
	return HOMboostAnyToInterpreterObject(self->rampValues());
    }
}
#endif

private:
    UT_Ramp myRamp;
    bool myIsColor;
};

#endif
