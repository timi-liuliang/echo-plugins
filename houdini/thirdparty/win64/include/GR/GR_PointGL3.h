/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_PointGL3 ( GR Library, C++)
 *
 * COMMENTS:
 *	Simple point renderer. Cd, alpha and pscale are supported.
 */
#ifndef GR_PointGL3_h
#define GR_PointGL3_h

#include "GR_API.h"
#include "GR_GeoRender.h"
#include <RE/RE_ShaderHandle.h>

class GR_API GR_PointGL3 : public GR_GeoRender
{
public:
	     GR_PointGL3(GR_Primitive *prim);
    virtual ~GR_PointGL3();

    virtual const char *className() const { return "GR_PointGL3"; }
    
    virtual void	update(RE_Render *r,
			       const GT_PrimitiveHandle &primh,
			       const GR_UpdateParms	&parms,
			       const char		*cache_name);
    
    virtual void	draw(RE_Render *r,
			     GR_RenderMode mode,
			     GR_RenderFlags flags,
			     GR_DrawParms dp);
    
    virtual exint	getNumGLPrimitives() const;
    virtual bool	hasTransparency() const { return myHasAlpha; }
    
private:
    void		updatePointAttribs(RE_Render *r,
					   const GT_PrimitiveHandle &primh,
					   const GR_UpdateParms   &parms);
    void		drawPrivate(RE_Render *r,
				    GR_RenderMode mode,
				    GR_RenderFlags flags,
				    GR_DrawParms dp);
    
    void		drawUtility(RE_Render *r,
				    GR_RenderMode mode,
				    GR_RenderFlags flags,
				    GR_DrawParms dp);
    
    static RE_ShaderHandle	thePointShader;
    static RE_ShaderHandle	thePixelShader;
    bool			myHasCdAttrib;
    bool			myHasAlphaAttrib;
    bool			myHasAlpha;
    bool			myObjectSelected;
};

#endif
