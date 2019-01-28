/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_CommonDispOption.h
 *
 * COMMENTS:	The display options that are common regardless of the geometry
 *		type (display, template, etc.).
 */

#ifndef __GR_CommonDispOption_h__
#define __GR_CommonDispOption_h__

#include "GR_API.h"
#include <SYS/SYS_Math.h>
#include <UT/UT_Color.h>
#include <UT/UT_String.h>
#include <RE/RE_TextureTypes.h>
#include <GEO/GEO_PrimTypeCompat.h>
#include <GU/GU_SelectType.h>
#include "GR_HandleParts.h"
#include "GR_DisplayColors.h"
#include "GR_Defines.h"

class RE_Render;
class GR_SchemeColors;

#define FLAG_ACCESSORS(FLAGS, METHOD, FLAG) \
    bool METHOD() const { return FLAGS.FLAG; } \
    void METHOD(bool onoff) { FLAGS.FLAG = onoff; }

#define FLAG_ACCESSORS_BUMP_OPTS(FLAGS, METHOD, FLAG) \
    bool METHOD() const { return FLAGS.FLAG; } \
    void METHOD(bool onoff) { if(FLAGS.FLAG != onoff) mySerialNum++; \
			      FLAGS.FLAG = onoff; }
 
class GR_API GR_CommonDispOption
{
public:
     GR_CommonDispOption();
     GR_CommonDispOption(const GR_CommonDispOption &opt);
    ~GR_CommonDispOption();

    // Return true if the load/save was successful, and false otherwise.
    bool		 save(std::ostream &, int indent=0) const;
    bool		 load(UT_IStream &);

    GR_CommonDispOption &operator=(const GR_CommonDispOption &opt);

    // The color scheme names are simply the suffixes applied to the
    // default color scheme file 3DSceneColors.  The dot '.' should
    // not be included in the scheme name. 
    void		 setColorScheme(const char *scheme_name);
    const UT_String	&getColorScheme(void) const { return myColorScheme; }
    static void		 reloadAllColorSchemes();

    // Grab the colors and the color scheme from opt:
    void		 setColors(const GR_CommonDispOption &opt);

    bool	 compatibleWith(const GR_CommonDispOption &opt) const;

    // Grab the color from the current pass
    const UT_Color	&color(GR_DisplayColor c) const
	{ return myColors[c][myGammaPassPtr?*myGammaPassPtr:0]; }

    void	 setGammaPassPtr(int *p) { myGammaPassPtr = p; }
    
    void		 setColor(GR_DisplayColor color_name,
				const UT_Color &c);
    void		 setColor(GR_DisplayColor target_color,
				GR_DisplayColor source_color);
    const UT_Color	&getColor(GR_DisplayColor color_name,
				bool gamma = false) const
			 { return myColors[color_name][gamma?1:0]; }
    
    // Color methods (soon to be deprecated; use color(GR_..._COLOR)
#define GET_COL(func, val) \
    const UT_Color &func() const \
    { return myColors[val][myGammaPassPtr?*myGammaPassPtr:0]; }
    
