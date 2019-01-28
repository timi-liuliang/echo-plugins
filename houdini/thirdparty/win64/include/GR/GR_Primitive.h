/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_Primitive.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Base class for all primitives that Houdini is natively capable of
 *	rendering with OpenGL.
 */
#ifndef GR_PRIMITIVE_H
#define GR_PRIMITIVE_H

class RE_Geometry;
class RE_Render;
class GR_GeoRender;
class GR_DisplayOption;
class GR_CommonDispOption;
class GT_Primitive;

#include <UT/UT_Array.h>
#include <UT/UT_String.h>
#include <RE/RE_Types.h>
#include <GEO/GEO_PrimTypeCompat.h>
#include <GT/GT_Types.h>
#include <GT/GT_PrimitiveTypes.h>
#include <GT/GT_Handles.h>
#include <GT/GT_GEOPrimitive.h>
#include <GU/GU_Detail.h>
#include "GR_Defines.h"
#include "GR_DecorationRender.h"
#include "GR_DrawParms.h"
#include "GR_ErrorManager.h"
#include "GR_PickSelection.h"
#include "GR_PickRender.h"
#include "GR_UpdateParms.h"
#include "GR_API.h"

class GA_Primitive;
class GA_PointGroup;
class GA_EdgeGroup;
class GA_PrimitiveGroup;
class OP_Node;
class GT_RefineParms;
class GR_DisplayOption;
class GR_DecorationRender;
class GR_RenderInfo;
class GEO_Primitive;

class GR_API GR_Primitive
{
public:
    virtual ~GR_Primitive();

    /// Return a string version of this class's name
    virtual const char *className() const = 0;

    /// The cache name is assigned to this primitive by the viewport. It can be
    /// used as a base name for multiple objects by adding a suffix, but do not
    /// change the base name. Returns true if the name changed.
    bool		setCacheName(const char *cache_name);

    /// returns true if the given GT primitive can be rendered by this primitive
    /// if t==GT_GEO_PRIMITIVE, geo_type will be the geo type id and prim will
    /// be non-NULL. If t is any other type, ph will be a valid GT primitive.
    /// The primitives themselves can be used for more sophisticated matching.
    virtual GR_PrimAcceptResult	acceptPrimitive(GT_PrimitiveType t,
						int geo_type,
						const GT_PrimitiveHandle &ph,
						const GEO_Primitive *prim) = 0;

    /// This is called if this primitive is collecting multiple GEO or GT
    /// primitives. It will be called just before the viewport starts iterating
    /// through the primitives. The GR_Primitive should clear its list of
    /// collected primitive IDs.
    virtual void		resetPrimitives() {}

    /// The base primitive type is usually NORMAL unless this is a packed type.
    virtual GR_BasePrimType	getBasePrimType() const
				{ return GR_BASE_PRIM_NORMAL; }

    /// Returns the contained instance prim. Will only be valid if
    /// getBasePrimType() is not GR_BASE_PRIM_NORMAL.
    virtual GT_PrimitiveHandle  getInstancePrim() const
				{ return GT_PrimitiveHandle(); }
    
    /// returns true if this primitive's type mask is part of "mask".
    bool		typeMask(GEO_PrimTypeCompat::TypeMask mask) const
			    { return bool(mask & myGAPrimMask); }


    virtual void	checkForDeferredDraw(const RE_Render *r,
					     GR_RenderMode mode,
					     GR_RenderFlags flags,
					     const GR_DrawParms &dp,
					     const UT_Matrix4D &proj_view,
					     const UT_Matrix4D &object,
					     bool &visible,
					     bool &defer);

    enum GR_DispOptChange
    {
	DISPLAY_UNCHANGED,
	DISPLAY_CHANGED,
	DISPLAY_VERSION_CHANGED
    };

    /// Called when the geometry-related display options change, to determine if
    /// an update is required. By default, no update is required. If the options
    /// did change, return either DISPLAY_CHANGED, which will call update()
    /// without bumping the display option version, or DISPLAY_VERSION_CHANGED,
    /// which will bump the version.
    GR_DispOptChange	checkDisplayOptionChange(const GR_DisplayOption &opts);


    /// If this primitive requires an update when the view changes, return true.
    virtual bool	updateOnViewChange(const GR_DisplayOption &) const
			{ return false; }

    /// called every time the metacount version is bumped on the GU_Detail.
    void		updatePrim(RE_Render			 *r,
				   const GT_PrimitiveHandle	 &primh,
				   const GR_UpdateParms		 &parms);

