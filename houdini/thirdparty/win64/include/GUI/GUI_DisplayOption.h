/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GUI_DisplayOption.h ( GUI Library, C++)
 *
 * COMMENTS:    This class contains the options for all the different types
 *		of details (templates, footprints, etc.) and options that
 *		are common to them.
 */

#ifndef __GUI_DisplayOption__
#define __GUI_DisplayOption__

#include "GUI_API.h"
#include <UT/UT_Rect.h>
#include <UT/UT_Vector2.h>
#include <RE/RE_TextureMap.h>
#include <UI/UI_Value.h>
#include <GR/GR_Defines.h>
#include <GR/GR_CommonDispOption.h>
#include <GR/GR_DisplayOption.h>
#include "GUI_RenderCommon.h"

class OPUI_RotoscopeData;
class GR_DisplayOption;

const int GUI_NUM_DISPLAY_SETS	     = 6;
const int GUI_NUM_DISPLAY_SCENE_SETS = 3;
const int GUI_NUM_DISPLAY_MODEL_SETS = 3;

/// Context specific display options for geometry states
enum GUI_DisplaySetType
{
    GUI_SCENE_OBJECT,
    GUI_SELECTED_SCENE_OBJECT,
    GUI_GHOST_SCENE_OBJECT,
    // ---- obj/model break
    GUI_DISPLAY_MODEL,
    GUI_CURRENT_MODEL,
    GUI_TEMPLATE_MODEL
};

#define ANY_FLAG_ACTIVE_IN_SETS(METHOD)					\
    bool METHOD##Any() const						\
         { return ((*this)[GUI_SCENE_OBJECT].METHOD() ||		\
		   (*this)[GUI_SELECTED_SCENE_OBJECT].METHOD() ||	\
		   (*this)[GUI_GHOST_SCENE_OBJECT].METHOD() ||		\
		   (*this)[GUI_DISPLAY_MODEL].METHOD() ||		\
		   (*this)[GUI_CURRENT_MODEL].METHOD() ||		\
		   (*this)[GUI_TEMPLATE_MODEL].METHOD()); }


/// Display options object that contains all high-level viewport display
/// settings, and contains the objects with context-specific display options
/// and common drawing options.
class GUI_API GUI_DisplayOption
{
public:
	      GUI_DisplayOption();
	     ~GUI_DisplayOption();

    GUI_DisplayOption		&operator=(const GUI_DisplayOption &opt);

    /// Context-specific geometry-specific display options. If a display set
    /// follows another set, it will be remapped to return that set.
    /// @{
	  GR_DisplayOption	&operator[](GUI_DisplaySetType type)
				{ return *myDisplaySet[myDisplaySetMap[type]]; }
    const GR_DisplayOption	&operator[](GUI_DisplaySetType type) const
				{ return *myDisplaySet[myDisplaySetMap[type]]; }
    /// @}
    
    /// Direct cccess to the display set, without any mapping.
    GR_DisplayOption	&getDisplaySet(GUI_DisplaySetType type)
				{ return *myDisplaySet[type]; }

    /// Low-level drawing display options common to all geometry contexts
    /// @{
    const GR_CommonDispOption &common() const { return myCommonOptions; }
	  GR_CommonDispOption &common()	      { return myCommonOptions; }
    /// @}

    /// Return true if the load/save was successful, and false otherwise.
    /// @{
    bool	 save(std::ostream &) const;
    bool	 load(UT_IStream &);
    /// @}

    /// Remap the display option set to another set.
    void		setDisplaySetRemap(GUI_DisplaySetType slave,
					   GUI_DisplaySetType master)
			    { myDisplaySetMap[slave] = master; }

    /// Query which display set this set is following. If none, it will refer
    /// to itself.
    GUI_DisplaySetType  getDisplaySetRemap(GUI_DisplaySetType t) const
			    { return myDisplaySetMap[t]; }

    /// Scene custom display option support - set and query scene options
    /// @{
    void		enableSceneOption(const char *name, bool enable);
    bool		isSceneOptionEnabled(const char *name) const;
    /// @}