    GET_COL(backgndColor, GR_BACKGROUND_COLOR)
    GET_COL(backgndBottomColor, GR_BACKGROUND_BOTTOM_COLOR)
    GET_COL(defaultWireColor, GR_WIREFRAME_COLOR)
    GET_COL(wireOverColor, GR_WIRE_OVER_COLOR)
    GET_COL(ghostFillColor, GR_GHOST_FILL_COLOR)
    GET_COL(selPrimColor, GR_SELECTED_PRIM_COLOR)
    GET_COL(auxSelPrimColor1, GR_AUX_SELECTED_PRIM1_COLOR)
    GET_COL(auxSelPrimColor2, GR_AUX_SELECTED_PRIM2_COLOR)
    GET_COL(cookSelectionColor, GR_COOK_SELECTION_COLOR)
    GET_COL(primClosureColor, GR_PRIM_CLOSURE_COLOR)
    GET_COL(regAuxColor, GR_AUX_GEO_COLOR)
    GET_COL(selAuxColor, GR_SELECTED_AUX_GEO_COLOR)
    GET_COL(profileColor, GR_PROFILE_WIRE_COLOR)
    GET_COL(axesColor, GR_AXES_COLOR)
    GET_COL(xAxisColor, GR_X_AXIS_COLOR)
    GET_COL(yAxisColor, GR_Y_AXIS_COLOR)
    GET_COL(zAxisColor, GR_Z_AXIS_COLOR)
    GET_COL(selAxesColor, GR_SELECTED_AXES_COLOR)
    GET_COL(primInfoColor, GR_PRIM_INFO_COLOR)
    GET_COL(selPrimInfoColor, GR_SELECTED_PRIM_INFO_COLOR)
    GET_COL(pointColor, GR_POINT_COLOR)
    GET_COL(selPointColor, GR_SELECTED_POINT_COLOR)
    GET_COL(guide1Color, GR_GUIDE1_COLOR)
    GET_COL(guide2Color, GR_GUIDE2_COLOR)
    GET_COL(guideSelColor, GR_OBJECT_SELECT_COLOR)
    GET_COL(cplaneColor, GR_CPLANE_COLOR)
    GET_COL(activeCplaneColor, GR_ACTIVE_CPLANE_COLOR)
    GET_COL(selCplaneColor,  GR_SELECTED_CPLANE_COLOR)
    GET_COL(gridColor, GR_GRID_COLOR)
    GET_COL(heightVectorColor, GR_HEIGHT_VECTOR_COLOR)
    GET_COL(gridXRulerColor, GR_GRID_X_RULER_COLOR)
    GET_COL(gridYRulerColor, GR_GRID_Y_RULER_COLOR)
    GET_COL(gridZRulerColor, GR_GRID_Z_RULER_COLOR)
    GET_COL(handleColor, GR_HANDLE_COLOR)
    GET_COL(pickedHandleColor, GR_PICKED_HANDLE_COLOR)
    GET_COL(selHandleColor, GR_SELECTED_HANDLE_COLOR)
    GET_COL(peanutColor, GR_PEANUT_COLOR)
    GET_COL(labelColor, GR_LABEL_COLOR)
    GET_COL(pillTextColor, GR_PILL_TEXT_COLOR)
    GET_COL(pillBGColor, GR_PILL_BG_COLOR)
    GET_COL(pillOutlineColor, GR_PILL_OUTLINE_COLOR)
    GET_COL(snapshotColor, GR_SNAPSHOT_COLOR)
    GET_COL(selLabelColor, GR_SELECTED_LABEL_COLOR)
    GET_COL(selPillTextColor, GR_SELECTED_PILL_TEXT_COLOR)
    GET_COL(selPillBGColor, GR_SELECTED_PILL_BG_COLOR)
    GET_COL(selPillOutlineColor, GR_SELECTED_PILL_OUTLINE_COLOR)
    GET_COL(overlayLabelColor, GR_OVERLAY_LABEL_COLOR)
    GET_COL(auxOverlayLabelColor, GR_AUX_OVERLAY_LABEL_COLOR)
    GET_COL(handlePivotColor, GR_HANDLE_PIVOT_COLOR)
    GET_COL(handleAxisColor, GR_HANDLE_AXIS_COLOR)
    GET_COL(handlePlaneAxisColor, GR_HANDLE_PLANE_AXIS_COLOR)
    GET_COL(handleTumbleSphereColor, GR_HANDLE_TUMBLE_SPHERE_COLOR)
    GET_COL(handleTracerColor, GR_HANDLE_TRACER_COLOR)
    GET_COL(handleDeltaTextColor, GR_HANDLE_DELTA_TEXT_COLOR)
    GET_COL(handleDeltaTextShadowColor, GR_HANDLE_DELTA_TEXT_SHADOW_COLOR)
    GET_COL(cplaneHandleXlateColor, GR_CPLANE_HANDLE_XLATE_COLOR)
    GET_COL(cplaneHandleRotateColor, GR_CPLANE_HANDLE_ROTATE_COLOR)
    GET_COL(cplaneHandleBorderColor, GR_CPLANE_HANDLE_BORDER_COLOR)
    GET_COL(orthoGridColor, GR_ORTHO_GRID_COLOR)
    GET_COL(orthoGridOriginColor, GR_ORTHO_GRID_ORIGIN_COLOR)
    GET_COL(uvGridColor, GR_UV_GRID_COLOR)
    GET_COL(uvGridOriginColor, GR_UV_GRID_ORIGIN_COLOR)
    GET_COL(uvGridBoundaryColor, GR_UV_GRID_BOUNDARY_COLOR)
    GET_COL(objSelectColor, GR_OBJECT_SELECT_COLOR)
    GET_COL(objCurrentColor, GR_OBJECT_CURRENT_COLOR)
    GET_COL(objSelectHintColor, GR_OBJECT_SELECT_HINT_COLOR)
    GET_COL(objGhostColor, GR_OBJECT_GHOST_COLOR)
    GET_COL(uvBackFaceFillColor, GR_UV_BACKFACE_FILL_COLOR)
    GET_COL(uvOverlapFillColor, GR_UV_OVERLAP_FILL_COLOR)
    GET_COL(uvSelBackFaceFillColor, GR_SELECTED_UV_BACKFACE_FILL_COLOR)
    GET_COL(ikCriticalZoneColor, GR_IK_CRITICAL_ZONE_COLOR)
    GET_COL(viewMaskOverlayColor, GR_VIEW_MASK_OVERLAY_COLOR)
    GET_COL(textColor, GR_TEXT_COLOR)
    GET_COL(textShadowColor, GR_TEXT_SHADOW_COLOR)
    GET_COL(geoWarningColor, GR_GEO_WARNING_COLOR)
    GET_COL(selectBoxOuterLineColor, GR_SELECTBOX_OUTER_LINE_COLOR)
    GET_COL(selectBoxInnerLineColor, GR_SELECTBOX_INNER_LINE_COLOR)
    GET_COL(selectBoxFillColor, GR_SELECTBOX_FILL_COLOR)
    GET_COL(handleLocateColor, GR_HANDLE_LOCATE_COLOR)
    GET_COL(handleLocatePassiveColor, GR_HANDLE_LOCATE_PASSIVE_COLOR)
    GET_COL(objectNameColor, GR_OBJECT_NAME_COLOR)
    GET_COL(pointTrailColor, GR_POINT_TRAIL_COLOR)
    GET_COL(selPointTrailColor, GR_POINT_TRAIL_SELECTED_COLOR)
    GET_COL(renderBorderColor, GR_RENDER_BORDER_COLOR)
    GET_COL(renderInactiveBorderColor, GR_RENDER_INACTIVE_BORDER_COLOR)
    GET_COL(renderTextColor, GR_RENDER_TEXT_COLOR)
    GET_COL(renderLocateColor, GR_RENDER_LOCATE_COLOR)
    GET_COL(renderBarColor, GR_RENDER_BAR_COLOR)
    GET_COL(flattenPinColor, GR_FLATTEN_PIN_COLOR)
    GET_COL(flattenPinSelColor, GR_FLATTEN_PIN_SELECTED_COLOR)
    GET_COL(flattenInitialPinColor, GR_FLATTEN_INITIAL_PIN_COLOR)
    GET_COL(messageColor, GR_MESSAGE_COLOR)
    GET_COL(promptColor, GR_PROMPT_COLOR)
    GET_COL(warningColor, GR_WARNING_COLOR)
    GET_COL(errorColor, GR_ERROR_COLOR)
    GET_COL(successColor, GR_SUCCESS_COLOR)
    GET_COL(overlayBackgroundColor, GR_OVERLAY_BACKGROUND_COLOR)
    GET_COL(boundaryColor, GR_BOUNDARY_COLOR)
    GET_COL(edgeTipColor, GR_EDGE_TIP_COLOR)
    GET_COL(viewPivotFGColor, GR_VIEW_PIVOT_FG_COLOR)
    GET_COL(viewPivotBGColor, GR_VIEW_PIVOT_BG_COLOR)
    GET_COL(bboxOutlineColor, GR_BBOX_OUTLINE_COLOR)
    GET_COL(bboxSolidColor, GR_BBOX_SOLID_COLOR)
    GET_COL(backfaceTint, GR_BACKFACE_TINT_COLOR)

    const UT_Color &wireColor() const
	{ return myCurrentWireColor[myGammaPassPtr?*myGammaPassPtr:0]; }
    const UT_Color &crtAuxColor() const
	{ return myColors[myCrtAuxColor][myGammaPassPtr?*myGammaPassPtr:0]; }
    
