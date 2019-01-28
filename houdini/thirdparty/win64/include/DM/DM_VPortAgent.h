/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DM_VPortAgent.h ( DM Library, C++)
 *
 * COMMENTS:
 * 	DM_VPortAgent is the class for the 2D & 3D viewport agents.
 * 	Viewport agents encapsulate the viewport functionality that is 
 * 	specific to a particular kind of viewport, such as
 * 	rendering. There is one agent for each viewport.
 */

#ifndef __DM_VPortAgent__
#define __DM_VPortAgent__

#include "DM_API.h"
#include "DM_Defines.h"
#include "DM_GeoDetail.h"
#include "DM_ViewportType.h"

#include <UI/UI_Object.h>
#include <OP/OP_Bundle.h>
#include <GEO/GEO_PrimTypeCompat.h>
#include <GR/GR_Defines.h>
#include <GUI/GUI_RenderCommon.h>
#include <GUI/GUI_ViewParameter.h>
#include <GUI/GUI_ViewState.h>
#include <RE/RE_Texture.h>
#include <RE/RE_LightList.h>
#include <RE/RE_TimerQuery.h>
#include <RE/RE_PickBuffer.h>
#include <RE/RE_OGLFramebuffer.h>
#include <PXL/PXL_Lookup.h>
#include <UT/UT_UniquePtr.h>

class RE_RenderWait;
class UI_Event;
class OBJ_Ambient;
class GR_PickRecord;
class GUI_DisplayOption;
class GUI_GeoRender;
class GUI_SceneLook;
class GUI_ViewState;
class DM_FloorInterface;
class DM_RenderHighQuality;
class DM_SceneManager;
class DM_Viewport;
class DM_ViewportHooks;

class DM_API DM_VPortAgent
{
public:
		 DM_VPortAgent(DM_SceneManager &app,
			       DM_Viewport *display,
			       GUI_ViewState *view,
			       DM_ViewportType type=DM_VIEWPORT_PERSPECTIVE);
    virtual	~DM_VPortAgent();

    /// Each viewport has a unique integer id used to differentiate them.
    int		 getUniqueId() const;
    
    /// Compute bounding box of the scene.
    int		 getBoundingBox(UT_BoundingBox &bbox,
				const UT_Matrix3R *rot,
				bool non_templated_only,
                                const DM_Viewport &viewport) const;

    /// Called by DM_Viewport to initialize and retire the viewport agent
    /// when opening and closing. Treat as private.
    void	 setupViewport();
    void	 cleanupViewport();

    void	 refresh() const;

    /// The projection type of this viewport
    DM_ViewportType getViewType() const { return myType; }

    /// For ortho viewports, the projection type (top, front, right, etc)
    GUI_ViewParameter::axis getHomeAxis() const;
    
    /// Projection type, ortho or perspective
    int			 isOrtho() const;

    /// The view state contains transform, projection and display options
    const GUI_ViewState	&getViewStateRef() const;
    GUI_ViewState       &getViewStateRef();

    /// Given a look id, and possibly a GUI_DetailList, find the GUI_DetailLook
    /// with the supplied id.
    GUI_DetailLook	*getDetailLook(int look_id, const GUI_DetailList *geos);
    
    const GUI_DetailLook *getDetailLook(int node_id) const;

    /// Fetch the detail of the current SOP or POP
    const GUI_DetailLook*currentDetail() const;
    GUI_DetailLook	*currentDetail();

    /// turn on/off the background elements
    void		 drawBackground(bool draw);

    /// Called when loading a new hip file or File>New
    void		 resetForNew();

    /// handle a change in our display options
    void		 handleDisplayOptionEvent(UI_Event *e);

    /// handle a change in our view parameters
    void		 handleViewParameterEvent(UI_Event *e);

    /// render all elements in this viewport
    void		 doRender(RE_Render *r, int  x, int  y);

    /// render any transient overlay elements in the viewport
    void		 overlayRender(RE_Render *r, int x, int y);

    /// render all handles present in the viewport
    void		 renderHandles(RE_Render *r, int x, int y,
				unsigned mask);

    /// Change the type of the viewport to a different projection or 2D/3D
    void		 changeType(UI_Object *by, DM_ViewportType new_type);

    /// Called when the viewport closes, but is not deleted (such as a pane tab
    /// switch).
    void		 close();

