/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GUI_GeoRender.h ( GUI Library, C++)
 *
 * COMMENTS:
 *	General rendering class for a GU_Detail.
 */
#ifndef GUI_GeoRender_h
#define GUI_GeoRender_h

#include <UT/UT_BoundingBox.h>
#include <UT/UT_Lock.h>
#include <UT/UT_Rect.h>
#include <UT/UT_String.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_Array.h>
#include <DEP/DEP_MicroNode.h>
#include <RE/RE_Types.h>
#include <RE/RE_Texture.h>
#include <RE/RE_TimerQuery.h>
#include <RE/RE_ContextSpecificTable.h>
#include <GA/GA_Attribute.h>
#include <GR/GR_Defines.h>
#include <GR/GR_RenderInfo.h>
#include <GT/GT_Primitive.h>
#include <GU/GU_DetailHandle.h>
#include <GU/GU_SelectType.h>
#include "GUI_GroupData.h"
#include "GUI_RenderCommon.h"
#include "GUI_DisplayOption.h"
#include "GUI_DetailLook.h"
#include "GUI_PrimitiveHook.h"
#include "GUI_RenderCommon.h"
#include "GUI_API.h"

class GA_EdgeGroup;
class GA_ElementGroup;
class GU_Detail;
class GEO_PrimClassifier;
class GEO_DetachedHedgeInterface;
class RE_Render;
class RE_Geometry;
class RE_LightList;
class RE_ReflectMap;
class RE_Shader;
class RE_VertexArray;
class RE_ElementArray;
class GUI_DetailLook;
class GA_ROAttributeRef;
class GA_PointGroup;
class GA_EdgeGroup;
class GA_PrimitiveGroup;
class GA_AIFSharedStringTuple;
class GEO_Primitive;
class GR_DisplayOption;
class GR_CommonDispOption;
class gui_AttribInfo;
class gui_CollectionPrim;
class GR_Primitive;
class GR_SelectionAdapter;
class GT_RefineParms;
class OBJ_Node;
class gui_HookedPrims;
class gr_PrimEntry;
class GR_VisualizerInfo;
class gui_RenderHookEntry;
class GUI_ObjectMaterialCache;

using GUI_PrimTypeKey = std::pair<GA_PrimitiveTypeId, GT_PrimitiveType>;

class GUI_API GUI_GeoRender
{
public:
			GUI_GeoRender(GUI_DetailLook *look,
				      const char *name,
				      int geo_index,
				      int name_index);
    virtual	       ~GUI_GeoRender();

    virtual const char *className() const { return "GUI_GeoRender"; }

    virtual bool	isLopRender() const { return false; }
    
    void		setGeometry(OP_Node *node);
    OP_Node	       *getGeometry() const;

    // set the base geometry name for the cache name.
    void		setGeoName(const char *name);
    // set the cache name to exactly 'name'. Only used for instancing.
    void		setInstanceName(const char *name);

    const char	       *getCacheName() const { return myCacheName; }

    // the index of the detail within the GUI_DetailLook.
    void		setGeoIndex(int idx) { myGeoIndex = idx; }
    int			getGeoIndex() const { return myGeoIndex; }
    void		setNameIndex(int idx) { myNameIndex = idx; }
    int			getNameIndex() const { return myNameIndex; }

    // If this represents a node, this will return its node id.
    int			getNodeID() const { return myNodeID; }

    // The geometry this object will be rendering.
    void		setDetailHandle(const GU_ConstDetailHandle &handle);
    virtual GU_ConstDetailHandle &getDetailHandle() { return myDetailHandle; }

    // control the display of the GUI_GeoRender object. Those not displayed
    // cannot be rendered or picked.
    void		setDisplayed(bool is_disp) { myIsDisplayed = is_disp; }
    bool		isDisplayed() const { return myIsDisplayed; }

    // local transform of the object
    bool		setDetailTransform(const UT_DMatrix4 &xform);
    const UT_DMatrix4  &getDetailTransform(int index) const;
    const UT_DMatrix4  &getDetailInvTransform(int index) const;
    int			getNumDetailTransforms() const
			{ return myDetailTransforms.entries(); }
    void		setDetailFullTransform(const UT_DMatrix4 &xform)
			{ myDetailFullTransform = xform; }
    const UT_DMatrix4  &getDetailFullTransform() const
			{ return myDetailFullTransform; }

    // For an instance object, this is the instance object's transform. The
    // detail transforms are the individual point transforms.
    void		setInstancerTransform(const UT_DMatrix4 &xform)
			    { myInstancerTransform = xform;
				myInstancerIdentityFlag = xform.isIdentity(); }
    void		setInstancerTransformIdentity()
			    { myInstancerTransform.identity();
				myInstancerIdentityFlag = true; }
    const UT_DMatrix4  &getInstancerTransform() const
			    { return myInstancerTransform; }
    
    // set the local bounding box of this object
    void 		  setLocalBBox(const UT_BoundingBox &bbox,
				       int )
    			  { myLocalBBox = bbox; }
    const UT_BoundingBox& localBBox();