    const UT_Color &locateColor(fpreal &alpha) const
	{
	    alpha = myAlphas[GR_LOCATE_ALPHA];
	    return myColors[GR_LOCATE_COLOR][myGammaPassPtr?*myGammaPassPtr:0];
	}
    void componentLocateColor(int component_type,
			      GU_SelectionRule sel_rule,
			      UT_Color &clr, fpreal &alpha) const;
    void componentLocateNearColor(UT_Color &clr, fpreal &alpha) const;

#undef GET_COL
#define GET_ALPHA(func, val) fpreal func() const { return myAlphas[val]; }
    GET_ALPHA(objGhostAlpha, GR_OBJECT_GHOST_ALPHA)
    GET_ALPHA(selectBoxFillAlpha, GR_SELECTBOX_ALPHA)
    GET_ALPHA(cplaneAlpha, GR_CPLANE_ALPHA)
    GET_ALPHA(cplaneLineAlpha, GR_CPLANE_LINE_ALPHA)
    GET_ALPHA(cplaneLineFadedAlpha, GR_CPLANE_LINE_FADED_ALPHA)
    GET_ALPHA(gridLineAlpha, GR_GRID_LINE_ALPHA)
    GET_ALPHA(gridLineFadedAlpha, GR_GRID_LINE_FADED_ALPHA)
    GET_ALPHA(handleLocateAlpha, GR_HANDLE_LOCATE_ALPHA)
    GET_ALPHA(handleAxisLocateAlpha, GR_HANDLE_AXIS_LOCATE_ALPHA)
    GET_ALPHA(handleTumbleSphereAlpha, GR_HANDLE_TUMBLE_SPHERE_ALPHA)
    GET_ALPHA(handleTracerFillAlpha, GR_HANDLE_TRACER_FILL_ALPHA);
    GET_ALPHA(fillSelectionAlpha, GR_FILL_SELECTION_ALPHA)
    GET_ALPHA(fillSelectionWireOverAlpha, GR_FILL_SELECTION_WIRE_OVER_ALPHA)
    GET_ALPHA(uvFillAlpha, GR_UV_FILL_ALPHA)
    GET_ALPHA(uvOverlapFillAlpha, GR_UV_OVERLAP_FILL_ALPHA)
    GET_ALPHA(overlayBackgroundAlpha, GR_OVERLAY_BACKGROUND_ALPHA)
    GET_ALPHA(pillMenuBGAlpha, GR_PILL_MENU_BG_ALPHA)
    GET_ALPHA(selPillMenuBGAlpha, GR_SELECTED_PILL_MENU_BG_ALPHA)
    GET_ALPHA(bboxOutlineAlpha, GR_BBOX_OUTLINE_ALPHA)
    GET_ALPHA(bboxSolidAlpha, GR_BBOX_SOLID_ALPHA)
    GET_ALPHA(backfaceTintBlend, GR_BACKFACE_TINT_ALPHA)
#undef GET_ALPHA
    
    const GR_HandleParts	&handleParts() const { return myHandleParts; }
          GR_HandleParts	&handleParts()	     { return myHandleParts; }

    GR_DecorFontSize    getDecorFontSize() const { return myDecorFontSize; }
    void		setDecorFontSize(GR_DecorFontSize s){myDecorFontSize=s;}

    fpreal		markerSize() const { return myMarkerSize*myDPIScale; }
    fpreal		getMarkerSize() const
			    { return myMarkerSize; }
    void		setMarkerSize(fpreal size)
			    { myMarkerSize = SYSmax(size, 0.1); }

    // Methods for accessing (setting/getting) the various flags.
    FLAG_ACCESSORS_BUMP_OPTS(mySaveFlags, drawTexture, drawTexture)
    FLAG_ACCESSORS_BUMP_OPTS(myNoSaveFlags, drawMultiTexture, drawMultiTexture)
    FLAG_ACCESSORS(mySaveFlags, drawTransparent, drawTransparent)
    FLAG_ACCESSORS(mySaveFlags, drawDisplacement, drawDisplacement)
    FLAG_ACCESSORS(mySaveFlags, drawTexturedSpots, drawTexturedSpots)
    FLAG_ACCESSORS(mySaveFlags, drawShadows, drawShadows)
    FLAG_ACCESSORS(mySaveFlags, drawTransparentMaps, drawTransparentMaps)
    FLAG_ACCESSORS(mySaveFlags, highQualityLighting, highQualityLighting)
    FLAG_ACCESSORS(mySaveFlags, drawDiffuse, drawDiffuse)
    FLAG_ACCESSORS(mySaveFlags, drawAmbient, drawAmbient)
    FLAG_ACCESSORS(mySaveFlags, drawEmission, drawEmission)
    FLAG_ACCESSORS(mySaveFlags, useLighting, useLighting)
    FLAG_ACCESSORS(mySaveFlags, ambientOcclusion, ambientOcclusion)
    FLAG_ACCESSORS(mySaveFlags, removeBackface, removeBackface)
    FLAG_ACCESSORS(mySaveFlags, fillSelection, fillSelection)
    FLAG_ACCESSORS(mySaveFlags, solidOnly, solidOnly)
    FLAG_ACCESSORS(mySaveFlags, correctBG, correctBG)
    FLAG_ACCESSORS_BUMP_OPTS(mySaveFlags, polygonConvexQuality,
			     polygonConvexQuality)
    FLAG_ACCESSORS_BUMP_OPTS(mySaveFlags, drawPartSprite, drawPartSprite)
    FLAG_ACCESSORS_BUMP_OPTS(mySaveFlags, orientDiscs, orientDiscs)

    FLAG_ACCESSORS_BUMP_OPTS(mySaveFlags, shadeOpenCurves, shadeOpenCurves)
    FLAG_ACCESSORS(mySaveFlags, displayAsSubdiv, displayAsSubdiv)

    FLAG_ACCESSORS_BUMP_OPTS(mySaveFlags, drawHullsOnly, drawHullsOnly)
    FLAG_ACCESSORS_BUMP_OPTS(mySaveFlags, drawPartOrigin, drawPartOrigin)
    FLAG_ACCESSORS(mySaveFlags, drawHandles, drawHandles)
    FLAG_ACCESSORS_BUMP_OPTS(mySaveFlags, showObjectSelection,
			     showObjectSelection)

    FLAG_ACCESSORS(mySaveFlags, use2DLimitsFor3D, use2DTexLimits)
    FLAG_ACCESSORS(mySaveFlags, dynamicTexControl, dynamicTexControl)
    FLAG_ACCESSORS(mySaveFlags, dynamicReduceTex2D, reduceTex2D )
    FLAG_ACCESSORS(mySaveFlags, dynamicReduceTex3D, reduceTex3D )
    FLAG_ACCESSORS(mySaveFlags, dynamicReduceMipMap, reduceMipMap )
    FLAG_ACCESSORS(mySaveFlags, dynamicReduceBitDepth, reduceBitDepth )

