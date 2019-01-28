/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GUI_DetailLook.h ( GUI Library, C++)
 *
 * COMMENTS:
 *	A detail look encapsulates all of the data necessary to render a
 *	geometry in a viewport.
 *
 *      The name is a holdover from when it was derived from UI_Rectangle.
 */

#ifndef __GUI_DetailLook__
#define __GUI_DetailLook__

#include "GUI_API.h"
#include <UT/UT_Assert.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_BoundingSphere.h>
#include <UT/UT_Color.h>
#include <UT/UT_Map.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_PerfMonAutoEvent.h>
#include <UT/UT_PerfMonTypes.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UI/UI_Value.h>
#include <GEO/GEO_PackedTypes.h>
#include <GU/GU_Detail.h>
#include <GU/GU_DetailHandle.h>
#include <OP/OP_Network.h>
#include <GR/GR_DisplayOption.h>
#include "GUI_DisplayOption.h"
#include "GUI_RenderCommon.h"
#include "GUI_VisInterface.h"
#include "GUI_GroupData.h"
#include "GUI_GUSelectionProxyHandle.h"

class RE_Render;
class RE_Light;
class RE_Material;
class RE_Geometry;
class SIM_Data;
class SIM_RootData;
class GUI_GeoRender;
class GU_Detail;
class GU_ConstDetailHandle;
class gui_Instance;
class gui_GeoInstanceInfo;
class GUI_VisInterface;

class GUI_API GUI_DetailLook
{
public:
	    GUI_DetailLook(GU_DetailHandle *gh = NULL,
			   const char *detail_name = NULL);
    virtual ~GUI_DetailLook();

    virtual const char	*className() const;

    virtual bool	isObjectList() const { return false; }
    virtual bool	isDopGuide() const { return false; }
    virtual bool	needsOverlayRender() { return false; } 

    const char	       *getName() const { return myDetailName; }
    void		setName(const char *name);

    void		setInitialized() { myInitializedFlag = true; }
    bool		isInitialized() const { return myInitializedFlag; }

    // Find out which node our gdp represents, if any:
    OP_Node	*getObject() const;		// inline, below
    OP_Node	*getSop() const;		// inline, below
    OP_Node	*getShopsRelativeTo() const;	// inline, below
    int		 getVisualizedNodeId() const; 	// inline, below

    // Set to new object/sop pair. returns true if the object or sop was
    // actually changed.
    bool	 setObject(OP_Node *node);
    bool	 setSop(OP_Node *node);

    // New Viewport codepaths

    // setupForRender() can be called in parallel. setup() methods are expected
    // to do non-GL work, such as convexing, surfacing, point normal computation
    virtual GUI_SetupResult setupForRender(
				const GUI_DisplayOption &opts,
				bool show_selections,
				GR_DecorationOverride selmask_dec,
				GR_DecorationOverride &sel_dec);

    // This is a stripped down version of setupForRender() when the viewer is
    // showing more than 1 viewport. The first viewport rendered does a full
    // setupForRender(), and subsequent viewports do fastSetupForRender(), since
    // many things do not change.
    bool		fastSetupForRender(GUI_DisplayOption &opts);

    // updateForRender() is called in serial. It does all the GL updating based
    // on the precomputed results in setup().
    void		updateForRender(
				RE_Render  *r,
				GUI_DisplayOption &opts,
				RE_LightList *lights,
				GR_DecorationOverride selmask_dec,
				const char *view_name,
				const GR_FrustumList &view_frustums,
				bool &needs_alpha,
				bool normal_update,
				GR_UpdateReason additional_reasons);
    
    virtual void	cleanupAfterRender(RE_Render *r);

    // called when this look is no longer displayed, but kept around for a short
    // time in case it might be displayed in the future
    void		retire();
    bool		isRetired() const { return myRetiredFlag; };
    void		reactivateRetired();

    // called when temporarily hidden, such as as display flag off.
    void		hide();

    // appends the contained geometry objects to 'geos'.
    void		getGeometryRenders(
				UT_Array<GUI_GeoRender*> &geos,
				GR_LightingPass p = GR_LIGHTING_ANY,
				int geo_index = -1);
    void		setSingleGeometryRender(OP_Node *node);
    void		resetGeometryRender();