    // These flags are stored here because they deal with different concepts
    // unknown to GR.  The flags at the GR level should only be used by GR when
    // it renders a specific detail.
    bool	 showSelected() const	  { return myFlags.showSelected; }
    bool	 showNonSelected() const  { return myFlags.showNonSelected; }
    bool	 showTemplates() const	  { return myFlags.showTemplates; }
    bool	 showFootprints() const	  { return myFlags.showFootprints; }
    bool	 showCurrentOutput() const { return myFlags.showCurrentOutput; }
    bool	 showDisplay() const	  { return myFlags.showDisplay; }
    bool	 showGuides() const	  { return myFlags.showGuides; }
    bool	 showObjectNames() const  { return myFlags.showObjectNames; }
    bool	 showFullNames() const    { return myFlags.showFullNames; }
    bool	 drawOrigin() const	  { return myFlags.drawOrigin; }
    bool	 drawStickyOrigin() const { return myFlags.drawStickyOrigin; }
    bool	 drawViewPivot() const	  { return myFlags.drawViewPivot; }
    bool	 drawWalls() const	  { return myFlags.drawWalls; }
    bool	 drawXYPlane() const	  { return myFlags.drawXYPlane; }
    bool	 drawXZPlane() const	  { return myFlags.drawXZPlane; }
    bool	 drawYZPlane() const	  { return myFlags.drawYZPlane; }
    bool	 drawFieldGuide() const	  { return myFlags.drawFieldGuide; }
    bool	 drawSafeArea() const	  { return myFlags.drawSafeArea; }
    bool	 drawBackgrounds() const  { return myFlags.drawBackgrounds; }
    bool 	 drawEnvironmentMaps() const
                                    { return myFlags.drawEnvironmentMaps; }
    bool	 drawFilteredBackgrounds() const
				    { return myFlags.drawFilteredBackgrounds; }
    bool	 showIKCriticalZones() const
				    { return myFlags.showIKCriticalZones; }
    bool	 getDrawCameraMask() const
					  { return myFlags.drawCameraMask; }
    bool	 drawBadges() const	  { return myFlags.drawBadges; }

    bool	 drawVertexAttrib2D() const
					{ return myFlags.drawVertexAttrib2D; }
    bool	 autoDetectAttribType2D() const
				{ return myFlags.autoDetectAttribType2D; }

    bool	 drawSpecular() const     { return myFlags.drawSpecular; }
    bool	 drawMultiTexture() const { return common().drawMultiTexture();}

    bool	 applySelectMask() const  { return myFlags.applySelectMask; }

    bool	 drawXRay() const	  { return myFlags.drawXRay; }
    bool	 xrayQuality() const	  { return myFlags.xrayQuality; }
    bool	 drawObjectOrigins() const { return myFlags.drawObjOrigins; }
    bool	 showMaterials() const    { return myFlags.showMaterials; }
    bool	 useMaterialOverrides() const { return myFlags.useOverrides; }
    bool	 mssEnable() const	  { return myFlags.mssEnable; }
    bool	 mssPerPrim() const	  { return myFlags.mssPerPrim; }
    bool	 mssPacked() const	  { return myFlags.mssPacked; }
    int		 objectMaterialLimit() const { return myObjectMaterialLimit; }
    fpreal	 xrayWireStrength() const { return myXRayWireStrength; }
    
    enum gui_MatAssignMode
    {
	MAT_ALWAYS = 0,
	MAT_NO_PLAYBACK = 1,
	MAT_MANUAL = 2
    };
    gui_MatAssignMode materialAssignMode() const   { return myMatAssignMode; }
    void	  materialAssignMode(gui_MatAssignMode mode)
						     { myMatAssignMode = mode; }
    bool	  materialUpdate() const      { return myMatUpdate; }
    void	  materialUpdate(bool update) { myMatUpdate = update; }
	
    
    // the threshold a shading mode is set to change when modifying view
    