    // bounding box of this object
    virtual int		getBoundingBox2D(UT_BoundingBox &bbox,
				const char *name,
				int use_vertex) const;
    virtual int		getBoundingBox3D(UT_BoundingBox &bbox,
				const UT_Matrix3R *rot) const;

    // set the view * projection matrix
    static void		setWorldToScreen(const UT_Matrix4R &xform);

    // instanced transforms for the object, when point instancing
    void		setIsInstancedGeo(bool instanced);
    bool		addInstanceTransform(const UT_DMatrix4 &xform,
					     int geo_index);
    void		clearInstanceTransforms();
    void		setInstanceVersion(int64 version);
    int64		getInstanceVersion() const;

    // setup() called before any update or drawing is done, and cleanup() is
    // called when all rendering is complete for the current redraw.
    virtual GUI_SetupResult setup(const GUI_DisplayOption &disp_opt,
				  GUI_DisplaySetType state,
				  bool show_selection,
				  GR_DecorationOverride selmask_dec,
				  GR_DecorationOverride &select_dec,
				  bool &needs_alpha,
				  bool is_current);

    // minimal setup for multiple viewports (such as a quad config). One
    // viewport is called with setup(), the others fastSetup(). 
    virtual bool	fastSetup(const GUI_DisplayOption &disp_opt);
    
    virtual void	update(RE_Render *r,
			       GR_DisplayOption *opt,
			       GUI_SetupResult setup_reason,
			       GR_DecorationOverride selmask_dec,
			       const char *view_name,
			       bool &needs_alpha,
			       const GR_FrustumList &view_frustums);
    virtual void	cleanup(RE_Render *r);

    bool		isSetup() const { return mySetupFlag; }
    bool		needsAlphaPass() const { return myNeedsAlphaPassFlag; }

    // called when this look is no longer displayed and placed in a short cache
    // in case it is displayed again.
    void		retire();

    // Called when an object isn't displayed anymore, aka retire()-lite.
    void		hide();

    // Return true if gl_lit is set on the contained detail.
    bool		isLit() const { return myLitFlag; }

    // return true if this geo has guide rendering
    bool		hasGuideParts() const { return myHasGuideParts; }

    // Return if gl_wireframe is set to a positive value.
    bool		forceWireframe() const { return myWireframeFlag; }

    // Return if gl_wireframe is set to a negative value.
    bool		forceShaded() const { return myForceShadedFlag; }

    // The post-pass ID is used to setup and render post-render passes
    void		setPostPassID(int id) { myPostPassID = id ; }
    int			getPostPassID() const { return myPostPassID; }

    // An additional deferred pass is required (not a post pass).
    bool		needsDeferredPass() const { return myNeedsDeferredPass;}

    void		setDeferredPass(bool dpass) { myIsDeferPass = dpass; }
    
    // Returns the time in seconds of various operations 
    fpreal		getSetupTime() const { return fpreal(mySetupTime)*1e-9;}
    fpreal		getDrawTime() const  { return fpreal(myDrawTime)*1e-9;}

    virtual bool	isObjectRender() const { return false; }

    bool		hasMaterialReflection() const
			{ return myMaterialHasReflection; }

    void		setGhosted(bool ghost)
			{ myInfo.myObjectGhosted = ghost; }
    void		setObjectSelection(int obj_selection)
			{ myInfo.myObjectSelection = obj_selection; }

    bool		getComponentSelection(
				const GR_DisplayOption *opt,
				GR_SelectionAdapter &adapter) const;

    // DRAWING --------------------------------------------------------------
    
    // wire representation of all objects.
    virtual void	wireframeRender(RE_Render *r,
					GUI_DisplayOption &disp_opt,
					GR_RenderMode mode,
					GR_RenderFlags render_flags);
    
    // Beauty pass shaded render.
    virtual GUI_RenderPassFlags
			beautyRender(RE_Render *r,
				     GUI_DisplayOption &disp_opt,
				     GR_ShadingPass shade_pass,
				     GR_AlphaPass  alphapass,
				     GR_RenderMode mode,
				     GR_RenderFlags render_flags);
    
    static void		setupRender(RE_Render *r, GR_RenderMode mode);
    static void		cleanupRender(RE_Render *r, GR_RenderMode mode);
    
    // Various simple rendering, such as matte or depth.
    virtual void	utilityRender(RE_Render *r,
				      GUI_DisplayOption &disp_opt,
				      GR_RenderMode mode,
				      GR_RenderFlags rflags);

    // rendering for post-render passes that primitives request.
    virtual void	postPassRender(RE_Render *r,
				       GUI_DisplayOption &disp_opt,
				       GR_AlphaPass apass);

    // Object bounding box render
    virtual void 	bboxRender(RE_Render *r,
				   GUI_DisplayOption &disp_opt,
				   GR_RenderMode render_mode);

    // Render any highlights required
    void		renderMatteHighlights(RE_Render *r,
					      GUI_DisplayOption &disp_opt,
					      GR_RenderFlags render_flags);
    // Render onion skins
    void		onionSkinRender(RE_Render *r,
					GUI_DisplayOption &disp_opt);