    /// @brief Access the beauty pass textures
    /// Get a texture containing the color or depth of the beauty pass buffer.
    /// This texture is always single-sampled.
    RE_Texture		*getBeautyPassTexture(RE_Render *r,
				bool resolve_aa = true) const;
    RE_Texture		*getBeautyPassDepthTexture(RE_Render *r,
				bool resolve_aa = true) const;

    /// copy the beauty pass to 'dst_texture'. 
    bool		 copyBeautyToTexture(RE_Render *r,
					     UT_DimRect src_area,
					     RE_Texture *dst_texture,
					     int dst_layer) const;
    
     /// Returns the LUT for the beauty pass, if any.
    PXL_Lookup		*getBeautyLUT();
    TIL_Raster		*getBeautyPassImage(RE_Render *r);
    TIL_Raster		*getBeautyPassDepthImage(RE_Render *r);
    bool		 getBeautyPassHDR() const;
    void		 setBeautyPassConsolidate(bool con);

    /// ensure all geometry is up to date before a redraw, and cleanup after
    void		 preSetupGeometry(RE_Render *r);
    void		 postCleanupGeometry(RE_Render *r);
    void		 setupGeometry(RE_Render *r);
    void		 cleanupGeometry(RE_Render *r);
    void		 fastSetupGeometry(RE_Render *r);

    /// returns a single pick result from the x,y mouse coords. The pick buffer
    /// must be rendered first. Returns false if nothing was picked.
    bool	 singlePickObject(RE_Render *r, int x, int y,
				  GR_PickRecord &result,
				  const GR_ObjectPickFlags &pick_flags,
				  fpreal *depth,
				  const GUI_DetailList *geos);
    
    /// Check for a pick on a handle. Returns true if a handle was picked.
    bool	 singlePickHandle(RE_Render *r, int x, int y,
				  GR_PickRecord &result,
				  fpreal *depth);

    /// @brief Area select objects, possibly with a matte. 
    /// returns a list of object IDs for those objects picked. This does not
    /// pick handles or SOP geometry. The return value is the length of the pick
    /// list.
    int		 boxPickObject(RE_Render *r,
			       const UT_DimRect &area,
			       uint8 *texture,
			       UT_Array<GR_PickRecord> &pick_records,
			       bool visible_only,
			       bool wholy_contained,
			       const GUI_DetailList *geos);

    /// @brief Single select a geomerty element.
    /// Returns a single pick result for components in objects 'geos'.
    bool	 singlePickComponent(RE_Render *r,
				     int x, int y,
				     GR_PickRecord &result,
				     unsigned pickmask,
				     GEO_PrimTypeCompat::TypeMask pmask,
				     fpreal *depth,
				     const GUI_DetailList *geos,
				     bool enable_occlusion);
    /// Box, lasso or paint select components.
    int		 boxPickComponent(RE_Render *r,
				  const UT_DimRect &area,
				  uint8 *texture,
				  UT_Array<GR_PickRecord> &pick_records,
				  bool sort_records,
				  unsigned pickmask,
				  GEO_PrimTypeCompat::TypeMask pmask,
				  bool visible_only,
				  bool contained,
				  bool interactive,
				  const GUI_DetailList *geos);
    int		 componentPickRender(RE_Render *r,
				     unsigned int pickmask,
				     unsigned int xray_pickmask,
				     GEO_PrimTypeCompat::TypeMask pmask,
				     const GUI_DetailList *geos,
				     GR_PickStyle pickstyle,
				     bool invert_mask,
				     UT_Array<GR_PickRecord> *pick_rec);

    /// @brief Single select a geometry element to snap to.
    /// Returns a single snap result for components in objects 'geos'.
    bool	 singleSnapComponent(RE_Render *r,
				     int x, int y,
				     bool hide_selected, bool enable_occlusion,
				     bool do_xray, bool extend_floor,
				     GR_PickRecord &result,
				     UT_Array<GR_PickRecord> &near_results,
				     unsigned pickmask,
				     GEO_PrimTypeCompat::TypeMask pmask,
				     const GUI_DetailList *geos=NULL);

