/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef HOM_UIEventDevice_h
#define HOM_UIEventDevice_h

#include "HOM_API.h"
#include "HOM_Module.h"

SWIGOUT(%rename(UIEventDevice) HOM_UIEventDevice;)
SWIGOUT(%feature("notabstract") HOM_UIEventDevice;)

class HOM_API HOM_UIEventDevice
{
public:
#ifdef SWIG
%extend
{
    HOM_UIEventDevice(int64 val, int64 val2)
    { 
	return HOM().newUIEventDevice(val, val2); 
    }
}
#else
    HOM_UIEventDevice()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif
    virtual ~HOM_UIEventDevice()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    // keyboard
    virtual bool isAltKey() = 0;
    virtual bool isAutoRepeat() = 0;
    virtual bool isCapsLock() = 0;
    virtual bool isCtrlKey() = 0;
    virtual bool isKeyPad() = 0;
    virtual bool isShiftKey() = 0;
    virtual bool isArrowDown() = 0;
    virtual bool isArrowLeft() = 0;
    virtual bool isArrowRight() = 0;
    virtual bool isArrowUp() = 0;

    // mouse
    virtual bool isLeftButton() = 0;
    virtual bool isMiddleButton() = 0;
    virtual bool isRightButton() = 0;
    virtual double mouseWheel() = 0;
    virtual double mouseX() = 0;
    virtual double mouseY() = 0;

    // tablet
    virtual bool isTablet() = 0;
    virtual double tabletAngle() = 0;
    virtual double tabletPressure() = 0;
    virtual double tabletRoll() = 0;
    virtual double tabletTilt() = 0;
    virtual double time() = 0;  
};

#endif
