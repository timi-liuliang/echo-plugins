/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_TextureMap.h ( RE Library, C++)
 *
 * COMMENTS:
 *	Base class for the RE texture map interface to RE_Textures. This class
 *	is basically a handle to a texture map. It allows you to load textures
 *	from images transparently, and cache generated textures.
 */

#ifndef __RE_TextureMap_h__
#define __RE_TextureMap_h__

#include <UT/UT_String.h>

#include "RE_API.h"
#include "RE_Texture.h"
#include <IMG/IMG_FileTypes.h>

class RE_TextureCache;
class re_AutoTextureSource;
class TIL_Raster;
class RE_Render;
class IMG_FileParms;

class RE_API RE_TextureHolder
{
public:
    RE_TextureHolder() : myTexture(NULL) {}
    RE_TextureHolder(RE_Texture *tex)
	: myTexture(tex)
    {
	if(myTexture)
	    myTexture->setInUse(true);
    }
    RE_TextureHolder(const RE_TextureHolder &src)
	: myTexture(src.myTexture)
    {
	if(myTexture)
	    myTexture->setInUse(true);
    }
    ~RE_TextureHolder()
    {
	if(myTexture)
	    myTexture->setInUse(false);
    }
    
    bool	 isValid() const    { return myTexture != NULL; }
    RE_Texture * operator*() const  { return myTexture; }
    RE_Texture * operator->() const { UT_ASSERT(myTexture); return myTexture; }
    
    void	 operator=(RE_Texture *tex)
    {
	if(myTexture)
	    myTexture->setInUse(false);
	myTexture = tex;
	if(myTexture)
	    myTexture->setInUse(true);
    }
    void	 operator=(const RE_TextureHolder &tex)
    {
	*this = tex.myTexture;
    }
    
    void	 clear()
    {
	if(myTexture)
	{
	    myTexture->setInUse(false);
	    myTexture=NULL;
	}
    }
    
    bool	 operator==(const RE_Texture *tex) const
			{ return myTexture == tex; }
    bool	 operator!=(const RE_Texture *tex) const
			{ return myTexture != tex; }
    bool	 operator==(const RE_TextureHolder &tex) const
		        { return myTexture == tex.myTexture; }
    bool	 operator!=(const RE_TextureHolder &tex) const
			{ return myTexture != tex.myTexture; }

    int		 usageCount() const
			{ return myTexture ? myTexture->getUsage() : 0; }
    
private:
    RE_Texture	*myTexture;
};
    

class RE_API RE_TextureMap
{
public:
    virtual ~RE_TextureMap();

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_TextureMap.
    virtual int64 getMemoryUsage(bool inclusive) const;
    
    virtual const char *className() const;
   
    // The texture dimension and type of this map (1D, 2D, Cube, 3D, etc)
    virtual RE_TextureDimension getMapType() = 0;

    // Create an exact replica of this texture map.
    virtual RE_TextureMap *clone() const = 0;

    // Create a new texture map of the given type. 
    static RE_TextureMap *newTextureMap(RE_TextureDimension type);

    // These may return false if the dimension of the texture does not match
    // what this map is, or if a file or node reference cannot be resolved.

    // Find an image from a disk file or node.
    bool	setSource(const char *mapname,
			  const char *relativeTo);

    // Store a generated texture in the cache. You cannot set the translation
    // options with this method, it just uses the map class as a container.
    // The filtering can be set, however. You are responsible for clearing the
    // texture if it is deleted. This allows the texture to be counted as used
    // when computing the #maps and total texture size for a render.
    bool	setSource(const char *mapname,
			  const char *relativeTo,
			  RE_Texture *texture, RE_CacheVersion version,
			  bool (*releaseCB)(RE_Texture *, void *),
			  void *releaseObject);

    // This is an optional callback which will modify the map before being used
    // to create the texture. Any of the image parameters can be changed except
    // for the basic type of the texture (ie 2D, cube). The operation must be
    // done inplace. 'pixeldata' will be NULL when querying for type/size
    // changes and non-NULL when about to generate the texture. In the non-NULL
    // case, none of the image parameters should be altered (they will be the
    // values returned from the previous pixeldata=NULL pass.
    class TexCBPixelData
    {
    public:
	TexCBPixelData()
	    : pixels(NULL),type(RE_GPU_FLOAT32),vector_size(1),w(0),h(0),d(0),
	      option(RE_TEXTURE_FORMAT_EXTRA_NONE), texture(NULL)
	    {}
	RE_GPUType	 type;
	int		 vector_size;
	int		 w,h,d;
	RE_TextureFormatExtra option;
	void		 *pixels;
	RE_Texture	 *texture;
    };
    void	setSourceModifyCallback(const char *opname,
				bool (*modifyCallback)(RE_TextureMap *map,
						       TexCBPixelData &pixdata,
						       void *user_data),
					void *user_data);
    
