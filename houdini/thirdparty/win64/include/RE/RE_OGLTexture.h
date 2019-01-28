/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_OGLTexture.h ( RE Library, C++)
 *
 * COMMENTS:
 *	Base class for various OpenGL texture types: 1D, 2D, 3D, Cube,
 *	2D rectangles, 1D arrays and 2D arrays.
 *	
 */
#ifndef RE_OGLTexture_h
#define RE_OGLTexture_h

class RE_Render;
class RE_TextureMap;

#include <UT/UT_ValArray.h>
#include <UT/UT_Rect.h>

#include "RE_CachedObject.h"
#include "RE_Types.h"
#include "RE_TextureTypes.h"
#include "RE_TextureFilter.h"
#include <IMG/IMG_FileTypes.h>

#include <iosfwd>

class RE_API RE_OGLTexture : public RE_CachedObject
{
public:
    virtual ~RE_OGLTexture();
    
    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_OGLTexture.
    virtual int64 getMemoryUsage(bool inclusive) const;

    virtual const char *className() const;
    
    // Texture type (dimensions)
    RE_TextureDimension getTextureType() const;

    // Returns true if this texture type is supported.
    virtual bool	hasTextureSupport(RE_Render *r);

    // True if this texture type supports mipmapping (all but tex rectangles)
    virtual bool	hasMipMapSupport(RE_Render *);

    // Returns the maximum size of a texture edge of the given texture type
    virtual int		getMaxTextureSize(RE_Render *r) = 0;

    // Returns the maximum index that can be used for arrays and cube maps
    virtual int		getMaxTextureIndex(RE_Render *r);

    // True if GL can compress textures itself.
    bool		hasAutoCompression(RE_Render *r);

    // true if this has a valid texture created for it.
    virtual bool	isValid() const;

    // true if the texture res, format, mip levels can be changed on the fly
    bool		isMutable() const { return myMutableFlag; }

    // Texture identifier - opaque handle.
    RE_TextureID	getID() const;

    // Releases the texture on the GPU; this object becomes invalid.
    virtual void	free();

    // Returns a new texture with no attributes of the given type.
    static RE_OGLTexture *newTexture(RE_TextureDimension t);

    // Returns a new, blank texture with the same attributes as this texture.
    RE_OGLTexture	*copyAttributes() const;

    // -----------------------------------------------------------------
    // Basic Texture data (modifying will invalidate texture,
    //                     requiring a setTexture() call)

    // The texture type can be color, stencil, or depth. Stencil and depth
    // textures are more restrictive with the types that can be sent to them.
    // They must be single channel textures; depth can be FLOAT16,24 or 32,
    // while stencil must be UINT1,2,4,8 or 16. The default is Color.
    // This must be set before calling setFormat().
    void		setDataType(RE_TextureDataType type);
    RE_TextureDataType  getDataType() const;

    // The data type is the CPU-side data format, vectorsize can be 1-4.
    // To use leaner GPU-side data formats, set a texture compression.
    void		setFormat(RE_GPUType data, int vectorsize,
				  RE_TextureFormatExtra format_extra =
						RE_TEXTURE_FORMAT_EXTRA_NONE);
    RE_GPUType		getFormatType() const;
    int			getFormatSize() const;
    RE_TextureFormatExtra getFormatExtra() const;

    // returns the GL internal format of the texture (ie, RGBA8).
    int			getInternalFormat() const { return myGLInternal; }

    // You can pass the data in a different format than the texture type.
    // This must be called after setFormat().
    void		setClientFormat(RE_GPUType data, int vectorsize);

    // Compress the texture on the GPU, default is to use the same format as
    // the CPU format (RE_COMPRESS_NONE). If auto_compress is false, it is
    // assumed that the data passed to this texture object is already
    // compressed.
    void		setCompression(RE_TextureCompress comp,
				       bool auto_compress);
    RE_TextureCompress	getCompression() const;
    bool		getAutoCompression() const;