    // Set selection data to be displayed with this geometry.
    const UT_Array<GUI_GroupDataHandle> &getSelectData(int geo_index) const;
    void		addSelectData(GUI_GroupDataHandle select_data,
				      int geo_index);
    void		clearSelectData();
    bool		hasSelectData() const;

    // This method determines whether this detail look should be rendered
    // for picking.  It only checks if it is potentially pickable, but does
    // not check if it currently being drawn. This only refers to the component
    // level picking present in SOPs. For object picking, check the isPickable()
    // flag on the getObject() node.
    virtual bool	isPickable() const;

    // This method checks if this detail look is even rendered given the
    // specified display options.
    virtual bool	isVisible(const GUI_DisplayOption &dopt) const;

    // Returns true if any of the contained details are lit.
    bool		areAnyLit() const { return !isGuide(); }

    // returns true if any of the contained details are unlit.
    bool		areAnyUnlit() const { return myHasUnlitGeometry; }

    // returns true if any of the contained details have transparency.
    bool		hasTransparency() const;

    // returns true if any of the materials are reflective.
    bool		hasReflectiveMaterial() const;
    
    // returns a bitfield of decorations supported by all the GR_Primitives in
    // this look.
    int64		getPrimDecorMask() const { return myDecorMask; }

    // This look will not free the detail, it is up to the user
    int				 setDetailHandle(GU_DetailHandle handle);
    virtual int			 getNumDetailHandles() const; // inlined below
    virtual GU_DetailHandle	 getDetailHandle(int i) const;
    virtual UT_Matrix4D		 getDetailXform(int i) const;
    virtual void		 appendDOPDataName(UT_String &str, int i,
					bool descriptive) const;
    virtual const SIM_Data	*getDOPData(int i) const;
    virtual const SIM_RootData	*getDOPRootData(int i) const;
    virtual bool		 getIsDetailTempSelected(int i) const;
    // Get world space position
    virtual UT_Vector4F		 getDetailPosition(int i) const;
    virtual void 		 getDetailName(UT_String &name, int i) const;
    virtual void		 getDetailPath(UT_String &path, int i) const;
    virtual void 		 getDetailDescriptiveName(UT_String &name,
							  int i) const;
    virtual void		 getDetailDescriptivePath(UT_String &path,
					int i) const;
    void			 signalNodeSelectionChanged(int i) const;

    virtual void		 setTempComponentSelection(
					int i, GUI_GUSelectionProxyHandle h);
    virtual GU_SelectionHandle	 getTempComponentSelection(int i) const;

    virtual void		 releaseAllTempComponentSelections();

    void			 setEnableDOPDisplay(bool enable)
				 { myEnableDOPDisplay = enable; }
    bool			 getEnableDOPDisplay() const
				 { return myEnableDOPDisplay; }
    void			 setIsChosenDopnet(bool ischosen)
				 { myIsChosenDopnet = ischosen; }
    bool			 getIsChosenDopnet() const
				 { return myIsChosenDopnet; }

    bool			 hasUserSelection() const;

    void		 setLightArray(RE_Light *lights[], int nLights = 0);

    void		 setGlobalLightArray(const UT_ValArray<RE_Light *> &lts,
					     bool headlight);
    void		 enableAllLights();

    int			 getNumLights() const { return myLights.entries(); }
    RE_Light		*getLight(int i) const { return myLights(i); }
    const UT_ValArray<RE_Light *> &getLights() const { return myLights; }
    const UT_ValArray<RE_Light *> &getEnabledLights() const
					       { return myEnabledLights; }

    static RE_Light	*getGlobalHeadlight();

    // Do not instruct the following two methods to keep the caches unless
    // you're really sure and know what you're doing.
    void		 setUseWireColor(int c, bool clear_caches = true);
    void		 setWireColor(const UT_Color &clr,
				      bool clear_caches = true);

    virtual UT_Color	 getWireColor(int node_id) const
			 { return myWireColor; }

    // Methods to manage the view matrix, the user may use any of the
    // UT_Matrix4 methods to transform the matrix.
    void		 setXform(const UT_Matrix4D &mat)
			 { myXform = mat; myXformDirty = true; }
    UT_Matrix4D		 getXform() const
			 { return myXform; }
    void		 setPivot(const UT_Vector3F &pivot)
			 { myPivot = pivot; }

    void		 setAllowShadingOpenCurves(bool val);
    bool		 shadeOpenCurves() const
				{ return flag.shade_open_curves; }
    
