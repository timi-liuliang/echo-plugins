/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_Light.h (C++)
 *
 * COMMENTS:	Header file for RE_Light and RE_Lights
 *
 *    Defines all the parameters required for rendering any Houdini light in
 *    the viewport. 
 */

#ifndef __RE_Light_H__
#define __RE_Light_H__

#include "RE_API.h"
#include "RE_Types.h"
#include <string.h>
#include <SYS/SYS_Types.h>
#include <UT/UT_String.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Matrix4.h>
#include "RE_TextureMap.h"
#include "RE_UniformBlock.h"

class UT_Ramp;
class UT_Raster;
class RE_Texture2DMap;
class RE_TextureCubeMap;
class RE_Render;
class RE_ShadowMap;
class RE_Shader;

#define RE_SHADOW_MAX_AREA_MAPS		8
#define RE_SHADOW_NUM_CUBE_MAPS		6

// THIS ENUM MUST BE IN-SYNC WITH OBJ_LightAreaShape in RE_Light.h.
enum RE_LightAreaShape
{
    RE_LIGHT_AREA_SHAPE_LINE,
    RE_LIGHT_AREA_SHAPE_GRID,
    RE_LIGHT_AREA_SHAPE_CUBE,
    RE_LIGHT_AREA_SHAPE_DISK,
    RE_LIGHT_AREA_SHAPE_SPHERE
};

class RE_API RE_LightData
{
public:
	     RE_LightData();
    virtual ~RE_LightData();
};

class RE_API RE_LightFlag
{
public:
    RE_LightFlag()
    {
	cone 	     	  = 0;
	infinite 	  = 0;
	ambient 	  = 0;
	mapprojector 	  = 0;
	clampprojectormap = 0;
	mipmap		  = 0;
	projectormapdirty = 0;
	envlight	  = 0;
	area		  = 0;
	portal		  = 0;
	globillum	  = 0;
	enabled		  = 1;
	headlight	  = 0;
    }

    unsigned	mapprojector:1,
		cone:1,
		infinite:1,
		ambient:1,
		clampprojectormap:1,
		projectormapdirty:1,
		mipmap:1,
		envlight:1,
		area:1,
		portal:1,
		globillum:1,
		enabled:1,
		headlight:1;
};


class RE_API RE_Light {
public:
	     RE_Light(const char *n = 0);
    virtual ~RE_Light();

    const char		*getName() const { return myName; }
    void		 setName(const char *name) { myName.harden(name); }

    void		 enable(bool e)    { flag.enabled = e; }
    bool		 isEnabled() const { return flag.enabled; }

    // Similar to isEnabled(), but takes into account color=(0,0,0) and other
    // factors
    bool		 isLightContributing() const;

    void		 setHeadlight(bool hl) { flag.headlight = hl; }
    bool		 isHeadlight() const { return flag.headlight; }

    const UT_Vector3	&getPosition() const { return myPos; }
    void		 setPosition(const UT_Vector3 &p)
			 { if(p != myPos) { myPos = p; invalidateShadowMap(); }}

    const UT_Vector3	&getDirection() const { return myDir; }
    void		 setDirection(const UT_Vector3 &d)
			 { if(d != myDir) { myDir = d; invalidateShadowMap(); }}
    void		 setOrientation(UT_Matrix3 &orient)
			 { if(orient != myOrientation)
			     { myOrientation = orient; invalidateShadowMap(); }
			 }
    const UT_Matrix3	&getOrientation() { return myOrientation; }

    const UT_Vector3F	&getColor() const { return myColor; }
    UT_Vector3		&getColor() { return myColor; }
    void		 setColor(const UT_Vector3 &c);

    const UT_Matrix4	&getTransform() const { return xform; }
    UT_Matrix4		&getTransform() { return xform; }
    void		 setTransform(const UT_Matrix4 &m)
			{ if(xform != m) { xform = m; invalidateShadowMap(); } }

    int		 	 isCone() const { return flag.cone; }
    int			 isInfinite() const { return flag.infinite; }
    int			 isAmbient() const { return flag.ambient; }
    int		 	 isMapProjector() const { return flag.mapprojector; }
    bool		 isEnvironmentLight() const { return flag.envlight; }
    bool		 isPortalLight() const { return flag.portal; }
    bool		 isGlobalIllumination() const { return flag.globillum; }


