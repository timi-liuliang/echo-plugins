/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_IconTextureCache.h (RE Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RE_IconTextureCache__
#define __RE_IconTextureCache__

#include "RE_API.h"

#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_Rect.h>
#include "RE_Texture.h"

class RE_Render;
class PXL_Raster;

class re_DeferTexDraw
{
public:
    re_DeferTexDraw()
    { }
    re_DeferTexDraw(RE_Texture *tex, const UT_DimRect &clip,
		    short x, short y, short w, short h,
		    float alpha, float z)
	: myTex(tex),
	  myClip(clip),
	  myX(x),
	  myY(y),
	  myW(w),
	  myH(h),
	  myAlpha(alpha),
	  myZ(z)
    { }

    RE_Texture		*myTex;
    UT_DimRect		 myClip;
    short		 myX;
    short		 myY;
    short		 myW;
    short		 myH;
    float		 myAlpha;
    float		 myZ;
};
SYS_DECLARE_IS_POD(re_DeferTexDraw);

class RE_API RE_IconTextureCache
{
public:
    RE_IconTextureCache();
    ~RE_IconTextureCache();

    /// Add a regular texture from a raster and return its unique ID.
    RE_Texture *addTexture(RE_Render *r, PXL_Raster *texture);

    /// Add a texture from a raster, and automatically generate the mip-map
    /// levels for it.
    RE_Texture *addMipMapTexture(RE_Render *r, PXL_Raster *texture);

    /// Add a mip-mapped texture, with many levels defined by the rasters passed
    /// as levels, and return its unique ID. The levels must all be powers of
    /// two in size, and must follow the progression from 2^Nx2^N,
    /// 2^(N-1)x2^(N-1), ..., 1x1.
    RE_Texture *addMipMapTexture(RE_Render *r,
				 const UT_ValArray<PXL_Raster *> &levels);

    /// Remove a texture from the cache.
    void removeTexture(RE_Texture *tex);

    /// Free all textures.
    void clearTextureCache();

    /// Returns a version # for the cache; every time it is cleared, this
    /// number is bumped;
    int  getTextureVersion() const { return myVersion; }

    /// Render a given texture at (x, y) with width and height w, h.
    void renderTexture(RE_Render *r, int x, int y, int w, int h,
		       RE_Texture *tex, float alpha, float z);

    /// These methods record the textures to be drawn, and draws them all at
    /// once when endTextureDefer() is called. If you need all cached textures
    /// to be drawn immediately without doing an end, call flushTextureDefer().
    void beginTextureDefer(RE_Render *r);
    void endTextureDefer(RE_Render *r);
    void flushTextureDefer(RE_Render *r);

    int64	getCacheSize() const { return myCacheSize; }
    
private:
    RE_Texture *createTextureObject(RE_Render *r, PXL_Raster *image,
				    RE_TextureCompress storage,
				    bool mipmap, bool autogen,
				    RE_TexFiltType min_filter, 
				    RE_TexClampType clamp);
    RE_Texture *addTextureObject(RE_Texture *texture);

    /// An array of OpenGL texture IDs.
    UT_ValArray<RE_Texture *>		 myTextures;

    /// Cache Version #; bumped at every cache clear.
    int					 myVersion;
    int64				 myCacheSize;

    /// Data for the texture defer system, which batches icon rendering together
    /// for improved GL performance.
    UT_Array<re_DeferTexDraw>		 myDeferTexDraw;
    UT_IntArray				 myTextureDeferStart;
};

/// Get the one and only texture cache.
RE_API extern RE_IconTextureCache *REgetIconTextureCache();

#endif