    void		 setDisplayAsSubdiv(bool val);

    // Methods to handle the look of the detail drawn.
    void		 copyFlagsFrom(const GUI_DetailLook *look);
    
    virtual void setDetailDisplay(bool display)
			    {
				if(!flag.overrideDisplay)
				{
				    if(flag.display != display)
					myDisplaySetDirty= true;
				    flag.display = display;
				}
			    }

    void	setGeometry(bool enable) { flag.isGeometry = enable; }
    void	setShowAxes(bool show_axes) { flag.showAxes = show_axes; }
    void	setPicking(bool picking)
			    { flag.shouldPick  = picking; }

    void	setRender(bool render)	{ flag.render	   = render; }
    void	setTemplate(bool templated)
		    {
			    if(flag.templated!=templated)
				myDisplaySetDirty=true;
			    flag.templated    = templated;
		    }
    void	setFootprint(bool footprint)
		    { if(flag.footprinted!=footprint)
			    myDisplaySetDirty=true;
		      flag.footprinted = footprint; }
    void	setHighlight(bool highlight)
		    { myDisplaySetDirty |= (flag.highlighted != highlight);
		      flag.highlighted = highlight; }
    void	setXray(bool xray)
		    { flag.xray	   = xray; }
    void	setTargetOutput(bool target_output)
		    {   if(flag.targetoutput !=target_output)
			    myDisplaySetDirty=true;
			flag.targetoutput = target_output; }
    void	setCurrentOutput(bool current_output, bool clear_caches = false);
    void	setDrawLit(int onoff); // it's a bit more complicated now
    void	setDrawCookComponentSelection(bool enable);
    void	setSnappable(int onoff) { flag.snappable   = onoff; }
    void	setSecondarySelection(int onoff);
    void	setObjSelection(bool obj_sel);
    void	setObjCurrent(bool obj_current);
    void	setObjSelectionHint(bool obj_sel_hint);
    virtual void	setObjGhost(bool obj_ghost, OP_Node *for_node);
    void	setWireframeOnly(bool wireframe_only)
		    { flag.wireframeOnly = wireframe_only; }
    void	setActiveModel(bool active_model)
		    {
			if(flag.active_model != active_model)
			{
			    myDisplaySetDirty = true;
			    flag.active_model = active_model;
			}
		    }

    static void	setUVObject(OP_Node *object);
    bool	isUVObject() const;

    void	setOnionSkinning(int onion_skin_state);
    bool	isOnionSkinning() const { return flag.onion_skin; }
    bool	isOnionSkinDeformation() const { return flag.onion_skin_deform;}
    
    void	onionSkinRender(RE_Render *r, GUI_DisplayOption &disp_opt,
				int nsamples);


    // Frustum culling is enabled by default.
    void	useFrustumCulling(bool frust_cull)
			{ flag.use_frustum_culling = frust_cull; }
    bool	usesFrustumCulling() const
			{ return flag.use_frustum_culling && !flag.onion_skin
				&& !myLastPointInstanceState &&
				getUsesGeoRenderInstancing(); }

    virtual void applyBundleToDisplayedObjects( OP_Bundle *bdl, int object_vis_change_count);

    // The following methods allow the user to set the display flag while
    // caching the old value, and resetting it back later.
    virtual void setDisplaySavingPrevValue(int onoff)
		{
		    flag.prevDisplay = flag.display;
		    flag.display = onoff;
		    flag.overrideDisplay = true;
		}

    virtual void resetOverrideUsingSavedValue()
		{
		    flag.display = flag.prevDisplay;
		    flag.overrideDisplay = false;
		}

    // this is not the underlying node's display flag state.
    bool	isDetailDisplayOn() const; // inlined below
    