    FLAG_ACCESSORS_BUMP_OPTS(mySaveFlags, overrideColorAttrib,
			     overrideColorAttrib);
    FLAG_ACCESSORS(mySaveFlags, pickWireAsSolid, pickWireAsSolid)
    FLAG_ACCESSORS(mySaveFlags, wireOverPacked, wireOverPacked)
    FLAG_ACCESSORS(mySaveFlags, showGeoColor, showGeoColor )

    FLAG_ACCESSORS(myNoSaveFlags, applyVisualizers, applyVisualizers)
    FLAG_ACCESSORS(myNoSaveFlags, snapToSelected, snapToSelected)
    FLAG_ACCESSORS(myNoSaveFlags, showComponentSelections, showComponentSelections)

    // NB: This flag is somewhat overloaded.  At SOPs it determines whether
    //     we're showing the temporary or cook selections.  At DOPs it also
    //     indicates whether we're drawing temporary DETAIL selections.
    FLAG_ACCESSORS(myNoSaveFlags, showTempSelections, showTempSelections)

    FLAG_ACCESSORS(myNoSaveFlags, pickFrontFaces, pickFrontFaces)
    FLAG_ACCESSORS(myNoSaveFlags, pickBackFaces, pickBackFaces)
    FLAG_ACCESSORS_BUMP_OPTS(myNoSaveFlags, drawAuxColors, drawAuxColors)
    FLAG_ACCESSORS_BUMP_OPTS(myNoSaveFlags, forceWireColor, forceWireColor)
    FLAG_ACCESSORS(myNoSaveFlags, shadingOpenCurves, shadingOpenCurves)

    // This returns true if the interactive mode display option is on
    FLAG_ACCESSORS(mySaveFlags, interactiveEnable, interactiveEnable)
	
    // This returns true if the user is interactively moving a handle or
    // tumbling, independent of the interative enable flag.
    FLAG_ACCESSORS(myNoSaveFlags, interactiveMove, interactive)

    FLAG_ACCESSORS_BUMP_OPTS(mySaveFlags, pointInstancing, pointInstancing)

    FLAG_ACCESSORS(mySaveFlags, cuspNormals, cuspNormals)
    fpreal getNormalCuspAngle() const
		 { return myNormalCuspAngle; }
    void   setNormalCuspAngle(fpreal a)
		 { myNormalCuspAngle = SYSclamp(a, 0.0, 180.0); }
    int	   getVertexNormalLimit() const
		 { return myVertNormalLimit; }
    void   setVertexNormalLimit(int poly_limit)
		 { myVertNormalLimit = SYSmax(0, poly_limit); }

    
    bool limitTextureSize() const { return mySaveFlags.limitTextureSize; }
    void limitTextureSize(bool ls){ mySaveFlags.limitTextureSize = ls;
				    myUpdates.texSize = true; }
    
    bool limitTexture3DSize() const { return mySaveFlags.limitTextureSize; }
    void limitTexture3DSize(bool ls){ mySaveFlags.limitTextureSize = ls;
				      myUpdates.tex3DSize = true; }

    int  textureMemLimitMB() const { return myTextureMemLimitMB; }
    void textureMemLimitMB(int max_size)
			    { myTextureMemLimitMB = SYSmax(32, max_size);
				myUpdates.textureMem = true; }

    // Specifies the anisotropic sampling for textures. getTextureAnisotropy()
    // must be called when rendering, to check for the GL extension.
    int getTextureAnisotropy(RE_Render *r) const;
    int textureAnisotropy() const;
    void textureAnisotropy(int ta);
    
    // Always use the version of getHDRTextureFormat() with RE_Render * when
    // rendering, to ensure hardware compatibility with the format.
    RE_TextureCompress getHDRTextureFormat(RE_Render *r) const;
    int getHDRTextureFormat() const;
    void setHDRTextureFormat(int hdrformat); // 2 = full HDR, 1=16b HDR, 0=8b
    void setDynamicFormat(int format);	     // -1 = compressed 8b
    int getDynamicFormat() const { return myDynamicFormat; }
					     

    RE_TextureCompress getHDRTexture3DFormat(RE_Render *r) const;
    int getHDRTexture3DFormat() const;
    void setHDRTexture3DFormat(int hdrformat); // 2 = full HDR, 1=16b HDR, 0=8b
    void setDynamic3DFormat(int format);
    int getDynamic3DFormat() const { return myDynamic3DFormat; }
    
    // Scale all textures down
    void  setTextureScale(float sc);	// 0.0 (min size) to 1.0 (full size)
    float getTextureScale() const;	// texture scale is applied before limit
    float getUserTextureScale() const;  // texture scale w/o dynamics applied
    void  setDynamicScale(float sc);	// For dynamic cache control
    float getDynamicScale() const;

    void  setTexture3DScale(float sc);	// 0.0 (min size) to 1.0 (full size)
    float getTexture3DScale() const;	// texture scale is applied before limit
    float getUserTexture3DScale() const;// texture scale w/o dynamics applied
    void  setDynamic3DScale(float sc);	// For dynamic cache control
    float getDynamic3DScale() const;

    // Maximum particle array texture size
    void setParticleTextureSize(int max_w, int max_h);
    void getParticleTextureSize(int &max_w, int &max_h) const;

    void setParticleDisplayType(GR_ParticleType t);
    GR_ParticleType particleDisplayType() const;

    // Limit texture resolution
    void setMaxTextureSize(bool limit, int max_w=0, int max_h=0);
    void getMaxTextureSize(int &max_w, int &max_h) const;
    void getMaxTextureSize(RE_Render *r, int &max_w, int &max_h) const;

    void setMaxTexture3DSize(bool limit, int max_w=0, int max_h=0, int max_d=0);
    void getMaxTexture3DSize(int &max_w, int &max_h, int &max_d) const;
    void getMaxTexture3DSize(RE_Render *r, int &max_w, int &max_h,
					   int &max_d) const;

    bool mipMapTextures() const;
    void mipMapTextures(bool onoff);
    void setDynamicMipMap(bool onoff);
    bool getDynamicMipMap() const { return myNoSaveFlags.dynamicMipSetting; }

    void resetDynamicParms();
    
    bool getHighQualityLighting(RE_Render *r) const;

    void alphaPass(GR_AlphaPass pass) { myAlphaPass = pass; }
    GR_AlphaPass alphaPass() const { return myAlphaPass; }

