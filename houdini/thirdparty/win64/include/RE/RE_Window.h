
#ifndef __RE_Window_h__
#define __RE_Window_h__

#include "RE_API.h"
#include "RE_IDType.h"
#include "RE_Types.h"

#include <UT/UT_Defines.h>
#include <UT/UT_Rect.h>
#include <UT/UT_Lock.h>
#include <UT/UT_Pair.h>
#include <UT/UT_ValArray.h>

class RE_Visual;
class RE_Server;
class RE_Render;

class QWidget;


struct RE_WindowFlags
{
    bool isOriginReadjusted:1,
	 alwaysOnTop:1,
	 maximized:1,
	 fullscreen:1;
};

class RE_API RE_Window
{
public:
    RE_Window(RE_Server *server,
	      RE_WindowType type = RE_WINDOW_NORMAL,
	      RE_VisualType visType = RE_NORMAL_VIS);

    virtual ~RE_Window();

    void setName(const char *name);
    void setIconName(const char *name);
    
    void setParentWindow(RE_Window *parent);
    const RE_Window	*getParentWindow() const { return parentWindow; }

    void setDialogParentHint(RE_IDType parent_hint);

    RE_Server		*getServer() { return myServer; }
    RE_Visual		*getVisual()
			 { return myVisualResource; }
    RE_Render		*getRender();
    void		 setRender(RE_Render *r) { myRender = r; }
    
    // Opens a window but does not show it immediately.
    // Call showWindow to show the window.
    RE_IDType	 openWindow();
    void	 showWindow();
    void	 closeWindow();

    void	 initMaxSize(int w, int h );
    void	 initMinSize(int w, int h );
    void	 initVisual(   RE_Visual *visual );
    void		 setLimitPositions(bool limitpos)
			 { myLimitPositions = limitpos; }
    bool		 getLimitPositions() const
			 { return myLimitPositions; }

    void moveWindow(int x, int y);
    void sizeWindow(int w, int h);

    // Set the window's opacity (0.0 - 1.0).
    // Values less than 1.0 will make the window translucent.
    void setWindowOpacity(fpreal opacity);


    // Maximize the window and/or mark it as maximized so that when openWindow
    // is called, it can open the window maximized.
    void maximizeWindow();
    void restoreWindow();

    bool isMaximized() const { return winFlags.maximized; }

    // Set the window as fullscreen. Depending on the windowing system, this
    // either informs the window manager to set us as fullscreen or simply
    // sets borders up so that higher-level classes can do the fullscreening
    // themselves.
    void fullscreenWindow(bool full);
    bool isFullscreenWindow() const { return winFlags.fullscreen; }
    void lowerWindow();
    void raiseWindow();

    // Show/Hide the Window Frame/ Titlebar
    void setHasWindowFrame(bool b);
    bool hasWindowFrame() const;
    
    int		 getBorderWidth() const
		 { return myLeftBorder+myRightBorder; }
    int		 getBorderHeight() const
		 { return myTopBorder+myBottomBorder; }
    int		 getBorderTopSize() const
		 { return myTopBorder; }
    int		 getBorderLeftSize() const
		 { return myLeftBorder; }
    int		 getBorderBottomSize() const
		 { return myBottomBorder; }
    int		 getBorderRightSize() const
		 { return myRightBorder; }

    UT_DimRect  findLocation() const;

    // Sets the window to sit on top of all other windows, or turns off that
    // state, depending on whether newstate is true or false.
    void	 setWindowAlwaysOnTop(bool newstate);
    int		getWindowAlwaysOnTop() const { return winFlags.alwaysOnTop; }

    void	 setWindowFloatOnTop(bool on_top);

    RE_DisplayMode	 getMode() const;

    RE_VisualType	 getVisualType() const
			 { return myVisualType; }
    void		 setVisualType(RE_VisualType t) { myVisualType = t; }

    // changes to the type will not occur until the window is opened again.
    void		 setWindowType(RE_WindowType t)
			 { myWindowType = t; }

