/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_PolySurfaceGL3.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Base class for polygon meshes and soups for the GL3 renderer
 */
#ifndef GR_PolySurfaceGL3_h
#define GR_PolySurfaceGL3_h

class RE_Render;
class RE_Geometry;

#include "GR_PolySurface.h"
#include <RE/RE_ShaderHandle.h>
#include <RE/RE_ShaderRegistry.h>

class GR_API GR_PolySurfaceGL3 : public GR_PolySurface
{
public:
    GR_PolySurfaceGL3(GR_Primitive *prim,
		      bool needs_convexing = false);
    virtual ~GR_PolySurfaceGL3();

    virtual const char *className() const { return "GR_PolySurfaceGL3"; }

    virtual void	draw(RE_Render *r,
			     GR_RenderMode mode,
			     GR_RenderFlags flags,
			     GR_DrawParms dp);
    
    virtual bool	supportsInstances() const { return true; }
    virtual bool	isConvexed() const { return (myConvexFlag &&
						     !myMultiMaterialFlag); }
    virtual bool	canFrustumCull();
    
protected:
    void		drawBeauty(RE_Render *r,
				   GR_RenderMode mode,
				   GR_RenderFlags flags,
				   GR_DrawParms dp);

    void		drawUtility(RE_Render *r,
				    GR_RenderMode mode,
				    GR_RenderFlags flags,
				    GR_DrawParms dp);

    void		drawHiddenLine(RE_Render *r,
				       GR_RenderMode mode,
				       GR_RenderFlags flags,
				       GR_DrawParms dp);
    
    int			buildSurfaceAttributes(RE_Render *r,
				const GT_Primitive *prim,
				const GR_UpdateParms &parms,
				bool needs_normal, bool needs_uv);

    bool		getGeometryHasColor() const;

    RE_Shader	       *getMaterialShader(RE_Render *r,
					  GR_RenderMode mode,
					  GR_RenderFlags flags,
					  int  nlights,
					  int prim_vertex,
					  bool has_color_attrib,
					  bool draw_textures,
					  bool needs_displacement,
					  bool multi_material,
					  bool fill_selections,
					  bool has_prim_transforms);

    virtual void updateBoneTransforms(RE_Render *r,const GT_DataArrayHandle &a);
    virtual int	 numMaterials() const { return myMatGroups.getNumGroups(); }


private:
    virtual int		 buildVertexAttributes(RE_Render *r,
					       const GT_PrimPolygonMesh *pmesh,
					       const GR_UpdateParms &parms,
					       bool needs_normal, bool needs_uv,
					       bool new_geometry,
					       bool &vertex_promote);

    virtual void	 buildConnectivity(RE_Render *r,
					   const GT_PrimPolygonMesh *pmesh,
					   const char *cache_name,
					   RE_CacheVersion top_version,
					   const RE_MaterialAtlas &mats,
					   int npoints,
					   bool wireframe,
					   bool vertex_promote,
					   bool convexed);

    void		 buildPrimTransforms(RE_Render *r,
					     const GR_UpdateParms &p,
					     const GT_DataArrayHandle &ptr);
    void		 buildPrimVisibility(RE_Render *r,
					     const GR_UpdateParms &p,
					     const GT_DataArrayHandle &prv);
    
    virtual GT_PrimitiveHandle convexSurface(const GT_PrimPolygonMesh *pmesh);
    
    void	 drawAsPoints(RE_Render *r,
			      GR_DrawParms dp);
    void	 drawAllGeometry(RE_Render *r,
				 GR_RenderFlags flags,
				 GR_DrawParms dp,
				 RE_OverrideList *override_list,
				 bool use_displacement);
    
    void	 drawLayered(RE_Render *r,
			     int mat_index,
			     int nlayers,
			     UT_StringHolder & current_layer,
			     const RE_MaterialPtr &mat,
			     int instances,
			     UT_UniquePtr<RE_OverrideList> &overrides,
			     bool use_displacement);
    
    bool	 getUVSetForLayer(const RE_Material *material,
				  const int layer,
				  UT_UniquePtr<RE_OverrideList>&override_list,
				  bool &has_overrides,
				  int &uv_override_idx,
				  UT_StringHolder *uvset_override = nullptr);
    void	 checkForUVs(const UT_StringHolder &uvset_name);
    void	 clearCachedDrawData();
    
    static RE_ShaderRegistry	&getShaders();
    static RE_ShaderRegistry	theShaders;
    static bool 		theShadersAreRegistered;

    bool	    myPrimSelectionInit;
    bool	    myVertexSelectionInit;
    RE_VertexArray *myPrimSelection;
    RE_VertexArray *myVertexSelection;
    RE_VertexArray *myBoneTransforms;
    void	   *myLastShader;
    UT_String	    myMatGroup;
    int		    myNumBoneTransforms;
    RE_CacheVersion myTopConnectID;
    unsigned	    myHasCd : 1,
		    myLastCd:1,
		    myLastPrimVert:2,
		    myLastNeedsUV:1;
    
    bool	    myMultiMaterialFlag;
    bool	    myLastShowSelections;
    bool	    myHasPrimTransform;
    bool	    myHasPrimVisibility;

    UT_IntArray	    myMaterialMap;
};

#endif