    bool		 isAreaLight() const { return flag.area; }
    UT_Vector3		 getAreaAxes() const { return myAreaAxes; }
    UT_Matrix4		 getAreaTransform() const { return myAreaTransform; }
    float		 getSurfaceArea() const { return mySurfaceArea; }
    bool		 isAreaSingleSided() const { return myAreaSingleSided; }
    bool		 isAreaSingleSideReversed() const
						{ return myAreaSingleReverse; }
    int			 isDiffuse() const	{ return myContribDiff; }
    int			 isSpecular() const	{ return myContribSpec; }

    bool		 isShadowed() const	{ return myUseShadows; }

    // bumped whenever light geometry is changed
    virtual int64	 getVersion() const	{ return 0; }

    // bumped whenever emitted light is changed 
    int64		 getEmitVersion() const { return myEmitSerial; }

    /// Can be used by OP_Node::lookupNode() to return the light object
    /// this light is attached to.
    virtual int		 getUniqueId() const	{ return -999; }
    
    //
    //  Light attribute settings, infinite, point, and cone light
    //
    void		 setInfinite(int onoff);
    void		 setAmbient(int onoff);
    void		 setCone(int onoff);
    void		 setArea(bool onoff);
    void		 setEnvironment(bool onoff);
    void		 setPortal(bool onoff);
    void		 setGlobalIllumination(bool onoff);
    void		 setAreaAxes(const UT_Vector3F &axes);
    void		 setAreaTransform(UT_Matrix4 t);
    void		 setAreaSide(bool single, bool reverse);
    void		 setAreaSamplePoints(const UT_Vector3FArray &pos,
					     const UT_Vector3FArray &dir);
    void		 buildAreaShape(int numsample);
    void		 setSurfaceArea(float area);
    void		 setDiffuse(int onoff);
    void		 setSpecular(int onoff);
    void		 setConeAttrib(float a = 45, float d = 10, float r = 1)
			 {
			     if(coneAngle != a ||
				coneDelta != d ||
				coneRoll  != r)
			     {
				 coneAngle = a;
				 coneDelta = d;
				 coneRoll = r;
				 setCone((a*.5 + d >= 180) ? 0 : 1);
				 invalidateShadowMap();
				 recomputeFalloffRamp();
			     }
			 }
    float		 getConeAngle() const { return coneAngle; }
    float		 getConeDelta() const { return coneDelta; }
    float		 getConeRoll() const { return coneRoll; }
    
    RE_Texture	*getFalloffRamp(RE_Render *r);

    // Attenuation
    void		 setAttenuation(float constant,
					float linear,
					float quadratic);
    void		 getAttenuation(float atten[3]) const
		         { atten[0] = myAtten[0];
			   atten[1] = myAtten[1];
			   atten[2] = myAtten[2]; }
    void		 setAttenuationRange(float near_dist, float far_dist);
    void		 getAttenuationRange(float range[2]) const
			 { range[0] = myAttenRange[0];
			   range[1] = myAttenRange[1];  }

    void		 setAttenuationRamp(const UT_Ramp *ramp,
					    float start_dist, float end_dist);
    RE_Texture		*getAttenuationRamp(RE_Render *r);
    void		 getAttenuationRampRange(float range[2]) const
			 { range[0] = myAttenRampStart;
			   range[1] = myAttenRampEnd;   }
    
			 
    // Shadows
    void		 setShadowed(bool onoff);
    void		 setShadowIntensity(float i) { myShadowIntensity = i; }
    void		 setShadowBias(float b)	 { myShadowBias = b; }
    void		 setShadowBlur(float b)  { myShadowBlur = b; }
    void		 setShadowQuality(float q) { myShadowQuality=q; }
    void		 setLightShadowMapSize(int s) { myLightShadowMapSize=s;}
    void		 setShadowMapSize(int s);
    void		 setAreaShadowShape(RE_LightAreaShape shape);
    void		 setShadowMask(const char *m);
    void		 setLinearShadowMap(bool linear);

    float		 getShadowIntensity() const { return myShadowIntensity;}
    float		 getShadowBias() const	    { return myShadowBias; }
    float		 getShadowBlur() const      { return myShadowBlur; }
    float		 getShadowQuality() const   { return myShadowQuality; }
    int			 getShadowMapSize() const   { return myShadowMapSize; }
    int			 getLightShadowMapSize() const
						{ return myLightShadowMapSize; }
    int			 getNumAreaMaps() const;
    const char		*getShadowMask() const { return myShadowMask; }
    bool		 getLinearShadowMap() const;
    
    // Only valid once the shadowmap exists.
    void		 getShadowZRange(fpreal &n, fpreal &f);