    /// If updateOnViewChange() returns true, this is called when the view
    /// changes.
    virtual void	viewUpdate(RE_Render *r,
				   const GR_ViewUpdateParms &parms)
			{}

    /// Update an array on the specified instance group. If instance group is
    /// -1, array->getInstanceGroup() must specify a valid instance group.
    /// If 'array' is null, it will be removed from the instances but
    /// you must specify the instance_group and attrib_name.
    virtual void	updateInstanceGroup(const RE_VertexArray *array,
					    int instance_group = -1,
					    const char *attrib_name = NULL,
					    RE_GenericAttribID id
					       = RE_GENATTRIB_NONE);

    /// For primitives that may need updating if the GL state changes, this
    /// hook allows you to perform a check if no update is otherwise required.
    /// Return true to have checkGLState() called. Returning true from that
    /// will trigger an update.
    /// @{
    virtual bool	needsGLStateCheck(const GR_DisplayOption &opts) const
			{ return false; }
    virtual bool	checkGLState(RE_Render *r,const GR_DisplayOption &opts)
			{ return false; }
    /// @}

    /// Main render call, which can be called several times in a redraw with
    /// different render modes. Calls render to do the real work specialized
    /// for each primitive type.
    void		draw(RE_Render *r,
			     GR_RenderMode render_mode,
			     GR_RenderFlags render_flags,
			     GR_DrawParms parms);

    /// Picking version of draw(). Calls renderPick to do the real work
    /// specialized for each primitive type.
    int	    		drawPick(RE_Render *r,
				   const GR_DisplayOption *opt,
				   unsigned int pick_type,
				   GR_PickStyle pick_style,
				   bool has_pick_map);

    /// Called just before drawing our first decoration.
    virtual void	setupDrawDecorations(RE_Render *r);
    /// Decoration version of draw(). Calls renderDecoration to do the real
    /// work specialized for each primitive type.
    void		drawDecoration(RE_Render *r,
				       GR_Decoration decor,
				       const GR_DecorationParms &parms);

    /// called once per viewport draw, setup() before any renders are done, and
    /// cleanup() after all are complete. There is a possibility that render()
    /// may not be called at all.
    /// @{
    virtual bool	setup(const GR_DisplayOption *)
			    { return true; }
    virtual void	cleanup(RE_Render *r) {}
    /// @}


    /// return true if the primitive is in or overlaps the view frustum.
    /// always returning true will effectively disable frustum culling.
    virtual bool	inViewFrustum(const UT_Matrix4D &objviewproj)
			    { return true; }

    /// Return a bitfield of what decorations are supported by this primitive.
    /// Set bit (1 << GR_Decoration) if that decoration is supported.
    /// Not all primitives will support all decorations.
    int64		supportedDecorations() { return myDecorMask; }

    /// Return true if this primitive supports drawing into a UV viewport.
    virtual bool	supportsRenderUV();

    /// Return true if this primitive requires an alpha pass.
    virtual bool	requiresAlphaPass() const { return false; }
    
    /// Return true if this should be drawn in the given alpha pass.
    bool		drawInAlphaPass(GR_AlphaPass alpha);

    /// Return true if this primitive renders in the specified alpha pass.
    virtual bool	renderInAlphaPass(GR_AlphaPass a);

    /// Returns the bounding box of this primitive if it can be determined.
    virtual bool	getBoundingBox(UT_BoundingBoxD &bbox) const
			    { return false; }

    /// Return the GA_Primitive corresponding to this primitive.  Some
    /// rendering primitives may be comprised of multiple GA/GU primitives.
    /// The @c id parameter (corresponding to the @c __primitive_id attribute)
    /// can be used to extract a specific primitive.
    virtual const GA_Primitive	*getGAPrimitive(const GU_Detail *gdp,
					exint primitive_id) const;

    /// Find the vertex offset for a given vertex id.  The integer returned
    /// corresponds with the GA_Offset for the given vertex.  If the vertex_id
    /// is invalid, the method will return -1.
    virtual exint		 getGAVertex(const GU_Detail *gdp,
					exint vertex_id) const;

    /// Return the actual or estimated number of GL primitive that this
    /// primitive will be rendering.
    virtual exint		 getNumGLPrimitives(const GR_DisplayOption *);

    /// The GL render version to use for this primitive
    GR_RenderVersion	getRenderVersion() const;

