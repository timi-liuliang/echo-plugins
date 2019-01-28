/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_ShadowMap.h ( RE Library, C++)
 *
 * COMMENTS:
 *	Contains the texture map and the code for generating a shadow map from
 *	a light.
 */
#ifndef RE_ShadowMap_h
#define RE_ShadowMap_h

class RE_Render;
class RE_Shader;
class RE_OGLTexture;
class RE_OGLFramebuffer;
class RE_Texture2DMap;
class RE_TextureCubeMap;
class RE_Uniform;

#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Types.h>
#include <UT/UT_Rect.h>

#include "RE_API.h"
#include "RE_Texture.h"
#include "RE_Light.h"

#define RE_SHADOW_MAX_MAPS					\
	(RE_SHADOW_MAX_AREA_MAPS > RE_SHADOW_NUM_CUBE_MAPS ?	\
	 RE_SHADOW_MAX_AREA_MAPS : RE_SHADOW_NUM_CUBE_MAPS)

class RE_API RE_ShadowMap
{
public:
		RE_ShadowMap(const RE_Light *light, int mapsize);
	       ~RE_ShadowMap();

    // If true, this shadow map contains multiple maps for doing blurry
    // shadows.
    void	 setMultiMap(bool m);
    bool	 isMultiMap() const;


    // If true, this shadow map renders to an 32b R32F texture, without
    // the non-linear clamping depth to [0,1]. OpenGL3 hardware required.
    void	 useLinearDepthMap(bool ld);
    bool	 usesLinearDepthMap() const;
	
    
    // Multiplies the current GL matrix by the transformation that maps a
    // world space point to a light space point. If 'mat' is supplied, the
    // transform is assigned to it rather than affecting the current GL matrix.
    void	 getShadowMapTransform(RE_Render *r, bool bias,
				       UT_Matrix4F *mat = NULL,
				       bool cube=false,
				       RE_TextureCubeFace face
				           = RE_TEXTURE_CUBE_POS_X,
				       int area_index = 0,
				       int xform_mask = 0x3);

    // Sets up GL to render the light's shadow map.  The proper transform
    // will be set for you.  If this returns false, shadow maps are not
    // supported and no further shadow map calls should be made (including
    // finishShadowMapRender).  If this returns true, the shadow map is
    // ready to be rendered to and finishShadowMapRender must be called
    // after rendering to it. When rendering a point light, the scene must be
    // rendered 6x for each face of the cube map, so 'face' is used to indicate
    // which cube face is currently being rendered. 'samples' allows for
    // multisampled shadow maps, providing better edge antialiaing. 
    bool	 prepShadowMapRender(RE_Render *r,
			     RE_TextureCubeFace face =RE_TEXTURE_CUBE_POS_X,
				     int area_index = 0);

    // When you are done rendering from the light's view, call this
    // to clean up your GL state and return you to the main viewport.
    void	 finishShadowMapRender(RE_Render *r,
			     RE_TextureCubeFace face =RE_TEXTURE_CUBE_POS_X,
				       int area_index = 0);


    // Enables or disables the use of shadow maps for the specified light.
    // If 'compare' is on, then the texture must be accessed by the shadow
    // versions of samplers (otherwise, off, uses normal texture2D() sampling)
    // area_index specifies the map to use for blurred area light shadows.
    void	 useShadowMap(RE_Render *r, RE_Shader *sh, bool ison);

    // Returns true if 6 separate shadow maps are used instead of a cube map,
    // for hardware that does not support cube shadow maps.
    bool	 useSeparatePointShadowMaps(RE_Render *r) const;

    // Sets up a distribution of point cube maps to approximate the area shape.
    void	 setAreaLightShape(RE_LightAreaShape sh) { myAreaShape=sh; }
    RE_LightAreaShape getAreaLightShape() const { return myAreaShape; }

    // Returns the number of area maps for a given light shape.
    int		 getNumAreaMaps() const;

    // Single point lights have one shadow map, while area lights have 4.
    RE_Texture	*getShadowMap(int area_index = 0);

    // Allows for versioned updates to shadow maps.
    void	 setVersion(int64 version) { myVersion = version; }
    int64	 getVersion() const	   { return myVersion; }

    void	 setFrameTime(fpreal t)	    { myTime = t; }
    fpreal	 getFrameTime() const	    { return myTime; }

    // Only valid after the shadowmap has been created.
    void	 getZClipRange(fpreal &n, fpreal &f) { n = myZNear; f=myZFar; }

    // generic method which can be used to rotate a view toward a specific cube
    // face.
    static UT_Matrix4D computeCubeTransform(RE_TextureCubeFace face,
					    UT_Vector4 position,
					    UT_Matrix4 transform);
private:
    bool	 prepShadowCubeRender(RE_Render *r, int area_index);
    void	 finishShadowCubeRender(RE_Render *r, int area_index);

    void	 clearMaps(bool keep_first_map);
    
    RE_OGLFramebuffer *getShadowFrameBuffer(RE_Render *r,
					    RE_OGLTexture *&tex,
					    bool cube,
					    RE_TextureCubeFace face,
					    int area_index);

    static bool		shTexDeleted(RE_Texture *tex, void *shmapobj);

    UT_Vector4		getAreaLinePos(int index);
    UT_Vector4		getAreaGridPos(int index);
    UT_Vector4		getAreaDiskPos(int index);
    UT_Vector4		getAreaCubePos(int index);
    UT_Vector4		getAreaSpherePos(int index);

    const RE_Light     *myLight;
    int			myShadowMapSize;
    RE_OGLFramebuffer  *myShadowMapFBO;
    RE_OGLFramebuffer  *myShadowCubeFBO;
    RE_Texture2DMap    *myShadowHandle[RE_SHADOW_MAX_MAPS];
    RE_OGLTexture      *myShadowMap[RE_SHADOW_MAX_MAPS];
    RE_TextureCubeMap  *myShadowCubeHandle[RE_SHADOW_MAX_AREA_MAPS];
    RE_OGLTexture      *myShadowCube[RE_SHADOW_MAX_AREA_MAPS];
    RE_OGLTexture      *myShadowCubeZ;
    RE_Uniform	       *myCubeView;
    int64		myVersion;
    fpreal		myTime;
    bool		myIsMultiMap;
    bool		myIsSeparateMaps;
    bool		myLinearDepthMap;
    UT_DimRect		myPrevViewport;
    RE_LightAreaShape	myAreaShape;
    fpreal		myZNear, myZFar;
};

#endif


