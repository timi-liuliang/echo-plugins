/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_GeoRender.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Base class for all drawing methds that Houdini is natively capable of
 *	rendering with OpenGL.
 */
#ifndef GR_GeoRender_H
#define GR_GeoRender_H

class RE_ElementArray;
class RE_Geometry;
class RE_Render;
class RE_Shader;
class RE_VertexArray;
class OP_Node;
class GU_Detail;
class GR_DisplayOption;
class GR_RenderInfo;
class UT_StringArray;
class GT_CountArray;
class GA_BreakpointGroup;
class GR_Primitive;

#include <RE/RE_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_UniquePtr.h>
#include <GT/GT_DataArray.h>
#include <GT/GT_Primitive.h>
#include <GA/GA_ElementGroup.h>
#include <GA/GA_EdgeGroup.h>

#include "GR_DrawParms.h"
#include "GR_MaterialGroups.h"
#include "GR_UpdateParms.h"
#include "GR_ErrorManager.h"

class GR_API GR_GeoRender
{
public:
	     GR_GeoRender(GR_Primitive *parent_prim);
    virtual ~GR_GeoRender();

    virtual const char *className() const = 0;

    /// Update the geometry of this render object using the GT primitive. No
    /// rendering commands should be issued.
    virtual void	update(RE_Render		*r,
			       const GT_PrimitiveHandle &primh,
			       const GR_UpdateParms     &p,
			       const char		*cache_name) = 0;

    /// add an array to, or update an array in, an instance group. If
    /// 'instance_group' is -1, RE_VertexArray must specify an instance group
    /// via setInstanceGroup().
    virtual void	updateInstanceGroupArray(const RE_VertexArray *array,
						 int instance_group = -1,
						 const char *attrib_name = NULL,
						 RE_GenericAttribID id
						    = RE_GENATTRIB_NONE);

    /// Called when the view changes, if the parent primitive requested view
    /// changes
    virtual void	viewUpdate(RE_Render *r,
				   const GR_DisplayOption &opts,
				   const char *cache_name,
				   RE_CacheVersion geo_version,
				   RE_CacheVersion select_version)
			{}

    /// Normal drawing method. Only issue rendering commands.
    virtual void	draw(RE_Render *r,
			     GR_RenderMode mode,
			     GR_RenderFlags flags,
			     GR_DrawParms draw_parms) = 0;

    /// By default, no instanced drawing support.
    virtual bool	supportsInstances() const { return false; }

    RE_Geometry		*createGeometry(int npoints = 0);
    RE_Geometry		*getGeometry()	       { return myGeometry; }
    virtual bool	 isConvexed() const    { return myConvexFlag; }

    /// Called if another entity makes changes to the RE_Geometry after update()
    /// is called (usually the entity is a GR_Primitive object).
    virtual void		 geometryUpdated(RE_Render *)     {}

    /// returns true if this drawable renders some transparent elements
    virtual bool		 hasTransparency() const { return false; }


    /// return the number of GL primitives that this renderer produces. May be
    /// roughly estimated.
    virtual exint		 getNumGLPrimitives() const = 0;
    
    // Accessors to various high-level data
    bool			 showSelections() const;
    GR_RenderVersion		 getRenderVersion() const;

    virtual bool		 canFrustumCull() { return true; }
    bool			 inViewFrustum(const UT_Matrix4D &objviewproj);

    GR_BasePrimType		 getParentType() const;
    
    void			 buildInformation(bool prim_id,
						  bool point_id)
				    {
					myBuildPrimInfo = prim_id;
					myBuildPointInfo = point_id;
				    }
    bool		         shouldBuildPrimID() const
				    { return myBuildPrimInfo; }
    bool		         shouldBuildPointID() const
				    { return myBuildPointInfo; }
    
    void			 setSelectionMask(GR_AttribMask selection_mask)
				    { mySelectionMask = selection_mask; }
    GR_AttribMask		 getSelectionMask() const
				    { return mySelectionMask; }

    void			 setSelectionDisplayMode(GR_SelectMode m)
				    { myCurrentSelection = m; }
    GR_SelectMode		 getSelectionDisplayMode() const
				    { return myCurrentSelection; }

    void			 setSelectionInstance(int i)
				    { myCurrentPointSelectInstance = i; }
    int				 getSelectionInstance() const
				    { return myCurrentPointSelectInstance; }

    void			 setMaterialOverride(int mat_index)
				    { myMaterialOverride = mat_index; }
    void			 clearMaterialOverride()
				    { myMaterialOverride = -1; }

    bool			 getDetailPrimID(int &id) const
				    {
					if(myHasDetailPrimID)
					{
					    id = myDetailPrimID;
					    return true;
					}
					id = 0;
					return false;
				    }

