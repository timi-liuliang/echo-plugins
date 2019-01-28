/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Color_h__
#define __HOM_Color_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_PtrOrNull.h"
#include "HOM_Module.h"
#include <UT/UT_Color.h>
#include <vector>
#include <stdexcept>

SWIGOUT(%rename(Color) HOM_Color;)

class HOM_API HOM_Color
{
public:
    HOM_Color()
    : myColor(UT_RGB, 0.0f, 0.0f, 0.0f)
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_Color(const std::vector<float> &rgb_tuple);

    HOM_Color(float r, float g, float b)
    : myColor(UT_RGB, r, g, b)
    { HOM_CONSTRUCT_OBJECT(this) }

    SWIGOUT(%ignore HOM_Color(const UT_Color&);)
    HOM_Color(const UT_Color &color)
    : myColor(color)
    { HOM_CONSTRUCT_OBJECT(this) }

    ~HOM_Color()
    { HOM_DESTRUCT_OBJECT(this) }

    bool operator==(HOM_PtrOrNull<HOM_Color> color)
    { return color.myPointer && myColor == color.myPointer->myColor; }

    bool operator!=(HOM_PtrOrNull<HOM_Color> color)
    { return !operator==(color); }

    int __hash__();
    std::string __repr__();

    SWIGOUT(%ignore operator=;)
    HOM_Color &operator=(const HOM_Color& color)
    {
	myColor = color.myColor;
	return *this;
    }

    std::vector<float> rgb();
    std::vector<float> hsv();
    std::vector<float> hsl();
    std::vector<float> xyz();
    std::vector<float> lab();
    std::vector<float> tmi();

    void setRGB(const std::vector<float> &tuple);
    void setHSV(const std::vector<float> &tuple);
    void setHSL(const std::vector<float> &tuple);
    void setXYZ(const std::vector<float> &tuple);
    void setLAB(const std::vector<float> &tuple);
    void setTMI(const std::vector<float> &tuple);

    static std::vector<std::string>	ocio_spaces();

    SWIGOUT(%ignore colorTypeForEnumValue;)
    static UT_ColorType	    colorTypeForEnumValue(HOM_EnumValue &enum_value);
    SWIGOUT(%ignore colorTypeToEnumValue;)
    static HOM_EnumValue &  colorTypeToEnumValue(UT_ColorType basis);

    void *_asVoidPointer()
    { return &myColor; }


    SWIGOUT(%ignore myColor;)
    UT_Color myColor;
};

#endif