    /// whether to display component selections or not.
    bool		showSelections() const;

    /// Instancing matrix list and version (for the vertex array)
    bool		canDoInstancing() const;

    /// Returns true if the full object is selected at the scene level.
    bool		isObjectSelection() const;

    /// Returns the current display option version for this primitive.
    /// It will only be non-zero if the primitive responds to display option
    /// changes and bumps the display option version.
    int32		getDisplayOptionVersion() const
			{ return myDispOptVersion; }
    
    int32		bumpDisplayOptionVersion()
			{
			    myDispOptVersion++;
			    return myDispOptVersion;
			}

    /// GT Primitive caching. This is handled by updatePrim()
    /// @{
    GT_PrimitiveHandle		getCachedGTPrimitive() { return myCachedPrim; }
    void		        cacheGTPrimitive(const GT_PrimitiveHandle &prim)
					{
					    if(prim)
					    {
						if(prim != myCachedPrim)
						    myCachedPrim = prim;
					    }
					    else
						myCachedPrim = NULL;
					}

    /// @}
    
    /// Called when a node is no longer actively displayed, but still cached
    virtual void		retirePrimitive();
    
    /// return the RE_Geometry containing the geometry to be decorated by user
    /// display options. May return NULL if no user decorations are to be shown.
    virtual RE_Geometry	      *getDecorationGeometry()  { return NULL; }

    /// Only for the Scene Graph view: highlighted prims.
    virtual bool	 isHighlighted() const { return false; }
    
    /// create a primitive for the given GT type, with appropriate cache name
    /// and GL render level
    static GR_Primitive *createPrimitive(GT_PrimitiveType ptype,
					 int   geo_type,
					 const GR_RenderInfo *info,
					 const char *cache_name);

    void		setPrimUsed(bool used) { myPrimUsed = used; }
    bool		isPrimUsed() const     { return myPrimUsed; }

    /// set for those primitives that are within packed primitives
    void		setParentPrimType(GR_BasePrimType type)
				{ myParentPrimType = type; }
    GR_BasePrimType	getParentPrimType() const
				{ return myParentPrimType; }

    const GR_RenderInfo *getRenderInfo() const { return myInfo; }

    virtual void	getGeoRenders(UT_Array<GR_GeoRender *> &renders) {}

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

    /// The base GL cache name assigned to this primitive.
    const char *	getCacheName() const { return myCacheName; }

protected:
    /// Primitive constructor. info and cache_name are assigned by the viewport.
    /// GR_Primitive subclasses can assign a typemask which is used to determine
    /// if the primitive is rendered in instances where only a subset of
    /// primitive types is requested.
			GR_Primitive(const GR_RenderInfo *info,
				     const char *cache_name,
				     GEO_PrimTypeCompat::TypeMask gaprimtype);


    /// Called when the geometry-related display options change, to determine if
    /// an update is required. By default, no update is required. If the options
    /// did change, return either DISPLAY_CHANGED, which will call update()
    /// without bumping the display option version, or DISPLAY_VERSION_CHANGED,
    /// which will bump the version.
    /// If 'first_init' is true, this is being called to initialize any cached
    /// display options. The return value will be ignored. This will happen
    /// just after the primitive is created.
    virtual GR_DispOptChange displayOptionChange(const GR_DisplayOption &opts,
						 bool first_init);

    /// Main update method, called when conditions change which may require the
    /// geometry to be updated. These include geometry changes, selections,
    /// volatile display options (level of detail), instancing updates,
    /// and some GL state changes (if needsGLStateCheck returns true)
    virtual void	update(RE_Render		 *r,
			       const GT_PrimitiveHandle  &primh,
			       const GR_UpdateParms	 &p) = 0;

    /// Main drawing method. update() will always be called before render(), but
    /// render() may execute multiple times before the next update call. This
    /// method should be fairly lightweight; do as much work in update() as
    /// possible. There is no access to the GU_Detail from this method.
    virtual void	render(RE_Render *r,
			       GR_RenderMode render_mode,
			       GR_RenderFlags flags,
			       GR_DrawParms dp) = 0;

    /// Render this primitive for picking, where pick_type is defined as one of
    /// the pickable bits in GU_SelectType.h (like GR_PICK_GEOPOINT)
    /// return the number of picks
    virtual int		renderPick(RE_Render *r,
				   const GR_DisplayOption *opt,
				   unsigned int pick_type,
				   GR_PickStyle pick_style,
				   bool has_pick_map) = 0;