    virtual void		retireGeometry();

    // Creates an RE_OverrideList (or adds to an existing one) for the color
    // and uv overrides if required. Cleanup will undo any changes to the
    // state setup made.
    void		setupGeoOverride(RE_Render *r,
				const GR_DisplayOption *opt,
				RE_Geometry *geo,
				UT_UniquePtr<RE_OverrideList> &override_list);
    void		cleanupGeoOverride(RE_Render *r,
				const GR_DisplayOption *opt,
				RE_Geometry *geo);

    // Message and error reporting
    void	addUpdateMessage(GR_ErrorManager::Source err_source,
				 GR_ErrorCode	 gr_code,
				 UT_StringHolder extra_text=UT_StringHolder(),
				 UT_StringHolder src_name=UT_StringHolder());
    void	addUpdateWarning(GR_ErrorManager::Source err_source,
				 GR_ErrorCode	 gr_code,
				 UT_StringHolder extra_text=UT_StringHolder(),
				 UT_StringHolder src_name=UT_StringHolder());
    void	addUpdateError  (GR_ErrorManager::Source err_source,
				 GR_ErrorCode	 gr_code,
				 UT_StringHolder extra_text=UT_StringHolder(),
				 UT_StringHolder src_name=UT_StringHolder());
    void	addUpdateFatal  (GR_ErrorManager::Source err_source,
				 GR_ErrorCode	 gr_code,
				 UT_StringHolder extra_text=UT_StringHolder(),
				 UT_StringHolder src_name=UT_StringHolder());   
    void	addRedrawMessage(GR_ErrorManager::Source err_source,
				 GR_ErrorCode	 gr_code,
				 UT_StringHolder extra_text=UT_StringHolder(),
				 UT_StringHolder src_name=UT_StringHolder());
    void	addRedrawWarning(GR_ErrorManager::Source err_source,
				 GR_ErrorCode	 gr_code,
				 UT_StringHolder extra_text=UT_StringHolder(),
				 UT_StringHolder src_name=UT_StringHolder());
    void	addRedrawError  (GR_ErrorManager::Source err_source,
				 GR_ErrorCode	 gr_code,
				 UT_StringHolder extra_text=UT_StringHolder(),
				 UT_StringHolder src_name=UT_StringHolder());
    void	addRedrawFatal  (GR_ErrorManager::Source err_source,
				 GR_ErrorCode	 gr_code,
				 UT_StringHolder extra_text=UT_StringHolder(),
				 UT_StringHolder src_name=UT_StringHolder());

    
    static RE_VertexArray *getDirtyAttribArray(
				RE_Render		 *r,
				RE_Geometry		 *geo,
				const char		 *attrib_name,
				const GT_DataArrayHandle &h,
				const GR_DisplayOption	 &opts,
				RE_ArrayType		  atype,
				RE_CacheVersion		  attr_version);

    static void		fillGeoAttribs(RE_Render	  *r,
				       const GT_Primitive *prim,
				       const GR_UpdateParms &p,
				       RE_Geometry	*geo,
				       bool		 needs_nml,
				       bool		 needs_uv,
				       bool		 need_prim_vert_info, 
				       const GT_DataArrayHandle &point_norm,
				       GR_BasePrimType   packed_type,
				       bool		 build_point_id,
				       bool		 build_prim_id,
				       GR_GeoRender	*geo_ren = nullptr,
				       bool *has_detail_prim_id = nullptr,
				       int  *detail_prim_id = nullptr,
				       int  *num_bone_deform = nullptr);
				       
protected:
    void	initLights(RE_Render *r,
			   RE_Shader *sh,
			   GR_RenderMode mode,
			   GR_RenderFlags flags,
			   const GR_DisplayOption *opt);
    
    GR_AttribMask hasAttribute(const GT_Primitive *prim,
			       const char *name,
			       GT_DataArrayHandle &h) const;

    // For picking, these methods turn the GT __primitive_id, __vertex_id and
    // points (__vertex_id + vertexlist)
    void	  buildPointID(RE_Render *r,
			       const GT_Primitive &prim,
			       const GU_ConstDetailHandle &geo,
			       RE_CacheVersion geo_version);
    void	  buildPrimitiveID(RE_Render		*r,
				   const GT_Primitive	&primh,
				   const GU_ConstDetailHandle	&geo,
				   RE_CacheVersion	 geo_version);

    // GL32 stuff.
    GR_SelectMode	buildSelection(RE_Render	        *r,
				       const GR_UpdateParms	&parms,
				       RE_Geometry	        &obj,
				       const GT_PrimitiveHandle &primh,
				       const GT_DataArrayHandle &vertexlist);
    
