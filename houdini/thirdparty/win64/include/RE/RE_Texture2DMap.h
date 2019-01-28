/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Render Library (C++)
 *
 * COMMENTS:
 *	Handle to a 2D texture map stored in the texture cache. This class is
 *	primarily responsible for fetching images from various 2D sources and
 *	creating the texture for them.
 */

#ifndef __RE_Texture2DMap_h__
#define __RE_Texture2DMap_h__

#include "RE_API.h"
#include "RE_TextureMap.h"

class RE_API RE_Texture2DMap : public RE_TextureMap
{ 
public:
	     RE_Texture2DMap();
    virtual ~RE_Texture2DMap();

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_Texture2DMap.
    virtual int64 getMemoryUsage(bool inclusive) const;

    virtual RE_TextureDimension getMapType() { return RE_TEXTURE_2D; }
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

    virtual void	cachedTextureFound(RE_Texture *tex);
    
    fpreal	myOpTime;
    size_t	myOpVersion;
    int		myOpID;
    TIL_Raster *myCopRaster;
};

#endif