    bool	getShowAxes() const	{ return flag.showAxes; }
    bool	isGeometry() const	{ return flag.isGeometry; }
    bool	isPickingOn() const	{ return flag.shouldPick; }
    bool	isRenderOn() const	{ return flag.render; }
    bool	isTemplated() const	{ return flag.templated; }
    bool	isFootprinted() const	{ return flag.footprinted; }
    bool	isHighlighted() const	{ return flag.highlighted; }
    virtual bool isXray() const		{ return flag.xray || myIsXRay; }
    bool	isTargetOutput() const	{ return flag.targetoutput; }
    bool	isCurrentOutput() const	{ return flag.currentoutput; }
    virtual bool isGuide() const	{ return false; }
    virtual bool useWireColor(int node_id) const { return flag.usewireclr; }
    bool	drawLit() const		{ return flag.drawLit; }
    bool	displayAsSubdiv()	{ return flag.displayAsSubdiv; }
    bool	drawCookComponentSelection() const
			    { return flag.drawCookComponentSelection; }
    bool	isSnappable() const	{ return flag.snappable; }
    bool	isSecondarySelection() const { return flag.secondarySelection; }
    bool	isObjSelection() const	{ return flag.objSelection; }
    bool	isObjCurrent() const	{ return flag.objCurrent; }
    bool	isObjSelectionHint() const { return flag.objSelectionHint; }
    bool	isObjGhost() const { return flag.objGhost; }
    bool	isWireframeOnly() const	{ return flag.wireframeOnly; }
    bool	isActiveModel() const { return flag.active_model; }
    bool	isPointInstanced() const { return myHasPointInstancing; }
    bool	hasInstanceFile() const { return myHasInstanceFile; }
    bool	useInstancerObjTransform() const
					{ return myInstanceUseTransform; }


    bool	needsAlphaPass() const
		    { return flag.needsAlphaPass; }
    void	setNeedsAlphaPass(bool e)
		    { flag.needsAlphaPass = e; }

    // Flag as a deleted look, so that it can be deleted rather than cached.
    void	setIsDeleted() { flag.being_deleted = true; }
    bool	isDeleted() { return flag.being_deleted; }

    // Set information about which geometry groups to color.
    const UT_StringHolder	&showGroupMask() const;
    void			 setShowGroupMask(const UT_StringHolder &mask);
    GA_GroupType		 showGroupType() const;
    void			 setShowGroupType(GA_GroupType group_type);
    int				 showGroupColorVersion() const;
    void			 setShowGroupColorVersion(int version);

    int				 getBoundingBox2D(UT_BoundingBox &bbox,
					const char *name,
					int use_vertex) const;
    int				 getBoundingBox3D(UT_BoundingBox &bbox,
					const UT_Matrix3R *rot) const;

    void			 setViewportMask(unsigned mask) 
				 { myViewportMask = mask; }
    unsigned int		 getViewportMask() const
				 { return myViewportMask; }

    static void setGlobalViewSerial(GUI_DisplayOption &disp,int serial, int id)
		{
		    disp.common().setViewSerialNum(serial, id);
		}

    // return the display set that this object belongs to.
    GUI_DisplaySetType	 getDisplaySetType() const
			 {
			     if(!myDisplaySetDirty)
				 return myDisplaySet;
			     return privGetDisplaySetType();
			 }

    GUI_DisplaySetType	 updateDisplaySetType()
			 { return privGetDisplaySetType(); }

    void 		renderObjectName(RE_Render *r,
				GUI_DisplayOption *dispopt);
    virtual void 	renderOrQueueObjectAxes(UT_Vector3FArray &axes,
						UT_Vector3FArray &pivots);
    static void		renderQueuedObjectAxes(RE_Render *r,
					       GUI_DisplayOption *dispopt,
					       const UT_Vector3FArray &axes,
					       const UT_Vector3FArray &pivots);

    // The global chosen dopnet is set to filter out the drawing of geometry
    // flagged as originating from this dopnet.
    static void		 setGlobalChosenDopnet(OP_Node *dopnet)
    {
	if( dopnet )
	{
	    dopnet->getFullPath(ourChosenDopnetPath);
	    ourChosenDopnetPathLen = ourChosenDopnetPath.length();
	}
	else
	{
	    ourChosenDopnetPath = 0;
	    ourChosenDopnetPathLen = 0;
	}
    }
    static const char	*getGlobalChosenDopnetPath()
    {
	return (const char *) ourChosenDopnetPath;
    }

    static bool		 matchesChosenDopnetPath(const char *path)
    {
	if( ourChosenDopnetPathLen > 0 &&
	    !strncmp(ourChosenDopnetPath, path, ourChosenDopnetPathLen) &&
	    (path[ourChosenDopnetPathLen] == ':' ||
	     path[ourChosenDopnetPathLen] == '\0') )
	    return true;
	return false;
    }

    // Used to assign a temporary ID to the look.
    void		 setLookID(int id) { myID = id; }
    int			 getLookID() const { return myID; }