    // Texture Resolution
    // Height/depth may not be required, depending on texture dimension.
    void		setResolution(int w, int h = 1, int depth = 1,
				      bool force_pow2 = false);
    int			getWidth() const;
    int			getHeight() const;
    int			getDepth() const;
    
    // GL may differ from getWidth/Height/Depth if GPU requires pow2 textures
    // These sizes will never be smaller than the above sizes.
    int			getGLWidth() const;
    int			getGLHeight() const;
    int			getGLDepth() const;

    // If the texture size differs from the GL size, this will return a value
    // less than 1 to represent the coord where the image ends in the texture 
    float		getTexS() const;
    float		getTexT() const;
    float		getTexR() const;

    // For multisample textures (2D)
    virtual void	setSamples(int samples);
    int			getSamples() const;

    // Enables mipmapping and auto-generation of mipmaps. Not all texture data
    // formats can be mipmapped (specifically floating point). This also sets
    // the minify filter to RE_FILT_LINEAR_MIPMAP_LINEAR if not set to a MIPMAP
    // filter and it is enabled, or back to RE_FILT_LINEAR if disabled.
    void		setMipMap(bool m, bool autogen = true);
    bool		getMipMap() const;
    bool		getMipMapAutoGenerate() const;

    // Returns true if the texture actually is mipmapped, not just requested
    // to be (some formats don't mipmap). 
    bool		isMipMapped() const;

    // This can be called before the texture is created to determine if it's
    // possible to use. Returns false if it's not possible to create. If the
    // texture is already created, it returns the actual OpenGL texture values.
    // 'format' is an internal OpenGL format specification (like GL_RGBA8). 
    bool		queryTextureStats(RE_Render *r,
					  int *width, int *height, int *depth,
					  int *format);

    void		setLabel(RE_Render *r, const char *label);

    
    // ---------------------------------------------------------------------
    // Texture transfer methods (CPU/GPU).
    
    // set/getTexture() writes or reads the entire texture.
    // It is assumed that data points to a chunk of memory large enough for the
    // texture. A set method must be called afterwards to reinitialize the
    // texture if the above parameters are changed.
    // If non-pow2 textures are not supported and you set the resolution to a
    // non-pow2 size, you still pass in non-pow2 data and it will be written
    // properly (do not pad to pow2). If you do not wish this to be the case,
    // set a pow2 resolution insteead. When rendering a non-pow2 emulated
    // texture, use the getTexS/T/R() methods to determine the edge of the data.

    // 'level' is the texture mipmap level.Must be -1 if mipmapping is disabled.
    // 'index' varies by texture type; face for cube maps and index for arrays.
    // 'proxy' does a texture proxy call, merely seeing if the texture can be
    //         created without generating any errors.
    virtual void	setTexture(RE_Render *r, const void *data,
				   int level=-1, int index=-1,
				   bool proxy=false) = 0;
    
    // How large a block is required to initialize or fetch this texture.
    virtual int64	getSizeBytes() const;

    // The size of one scanline in the image. The second version using sublen
    // instead of GLwidth
    int64		getScanBytes() const;
    int64		getScanBytes(int sublen) const;

    // Number of bits in a full pixel (ie, fp16 RGB has 48b). 
    int64		getBitsPerPixel() const;
    
    // Actual size of texture in GPU mem.
    int64		getTextureSize() const;

    // Write to or read from a portion of the texture. Not all dimensions may
    // be used. See derived classes for the meaning of 'z'. These can only be
    // used after setTexture(), though it can be called with data=NULL to set
    // up the texture for setSubTexture(). 
    virtual void	setSubTexture(RE_Render *r,
				      const void *data, int level,
				      int xoffset,   int xsize,
				      int yoffset=0, int ysize=1,
				      int zoffset=0, int zsize=1) = 0;

    // Reads the entire texture at 'level' back into 'data'. 'data' must point
    // to a block of at least 'getSizeBytes()' in size.
    // Note that a non-pow2 emulated texture will return the entire GL pow2
    // size, since there is no way to read a portion of a texture.
    virtual void	getTexture(RE_Render *r, void *data,
				   int level=0, int index=-1);

