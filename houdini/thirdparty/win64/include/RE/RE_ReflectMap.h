/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_ReflectMap.h ( RE Library, C++)
 *
 * COMMENTS:
 *	Contains a cube texture map and the code for generating a reflection
 *	map from an object.
 */
#ifndef RE_ReflectMap_h
#define RE_ReflectMap_h

class RE_Render;
class RE_OGLTexture;
class RE_OGLFramebuffer;
class RE_TextureCubeMap;

#include <UT/UT_String.h>
#include <UT/UT_Vector3.h>

#include "RE_Types.h"
#include "RE_TextureTypes.h"

class RE_API RE_ReflectMap
{
public:
		 RE_ReflectMap(const char *name);
		~RE_ReflectMap();

    void	setResolution(int size);
    void	setHDR(bool enable);
    void	setOrigin(UT_Vector3F org);
    void	setMipmap(bool enable);

    UT_Vector3F getOrigin() const { return myOrigin; }

    bool	isDirty() const { return myChanged; }

    bool	prepRender(RE_Render *r,
			   RE_TextureCubeFace face =RE_TEXTURE_CUBE_POS_X);
    void	doneRender(RE_Render *r);

    RE_OGLTexture *getReflectMap() const  { return myColorTex; }

    void	setVersion(int64 version) { myVersion = version; }
    int64	getVersion() const	  { return myVersion; }
     
    void	setFrameTime(fpreal t)	    { myTime = t; }
    fpreal	getFrameTime() const	    { return myTime; }
    
    bool	createFBO(RE_Render *r);
	
private:
    void	resetFBO();
    static bool	shTexDeleted(RE_OGLTexture *tex, void *rmapobj);
    
    UT_String	myName;
    int		myRes;
    bool	myHDR;
    UT_Vector3F	myOrigin;

    RE_OGLFramebuffer *myFBO;
    RE_OGLTexture    *myColorTex;

    RE_TextureCubeMap *myCubeMap;

    bool	myRenderFlag;
    bool	myChanged;
    bool	myMipmap;
    int64	myVersion;
    fpreal	myTime;
};
#endif
