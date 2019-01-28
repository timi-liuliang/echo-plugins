/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Render Library (C++)
 *
 * COMMENTS:	Material definition for lit geometry
 *
 */

#ifndef __RE_Material_h__
#define __RE_Material_h__

#include "RE_API.h"
#include <DEP/DEP_MicroNode.h>
#include <SYS/SYS_AtomicInt.h>
#include <UT/UT_Color.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_Options.h>
#include <UT/UT_Rect.h>
#include <UT/UT_String.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_XformOrder.h>
#include "RE_Types.h"
#include "RE_TextureTypes.h"
#include "RE_TextureMap.h"
#include "RE_UniformBlock.h"

class RE_OGLRender;
class RE_OGLTexture;
class RE_Render;
class RE_Shader;
class RE_Texture2DMap;
class RE_TextureMap;
class RE_TextureCubeMap;

class RE_API RE_TextureCallback
{
public:
    /// This class is used to provide texture parameters for each texture layer
	     RE_TextureCallback();
    virtual ~RE_TextureCallback();

    enum RE_TexMapType
    {
	DIFFUSE,
	SPECULAR,
	BUMP,
	NORMAL,
	OPACITY,
	ENVIRONMENT,
	DISPLACEMENT,
	EMISSION,
	ROUGHNESS,
	OCCLUSION,
	METALLIC,
	COAT_INTENSITY,
	COAT_ROUGHNESS,
	REFLECTION_AMOUNT,

	HAIR_DIFFUSE,
	HAIR_DIFFUSE_BLEND,
	HAIR_SPECULAR,
	HAIR_SPECULAR_BLEND,
	HAIR_ALPHA,

	GAME_BLEND
    };

    virtual int			 getNumLayers() const   { return 1; }
    virtual void		 updateAllSources(RE_Material *mat,
						  const UT_Options *opts,
						  fpreal time) {} 
    virtual RE_TextureMap	*createTexture(RE_Render *r, int layer,
					       RE_TexMapType type,
					       UT_String &uvsetname);

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_TextureCallback.
    virtual int64 getMemoryUsage(bool inclusive) const = 0;

    static bool fetchRegOrUDIM(const UT_StringHolder &tex_name,
			       const UT_StringHolder &node_path,
			       RE_TextureMap *&map);
};

class RE_API RE_Material : public UT_NonCopyable
{
public:
    explicit RE_Material(const char *materialName);

    const UT_StringHolder &
		getMaterialName() const	{ return myMaterialName; }
    int		getUniqueId() const	{ return myUniqueId; }
    
    int64	getVersion() const	{ return myVersion; }
    void	setVersion(int64 v)	{ myVersion = v; }
    
    void	dirty(bool dirty = true) { myDirtyFlag = dirty; }
    bool	isDirty(fpreal time) const
		    {
			if(myDirtyFlag)
			    return true;
			if(myTimeDep && !SYSisEqual(time, myTime))
			    return true;
			return false;
		    }

    void	setTimeDependent(bool timedep, fpreal t = 0.0)
		    {
			myTimeDep = timedep;
			myTime = timedep ? t : 0.0;
		    }
    bool	isTimeDependent() const { return myTimeDep; }
    fpreal	getCurrentTime() const  { return myTime; }

    void	incref()	{ myRefCount.add(1); }
    void	decref()
		{
		    if (!myRefCount.add(-1))
			delete this;
		}

    void	 setNodePath(UT_StringHolder path, int node_id);
    const UT_StringHolder &
		 getNodePath() const { return myNodePath; }
    int		 getNodeId() const { return myNodeID; }

    // Parameter overrides
    void	 setOverrides(const UT_Options *overrides);
    const UT_Options  &getOverrides() const { return myOverrides; }
    bool	 hasOverrides() const { return myOverrides.getNumOptions()>0; }

    // Vertex array overrides
    void	 setVertexOverrides(const UT_StringMap<UT_StringHolder> &ovr)
					{ myVertexOverrides = ovr; }
    const UT_StringMap<UT_StringHolder> &vertexOverrides() const
					{ return myVertexOverrides; }

    enum RE_MaterialType
    {
	FILM_SURFACE,
	GAMES_SURFACE,
	HAIR,
	TERRAIN_SURFACE
    };
    void	 setMaterialType(RE_MaterialType type);
    RE_MaterialType getMaterialType() const { return myMaterialType; }

