/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_Server.h (C++)
 *
 * COMMENTS:
 * 	Window server.
 *
 */
#ifndef __RE_Server__
#define __RE_Server__

#include <UT/UT_Array.h>
#include <UT/UT_Rect.h>

#include "RE_API.h"
#include "RE_IDType.h"
#include "RE_Types.h"

class RE_Window;
class RE_Render;

class RE_API RE_Server
{
public:
    RE_Server();
    ~RE_Server();

    bool initServer();

    void flush() const;

    RE_IDType getFocusWindow() const { return myFocusWindow; }
    void setFocusWindow(RE_IDType wid);

    static int getMinXRes() { return myMinXRes; }
    static int getMinYRes() { return myMinYRes; }
    bool meetsMinRes( ) const;

    /// Determine if the given window (and not a child window) is currently
    /// under the mouse.
    /// The current x,y mouse coordinates are optional under
    /// certain platforms
    bool windowUnderCursor(RE_IDType wid, int x, int y) const;

    /// Return the id of the Houdini window that appears immediately
    /// underneath the mouse cursor.  Return 0/nullptr if there is no Houdini
    /// window immediately underneath the mouse cursor. 
    static RE_IDType windowUnderCursor();

    bool getColorUnderCursor(float color[3]) const;

    bool  GLMakeCurrent( OGLDrawable draw, RE_OGLContext context,
				 bool ignore_errors);
    RE_OGLContext GLCurrentContext() const;
    int   GLMakeCopyCurrent( OGLDrawable    draw,
				     RE_OGLContext  sourceContext,
				     RE_OGLContext  targetContext );

    bool  GLSwapBuffers(RE_Window *currentWindow);
    OGLDrawable GLGetCurrentDrawable();
    void  GLWaitGL();

    // provide some reasonable defaults
    static int		 getDragTolX()   { return 4; }
    static int		 getDragTolY()   { return 4; }
    static fpreal64	 getDragDelay()  { return 0.75; }

    /// @{
    /// Desktop support.
    static int primaryScreenNum();
    static int numScreens();
    static int screenContaining(int x, int y, bool native=false);
    static const UT_DimRect &screenArea(int screen_num);
    static const UT_DimRect &nativeScreenArea(int screen_num);
    static const UT_DimRect &workArea(int screen_num);
    static const UT_DimRect &nativeWorkArea(int screen_num);
    static const UT_DimRect &displayScreenArea(int screen_num);
    static const UT_DimRect &displayWorkArea(int screen_num);
    static fpreal screenToDeviceScale(int screen_num);
    static const UT_DimRect &virtualScreenArea();
    static const UT_DimRect &virtualWorkArea();
    /// @}

    /// @{
    /// Convert from native coordinates to scaled Houdini UI coordinates
    /// and vice versa.
    /// @}
    static void	toScreenCoords(int &x, int &y);
    static void	fromScreenCoords(int &x, int &y);
    
    /// @{
    /// Convert from a value that is scaled in Houdini UI space
    /// to a value that is scaled in screen space and vice versa.
    /// }@
    /// 
    static void toScreenScale(int screen, int &value);
    static void fromScreenScale(int screen, int &value);

    /// Add a window to the list of current main windows. If it is already in
    /// the list, it will not be added again.
    void	 addCurrentMainWindow(RE_Window *win);

    /// Remove a window from the list of current main windows.
    void	 removeCurrentMainWindow(RE_Window *win);

    /// Find the current main window that's focused, so you can attach
    /// dialog windows to it. If none is marked as focused, just returns
    /// the first window.
    RE_Window	*getCurrentMainWindow() const;

    /// Find the first current main window, regardless of whether it's focused.
    /// Used on Windows to ensure that all subwindows are made tool windows of
    /// at least the main Houdini window.
    RE_Window	*getFirstCurrentMainWindow() const;

    /// Creates a new RE_Window, ready to be opened.
    RE_Window	*newWindow(RE_DisplayMode mode,
			   RE_WindowType type,
			   RE_VisualType visType=RE_NORMAL_VIS);

    /// Returns current render context matching the mode passed in. All render
    /// contexts are tied to a specific drawable. This may return NULL if
    /// there are no render contexts currently created that match mode.
    RE_Render	*getContextMatching(RE_DisplayMode mode) const;

    /// Return whether the Qt objects are valid or not.
    /// Qt objects are not valid when the Qt main event loop has exited.
    static bool		isAppExiting();

    /// This method should only be called when the Qt main event loop has
    /// exited.  It lets RE know that it is no longer safe to work with any Qt
    /// objects.
    static void		setAppExiting();

private:
    RE_OGLContext myCurrentContext;

    RE_IDType myFocusWindow;

    /// The current main windows are all the toplevel application windows.
    /// We keep track of them to find out what window we should use when
    /// creating dialogs - windowing systems can decorate and treat dialog
    /// windows differently if they know which window is the parent.
    UT_Array<RE_Window *> myCurrentMainWindows;

    static int myMinXRes;
    static int myMinYRes;
};


#endif
