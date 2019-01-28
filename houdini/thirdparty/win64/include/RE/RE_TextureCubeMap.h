/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Render Library (C++)
 *
 * COMMENTS:
 *	Handle to a cube texture map stored in the texture cache. This class is
 *	primarily responsible for fetching images from various 2D sources and
 *	creating the texture for them.
 */

#ifndef __RE_TextureCubeMap_h__
#define __RE_TextureCubeMap_h__

#include "RE_API.h"
#include "RE_TextureMap.h"

class IMG_File;

class RE_API RE_TextureCubeMap : public RE_TextureMap
{ 
public:
	     RE_TextureCubeMap();
    virtual ~RE_TextureCubeMap();

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_TextureCubeMap.
    virtual int64 getMemoryUsage(bool inclusive) const;

    virtual RE_TextureDimension getMapType() { return RE_TEXTURE_CUBE; }
    virtual RE_TextureMap *clone() const;
        
    // Public interface is in RE_TextureMap
private:
    virtual bool	buildTexture(RE_Render *r, RE_Texture *tex, void *data);
    virtual bool	getNodeSource(const char *map, const char *rel,
				      void **mapptr,
				      RE_TextureDimension &textype,
				      RE_GPUType &datatype,
				      int &vectorsize,
				      int &w, int &h, int &d);
    virtual bool	getFileSource(const char *map,
				      void **mapptr,
				      RE_TextureDimension &textype,
				      RE_GPUType &datatype,
				      int &vectorsize,
				      int &w, int &h, int &d);
    virtual void	buildTextureName(UT_String &,
					 const char *map, const char *rel);

    bool		loadCubeMap(IMG_File *fp, int w, int h,
				    RE_GPUType dt, int vs, void **mapptr);
    bool		loadSphereMap(IMG_File *fp,int w, int h, int sw, int sh,
				      RE_GPUType dt, int vs, void **mapptr);

    float		myOpTime;
};

#endif