    void	 markDefaultShader(bool def) { myDefaultShader = def; }
    bool	 isDefaultShader() const  { return myDefaultShader; }

    void	 assignShader(RE_Shader *sh,
			      RE_ShaderTarget ptype = RE_SHADER_TARGET_TRIANGLE,
			      bool owned = true);
    void	 assignShader(UT_UniquePtr<RE_Shader> &&sh,
			      RE_ShaderTarget ptype=RE_SHADER_TARGET_TRIANGLE);

    // methods to deal with textures
    void	 clearTexture(RE_Render *r);
    void	 clearDiffuseMap();

    // General diffuse texture, 1 allowed per layer.
    RE_TextureHolder getTexture(RE_Render *r, int layer);

    // Extra textures, only 1 allowed for the first layer.
    RE_TextureHolder getSpecularMap(RE_Render *r);
    RE_TextureHolder getBumpMap(RE_Render *r);
    RE_TextureHolder getNormalMap(RE_Render *r);
    RE_TextureHolder getOpacityMap(RE_Render *r);
    RE_TextureHolder getDisplaceMap(RE_Render *r);
    RE_TextureHolder getEmissionMap(RE_Render *r);
    RE_TextureHolder getRoughnessMap(RE_Render *r);
    RE_TextureHolder getOcclusionMap(RE_Render *r);
    RE_TextureHolder getMetallicMap(RE_Render *r);
    RE_TextureHolder getCoatIntensityMap(RE_Render *r);
    RE_TextureHolder getCoatRoughnessMap(RE_Render *r);
    RE_TextureHolder getReflectionAmountMap(RE_Render *r);

    RE_TextureHolder getHairDiffuseMap(RE_Render *r);
    RE_TextureHolder getHairSpecularMap(RE_Render *r);
    RE_TextureHolder getHairDiffuseRamp(RE_Render *r);
    RE_TextureHolder getHairSpecularRamp(RE_Render *r);
    RE_TextureHolder getHairAlphaRamp(RE_Render *r);
   
    // Environment map to affect specular reflection. One allowed for layer 0.
    RE_TextureHolder getEnvMap(RE_Render *r);

    void	   clearTextureLayer(int layer);

    // In order for these to return the correct value, the corresponding
    // get..() method must be called first.
    bool	   hasSpecularMap() const { return mySpecMap.myTex.isValid(); }
    bool	   hasBumpMap() const { return myBumpMap.myTex.isValid(); }
    bool	   hasNormalMap() const { return myNormalMap.myTex.isValid(); }
    bool	   hasOpacityMap() const{ return myOpacityMap.myTex.isValid(); }
    bool	   hasDisplaceMap() const{return myDisplaceMap.myTex.isValid();}
    bool	   hasEnvMap() const { return myEnvMap.isValid(); }
    bool	   hasEmissionMap() const
			{ return myEmissionMap.myTex.isValid(); }
    bool	   hasRoughnessMap() const
			{ return myRoughMap.myTex.isValid(); }
    bool	   hasOcclusionMap() const
			{ return myOcclusionMap.myTex.isValid(); }
    bool	   hasMetallicMap() const
			{ return myMetallicMap.myTex.isValid(); }
    bool	   hasCoatRoughnessMap() const
			{ return myCoatRoughMap.myTex.isValid(); }
    bool	   hasCoatIntensityMap() const
			{ return myCoatIntMap.myTex.isValid(); }
    bool	   hasReflectionAmountMap() const
			{ return myReflectAmountMap.myTex.isValid(); }

    bool	   hasHairDiffuseMap() const{ return myHairDiffMap.isValid(); }
    bool	   hasHairSpecularMap() const{ return myHairSpecMap.isValid();}
    bool	   hasHairDiffuseRamp() const{ return myHairDiffRamp.isValid();}
    bool	   hasHairSpecularRamp() const{return myHairSpecRamp.isValid();}
    bool	   hasHairAlphaRamp() const{ return myHairAlphaRamp.isValid(); }

    // Query the material if it is opaque or transparent. 'check_diffuse_alpha'
    // will check the diffuse texture's alpha channel.
    bool	   isTransparent(bool check_diffuse_alpha) const;

    // Returns true if this material has a texture map (diffuse, bump, etc).
    bool	   isTextured() const	{ return myTexturedFlag; }