    virtual int64	getPrimDecorationMask() const
			{ return myPrimDecorMask; }

    // Modelling aids, like normals or uv coord display.
    // This function is called before any specific decorations are rendered.
    virtual void	setupDecorationsRender(RE_Render *r);
    // This functions renders one type of decoration.
    virtual void	decorationRender(RE_Render *r,
				GUI_DisplayOption &disp_opt,
				GR_Decoration dec,
				GR_DecorVisibility vis,
				bool override_dispopt,
				GR_DecorRenderFlags render_flags);
    
    virtual void	customDecorationRender(RE_Render *r,
        		                        GUI_DisplayOption &disp_opts,
        		                        const GR_VisualizerInfo *dec,
						GR_DecorRenderFlags flags);

    // PICKING --------------------------------------------------------------

    // Return true if this object can be selected, and if 'node' isn't null,
    // assign to it the node that should be selected when this object is picked
    bool		canBeSelected(OP_Node *op,
				      GUI_DisplayOption &disp_opt,
				      bool pick_template,
				      bool pick_non_sel,
				      bool pick_ghost,
				      OBJ_Node **node = nullptr) const;

    // A version of canBeSelected() used when every displayed detail is treated
    // as an "object".
    bool		canBeSelectedWhenNotUsingOps(
				      GUI_DisplayOption &disp_opt,
				      bool pick_template,
				      bool pick_non_sel,
				      bool pick_ghost) const;
    
    // Full object pick - all or nothing. 
    virtual void	objectPick(RE_Render *r,
				GUI_DisplayOption &opts,
				GR_RenderFlags render_flags,
				const GR_ObjectPickFlags &pick_flags,
				bool force_wire);

    // Component pick, component type based on display option.
    virtual int		componentPick(RE_Render *r,
				GUI_DisplayOption &opts,
				unsigned int primmask,
				GEO_PrimTypeCompat::TypeMask primask,
				GR_PickStyle pickstyle,
				bool has_pick_map,
				UT_Array<GR_PickRecord> *pick_records);

    GUI_DetailLook     &look() const { return *myLook; }
    const char	       *name() const { return myCacheName; }

    // LIGHTING ------------------------------------------------------------
    
    void		setupLighting(RE_LightList *list, bool headlight);
    void		cleanupLighting();

    static void		resetStats();
    static void		getStats(int &num_objs,
				 int &num_polys,
				 int &num_vertices);
    
    static const RE_MaterialPtr	&getDefaultMaterial(bool lit);

    static void	 setDefaultMaterialParms(UT_Color diff,
					 UT_Color spec,
					 UT_Color amb,
					 UT_Color emit);

    void		bumpObjectMaterial() { myObjectMaterialDirty=true; }

    void		useObjectMaterials(bool use_obj_mat);
    bool		 getAssignedMaterials(UT_Array<OP_Node *> &nodes) const;

    int			getObjectMaterialID() const { return myObjMaterialID; }

    void		adoptAtlas(const RE_MaterialAtlas &from_atlas);

    bool 		isBboxShadingMode(const GR_DisplayOption *opt) const
			{
			    return opt->shadingMode()==GR_BOUNDING_BOX_WIRE
				|| opt->shadingMode()==GR_BOUNDING_BOX_SHADED;
			}

    bool 		isBboxShadingMode(GUI_DisplayOption &opt) const
			{
			    return isBboxShadingMode(getDisplayOptionSet(opt));
			}

    // Reflection map support, for this object
    bool		prepReflectionMap(RE_Render *r,
					  GUI_DisplayOption &opt,
					  int64 version,
					  fpreal time);
    void	        startReflectionPass(RE_Render *r, RE_TextureCubeFace f);
    void	        endReflectionPass(RE_Render *r);
    void		finishReflectionMap(RE_Render *r,
					    GUI_DisplayOption &opt);
    RE_Texture	       *getReflectionMap() const;

    // VISUALIZERS -----------------------------------------------------------
    
    exint		 getVisUpdateTime() const
			    { return myVisUpdateTime; }

    exint		 getVisRefineTime() const
			    { return myVisRefineTime; }

    void		 refreshVisUpdateTime()
			    { myVisUpdateTime =
				GUI_VisInterface::getHandler()->bumpClock(); }

    void		 refreshVisRefineTime()
			    { myVisRefineTime = myVisUpdateTime =
				GUI_VisInterface::getHandler()->bumpClock(); }

    bool		 hasUpdateVisualizer(int vis_id) const
			    { return myUpdateVisualizers.find(vis_id) >= 0; }

    bool		 hasRefineVisualizer(int vis_id) const
    			    { return myRefineVisualizers.find(vis_id) >= 0; }

    const UT_IntArray	&getActiveVisualizers() const
			    { return look().getActiveVisualizers(); }

    const UT_IntArray	&getUpdateVisualizers() const
			    { return myUpdateVisualizers; }

    const UT_IntArray	&getRefineVisualizers() const
			    { return myRefineVisualizers; }