    /// @brief Single select a peanut element to snap to.
    /// Returns a single snap result for peanuts.
    bool	 singleSnapPeanut(RE_Render *r,
				     int x, int y,
				     bool hide_selected, bool enable_occlusion,
				     bool do_xray, bool extend_floor,
				     GR_PickRecord &result,
				     unsigned pickmask,
				     GEO_PrimTypeCompat::TypeMask pmask,
				     const GUI_DetailList *geos=NULL);

    /// Call to manually invalidate any cached pick/snap buffers when the
    /// automatic detection is insufficient.
    void		 invalidatePickBuffers();

    /// Box, lasso or paint select components.
    /// Render all geometry in the viewport using render mode 'render_type'.
    GUI_RenderPassFlags	 renderGeometry(RE_Render *r,
				GR_RenderMode render_type,
				GR_ShadingPass shade_pass,
				GR_AlphaPass  apass,
				OP_Bundle *visible_objs = NULL,
				DM_GeoSortMode sort=DM_GEO_UNSORTED);

    /// @brief Draw geometry decorations and markers
    /// Render all decorations. If 'hover_decorations' is true, only the
    /// locate-style decorations will be drawn.
    void		 renderDecorations(RE_Render *r,
				GUI_DisplayOption *dispopt,
				bool hover_decorations);
    
    /// Render geometry corresponding to the OP_Nodes is the passed list.
    GUI_RenderPassFlags	 renderSomeGeometry(RE_Render *r, int x, int y,
				const UT_Array<OP_Node*> &nodes,
				const UT_IntArray *detail_indexes,
				GR_RenderMode render_type,
				GR_ShadingPass shade_pass,
				GR_AlphaPass apass,
				bool do_geometry_setup);

    /// Render a single geometry using render mode 'render_type'.
    GUI_RenderPassFlags	 renderSingleGeo(RE_Render *r,
				GUI_GeoRender *gr,
				GR_RenderMode render_type,
				GR_ShadingPass shading_pass,
				GR_AlphaPass ap,
				bool init_lights,
				bool treat_uv_viewport_as_3d = false);
    
    /// Our floor or construction plane.
    void		 renderFloor(RE_Render *r, int x, int y);
    /// Render construction plane in overlayRender.
    void		 overlayRenderFloor(RE_Render *r, int x, int y);

    DM_FloorInterface	*getFloorInterface() { return myFloorInterface; }

    /// Interface to viewport hooks (render and mouse)
    DM_ViewportHooks	&hooks() const { return *myViewportHooks; }

    bool		 needsOverlayRender() const
			 { return myNeedsOverlayRender; }

    /// Methods to process mouse movement
    /// @{
    bool		 hasLocates();
    void		 handleLocateEvent(UI_Event *e);
    /// @}

    /// @brief Access displayed objects in the viewport
    /// Return a handle to an object displayed in the viewport. Do not cache
    /// these over several redraws, as they may become invalid. get*Object()'s
    /// index parameter can be 0 to getNum*Objects()-1. 
    /// @{
    int			 getNumOpaqueObjects() const;
    DM_GeoDetail	 getOpaqueObject(int index) const;

    int			 getNumTransparentObjects() const;
    DM_GeoDetail	 getTransparentObject(int index) const;
    
    int			 getNumUnlitObjects() const;
    DM_GeoDetail	 getUnlitObject(int index) const;

    int			 getNumXRayObjects() const;
    DM_GeoDetail	 getXRayObject(int index) const;
    /// @}

    /// Returns the current detail within the object, when at SOP/POP/DOP level.
    DM_GeoDetail	 getCurrentDetail() const;

    void		 initGPUTimers(RE_Render *r);
    void		 collectGPUTimers(RE_Render *r);

    GUI_RotoView	 getRotoViewType();

    void		 markGeoSetup(bool setup,
				      DM_Viewport *setup_port = NULL);
    bool		 isGeoSetup() const { return mySetupFlag; }

    // All lights in the scene, for the material render codepath.
    void		 setGlobalLightArray(const UT_Array<OBJ_Ambient *> &l);
    void		 setGlobalLightArray(const UT_ValArray<RE_Light*> &l);

    // Get and set lighting information.
    bool		 setupLights(RE_Render *r, GUI_DisplayOption *opt);
    void		 cleanupLights(RE_Render *r);
    RE_LightList	*getLightList() { return &myLightList; }

    const UT_ValArray<RE_Light*> &getAllLights() const
				{ return myMaterialLights; }