    void	 showSelected(bool onoff)     { myFlags.showSelected	=onoff;}
    void	 showNonSelected(bool onoff)  { myFlags.showNonSelected	=onoff;}
    void	 showTemplates(bool onoff)    { myFlags.showTemplates	=onoff;}
    void	 showFootprints(bool onoff)   { myFlags.showFootprints	=onoff;}
    void	 showCurrentOutput(bool onoff)
					{ myFlags.showCurrentOutput = onoff;}
    void	 showDisplay(bool onoff)      { myFlags.showDisplay     =onoff;}
    void	 showGuides(bool onoff)	      { myFlags.showGuides	=onoff;}
    void	 showObjectNames(bool onoff)  { myFlags.showObjectNames	=onoff;}
    void	 showFullNames(bool onoff)    { myFlags.showFullNames	=onoff;}
    void	 drawOrigin(bool onoff)	      { myFlags.drawOrigin	=onoff;}
    void	 drawStickyOrigin(bool onoff) { myFlags.drawStickyOrigin=onoff;}
    void	 drawViewPivot(bool onoff)    { myFlags.drawViewPivot	=onoff;}
    void	 drawWalls(bool onoff)	      { myFlags.drawWalls	=onoff;}
    void	 drawXYPlane(bool onoff)      { myFlags.drawXYPlane	=onoff;}
    void	 drawXZPlane(bool onoff)      { myFlags.drawXZPlane	=onoff;}
    void	 drawYZPlane(bool onoff)      { myFlags.drawYZPlane	=onoff;}
    void	 drawFieldGuide(bool onoff)   { myFlags.drawFieldGuide	=onoff;}
    void	 drawSafeArea(bool onoff)     { myFlags.drawSafeArea	=onoff;}
    void	 drawBackgrounds(bool onoff)  { myFlags.drawBackgrounds	=onoff;}
    void	 drawEnvironmentMaps(bool onoff) 
                                       { myFlags.drawEnvironmentMaps	=onoff;}
    void	 drawFilteredBackgrounds(bool onoff)
				       { myFlags.drawFilteredBackgrounds=onoff;}
    void	 showIKCriticalZones(bool onoff)
				       { myFlags.showIKCriticalZones	=onoff;}
    void	 setDrawCameraMask(bool onoff)
				       { myFlags.drawCameraMask		=onoff;}
    void	 drawBadges(bool onoff)	      { myFlags.drawBadges	=onoff;}
    void	 drawVertexAttrib2D(bool onoff)
					{ myFlags.drawVertexAttrib2D = onoff; }
    void	 autoDetectAttribType2D(bool onoff)
				{ myFlags.autoDetectAttribType2D = onoff; }
    void	 drawSpecular(bool onoff)
					{ myFlags.drawSpecular = onoff; }
    void	 drawMultiTexture(bool onoff)
				        { common().drawMultiTexture(onoff); }
    void	 applySelectMask(bool onoff)
					{ myFlags.applySelectMask = onoff; }
    void	 drawXRay(bool xray)	  { myFlags.drawXRay = xray; }
    void	 xrayQuality(bool q)	  { myFlags.xrayQuality = q; }
    void	 xrayWireStrength(fpreal w) { myXRayWireStrength = w; }
    void	 drawObjectOrigins(bool org) { myFlags.drawObjOrigins = org; }
    void	 showMaterials(bool show) { myFlags.showMaterials = show; }
    void	 useMaterialOverrides(bool ovr) { myFlags.useOverrides = ovr; }
    void	 mssEnable(bool mss)	 { myFlags.mssEnable = mss; }
    void	 mssPerPrim(bool mpp)	 { myFlags.mssPerPrim = mpp; }
    void	 mssPacked(bool mp)	 { myFlags.mssPacked = mp; }
    void	 objectMaterialLimit(int m)
					{ myObjectMaterialLimit=SYSmax(1,m); }

    void	 handleHighlightSize(int hl)
				{ myHandleHighlightSize = SYSclamp(hl, 0, 2); }
    int		 handleHighlightSize() const { return myHandleHighlightSize; }
    void	 originGnomonSize(fpreal size)
				{ myOriginGnomonSize = SYSmax(size, 0.0); }
    fpreal	 originGnomonSize() const { return myOriginGnomonSize; }
    