    // Returns true if textures are used, and the uvsets used by all layers of
    // the 
    bool	   getUVSets(UT_Map<UT_StringHolder, int> &uvsets) const;

    // Return the texture coord set used by 'layer'
    const UT_StringHolder &getLayerUVSet(int layer) const;

    // This allows the creator of this object to hook up a callback
    // with which an instance of RE_Material could ask for a texture
    // if and when the texture is needed.  The presence of a callback
    // marks this material as being textured, so only set when necessary.
    void		 setCallback(RE_TextureCallback *cb);
    RE_TextureCallback	*getCallback()	{ return myTexCallback; }

    void	 setTextureFormat(RE_TextureCompress format);
    RE_TextureCompress getTextureFormat() const	{ return myTexFormat; }
    
    void	 limitTextureSize(bool limit, int w, int h);
    bool	 limitTextureSize() const	{ return myLimitTextureSize; }
    void	 getMaxTextureSize(int &w,int &h)
					        { myMaxTextureW = w;
						  myMaxTextureH = h; }
    
    void	 setTextureScale(float sc);
    float	 getTextureScale() const	{ return myTextureScale; }

    void	 setTextureMemLimitMB(int mem);
    int		 getTextureMemLimitMB() const   { return myTextureMemLimitMB; }
    
    void	 mipMapTextures(bool yes);
    bool	 mipMapTextures() const		{ return myMipMapTextures; }

    void	 setAnisotropySamples(int samp);
    int		 getAnisotropySamples() const	{ return myTextureAnisotropy; }

    void	 allowShaders(bool yes)		{ myAllowShaders = yes; }
    bool	 allowShaders() const		{ return myAllowShaders; }

    // methods to deal with lighting
    void	 doLighting(bool yes );
    bool	 isLit() const			{ return myDoLighting; }

    RE_Shader   *createShader(RE_Render *r,
			      const char *name = 0,
			      RE_ShaderTarget ptype= RE_SHADER_TARGET_TRIANGLE);
    RE_Shader	*getShader(RE_ShaderTarget sht=RE_SHADER_TARGET_TRIANGLE) const
						{ return myShader[sht]; }
    bool	 isShaderOwned(RE_ShaderTarget sht
			        = RE_SHADER_TARGET_TRIANGLE) const
		    { return myOwnShader[sht]; }

    /// caches the source for the shader in the material.
    void	 setShaderSource(const char *vert_src,
				 const char *geom_src,
				 const char *frag_src,
				 RE_ShaderTarget ptype
				                 = RE_SHADER_TARGET_TRIANGLE);
    
    /// checks this materials' shader against the source, and returns true if
    /// it matches.
    bool	 hasShaderSource(const char *vert_src,
				 const char *geom_src,
				 const char *frag_src,
				 RE_ShaderTarget ptype
				                 = RE_SHADER_TARGET_TRIANGLE);

    void	 setShaderProgram(const char *program_file);
    bool	 hasShaderProgram(const char *program_file) const;
    bool	 hasCustomShader(RE_ShaderTarget ptype
				 = RE_SHADER_TARGET_TRIANGLE) const
				{ return myShaderProgramFile.isstring() ||
					myVertexSource[ptype].isstring() ||
					myGeometrySource[ptype].isstring() ||
					myFragmentSource[ptype].isstring(); }
    bool	 customShaderDirty() const { return myCustomShaderDirty; }
    
    void	 removeShader();

    // methods to access and change the material information
    float	 amb( unsigned idx ) const	{ return myAmb[idx]; }
    UT_Vector3F	 amb() const			{ return myAmb; }
    float	 diff( unsigned idx ) const	{ return myDiff[idx]; }
    UT_Vector3F	 diff() const			{ return myDiff; }
    float	 spec( unsigned idx ) const	{ return mySpec[idx]; }
    UT_Vector3F	 spec() const			{ return mySpec; }
    float	 emit( unsigned idx ) const	{ return myEmit[idx]; }
    UT_Vector3F  emit() const			{ return myEmit; }
    UT_Vector3F  metallicEdgeTint() const	{ return myMetallicEdge; }
    float	 metallic() const		{ return myMetallic; }
    float	 alpha() const			{ return myAlpha; }
    float	 roughness() const		{ return myRoughness; }
    float	 reflection() const		{ return myReflection; }
    float	 coatRoughness() const		{ return myCoatRough; }
    float	 coatIntensity() const		{ return myCoatIntensity; }
    float	 diffuseRoughness() const	{ return myDiffRoughness; }