    // Shadow map usage
    RE_Texture		*getShadowMap(int area_index = 0);
    void		 setMultiMap(bool m);
    void		 useShadowMap(RE_Render *r, RE_Shader *shadow_shader,
				      bool onoff);
    bool		 beginShadowRender(RE_Render *r,
					   RE_TextureCubeFace face
					      = RE_TEXTURE_CUBE_POS_X,
					   int area_index = 0);
    void		 endShadowRender(RE_Render *r,
					 RE_TextureCubeFace face
					    = RE_TEXTURE_CUBE_POS_X,
					 int area_index = 0);
    void		 setShadowMapVersion(uint64 version);
    int64		 getShadowMapVersion() const;
    void		 setShadowMapTime(fpreal t);
    fpreal		 getShadowMapTime() const;
    
    // Returns true if 6 separate shadow maps are used instead of a cube map,
    // for hardware that does not support cube shadow maps.
    bool		 useSeparatePointShadowMaps(RE_Render *r) const;
    
    //
    //  Other attributes (like zoom)
    void		 setZoom(float z);
    void		 setFocalLength(float focal);
    void		 setAperture(float aper);
    void		 setOrthoWidth(float width);
    void		 setNearDistance(float znear);
    void		 setFarDistance(float zfar);
    
    float		 getZoom() const { return zoom; }
    float		 getFocalLength() const      { return myFocalLength; }
    float		 getAperture() const         { return myAperture; }
    float		 getOrthoWidth() const       { return myOrthoWidth; }
    float		 getNearDistance() const     { return myNearDist; }
    float		 getFarDistance() const	     { return myFarDist; }

    int64		 getLightVersion() const     { return myLightVersion; }
    void		 bumpLightVersion()	     { myLightVersion++; }
    
    int			 getConeTextureIndex() const  
    						{ return myGLConeTextureIndex;}
    void		 setConeTextureIndex(int idx) 
    						{ myGLConeTextureIndex = idx; }

    //
    //  Methods to set up different texture maps used by lights
    void		 setProjectMap(const char *name,
				       const char *relativeTo); 
    void		 setProjectMapClamp(int onoff);
    void		 setProjectMapFormat(RE_TextureCompress format);
    void		 setProjectMapScale(float sc, bool limitres,
					    int maxw, int maxh);
    void		 setProjectMapMipmap(bool onoff);
    void		 setProjectMapAnisotropy(int samples);

    RE_Texture2DMap	*getProjectMap(RE_Render *r);

    void		 setEnvironmentMap(const char *name,
					   const char *relativeTo);
    void		 setEnvironmentFormat(RE_TextureCompress format);
    void		 setEnvironmentMipmap(bool onoff);
    void		 setEnvironmentCone(float angle);
    RE_TextureHolder	 getEnvironmentMap(RE_Render *r, int max_width);
    float		 getEnvironmentCone() const;

    //
    // Uniform blocks for lights
    void		 updateBasicLighting(RE_Render *r,
					     RE_Shader *sh,
					     int index);
    void		 updateBasicBlock(RE_UniformBlock *lighting_block,
					  int light_index,
					  const char *light_prefix);
    void		 updateHQLighting(RE_Render *r, RE_Shader *sh);
    void		 updateAreaLighting(RE_Render *r, RE_Shader *sh);
    void		 updateShadow(RE_Render *r, RE_Shader *sh);
    
    static void		 clearLight(RE_Shader *sh, int index);

    /// @brief Allows data, like shadow maps, to be stored with the light
    /// Attaching data to the light gives it ownership, meaning that it will be
    /// deleted when the light is deleted, or when a different data chunk is
    /// assigned to the same index. To avoid it being deleted, detach the chunk.
    /// @{
    void		 attachUserData(RE_LightData *data, int index);
    RE_LightData	*detachUserData(int index);
    RE_LightData	*getAttachedUserData(int index) const;
    /// @}

    /// @brief Accessor to underlaying shadow map object
    /// Each light may have a shadow map object. Turning off shadows on the
    /// light will delete the map, so do not cache the pointer. If a shadow map
    /// does not exist, this will create a new one.
    RE_ShadowMap	*getShadowMap();
    
protected:

    void		 invalidateShadowMap(bool purge_map = false);
    void		 invalidateFalloffRamp();
    void		 recomputeFalloffRamp();
    void		 createEnvMap();

