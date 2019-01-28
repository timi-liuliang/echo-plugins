/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_OGLTexture.h ( RE Library, C++)
 *
 * COMMENTS:
 *	Base class for various OpenGL texture types.
 */
#ifndef RE_OGLTextureType_h
#define RE_OGLTextureType_h

typedef unsigned int RE_TextureID;

// The maximum number of texture units we support
#define RE_MAX_TEXTURES		48
#define RE_MAX_TEXTURE_COORDS	16

// The max number of times the texture state can be pushed (not the texture
// transform matrix). 
#define RE_MAX_TEXTURE_STATE_STACK	16

#define RE_BAD_UNIFORM_UNIT		-1
#define RE_ACTIVE_UNIT			-2
#define RE_ALL_UNITS			-3

#define RE_TEXTURE_BAD_WRAPPING		3

enum RE_TextureDimension
{
    // uninitlized or invalid texture
    RE_TEXTURE_NONE = 0,

    // Base OpenGL 1.0 textures
    RE_TEXTURE_1D,
    RE_TEXTURE_2D,

    // These textures require extensions
    RE_TEXTURE_3D,		       // RE_EXT_TEXTURE_3D
    RE_TEXTURE_CUBE,		       // RE_EXT_TEXTURE_CUBE_MAP	
    RE_TEXTURE_RECTANGLE,	       // RE_EXT_TEXTURE_RECTANGLE
    RE_TEXTURE_1D_ARRAY,	       // RE_EXT_TEXTURE_ARRAY
    RE_TEXTURE_2D_ARRAY,	       // RE_EXT_TEXTURE_ARRAY,RE_EXT_TEXTURE_3D
    RE_TEXTURE_2D_MULTISAMPLE,	       // RE_EXT_TEXTURE_MULTISAMPLE
    RE_TEXTURE_BUFFER		       // RE_EXT_TEXTURE_BUFFER
};

// Type of data the texture contains.
// Color can be 1-4 channels of any format but FLOAT24 and UINT1,2,4.
// Depth can take a single channel of RE_GPU_DEPTH32,24,16
// Stencil can take a single channel of RE_GPU_UINT1,2,4,8,16
// Integer must be 1-4 channels of RE_GPU_UINT or RE_GPU_INT.
// Note: FP and Integer textures require OpenGL support (check extensions
//       RE_EXT_TEXTURE_FLOAT and RE_EXT_TEXTURE_INTEGER before using)
enum RE_TextureDataType
{
    RE_TEXTURE_DATA_COLOR,	// Normal Color texture. 
    RE_TEXTURE_DATA_DEPTH,	// For ZDepth framebuffer attachments
    RE_TEXTURE_DATA_DEPTH_STENCIL, // 24b Z + 8b Stencil FBO attachments
    RE_TEXTURE_DATA_INTEGER	// Shaders read the actual integer values
};

    
enum RE_TexFiltType
{
    RE_FILT_MIN_FILTERS = -1,	// this isn't a real filter
    
    RE_FILT_NEAREST,
    RE_FILT_LINEAR,
    RE_FILT_NEAREST_MIPMAP_NEAREST,
    RE_FILT_NEAREST_MIPMAP_LINEAR,
    RE_FILT_LINEAR_MIPMAP_NEAREST,
    RE_FILT_LINEAR_MIPMAP_LINEAR,
    
    RE_FILT_MAX_FILTERS		// this isn't a real filter
};

enum RE_TexClampType
{
    RE_CLAMP_REPEAT = 0,
    RE_CLAMP_BORDER,
    RE_CLAMP_EDGE, 
    RE_CLAMP_MIRROR
};

enum RE_TextureBorder
{
    RE_TEX_BORDER_ZERO = 0,	// (0,0,0,0)
    RE_TEX_BORDER_BLACK,	// (0,0,0,1)
    RE_TEX_BORDER_WHITE,	// (1,1,1,1)
    RE_TEX_BORDER_COLOR		// user defined
};


enum RE_TextureCompress
{
    RE_COMPRESS_NONE,