    GR_LightingPass	 getLightingPass() const { return myLightingPass; }
    void		 setLightingPass(GR_LightingPass p) { myLightingPass=p;}

    // Returns the approximate memory usage of the viewport at the current size
    // given hdr on/off, high quality lighting on/off, transparency > low,
    // and aa at 1,2,4 or 8x.
    int64		 getFramebufferUsageKB(bool hdr, bool hql, bool trans,
					       int aa);

    // using zt[0] / (Z + zt[1]) will transform zbuffer values into viewspace Z
    // If light is specified, then the shadowmap range of that light is computed
    void		 getZBufferTransform(float zt[2],RE_Light *light=NULL);

    // Handle a change in which geometry is displayed.
    // For keeping track of what type of UVs to show in UV mode.
    void		 handleNewGeometryEvent(UI_Event *e);
    void		 updateAttributeOptions(const GU_DetailHandle &gdh);

    // ---------------------------------------------------------------------
    // the following methods should be treated as private to DM.
    
    /// Methods for starting and stopping events
    /// in the old performance monitor.
    void	 startPerfMonTimer(RE_Render *r,
				   RE_GPUTimer &timer,
				   const char *msg) const;
    void	 stopPerfMonTimer(RE_Render *r,
				  RE_GPUTimer &timer) const;
    
    // setup proper winding and culling, returning a set of flags to use for
    // the cleanup method.
    int		 setupBackfaceAndWinding(RE_Render *r,
					 const GUI_DisplayOption *opts);
    void	 cleanupBackfaceAndWinding(RE_Render *r,
					   int setup_flags);
    
    // resolve the AA Z buffer to a single sample, taking the further sample
    bool	 resolveDepthBuffer(RE_Render *r,
				    RE_Texture *aa_depth_tex,
				    RE_Texture *depth_tex) const;

    int64	 getSceneVersion(fpreal t, bool shadow, bool &timedep);

    // access to parent viewport
    const DM_Viewport	*getDisplay() const { return myDisplay; }
    DM_Viewport 	*getDisplay() { return myDisplay; }

    // for transparency
    int			 getTransparencyLayers(const GUI_DisplayOption *disp);
    void 		 prepareStencilRouting(RE_Render *r, int nsamples);
    void 		 prepareABufferFront(RE_Render *r, int nsamples);

    void		 updateTimeDependentLook(fpreal t);
    
    bool		 isToolbarLinked() const; // see .C file note

    const GUI_DetailList &getDetails() const;
    const GUI_DetailList &getDetails();

    void		 setupViewUniforms(RE_Render *r);
    void		 cleanupViewUniforms(RE_Render *r);

    // Looks through the lights for an environment map
    void		 applyEnvMapOverride(RE_Render *r) const;
    
protected:
    friend class dm_SetupTask;
    
private:
    
    const DM_SceneManager &modeler() const
			{ return mySceneManager; }
    DM_SceneManager	&modeler()
			{ return mySceneManager; }
    
    const GUI_SceneLook	*getLook() const { return mySceneLook; }
    GUI_SceneLook	*getLook() { return mySceneLook; }

    bool	shouldDisplayObject(const DM_Viewport &vport,
				    GUI_DetailLook *look,
				    OBJ_Node *node);
    
    bool	shouldVisualizeObject(const DM_Viewport &vport,
				      GUI_DetailLook *look,
				      OBJ_Node *node);
    
    bool	shouldShowComponentSelections(const GUI_DetailLook &look,
					      const GUI_DisplayOption &d) const;

    enum DM_BufferBlend
    {
	ACCUM_ADD,
	ACCUM_REPLACE,
	ACCUM_ALPHA_BLEND
    };

    // prints a message in the viewer (centrally justified) on a given line
    void	renderMessage( RE_Render *ren, int x, int y,
			       const char* msg, int line );
    // renders true viewer name
    void	doRenderCurrentName(RE_Render *r, int x, int y);
    // renders the snapshot label
    void	doRenderSnapshotName(RE_Render *r, int x, int y); 

    // prints a message in the viewer for camera (central, bottom justified)
    void	renderCameraMessage( RE_Render *ren, int x, int y,
				     const char* msg);

    // Render a mask over the parts of the viewport outside of the view
    // subregion.
    void		 renderViewMask(RE_Render *r, int x, int y);
 