    /// set/query enable/disable of shading mode change
    /// @{
    void	 moveModeOn(bool onoff)	{ myMoveModeOn = onoff; }
    bool	 moveModeOn() const	{ return myMoveModeOn; }
    /// @}
    
    /// Rotoscoping background image set/query methods
    /// @{
    RE_TextureHolder getRotoTexture(RE_Render *r,
				    GUI_RotoView type,
				    float &quality,
				    float &aspect_ratio,
				    UT_DimRect *udim_area = nullptr) const;
    void	 getRotoResolution(GUI_RotoView type, UT_Vector2i &res);

    bool	 setRotoFileParms(GUI_RotoView type,
				  const char *file, float quality);
    bool	 setRotoCopParms(GUI_RotoView type,
				 const char *path, float quality);
    void	 setRotoSource(GUI_RotoView type,bool file);

    void	 setRotoTime(fpreal t);
    bool	 isRotoTimeDependent(GUI_RotoView type) const;
    
    fpreal	 getRotoQuality() const;
    void	 setRotoQuality(fpreal q);

    void	 setRotoPath(GUI_RotoView type,
			     bool file_path, const char *path);
    bool	 isRotoFile(GUI_RotoView type) const;
    const char  *getRotoCopPath(GUI_RotoView type) const;
    const char  *getRotoFilePath(GUI_RotoView type) const;

    UI_Value	&getRotoCopCookNotify(GUI_RotoView type);
    UI_Value	&getRotoCopNameChangeNotify(GUI_RotoView type);

    void	 getImageMinUV(float arr[2]) const
		 {
		     arr[0] = myImageMinUV[0];
		     arr[1] = myImageMinUV[1]; 
		 }
    void	 setImageMinUV(float arr[2])
		 {
		     myImageMinUV[0] = arr[0];
		     myImageMinUV[1] = arr[1];
		 }
    void	 getImageMaxUV(float arr[2]) const
		 {
		     arr[0] = myImageMaxUV[0];
		     arr[1] = myImageMaxUV[1]; 
		 }
    void	 setImageMaxUV(float arr[2])
		 {
		     myImageMaxUV[0] = arr[0];
		     myImageMaxUV[1] = arr[1];
		 }
    
    void	 getImageOffset(GUI_RotoView view, float arr[2]) const
		 {
		     arr[0] = myRotoViews[view].myImageOffset[0];
		     arr[1] = myRotoViews[view].myImageOffset[1];
		 }
    void	 setImageOffset(GUI_RotoView view, float arr[2])
		 {
		     myRotoViews[view].myImageOffset[0] = arr[0];
		     myRotoViews[view].myImageOffset[1] = arr[1];
		 }
    void	 getImageScale(GUI_RotoView view, float arr[2]) const
		 {
		     arr[0] = myRotoViews[view].myImageScale[0];
		     arr[1] = myRotoViews[view].myImageScale[1];
		 }
    void	 setImageScale(GUI_RotoView view, float arr[2])
		 {
		     myRotoViews[view].myImageScale[0] = arr[0];
		     myRotoViews[view].myImageScale[1] = arr[1];
		 }

    float	 getBrightness(GUI_RotoView view) const
		 { return myRotoViews[view].myBrightness; }
    void	 setBrightness(GUI_RotoView view, float brightness)
		 { myRotoViews[view].myBrightness = brightness; }

    void	 autoPlaceImage(GUI_RotoView view, bool onoff) 
		 { myRotoViews[view].myAutoPlaceImage = onoff; }
    bool	 autoPlaceImage(GUI_RotoView view) const
		 { return myRotoViews[view].myAutoPlaceImage; }

    static const char *getRotoViewName(GUI_RotoView view);
    /// @}
 
    /// Methods to query/change the scale at which handles are drawn.
    /// @{
    static float getHandleScale() { return theHandleScale; }
    static void	 setHandleScale(float v)
			{ theHandleScale = SYSclamp(v, 0.01f, 1.0f); }
    /// @}

    // This method is called when user options are being destroyed.
    void	 removeUserOption(int option_id);