    void		 refreshUpdateVisualizers()
			    { myUpdateVisualizers = getActiveVisualizers(); }

    void		 clearUpdateVisualizers()
			    { myUpdateVisualizers.clear(); }

    void		 refreshRefineVisualizers()
			    { myRefineVisualizers = getActiveVisualizers(); }

    void		 clearRefineVisualizers()
			    { myRefineVisualizers.clear(); }

    void		 runCreatorVisualizers(int &listcount,
				UT_Array<GT_PrimitiveHandle> &handles);

    GUI_VisFlags	 getActiveVisFlags(int prim_idx,
				GUI_VisContext context) const;

    // Look for an attribute in the given owner class (or all of them for
    // GT_OWNER_INVALID) in all cached GT_Primitives in the detail
    // (those used for updating GR_Primitives in the last update, which,
    // in particular, have been filtered by visualizes) and return
    // the first matching attribute if any.

    const
    GT_DataArrayHandle	 findCachedAttribute(const char *attrib_name,
				GT_Owner owner = GT_OWNER_INVALID) const;

    // Cached data used for display. Tied to the underlying GU_Detail.
    virtual const GEO_DetachedHedgeInterface	*getHedgeInterface(
							bool use_hidden_grp);
    virtual const GEO_PrimClassifier	*getPrimClassifier(GU_Connectivity con);
    virtual const GA_EdgeGroup		*getHiddenEdgeGroup();
    virtual const GA_ElementGroup	*getVisibleElementGroup(GA_GroupType t);

    static int		getPolygonMeshLimit();
    static int		getPointMeshLimit();

    static void		handleMaterialChange(OP_Node *src, void *dest,
					     OP_EventType type, void *data);

    bool		isMSSEnabled() const	    { return myMSSEnable; }
    bool		isMSSPackedEnabled() const  { return myMSSPackedGeo; }
    bool		isMSSPerPrimEnabled() const { return myMSSPerPrim; }
    bool		areOverridesEnabled() const
				{ return myMaterialUseOverrides; }
    int			getMaterialLimit() const { return myMaterialLimit; }
    void		setMaterialLimitExceeded(bool e)
				{ myMaterialLimitExceeded = e; }
    
    // clear all primitives and associated caching information
    void		invalidateGeo();

protected:
    virtual void	 updateGLSpecific(RE_Render *r,
				GR_DisplayOption *opts,
				GUI_SetupResult setup_reason,
				GR_DecorationOverride selmask_dec,
				const char *view_name,
				const GU_Detail *dtl)
			 {}
    
    virtual void	 cleanupMaterials() {}
    virtual bool	 shouldRender(const GU_Detail *geo) const;

    int			 getNumPrimitives() const
			    { return myPrimitives.entries(); }
    GR_Primitive	*getPrimitive(int index) const;

    GR_DisplayOption	*getDisplayOptionSet(GUI_DisplayOption &disp_opt)
			 { return &disp_opt[ look().getDisplaySetType() ]; }
     
    const GR_DisplayOption *getDisplayOptionSet(
				    const GUI_DisplayOption &disp_opt) const
			 { return &disp_opt[ look().getDisplaySetType() ]; }
    
    // Required Render functions -------------------------------------------
    
    // Wireframe only.
    virtual void	wireRender(RE_Render *r,
				GUI_DisplayOption &disp_opt,
				const GU_Detail *geo,
				GR_RenderMode mode,
				GR_RenderFlags flags);

    // Solid polygon render
    virtual void	solidRender(RE_Render *r, 
				GUI_DisplayOption &disp_opt,
				const GU_Detail *geo,
				GR_RenderMode mode,
				GR_RenderFlags flags,
				GR_AlphaPass pass) {};

    virtual void	simpleRender(RE_Render *r,
				GUI_DisplayOption &opt,
				const GU_Detail *geo,
				GR_RenderMode mode,
				GR_RenderFlags flags) {};

    virtual void 	simpleBboxRender(RE_Render *r,
				GUI_DisplayOption &opt,
				RE_Shader *shader=0,
				bool fill=true,
				bool outline=true) {};
    
    // Utility functions ---------------------------------------------------
    
    // Returns the decorations required for the selection types on this detail
    GR_DecorationOverride getSelectionDecorations(
				const GR_DisplayOption *opt) const;

    // Returns true if the geometry has a selection which requires a closure
    // color.
    bool		 isClosureSelection(GU_SelectionHandle sel, 
				bool &has_edges) const;
    bool		 isClosureSelection(GR_SelectionAdapter &sel,
				bool &has_edges) const;

    // returns true if the buffer was filled because the attribute existed in
    // the detail, or false if a constant attribute was created (which must be
    // filled in)
    enum FetchBufResult
    {
	FETCH_NO_GEO_ATTRIB,
	FETCH_NO_SHADER_ATTRIB,
	FETCH_CACHED,
	FETCH_FILLED,
	FETCH_ERROR
    };

    void	prepMaterials(RE_Render *r, 
			      const GR_DisplayOption *opts);
    
