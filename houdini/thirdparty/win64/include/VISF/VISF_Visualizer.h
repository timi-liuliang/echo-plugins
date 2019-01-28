/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VISF_Visualizer.h (VISF Library, C++)
 *
 * COMMENTS:
 *      A VISF_Visualizer is a custom display option which can be toggled on/off
 *      and contain any arbitrary settings.
 */

#ifndef __VISF_Visualizer__
#define __VISF_Visualizer__

#include "VISF_API.h"

#include "VISF_PrimitiveType.h"
#include <GUI/GUI_DisplayOption.h>
#include <GUI/GUI_VisInterface.h>
#include <VIS/VIS_Visualizer.h>
#include <UT/UT_Array.h>

#include <GR/GR_Defines.h>
#include <GT/GT_Handles.h>

class GEO_Primitive;
class GR_Primitive;
class GR_RenderInfo;
class GU_Detail;
class GUI_GeoRender;
class RE_Render;
class VISF_Primitive;

class VISF_API VISF_Visualizer : public VIS_Visualizer
{
public:
			 VISF_Visualizer();
    virtual 		~VISF_Visualizer();

    /// Determines which primtive types can be passed to the visualizer
    /// for filtering or creation.
    virtual bool	 acceptsPrimitiveType(VISF_PrimitiveType type)
			    { return false; }

    virtual
    GR_DecorVisibility	 getDecorVisibility()
			    {	return GR_DECOR_ALWAYS_VISIBLE; }

    virtual void	 renderDecorations(RE_Render *r,
				const UT_Array<GUI_GeoRender *> &geos,
				const GUI_DisplayOption &disp_opts,
				GR_DecorRenderFlags flags) const {}

    virtual bool	 canSetupRenderForGeo(RE_Render *r,
				GUI_DisplaySetType dset,
				GR_RenderMode render_mode,
				const GUI_GeoRender *geo,
				const GUI_DisplayOption &disp_opts);

    virtual void	 setupRender(RE_Render *r,
				GUI_DisplaySetType dset,
				GR_RenderMode render_mode,
				const GUI_GeoRender *geo,
				GUI_DisplayOption &disp_opts) const {}

    virtual void	 cleanupRender(RE_Render *r,
				GUI_DisplaySetType dset,
				GR_RenderMode render_mode,
				const GUI_GeoRender *geo,
				GUI_DisplayOption &disp_opts) const {}

    virtual
    GT_PrimitiveHandle	 filterPrimitive(const GU_Detail *gdp,
				const GT_PrimitiveHandle &gt_prm,
				const GEO_Primitive *geo_prm,
				const GR_RenderInfo *info,
				GR_PrimAcceptResult &processed,
				GUI_VisFlags &vis_flags) const;

    virtual
    VISF_Primitive 	*createPrimitive(const GU_Detail *gdp,
				const GT_PrimitiveHandle &gt_prim,
				const GEO_Primitive *geo_prim,
				const GR_Primitive *gr_prim,
				const GR_RenderInfo *info,
				const char *cache_name,
				GR_PrimAcceptResult &processed) const;

    virtual VISF_Visualizer *castToVISR() { return this; }
protected:
    /// Copy constructore
			 VISF_Visualizer(const VISF_Visualizer &src);
};

static inline
UT_Vector2F
VISgetViewCoord(const UT_Vector3F& p,
		const UT_Matrix4D *xform,
		const UT_DimRect *viewport)
{
    UT_Vector4F	     pos(p(0), p(1), p(2), 1.0);
    fpreal32	     scx, scy;
    // Get the screen space x,y,z after the view and projection transforms.
    pos *= *xform;
    pos.dehomogenize();
    scx = (pos.x() * 0.5 + 0.5) * viewport->width() + viewport->x1();
    scy = (pos.y() * 0.5 + 0.5) * viewport->height() + viewport->y1();
    return UT_Vector2F(scx, scy);
}

#endif
