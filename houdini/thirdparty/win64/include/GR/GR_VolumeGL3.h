/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_VolumeGL3.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Volume rendering in GL3.
 */
#ifndef GR_VolumeGL3_h
#define GR_VolumeGL3_h

class UT_Color;
class RE_OGLTexture;
class GU_PrimVolume;
struct gr_VolumeAngleVector;
class gr_GLViewAlignedSlice;

#include "GR_GeoRender.h"

#include <RE/RE_Geometry.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_Lock.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Vector3.h>
#include <GU/GU_Detail.h>
#include <GEO/GEO_PrimVolume.h>

class GR_VolumeGL3 : public GR_GeoRender
{
public:
	     GR_VolumeGL3(GR_Primitive *prim);
    virtual ~GR_VolumeGL3();

    virtual const char *className() const { return "GR_VolumeGL3"; }
    
    virtual void	update(RE_Render *r,
			       const GT_PrimitiveHandle &primh,
			       const GR_UpdateParms	&p,
			       const char		*cache_name);

    virtual void	viewUpdate(RE_Render *r,
				   const GR_DisplayOption &opts,
				   const char *cache_name,
				   RE_CacheVersion       geo_version,
				   RE_CacheVersion       select_version);
    virtual void	draw(RE_Render *r,
			     GR_RenderMode mode,
			     GR_RenderFlags flags,
			     GR_DrawParms dp);

    void		setTexture(const RE_TextureHolder &tex)
			{
			    myVolumeTexture = tex;
			}

    void		updateTexture(const RE_TextureHolder &tex);
    void		clearTexture();

    static void		cleanupSlices();
    
    virtual exint getNumGLPrimitives() const;
    
private:
    bool		updateViewAligned(RE_Render *r,
				  const GR_DisplayOption &drawOpt);
    static void		sliceCube(void *data, int idx, int num, UT_Lock &lock);
    static void	barycenterTriangulateConvexPoly(const UT_Vector3Array &points,
						float tolerance2,
						UT_Vector3Array *out_points,
						int total_depth=1, int d=0);
    static void		sortPolygonCCW(gr_VolumeAngleVector *points, int n);
    static UT_Vector3	pointAverage(const UT_Vector3Array &points);
    static void		triangulate(const UT_Vector3Array &points,
				    float tolerance2,
				    UT_Vector3Array *out_points,
				    int total_depth=-1, int d=0);

    bool		instanceSlices(RE_Render *r,
				       UT_Matrix4D &view, UT_Matrix4D &object,
				       fpreal zinc);
    void		drawSlices(RE_Render *r,
				   UT_ValArray<gr_GLViewAlignedSlice *> *slices,
				   GR_RenderMode render_mode,
				   const GR_DisplayOption *opt);
    void		clearSlices();

    GU_Detail		myBox;
    GEO_PrimVolumeXform myVolumeSpace;
    UT_Vector3I		myVolumeRes;
    UT_BoundingBox	myVolumeBounds;
    RE_TextureHolder    myVolumeTexture;
    fpreal32		myDepthStep;
    GR_RenderMode	myDrawMode;
    UT_Matrix4DArray	myInstanceTransforms;
    
    static int		theVolumePassID;
    
    UT_ValArray<gr_GLViewAlignedSlice *> myGLVolumeSlices;
 };

#endif