    /// Sets the perpendicular and parallel alpha factors. If the parallel
    /// alpha is less than zero, they are assumed to be the same.
    void	 setAlpha( float alpha_perp, float alpha_para = -1.0 );

    /// If true, the shader generates its own alpha independent of the material
    /// parameters. This indicates that the material requires a transparency
    /// pass, since the renderer can otherwise not determine this from the
    /// shader code.
    void	 setShaderAlpha(bool enabled)
					{ myShaderAlphaFlag = enabled; }
    bool	 hasShaderAlpha() const { return myShaderAlphaFlag; }
   
    // give roughness: this method will calculate shininess needed by OGL
    void	 setRoughness( float r );

    // give shininess: this method will enforce sh within [0.001, 1e10]
    void	 setShininess( float sh );

    void	 setDiffuseRoughness(fpreal r);

    void	 setAmb( float r, float g, float b );
    void	 setDiff( float r, float g, float b );
    void	 setSpec( float r, float g, float b );
    void	 setEmit( float r, float g, float b );
    void	 setMetallic(float metal); 
    void	 setMetallicEdge(const UT_Vector3F &metal);

    void	 setAmbient( UT_Color col );
    void	 setDiffuse( UT_Color col );
    void	 setSpecular( UT_Color col );
    void	 setEmission( UT_Color col );
    
    void	 setAmbient( const UT_Vector3F &col );
    void	 setDiffuse( const UT_Vector3F &col );
    void	 setSpecular( const UT_Vector3F &col );
    void	 setEmission( const UT_Vector3F &col );

    void	 setSpecularTint(fpreal t);
    fpreal	 getSpecularTint() const { return mySpecTint; }

    enum { PHONG=0, BLINN=1, GGX=2, SPEC_ANGLE=3 };
    void	 setSpecularModel(int spec_model);
    int		 getSpecularModel() const { return mySpecModel; }
    void	 setCoatSpecularModel(int spec_model);
    int		 getCoatSpecuarModel() const { return myCoatModel; }
    void	 setCoatIntensity(fpreal i);
    fpreal	 getCoatIntensity() const { return myCoatIntensity; }
    void	 setCoatRoughness(fpreal r);
    fpreal	 getCoatRoughness() const { return myCoatRough; }
    
    void	 setIOR(float ior);
    fpreal	 getIOR() const { return myIOR; }

    void	 setReflection(float r, bool explicit_r);
    fpreal	 getReflection() const;

    void	 usePackedColor(bool enable);
    void	 useGeometryColor(bool enable);
    bool	 usePackedColor() const { return myUsePackedColor; }
    bool	 useGeometryColor() const { return myUseGeoColor; }

    // returns true if reflection was explicity set, not derived from IoR.
    bool	 hasExplicitReflection() const { return myExplicitReflection; }

    // Parameters for the bump, spec, opac, env, disp and normal maps.
    enum re_NormalMapType { NMLMAP_TANGENT=0, NMLMAP_WORLD=1, NMLMAP_OBJECT=2 };

    void	 setOpacityMapParms(bool invert);
    void	 setNormalParms(re_NormalMapType type, bool bias,
				UT_Vector2F scale,
				bool flip_x, bool flip_y, bool flip_z);
    void	 setEnvParms(float scale, const UT_Vector3 &rot,
			    const UT_XformOrder &rord);

    enum re_Displace {
	DISPLACE_NORMAL,
	DISPLACE_VECTOR_TANGENT,
	DISPLACE_VECTOR_WORLD,
	DISPLACE_VECTOR_OBJECT };
    void	 setDisplaceParms(float scale, float offset,
				  re_Displace vector_space, bool y_up);
    void	 setRoughParms(bool invert_map, int component);
    void	 setOcclusionParms(int component);
    void	 setMetallicParms(int component);
    void	 setCoatRoughnessParms(int component);
    void	 setCoatIntensityParms(int component);
    void	 setReflectionAmountParms(int component, bool affect_ior);

    // Hair parameters
    void	 enableHairParameters(bool e) { myHairMaterialEnabled = e; }
    
