/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        DM_RenderTable.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Table for all GL rendering hooks. H11 rendering hooks are handled in
 *	GR_RenderTable, which is the base class of DM_RenderTable.
 */
#ifndef DM_RenderTable_h
#define DM_RenderTable_h

#include "DM_API.h"
#include "DM_SceneHook.h"

#include <GUI/GUI_PrimitiveHook.h>
#include <GA/GA_PrimitiveTypeId.h>
#include <GT/GT_PrimitiveTypes.h>

class GR_UserOption;

class DM_API DM_RenderTable
{
public:
    static DM_RenderTable *getTable();
    
    /// @brief Register a geometry primitive render hook
    /// Register a primitive hook that uses a GT_Primitive as its source.
    ///  type      Specifies the type of GT primitive this hook is intended for
    ///  priority  If more than one hook is present for the same GT type, the
    ///            higher of the two priorities is processed first. MAX_INT is
    ///            reserved, otherwise any integer is valid.
    ///  flags     Special options for the hook:
    ///            GUI_HOOK_FLAG_AUGMENT_PRIM: Add additional rendering to the
    ///		   Houdini primitive, which still draws.
    ///		   GUI_HOOK_FLAG_PRIM_FILTER: Alter or create a new GT_Primitive
    ///            from the GT primitive normally used to render. Does not
    ///		   create a GR_Primitive.
    ///  hook_ver  Leave at its default. This will notify you if a hook needs to
    ///            be recompiled.
    /// returns true if the hook was sucessfully registered.
    bool	registerGTHook(GUI_PrimitiveHook     *hook,
			       GT_PrimitiveType       type,
			       int		      priority,
			       GUI_PrimitiveHookFlags flags=GUI_HOOK_FLAG_NONE,
			       int hook_ver       = GUI_PRIMITIVE_HOOK_VERSION);

    /// @brief Register a geometry primitive render hook
    /// Register a primitive hook that uses a GEO_Primitive as its source.
    ///  geo_type  Specifies the geometry type this hook is intended for.
    ///  priority  If more than one hook is present for the same GT type, the
    ///            higher of the two priorities is processed first. MAX_INT is
    ///            reserved, otherwise any integer is valid.
    ///  flags     Special options for the hook:
    ///            GUI_HOOK_FLAG_AUGMENT_PRIM: Add additional rendering to the
    ///		   Houdini primitive, which still draws.
    ///		   GUI_HOOK_FLAG_PRIM_FILTER: Alter or create a new GT_Primitive
    ///            from the GT primitive normally used to render. Does not
    ///		   create a GR_Primitive.
    ///  hook_ver  Leave at its default. This will notify you if a hook needs to
    ///            be recompiled.
    /// returns true if the hook was sucessfully registered.
    bool	registerGEOHook(GUI_PrimitiveHook     *hook,
				GA_PrimitiveTypeId     geo_type,
				int		       priority,
				GUI_PrimitiveHookFlags flags=GUI_HOOK_FLAG_NONE,
				int hook_ver = GUI_PRIMITIVE_HOOK_VERSION);


    /// @brief Register a scene render hook
    /// Register a scene hook that is associated with a specific render pass.
    ///  type            The rendering task this hook creates render hooks for.
    ///  policy          How the hook interacts with native Houdini rendering
    ///  hook_ver        Leave at its default. This will notify you if a hook
    ///			 needs to be recompiled.
    /// returns true if the hook was successfully rgistered.
    bool	registerSceneHook(DM_SceneHook	    *hook,
				  DM_SceneHookType   type,
				  DM_SceneHookPolicy policy,
				  int hook_ver = DM_SCENE_HOOK_VERSION);

    /// @brief Install a new scene file visualizer for all geometry contexts
    /// Installs a generic visualizer that can be enabled individually for
    /// the various contexts, such as Scene Object and Current Geometry.
    /// The @c vis_name is the name used by commands to reference the option,
    /// while the @c vis_label is the English readable name. If not given,
    /// the @c vis_name will be used as the readable name as well.
    /// If the visualizer already exists, it will not create it and false is
    /// returned. If successful, true is returned.

    bool	installGeometryOption(const char *vis_name,
        	                      const char *vis_label = NULL,
        	                      GUI_GeometryOptionFlags flags =
        	                	  GUI_GEO_OPT_NONE);

    /// @brief Install a new scene file visualizer for controlling scene hooks
    /// Install a new scene file generic visualizer untied to any geometry
    /// context. This can be used to control scene hooks more easily.
    /// The @c vis_name is the name used by commands to reference the visualizer,
    /// while the @c vis_label is the English readable name. If not given,
    /// the @c vis_name will be used as the readable name as well.
    /// If the visualizer already exists, it will not create it and return
    /// false. Otherwise true is returned.
    bool	installSceneOption(const char *vis_name,
        	                   const char *vis_label = NULL);
    
private:
		 DM_RenderTable();
    virtual	~DM_RenderTable();

    void	installHooks();
    
    bool 	installOption(const char *option_name,
         	              const char *option_label,
         	              GUI_GeometryOptionFlags flags = GUI_GEO_OPT_NONE);
    
    bool	registerHook(GUI_PrimitiveHook       *hook,
			     GT_PrimitiveType	      type,
			     int		      geo_type,
			     int		      priority,
			     GUI_PrimitiveHookFlags   flags,
			     int		      render_hook_version);

    static void clearRenderHookTable(void *);
    static void clearSceneRenderHooks(void *);

    bool	 myInitialized;
    
    friend class DM_Viewport;
};

extern "C" {
    /// The plugin entry point for render hook registration 
    SYS_VISIBILITY_EXPORT extern void newRenderHook(DM_RenderTable *table);
};



#endif
