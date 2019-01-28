/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        DM_MouseHook.h ( DM Library, C++)
 *
 * COMMENTS:
 *	Base classes to hook into the viewport mouse event handling pipeline.
 */
#ifndef __DM_MouseHook_H__
#define __DM_MouseHook_H__

#include "DM_API.h"
#include "DM_ViewportType.h"

#include <UT/UT_String.h>

class DM_VPortAgent;
class UI_Event;

/// Data structure for rendering scene hooks.
class DM_API DM_MouseHookData
{
public:
    // Currently this is an empty class.
};



/// Mouse event hook instance, which does the actual handling for a viewport.
///
/// Allocated by the DM_MouseHook factory object.
class DM_API DM_MouseEventHook
{
public:
    /// A mouse event hook has a parent viewport and a view type mask which
    /// determines which viewport types it will process mouse events within
    /// (eg. perspective, ortho, UV, all).
	     DM_MouseEventHook(DM_VPortAgent &vport, DM_ViewportType view_mask);
    virtual ~DM_MouseEventHook();

    /// @brief Method called to handle a mouse motion or button event.
    ///
    /// This method is called to handle a regular mouse event.  It should
    /// return true when it handles the event to prevent any further code
    /// from processing it.
    virtual bool	 handleMouseEvent(const DM_MouseHookData &hook_data,
					  UI_Event *event) = 0;

    /// @brief Method called to handle a mouse wheel scroll event.
    ///
    /// This method is called to handle a mouse wheel scroll event.  It should
    /// return true when it handles the event to prevent any further code from
    /// processing it.
    virtual bool	 handleMouseWheelEvent(
					    const DM_MouseHookData &hook_data,
					    UI_Event *event) = 0;

    /// @brief Method called to handle a mouse button double click event.
    ///
    /// This method is called to handle a mouse double click event.  It should
    /// return true when it handles the event to prevent any further code from
    /// processing it.
    virtual bool	 handleDoubleClickEvent(
					    const DM_MouseHookData &hook_data,
					    UI_Event *event) = 0;

    /// @brief Method called to determine whether to allow a RMB menu popup.
    ///
    /// This method is called to determine if Houdini is allowed to open a RMB
    /// menu on a particular mouse event.  It is called in descending order of
    /// priority with the RMB menu event allowed only if all hooks allow it.
    virtual bool	 allowRMBMenu(const DM_MouseHookData &, UI_Event *)
			    { return true; }


    /// Viewport this scene render is attached to.
    DM_VPortAgent	&viewport() const { return myAgent; }
    DM_ViewportType	 getViewportMask() const { return myViewportMask; }

private:
    DM_VPortAgent	&myAgent;
    DM_ViewportType	 myViewportMask;
};



/// A DM_MouseHook creates new DM_MouseEventHook objects when new viewports are
/// created.
class DM_API DM_MouseHook
{
public:
    /// Create a mouse hook which creates mouse event hook instances for
    /// specific viewports. Only one mouse hook is ever created, and it is
    /// responsible for managing the mouse event hooks for viewports.
    /// Each hook requires a name (for error reporting) and a priority level
    /// to resolve multiple mouse hook conflicts.
	     DM_MouseHook(const char *hook_name,
			  int priority); 
    virtual ~DM_MouseHook();

    const char		       *getName() const     { return myHookName; }
    int				getPriority() const { return myPriority; }

    /// @brief Called when a viewport needs to create a new hook.
    ///
    /// Each viewport has its own event hook.
    virtual DM_MouseEventHook	*newEventHook(DM_VPortAgent &vport) = 0;

    /// @brief Called when a viewport no longer requires the hook.
    ///
    /// When a viewport is destroyed, it retires all its hooks. Because a hook
    /// could be shared between all viewports, this method gives the mouse hook
    /// the opportunity to delete it, dereference it, etc. The viewport doing
    /// the retiring is passed in along with the hook it is retiring.
    virtual void		 retireEventHook(DM_VPortAgent &vport,
						 DM_MouseEventHook *hook) = 0;

private:
    UT_String		myHookName;
    int			myPriority;
};
#endif