    bool		 resizeBeautyPassBuffer(RE_Render *r,
				int width, int height,
				bool hdr, bool needs_aa);
    void		 destroyBeautyPassBuffer();
    RE_OGLFramebuffer   *getBeautyPassBuffer(bool allow_aa);

    // initializes the view and projection to a 1:1 pixel transform.
    void		 pushIdentityOrthoView(RE_Render *r);
    void		 popIdentityOrthoView(RE_Render *r);
   
    // Render the viewport name.
    void		 doRenderName(RE_Render *r, int x, int y,
				bool snapshot_name = false);
    // Render the current state information.
    void		 doRenderStateStatus(RE_Render *r);
    // Render a mask over the parts of the viewport outside of the view
    // subregion.
    void		 doRenderViewMask(RE_Render *r, int x, int y);

    // Common utility methods to setup lights and the z buffer.
    void		 setupForRender(RE_Render &ren,
				bool allow_smooth = true);
    void		 cleanupAfterRender(RE_Render &ren);


    void		 setGammaPass(bool b) { myGammaPass = b?1:0; }
    
    void		 doRenderNoHookUnlitObjects(RE_Render *r, int x, int y);
    void		 doRenderUnlitObjects(RE_Render *r, int x, int y);
    
    // Render the camera's mask (bars and anything else inside the mask)
    void		 doRenderMask(RE_Render *r, int, int);
    
    void		 startBeautyPass(RE_Render *r, bool allow_aa=true);
    void		 endBeautyPass(RE_Render *r, bool allow_aa=true);
    bool		 isBeautyPassAA(bool allow_aa = true);
    
    // helper methods for setting up stereo passes
    void		 setupStereo3d(RE_Render *r);
    void		 setupStereoVR(RE_Render *r);
    void		 cleanupStereo3d(RE_Render *r);
    void		 interruptStereo(RE_Render *r);
    void		 resumeStereo(RE_Render *r);
    void		 setupStereo3dForLeftCamera(RE_Render *r,
						    bool uniforms_for_3d);
    void		 setupStereo3dForRightCamera(RE_Render *r,
						     bool uniforms_for_3d);
    void		 updateStereoViewUniforms(RE_Render *r, bool left);
    void		 assignStereoLeft(RE_Render *r,
					  const GUI_ViewState::GUI_ViewType vt);
    void		 assignStereoRight(RE_Render *r,
					  const GUI_ViewState::GUI_ViewType vt);
    void		 assignStereoTop(RE_Render *r,
					  const GUI_ViewState::GUI_ViewType vt);
    void		 assignStereoBottom(RE_Render *r,
					  const GUI_ViewState::GUI_ViewType vt);

    bool		 setupTransparentBuffers(RE_Render *r, int hdr);
    void		 destroyTransparentBuffers();
    
    void		 renderViewport(RE_Render *r, int x, int y);
    void		 renderSceneView(RE_Render *r, int x, int y);

    // Render the transparent bars defined by the camera's mask parameters
    void		 renderCameraMask(RE_Render *r);

    // Render the object that goes on top of the camera mask
    void		 renderCameraMaskOverlay(RE_Render *, int, int);

    void 		 renderTransparentGeometry(
				RE_Render *r,
				GUI_DisplayOption *dispopt);
    bool 		 renderHQTransparencyPass(
				RE_Render *r, int x, int y,
				GUI_DisplayOption *dispopt,
				const UT_Array<GUI_DetailLook *> *geos = NULL,
				bool for_xray = false);
    bool 		 setupHQTransparentBuffers(RE_Render *r, int nsamples,
				bool hdr);
    void 		 destroyHQTransparentBuffers();

    void 		 initStencilABuffer(RE_Render *r, int nsamples);

    bool		 setupUnlitAlphaBuffer(RE_Render *r, bool zbuf);
    void		 destroyUnlitAlphaBuffer();
    
    // Geometry pick code
    bool	 createPickBuffer(RE_Render *r);
    bool	 renderPickBuffer(RE_Render *r,
				  bool object_pick,
				  bool enable_occlusion,
				  unsigned int pickmask,
				  GEO_PrimTypeCompat::TypeMask pmask,
				  const GUI_DetailList *geos,
				  const GR_ObjectPickFlags &pick_flags);
    
