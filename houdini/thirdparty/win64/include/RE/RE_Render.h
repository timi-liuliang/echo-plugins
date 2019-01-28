/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_Render.h ( RE Library, C++)
 */

#ifndef __RE_Render__
#define __RE_Render__

/*****************************************************************************

DESCRIPTION:

RE_Render is a switcher for platform-dependent renderers. Code using
RE_Render should therefore be platform-independent.

*****************************************************************************/

#include "RE_API.h"
#include <SYS/SYS_Floor.h>
#include <SYS/SYS_Math.h>
#include <UT/UT_Unicode.h>
#include "RE_OGLRender.h"

class QWidget;

class RE_API RE_Render : public RE_OGLRender
{
public:
		    RE_Render(int do_foreground, const char *appname = 0);
    virtual	   ~RE_Render();
    
    static void	    setMainRender(RE_Render * main);
    static RE_Render * getMainRender();
    static bool	    isMainRenderInitialized();

    // The list of OpenGL render contexts that are currently available
    static int	    getNumRenderContexts();
    static RE_Render *getRenderContext(int index);

    static RE_Render *findRenderWithID(int id);    
    static RE_Render *findRenderWithContext(RE_OGLContext context);


    // Control whether or not to allow adaptive subdivision in the viewport.
    static bool	     getAllowAdaptiveSubd();
    static void	     setAllowAdaptiveSubd(bool allow);

    // High DPI mode upscales some elements.
    static void	     setHighDPIMode(bool hi);
    static bool	     isHighDPIMode();

    // Creates a new OpenGL context using the same drawable as this context.
    RE_Render	    *createNewRenderContext();
    
    // Creates a new OpenGL context using a new offscreen window. Delete the
    // created window to free the render when finished.
    static RE_Render *createNewOffscreenContext(RE_Window *&win);

private:
    static RE_Render * theMainRender;
};


RE_API extern RE_Render	*REgetRender();
inline RE_Render *REgetMainRender() { return RE_Render::getMainRender(); }
inline bool REisMainRenderInitialized() { return RE_Render::isMainRenderInitialized(); }

// Inches are an arbitrary unit that get translated into pixels based on the
// current DPI setting of the server (which in turn is modified by the
// HOUDINI_UISCALE env variable). Inches scale with the DPI.

inline int REtoPixels(fpreal inches)
{
    return RE_OGLRender::inchesToPixels(inches);
}

inline fpreal REtoInches(int pixels)
{
    return RE_OGLRender::pixelsToInches(pixels);
}

// When using pixels, absolute pixel values do not scale with the DPI, they
// always assume a DPI of 85. These methds will scale a pixel length using the
// current DPI to 85dpi ratio.

inline int REtoScaledPixels(int pixels)
{
    if(pixels != 0)
    {
	return SYSmax(1, (int) SYSrint(pixels *
				       (RE_OGLRender::dpi()/RE_DEFAULT_DPI)));
    }
    return 0;
}
inline int REfromScaledPixels(int pixels)
{
    return (int) SYSrint(pixels * (RE_DEFAULT_DPI/RE_OGLRender::dpi()));
}

RE_API extern fpreal REgetScreenToDeviceScale(int screen=-1);

#endif // __RE_Render__