    // Setup wire selection uniforms on 'r', and modify 'flags' to include
    // GR_RENDER_FLAG_WIRE_OVER if needed.
    void	setupWireSelectParms(RE_Render *r,
				     GUI_DisplayOption &disp_opts,
				     bool is_wireframe,
				     GR_RenderFlags &flags);
    void	cleanupWireSelectParms(RE_Render *r);
    
    // Sets the object matrix uniform based on our instancer transform,
    // detail transform, etc.
    void	setObjectMatrixUniform(RE_Render *r);

    void	updatePrimitives(RE_Render *r,
				 GR_DisplayOption *opts,
				 UT_Array<GT_PrimitiveHandle> &hlist,
				 GT_RefineParms &refine_opts,
				 GR_UpdateReason reason,
				 GR_DecorationOverride req_dec,
				 const char *view_name,
				 const GR_FrustumList &view_frustums);
    void	processPrimitives(const GU_ConstDetailHandle &geo,
				  GT_PrimitiveHandle &ph,
				  GT_RefineParms &refparms,
				  int &listcount,
				  const GR_DisplayOption &dispopt,
				  GR_UpdateReason reason,
				  const char *cache_name,
				  UT_Array<GT_PrimitiveHandle> &handles);
    bool	processPrim(const GU_ConstDetailHandle &geo,
			    GT_PrimitiveHandle &ph,
			    GT_RefineParms &refparms,
			    int &listcount,
			    const GR_DisplayOption &dispopt,
			    GR_UpdateReason reason,
			    const char *cache_name,
			    UT_Array<GT_PrimitiveHandle> &handles,
			    bool apply_filter_vis,
			    int &packed_index);

    void	processAlembicArchive(GT_PrimitiveHandle &ph,
				      const GU_ConstDetailHandle &gdh,
				      GT_RefineParms &refparms,
				      int &listcount,
				      const GR_DisplayOption &dispopt,
				      GR_UpdateReason reason,
				      const char *cache_name,
				      UT_Array<GT_PrimitiveHandle> &handles,
				      exint mat_id);
    void	clearUnusedAlembicArchives();

    void	renderMatteGroupColors(RE_Render *r,
				       GUI_GroupDataHandle group_data,
				       GR_DisplayOption *opt,
				       GR_RenderFlags render_flags);

    // Lower level render function used by beautyRender, wireRender, etc.
    void	renderPrims(RE_Render *r,
			    GR_DisplayOption *opt,
			    GR_RenderMode mode,
			    GR_RenderFlags flags,
			    GR_AlphaPass alphapass,
			    bool use_materials);
    
    bool	createOrReuseRefinedPrim(GT_PrimitiveHandle &ph,
					 const GU_ConstDetailHandle &gdh,
					 const GEO_Primitive *geo_prim,
					 int &listcount,
					 const GR_DisplayOption &disp,
					 GR_UpdateReason reason,
					 const GT_RefineParms &ref_parms,
					 const char *cache_name,
					 UT_Array<GT_PrimitiveHandle> &handles,
					 bool processing_geo_prims,
					 bool apply_filter_vis,
					 int packed_index,
					 int64 mat_id);
    GR_PrimAcceptResult checkForExistingPrim(GT_PrimitiveHandle &ph,
					     GR_Primitive *prim,
					     int	   pindex,
					     GR_UpdateReason	 reason,
					     const GT_RefineParms &ref_parms,
					     const GU_ConstDetailHandle &gdh,
					     const GEO_Primitive *geo_prim,
					     int	  &listcount,
					     UT_Array<GT_PrimitiveHandle> &hdl,
					     bool	  processing_geo_prims,
					     int	  packed_index);
    void	accumulateTime(RE_Render *r);
    
    void	buildEdgeHighlight(RE_Render *r,
				   const GU_Detail *geo,
				   GR_UpdateReason reason,
				   const GR_DisplayOption *opt,
				   const char *view_name);

    bool		 buildInstanceSelection(bool has_sel_overrides);
    fpreal		 getPointInstanceFraction(GR_DisplayOption *opts) const;
    UT_Matrix4DArray	*getPointInstanceTransforms(GR_DisplayOption *opts);

    RE_CacheVersion	 getGeoVersion(const GU_Detail *geo,
				       const GR_DisplayOption *opt);
    RE_CacheVersion	 getSelectionVersion(const GU_Detail *geo,
					     const GR_DisplayOption *opt);
    RE_CacheVersion	 getEdgeHighlightVersion(const GU_Detail *geo,
						 const GR_DisplayOption *opt,
						 bool include_selection);
        
    void		setSelection(int64 id, int64 version)
			  { mySelectionId = id; mySelectionRevision = version; }

    GR_PrimAcceptResult  processHookPrimitive(GT_PrimitiveHandle &gt_prim,
					      const GEO_Primitive *geo_prim,
					      const GU_ConstDetailHandle &gdh,
					      GR_UpdateReason	 reason,
					      const GT_RefineParms &ref_parms,
					      const char *cache_name,
					      GR_Primitive *&created,
					      int &list_index,
					      int &hook_priority,
					      bool &filtered_prim,
					      gui_RenderHookEntry *&prev,
					      UT_Array<GT_PrimitiveHandle>&hdls,
					      bool	  processing_geo_prims,
					      int		  packed_index);
    
