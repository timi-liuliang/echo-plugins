/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_qt_h__
#define __HOM_qt_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Module.h"

class HOM_FloatingPanel;

SWIGOUT(%rename(qt) HOM_qt;)
class HOM_API HOM_qt
{
public:
    virtual ~HOM_qt()
    {}

    virtual std::string __repr__() = 0;

    virtual void* _mainWindow() = 0;
    virtual void* _floatingPanelWindow(HOM_FloatingPanel *panel) = 0;

    virtual void* _createWindow() = 0;
    virtual void* _createDialog() = 0;
    virtual void* _createMenu() = 0;
    virtual void* _createIcon(const char* icon_name, int width, int height) = 0;

    virtual void* _getColor(const char* color_name) = 0;

    virtual void* _getCursor(const char *cursor_name) = 0;

    virtual std::string styleSheet() = 0;

    virtual std::map<std::string, int> qtKeyToUIKey(int qtKey,
						    int qtKeymodifiers) = 0;
    virtual std::string qtKeyToString(int qtKey,
				      int qtKeymodifiers,
				      std::string qtkeystring) = 0;

    virtual double inchesToPixels(double inches) = 0;
    virtual double pixelsToInches(double pixels) = 0;

    virtual void skipClosingMenusForCurrentButtonPress() = 0;
};

#endif