    int			 myGLConeTextureIndex;	// for lights that project maps
    unsigned		 myContribDiff:1,	// contributes to diffuse
			 myContribSpec:1,	// contributes to specular
			 myUseShadows:1,	// uses shadow maps
			 myAreaSingleSided:1,   // single-sided area light 
			 myAreaSingleReverse:1; // reverse dir for ss area light
    RE_LightFlag	 flag;
    UT_String		 myName; 
    UT_Vector3		 myPos;
    UT_Vector3		 myDir;
    UT_Vector3		 myColor;
    UT_Matrix3		 myOrientation;
    UT_Matrix4		 xform;
    float		 coneAngle, coneDelta, coneRoll;
    float		 myAperture, myFocalLength;
    float		 myOrthoWidth;
    float		 myNearDist;
    float		 myFarDist;
    float		 zoom;
    float		 myAtten[3];
    float		 myAttenRange[2];
    float		 myMaxLightRadius;
    RE_Texture2DMap	*projectMap;		// Projection texture
    RE_TextureCubeMap	*myEnvMap;		// Environment map
    float		 myEnvCone;
    int			 myMaxEnvMapSize;

    UT_String		 myProjectMapName;
    UT_String		 myProjectMapRelativeTo;
    bool		 myProjectMapLimit;
    int			 myProjectMapMaxW;
    int			 myProjectMapMaxH;
    float		 myProjectMapScale;
    RE_TextureCompress   myProjectMapFormat;
    int			 myAnisotropySamples;
    float		 myShadowIntensity;
    float		 myShadowBias;
    float		 myShadowBlur;
    float		 myShadowQuality;
    int			 myShadowMapSize;
    UT_String		 myShadowMask;
    int			 myLightShadowMapSize;
    RE_LightAreaShape	 myAreaLightShape;

    RE_ShadowMap	*myShadowMap;
    RE_Texture		*myFalloffRamp;
    bool		 myFalloffRampDirty;
    RE_Texture		*myAttenMap;
    UT_Ramp		*myAttenRamp;
    float		 myAttenRampStart;
    float		 myAttenRampEnd;
    bool		 myAttenRampDirty;

    UT_Vector3		 myAreaAxes;
    float		 mySurfaceArea;
    UT_Matrix4		 myAreaTransform;
    UT_Vector3FArray	 myAreaPos;
    UT_Vector3FArray	 myAreaDir;

    int64		 myLightVersion;
    int64		 myEmitSerial;

    UT_Array<RE_LightData *> myAttachedLightData;

    // Cached uniform blocks
    RE_UniformBlockHandle myBasicLightingBlock;
    RE_UniformBlockHandle myHighQualityBlock;
    RE_UniformBlockHandle myShadowBlock;
    RE_UniformBlockHandle myAreaBlock;
};

inline void
RE_Light::setColor(const UT_Vector3 &c)
{
    if(myColor != c)
    {
	bumpLightVersion();
	myEmitSerial++;
	myColor = c;
    }
}

inline void
RE_Light::setInfinite(int onoff)
{
    if(flag.infinite != onoff)
    {
	invalidateShadowMap(true);
	flag.infinite = onoff;
	if(onoff)
	{
	    flag.area = false;
	    flag.cone = false;
	    flag.envlight = false;
	    flag.ambient = false;
	    flag.globillum = false;
	}
    }
}

inline void
RE_Light::setAmbient(int onoff)
{
    if(flag.ambient != onoff)
    {
	invalidateShadowMap(true);
	flag.ambient = onoff;
	if(onoff)
	{
	    flag.area = false;
	    flag.infinite = false;
	    flag.cone = false;
	    flag.envlight = false;
	    flag.globillum = false;
	}
    }
}

inline void
RE_Light::setCone(int onoff)
{
    if(flag.cone != onoff)
    {
	invalidateShadowMap(true);
	flag.cone = onoff;
	if(onoff)
	{
	    recomputeFalloffRamp();
	    flag.infinite = false;
	    flag.ambient = false;
	    flag.envlight = false;
	    flag.globillum = false;
	}
	else
	    invalidateFalloffRamp();
    }
}

inline void
RE_Light::setArea(bool onoff)
{
    if(flag.area != onoff)
    {
	invalidateShadowMap(true);
	flag.area = onoff;
	if(onoff)
	{
	    flag.envlight = false;
	    flag.infinite = false;
	    flag.ambient  = false;
	    flag.globillum = false;
	}
    }
}

inline void
RE_Light::setPortal(bool onoff)
{
    if(flag.portal != onoff)
    {
	invalidateShadowMap(true);
	flag.portal = onoff;
    }
}
    