    void	 setHairDiffuse(float intensity,
				UT_Vector3F root_col,
				UT_Vector3F tip_col,
				bool use_map,
				bool use_ramp);
    
    void	 setHairSpecular(float intensity,
				 float angle,
				 float shift,
				 UT_Vector3F root_col,
				 UT_Vector3F tip_col,
				 bool use_map,
				 bool use_ramp);
    void	setHairAlpha(float alpha,
			     bool use_alpha_ramp);

    // randomize hue, sat, and intensity of diffuse (and specular intensity)
    void	setHairRandomize(bool random_hue, UT_Vector2F hue_range,
				 bool random_sat, UT_Vector2F sat_range,
				 bool random_int, UT_Vector2F int_range,
				 bool random_spec_int,
				 UT_Vector2F spec_int_range);

    // Game shader

    // Use a transformed UV set.
    void	setUVTransform(int layer,
			       UT_Vector2F t,
			       fpreal	   r,
			       UT_Vector2F s);
    void	getUVTransform(int layer,
			       UT_Vector2F &t,
			       fpreal	   &r,
			       UT_Vector2F &s ) const;
    void	setUVLayerTint(int layer, bool use_tint, UT_Vector4F tint);

    // Use triplanar projection.
    void	setUVTriplanar(int layer,
			       fpreal scale = 1.0,
			       fpreal sharp = 1.0,
			       UT_Vector3F blend =UT_Vector3F(1.0f),
			       UT_Vector3F offset = UT_Vector3F(0.0f));
    bool	isUVTriplanar(int layer) const;
    fpreal	getUVTriplanarSharp(int layer) const;
    fpreal	getUVTriplanarScale(int layer) const;
    UT_Vector3F	getUVTriplanarBlend(int layer) const;
    UT_Vector3F	getUVTriplanarOffset(int layer) const;
    bool	getUVLayerUseTint(int layer) const;
    UT_Vector4F	getUVLayerTint(int layer) const;


    // For environment lights, which temporarily apply their map to any
    // materials that don't already have an env map.
    static void			setEnvMapOverride(RE_TextureHolder env_map,
						  const UT_Matrix3F &rot,
						  const UT_Vector3F &col);
    static void			clearEnvMapOverride();
    static RE_TextureHolder	getEnvMapOverride();

    void	setReflectionMap(RE_Texture *map) { myReflectMap = map; }
    void	clearReflectionMap()		  { myReflectMap = NULL; }
    
#ifdef UT_DEBUG
    // debug output
    void	 output();
#endif

    RE_MaterialPtr	cloneWithoutTexture() const;

    void	 loadTextureMaps(RE_Render *r);

    // Adds an explicit map to 'maptype' which is not overriden by the
    // RE_TextureCallback. NULL is acceptable (blocks the RE_TextureCB map).
    void	 assignTextureMap(RE_Texture *tex,
				  RE_TextureCallback::RE_TexMapType maptype,
				  int index = 0);
    // Remove the explicit map from 'maptype', allowing RE_TextureCallback to
    // be called for the map.
    void         removeTextureMap(RE_TextureCallback::RE_TexMapType maptype,
				  int index = 0);

    int		 getNumDiffuseLayers() const
		 { return myMaterialType==FILM_SURFACE ?myNumDiffuseLayers :1; }

    bool	 updateShaderForMaterial(RE_Render *r,
			       int layer,
			       bool lit,
			       bool textured,
			       RE_ShaderTarget sht=RE_SHADER_TARGET_TRIANGLE,
			       RE_Shader *shader = NULL);
    void	 restoreShader(RE_Render *r,
			       RE_ShaderTarget sht=RE_SHADER_TARGET_TRIANGLE);

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_Material.
    int64	 getMemoryUsage(bool inclusive) const;


    void	 writeMaterialParmsToBlockGL4(RE_Render *r,
					      RE_UniformBlock *block,
					      int material_idx,
					      int layer,
					      bool lit,
					      bool textured);
    // For bindless texturing.
    void	 makeTexturesResident(RE_Render *r);
    void	 removeTexturesFromResident(RE_Render *r);

    DEP_MicroNode &dependencies() { return myDepNode; }

    const RE_UniformBlockHandle &materialBlock() const
				{ return myMaterialBlock; }
    const RE_UniformBlockHandle &hairBlock() const
				{ return myHairMaterialBlock; }
    const RE_UniformBlockHandle &gameMatBlock() const
				{ return myGameMaterialBlock; }
private:

    void	 loadShaderAttribNames();

    class re_MaterialTextureMap
    {
    public:
	re_MaterialTextureMap()
	    : myMap(NULL), myHasMap(0), myMapIndex(-1),
	      myArrayMapIndex(-1), myTexCoords(0), myComponent(0),
	      myExplicitMap(false) {}

	RE_TextureMap	*myMap;
	RE_TextureHolder myTex;
	UT_StringHolder	 myUVSet;
	int		 myHasMap;
	UT_DimRect	 myUDIMArea;
	int		 myMapIndex;
	int		 myArrayMapIndex;
	int		 myTexCoords;
	int		 myComponent;
	bool		 myExplicitMap;
    };
    re_MaterialTextureMap *fetchMapInfo(RE_TextureCallback::RE_TexMapType t,
					int index);

    RE_TextureHolder fetchTexture(RE_Render *r, RE_TextureMap *map,
				  bool allow_mipmap = true);
    RE_TextureHolder fetchMap(RE_Render *r,
			      RE_TextureCallback::RE_TexMapType map_target,
			      re_MaterialTextureMap &map,
			      bool promote_1chan_to_rgb = false);

    bool	 checkForMap(re_MaterialTextureMap &map, bool textured);
    bool	 bindTextureMap(RE_Render *r, re_MaterialTextureMap &map,
				const char *udim_stride_name,
				const char *component_name = NULL);
private:
    ~RE_Material();	// No subclassing

    void	 updateHairMaterialForShader(RE_Render	*r,
					     bool	 textured,
					     RE_ShaderTarget sht,
					     RE_Shader	*sh);
    void	 updateGameMaterialForShader(RE_Render *r,
					     bool textured,
					     RE_ShaderTarget sht,
					     RE_Shader	*sh);

    // -- Bindless texturing support
    void	 makeResident(RE_Render *r, RE_TextureHolder texmap);
    void	 removeResident(RE_Render *r, RE_TextureHolder texmap);
    bool	 bindTextureHandle(RE_Render *r,
				   re_MaterialTextureMap &map,
				   RE_UniformBlock *material_block,
				   int material_index,
				   const char *texture_name);
    // --
    
    UT_Vector4F	 getComponentVector(int comp) const;
    
    DEP_MicroNode		 myDepNode;
    RE_TextureCallback		*myTexCallback;
    UT_Vector3F			 myAmb;
    UT_Vector3F			 myDiff;
    UT_Vector3F			 mySpec;
    UT_Vector3F			 myEmit;
    UT_Vector3F			 myMetallicEdge;
    float			 myMetallic;
    float			 myAlpha;
    float			 myRoughness;
    float			 myDiffRoughness;
    float			 mySpecTint;
    RE_MaterialType		 myMaterialType;

    // I own map if if I created my RE_OGLTex
    UT_ValArray<re_MaterialTextureMap> myTextureMap;
    
    RE_TextureHolder	 myEnvMap;
    RE_Texture		*myReflectMap; // always temporary during a render

    RE_TextureHolder	 myHairDiffMap;
    RE_TextureHolder	 myHairSpecMap;
    RE_TextureHolder	 myHairDiffRamp;
    RE_TextureHolder	 myHairSpecRamp;
    RE_TextureHolder	 myHairAlphaRamp;

    re_MaterialTextureMap mySpecMap;
    re_MaterialTextureMap myOpacityMap;
    re_MaterialTextureMap myBumpMap;
    re_MaterialTextureMap myRoughMap;
    re_MaterialTextureMap myEmissionMap;
    re_MaterialTextureMap myNormalMap;
    re_MaterialTextureMap myDisplaceMap;
    re_MaterialTextureMap myOcclusionMap;
    re_MaterialTextureMap myMetallicMap;
    re_MaterialTextureMap myCoatRoughMap;
    re_MaterialTextureMap myCoatIntMap;
    re_MaterialTextureMap myReflectAmountMap;
    re_MaterialTextureMap myGameBlendMap;


    // The above maps are only valid pointers if myRenderCount is equal to
    // RE_OGLRender::getRenderCount.
    int64		 myRenderCount;
    

