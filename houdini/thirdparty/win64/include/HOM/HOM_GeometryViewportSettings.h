/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_GeometryViewportSettings_h__
#define __HOM_GeometryViewportSettings_h__

#include "HOM_API.h"
#include "HOM_Color.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_GeometryViewportBackground.h"
#include "HOM_GeometryViewportDisplaySet.h"
#include "HOM_Module.h"
#include "HOM_ObjNode.h"
#include "HOM_EnumValue.h"

SWIGOUT(%rename(GeometryViewportSettings) HOM_GeometryViewportSettings;)

class HOM_API HOM_GeometryViewportSettings
{
public:
    HOM_GeometryViewportSettings()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_GeometryViewportSettings(const HOM_GeometryViewportSettings &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_GeometryViewportSettings()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual HOM_EnumValue &viewportType() = 0;

    // Markers tab -------------------------------------------------------
    
    SWIGOUT(%newobject displaySet;)
    virtual HOM_GeometryViewportDisplaySet *displaySet(HOM_EnumValue &settype) = 0;
	
    virtual float viewAspectRatio(bool masked = true) = 0;
    virtual double normalScale() = 0;
    virtual void setNormalScale(double normal_scale) = 0;
    virtual double vectorScale() = 0;
    virtual void setVectorScale(double vec_scale) = 0;
    virtual double pointMarkerSize() = 0;
    virtual void setPointMarkerSize(double psize) = 0;
    virtual double originGnomonSize()= 0;
    virtual void setOriginGnomonSize(double size) = 0;

    SWIGOUT(%newobject camera;)
    virtual HOM_ObjNode *camera() = 0;
    virtual void setCamera(HOM_ObjNode &camera_node) = 0;
    virtual void setCamera(const char *camera_path) = 0;
    virtual void saveViewToCamera(HOM_ObjNode &camera_node) = 0;

    virtual void geometryInfo(HOM_EnumValue &info) = 0;
    virtual HOM_EnumValue &geometryInfo() = 0;
    
    virtual void handleHighlight(HOM_EnumValue &info) = 0;
    virtual HOM_EnumValue &handleHighlight() = 0;
    
    virtual void closureSelection(HOM_EnumValue &info) = 0;
    virtual HOM_EnumValue &closureSelection() = 0;
    
    virtual void guideFontSize(HOM_EnumValue &info) = 0;
    virtual HOM_EnumValue &guideFontSize() = 0;

    virtual void setOffsetVertexMarkers(bool enable) = 0;
    virtual bool offsetVertexMarkers() = 0;

    // guide enum = viewportGuide
    virtual void enableGuide(HOM_EnumValue &guide, bool enabled) = 0;
    virtual bool guideEnabled(HOM_EnumValue &guide) = 0;

    // Geometry tab -----------------------------------------------------

    virtual void levelOfDetail(double value) = 0;
    virtual double levelOfDetail() = 0;

    // HOM_ViewportVolumeQuality
    virtual void volumeQuality(HOM_EnumValue &quality) = 0;
    virtual HOM_EnumValue &volumeQuality() = 0;
    
    virtual void volumeWireAsPoints(bool as_points) = 0;
    virtual bool volumeWireAsPoints() = 0;
    
    virtual void polygonConvexQuality(bool quality) = 0;
    virtual bool polygonConvexQuality() = 0;

    virtual void subdivsionLimit(int limit_in_millions) = 0;
    virtual int  subdivsionLimit() = 0;

    virtual void wireWidth(double width) = 0;
    virtual double wireWidth() = 0;
    
    virtual void wireBlend(double blend) = 0;
    virtual double wireBlend() = 0;
    
    virtual void interiorWireAlpha(double alpha) = 0;
    virtual double interiorWireAlpha() = 0;

    virtual void shadeOpenCurves(bool shade) = 0;
    virtual bool shadeOpenCurves() = 0;
    
    virtual void selectWireframeAsSolid(bool as_solid) = 0;
    virtual bool selectWireframeAsSolid() = 0;
    
    virtual void setWireOverPackedGeo(bool wire_over_packed) = 0;
    virtual bool wireOverPackedGeo() = 0;
    

    // HOM_ViewportParticleDisplay
    virtual void particleDisplayType(HOM_EnumValue &quality) = 0;
    virtual HOM_EnumValue &particleDisplayType() = 0;
    
    virtual void allowParticleSprites(bool sprites) = 0;
    virtual bool allowParticleSprites() = 0;

    virtual void particlePointSize(double size) = 0;
    virtual double particlePointSize() = 0;
    
    virtual void particleDiscSize(double size) = 0;
    virtual double particleDiscSize() = 0;

    virtual void orientDiscToNormal(bool n_orient) = 0;
    virtual bool orientDiscToNormal() = 0;
    
    virtual void spriteTextureLimit(std::vector<int> &max_res)
	    = 0;
    virtual std::vector<int> spriteTextureLimit() = 0;

    
    virtual void pointInstancing(bool enable) = 0;
    virtual bool pointInstancing() = 0;
    
    virtual void pointInstancingPercent(double show_instances) = 0;
    virtual double pointInstancingPercent() = 0;
    
    virtual void pointInstancingLimit(int millions_of_instances) = 0;
    virtual int pointInstancingLimit() = 0;
   
    // HOM_ViewportStandInGeometry
    virtual void instanceStandInGeometry(HOM_EnumValue &standin) = 0;
    virtual HOM_EnumValue &instanceStandInGeometry() = 0;

    virtual void autoGenerateVertexNormals(bool vertex) = 0;
    virtual bool autoGenerateVertexNormals() = 0;

    virtual void vertexNormalCuspAngle(double angle) = 0;
    virtual double vertexNormalCuspAngle() = 0;
    
    virtual void vertexNormalLimit(int millions_of_polys) = 0;
    virtual int vertexNormalLimit() = 0;

    // Scene tab -------------------------------------------------------

    virtual void setSceneAntialias(int aa) = 0;
    virtual int sceneAntialias() = 0;

    virtual void setHdrRendering(bool hdr) = 0;
    virtual bool hdrRendering() = 0;
    
    virtual void setXrayDrawing(bool global_enable) = 0;
    virtual bool xrayDrawing() = 0;
    
    virtual void setXrayQuality(bool quality) = 0;
    virtual bool xrayQuality() = 0;
    
    virtual void   setXrayStrength(double strength) = 0;
    virtual double xrayStrength() = 0;

    virtual void setObjectOrigins(bool globel_enable) = 0;
    virtual bool objectOrigins() = 0;

    virtual void setOnionSkinning(bool global_enable) = 0;
    virtual bool onionSkinning() = 0;
    
    virtual void setOnionSkinFramesBeforeCount(int num_frames) = 0;
    virtual int  onionSkinFramesBeforeCount() = 0;

    virtual void setOnionSkinFramesBeforeTint(HOM_Color &tint) = 0;
    virtual HOM_Color  onionSkinFramesBeforeTint() = 0;

    virtual void setOnionSkinFramesAfterCount(int num_frames) = 0;
    virtual int  onionSkinFramesAfterCount() = 0;

    virtual void setOnionSkinFramesAfterTint(HOM_Color &tint) = 0;
    virtual HOM_Color  onionSkinFramesAfterTint() = 0;

    virtual void setOnionSkinFrameIncrement(int frame_increment) = 0;
    virtual int  onionSkinFrameIncrement() = 0;

    virtual void   setOnionSkinOpacity(double opacity) = 0;
    virtual double onionSkinOpacity() = 0;

    virtual void   setSceneGamma(double gamma) = 0;
    virtual double sceneGamma() = 0;

    virtual void setUseSceneLUT(bool enable) = 0;
    virtual bool useSceneLUT() = 0;

    virtual void        setSceneLUT(std::string lut_file) = 0;
    virtual std::string sceneLUT() = 0;

    virtual void setBackgroundImageGammaLUT(bool apply_to_bg) = 0;
    virtual bool backgroundImageGammaLUT() = 0;
    
    // Viewport tab ----------------------------------------------------

    virtual void showsName(bool show) = 0;
    virtual bool showName() = 0;

    virtual void showsCameraName(bool show) = 0;
    virtual bool showCameraName() = 0;

    virtual void showsStateStatus(bool show) = 0;
    virtual bool showStateStatus() = 0;
    
    virtual void showsBadges(bool show) = 0;
    virtual bool showBadges() = 0;
    
    virtual void useAspectRatio(bool enable) = 0;
    virtual bool usingAspectRatio() = 0;

    virtual void   setAspectRatio(double aspect) = 0;
    virtual double aspectRatio() = 0;

    virtual void   setViewMaskOpacity(double opacity) = 0;
    virtual double viewMaskOpacity() = 0;

    virtual void setStereoMode(HOM_EnumValue &viewportStereoMode) = 0;
    virtual HOM_EnumValue stereoMode() = 0;
    
    virtual void setHomeAutoAdjustsClip(HOM_EnumValue &viewportHomeClipMode)= 0;
    virtual HOM_EnumValue homeAutoAdjustClip() = 0;
    
    virtual void setClipPlanes(std::vector<double> clip) = 0;
    virtual std::vector<double> clipPlanes() = 0;

    virtual void   setMinHomeSize(double min_size) = 0;
    virtual double minHomeSize() = 0;
    
    virtual void setUVDisplayAttribute(const std::string &uv) = 0;
    virtual std::string uvDisplayAttribute() = 0;

    virtual void setUVAutoAttribute(bool detect) = 0;
    virtual bool uvAutoAttribute() = 0;

    virtual void setUVVertexType(bool is_vertex_uv) = 0;
    virtual bool uvVertexType() = 0;

    // Effects tab -----------------------------------------------------

    virtual void setLighting(HOM_EnumValue &viewportLighting) = 0;
    virtual HOM_EnumValue &lighting() = 0;

    virtual void showDiffuse(bool enable) = 0;
    virtual bool showingDiffuse() = 0;

    virtual void showSpecular(bool enable) = 0;
    virtual bool showingSpecular() = 0;

    virtual void showAmbient(bool enable) = 0;
    virtual bool showingAmbient() = 0;

    virtual void showEmission(bool enable) = 0;
    virtual bool showingEmission() = 0;

    virtual void setLightSampling(int num_samples) = 0;
    virtual int  lightSampling() = 0;

    virtual void   setHeadlightIntensity(double intensity) = 0;
    virtual double headlightIntensity() = 0;

    virtual void   setHeadlightDirection(std::vector<double> dir) = 0;
    virtual std::vector<double> headlightDirection() = 0;

    virtual void setHeadlightSpecular(bool enable) = 0;
    virtual bool headlightSpecular() = 0;

    virtual void showMaterials(bool enable) = 0;
    virtual bool showingMaterials() = 0;

    virtual void showGeometryColor(bool enable) = 0;
    virtual bool showingGeometryColor() = 0;

    virtual void useTransparency(bool enable) = 0;
    virtual bool usingTransparency() = 0;

    virtual void setTransparencyQuality(HOM_EnumValue &viewportTransparency) = 0;
    virtual HOM_EnumValue transparencyQuality() = 0;

    virtual void useDisplacement(bool enable) = 0;
    virtual bool usingDisplacement() = 0;
    
    virtual void   setDisplacementLevel(double level) = 0;
    virtual double displacementLevel() = 0;

    virtual void       setDefaultMaterialDiffuse(HOM_Color &tint) = 0;
    virtual HOM_Color  defaultMaterialDiffuse() = 0;
    
    virtual void       setDefaultMaterialSpecular(HOM_Color &tint) = 0;
    virtual HOM_Color  defaultMaterialSpecular() = 0;
    
    virtual void       setDefaultMaterialAmbient(HOM_Color &tint) = 0;
    virtual HOM_Color  defaultMaterialAmbient() = 0;
    
    virtual void       setDefaultMaterialEmission(HOM_Color &tint) = 0;
    virtual HOM_Color  defaultMaterialEmission() = 0;

    
    virtual void setAmbientOcclusion(bool enable) = 0;
    virtual bool ambientOcclusion() = 0;
    
    virtual void setAmbientOcclusionLevel(int level) = 0;
    virtual int  ambientOcclusionLevel() = 0;

    virtual void setShadowQuality(HOM_EnumValue &viewportShadowQuality) = 0;
    virtual HOM_EnumValue shadowQuality() = 0;

    virtual void setShadowSensitivity(std::vector<int> offset) = 0;
    virtual std::vector<int>  shadowSensitivity() = 0;
    
    virtual void setShadowMapSize(int size) = 0;
    virtual int  shadowMapSize() = 0;

    virtual void setShadowMapSizeFromLight(bool enable) = 0;
    virtual bool shadowMapSizeFromLight() = 0;

    virtual void useReflections(bool enable) = 0;
    virtual bool usingReflections() = 0;
    
    virtual void setHdrReflections(bool hdr_reflect) = 0;
    virtual bool hdrReflections() = 0;
    
    virtual void setReflectMapSize(int size) = 0;
    virtual int  reflectMapSize() = 0;
    
    virtual void   setMinReflectAmount(double min_reflect) = 0;
    virtual double minReflectAmount() = 0;

    // Grid tab ------------------------------------------------------------

    virtual void setDisplayOrthoGrid(bool enable) = 0;
    virtual bool displayOrthoGrid() = 0;

    virtual void setOthroGridOffset(std::vector<double> offset) = 0;
    virtual std::vector<double> othroGridOffset() = 0;
    
    virtual void setOthroGridSpacing(std::vector<double> offset) = 0;
    virtual std::vector<double> othroGridSpacing() = 0;
    
    virtual void setOthroGridRuler(std::vector<int> offset) = 0;
    virtual std::vector<int> othroGridRuler() = 0;
    
    virtual void setOrthoRuler(HOM_EnumValue &viewportGridRuler) = 0;
    virtual HOM_EnumValue orthoRuler() = 0;
    
    virtual void setUVReferenceGrid(bool enable) = 0;
    virtual bool uvReferenceGrid() = 0;
    
    virtual void setUVDisplayGridOverImage(bool enable) = 0;
    virtual bool uvReferenceGridOverImage() = 0;
    
    virtual void setUVGridPixelSpacing(std::vector<int> offset) = 0;
    virtual std::vector<int> uvGridPixelSpacing() = 0;
    
    virtual void setUVGridPixelOffset(std::vector<double> offset) = 0;
    virtual std::vector<double> uvGridPixelOffset() = 0;
    
    virtual void setClampUVGridToBackground(bool enable) = 0;
    virtual bool clampUVGridToBackground() = 0;
    
    virtual void setDisplayUVTileBoundaries(bool enable) = 0;
    virtual bool displayUVTileBoundaries() = 0;
    
    // Background tab ------------------------------------------------------

    virtual void setColorScheme(HOM_EnumValue &viewportColorScheme) = 0;
    virtual HOM_EnumValue colorScheme() = 0;

    SWIGOUT(%newobject backgroundImage;)
    virtual HOM_GeometryViewportBackground *backgroundImage(
		HOM_EnumValue &viewportBackgroundView,
		int layer = 1) = 0;

    virtual void setDisplayBackgroundImage(bool enable) = 0;
    virtual bool displayBackgroundImage() = 0;
    
    virtual void setDisplayEnvironmentBackgroundImage(bool enable) = 0;
    virtual bool displayEnvironmentBackgroundImage() = 0;
    
    virtual void setFilterBackgroundImage(bool enable) = 0;
    virtual bool filterBackgroundImage() = 0;

    virtual void setApplyZoomToBackgroundImage(bool enable) = 0;
    virtual bool applyZoomToBackgroundImage() = 0;

    virtual void setBackgroundImageQuality(int quality) = 0;
    virtual int  backgroundImageQuality() = 0;
    
    // Texture tab --------------------------------------------------------
    
    virtual void setDisplayTextures(bool enable) = 0;
    virtual bool displayTextures() = 0;
    
    virtual void setDisplayTextureLayers(bool enable) = 0;
    virtual bool displayTextureLayers() = 0;
    
    virtual void setDisplayProjectedTextures(bool enable) = 0;
    virtual bool displayProjectedTextures() = 0;
    
    virtual void setTextureMipmapping(bool enable) = 0;
    virtual bool textureMipmapping() = 0;
    
    virtual void setTextureAnisotropicFilter(int num_samples) = 0;
    virtual int  textureAnisotropicFilter() = 0;

    virtual void setTextureCacheSize(int size_in_mb) = 0;
    virtual int  textureCacheSize() = 0;

    virtual void setTextureMaxMemory(int size_in_mb) = 0;
    virtual int  textureMaxMemory() = 0;

    virtual void setTextureAutoReduce(bool enable) = 0;
    virtual bool textureAutoReduce() = 0;
    
    virtual void setTextureAutoReduce2D(bool enable) = 0;
    virtual bool textureAutoReduce2D() = 0;
    
    virtual void setTextureAutoReduce3D(bool enable) = 0;
    virtual bool textureAutoReduce3D() = 0;
    
    virtual void setTextureResLimit2D(bool enable) = 0;
    virtual bool textureResLimit2D() = 0;
    
    virtual void setTextureMaxRes2D(std::vector<int> res) = 0;
    virtual std::vector<int>  textureMaxRes2D() = 0;
    
    virtual void setTextureBitDepthLimit2D(HOM_EnumValue &viewportTextureDepth)= 0;
    virtual HOM_EnumValue textureBitDepthLimit2D() = 0;

    virtual void setTextureScale2D(double scale) = 0;
    virtual double textureScale2D() = 0;

    virtual void setTextureResLimit3D(bool enable) = 0;
    virtual bool textureResLimit3D() = 0;
    
    virtual void setTexture2DSettingsFor3D(bool enable) = 0;
    virtual bool texture2DSettingsFor3D() = 0;

    virtual void setTextureMaxRes3D(std::vector<int> res) = 0;
    virtual std::vector<int>  textureMaxRes3D() = 0;
    
    virtual void setTextureBitDepthLimit3D(HOM_EnumValue &viewportTextureDepth)= 0;
    virtual HOM_EnumValue textureBitDepthLimit3D() = 0;

    virtual void setTextureScale3D(double scale) = 0;
    virtual double textureScale3D() = 0;

    // Optimize tab --------------------------------------------------------
    virtual void useMaterialStylesheets(bool enable) = 0;
    virtual bool usingMaterialStylesheets() = 0;
   
    virtual void usePerPrimStylesheets(bool enable) = 0;
    virtual bool usingPerPrimStylesheets() = 0;
   
    virtual void usePackedStylesheets(bool enable) = 0;
    virtual bool usingPackedStylesheets() = 0;
   
    virtual void useMaterialOverrides(bool enable) = 0;
    virtual bool usingMaterialOverrides() = 0;
   
    virtual void setSingleObjectMaterialLimit(int num_materials) = 0;
    virtual int  singleObjectMaterialLimit() = 0;

    virtual void setMaterialUpdate(HOM_EnumValue &viewportMaterialUpdate) = 0;
    virtual HOM_EnumValue materialUpdate() = 0;
    
    virtual void setInteractiveMaterialUpdate(bool enable) = 0;
    virtual bool interactiveMaterialUpdate() = 0;
   
    virtual void updateMaterials() = 0;

    
    virtual void setVisibleObjects(const std::string &uv) = 0;
    virtual std::string visibleObjects() = 0;

    virtual void setRemoveBackfaces(bool enable) = 0;
    virtual bool removeBackfaces() = 0;
   
    virtual void setHullsOnly(bool enable) = 0;
    virtual bool hullsOnly() = 0;

    virtual void setDistanceBasedPackedCulling(bool enable) = 0;
    virtual bool distanceBasedPackedCulling() = 0;

    virtual void setScenePolygonLimit(int millions_of_polygons) = 0;
    virtual int  scenePolygonLimit() = 0;

    virtual void setPackedBoundingBoxMode(HOM_EnumValue &viewportPackedBoxMode) = 0;
    virtual HOM_EnumValue packedBoundingBoxMode() = 0;

    virtual void setOptimizeGeometry(bool enable) = 0;
    virtual bool optimizeGeometry() = 0;

    
    virtual void setAgentLODReduction(bool enable) = 0;
    virtual bool agentLODReduction() = 0;

    virtual void setAgentLODReductionBias(int reduce_bias) = 0;
    virtual int  agentLODReductionBias() = 0;
    
    virtual void setAgentShapePointCutoff(int num_points) = 0;
    virtual int  agentShapePointCutoff() = 0;
    
    virtual void setMaxLODLevels(int num_levels) = 0;
    virtual int  maxLODLevels() = 0;
    
    virtual void setBaseLODLevel(int base_level) = 0;
    virtual int  baseLODLevel() = 0;
    
    virtual void setSingleBoneDeform(HOM_EnumValue &viewportAgentBoneDeform)=0;
    virtual HOM_EnumValue singleBoneDeform() = 0;
    
    virtual void setAgentWireframe(HOM_EnumValue &viewportAgentWireframe)=0;
    virtual HOM_EnumValue agentWireframe() = 0;
};

#endif