inline void
RE_Light::setEnvironment(bool onoff)
{
    if(onoff != flag.envlight)
    {
	flag.envlight = onoff;
	if(onoff)
	{
	    invalidateShadowMap(true);
	    flag.area = false;
	    flag.cone = false;
	    flag.ambient = false;
	    flag.infinite = false;
	    flag.globillum = false;
	}
	else
	    bumpLightVersion();

    }
}

inline void
RE_Light::setGlobalIllumination(bool onoff)
{
    if(onoff != flag.globillum)
    {
	flag.globillum = onoff;
	if(onoff)
	{
	    invalidateShadowMap(true);
	    flag.area = false;
	    flag.cone = false;
	    flag.ambient = false;
	    flag.infinite = false;
	    flag.envlight = false;
	}
	bumpLightVersion();
    }
}

inline void
RE_Light::setAreaSide(bool single, bool reverse)
{
    if(myAreaSingleSided != single ||
       myAreaSingleReverse != reverse)
    {
	myAreaSingleSided = single;
	myAreaSingleReverse = reverse;
	bumpLightVersion();
    }
}

inline void
RE_Light::setEnvironmentCone(float angle)
{
    angle = SYSmax(0.0f, angle);
    if(angle != myEnvCone)
    {
	bumpLightVersion();
	myEnvCone = angle;
    }
}

inline float
RE_Light::getEnvironmentCone() const
{
    return myEnvCone;
}

inline void
RE_Light::setNearDistance(float znear)
{
    if(znear != myNearDist)
    {
	myNearDist = znear;
	invalidateShadowMap();
    }
}

inline void
RE_Light::setFarDistance(float zfar)
{
    if(myFarDist != zfar)
    {
	myFarDist = zfar; 
	invalidateShadowMap();
    }
}

inline void
RE_Light::setShadowMask(const char *m)
{
    if(myShadowMask != m)
    {
	myShadowMask.harden(m);
	invalidateShadowMap();
    }
}

inline void
RE_Light::setAttenuation(float constant,
			 float linear,
			 float quadratic)
    
{
    if(!SYSisEqual(myAtten[0], constant) ||
       !SYSisEqual(myAtten[1], linear) ||
       !SYSisEqual(myAtten[2], quadratic))
    {
	myAtten[0] = constant;
	myAtten[1] = linear;
	myAtten[2] = quadratic;
	bumpLightVersion();
    }
}

inline void
RE_Light::setAttenuationRange(float near_dist, float far_dist)
{
    if(!SYSisEqual(myAttenRange[0], near_dist) ||
       !SYSisEqual(myAttenRange[1], far_dist))
    {
	myAttenRange[0] = near_dist;
	myAttenRange[1] = far_dist;
	bumpLightVersion();
     }
}

inline void
RE_Light::setAreaAxes(const UT_Vector3F &axes)
{
    if(myAreaAxes!= axes)
    {
	myAreaAxes=axes;
	bumpLightVersion();
    }
}

inline void
RE_Light::setAreaTransform(UT_Matrix4 t)
{
    if(myAreaTransform!=t)
    {
	myAreaTransform=t;
	bumpLightVersion();
    }
}

inline void
RE_Light::setSurfaceArea(float area)
{
    if(!SYSisEqual(mySurfaceArea, area))
    {
	mySurfaceArea = area;
	bumpLightVersion();
    }
}

inline void
RE_Light::setDiffuse(int onoff)
{
    if(myContribDiff != onoff)
    {
	myContribDiff = onoff;
	bumpLightVersion();
    }
}

inline void
RE_Light::setSpecular(int onoff)
{
    if(myContribSpec != onoff)
    {
	myContribSpec = onoff;
	bumpLightVersion();
    }
}

inline void
RE_Light::setZoom(float z)
{
    if(!SYSisEqual(zoom,z))
    {
	zoom = z;
	bumpLightVersion();
    }
}

inline void
RE_Light::setFocalLength(float focal)
{
    if(!SYSisEqual(myFocalLength,focal))
    {
	myFocalLength = focal; 
	bumpLightVersion();
    }
}

inline void
RE_Light::setAperture(float aper)
{
    if(!SYSisEqual(myAperture,aper))
    {
	myAperture = aper;
	bumpLightVersion();
    }
}

inline void
RE_Light::setOrthoWidth(float width)
{
    if(!SYSisEqual(myOrthoWidth,width))
    {
	myOrthoWidth = width;
	bumpLightVersion();
    }
}

#endif