    // Get/Set the object's viewport LOD
    GEO_ViewportLOD	 getObjectLOD() const	{ return myObjectLOD; }
    void		 setObjectLOD(GEO_ViewportLOD lod)
			    { myObjectLOD = lod; }

    void		 useObjectMaterials(bool use_obj_mat);
    bool		 getAssignedMaterials(UT_Array<OP_Node *> &nodes) const;

    virtual bool	 isTimeDependent(fpreal t);
	    bool	 areInstancesTimeDependent(fpreal t);
    virtual int64	 getVersion();
    void		 bumpVersion() { myLookVersion++; }

    // The name of the actively rendering or updating viewport
    static void		 setActiveViewport(const char *name);
    // viewname (persp1)
    static const char	*getActiveViewport();
     // viewname/Material setup
    static const char	*getActiveViewportMatString();

    
    static void		 visibleViewportTypes(bool view_3d, bool view_2d);
    static bool		 hasVisible3D();
    static bool		 hasVisibleUV();

    static void		 visibleUVAttribs(UT_StringArray &attribs);
    static const UT_StringArray &visibleUVAttribs();
    static int64	 visibleUVAttribSerial();

    enum { VIEW_3D = 0x1, VIEW_2D = 0x2 }; // for the masks
    static void		 visibleViewportBoundaries(
				bool bound_3d,
				UT_StringArray &bound_3d_uvs,
				UT_Array<UT_Pair<UT_StringHolder,int> > &uvs);
    static bool		 hasVisibleBoundaries();
    static bool		 visible3DBoundary();
    static const UT_StringArray &visible3DBoundariesInUV();
    // uv name, view mask
    static const UT_Array<UT_Pair<UT_StringHolder,int> > &visibleUVBoundaries();

    const UT_IntArray	&getActiveVisualizers() const
			    { return myActiveVisualizers; }
    int			 numActiveVisualizers() const
			    { return myActiveVisualizers.entries(); }
    void		 appendActiveVisualizer(int vis_id)
			    { myActiveVisualizers.append(vis_id);
			      myCollectVisPrimitives.append(0); }
    void		 clearActiveVisualizers()
			    {
				myActiveVisualizers.clear();
				myCollectVisPrimitives.clear();
			    }
    GR_Primitive 	*getCollectVisPrimitive(int i)
			    { return myCollectVisPrimitives(i); }
    void		 setCollectVisPrimitive(int i, GR_Primitive *prim)
			    { myCollectVisPrimitives(i) = prim; }

    void		 checkForErrors();
    int64		 getNodeErrorID() const { return myOpErrorPosted; }

    void		 setRedrawValue(UI_Value *redraw)
			    { myRedrawValue = redraw; }
    void		 requestRedraw()
			    { if(myRedrawValue) myRedrawValue->changed(NULL); }

    // Wrapper class to avoid sprintf's when not recording
    class PerfMonAutoMatEvent : public UT_PerfMonAutoEvent
    {
    public:
	PerfMonAutoMatEvent(const GUI_DetailLook &look)
	{
	    // Early exit if we're not recording
	    UT_Performance *perf = UTgetPerformance();
	    bool record_viewport = perf->isRecordingViewportStats();
	    bool record_memory = perf->isRecordingMemoryStats();
	    if (!record_viewport && !record_memory)
		return;
	    
	    const char *
		viewport_name = GUI_DetailLook::getActiveViewportMatString();
	    const char *
		look_name = look.getName();

	    if (record_viewport)
	    {
		setTimedEventId_(
		    perf->startTimedViewportDrawEvent(
			viewport_name, look_name, UT_PERFMON_3D_VIEWPORT));
	    }
		    
	    if (record_memory)
	    {
		setMemoryEventId_(
		    perf->startMemoryViewportDrawEvent(
			viewport_name, look_name, UT_PERFMON_3D_VIEWPORT));
	    }
	}
    };

protected:
    void		 cacheNodePtrs();

    // Multi-object instancing
    void		 initInstanceTable();
    GUI_GeoRender	*addInstance(GU_ConstDetailHandle cgdh,
				     const char *name,
				     const UT_Matrix4D &xform,
				     int64 version,
				     int instance_index,
				     int object_id,
				     int inst_object_id,
				     bool is_guide,
				     bool time_dep);
    void		 finalizeInstanceTable();