    // Query the pick buffer.  Returns true on a hit.
    //   allow_geo		- allow geometry picks (either obj or component
    //   			  depending on geo_is_object)
    //   allow_handle		- allow handle picks
    //   geo_is_object		- geometry records are objects instead of
    //   			  components
    //   geo_ignore_depth	- avoid scanning samples looking for a closer
    //				  hit (only when !geo_is_object)
    //   geo_prioritize		- scan samples looking for a higher priority
    //				  component pick (only when !geo_is_object)
    //   floor_prioritize	- scan samples looking for a higher priority
    //				  floor component pick (only when allow_handle
    //				  and does not differentiate between different
    //				  floors)
    //	 overlap_dist		- the distance at which to consider picks to
    //				  be essentially overlapping and employ some
    //				  heuristics to pick the best one
    bool	 pickFromBuffer(RE_Render *r,
				RE_PickBuffer *pick_buffer,
				int x, int y,
				bool allow_geo,
				bool allow_handle,
				bool geo_is_object,
				bool geo_ignore_depth,
				bool geo_prioritize,
				bool floor_prioritize,
				int pick_dist,
				int overlap_dist,
				GR_PickRecord &result,
				UT_Array<GR_PickRecord> *results,
				fpreal *depth,
				const GUI_DetailList *geos);
     
    int		 filterPickData(int32 *pick_base_data,
				int32 *pick_component_data,
				int npicks,
				UT_Array<GR_PickRecord> &pick_records);
    int		 subtractPickData(const UT_Vector3iArray &base,
				  const UT_Vector3iArray &comp,
				  const UT_Array<GR_PickRecord> &outside,
				  UT_Array<GR_PickRecord> &results);
    void	 objectPickRender(RE_Render *r,
				  unsigned int pickmask,
				  GEO_PrimTypeCompat::TypeMask primmask,
				  const GUI_DetailList *geos,
				  const GR_ObjectPickFlags &pick_flags);
    void	 doObjectBoxPick(RE_Render *r,
				 const UT_DimRect &area,
				 UT_Array<GR_PickRecord> &pick_records,
				 bool visible_only,
				 bool invert_mask,
				 bool use_texture,
				 bool clip_edges,
				 const GUI_DetailList *geos);
	
    int		 boxPickVisibleComponent(RE_Render *r,
					 const UT_DimRect &area,
					 uint8 *texture,
					 UT_Array<GR_PickRecord> &pick_records,
					 unsigned pickmask,
					 GEO_PrimTypeCompat::TypeMask pm,
					 const GUI_DetailList *geos,
					 bool contained);
    int		 boxPickFrustumComponent(RE_Render *r,
					 const UT_DimRect &area,
					 uint8 *texture,
					 UT_Array<GR_PickRecord> &pick_records,
					 unsigned pickmask,
					 GEO_PrimTypeCompat::TypeMask pm,
					 const GUI_DetailList *geos,
					 bool visible_only,
					 bool contained);
    int		 boxPickComponentGL42(RE_Render *r,
				      const UT_DimRect &area,
				      uint8 *texture,
				      UT_Array<GR_PickRecord> &pick_records,
				      unsigned pickmask,
				      GEO_PrimTypeCompat::TypeMask pm,
				      const GUI_DetailList *geos,
				      bool visible_only,
				      bool contained);
    void	 renderOffsetDepthPass(RE_Render *r,
				       bool skip_obj_sel_looks = false);
    // end pick code

    // Snapping and handle pick code
    bool		 createSnapHandleBuffer(RE_Render *r);
    bool		 renderHandleBuffer(RE_Render *r);
    bool		 renderSnapBuffer(RE_Render *r,
					  unsigned int snapmask,
					  GEO_PrimTypeCompat::TypeMask pmask,
					  const GUI_DetailList *geos,
					  bool hide_selected,
					  bool enable_occlusion,
					  bool do_xray, bool extend_floor);
    // end snapping and handle pick code

    bool		 sampleDepth(RE_Render *r, int x, int y, fpreal &z,
				RE_Texture *depth_texture);

    void		 renderXray(RE_Render *r,
				    GUI_DisplayOption *dispopts);
    void		 resolveXrayBeautyPass(RE_Render *r) const;
    void		 renderOnionSkin(RE_Render *r,
					 GUI_DisplayOption *dispopts);
    