    // copies 'area' block of pixels from the read buffer to this texture, 
    // in screen coords (if NULL, the entire area) at offset  (x,y,z) and
    //  miplevel 'level'. The pixels are converted to this texture's data format
    virtual bool	copyFromBuffer(RE_Render *r, int x, int y, int z = 0, 
				       const UT_DimRect *bufferarea = NULL,
				       int level = 0) = 0;

    // Build mip-maps from mip level 0 of the texture. Useful for
    // filtering render-to-texture targets. Returns true if mipmaps were
    // generated.
    bool		generateMipMaps(RE_Render *r);

    // ---------------------------------------------------------------------
    // Texture Parameters (changing will modify, does not require setTexture)
    
    // Filter to use when displaying the texture at less than 100% zoom
    void		setMinFilter( RE_Render *r, RE_TexFiltType t );
    RE_TexFiltType	getMinFilter()	const;

    // Filter to use when displaying the texture at greater than 100% zoom
    void		setMagFilter( RE_Render *r, RE_TexFiltType t );
    RE_TexFiltType	getMagFilter()	const;

    void		setMaxAnisotropy(RE_Render *r, int atf);
    int			getMaxAnisotropy() const;

    // Fixed bias value that is added to the level-of-detail parameter for
    // the texture before texture sampling (when mipmapping).
    void		setLodBias(RE_Render *r, float bias);
    float		getLodBias() const;

    // Texture wrapping - clamp or repeat. Not all dimension may be used.
    void		setTextureWrap(RE_Render *r,
				       RE_TexClampType clamp_s,
				       RE_TexClampType clamp_t = RE_CLAMP_EDGE,
				       RE_TexClampType clamp_r = RE_CLAMP_EDGE);
    bool		getWrapS() const;
    bool		getWrapT() const;
    bool		getWrapR() const;

    void		setBorder(RE_Render *r,
				  RE_TextureBorder border,
				  const UT_Vector4F *color_alpha = NULL);
    UT_Vector4F		getBorderColor() const;

    // Sets all of the above in one go.
    void		setTextureFilter(RE_Render *r,
					 const RE_TextureFilter &filter);

    // When on, depth comparison is used. When used in a shader, a shadow
    // sampler must be used. If disabled, the raw depth value is returned and
    // a normal texture sampler must be used. 'compare' can be set to a
    // negative, positive or zero value, indicating less/equal, greater/equal or
    // equal compare mode.
    void		setDepthCompareMode(RE_Render *r, bool enabled,
					    RE_TextureCompare compare
					      = RE_TEXTURE_COMPARE_LESS_EQUAL);

    // If RE_EXT_TEXTURE_SWIZZLE is supported, texture components can be
    // shuffled about or set to a constant 0 or 1 value.
    bool		setTextureSwizzle(RE_Render *r,
					  RE_TextureSwizzle red,
					  RE_TextureSwizzle green,
					  RE_TextureSwizzle blue,
					  RE_TextureSwizzle alpha);

    void		setAlphaInfo(IMG_FileAlphaInfo a) { myAlphaInfo = a; }
    IMG_FileAlphaInfo	getAlphaInfo() const		  { return myAlphaInfo;}

    // --------------------------------------------------------------------
    // Bindless Textures  (requires RE_EXT_TEXTURE_BINDLESS)

    // Creates a bindless handle for the texture. Once this is done, the texture
    // is immutable - its texel contents can change, but nothing else.
    // Will return false if bindless texture is unsupported.
    bool		createTextureHandle(RE_Render *r);

    // returns the bindless texture handle. createTextureHandle() must be called
    // first. If bindless is unsupported or create was not called, returns 0.
    uint64		getTextureHandle() const { return myBindlessHandle; }

    // Before a bindless texture can be used, it must be made resident.
    bool		makeResidentTexture(RE_Render *r);

    // When a bindless texture is no longer needed, it should be removed from
    // resident status to free up resources.
    bool		removeResidentTexture(RE_Render *r);
    