    using gui_CachedInstGeo = UT_Pair<GUI_GeoRender *, bool>;

    bool		 createObjectInstance(const char *instance_path,
				int64 base_version,
				fpreal t,
				gui_Instance &instance,
				OBJ_Node *instancer,
				UT_StringMap<gui_CachedInstGeo> &existing,
				bool rel_to_sop);
    void		 markGeoForRemoval(GUI_GeoRender *gr);
    void		 removeDeletedGeos();
    int64		 computeInstanceVersion(const GU_Detail *dtl);

    // Indicates whether instancing should be implemented using the
    // GUI_GeoRender instancing capabilities. If false, GUI_GeoRenderShared
    // objects are used to instance the geometry without true OGL instancing.
    virtual bool	 getUsesGeoRenderInstancing() const
			 { return true; }

    // End multi-object instancing
    GUI_SetupResult	 setupAllGeos(const GUI_DisplayOption &disp_opt,
				bool show_selections,
				GR_DecorationOverride selmask_dec,
				GR_DecorationOverride &sel_dec);

    virtual GUI_GeoRender *getOrCreateGeometryRender(int index);

    void		 setXRayOverride(bool xray) { myIsXRay = xray; }

    UT_Array<GUI_GeoRender *>			 myGeoRender;
    UT_Array<GUI_GeoRender *>			 myStashedGeoRenderShared;
    UT_Array<GUI_SetupResult>			 mySetupResults;
    UT_Map<int, UT_Array<GUI_GroupDataHandle> >	 mySelectData;

    UT_Array<GUI_GeoRender *>			 myOnionSkinRenders;
    UT_Array<GUI_SetupResult>			 myOnionSkinSetupResults;
    
private:
    void 		 generateObjectName(RE_Render *r,
				GUI_DisplayOption *dispopt,
				UT_String name,
				int geo_index,
				GR_DecorFontSize fontsize,
				RE_Geometry &geo);
    bool		 setupPointInstancing(bool show_selections,
				GR_DecorationOverride selmask_dec,
				GR_DecorationOverride &select_dec,
				fpreal instance_fract,
				int limit);
    GUI_DisplaySetType	 privGetDisplaySetType() const;

    void		 updateInstancingStatus();
    GUI_GeoRender *	 createGeoRender(int index,const char *opt_suffix=NULL);

    GUI_GeoRender	*checkForDetailCaching(GUI_GeoRender *geo);
    void		 freeCachedDetails();
    void		 freeDeletedCachedDetails();

    int			 getNumOnionSkins() const;
    const GU_DetailHandle &getOnionSkinDetail(int index) const;
    GUI_SetupResult	 processSkins(const GUI_DisplayOption &dopt);
    void		 addOnionSkin(OBJ_Node *obj, SOP_Node *sop,
				      fpreal skin_t, fpreal geo_t,
				      bool deforming,
				      bool is_pre_frame);
    void		 clearOnionSkins();

    class GUI_DetailLookFlags
    {
    public:
	GUI_DetailLookFlags()
	{
	    showAxes		= false;
	    render		= true;
	    isGeometry		= true;
	    shouldPick		= true;
	    display		= true;
	    templated		= false;
	    footprinted		= false;
	    highlighted		= false;
	    xray		= false;
	    targetoutput	= false;
	    currentoutput	= false;
	    usewireclr		= false;
	    drawLit		= true;
	    drawCookComponentSelection = true;
	    needsAlphaPass	= false;
	    snappable		= true;
	    secondarySelection	= false;
	    objSelection	= false;
	    objCurrent		= false;
	    objSelectionHint	= false;
	    objGhost		= false;
	    wireframeOnly	= false;
	    prevDisplay		= true;
	    prevShouldPick	= true;
	    overrideDisplay	= false;
	    displayAsSubdiv	= false;
	    shaders_used	= false;
	    active_model        = false;
	    being_deleted	= false;
	    shade_open_curves   = false;
	    onion_skin		= false;
	    onion_skin_deform	= false;
	    use_frustum_culling = true;
	}