    void		 processFilterHooks(GT_PrimitiveHandle  &gt_prim,
					    const GEO_Primitive *geo_prim);

    void		 restartCollectionPrimitives();
    void		 removeUnusedCollectionPrimitives();

    void		 find3DBoundaries(const GU_Detail *geo,
					  const GA_PrimitiveGroup *within_prims,
					  GA_EdgeGroup *boundary_group);
    void		 findUVBoundaries(const GU_Detail *geo,
					  const GA_PrimitiveGroup *within_prims,
					  GA_EdgeGroup * uv_boundary_group,
					  const GA_ROHandleV2 &uv,
					  bool vert_uvs);
    
    class gui_BoundaryData
    {
    public:
	gui_BoundaryData()  : myUsed(false), myAttribVersion(-1)   {}
	
	int64			   myAttribVersion;
	UT_SharedPtr<GA_EdgeGroup> myEdges;
	UT_SharedPtr<RE_Geometry>  myGeo;
	UT_SharedPtr<RE_Geometry>  myGeoV;
	UT_SharedPtr<RE_Geometry>  my2DGeo;
	UT_SharedPtr<RE_Geometry>  my2DGeoV;
	bool			   myUsed;
    };

    bool		matchesVersion(gui_BoundaryData &bdata,
				       const UT_StringHolder &attrib_name,
				       const RE_CacheVersion &ver,
				       bool view_3d, bool vert_uv);
    void		 createEdgeGeo(RE_Render *r,
				       gui_BoundaryData &bdata,
				       const GA_EdgeGroup *edges,
				       bool is_3d,
				       bool is_vertex,
				       const UT_StringHolder &attrib_name,
				       const UT_StringHolder &cache_suffix,
				       const GU_Detail *geo,
				       const RE_CacheVersion &ev,
				       GA_ROHandleV2 *uvhandle = nullptr);
    
    GR_RenderInfo	&getRenderInfo()
			 { return myInfo; }

    void		getRefineParms(RE_Render *r,
				       const GR_DisplayOption *opts,
				       GT_RefineParms &p) const;

    void		 resizePrimArray(exint nprims, bool clear = false);
    void		 assignPrim(exint index,
				    GR_Primitive *prim,
				    const GT_PrimitiveHandle &gtprim);
    void		 updateWorldTransform();

    static UT_Lock	 theShaderLock;
    static RE_MaterialPtr theDefaultMaterial[2];

    static int		 theNumObjects;
    static int		 theNumPolys;
    static int		 theNumVertices;

    const GU_Detail	*myLockedDetail;
    RE_MaterialAtlas	 myMaterialAtlas;
    GUI_ObjectMaterialCache *myObjectMaterialCache;
    UT_Map<UT_StringRef,int> myAttributes;

    int64		 myEdgePrimsVersion;
    UT_SharedPtr<GA_PrimitiveGroup> myEdgePrims;

    gui_BoundaryData	 my3DBoundary;
    UT_StringMap<gui_BoundaryData> myUVBoundary;
    UT_StringMap<bool>	 myUVAttribIsVertex;
    gui_BoundaryData	 myEdgeSelection;
    bool		 myHasEdgeSelection;
    int64		 myUVAttribSerial;
    
    int64		 myInstanceVersion;
    int64		 myLimitInstanceVersion;
    int			 myViewSerial;
    int			 myViewID;

    RE_LightList	*myLightList;
    bool		 myUseHeadlight;
    int64		 myPrimDecorMask;

private:
    bool		shouldUVRender(const GU_Detail *geo,
				       const GUI_DisplayOption &disp,
				       bool is_vertex) const;
    bool		shouldRender(const GU_Detail *geo,
				     const GUI_DisplayOption &disp,
				     GR_RenderFlags render_flags) const;
    bool		shouldRender(const GU_Detail *geo,
				     const GUI_DisplayOption &disp,
				     GR_DecorRenderFlags decor_flags) const;
    bool		shouldRender(const GU_Detail *geo,
				     const GUI_DisplayOption &disp,
				     GR_PickStyle pick_style) const;
    
    bool		requiresStateChange(const GUI_DisplayOption &disp_opt,
					    GUI_DisplaySetType newset,
					    GUI_DisplaySetType oldset) const;
    
    GR_UpdateReason	checkForShaderUpdates(const GU_Detail *geo);
    bool		prepObjectMaterial(RE_Render *r,
					   const GR_DisplayOption &opts);
    bool		checkAutoVertexNormal(const GU_Detail *dtl,
					      const GR_CommonDispOption &opts);

    bool		inPackedPrim() const
			    { return myPackedPrimLevel > 0; }
    bool		inInstancePrim() const
			    { return myInstancePrim.entries() > 0; }
    void		updateInstanceEntry(gr_PrimEntry *entry,
					    int prim_index);