    RE_WindowType	 getWindowType( bool extra=false) const;

    
    void		 setWindowID(RE_IDType id) { myWindowID = id; }
    RE_IDType		 getWindowID()  const
			 { return myWindowID; }
    int			 matchID( RE_IDType wid ) const
			 { return wid == myWindowID; }
    
    virtual RE_IDType	 getBufferID() const { return 0; }

    OGLDrawable	 getGraphicsDrawable() const { return myDrawable; }
    void	 setGraphicsDrawable(OGLDrawable d) { myDrawable = d; }

    
    void	 setIMESupport(bool enable);

    /// Return the Qt main window.
    static QWidget *    mainQtWindow();

    /// Create a Qt window widget for the given window type.
    /// The caller takes ownership of the returned object.
    static QWidget *	createQtWindow(RE_WindowType win_type);

    /// Returns true if Houdini or any of its sub windows are focused
    static bool		appActive();

    void	 setWindowBorders(int left, int right, int top, int bottom)
		 {
		     myLeftBorder = left;
		     myRightBorder = right;
		     myTopBorder = top;
		     myBottomBorder = bottom;
		 }

    // This is called by the native window to inform that it has changed its
    // maximized state.
    void	updateMaximizedFlag(bool is_maximized)
		{
		    winFlags.maximized = is_maximized;
		}

    static bool		isTabFocusEnabled();
    static void		enableTabFocus(bool enable);

    /// Return the string tag used to identify whether a Qt widget is the
    /// window for an RE_Window object.
    static const char * windowTag() { return "RE_Window"; }

    bool	isQuadBufferStereo() const;

    /// Return the screen number containing this window.
    /// If the underlying platform window has not been created
    /// yet then return the primary screen number.
    int                          screen() const;

    /// Get screen-to-window scale.
    fpreal			 screenToDeviceScale() const;

    /// Get Default Window Border Sizes
    static const int* getDefaultWindowBorderSizes();
    static int getDefaultWindowTitleBarHeight();

private:
    /// Initialize window borders.
    void			 initBorderSizes_();

    /// Move the underlying Qt window to the position specified by myFrame.
    void			 moveQtWindow_();

#if defined(WIN32)
    /// Add the Always On Top menu item to the window's system menu.
    void			 addAlwaysOnTopSystemMenuItem_();
#endif
    
private:
    RE_Server		*myServer;
    RE_Render		*myRender;
    RE_Visual		*myVisualResource;
    RE_VisualType	 myVisualType;
    RE_IDType		 myWindowID;
    
    UT_String		 myName;
    UT_String		 myIconName;

    // Window geometry in native screen coordinates and scaling.
    UT_DimRect		 myFrame;

    // Window sizes in native screen scaling.
    UT_IntPair		 myMinSize;
    UT_IntPair		 myMaxSize;

    OGLDrawable		 myDrawable;
    
    
    unsigned long	 myIconPixmap;
    RE_WindowFlags	 winFlags;
    RE_Window		*parentWindow;
    RE_IDType		 myDialogParentHint;

    RE_WindowType	 myWindowType;

    int			 myLeftBorder;
    int			 myTopBorder;
    int			 myRightBorder;
    int			 myBottomBorder;

    unsigned int	 myLimitPositions:1;

    fpreal		 myWindowOpacity;

#if defined(WIN32)
    bool		 myAddedAlwaysOnTopMenuItem;
#endif
};

class RE_API RE_WindowList
{
public:
			 RE_WindowList()
			 { }
			~RE_WindowList()
			 { }

    void		 lock()
			 { myLock.lock(); }
    void		 unlock()
			 { myLock.unlock(); }
    void		 addWindow(RE_Window *w)
			 { lock(); myWindows.append(w); unlock(); }
    void		 removeWindow(RE_Window *w)
			 { lock(); myWindows.findAndRemove(w); unlock(); }
    int			 getNumWindows() const
			 { return myWindows.entries(); }
    RE_Window		*getWindow(int index) const
			 { return myWindows(index); }
    void		 removeWindow(RE_IDType wid);
    RE_Window		*isWindowHere(RE_IDType wid);

private:
    UT_ValArray<RE_Window *>	 myWindows;
    UT_Lock			 myLock;
};

#endif