	bool      showAxes:1,		// draw the axes of the detail
	        isGeometry:1,
		    render:1,		// render the detail
		shouldPick:1,
		   display:1,		// display the detail
		 templated:1,
	       footprinted:1,
	       highlighted:1,		// Node has the highlight flag set
		      xray:1,		// display wireframe within other geo
	      targetoutput:1,		// display sop when in view current
					// mode
	     currentoutput:1,		// current sop when in view current
					// mode and the display flag is on a
	                                // different sop.
		usewireclr:1,		// Override default wire color
		   drawLit:1,		// In shaded mode, draw shaded
drawCookComponentSelection:1,		// Draw the GU cook component selection
	    needsAlphaPass:1,		// requires a second pass
	         snappable:1,		// is the detail snappable
	secondarySelection:1,		// Draw a secondary selection
              objSelection:1,		// object selection
                objCurrent:1,		// object current flag
          objSelectionHint:1,		// hint that object makes good selection
	          objGhost:1,		// ghosted object
             wireframeOnly:1,		// draw in wireframe only
               prevDisplay:1,		// cached value of the previous value of the display flag.
               prevShouldPick:1,	// cached value of the previous value of the pick flag.
	   overrideDisplay:1,		// don't allow changes to the display
	   displayAsSubdiv:1,		// draw as subdivision.
	      shaders_used:1,		// Shaders have been used on the object
	      active_model:1,		// Use SOP-level display sets
	     being_deleted:1,		// We are in the process of deletion
	 shade_open_curves:1,		// Open curves are light-shaded
		onion_skin:1,		// Draw faded images before/after
	 onion_skin_deform:1,		// Use deformation onion skinning
       use_frustum_culling:1;		// Frustum culling is allowed
    };
    GUI_DetailLookFlags	 flag;

    UT_StringHolder	 myShowGroupMask;
    GA_GroupType	 myShowGroupType;
    int			 myShowGroupColorVersion;
    GU_DetailHandle	 myDetailHandle;
    GUI_GUSelectionProxyHandle	 myTempComponentSelection;
    UT_Array<GU_DetailHandle> myOnionSkinDetails;

    // Override the wireframe display color.
    UT_Color		 myWireColor;

    int			 myObjectID;
    int			 mySopID;
    OP_Node		*myCachedObject;
    OP_Node		*myCachedSop;
    UT_String		 myDetailName;
    int			 myID;
    GEO_ViewportLOD	 myObjectLOD;
    bool		 myIsXRay;
    bool		 myIsOnionSkin;
    UT_String		 myInstanceObjectRef;

    UT_ValArray<RE_Light *> myLights;
    UT_ValArray<RE_Light *> myEnabledLights;

    unsigned int	 myViewportMask;
    bool		 myEnableDOPDisplay;
    bool		 myIsChosenDopnet;
    bool		 myNullDetailFlag;
    bool		 myRetiredFlag;
    bool		 myInitializedFlag;

    mutable bool		 myDisplaySetDirty;
    mutable GUI_DisplaySetType	 myDisplaySet;    

    fpreal		 myPrevInstanceFract;
    int			 myPrevInstanceLimit;
    int			 myInstanceFractSerial;
    int64		 myPointInstanceVersion;
    int			 myInstanceXformVersion;
    fpreal		 myInstanceTime;
    int			 myInstanceObjectID;
    bool		 myHasInstancing;
    bool		 myHasPointInstancing;
    bool		 myInstanceUseTransform;
    bool		 myHasTransparentPrimitives;
    bool		 myLastPointInstanceState;
    bool		 myHasInstanceFile;
    bool		 myXformDirty;
    bool		 myDetailDirty;
    UT_Matrix4D		 myXform;
    UT_Vector3F		 myPivot;
    int64		 myOpErrorPosted;
    int64		 myLookVersion;

    // Variable geometry Instancing
    UT_Map<int, gui_GeoInstanceInfo *> myDetailTable;
    int			 myGeoCountId;
    const GU_Detail	*myTempLastDetail;
    GUI_GeoRender	*myTempLastGeo;

    // Cached details for cached playback (ie cache SOP)
    UT_Map<exint, GUI_GeoRender *> myCachedDetailTable; 
    bool		 myCachingDetails;
    int64		 myCacheSopID;
    int64		 myCacheSopSerial;

    bool		 myHasUnlitGeometry;
    int64		 myDecorMask;

    exint		 myVisUpdateTime;	// last visualization-triggered
						// update time (visualization
						// clock measurement)
    exint		 myVisUpdateVersion;	// version at the time of last
						// visualization-triggered
						// update

    UT_IntArray	 	 myActiveVisualizers;	// unique ids of visualizers
						// currently active for look