    int	 getTransparentQuality() const { return myTransparentQuality; }
    void setTransparentQuality(int q)  { myTransparentQuality=SYSclamp(q,-1,2); }

    fpreal getDisplacementQuality() const { return myDisplacementQuality; }
    void setDisplacementQuality(fpreal q)
				{ myDisplacementQuality=SYSclamp(q,0.0,10.0); }

    int	 getLightSampling() const      { return myLightSamples; }
    void setLightSampling(int s)       { myLightSamples = SYSclamp(s,1,100); }

    int	 getVolumeQuality() const      { return myVolumeQuality; }
    void setVolumeQuality(int q)       { myVolumeQuality = SYSclamp(q,0,3); }

    bool volumeWireAsPoints() const { return mySaveFlags.volumeWireAsPoints;}
    void volumeWireAsPoints(bool vw){ mySaveFlags.volumeWireAsPoints = vw; }

    // DPI support.
    void   DPIScale(fpreal f) { myDPIScale = f; }
    fpreal DPIScale() const   { return myDPIScale; }

    void autoDPIScale(bool dpi);
    bool autoDPIScale() const { return mySaveFlags.autoDPIScale; }

    // Reflection cube maps
    bool	 useReflectionMaps() const
				{ return mySaveFlags.enableReflectionMaps; }
    void	 useReflectionMaps(bool e)
				{ mySaveFlags.enableReflectionMaps = e; }
    bool	 hdrReflectionMaps() const
				{ return mySaveFlags.hdrReflectionMaps; }
    void	 hdrReflectionMaps(bool e)
				{ myUpdates.hdrReflect = true;
				  mySaveFlags.hdrReflectionMaps = e; }
    int		 reflectionMapSize() const { return myReflectMapSize; }
    void	 reflectionMapSize(int wh)
				{ myUpdates.reflectSize = true;
				  myReflectMapSize = SYSclamp(wh, 16, 16384); }
    fpreal	 reflectionMinLevel() const { return myReflectMinLevel; }
    void	 reflectionMinLevel(fpreal lvl)
				{ myReflectMinLevel = SYSclamp(lvl, 0.0, 1.0); }
   
    // Agents
    FLAG_ACCESSORS_BUMP_OPTS(mySaveFlags, agentLODEnable, agentLODEnable)

    void   setAgentLODBias(fpreal b)
	    {
		if(!SYSisEqual(b, myAgentLODBias))
		    mySerialNum++;
		myAgentLODBias = b;
	    }
    fpreal getAgentLODBias() const { return myAgentLODBias; }

    void   setAgentMinPoints(int minp) { myAgentMinPoints = SYSmax(1, minp); }
    int    getAgentMinPoints() const { return myAgentMinPoints; }

    void   setAgentNumLODLevels(int lvls) { myAgentMaxLOD=SYSclamp(lvls,1,16);}
    int    getAgentNumLODLevels() const { return myAgentMaxLOD; }

    void   setAgentBaseLODLevel(int lvl)
	    {
		if(lvl != myAgentBaseLOD)
		    mySerialNum++;
		myAgentBaseLOD=SYSclamp(lvl,0,16);
	    }
    int    getAgentBaseLODLevel() const { return myAgentBaseLOD; }

    void   setAgentBoneDeform(int d) { myAgentBoneDeform = SYSclamp(d,0,2); }
    int	   getAgentBoneDeform() const { return myAgentBoneDeform; }

    int	   getAgentWireMode() const    { return myAgentWireMode; }
    void   setAgentWireMode(int mode)
		{
		    mode = SYSclamp(mode,0,1);
		    if(mode != myAgentWireMode)
		    {
			mySerialNum++;
			myAgentWireMode =  mode;
		    }
		}

    void   optimizePackedGeometry(bool opt){ mySaveFlags.optPackedGeo = opt; }
    bool   optimizePackedGeometry() const { return mySaveFlags.optPackedGeo; }

    void   offsetVertexDecor(bool use_offset)
				{ mySaveFlags.offsetVertexDecor = use_offset; }
    bool   offsetVertexDecor() const
				{ return mySaveFlags.offsetVertexDecor; }

    enum gr_BBoxMode
    {
	NO_BBOX,
	WIRE_BBOX,
	SHADED_BBOX,
	CURRENT_SHADING_BBOX // follow the current shading mode
    };
    void   shadedBoundingBoxes(gr_BBoxMode m) { myBBoxMode = m; }
    gr_BBoxMode shadedBoundingBoxes() const     { return myBBoxMode; }

    // "Get" methods:
    const UT_String &
		 visibleObjects()    const { return myVisibleObjects; }
    GA_PrimCompat::TypeMask  primMask() const { return myPrimMask; }
    unsigned     pickMask()	     const { return myPickMask; }
    float	 normalScale()	     const { return myNormalScale; }
    float	 getNormalScale()    const { return myNormalScale; }
    fpreal	 vectorScale()	     const { return myVectorScale; }
    fpreal	 getVectorScale()    const { return myVectorScale; }
    float	 LOD()		     const { return myLOD; }
    float	 hlOffsetVariable()  const { return myHLOffsetVariable; }
    float	 hlOffsetConstant()  const { return myHLOffsetConstant; }
    float	 wireWidth()	     const { return SYSmax(0.1,
						myWireWidth*myDPIScale); }
    float	 selectionWireWidth()const { return (wireWidth() > 1.0)
						? wireWidth() + 1.0
						: wireWidth() * 2.0; }
    float	 getWireWidth()	     const { return myWireWidth; }
    float	 wireBlend()	     const { return SYSclamp(myWireBlend,
						0.0, 1.0); }
    float	 interiorWireAlpha() const { return myInteriorWireAlpha; }
    float	 pointSize()	     const { return myPointSize*myDPIScale; }
    float	 getPointSize()	     const { return myPointSize; }
    float	 discSize()	     const { return myDiscSize; }
    float	 shadowOffsetVariable() const { return myShadowOffsetVariable; }
    float	 shadowOffsetConstant() const { return myShadowOffsetConstant; }
    int		 shadowAA() const	   { return myShadowAA; }
    bool	 useLightShadowSize() const { return myUseLightShadowSize; }
    int		 getShadowMapSize() const   { return myShadowMapSize; } 
    int		 selectionListInst() const { return mySelectionListInst; }

    const char	*colorAttrib() const { return myColorAttrib; }
    int		 colorType() const { return myColorType; }
    void	 getColorDomain(fpreal &s, fpreal &e) const
			{  s = myColorDomain[0];
			   e = myColorDomain[1]; }