    bool		 renderBeautyPass(RE_Render *r, int x, int y,
					  GUI_DisplayOption *dispopt,
					  bool &needs_lighting);
    bool		 renderTransparencyPass(RE_Render *r, int x, int y,
						GUI_DisplayOption *dispopt);
    bool		 renderEnvironmentMaps(RE_Render *r);
    void		 compositePasses(RE_Render *r,
					 const GUI_DisplayOption *dispopt,
					 bool has_trans_pass,
					 bool has_alpha_buffers);
    
    void		 normalQualityRender(
				RE_Render *r,
				GUI_DisplayOption *dispopt,
				GR_AlphaPass alpha_pass,
				DM_GeoSortMode sort = DM_GEO_UNSORTED,
				const UT_Array<GUI_DetailLook*> *geos=NULL);
    
    bool  		 isUsingHQLighting(RE_Render *r,
					   const GUI_DisplayOption *dispopt);

    void		 renderOverlappingUV(RE_Render *r,
					     GUI_DisplayOption *disp);
    bool		 setupOverlappingUVBuffer(RE_Render *r, int w, int h);
    void		 destroyOverlappingUVBuffer();

    void		 renderCoincidentPoints(RE_Render *r,
						GUI_DisplayOption *disp);
    bool		 setupCoincidentBuffer(RE_Render *r, int w, int h);
    void		 destroyCoincidentBuffer();

    void		 subtractFromSelection(UT_Array<GR_PickRecord> &results,
					UT_Array<GR_PickRecord> &picks,
					exint start_subtracting_at,
					UT_Array<GR_PickRecord> &sub) const;
#ifdef MBSD_COCOA
    void		 dmOSXDepthBufferWorkaround(RE_Render *r,
						    RE_Texture *tex);
#endif
    void		 updateVisibleBoundariesAndViews(int vis_display_sets);

    mutable RE_ShaderHandle		*myDepthResolveShader;

    UT_ValArray<RE_Light *>		 myMaterialLights;
    
    RE_OGLFramebuffer			*myBeautyPassBuffer;
    RE_OGLFramebuffer			*myBeautyPassAABuffer;
    RE_Texture				*myBeautyColorTexture;
    RE_Texture				*myBeautyDepthTexture;
    RE_Texture				*myBeautyPassAACTexture;
    RE_Texture				*myBeautyPassAAZTexture;
    int					 myBeautyPassSamples;
    bool				 myBeautyPassHDR;

    RE_OGLFramebuffer			*myTransparentFBO;
    RE_Texture				*myTransparentColorTex;
    RE_Texture				*myTransparentZTex;
    bool				 myTransparentHDR;

    DM_RenderHighQuality		*myHQRender;

    RE_Texture				*myRandomDitherTexture;
    
    PXL_Lookup				 myLookup;
    UT_String				 myLUTFile;
    UT_String				 myOCIOLooks;

    bool				 myIsBeautyPass;
    GR_LightingPass			 myLightingPass;
    bool				 myIsAlphaPass;
    bool				 myUseUnlitMask;
    bool				 myHighQualityMode;
    bool				 myNeedsXRayResolve;
    bool				 myHasGILight;
    bool				 myHasEnvLight;
    bool				 myHasShadowedLights;
    bool				 myNeedsOverlayRender;
    uint64				 myStandaloneFlag;
    int					 myStereoMode;
    GR_DecorationOverride		 myDecorationOverride;
    UT_StringHolder			 myDisplayedUVAttrib;

    mutable RE_OGLFramebuffer		*myDepthResolveFBO;
    mutable unsigned int		 myDepthResolveShaderFailed : 1,
					 myZCompositeShaderFailed : 1;

    DM_FloorInterface			*myFloorInterface;
    UT_IntArray				 mySetupLookShowSelection;
    bool				 myHasSelectionsShown;
    int					 myDecorLocateMask;
    int					 myDecorVisibleMask;
    int					 myGammaPass;

    RE_GPUTimer				 myBGTimer;
    RE_GPUTimer				 myFGTimer;
    RE_GPUTimer				 myGeoCleanupTimer;
    RE_GPUTimer				 myUnlitTimer;

    DM_ViewportType			 myType;
    int					 myRenderCount;
    RE_OGLContext			 myInitContext;

