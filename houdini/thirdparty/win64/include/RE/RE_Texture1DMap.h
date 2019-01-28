/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Render Library (C++)
 *
 * COMMENTS:
 *	Handle to a 1D texture map stored in the texture cache. This class is
 *	primarily responsible for fetching images from various 1D sources and
 *	creating the texture for them (ramps, channel formats)
 */

#ifndef __RE_Texture1DMap_h__
#define __RE_Texture1DMap_h__

#include "RE_API.h"
#include "RE_TextureMap.h"

#include <CL/CL_Clip.h>

class RE_API RE_Texture1DMap : public RE_TextureMap
{ 
public:
	     RE_Texture1DMap();
    virtual ~RE_Texture1DMap();

    /// Returns the amount of main memory owned by this map..
    /// (graphics memory is owned by the returned RE_Texture)
    virtual int64 getMemoryUsage(bool inclusive) const;

    virtual RE_TextureDimension getMapType() { return RE_TEXTURE_1D; }
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

    int			myOpId;
    int			myOpVersion;
    UT_Vector4Array	myData;
    int			myVectorSize;
};

#endif