    // Clears the texture stored in the map, if the first setSource() method
    // was called.
    void	 clearStoredTexture();

    // Clears the texture and removes it from the cache. Useful if you're about
    // to delete the texture.
    void	 deleteTexRef(RE_Texture *tex);

    /// Access to the texture itself. This method may actually build it.
    RE_TextureHolder getTexture(RE_Render *r);

    /// Access to the texture itself, this method will only check the cache.
    RE_TextureHolder getCachedTexture(RE_Render *r);

    /// Return a texture that is not cached in the GL cache. You own it. 
    RE_Texture	*getUncachedTexture(RE_Render *r);

    // Returns true if this map has a tagged texture, setSource(.., RE_Texture)
    bool	 hasTaggedTexture() const { return myTaggedTexture != NULL; }

    // When the version changes, the texture 
    void		setVersion(RE_CacheVersion v)	{ myVersion = v; }
    void		bumpVersion();
    RE_CacheVersion	getVersion() const		{ return myVersion; }

    // Basic information about the map
    const char  *getMapName() const		{ return myMapName; }
    const char  *getRelativePath() const	{ return myRelative; }

    // Set the source to NULL without changing any of the other parameters.
    void	 reset();

    // Set the source to NULL and remove it from the cache.
    void	 clear(const char *clear_name = NULL);

    // returns true if the map/relative names point to a valid source. This
    // allows you to use the methods below to query the data.
    bool	 isSourceValid();

    // Returns the resolution of the image
    void	 getResolution(int &w, int &h, int &d);

    // The data format of the pixel (8b, 32 FP)
    RE_GPUType   getFormatType();

    // The number of components in a pixels (1-4)
    int		 getFormatSize();

    // Returns the original aspect ratio of the image (before scaling or sizing)
    float	 getAspectRatio();
    // For 3D textures, X:Z ratio.
    float	 getAspectRatioXZ();
    

    // Accessors to the filter options for the texture. A single texture map
    // be referenced by several maps with different filter options.
    RE_TextureFilter	   &filter()       { return myFilter; }
    const RE_TextureFilter &filter() const { return myFilter; }

    // Set the texture colorspace. PXL_CS_CUSTOM_GAMMA is unsupported.
    void		setColorSpace(PXL_ColorSpace space);
    PXL_ColorSpace	getColorSpace() const;
    
    // Image to Texture translation. These parameters are used to convert an
    // image (1D, 2D or 3D) to a texture.

    // explicitly scale to the given resolution. If scale is specified as well,
    // it further scales this size down. Setting to 0,0,0 will disable this.
    void	 sizeTo(int w, int h, int d=1);

    // Scale the map up or down by a factor. By default, a uniform scale is
    // used (h/d = -1.0f)
    void	 scaleTo(fpreal w, fpreal h = -1.0f, fpreal d = -1.0f);

    // Cap the size at these maximums, scaling down uniformly to fit. This is
    // applied after the sizeTo() and scaleTo() methods. Setting to 0,0,0 will
    // disable the max size.
    void	 maxSize(int w, int h, int d);

    // Cap the resulting texture so that it doesn't use more than this limit.
    // A limit of 0 will disable the texture memory limit.
    void	 maxMemSize(int mem_limit_mb);

    // If enabled, this will round the texture to the next power of two if a
    // dimension is not a power of two.
    void	 makePow2(bool pow2);
    
    // By default, the map uses the same format as the image.
    void	 setCompression(RE_TextureCompress comp);

    // By default, the texture is not mipmapped.
    void	 setMipMap(bool mip);

    // No options by default. Can currently set a 1chan texture to be
    // interpreted as A, and 2chan texture RG instead of Lum-A.
    void	 setTextureFormatOption( RE_TextureFormatExtra opt )
			    { myFormatOption = opt; }

    // Texture Component to sample from when using a single channel from
    // packed maps. -1 indicates that no mapping is done.
    void	 setTextureComponent(int texcomp)
		    { myTextureComponent = SYSclamp(texcomp, -1, 3); }

    // Sample texture map as { R, R, R, A }. If the texture component is given
    // above, use that component (eg. texcomp=2 would sample B,B,B,A). 
    void	 fillRGBFromSingle(bool fill_rgb_mono)
		    { myTextureRGBMono = fill_rgb_mono; }

    // If true, set A to 1. Otherwise, use A from the map.
    void	 setOpaqueAlpha(bool opaque_alpha)
		    { myOpaqueAlpha = opaque_alpha; }
    // returns the opaque alpha setting, not whether the texture itself
    // actually has fully opaque alpha.
    bool	 getOpaqueAlpha() const { return myOpaqueAlpha; }

    // When loading the map, look for details about the alpha matte. Currently
    // only implemented for 2D maps.
    void	 checkAlphaDetails(bool check)  { myAlphaDetails = check; }
    IMG_FileAlphaInfo getAlphaDetails()	const	{ return myAlphaInfo; }