    int		 getSceneAntialias() const { return mySceneAntialias; }
    bool	 getHDRRendering() const   { return mySaveFlags.hdrRender; }
    float	 getViewGamma() const	   { return myViewGamma; }
    bool	 useViewLUT() const	   { return myUseLUT; }
    const char  *getViewLUT() const	   { return myViewLUT; }
    bool	 useOCIO() const	   { return myUseOCIO; }
    const char  *getOCIOColorspace() const { return myOCIOColorspace; }
    const char  *getOCIOLooks() const	   { return myOCIOLooks; }
    float	 getViewBlack() const	   { return myViewBlack; }
    float	 getViewWhite() const	   { return myViewWhite; }
    int		 getViewComponents() const { return myViewComponents; }
    
    int		 getAmbOcclusionLevel() const { return myAmbOcclusionLevel; }

    fpreal	 getInstancingFraction() const { return myInstancingFraction; }
    int		 getInstancingLimit() const    { return myInstancingLimit; }
    int		 getInstancingLimitAction() const
					    { return myInstancingLimitAction; }
    
    fpreal	 getMinHomeZoom() const	       { return myMinHomeZoom; }

    // In millions of polygons (eg, 10 for 10M).
    int		 getSubdivisionPolyLimit() const { return mySubdPolyLimit; }

    // "Set" methods:
    void	 visibleObjects(const char *mask)
					     { myVisibleObjects.harden(mask); }
    void	 primMask(GA_PrimCompat::TypeMask mask)
					     { myPrimMask = mask; }
    void	 pickMask(unsigned mask)     { myPickMask = mask; }
    void	 setNormalScale(float s);
    void	 setVectorScale(float s);

    void	 useRegularAuxColor() { myCrtAuxColor=GR_AUX_GEO_COLOR; }
    void	 useSelectedAuxColor(){ myCrtAuxColor=GR_SELECTED_AUX_GEO_COLOR;}
    
    void	 setCurrentWireColor(const UT_Color &color);
    void	 setCurrentWireColor(GR_DisplayColor color);
    const UT_Color &getCurrentWireColor() const { return myCurrentWireColor[0]; }
    
    void	 LOD(float l);
    void	 hlOffsetVariable(float f)   { myHLOffsetVariable = (f>=0.0f)
							     ? f :0.0f;}
    void	 hlOffsetConstant(float f)   { myHLOffsetConstant = (f>=0.0f)
							     ? f :0.0f;}
    void	 setWireWidth(float width)   { myWireWidth   = width;}
    void	 wireBlend(float blend)	     { myWireBlend   = blend;}
    void	 interiorWireAlpha(float a)  { myInteriorWireAlpha
						   = SYSclamp(a, 0.0f, 1.0f); }
    void	 setPointSize(float size)    { myPointSize   = size; }
    void	 discSize(float size)	     { myDiscSize   = size; }
    void	 shadowOffsetVariable(float f)
			{ myShadowOffsetVariable = (f>=0.0f) ? f : 0.0f;}
    void	 shadowOffsetConstant(float f)
			{ myShadowOffsetConstant = (f>=0.0f) ? f : 0.0f;}
    void	 shadowAA(int aa)	     { myShadowAA = SYSclamp(aa, 1,4); }
    void	 selectionListInst(int inst) { mySelectionListInst = inst; }
    void	 setUseLightShadowSize(bool e) { myUseLightShadowSize = e; }
    void	 setShadowMapSize(int s)     { myShadowMapSize = s;
						myUpdates.shadowSize = true; }

    void	 setColorAttrib(const char *a);
    void	 setColorType(int t);
    void	 setColorDomain(fpreal s, fpreal e);

    void	 setSceneAntialias(int a)  { mySceneAntialias = SYSmax(a,0);
					     myUpdates.sceneAA = true; }

    void	 setHDRRendering(bool hdr) { mySaveFlags.hdrRender = hdr; }

    void	 setViewGamma(float gamma);
    void	 setUseViewLUT(bool b)	   { myUseLUT = b; }
    void	 setViewLUT(const char *lut) { myViewLUT.harden(lut); }
    void	 useOCIO(bool use)	   { myUseOCIO = use; }
    void	 setOCIOColorspace(const char *cs)
				       { myOCIOColorspace.harden(cs); }
    void	 setOCIOLooks(const char *looks)
				       { myOCIOLooks.harden(looks); }
    void	 setViewBlack(float b) { myViewBlack = b; }
    void	 setViewWhite(float w) { myViewWhite = w; }
    void	 setViewComponents(int c) { myViewComponents = c; }
    void	 setAmbOcclusionLevel(int lvl)
		 { myAmbOcclusionLevel = SYSclamp(lvl, 1, 4); }
    
    void	 setInstancingFraction(fpreal s)
		 { if(!SYSisEqual(s, myInstancingFraction))
			mySerialNum++;
		     myInstancingFraction = s; }
    void	 setInstancingLimit(int limit)
		 { limit = SYSmax(0, limit);
		     if(!SYSisEqual(limit, myInstancingLimit))
			 mySerialNum++;
		     myInstancingLimit = limit;
		 }
    void	 setInstancingLimitAction(int action)
		 { myInstancingLimitAction = SYSclamp(action, 0,2); }
    
    void	 setMinHomeZoom(fpreal val)
		 { myMinHomeZoom = SYSmax(val, 0.01); }

    // In millions of polygons (eg, 10 for 10M).
    void	 setSubdivisionPolyLimit(int limit)
		 {
		     limit = SYSmax(0, limit);
		     if(limit != mySubdPolyLimit)
			 mySerialNum++;
		     mySubdPolyLimit = limit;
		 }
    
    void	 setUVDisplayAttrib(const UT_StringHolder &attrib)
		 {
		     if(myUVAttrib != attrib)
		     {
			 mySerialNum++;
			 myUVAttrib = attrib;
		     }
		 }
    const UT_StringHolder &getUVDisplayAttrib() const { return myUVAttrib; }
    
    // Onion skinning
    bool	isOnionSkinEnabled() const
			{ return mySaveFlags.onionSkinEnable; }
    int		getOnionSkinFrameIncrement() const
			{ return myOnionSkinFrameInc; }
    int		getOnionSkinPreFrameCount() const
			{ return myOnionSkinPreFrameCount; }
    int		getOnionSkinPostFrameCount() const
			{ return myOnionSkinPostFrameCount; }
    const UT_Color &getOnionSkinPreFrameColor() const
			{ return myOnionSkinPreFrameColor; }
    const UT_Color &getOnionSkinPostFrameColor() const
			{ return myOnionSkinPostFrameColor; }
    fpreal	getOnionSkinAlpha() const { return myOnionSkinAlpha; }