    // --------------------------------------------------------------------
    
    static int		getGLTextureType(RE_TextureDimension t);
    static RE_TextureDimension getRETextureType(int gl_type);
    static RE_OGLTexture *findTextureWithID(RE_TextureID id);

    // Mostly for debug, this writes the texture out to a file (any supported
    // image format, determined by the extension). 3D textures are best written
    // as deep raster formats, like .pic.
    bool		writeToFile(RE_Render *r, const char *name,
				    int level = 0);

    // Expands a 2D multisample texture into a 2D texture with dimensions
    // increased by the sampling factor: 2x (2,1), 4x (2,2), 6x (3,2), 8x (4,2)
    RE_OGLTexture *	expandMultisampleTexture(RE_Render *r);
    
    static void		adjustPow2(int &w, int &h, int &d);

    
    // References on this texture by other objects
    void		addReference(void (*cb)(void *, RE_OGLTexture *),
				     void *ref_obj);
    void		removeReference(void *ref_obj);

    // print a summary of this texture. If os is NULL, use std::cerr.
    void		print(std::ostream *os = NULL);

    // Dump all textures currently in use by Houdini. If min_tex_size is
    // specified, only those textures >= the size will be printed. The summary
    // line will always include stats from all textures.
    static void		dumpTextureTable(std::ostream &os,
					 int64 min_tex_size = -1);
protected:
    
			RE_OGLTexture(RE_TextureDimension d);
	     
    void		deleteObject();
    void		setupTextureParameters(RE_Render *r, bool force);
    void		validate();

    // These return various GL types (ie, GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D,
    // GL_TEXTURE_BINDING_2D)
    virtual int		getGLType() const = 0;
    virtual int		getGLProxy() const = 0;
    virtual int		getGLTypeBinding() const = 0;
    virtual int		getGLCreateType() const { return getGLType(); }

    // These methods must be called from within a locked context
    virtual bool	buildMipMaps(RE_Render *r, const void *data) = 0;
    
    bool		createTextureID();

    RE_GPUType		getClientType() const;
    
    // returns a malloc'ed image with the compressed data, which must be freed.
    // If no compression is necessary, this will return NULL.
    void *		compressTexData(const void *src, int &w, int &h);
    void		determineCompressionInternal();
    void		determineTextureSize(RE_Render *r);
    
    // OpenGL data
    RE_TextureID	myID;
    int			myGLType;
    int			myGLFormat;
    int			myGLInternal;
    int			myGLWidth;
    int			myGLHeight;
    int			myGLDepth;
    bool		myCanMipMap;
    
    void		updateTextureWrap(RE_Render *r, bool force);
    void		updateFilters(RE_Render *r, bool force);

private:
    void		buildFormatTable();
    
    // RE data
    RE_TextureDataType	myTextureDataType;
    RE_GPUType		myDataType;
    RE_GPUType		myClientType;
    RE_TextureFormatExtra myFormatExtra;
    RE_TextureDimension myTexDimension;
    int			myVectorSize;
    int			myWidth;
    int			myHeight;
    int			myDepth;
    RE_TextureCompress	myCompression;
    bool		myMipMapFlag;
    bool		myMipMapAutoGenFlag;
    bool		myAutoCompressFlag;
    bool		myCompressFlag;
    bool		myValidFlag;
    bool		myMutableFlag;
    RE_TextureFilter	myCurrentFilterState;
    RE_TextureFilter	myFilter;
    int64		myTexSize;
    IMG_FileAlphaInfo	myAlphaInfo;
    UT_ValArray<void *> myReferences;
    UT_ValArray<void (*)(void *, RE_OGLTexture *)> myReferenceCBs;

    uint64		myBindlessHandle;
    int			myResidentCount;
    
protected:
    int			mySamples;
    friend class	RE_OGLRender;
    friend class	RE_OGLFramebuffer;
    friend class	RE_TextureMap;
};

inline RE_TextureDimension
RE_OGLTexture::getTextureType() const
{
    return myTexDimension;
}