    GR_SelectMode	addToPointSelection(RE_Render		*r,
					    RE_VertexArray	*select_array,
					    int			 length,
					    const GU_Detail	&geo, 
					    const GT_DataArrayHandle &vh,
					    const GT_DataArrayHandle &vl,
					    const GA_PointGroup *pntsel,
					    bool use_vertex_for_point_select,
					    bool &init);
    
    GR_SelectMode	addToVertexSelection(RE_Render		*r,
					     RE_VertexArray	*select_array,
					     int		 length,
					     const GU_Detail	&geo, 
					     const GT_DataArrayHandle &vh,
					     const GA_VertexGroup *vertsel,
					     bool		&init);
    
    GR_SelectMode	addToEdgeSelection(RE_Render		*r,
					   RE_VertexArray	*select_array,
					   int			 length,
					   const GU_Detail	&geo, 
					   const GT_DataArrayHandle &vt,
					   const GT_DataArrayHandle &vert_list,
					   const GA_EdgeGroup   *edgesel,
					   bool			&init);
    
    GR_SelectMode	addToPrimSelection(RE_Render		    *r,
					   RE_VertexArray	    *sel_array,
					   const GT_DataArrayHandle &vh,
					   const GA_PrimitiveGroup  *primsel,
					   bool			    &init);

    virtual GR_SelectMode addToBreakpointSelection(RE_Render *r,
						   RE_VertexArray *select,
						   int bpnt_len,
						   const GA_BreakpointGroup *bps,
						   bool &init);

    GR_SelectMode	buildInstanceSelection(RE_Render *r,
					       const GR_UpdateParms &p,
					       RE_Geometry &obj);

    void		fillAttributesGL3(RE_Render	       *r,
					  const GT_Primitive   *prim,
					  const GR_UpdateParms &p,
					  RE_Geometry	*geo,
					  bool		 needs_nml,
					  bool		 needs_uv,
					  bool		 need_prim_vert_info, 
					  const GT_DataArrayHandle &point_norm);

    /// Set selection uniforms SELECT_MODE and SELECT_INSTANCE. Returns an int
    /// which should be passed to cleanup selection.
    virtual int		setupSelection(RE_Render *r,
				       const GR_DisplayOption *opt,
				       GR_RenderFlags flags) const;
    
    /// Clean up selection uniforms; much be paired with a setupSelection().
    virtual void	cleanupSelection(RE_Render *r, int key) const;

    GT_PrimitiveHandle  getCachedGTPrimitive();
    void		cacheGTPrimitive(GT_PrimitiveHandle &primh);

    void		updateWireColor(RE_Render *r,
					RE_Geometry *geo,
					const GR_DisplayOption *opt,
					int start_draw_idx,
					int num_draw_idx = 1) const;

    int			 getCurrentLayer() const
			 { return myCurrentLayer; }

    // for the materials in matlist, map them to the shop_*path in 'prim'.
    // returns 'matmap' array which indicates what the index in shop_*path is
    // for the materials it matlist.
#ifdef USE_MATERIAL_MAP
    bool buildMaterialMap(const GT_Primitive    *prim,
			  const RE_MaterialAtlas &mats,
			  UT_IntArray		&matmap);
#endif
public:
    // For derived class's parallel helpers
    GT_DataArrayHandle	  buildPointIDData(const GT_Primitive &prim,
					   const GU_ConstDetailHandle &geo,
					   int npoints);
    GT_DataArrayHandle	  buildPrimitiveIDData(const GT_Primitive &prim,
					       const GU_ConstDetailHandle &geo);
    
    void	  setGeoBounds(const UT_BoundingBox &b) { myBounds = b; }
    const UT_BoundingBoxD &getGeoBounds() const { return myBounds; }

protected:
    RE_Geometry		*myGeometry;
    const GR_RenderInfo *myInfo;
    GR_MaterialGroups	 myMatGroups;
    UT_BoundingBoxD	 myBounds;
    unsigned		 myBuildPointInfo : 1,
			 myBuildPrimInfo : 1,
			 myHasDetailPrimID : 1,
			 myHasInstancedTransform : 1,
			 myHasIdentityTransform : 1,
			 myUseBoneDeform : 1,
			 myNumBoneDeform : 2;

    UT_Matrix4D		 myPrimitiveTransform;
    GR_AttribMask	 mySelectionMask;
    GR_SelectMode	 myCurrentSelection;
    int			 myCurrentPointSelectInstance;
    int			 myMaterialOverride;
    int			 myConvexFlag;
    int			 myDetailPrimID;

private:
    GR_Primitive	*myParentPrim;
    int			 myCurrentLayer;
};

#endif