    bool		processInstancedPrims(const GT_PrimitiveHandle &ph,
					      int &listcount,
					      UT_Array<GT_PrimitiveHandle> &hdl,
					      int64 geo_id, int64 geo_count,
					      int packed_index,
					      int64 mat_id);

    // instancing
    void		buildPointInstanceArray(RE_Render *r,
						GR_DisplayOption *dopts,
						const GU_Detail *dtl,
						bool use_fraction_parm);
    void		pointInstanceGuideRender(RE_Render *r,
						 GUI_DisplayOption &disp_opt);
    void		renderEdgeHighlight(RE_Render *r,
					    const GR_DisplayOption *op,
					    GR_RenderFlags render_flags);
    void	 	renderBBox(RE_Render *r,
				   GUI_DisplayOption &opt,
				   RE_Shader *shader=0,
				   bool fill=true,
				   bool outline=true);
    OP_Node *		getObjectMaterial(fpreal t) const;
    void		clearMaterialInterests();
    
    UT_Array<gr_PrimEntry *>	 myPrimitives;
    UT_IntArray			 myPrimitiveIndex;
    
    GUI_GroupDataHandle		 myGroupData;

    UT_Array<gui_HookedPrims *>	 myHookedGEOPrims;

    bool			 myObjectMaterialDirty;
    OBJ_Node			*myTopObject;
    int				 myNodeID;
    bool			 myUseObjectMaterial;
	
    GR_RenderInfo		 myInfo;
    GUI_DetailLook		*myLook;
    int				 myGeoIndex;
    int				 myNameIndex;
    UT_String			 myCacheName;
    bool			 mySetupFlag;
    bool			 myHasGLStateCheckPrims;
    bool			 myLitFlag;
    bool			 myHasGuideParts;
    bool			 myHasPackedPrims;
    bool			 myHasPackedFragments;
    bool			 myHasPackedInstances;
    bool			 myWireframeFlag;
    bool			 myForceShadedFlag;
    bool			 myRecordingTime;
    bool			 myIsDisplayed;
    
    bool			 myHasAgentPrims;
    bool			 myAgentLastLODEnable;
    int				 myAgentLastMinPoints;
    int				 myAgentLastMaxLOD;
    
    
    RE_ContextSpecificTable<RE_TimerQuery> myDrawStartTime;
    RE_ContextSpecificTable<RE_TimerQuery> myDrawEndTime;
    int				 myDrawTimeWaiting;
    int64			 myDrawTime;
    int64			 mySetupTime;

    int				 myDetailID;
    int64			 myVersion;
    int64			 myTopologyVersion;
    int				 myCachedShowSelect;
    int64			 myPrimitiveVersion;
    int64			 myPosVersion;
    exint			 mySelectionId;
    exint			 mySelectionRevision;
    int				 myPostPassID;
    bool			 myNeedsDeferredPass;

    UT_String			 myDopnetPath;
    
    // For determining if a display option which affects geometry has changed.
    int64			 myDispOptID;
    int				 myDispOptSerial;
    fpreal			 myLOD;
    bool			 myPolygonConvexQuality;
    bool			 myVertexNormalEnable;
    fpreal			 myVertexNormalCuspAngle;
    GR_DecorationOverride	 myRequiredDecorations;
    bool			 myShowAllPoints;
    fpreal			 myDetailLOD;

    GUI_RenderMask		 myRenderMask;
    GUI_DisplaySetType		 myObjectState;
    GEO_ViewportLOD		 myObjectLOD;
    bool			 myObjectDisplayAsSubdiv;

    GU_ConstDetailHandle	 myDetailHandle;
    UT_Matrix4DArray		 myDetailTransforms;
    UT_Matrix4DArray		 myDetailInvTransforms;
    UT_Matrix4DArray		 myReducedDetailTransforms;
    bool			 myDetailTransformIdentity;
    UT_Matrix4D			 myInstancerTransform;
    UT_Matrix4D			 myDetailFullTransform;
    bool			 myInstancerIdentityFlag;
    UT_IntArray			 myDetailGeoIndex;
    UT_BoundingBox 		 myLocalBBox;
    int64			 myCachedInstanceVersion;
    int				 myRefineVersion;
    bool			 myCheckUpdateViewFlag;
    bool			 myObjectTransformDirty;
    bool			 myNeedsAlphaPassFlag;
    bool			 myPrimAlphaPassFlag;
    bool			 myIsDeferPass;
    bool			 myRefineOnLODChange;
    int				 mySelectionInstance;
    fpreal			 myLimitedInstanceFraction;
    bool			 myShowMaterials;

    RE_CacheVersion		 myInstanceSelectVersion;
    UT_BitArray			 myInstanceSelection;
    bool			 myHasInstanceSelection;

    // for GT_PrimInstance types, this holds the array of instance transforms
    // while its geometry is being refined.
    class gui_Instance
    {
    public:
	gui_Instance() { myBBox.makeInvalid(); }
	