    /// Refresh the cached version keys for the enabled user options to reflect
    /// the most up-to-date version.
    void	 refreshUserOptionVersions();

    /// State which switches the geometry context display options between the
    /// Scene and Model display sets. When at the object level, the scene is
    /// used.
    /// @{
    bool	 isObjectLevel() const   { return myNoSaveFlags.isObjectLevel; }
    void	 setObjectLevel(bool onoff);
    /// @}

    /// The select mask is set when the user is in a "select" state to
    /// automatically turn on some decorations.  The mask is used to
    /// temporarily tweak the appropriate display options, and does not,
    /// in and of itself, invalidate display lists.
    /// @{
    unsigned	 getSelectMask() const { return mySelectMask; }
    void	 setSelectMask(unsigned mask) { mySelectMask = mask; }
    /// @}

    /// This value is changed when the scene is switched from obj to sop/dop
    /// level.
    UI_Value	&getObjectLevelNotify() { return myObjectLevelNotify; }

    /// For the OpenGL ROP only: Allows a single stereo camera to be rendered.
    enum gui_StereoPass
    {
	GUI_DISP_STEREO_BOTH,
	GUI_DISP_STEREO_LEFT,
	GUI_DISP_STEREO_RIGHT
    };

    /// Allow rendering of a single stereo camera eye.
    /// @{
    void	 setSeparateStereoRender(gui_StereoPass pass)
		    { myStereoPass = pass; }
    bool	 isStereoLeftRender() const
		    { return (myStereoPass == GUI_DISP_STEREO_BOTH ||
			      myStereoPass == GUI_DISP_STEREO_LEFT); }
    bool	 isStereoRightRender() const
		    { return (myStereoPass == GUI_DISP_STEREO_BOTH ||
			      myStereoPass == GUI_DISP_STEREO_RIGHT); }
    /// @}

    /// Returns true if any of the geometry contexts have this option set.
    /// @{
    ANY_FLAG_ACTIVE_IN_SETS(drawPointCoincident);
    ANY_FLAG_ACTIVE_IN_SETS(drawPointMarkers);
    ANY_FLAG_ACTIVE_IN_SETS(uvFillOverlap);
    /// @}

    /// Auto-select quality/preformance presets based on hardware queries.
    /// Only works at startup.
    void	 updateOptionsForHardware(RE_Render *r);


    /// Returns true if the passed options are compatible with these options,
    /// meaning that update() does not need to be called on GR_Primitives if
    /// 'opt' is used after this option. This could happen if 'opt' had a marker
    /// on that required an attribute that this options set does not.
    bool	 compatibleWith(const GUI_DisplayOption &opt) const;


    void	 setHeadlightIntensity(fpreal i);
    fpreal	 getHeadlightIntensity() const { return myHeadlightIntensity; }

    void	 setHeadlightDirection(UT_Vector3D dir);
    UT_Vector3D  getHeadlightDirection() const { return myHeadlightDir; }

    void	 setHeadlightSpecular(bool use_spec);
    bool	 getHeadlightSpecular() const { return myHeadlightSpec; }

    void	 setDefaultMaterialDiffuse(UT_Color col);
    void	 setDefaultMaterialSpecular(UT_Color col);
    void	 setDefaultMaterialAmbient(UT_Color col);
    void	 setDefaultMaterialEmission(UT_Color col);

    UT_Color	 getDefaultMaterialDiffuse() const { return myDefaultDiffuse; }
    UT_Color	 getDefaultMaterialSpecular() const{ return myDefaultSpecular; }
    UT_Color	 getDefaultMaterialAmbient() const { return myDefaultAmbient; }
    UT_Color	 getDefaultMaterialEmission() const{ return myDefaultEmission; }

    bool	 isDefaultMaterialDirty() const{ return myDefaultMaterialDirty;}
    void	 clearDefaultMaterialDirty() { myDefaultMaterialDirty=false;}

    void	 forceMSSUpdate(bool) { myMSSUpdateSerial++; }
    exint	 getMSSUpdateSerial() const { return myMSSUpdateSerial; }
    