    RE_COMPRESS_NO_FP32,        // native, except fp32 converted to fp16
    RE_COMPRESS_LOW_BIT_DEPTH,	// 8b for fixed, 16b FP for FP
    RE_COMPRESS_8_BIT_DEPTH,	// 8b for all types
    RE_COMPRESS_MIN_BIT_DEPTH,	// 4b for fixed, 9:9:9:5e FP for FP

    RE_COMPRESS_FP24,		// 24b FP, usually for Zdepth
    RE_COMPRESS_BIT,		// single bit per pixel

    RE_COMPRESS_GL_FAST,	// Let GL attempt to compress the image.
    RE_COMPRESS_GL_NICE,	// fast for streamed tex, nice for cached

    // Manual stuff.
    // 4:1 ratio, interpolated 16b color + 4b alpha
//     RE_COMPRESS_DXT_SHARP,	// sharp alpha changes - not implemented yet
//     RE_COMPRESS_DXT_GRADUAL	// gradual alpha changes - not implemented yet

    RE_COMPRESS_UNKNOWN		// Can't use to specify a compression.
};


// This controls how multitexturing and the GL base color interact. It acts on
// the previous color (for tex unit 0, that is the GL base color)
enum RE_TextureMode
{
    RE_TEXTURE_MODULATE,	// multiply (default)
    RE_TEXTURE_ADD,		// simple add
    RE_TEXTURE_DECAL,		// Over
    RE_TEXTURE_REPLACE,		// discard previous texture, partially probably
    
    // GL_BLEND and GL_COMBINE are currently not fully supported through RE.
    // You probably want to use GLSL rather than thesemore complicated modes.
    
    RE_TEXTURE_BLEND,		// Blend with previous color
    RE_TEXTURE_COMBINE,		// Combine of up to 3 RGB and 3 A textures.

    RE_TEXTURE_UNKNOWN
};

enum RE_TextureCubeFace
{
    RE_TEXTURE_CUBE_POS_X = 0,
    RE_TEXTURE_CUBE_NEG_X,
    RE_TEXTURE_CUBE_POS_Y,
    RE_TEXTURE_CUBE_NEG_Y,
    RE_TEXTURE_CUBE_POS_Z,
    RE_TEXTURE_CUBE_NEG_Z,

    RE_TEXTURE_CUBE_ALL_FACES	// All faces at once, scans stacked vertically
};				// in the order listed above

// For buffer textures.
enum RE_TextureBufferMode
{
    RE_TEXTURE_ASSIGN_BUFFER = -1,
    RE_TEXTURE_ASSIGN_DATA   = 0
};

enum RE_TextureCompare
{
    RE_TEXTURE_COMPARE_LESS,
    RE_TEXTURE_COMPARE_LESS_EQUAL,
    
    RE_TEXTURE_COMPARE_EQUALS,
    RE_TEXTURE_COMPARE_NOT_EQUAL,

    RE_TEXTURE_COMPARE_GREATER_EQUAL,
    RE_TEXTURE_COMPARE_GREATER
};

enum RE_TextureFormatExtra
{
    RE_TEXTURE_FORMAT_EXTRA_NONE,
    // Flags whether 2-channel formats use red/green (1,2,x,x) when shading
    // (RE_EXT_TEXTURE_2CHAN must be supported), instead of lum/alpha (1,1,1,2)
    RE_TEXTURE_FORMAT_EXTRA_2CHAN_USE_RG,

    // Flags whether 1-channel formats use alpha textures rather than luminance.
    RE_TEXTURE_FORMAT_EXTRA_1CHAN_USE_ALPHA
};

// For use with RE_Texture::setTextureSwizzle(). Requires RE_EXT_TEXTURE_SWIZZLE
enum RE_TextureSwizzle
{
    RE_SWIZZLE_RED,
    RE_SWIZZLE_GREEN,
    RE_SWIZZLE_BLUE,
    RE_SWIZZLE_ALPHA,
    RE_SWIZZLE_ZERO,
    RE_SWIZZLE_ONE
};

#endif
