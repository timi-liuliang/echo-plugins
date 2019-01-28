/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_RasterObject.h (RE Library, C++)
 *
 * COMMENTS:
 *
 *	Wraps RE_OGLFramebuffer rendering into a nice interface.
 *
 */

#ifndef __RE_RasterObject__
#define __RE_RasterObject__

#include "RE_API.h"

#include "RE_Texture.h"
#include "RE_OGLFramebuffer.h"
#include "RE_ContextSpecificTable.h"

#include <SYS/SYS_Types.h>

#include <functional>


class TIL_Raster;
class RE_OGLFramebuffer;
class RE_Render;


using RE_RenderCallback
            = std::function<void (RE_Render *, int, int, int, int, void *)>;
		              // (     render,   x,   y,   w,   h,   data)

class RE_API RE_RasterObject
{
public:
    /// Provide this class with a callback, the width and height you want to
    /// render into, the height and width you want the texture to be (this
    /// should be a power of two), and some data to be passed to the callback.
    RE_RasterObject(RE_RenderCallback callback, int w, int h, int tw, int th,
		    void *data);

    /// Be very careful before calling this - do you have an OpenGL context
    /// pushed?
    ~RE_RasterObject();

    /// Frees the offscreen buffer associated with this object without
    /// clearing the other data (texture, raster).
    void            resetBuffer();

    // build() does not create the texture because a valid OGL Context may
    // not be present yet. It can safely render into a RasterWindow, as that
    // pushes a valid context. We can't render to a RasterWindow in the middle
    // of a redraw as that messes up stacks & contexts. So, the texture
    // creation is split into 2 parts - build() (called during init) and
    // getTexture() (called during redraw).

    /// Render offscreen, calling the render callback.
    bool	build(RE_Render *r);

    /// Generate and get the OpenGL ID of a texture that contains the contents
    /// of what was render when build() was called.
    RE_Texture *getTexture(RE_Render *r);

    /// If the color scheme changes, we need to re-render everything.
    void	rebuild(RE_Render *r);

    /// Whether build() has successfully been called.
    bool	isBuilt() const;

    /// The texture's width. Should be a power of two.
    int getTexWidth() const;

    /// The texture's height. Should be a power of two.
    int getTexHeight() const;

    /// The width of what you actually want to render.
    int getWidth() const;

    /// The height of what you actually want to render.
    int getHeight() const;
    
    /// Force PXL_Raster creation
    void	setForceRaster(bool on) { myForceRaster = on; }

    TIL_Raster*	getRaster() { return myRaster; }

    /// Renders this object directly using the callback.
    void	renderDirect(RE_Render *r, int x, int y, int w, int h);

private: // methods
    void	render(RE_Render *r);
    bool	buildFramebuffer(RE_Render *r);

private:

    RE_RenderCallback	 myCallback;
    void		*myVoidData;
    int			 myTexWidth;
    int			 myTexHeight;
    int			 myWidth;
    int			 myHeight;
    bool		 myBuiltFlag;
    RE_Texture		*myTexture;
    TIL_Raster		*myRaster;
    bool		 myForceRaster;
    RE_ContextSpecificTable<RE_OGLFramebuffer> myFramebuffer;
    
    static bool		   theRenderToBuffer;
};

#endif