    UT_StringHolder	 myMaterialName;
    UT_StringHolder	 myNodePath;
    int			 myNodeID;
    UT_Options		 myOverrides;
    UT_StringMap<UT_StringHolder> myVertexOverrides;
    unsigned		 myDoLighting:1,
			 myMipMapTextures:1,
			 myLimitTextureSize:1,
			 myDefaultShader:1,
			 myAllowShaders:1,
			 myTrivialTextureAlpha:1,
			 myInvertRoughMap:1,
			 myCustomShaderDirty:1;
    int			 myTextureAnisotropy;
    int			 myNumDiffuseLayers;

    RE_TextureCompress	 myTexFormat;
    int			 myMaxTextureW, myMaxTextureH;
    float		 myTextureScale;
    int			 myTextureMemLimitMB;
    float		 myAlphaPara;
    bool		 myShaderAlphaFlag;
    bool		 myUseGeoColor;
    bool		 myUsePackedColor;
    bool		 myDirtyFlag;

    // Specular parms
    fpreal		 myIOR;
    fpreal32		 myReflection;
    int			 mySpecModel;
    bool		 myReflectAsIOR;
    bool		 myExplicitReflection;

    // Coat parms
    fpreal		 myCoatRough;
    fpreal		 myCoatIntensity;
    int			 myCoatModel;

    // normal map parms
    bool		 myNormalBias;
    bool		 myNormalFlipX;
    bool		 myNormalFlipY;
    bool		 myNormalFlipZ;
    UT_Vector2F		 myNormalScale;
    re_NormalMapType	 myNormalMapType;

    // environment map parms
    UT_Vector3		 myEnvRotation;
    float		 myEnvScale;
    UT_XformOrder	 myEnvRotOrder;

    // displacement map parms
    float		 myDisplaceScale;
    float		 myDisplaceOffset;
    re_Displace		 myDisplaceSpace;
    bool		 myDisplaceYUp;

    bool		 myOpacityMapInvert;
    int			 myRoughComp;
    int			 myOcclusionComp;
    int			 myMetallicComp;

    RE_Shader		*myShader[RE_SHADER_NUM_TARGETS];
    bool		 myOwnShader[RE_SHADER_NUM_TARGETS];
    UT_String		 myVertexSource[RE_SHADER_NUM_TARGETS];
    UT_String		 myGeometrySource[RE_SHADER_NUM_TARGETS];
    UT_String		 myFragmentSource[RE_SHADER_NUM_TARGETS];
    UT_String		 myShaderProgramFile;

    RE_UniformBlockHandle myMaterialBlock;
    int			 myMaterialOverride;
    bool		 myMatBlockDirty;
    bool 		 myMatBlockHasEnvMapTexture;
    bool 		 myMatBlockHasReflectMapTexture;
    bool		 myMatBlockHasDiffuseTexture;
    bool		 myTexturedFlag;

    // Hair parameters
    class re_HairCompParms
    {
    public:
	re_HairCompParms() : intensity(0.5), shift(0.0), angle(1.0),
			     root_col(1,1,1), tip_col(1,1,1),
			     use_ramp(false), use_map(false) {}
	float intensity;
	float angle;
	float shift;
	UT_Vector3F root_col;
	UT_Vector3F tip_col;
	bool use_ramp;
	bool use_map;
    };
    struct re_HairRandomParms
    {
	re_HairRandomParms() : randomize(false) {}
	bool randomize;
	UT_Vector2F range;
    };

    bool		 myHairMaterialEnabled;
    re_HairCompParms	 myHairDiffuse;
    re_HairCompParms	 myHairSpecular;
    float		 myHairAlpha;
    bool		 myHairUseAlphaRamp;
    re_HairRandomParms	 myHairRandomDiffHue;
    re_HairRandomParms	 myHairRandomDiffSat;
    re_HairRandomParms	 myHairRandomDiffInt;
    re_HairRandomParms	 myHairRandomSpecInt;
    RE_UniformBlockHandle myHairMaterialBlock;
    int			 myHairMatBlockIndex;
    int			 myHairRandomTextureIndex;
    int			 myHairDiffBaseMapIndex;
    UT_DimRect		 myHairDiffBaseMapUDIMArea;
    UT_DimRect		 myHairSpecBaseMapUDIMArea;

    RE_TextureHolder	 myRandomTexture;

    // cached texture units for 'myLastMatShader'.
    void		*myLastMatShader;
    int			 myLastMatShaderSerial;
    