    UT_Array<GUI_DetailLook *>		 mySetupLooks;
    UT_Array<OBJ_Node *>		 mySetupObjects;
    UT_Array<GUI_DetailLook *>		 myUnlitLooks;
    UT_Array<GUI_DetailLook *>		 myXRayLooks;
    UT_Array<GUI_DetailLook *>		 myOnionSkinLooks;
    UT_Array<GUI_DetailLook *>		 myTransparentLooks;
    UT_Array<OBJ_Node *>		 myTransparentObjects;

    bool				 mySavedAntiAliasOption;
    GUI_SceneLook			*mySceneLook;
    DM_SceneManager			&mySceneManager;
    DM_Viewport				*myDisplay;
    bool				 mySetupFlag;

    // Buffers for picking geometry.
    UT_UniquePtr<RE_PickBuffer>		 myPickBuffer;
    UT_TokenString			*myPickBufferTestHash;

    // Extra data for box picking.
    RE_ShaderHandle			 myBoxPickObjectShader;
    RE_Geometry				*myBoxPickObjectGeometry;
    RE_Texture				*myBoxPickMaskTexture;

    // Buffers for snapping to components.
    UT_UniquePtr<RE_PickBuffer>		 mySnapHandleBuffer;
    UT_TokenString			*mySnapHandleBufferTestHash;

    RE_LightList			 myLightList;
    UT_ValArray<RE_Light *>		 myNormalQualityLights;

    RE_OGLFramebuffer			*myUnlitAlphaFBO;
    RE_Texture				*myUnlitAlphaTexture;
    RE_Texture				*myUnlitDepthTexture;

    static RE_ShaderHandle		 theBeautyCompShader;
    static RE_ShaderHandle		 theDepthCompShader;
    static RE_ShaderHandle		 theDepthComp8Shader;
    static RE_ShaderHandle		 theDepthComp16Shader;
    static RE_ShaderHandle		 theUnlitAlphaShader;
    static RE_ShaderHandle		 theUnlitDepthShader;
    static RE_ShaderHandle		 theXRayShader;

    int					 myDecorLocateRawPos[2];
    int					 myDecorLocatePos[2];
    RE_ShaderHandle			 mySampleDepthShader;

    RE_OGLFramebuffer			*myHQTransparentFBO;
    RE_Texture				*myHQTransparentColorTex;
    RE_Texture				*myHQTransparentStencil;
    bool				 myHQTransparentHDR;

    UT_UniquePtr<RE_Geometry>		 myCubeGeo;

    RE_OGLFramebuffer			*myCoincidentBuffer;
    RE_Texture				*myCoincidentTexture;
    RE_OGLFramebuffer			*myOverlappingUVBuffer;

    bool				 myRampUnitPushed;

    RE_RenderWait			*myBeautyPassSync;

    RE_GPUTimer				 myPreRenderTimer;
    RE_GPUTimer				 myPostRenderTimer;
    RE_GPUTimer				 myXRayTimer;
    RE_GPUTimer				 myOnionSkinTimer;
    RE_GPUTimer				 myCompositeTimer;
    RE_GPUTimer				 myBGEnvMapTimer;
    RE_GPUTimer				 myDecorTimer;
    RE_GPUTimer				 myHandleTimer;

    bool				 myUseHQLighting;

    DM_ViewportHooks			*myViewportHooks;
    UT_IntArray				 myViewportConfig;
};

inline int
DM_VPortAgent::getNumOpaqueObjects() const
{
    return mySetupLooks.entries();
}

inline DM_GeoDetail
DM_VPortAgent::getOpaqueObject(int index) const
{
    return DM_GeoDetail(mySetupLooks(index));
}

inline int
DM_VPortAgent::getNumTransparentObjects() const
{
    return myTransparentLooks.entries();
}

inline DM_GeoDetail
DM_VPortAgent::getTransparentObject(int index) const
{
    return DM_GeoDetail(myTransparentLooks(index));
}
    
inline int
DM_VPortAgent::getNumUnlitObjects() const
{
    return myUnlitLooks.entries();
}

inline DM_GeoDetail
DM_VPortAgent::getUnlitObject(int index) const
{
    return DM_GeoDetail(myUnlitLooks(index));
}

inline int
DM_VPortAgent::getNumXRayObjects() const
{
    return myXRayLooks.entries();
}

inline DM_GeoDetail
DM_VPortAgent::getXRayObject(int index) const
{
    return DM_GeoDetail(myXRayLooks(index));
}
    
#endif