    UT_Array<GR_Primitive *>
			 myCollectVisPrimitives;

    UT_Array<GUI_GeoRender *> myGeosToRemove;

    class gui_OnionSkinDetail
    {
    public:
	gui_OnionSkinDetail(const GU_DetailHandle &dh)
	    : myDetail(dh), myRender(NULL), myUsedFlag(true)
	    {
		myDetail.addPreserveRequest();
	    }
	~gui_OnionSkinDetail();
	      
	GU_DetailHandle myDetail;
	GUI_GeoRender  *myRender;
	bool		myUsedFlag;
    };
    typedef UT_Map<exint, gui_OnionSkinDetail *> gui_OnionSkins;
    gui_OnionSkins myOnionSkins;

    class gui_OnionSkinLayer
    {
    public:
	gui_OnionSkinLayer(int id, fpreal t, const UT_Matrix4D &mat, bool pre)
	    : myDetailID(id), myTime(t), myRender(NULL),
	      myTransform(mat), myIsPreFrame(pre) {}

	int	       myDetailID;
	fpreal	       myTime;
	GUI_GeoRender *myRender;
	UT_Matrix4D    myTransform;
	bool	       myIsPreFrame;
    };
    UT_Array<gui_OnionSkinLayer *> myOnionSkinLayers;

    class CachedDetailPos {
    public:
	CachedDetailPos()
	    : myPos(0,0,0,1)
	    , myCacheCount(-1)
	    , myUniqueId(-1)
	{}

	CachedDetailPos(const GU_Detail *detail)
	{
	    if(detail)
	    {
		myCacheCount = detail->getMetaCacheCount();
		myUniqueId = detail->getUniqueId();
	    }
	}

	void computePosition(const GU_Detail *detail)
	{
	    if(detail)
	    {
		UT_BoundingBox bbox;
		detail->getBBox(bbox, UT_Matrix4R::getIdentityMatrix());
		
		UT_Vector3 point3 = bbox.center();
		myPos = UT_Vector4F(point3.x(), point3.y(), point3.z(), 1.0);
	    }
	}

	bool operator==(const CachedDetailPos& p)
	{
	    return myCacheCount == p.myCacheCount
		&& myUniqueId   == p.myUniqueId;
	}

	const UT_Vector4F& position() { return myPos; }

    private:
	UT_Vector4F myPos;
	int 	    myCacheCount;
	int 	    myUniqueId;
    };
    // mutable because these are cached positions
    mutable UT_ValArray<CachedDetailPos> myDetailPositions;

    static RE_Light		*theHeadlight;

    static UT_String		 ourChosenDopnetPath;
    static int			 ourChosenDopnetPathLen;
    static bool			 ourPickDisplayListUsedFlag;

    UI_Value			*myRedrawValue;
};

inline int
GUI_DetailLook::getNumDetailHandles() const
{
    return myNullDetailFlag ? 0 : 1;
}

inline OP_Node *
GUI_DetailLook::getObject() const
{
    if(myCachedObject)
	return myCachedObject;
    if(myObjectID != OP_INVALID_NODE_ID)
	return OP_Node::lookupNode(myObjectID);
    return NULL;
}

inline OP_Node *
GUI_DetailLook::getSop() const
{
    if(myCachedSop)
	return myCachedSop;
    if(mySopID != OP_INVALID_NODE_ID)
	return OP_Node::lookupNode(mySopID);
    return NULL;
}

inline OP_Node *
GUI_DetailLook::getShopsRelativeTo() const
{
    OP_Node *node = getObject();
    if(!node)
    {
	node = getSop();
	if(node)
	    return node->getCreator();
    }
    return node;
}

inline int
GUI_DetailLook::getVisualizedNodeId() const
{
    if (mySopID != OP_INVALID_NODE_ID)
	return mySopID;
    else
	return myObjectID;
}

inline bool
GUI_DetailLook::isDetailDisplayOn() const
{
    if(flag.display)
    {
	if((myObjectID == OP_INVALID_NODE_ID || OP_Node::lookupNode(myObjectID))
	   && (mySopID == OP_INVALID_NODE_ID || OP_Node::lookupNode(mySopID)))
	    return true;
	UT_ASSERT_MSG(!"Stale GUI_DetailLook: ", myDetailName);
    }
    return false;
}



#endif
