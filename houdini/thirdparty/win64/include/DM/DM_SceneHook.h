/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        DM_SceneHook.h ( DM Library, C++)
 *
 * COMMENTS:
 *	Base class to hook into various parts of the scene rendering pipeline.
 */
#ifndef DM_SCENE_HOOK_H
#define DM_SCENE_HOOK_H

#include "DM_API.h"
#include "DM_ViewportType.h"

#include <UT/UT_String.h>
#include <GR/GR_Defines.h>

class DM_VPortAgent;
class GUI_DisplayOption;
class RE_Light;
class RE_Render;

#define DM_SCENE_HOOK_VERSION 2

/// The rendering task that the hook performs.
enum DM_SceneHookType
{
    DM_HOOK_BACKGROUND,
    DM_HOOK_PRE_RENDER,
    DM_HOOK_BEAUTY,
    DM_HOOK_BEAUTY_TRANSPARENT,
    DM_HOOK_UNLIT,
    DM_HOOK_XRAY,
    DM_HOOK_POST_RENDER,
    DM_HOOK_FOREGROUND,

    // Replace-only hooks
    DM_HOOK_SHADOW,
    DM_HOOK_HQ_LIGHT_PASS,
    
    DM_HOOK_FULL_SCENE
};

/// How the hook augments or replaces the native Houdini rendering task.
enum DM_SceneHookPolicy
{
    DM_HOOK_REPLACE_NATIVE = 0,
    DM_HOOK_BEFORE_NATIVE,
    DM_HOOK_AFTER_NATIVE
};


/// Data structure for rendering scene hooks.
class DM_API DM_SceneHookData
{
public:
    // class containing all viewport display options
    const GUI_DisplayOption *disp_options;

    // Size of the viewport in pixels
    int			     view_width;
    int			     view_height;

    // only non-NULL in DM_HOOK_SHADOW and DM_HQ_LIGHT_PASS
    RE_Light		    *light;

    // In case this is registered more than once to different spots, this
    // allows the hook to differentiate the callers.
    DM_SceneHookType	     hook_type;
    DM_SceneHookPolicy	     hook_policy;
};



/// Scene render hook instance, which does the actual rendering for a viewport
class DM_API DM_SceneRenderHook
{
public:
    /// A Scene render hook has a parent viewport and a view type mask which
    /// determines which viewport types it will display in (eg. perspective,
    /// ortho, UV, all)
	     DM_SceneRenderHook(DM_VPortAgent &vport,
				DM_ViewportType view_mask);
    virtual ~DM_SceneRenderHook();

    /// @brief Restrict hook to only certain Viewport Renderer versions
    /// This method allows you to restrict this hook to only operate in some of
    /// the viewport renderers (H11 - GL3). If the viewport is set to a
    /// a renderer and this method returns false, it will be skipped. The
    /// default action is to support all viewport renderers.
    virtual bool	 supportsRenderer(GR_RenderVersion version)
			    { return true; }
    
    /// @brief Render method called when the rendering task is required.
    /// render() performs the rendering task for the hook, or returns false if
    /// if it cannot. The RE_Render contains the active GL context and the
    /// hook_data contains viewport-specific and hook-specific data.
    virtual bool	 render(RE_Render *r,
				const DM_SceneHookData &hook_data) = 0;

    /// @brief Callback method when the viewport is no longer visible.
    /// Called when the viewport is temporarily closed (quad/single switch, pane
    /// switch). This gives the hook the opportunity to clean up expensive GL
    /// objects or caches.
    virtual void	 viewportClosed();
   
    /// Viewport this scene render is attached to.
    DM_VPortAgent	&viewport() const { return myAgent; }
    DM_ViewportType	 getViewportMask() const { return myViewportMask; }

private:
    DM_VPortAgent	&myAgent;
    DM_ViewportType	 myViewportMask;
};



/// A Scene Hook creates new scene render hooks when new viewports are created.
class DM_API DM_SceneHook
{
public:
    /// Create a scene hook which creates scene render hook instances for
    /// specific viewports. Only one scene hook is ever created, and it is
    /// responsible for managing the scene render hooks for viewports.
    /// Each hook requires a name (for error reporting) and a priority level
    /// to resolve multiple scene hook conflicts.
	     DM_SceneHook(const char *hook_name,
			  int priority); 
    virtual ~DM_SceneHook();

    const char		       *getName() const     { return myHookName; }
    int				getPriority() const { return myPriority; }

    /// @brief Called when a viewport needs to create a new hook.
    /// Each viewport has its own scene hook.
    virtual DM_SceneRenderHook *newSceneRender(DM_VPortAgent &vport,
					       DM_SceneHookType type,
					       DM_SceneHookPolicy policy) = 0;

    /// @brief Called when a viewport no longer requires the hook.
    /// When a viewport is destroyed, it retires all its hooks. Because a hook
    /// could be shared between all viewports, this method gives the scene hook
    /// the opportunity to delete it, dereference it, etc. The viewport doing
    /// the retiring is passed in along with the hook it is retiring.
    virtual void		retireSceneRender(DM_VPortAgent &vport,
						  DM_SceneRenderHook *hook) = 0;

private:
    UT_String		myHookName;
    int			myPriority;
};
#endif