    // The ClipOutPass flag should be set when the rendering pass being
    // performed is for geometry outside the six custom clipping planes.
    bool	 isClipOutPass() const	  { return myNoSaveFlags.isClipOutPass;}
    void	 setClipOutPass(bool out_pass)
		 {  myNoSaveFlags.isClipOutPass = out_pass; }

    void	 getRecommendedTextureLimit(RE_Render *r,
					    int &max2d, int &max3d) const;
private:    
    // data:
    
    GR_DisplayOption	*myDisplaySet[GUI_NUM_DISPLAY_SETS];
    GUI_DisplaySetType	 myDisplaySetMap[GUI_NUM_DISPLAY_SETS];
    GR_CommonDispOption	 myCommonOptions;	// global to viewport

    float		 myImageMinUV[2];
    float		 myImageMaxUV[2];

    fpreal		 myOriginGnomonSize;
    int			 myHandleHighlightSize;

    static float	 theHandleScale;
    
    UT_Vector3D		 myHeadlightDir;
    fpreal		 myHeadlightIntensity;
    bool		 myHeadlightSpec;

    UT_Color		 myDefaultDiffuse;
    UT_Color		 myDefaultSpecular;
    UT_Color		 myDefaultEmission;
    UT_Color		 myDefaultAmbient;
    bool		 myDefaultMaterialDirty;

    fpreal		 myXRayWireStrength;

    struct
    {
	OPUI_RotoscopeData	*myRotoscopeData;
	float			 myImageOffset[2];
	float			 myImageScale[2];
	float			 myBrightness;
	bool		         myAutoPlaceImage;
    } myRotoViews[GUI_NUM_ROTO_VIEWS];

    unsigned		 mySelectMask;		// mask of entities the user
						// wants to select. Not saved.
    
    struct
    {
	unsigned
	    showSelected	:1,
	    showNonSelected	:1,
	    showTemplates	:1,
	    showFootprints	:1,
	    showCurrentOutput	:1,
	    showDisplay		:1,
	    showGuides		:1,
	    showObjectNames	:1,
	    showFullNames	:1,
	    drawOrigin		:1,
	    drawStickyOrigin	:1,
	    drawViewPivot	:1,
	    drawWalls		:1, // draw bounding walls
	    drawXYPlane		:1, // draw XY reference plane
	    drawXZPlane		:1, // draw XZ reference plane
	    drawYZPlane		:1, // draw YZ reference plane
	    drawFieldGuide	:1,
	    drawSafeArea	:1,
	    drawBackgrounds	:1, // draw background images
	    drawEnvironmentMaps :1,
	    drawFilteredBackgrounds:1, // filter background in UV viewport.
	    drawVertexAttrib2D	:1, // use vertex attribute
	    autoDetectAttribType2D:1, // automatically track the attribute type	
	    drawSpecular	:1, // specular highlights
	    showIKCriticalZones	:1, // inverse kin. danger zones
	    drawCameraMask	:1,
	    drawBadges		:1, // show viewport status badges
	    drawXRay		:1, // allow draw of xray objs
	    xrayQuality		:1, // fast/quality xray draw
	    drawObjOrigins	:1, // allow draw of obj axes
	    applySelectMask	:1, // select mask drives specific decorations
	    showMaterials	:1, // SHOP materials used
	    useOverrides	:1, // Apply material overrides
	    mssEnable		:1, // Apply material stylesheets
	    mssPacked		:1, // Eval material stylesheets for packed geo
	    mssPerPrim		:1; // per-prim material stylesheet eval
	    
    } myFlags;
    
    struct
    {
	    unsigned	 isClipOutPass		:1,
			 isObjectLevel		:1; // Is this an object view?
    } myNoSaveFlags;

    // whether shading mode change is enabled/disabled
    bool		 myMoveModeOn;
    int			 myObjectMaterialLimit;

    gui_StereoPass	 myStereoPass;
    UI_Value		 myObjectLevelNotify;
    bool		 myLoadedDefaults;
    exint		 myMSSUpdateSerial;
    gui_MatAssignMode	 myMatAssignMode;
    bool		 myMatUpdate;
    
    UT_IntArray		 mySceneOptions;
};

#endif
