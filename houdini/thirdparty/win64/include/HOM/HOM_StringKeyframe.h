/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_StringKeyframe_h__
#define __HOM_StringKeyframe_h__

#include "HOM_API.h"
#include "HOM_BaseKeyframe.h"

SWIGOUT(%rename(StringKeyframe) HOM_StringKeyframe;)

SWIGOUT(%feature("notabstract") HOM_StringKeyframe;)
class HOM_API HOM_StringKeyframe : public HOM_BaseKeyframe
{
public:
#ifdef SWIG
%extend
{
    HOM_StringKeyframe()
    { return HOM().newStringKeyframe(); }

    HOM_StringKeyframe(const std::string &value,
	HOM_EnumValue &language = HOM_exprLanguage::Python)
    { return HOM().newStringKeyframe(value, language); }

    HOM_StringKeyframe(const std::string &value, double time,
	HOM_EnumValue &language = HOM_exprLanguage::Python)
    { return HOM().newStringKeyframe(value, time, language); }

    HOM_StringKeyframe(const HOM_StringKeyframe &string_keyframe)
    { return HOM().newStringKeyframe(string_keyframe); }
}
#else
    HOM_StringKeyframe();
    HOM_StringKeyframe(const std::string &value,
	HOM_EnumValue &language = HOM_exprLanguage::Python);
    HOM_StringKeyframe(const std::string &value, double time,
	HOM_EnumValue &language = HOM_exprLanguage::Python);
    HOM_StringKeyframe(const HOM_StringKeyframe &string_keyframe);
#endif
    virtual ~HOM_StringKeyframe();

    virtual std::string __repr__();

    virtual HOM_EnumValue &evaluatedType();
};

#endif
