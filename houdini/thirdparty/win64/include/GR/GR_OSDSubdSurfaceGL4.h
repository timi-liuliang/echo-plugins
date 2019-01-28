/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_OSDSubdSurfaceGL4.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Class for rendering OSD subdivision surface using GL4.
 */
#ifndef GR_OSDSubdSurfaceGL4_h
#define GR_OSDSubdSurfaceGL4_h

#include "GR_Defines.h"
#include "GR_PolySurfaceGL3.h"

#include <UT/UT_UniquePtr.h>

class GT_UtilOpenSubdiv;
class GT_OSDDrawContext;

class GR_API GR_OSDSubdSurfaceGL4 : public GR_PolySurfaceGL3
{
public:
	     GR_OSDSubdSurfaceGL4(GR_Primitive *prim);
    virtual ~GR_OSDSubdSurfaceGL4();

    virtual const char	*className() const { return "GR_OSDSubdSurfaceGL4"; }

    virtual void	 update(RE_Render *r,
				const GT_PrimitiveHandle &primh,
				const GR_UpdateParms &p,
				const char *cache_name);

    virtual void	 draw(RE_Render *r,
			      GR_RenderMode mode,
			      GR_RenderFlags flags,
			      GR_DrawParms dp);

private:
#if defined(HOUDINI_USE_OSD)
    void		 drawOSDDrawContext(RE_Render *r,
					    GR_RenderMode mode,
					    GR_RenderFlags flags,
					    GR_DrawParms dp,
					    const GT_OSDDrawContext *dc);

    UT_UniquePtr<GT_UtilOpenSubdiv>	     myOsd;
    int64				     myVertexAttributeDataIdHash;
    float				     myTessLevel;
#endif
};

#endif