	UT_Matrix4DArray	 myTransforms;
	GU_ConstDetailHandle	 myDetail;
	GT_PrimitiveHandle	 myPrim;
	UT_BoundingBox		 myBBox;
    };
    UT_Array<gui_Instance>	 myInstancePrim;
    GT_AttributeListHandle	 myInstancePrimAttribs;
    GT_AttributeListHandle	 myInstancePointAttribs;
    
    class gui_DiskFile
    {
    public:
	gui_DiskFile() : myPrimIndex(0) {}
	UT_StringHolder myDiskFile;
	int		myPrimIndex;
    };
    UT_Array<gui_DiskFile>	 myPackedDiskStack;
    
    int				 myPackedPrimLevel;
    GT_PrimitiveHandle		 myPackedPrim;
    mutable bool		 myCachedUseSampleMask;
    mutable int			 myCachedSampleMask;
    mutable bool		 myCachedMultisample;
    mutable bool		 myCachedSampleCoverage;
    
    RE_Geometry			*myPointGeo;
    RE_Geometry			*myCubeGeo;
    RE_VertexArray		*myCubeInstance;
    fpreal			 myCubeInstanceFraction;

    RE_ReflectMap		*myReflectMap;

    // last update and refinement time in visualization clock (counter)
    exint		 	 myVisUpdateTime;
    exint 		 	 myVisRefineTime;

    UT_IntArray			 myUpdateVisualizers;
    UT_IntArray			 myRefineVisualizers;

    // if set to true in setup, updates are upgraded to full refinements.
    bool			 myRefineOnUpdate;
    unsigned			 myMaterialTextureFlag :1,
				 myMaterialHasDisplaceMap :1,
				 myMaterialHasReflection :1,
				 myMaterialUseOverrides :1,
				 myMSSEnable :1,
				 myMSSPackedGeo :1,
				 myMSSPerPrim :1;
    GUI_DisplayOption::gui_MatAssignMode myMatAssignMode;
    int				 myMaterialLimit;
    bool			 myMaterialLimitExceeded;
    int				 myObjMaterialID;

    GT_DataArrayHandle		 myBaseMaterialAssignments;
    GT_DataArrayHandle		 myBaseMaterialRemap;
    int64			 myBaseMaterialPrevID;
    int64			 myBaseMaterialRemapPrevID;
    bool			 myDefaultMaterialLazyMode;
    bool			 myForceMaterialUpdate;
    bool			 myMaterialAllowUpdates;

    class gui_DetailAttribLists
    {
    public:
	bool	checkForAttribListChanges(const GU_Detail &dtl);

	UT_StringMap<bool> myPoint;
	UT_StringMap<bool> myVertex;
	UT_StringMap<bool> myPrim;
	UT_StringMap<bool> myDetail;

	UT_StringMap<bool> myVisAttribs;
    };

    gui_DetailAttribLists			 myDetailAttribLists;
    struct
    {
	UT_UniquePtr<GEO_DetachedHedgeInterface> interface;
	int					 detail_id;
	int64					 cache_version;
	int64					 top_version;
    } myHedgeInterface[2];

    UT_UniquePtr<GEO_PrimClassifier>		 myPrimClassifier;
    int						 myPrimClassifierDetailId;
    int64					 myPrimClassifierCacheVersion;
    int64					 myPrimClassifierTopVersion;
    int64					 myPrimClassifierSelectId;
    int64					 myPrimClassifierSelectRevision;
    GU_Connectivity				 myPrimClassifierConnectivity;

    UT_UniquePtr<GA_Group>			 myCachedGroup;
    GA_GroupType				 myCachedGroupType;
    int						 myCachedGroupDetailId;
    int64					 myCachedGroupCacheVersion;
    int64					 myCachedGroupTopVersion;
   
    static RE_ShaderHandle			 theBboxShader;
    static RE_ShaderHandle			 theObjectPickShader;
    static RE_ShaderHandle			 theEdgeHighlightShader;
    static RE_ShaderHandle			 theEdgeHighlightUVShader;

    UT_StringMap<GT_PrimitiveHandle>		 myAlembicArchives; 
    UT_StringMap<int>				 myUsedAlembicArchives;
    
    UT_StringHolder				 myUVDisplay;
public:
    // treat as private. For DM_RenderTable only.
    static bool registerHook(GUI_PrimitiveHook     *hook,
			     GT_PrimitiveType	    gt_type,
			     int		    geo_type,
			     int		    priority,
			     GUI_PrimitiveHookFlags flags);

    friend class gr_PrimEntry;
};

inline void
GUI_GeoRender::setInstanceVersion(int64 version)
{
    myInstanceVersion = version;
}

inline int64
GUI_GeoRender::getInstanceVersion() const
{
    return myInstanceVersion;
}

inline const UT_DMatrix4 &
GUI_GeoRender::getDetailTransform(int index) const
{
    return myDetailTransforms(index);
}

inline const UT_DMatrix4 &
GUI_GeoRender::getDetailInvTransform(int index) const
{
    return myDetailInvTransforms(index);
}

#endif