inline void
RE_OGLTexture::setDataType(RE_TextureDataType type)
{
    myTextureDataType = type;

    if(myVectorSize > 0)
	setFormat(myDataType, myVectorSize, myFormatExtra);
}

inline RE_TextureDataType
RE_OGLTexture::getDataType() const
{
    return myTextureDataType;
}

inline bool
RE_OGLTexture::isValid() const
{
    return myID != 0 && myValidFlag;
}

inline RE_TextureID
RE_OGLTexture::getID() const
{
    return myID;
}

inline RE_GPUType
RE_OGLTexture::getFormatType() const
{
    return myDataType;
}

inline RE_GPUType
RE_OGLTexture::getClientType() const
{
    return myClientType;
}

inline int
RE_OGLTexture::getFormatSize() const
{
    return myVectorSize;
}

inline RE_TextureFormatExtra
RE_OGLTexture::getFormatExtra() const
{
    return myFormatExtra;
}

inline int
RE_OGLTexture::getWidth() const
{
    return myWidth;
}

inline int
RE_OGLTexture::getHeight() const
{
    return myHeight;
}

inline int
RE_OGLTexture::getDepth() const
{
    return myDepth;
}

inline int
RE_OGLTexture::getGLWidth() const
{
    return myGLWidth;
}

inline int
RE_OGLTexture::getGLHeight() const
{
    return myGLHeight;
}

inline int
RE_OGLTexture::getGLDepth() const
{
    return myGLDepth;
}

inline RE_TextureCompress
RE_OGLTexture::getCompression() const
{
    return myCompression;
}

inline bool
RE_OGLTexture::getAutoCompression() const
{
    return myAutoCompressFlag;
}

inline bool
RE_OGLTexture::hasMipMapSupport(RE_Render *)
{
    return true;
}

inline bool
RE_OGLTexture::getMipMap() const
{
    return myMipMapFlag;
}

inline bool
RE_OGLTexture::isMipMapped() const
{
    return myMipMapFlag && myCanMipMap;
}

inline bool
RE_OGLTexture::getMipMapAutoGenerate() const
{
    return myMipMapAutoGenFlag;
}

inline RE_TexFiltType
RE_OGLTexture::getMinFilter() const
{
    return myCurrentFilterState.getMinFilter();
}

inline RE_TexFiltType
RE_OGLTexture::getMagFilter() const
{
    return myCurrentFilterState.getMagFilter();
}

inline float
RE_OGLTexture::getLodBias() const
{
    return myCurrentFilterState.getLodBias();
}

inline int
RE_OGLTexture::getMaxAnisotropy() const
{
    return myCurrentFilterState.getMaxAnisotropy();
}

inline bool
RE_OGLTexture::getWrapS() const
{
    return myCurrentFilterState.getWrapS();
}

inline bool
RE_OGLTexture::getWrapT() const
{
    return myCurrentFilterState.getWrapT();
}

inline bool
RE_OGLTexture::getWrapR() const
{
    return myCurrentFilterState.getWrapR();
}

inline const char *
RE_OGLTexture::className() const
{
    return  "RE_OGLTexture";
}

inline bool
RE_OGLTexture::hasTextureSupport(RE_Render *r)
{
    return true;
}

inline void
RE_OGLTexture::validate()
{
    myValidFlag = true;
}

inline int
RE_OGLTexture::getMaxTextureIndex(RE_Render *) 
{
    return 1;
}

inline float
RE_OGLTexture::getTexS() const
{
    return float(myWidth) / float(myGLWidth);
}

inline float
RE_OGLTexture::getTexT() const
{
    return float(myHeight) / float(myGLHeight);
}

inline float
RE_OGLTexture::getTexR() const
{
    return float(myDepth) / float(myGLDepth);
}

inline int64
RE_OGLTexture::getTextureSize() const
{
    return myTexSize;
}

inline int64
RE_OGLTexture::getScanBytes() const
{
    return getScanBytes(myGLWidth);
}


#endif
