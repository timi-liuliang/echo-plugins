/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GUI_VisInterface.h ( GR Library, C++)
 *
 * COMMENTS:
 * 	Basic definition needed to access functions defined in VIS which is
 * 	set above the GUI level.
 */

#ifndef __GUI_VisInterface_h__
#define __GUI_VisInterface_h__

#include "GUI_API.h"
#include "GUI_PrimitiveHook.h"
#include <GR/GR_UpdateParms.h>
#include <VIS/VIS_Defines.h>

class GUI_DetailLook;
class GUI_GeoRender;

typedef VIS_Context GUI_VisContext;

enum GUI_VisFlags
{
    GUI_VIS_FLAG_NONE 			= 0x0,
    GUI_VIS_FLAG_SKIP_RENDER		= 0x1
};

class GUI_API GUI_VisInterface
{
public:
    GUI_VisInterface() {}
    virtual ~GUI_VisInterface() {}

    virtual exint	 bumpClock() = 0;

    virtual
    GT_PrimitiveHandle	 filterPrimitive(GUI_GeoRender *geo,
				const GT_PrimitiveHandle &gt_prm,
				const GEO_Primitive *geo_prm,
				const GR_RenderInfo *info,
				GR_PrimAcceptResult &processed,
				UT_Array<GUI_VisFlags> &vis_flags) = 0;

    virtual void	 createPrimitive(GUI_GeoRender *geo,
    				const GT_PrimitiveHandle &gt_prim,
    				const GEO_Primitive *geo_prim,
				const GR_Primitive *gr_prim,
    				const GR_RenderInfo *info,
    				const char *cache_name,
    				GR_PrimAcceptResult &processed,
				UT_Array<GR_Primitive *> &prims) = 0;

    virtual
    GR_UpdateReason	 getUpdateReason(const GUI_GeoRender *geo,
                   	                 bool &refine_on_update) = 0;

    virtual void	 getRequestedAttributes(
				const GUI_DetailLook *look,
				UT_Map<UT_StringRef, int> &needed) = 0;

    virtual bool	 isHookVisualizerActive(const char *vis_name,
                		GUI_VisContext context) = 0;

    virtual
    GUI_VisFlags	 getActiveVisFlags(const GUI_GeoRender *geo,
				UT_Array<GUI_VisFlags> &vis_flags,
				GUI_VisContext context,
				int display_set) = 0;

    static void 	 setHandler(GUI_VisInterface *handler)
			    { theHandler = handler; }

    static
    GUI_VisInterface 	*getHandler()
			    { return theHandler; }

    static GUI_VisInterface	*theHandler;
};

#endif