    // Called when the texture this map references was deleted.
    static void	 textureReferenceDeleted(void *, RE_Texture *t);

    // Texture cache interface
    static void	 setTextureCacheSize(int64 size_in_mb);
    static int64 getTextureCacheSize();
    
    static int64 getTextureCacheUsage();
    static int64 get2DTextureUsage();
    static int64 get3DTextureUsage();
    static int64 getMipMapUsage();
    static int64 get3DTextureFP32Size();
    static int64 get3DTextureFP16Size();
    static int64 get2DTextureFP32Size();
    static int64 get2DTextureFP16Size();
    static int64 getOrig3DTextureFP32Size();
    static int64 getOrig3DTextureFP16Size();
    static int64 getOrig2DTextureFP32Size();
    static int64 getOrig2DTextureFP16Size();
    
    static void	 pruneTextureCache();
    static void  clearTextureCache();
    static void  debugDumpCache();

    // These methods can be used to query texture map usage over a long period
    // of time (such as a single redraw). The query methods must be called
    // *before* recording is disabled.
    static void	 recordTextureMapUsage(bool enable);
    static bool	 recordingTextureUsage();
    static void	 textureUsed(RE_Texture *tex);
    static void  textureDeleted(RE_Texture *tex);
    
    static int	 getNumUsedTextureMaps();
    static int64 getUsedTextureMemory();
    
protected:
    // You can't create an RE_TextureMap directly; you have to use one of the
    // derived classes to define a type.
    RE_TextureMap();

    virtual void	buildSourceName(UT_String &cachedname,
					const char *map, const char *rel);
    virtual void	buildTextureName(UT_String &cachedname,
					 const char *map, const char *rel);
    re_AutoTextureSource getTextureSource(const char *map, const char *relto,
					 void **tex_data);
    
    // Adjusts a resolution based on the maxSize, sizeTo and scaleTo settings.
    // 1D and 2D maps can set the extra dimensions to NULL.
    void		getDesiredResolution(RE_GPUType type, int vsize,
					     int *w, int *h, int *d);

    // Override to actually create the texture from the data returned in
    // mapptr from getNodeSource() and getFileSource().
    virtual bool	buildTexture(RE_Render *r, RE_Texture *tex,
				     void *data) = 0;

    // Override if you're interested in when a texture is retrieved from the
    // cache instead of built.
    virtual void	cachedTextureFound(RE_Texture *tex) {}

    // Note that mapptr may be NULL if the actual data of the image is not
    // required yet.
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

    // override to return false if the derived class will call the callback to
    // modify the pixel data.
    void *		callModifyCallback(RE_Texture *tex, void *data);

    // Can be called after the source is set.
    bool		isNodeSource();

    RE_TextureCompress	 getCompression() const { return myCompression; }
    bool		 getMipMap() const { return myMipMap; }

    // Returns a empty texture object with the desired attributes.
    RE_Texture		*getTextureStub();

    void		 copy(const RE_TextureMap *src);
    
    RE_TextureFormatExtra getTextureFormatOption() const
			 { return myFormatOption; }

    void		 setupGammaCorrect(IMG_FileParms &parms,
					   const char *filename,
					   RE_GPUType datatype);

    RE_TextureHolder	 fetchTexture(RE_Render *r, bool build_if_missing);
   
    // Translation options
    RE_TextureCompress	 myCompression;
    fpreal		 myScale[3];
    int			 myRes[3];
    int			 myMaxRes[3];
    int			 myMaxMemMB;
    int			 myMipMap;
    bool		 myMakePow2;
    UT_String		 myMapName;
    bool		 myAlphaDetails;
    IMG_FileAlphaInfo	 myAlphaInfo;
    
private:
    void		 init(RE_Texture *t);
    
    static bool		 cachedMapReleased(void *tex, void *map);
    bool		 clearTexture(RE_Texture *tex);
    
    UT_String		 myRelative;
    UT_String		 myOpName;
    RE_TextureFilter	 myFilter;
    bool		 myMapOwned;
    RE_CacheVersion	 myVersion;
    PXL_ColorSpace	 myColorSpace;
    RE_TextureFormatExtra myFormatOption;
    int			 myTextureComponent;
    bool		 myTextureRGBMono;
    bool		 myOpaqueAlpha;

    UT_String		 myModifyOp;
    bool	       (*myModifyCallback)(RE_TextureMap*, TexCBPixelData&,
					   void*);
    void		*myModifyCallbackData;
    
    // Only for the setSource(src, rel, tex, version) call.
    RE_TextureHolder     myTaggedTexture;
    RE_TextureCache	*myCache;
    RE_TextureHolder	 myCachedTexture;

    bool		(*myReleaseCB)(RE_Texture *, void *);
    void		*myReleaseObject;
};


inline const char *
RE_TextureMap::className() const
{
    return "RE_TextureMap";
}

#endif