    /// Render the attributes required for decoration 'decor' (like point
    /// markers, uv text, normals, etc). 'overlay' should be set to true if
    /// currently rendering in the overlay texture, false if in the main
    /// viewport.
    virtual void	renderDecoration(RE_Render *r,
					 GR_Decoration decor,
					 const GR_DecorationParms &parms);

    /// Check if the builtin Houdini markers (point numbers, profile curves,
    /// prim normals, etc) have changed since the last redraw, and return true
    /// if at least one did. 'markers' is a list of markers to check that must
    /// be terminated by GR_NO_DECORATION.
    bool		standardMarkersChanged(const GR_DisplayOption &opts,
					       const GR_Decoration *markers,
					       bool first_init);

    /// Updates a decoration for the pass RE_Geometry object. Called from
    /// update().
    void		updateDecoration(RE_Render *r,
				GR_Decoration dec,
				const GR_UpdateParms &p,
				const GT_PrimitiveHandle &prim,
				RE_Geometry *geo);

    /// Helper mother to draw a decoration on a given GR_GeoRender object.
    /// Called from renderDecoration() or render().
    void		drawDecoration(RE_Render *r,
				GR_GeoRender *geo,
				GR_Decoration dec,
				const GR_DisplayOption *opts,
				GR_DecorRenderFlags flags,
				bool overlay,
				bool override_vis,
				int instance_group = -1,
				GR_SelectMode *select_override = NULL,
				GR_DecorationRender::PrimitiveType t
				    = GR_DecorationRender::PRIM_TRIANGLE);
    
    void		 drawVisualizer(RE_Render *r,
				GR_GeoRender *geo,
				const GR_VisualizerInfo *dec_info,
				const GR_DisplayOption *opts,
				GR_DecorRenderFlags flags,
				GR_DecorationRender::PrimitiveType
				    ptype = GR_DecorationRender::PRIM_TRIANGLE);


    /// Helper method to draw a decoration on a given RE_Geometry object.
    /// Called from renderDeocration() or render().
    void		drawDecorationForGeo(RE_Render *r,
				RE_Geometry *geo,
				GR_Decoration dec,
				const GR_DisplayOption *opts,
				GR_DecorRenderFlags flags,
				bool overlay,
				bool override_vis,
				int instance_group,
				GR_SelectMode smode,
				GR_DecorationRender::PrimitiveType t
				    = GR_DecorationRender::PRIM_TRIANGLE,
				RE_OverrideList *override_list = NULL);

    void		 drawVisualizerForGeo(RE_Render *r,
				RE_Geometry *geo,
				const GR_VisualizerInfo *dec_info,
				const GR_DisplayOption *opts,
				GR_DecorRenderFlags flags,
				GR_DecorationRender::PrimitiveType
				    ptype = GR_DecorationRender::PRIM_TRIANGLE);


    /// Helper method to draw a GR_GeoRender object. Called from render() or
    /// renderDecoration().
    void		drawGeoRender(RE_Render *r,
				      GR_GeoRender *geo,
				      GR_RenderMode mode,
				      GR_RenderFlags flags,
				      GR_DrawParms dp);
    bool		 checkUserOptions(const GR_DisplayOption &opt) const;
    void		 updateUserOptions(const GR_DisplayOption &opt);
    const GR_UserOption *getUserDecoration(GR_Decoration dec) const;
    void		 updateUserDecorations(RE_Render *r,
					       const GT_PrimitiveHandle &primh,
					       const GR_UpdateParms &p,
					       RE_Geometry *for_geo = NULL);

    GR_DispOptChange	 checkColorOverride(const GR_DisplayOption &opts,
					    bool first_init);

    GT_Primitive       *createUnitSphere(const GT_RefineParms &parms);

	

    /// Return a GEO_Primitive derivative (GeoPrim) from the GT handle. The
    /// GT handle must be referencing a GT_GEOPrimitive.
    template <class GeoPrim>
    void getGEOPrimFromGT(const GT_PrimitiveHandle &primh,
			  const GeoPrim *&prim)
    {
	const GT_GEOPrimitive *geo =
	    dynamic_cast<const GT_GEOPrimitive*>(primh.get());
	prim = dynamic_cast<const GeoPrim *>(geo->getPrimitive(0));
    }

