/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GUI_PrimitiveHook.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Base class for generating custom GR primitives.
 */
#ifndef GUI_PRIMITIVE_HOOK_H
#define GUI_PRIMITIVE_HOOK_H

#include "GUI_API.h"
#include <GR/GR_Defines.h>
#include <GR/GR_RenderInfo.h>
#include <GT/GT_PrimitiveTypes.h>
#include <GT/GT_Primitive.h>
#include <GA/GA_PrimitiveTypes.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>

class GR_Primitive;
class GEO_Primitive;

#define GUI_PRIMITIVE_HOOK_VERSION 3

/// Special modifiers to primitive hooks which provide different behaviours.
/// They may be combined, except for GUI_HOOK_FLAG_PRIM_FILTER which cannot
/// have any other flags.
enum GUI_PrimitiveHookFlags
{
    /// Hook has no special behaviour.
    GUI_HOOK_FLAG_NONE		= 0,

    /// Hook will render extra bits, but still allow lower-priority hooks or
    /// the native primitive renderer to render as well.
    GUI_HOOK_FLAG_AUGMENT_PRIM  = 0x2,

    /// Hook will modify the geometry and produce a new GT_Primitive, but not
    /// do any rendering itself.
    GUI_HOOK_FLAG_PRIM_FILTER	= 0x4
};

enum GUI_RenderMask
{
    GUI_RENDER_MASK_GL1	= 0x1,	// Deprecated: GL1 and GL2 are obsolete and
    GUI_RENDER_MASK_GL2 = 0x2,  // provided only for H13 code compatibility.
    
    GUI_RENDER_MASK_GL3 = 0x4,	// Hook works with GL3
    GUI_RENDER_MASK_GL4 = 0x8,  // Hook works only with GL4+

    GUI_RENDER_MASK_ALL = 0xFFFFFFFC   // all but GL1/GL2.
};

enum GUI_GeometryOptionFlags
{
    GUI_GEO_OPT_NONE 				= 0,
    GUI_GEO_OPT_REFINE_ON_ACTIVATION 		= 1,
    GUI_GEO_OPT_REFINE_ON_DEACTIVATION 		= 1 << 1,
    GUI_GEO_OPT_GLOBAL_TOGGLE_VALUE 		= 1 << 2
};


class GUI_API GUI_PrimitiveHook
{
public:
    /// Only one primitive hook will ever be created.
    /// Each hook requires a name for identification and error reporting.
    /// A hook can target all renderers, a subset, or a specific renderer.
		   GUI_PrimitiveHook(const char *hook_name,
				     GUI_RenderMask mask = GUI_RENDER_MASK_ALL);

    /// Name of this hook
    const char *   getName() const	 { return myName; }

    /// Bitmask of renderers supported by this hook.
    GUI_RenderMask getRenderMask() const { return myRenderMask; }

    /// Create a new primitive based on the GT or GEO prim. The info and
    /// cache_name should be passed to the GR_Primitive constructor.
    /// 'processed' should indicate that the primitive is consumed
    /// (GR_PROCESSED), or processed but available for further hooks or
    /// internal Houdini processing (GR_PROCESSED_NON_EXCLUSIVE).
    /// GR_NOT_PROCESSED should only be returned if a GR_Primitive was not
    /// created. Hooks with GUI_HOOK_FLAG_PRIM_FILTER do not call this method.
    virtual GR_Primitive  *createPrimitive(const GT_PrimitiveHandle &gt_prim,
					   const GEO_Primitive	*geo_prim,
					   const GR_RenderInfo	*info,
					   const char		*cache_name,
					   GR_PrimAcceptResult  &processed);

    /// For hooks with GUI_HOOK_FLAG_PRIM_FILTER, this method is called instead
    /// of createPrimitive.
    virtual GT_PrimitiveHandle filterPrimitive(const GT_PrimitiveHandle &gt_prm,
					       const GEO_Primitive  *geo_prm,
					       const GR_RenderInfo  *info,
					       GR_PrimAcceptResult  &processed);
protected:    
    virtual		  ~GUI_PrimitiveHook();
    
private:
    UT_String	   myName;
    GUI_RenderMask myRenderMask;
    
    friend class   DM_RenderTable;
};

#endif