    void	setOnionSkinEnable(bool enable)
			{ mySaveFlags.onionSkinEnable = enable; }
	
    void	setOnionSkinParms(int frame_inc,
				  int pre_count,
				  int post_count,
				  UT_Color pre_color,
				  UT_Color post_color,
				  fpreal alpha);

    // These are used to determine if geometry-related parms have changed on a
    // very coarse level.
    int		 getUniqueID() const	     { return myUniqueID; }
    int		 getSerialNum() const	     { return mySerialNum; }

    // changes when the view is changed. ID changes if the rendered viewport
    // is different.
    int		 getViewSerialNum() const    { return mySerialView; }
    int		 getViewSerialID() const    { return mySerialID; }
    void	 setViewSerialNum(int s, int id)
			{
			    mySerialView = s;
			    mySerialID = id;
			}

    bool	 defaultTextureSize() const  { return !myUpdates.texSize; }
    bool	 default3DTextureSize() const{ return !myUpdates.tex3DSize; }
    bool	 defaultShadowSize() const   { return !myUpdates.shadowSize; }
    bool	 defaultReflectSize() const  { return !myUpdates.reflectSize; }
    bool	 defaultHDRTexture() const   { return !myUpdates.hdrTexture; }
    bool	 defaultHDRReflect() const   { return !myUpdates.hdrReflect; }
    bool	 defaultSmoothLines() const  { return !myUpdates.smoothLines; }
    bool	 defaultSceneAA() const      { return !myUpdates.sceneAA; }
    bool	 defaultTextureMem() const   { return !myUpdates.textureMem; }
    bool	 defaultGuideFont() const    { return !myUpdates.guideFont; }
    bool	 defaultDPIScale() const     { return !myUpdates.dpiScale; }

    // Turns on the closure selection on hull-based or all geometry
    int		 closureSelection() const    { return myClosureSelection; }
    void	 closureSelection(int s) { myClosureSelection=SYSclamp(s,0,2); }

    int		 getVisContext() const 	     { return myVisContext; }
    void	 setVisContext(int ctxt)     { myVisContext = ctxt; }

    // dummy callbacks for obsolete options.
    bool	 obsoleteOption() const { return false; }
    void	 obsoleteOption(bool onoff) {}
    
private:
    void	 loadDefaultColors();
    void	 getColorsByName();

    // data
    UT_Color	 myColors[GR_NUM_DISPLAY_COLORS][2];
    fpreal	 myAlphas[GR_NUM_DISPLAY_ALPHAS];
    UT_Color	 myCurrentWireColor[2];
    
    UT_String	 myVisibleObjects;	// Visibility mask
    GA_PrimCompat::TypeMask myPrimMask;	// What types of prims to draw
    unsigned     myPickMask;		// What geo entities to pick

    fpreal	 myMarkerSize;		// point,vertex,breakpoint marker diam
    float	 myNormalScale;		// A size to scale the normal by
    fpreal	 myVectorScale;		// Scale custom vectors
    float	 myLOD;			// level of detail
    float	 myHLOffsetVariable;	// hidden line constant offset
    float	 myHLOffsetConstant;	// hidden line variable offset
    float	 myWireWidth;
    fpreal32	 myWireBlend;
    fpreal32	 myInteriorWireAlpha;
    GR_ParticleType myParticleDisplayType;
    fpreal	 myDPIScale;		// DPI scale factor
    float	 myPointSize;		// point size in pixels
    float	 myDiscSize;		// disc size
    int 	 myParticleTextureWidth;  // in pixels
    int 	 myParticleTextureHeight; // in pixels
    float	 myShadowOffsetVariable;// shadow constant offset
    float	 myShadowOffsetConstant;// sadow line variable offset
    int		 myShadowAA;		// Antialiasing of shadow edges.
    bool	 myUseLightShadowSize;  // Shadowmap size source from light
    int		 myShadowMapSize;       // Shadowmap size or max size
    int		 myReflectMapSize;	// Reflect map resolution
    fpreal	 myReflectMinLevel;	// No reflect map below min level
    int		 mySelectionListInst;	// which selection instance to draw
    int		 mySceneAntialias;	// scene antialiasing (2^a samples).
    int		 myTextureAnisotropy;	// Texture anisotropic sampling
    int		 myAmbOcclusionLevel;	// 1-4, for more occlusion
    int		 myLightSamples;	// 1-64, for area/env light sampling
    float	 myViewGamma;
    bool	 myUseLUT;
    UT_String	 myViewLUT;
    bool	 myUseOCIO;
    UT_String	 myOCIOColorspace;
    UT_String	 myOCIOLooks;
    float	 myViewBlack;
    float	 myViewWhite;
    int		 myViewComponents;
    int		 myTransparentQuality;  // Transparent object layering quality
    fpreal	 myDisplacementQuality; // Bias displacement auto-LOD
    int		 myVolumeQuality;	// Volume slice quality
    int		 myClosureSelection;	// 0=hide, 1=hull geo only, 2=show

    fpreal	 myNormalCuspAngle;
    int		 myVertNormalLimit;
    
    // 2D Textures
    float	 myTextureScale;	// 0.0-1.0
    float	 myDynamicScale;	// when dynamic cache adjust is on
    int		 myMaxTextureWidth;	// in pixels
    int		 myMaxTextureHeight;	// in pixels
    int		 myHDRTextureFormat;	// 2=full,1=16FP,0=8b,-1=compressed
    int		 myDynamicFormat;

    // 3D Textures. If mySaveFlags.use2DTextureLimits is true, these are ignored
    float	 myTexture3DScale;	// 0.0-1.0
    float	 myDynamic3DScale;	// when dynamic cache adjust is on
    int		 myMaxTexture3DWidth;	// in pixels
    int		 myMaxTexture3DHeight;	// in pixels
    int		 myMaxTexture3DDepth;	// in pixels
    int		 myHDRTexture3DFormat;	// 2=full,1=16FP,0=8b,-1=compressed
    int		 myDynamic3DFormat;

    int		 myTextureMemLimitMB;	// max single texture size, in MB

    fpreal	 myInstancingFraction;	// portion of instances to draw, 0..1
    int		 myInstancingLimit;	// #tris
    int		 myInstancingLimitAction; // How to handle over-limit instances

    fpreal	 myMinHomeZoom;		// minimum area to zoom to when homing