    int			 myMatBlockDiffuseIndex;
    int			 myMatBlockDiffuseArrayIndex;
    
    int 		 myMatBlockEnvMapIndex;
    int 		 myMatBlockReflectMapIndex;
    int			 myMatBlockIndex;
    int			 myHairMaterialOverride;

    // hair material block.
    void		*myLastHairShader;
    int			 myLastHairShaderSerial;
    int			 myHairDiffIndex;
    int			 myHairDiffArrayIndex;
    int			 myHairSpecIndex;
    int			 myHairSpecArrayIndex;
    int			 myHairDiffRampIndex;
    int			 myHairSpecRampIndex;
    int			 myHairAlphaRampIndex;
    int			 myMatBlockHairIndex;

    // game shader material block
    RE_UniformBlockHandle myGameMaterialBlock;
    int			  myGameMaterialOverride;
    
    struct re_GameLayer
    {
	re_GameLayer() : triplanar(false), scale(1,1), trans(0,0), rot(0),
			 sharp(1), unit(-1), tpscale(1), transform(1.0),
			 tpblend(1,1,1), tpoff(0,0,0), use_tint(false),
			 tint(1.0, 1.0, 1.0, 1.0) {}
	bool	    triplanar;
	UT_Vector2F scale;
	UT_Vector2F trans;
	float	    rot;
	float	    sharp;
	float       tpscale;
	UT_Vector3F tpblend;
	UT_Vector3F tpoff;
	UT_Matrix3F transform;
	bool	    use_tint;
	UT_Vector4F tint;

	// unit for shader
	int	    unit;
    };
    UT_Array<re_GameLayer> myGameLayers;
    const RE_Shader	*myLastGameShader;
    int			 myLastGameShaderSerial;
    int			 myGameBlendIndex;
    int			 myGameMatBlockIndex;

    int			 myPrevSeamlessCubeMap;
    
    bool		 myTimeDep;
    fpreal		 myTime;

    SYS_AtomicInt32	 myRefCount;
    int64		 myUniqueId;
    int64		 myVersion;
};

static inline void intrusive_ptr_add_ref(RE_Material *m) { m->incref(); }
static inline void intrusive_ptr_release(RE_Material *m) { m->decref(); }

inline void
RE_Material::setAmbient( const UT_Vector3F &col )
{
    setAmb(col.x(), col.y(), col.z());
}
inline void
RE_Material::setDiffuse( const UT_Vector3F &col )
{
    setDiff(col.x(), col.y(), col.z());
}
inline void
RE_Material::setSpecular( const UT_Vector3F &col )
{
    setSpec(col.x(), col.y(), col.z());
}
inline void
RE_Material::setEmission( const UT_Vector3F &col )
{
    setEmit(col.x(), col.y(), col.z());
}


// This contains a list of materials for drawing a given geometry, and is
// normally used as a transient list of materials during a redraw.
// This class does not own the materials as they are also held in
// a higher-level cache as well.
class RE_API RE_MaterialAtlas : public UT_Map<int,RE_MaterialPtr>
{
public:
    // Default material for unassigned prims.
    const RE_MaterialPtr &getDefaultMaterial() const
		{ return myDefault; }

    // Factory default material if materials are disabled.
    const RE_MaterialPtr &getFactoryMaterial() const
		{ return myFactory; }
    
    bool	isDirty(fpreal time) const;

    RE_MaterialPtr get(int material_id) const
		{
		    if(material_id == -1)
			return myDefault ? myDefault : myFactory;
		    const_iterator entry =  find(material_id);
		    if(entry != end())
			return entry->second;

		    return NULL;
		}


    // Add a new material to the atlas
    void   add(const RE_MaterialPtr &material)
		{ (*this)[ material->getUniqueId() ] = material; }

    // Set the default material, which will be used if no other material
    // assignments are present.
    void   setDefaultMaterial(const RE_MaterialPtr &material,bool add_list=true)
	    { myDefault = material; if(add_list && material) add(material); }

    // Sets the factory default material, which is used when all materials
    // are turned off (as the default material may be the object material).
    void   setFactoryMaterial(const RE_MaterialPtr &material,bool add_list=true)
	    { myFactory = material; if(add_list && material) add(material); }

private:
    RE_MaterialPtr	       myDefault;
    RE_MaterialPtr	       myFactory;
};

#endif