    /// Create a pick buffer large enough to hold all picks from 'geo' with
    /// 'picktype' selection style (GR_PICK_GEOPOINT, PRIMITIVE, etc)
    /// The required vector size (1 or 2) will be returned in vsize (edges need
    /// 2) and needs_2pass will be true if some picks require CPU support.
    int			getPickBufferSize(RE_Geometry *geo,
					  unsigned int picktype,
					  int &vector_size,
					  bool &needs_2pass,
					  int start_group = 4,
					  int end_group = -1,
					  GR_PickStyle pickstyle
					      = GR_PICK_NONE) const;
    
    /// creates a ivec4 pick buffer with a pickID attribute with buffer_size
    /// points. This primitive owns the buffer, and there is only one.
    RE_Geometry	       *createPickBuffer(RE_Render *r, int buffer_size,
					 int vector_size = 1,
					 bool inconclusive_stream_too = false);

    /// accumulate picks into the myInfo.myPicks array so that it can be passed
    /// up to higher level code. Some effort is made to remove duplicate pickIDs,
    /// mostly from neighbouring picks. If 'pick_buffer' is NULL, myPickBuffer
    /// is used.
    int			accumulatePickIDs(RE_Render *r, int npicks,
					  RE_Geometry *pick_buffer = NULL);

    GR_PickCompMode	setupPrimPickMode(RE_Render *r,GR_GeoRender *geo) const;
					  

    void		setPrimTypeMask( GEO_PrimTypeCompat::TypeMask m)
			    { myGAPrimMask = m; }

    void		setSupportedDecorations(int64 mask)
			{ myDecorMask = mask; }
    
    bool		 myHasDetailPointNormal;
    GR_DecorationRender *myDecorRender;
    const GR_RenderInfo *myInfo;

    // For deferred rendering
    UT_BoundingBox	myBBox;
    UT_Matrix4D		myLocal;
    int			myQueuedDrawID;
    int			myQueuedPointID;
    
private:
    UT_String		 myCacheName;
    UT_Int64Array	 myUserDispOptions;
    int32		 myDispOptVersion;
    GEO_PrimTypeCompat::TypeMask myGAPrimMask;
    UT_String		 myColorOverrideOpt;
    unsigned		 myPrimUsed : 1,
			 myUserDecorationsProcessed : 1,
			 myDispOptInit : 1;
    GR_BasePrimType	 myParentPrimType;

    // for cache access only; do not dereference.
    GT_PrimitiveHandle	 myCachedPrim;

    int			 myMarkerState[GR_MAX_DECORATIONS];//0=off,1=on,-1=undef
    int64		 myDecorMask;

    // uncached buffer for doing transform feedback picking
    RE_Geometry		 *myPickBuffer;

};

inline GR_Primitive::GR_DispOptChange
GR_Primitive::checkDisplayOptionChange(const GR_DisplayOption &opts)
{
    GR_DispOptChange changed = displayOptionChange(opts, !myDispOptInit);

    myDispOptInit = true;

    return changed;
}

inline void
GR_Primitive::updatePrim(RE_Render		  *r,
			 const GT_PrimitiveHandle &primh,
			 const GR_UpdateParms	  &p)
{
    GU_DetailHandleAutoReadLock	rlock(p.geometry);
    myCachedPrim = primh;
    myHasDetailPointNormal =
	rlock.isValid() ? rlock->findPointAttribute("N") != NULL : false;
    myUserDecorationsProcessed = false;

    if(!myDispOptInit)
    {
	displayOptionChange(p.dopts, true);
	myDispOptInit = true;
    }
    
    update(r, primh, p);
    updateUserDecorations(r, primh, p);
}

inline bool
GR_Primitive::drawInAlphaPass(GR_AlphaPass alpha)
{
    if(alpha == GR_ALPHA_PASS_ALL)
	return true;

    return renderInAlphaPass(alpha);
}

inline void
GR_Primitive::updateDecoration(RE_Render *r,
				GR_Decoration dec,
				const GR_UpdateParms &p,
				const GT_PrimitiveHandle &prim,
				RE_Geometry *geo)
{
    // Don't draw decorations for packed primitives
    if (getParentPrimType() == GR_BASE_PRIM_PACKED ||
	getParentPrimType() == GR_BASE_PRIM_INSTANCE ||
	getParentPrimType() == GR_BASE_PRIM_FRAGMENT)
	return;

    myDecorRender->updateDecoration(r, dec, *prim.get(), p, *geo);
}

#endif