    GR_DisplayColor myCrtAuxColor;	// current auxiliary color

    char	 myGotDefaults;
    UT_String	 myColorScheme;

    UT_String	 myColorAttrib;		// Color override parameters
    int		 myColorType;
    fpreal	 myColorDomain[2];

    GR_AlphaPass myAlphaPass;		// Type of alpha pass
    GR_DecorFontSize myDecorFontSize;

    fpreal	 myAgentLODBias;
    int		 myAgentMinPoints;
    int		 myAgentBaseLOD;
    int		 myAgentMaxLOD;
    int		 myAgentBoneDeform;
    int		 myAgentWireMode;

    int		 myOnionSkinPreFrameCount;
    int		 myOnionSkinPostFrameCount;
    int		 myOnionSkinFrameInc;
    UT_Color	 myOnionSkinPreFrameColor;
    UT_Color	 myOnionSkinPostFrameColor;
    fpreal	 myOnionSkinAlpha;

    gr_BBoxMode	 myBBoxMode;
    int		 mySubdPolyLimit;

    UT_StringHolder	 myUVAttrib;
    
    static UT_SymbolMap<GR_SchemeColors *>	*ourSchemeDefaults;
    static UT_Array<GR_CommonDispOption *>	 ourInstancesArray;

    GR_HandleParts	myHandleParts;

    struct grSaveFlags
    {
	unsigned     drawTexture	:1,
		     drawTransparent	:1,
		     drawDisplacement	:1,
		     drawTexturedSpots  :1,
		     highQualityLighting:1,
		     ambientOcclusion   :1,
		     limitTextureSize	:1,
		     limitTexture3DSize	:1,
	             mipMapTextures	:1,
		     hdrRender		:1,
		     drawShadows        :1,
		     drawTransparentMaps:1,
		     removeBackface	:1,
		     correctBG		:1,
		     frameTime		:1,
		     fillSelection	:1,
		     solidOnly		:1,	// Only draw solid geometry.
						// Don't bother with shading
						// information (normals, etc).
		     drawPartSprite	:1,	// draw particles as sprites
		     orientDiscs	:1,	// draw particles as sprites

		     drawPartOrigin	:1,	// draw particle origin
		     drawHandles	:1,	// draw handles
		     drawHullsOnly      :1,
		     showObjectSelection:1,	// object selection highlight

		     shadeOpenCurves    :1,	// shade open curve in shaded
		     displayAsSubdiv    :1,	// display as subdiv
		     overrideColorAttrib:1,	// override Cd attribute
		     use2DTexLimits     :1,	// 3D tex use 2D tex settings
		     dynamicTexControl	:1,	// dynamic texture cache
		     reduceTex2D	:1,	// controls - auto reduction of
	             reduceTex3D	:1,	// quality when resources are
		     reduceMipMap	:1,	// low.
		     reduceBitDepth	:1,	//
		     drawDiffuse	:1,
		     drawAmbient	:1,
		     drawEmission	:1,
		     useLighting	:1,
		     pointInstancing    :1,
		     polygonConvexQuality:1,
		     autoDPIScale	:1,	// auto-set DPI scale
		     volumeWireAsPoints :1,
		     pickWireAsSolid	:1,	// pick wire prims as solid
		     wireOverPacked	:1,	// draw wire-over for packed geo
		     agentLODEnable     :1,
		     onionSkinEnable    :1,	// global onion skinning flag
		     cuspNormals	:1,
		     enableReflectionMaps:1, // cubemap gen for refl obj
		     hdrReflectionMaps	:1, // 8b or fp16 refl cubmaps
		     showGeoColor	:1, // show or ignore Cd on geometry
		     interactiveEnable  :1,     // interactive mode is on
		     optPackedGeo	:1, // optimize packed geo for GL
		     offsetVertexDecor  :1; 
    };
    grSaveFlags mySaveFlags;			// flags that get saved

    struct grNoSaveFlags
    {
	unsigned     snapToSelected	:1,	// snap to selection
		     showComponentSelections :1, // draw geometry component  
		     showTempSelections :1,	// draw temporary or cooked
						// selections?
		     pickFrontFaces	:1,	// pick front-facing polygons
		     pickBackFaces	:1,	// pick back-facing polygons
		     drawAuxColors      :1,	// draw auxilary colors for
						// things like prim axes
		     forceWireColor     :1,	// use wire color for wire
		     shadingOpenCurves  :1,	// shade open curves
		     drawMultiTexture   :1,	// mat layers in GUI_DispOpts
		     dynamicMipSetting  :1,
		     interactive        :1,     // interactively moving
		     applyVisualizers	:1; 	// master visualization toggle
    };

    // These option can be auto-set by Houdini, so it is important to keep
    // track of whether they have been manually set or loaded from defaults/hip
    // so that they are not overwritten by the auto-set.
    struct gr_UpdatedOptions
    {
	bool	texSize;
	bool	tex3DSize;
	bool	shadowSize;
	bool	reflectSize;
	bool	hdrTexture;
	bool	hdrReflect;
	bool	smoothLines;
	bool	sceneAA;
	bool	textureMem;
	bool    guideFont;
	bool    dpiScale;
    } myUpdates;
    
    grNoSaveFlags      myNoSaveFlags;		// flags that don't get saved

    int		       myVisContext;		// vis context (viewport index)
						// assigned by vis manager.

    int		       myUniqueID;
    int		       mySerialNum;
    int		       mySerialView;
    int		       mySerialID;
    int		      *myGammaPassPtr;		// ptr to gamma pass flag which
						// can be changed during render
};

inline void
GR_CommonDispOption::LOD(float l)
{
    float lod = (l>0.0) ? l : 0.001;
    if(myLOD != lod)
    {
	myLOD = lod;
	mySerialNum++;
    }
}

inline void
GR_CommonDispOption::setColorAttrib(const char *a)
{
    if(myColorAttrib != a)
    {
	myColorAttrib.harden(a);
	mySerialNum++;
    }
}

inline void
GR_CommonDispOption::setColorType(int t)
{
    if(myColorType != t)
    {
	myColorType = t;
	mySerialNum ++;
    }
}

inline void
GR_CommonDispOption::setColorDomain(fpreal s, fpreal e)
{
    if(!SYSisEqual(myColorDomain[0], s) ||
       !SYSisEqual(myColorDomain[1], e))
    {
	myColorDomain[0] = s;
	myColorDomain[1] = e;
	mySerialNum ++;
    }
}
       

#undef FLAG_ACCESSORS

#endif
