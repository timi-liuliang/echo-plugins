/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_PrimPolySurface.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Generic base class for polygon meshes and soups for all GL renderers
 */
#ifndef GR_PrimPolySurface_h
#define GR_PrimPolySurface_h

#include "GR_Defines.h"
#include "GR_GeoRender.h"

#include <GT/GT_Primitive.h>

#include <UT/UT_NonCopyable.h>
#include <UT/UT_Vector3.h>

class GT_DataArray;
class GT_PrimPolygonMesh;

class GR_API GR_PolySurface : public GR_GeoRender, private UT_NonCopyable
{
public:
    static   GR_PolySurface *create(GR_Primitive *parent, bool convex);
    
	     GR_PolySurface(GR_Primitive *prim,
			    bool needs_convexing);
    virtual ~GR_PolySurface();

    virtual const char *className() const { return "GR_PolySurface"; }

    virtual void update(RE_Render *r,
			const GT_PrimitiveHandle &primh,
			const GR_UpdateParms &p,
			const char *cache_name);

    virtual bool hasTransparency() const { return myHasAlpha; }

    GT_PrimitiveHandle getConvexGeometry() { return myConvexGeo; }
    
    virtual exint      getNumGLPrimitives() const;
    
protected:
    virtual void       retireGeometry();
    
    virtual int buildVertexAttributes(RE_Render		       *r,
				      const GT_PrimPolygonMesh *pmesh,
				      const GR_UpdateParms     &p,
				      bool needs_normal,
				      bool needs_uv,
				      bool new_geo,
				      bool &vertex_promote) = 0;

    // build based on a Material Atlas
    virtual void buildConnectivity(RE_Render *r,
				   const GT_PrimPolygonMesh *pmesh,
				   const char *cache_name,
				   RE_CacheVersion geo_version,
				   const RE_MaterialAtlas &mats,
				   int npoints,
				   bool wireframe,
				   bool vertex_promote,
				   bool convexed) = 0;

    virtual GT_PrimitiveHandle convexSurface(const GT_PrimPolygonMesh *pmesh)=0;

    GT_PrimitiveHandle convexWithEdgeInfo(const GT_PrimPolygonMesh *pmesh,
					  GT_DataArrayHandle &ph,
					  GT_DataArrayHandle &vh,
					  GT_DataArrayHandle &vch);

    GT_DataArrayHandle getPointNormals() { return myAutoGenPointN; }
    void	       freePointNormals();

    virtual void updateBoneTransforms(RE_Render *r,
				      const GT_DataArrayHandle &a) = 0;

    virtual int	 numMaterials() const = 0;

    bool	       myHasAlpha;
    
private:
    GT_PrimitiveHandle convexMesh(const GT_PrimPolygonMesh *pmesh,
				  const GR_UpdateParms &p,
				  bool &convexed,
				  bool prev_convex_state);

    void		cacheConvexMesh(const GT_PrimitiveHandle &convexed,
					const GT_PrimPolygonMesh *original);
    void		clearCachedConvexMesh();
    
    bool	       myNeedsConvex;
    GT_PrimitiveHandle myOriginalGeo;
    GT_PrimitiveHandle myConvexGeo;
    GT_DataArrayHandle myAutoGenPointN;
    int64	       myTopVersion;
    int64	       myMatIDVersion;
    
    GT_DataArrayHandle myConvexUniformIndirect;
    GT_DataArrayHandle myConvexVertexIndirect;
    GT_DataArrayHandle myConvexVertexList;
    GT_DataArrayHandle myConvexPrimInfo;
    GT_DataArrayHandle myConvexVertInfo;
    UT_Pair<int64, int64> myConvexVersion;
};
#endif

