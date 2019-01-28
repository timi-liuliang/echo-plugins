/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Keyframe_h__
#define __HOM_Keyframe_h__

#include "HOM_API.h"
#include "HOM_BaseKeyframe.h"
#include "HOM_EnumModules.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"

SWIGOUT(%rename(Keyframe) HOM_Keyframe;)

SWIGOUT(%feature("notabstract") HOM_Keyframe;)
class HOM_API HOM_Keyframe : public HOM_BaseKeyframe
{
public:

#ifdef SWIG
%extend
{
    HOM_Keyframe()
    { return HOM().newKeyframe(); }
    HOM_Keyframe(double value)
    { return HOM().newKeyframe(value); }
    HOM_Keyframe(double value, double time)
    { return HOM().newKeyframe(value, time); }
    HOM_Keyframe(const HOM_Keyframe &keyframe)
    { return HOM().newKeyframe(keyframe); }
}
#else
    HOM_Keyframe();
    HOM_Keyframe(double value);
    HOM_Keyframe(double value, double time);
    HOM_Keyframe(const HOM_Keyframe &keyframe);
#endif

    virtual ~HOM_Keyframe();

    virtual bool operator==(HOM_PtrOrNull<HOM_BaseKeyframe> keyframe);

    virtual int __hash__();
    virtual std::string __repr__();

    virtual HOM_EnumValue &evaluatedType()
    { return HOM_parmData::Float; }

    void setValue(double value);
    double value();
    bool isValueSet();

    // If you set the in value and the (out) value is not set, it will be
    // set to the same value.  Setting the in value breaks the tie between
    // the values.  If neither of the in or (out) values are set, they
    // are considered tied.
    void setInValue(double in_value);
    void unsetInValue();
    double inValue();
    bool isValueTied();
    bool isValueUsed();
    void useValue(bool use);

    void setSlope(double slope);
    double slope();
    bool isSlopeSet();
    bool isSlopeUsed();
    void useSlope(bool use);
    bool isSlopeAuto();
    void setSlopeAuto(bool on);

    void setInSlope(double in_slope);
    void unsetInSlope();
    double inSlope();
    bool isSlopeTied();
    bool isInSlopeAuto();
    void setInSlopeAuto(bool on);

    void setAccel(double accel);
    double accel();
    bool isAccelSet();
    bool isAccelUsed();
    void useAccel(bool use);

    void setInAccel(double in_accel);
    void unsetInAccel();
    double inAccel();
    bool isAccelTied();
    void interpretAccelAsRatio(bool on);
    bool isAccelInterpretedAsRatio();

private:
    double myValue;
    bool myIsValueSet;
    bool myIsValueUsed;
    double myInValue;
    bool myIsValueTied;

    double mySlope;
    bool myIsSlopeSet;
    bool myIsSlopeUsed;
    double myInSlope;
    bool myIsSlopeTied;
    bool myIsSlopeAuto;
    bool myIsInSlopeAuto;

    double myAccel;
    bool myIsAccelSet;
    bool myIsAccelUsed;
    double myInAccel;
    bool myIsAccelTied;
    bool myAccelAsRatio;
};

#endif
