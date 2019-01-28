/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_Types.h ( UI Library, C++)
 *
 * COMMENTS:
 *	Common enums and defines for Open GL.
 */
#ifndef __RE_Types__
#define __RE_Types__

#include "RE_API.h"

#define RE_MAX_PICK_DIST	100

// The type of a window. Used to set hints for the windowing system
// to treat the window differently, and to set up borders and icon settings.
enum RE_WindowType
{
    RE_WINDOW_MAIN,		// An app window. App exits when all are closed.
    RE_WINDOW_NORMAL,		// A dialog window with min/max-imize buttons.
    RE_WINDOW_DIALOG,		// A transient dialog that should be on top 
				// of its parent window.
    RE_WINDOW_UTILITY,		// A non-transient (i.e., persistent) dialog
				// like a toolbox or palette.
    RE_WINDOW_SPLASH,		// A splash screen window.
    RE_WINDOW_TRANSIENT,	// A *very* transient window displayed
				// temporarily (like a tooltip or XCF slider).
				// Grabs the pointer & is not managed.
    RE_WINDOW_CHILD,		// A child of a window (probably a child of a
				// NORMAL window). Isn't managed.
    RE_WINDOW_MENU,		// A window for popup menus.
    RE_WINDOW_HELPTIP,		// Same as TRANSIENT, but without focus lock.

    RE_WINDOW_NORMAL_BORDERLESS,// A dialog window without a border, it is an extra type
    RE_WINDOW_INVALID_TYPE	// Not a type. Used to mean "uninitialized."
};
// should always point to the last type.
#define RE_NUM_WINDOW_TYPES RE_WINDOW_INVALID_TYPE
#include <UT/UT_Assert.h>
#include <UT/UT_ComputeGPU.h>
enum RE_GPUType
{
    RE_GPU_UINT1 = 0, // corresponds to GL_STENCIL_INDEX1
    RE_GPU_UINT4,     // corresponds to GL_STENCIL_INDEX4
    RE_GPU_UINT8,
    RE_GPU_UINT16,
    RE_GPU_UINT32,
	
    RE_GPU_INT8,
    RE_GPU_INT16,
    RE_GPU_INT32,

    RE_GPU_FLOAT16,
    RE_GPU_FLOAT24,   // corresponds to GL_DEPTH_COMPONENT24
    RE_GPU_FLOAT32,
    RE_GPU_FLOAT64,

    // These can only be used in the context of a GLSL shader.
    RE_GPU_MATRIX2,
    RE_GPU_MATRIX3,
    RE_GPU_MATRIX4
};

inline UT_GPUType REconvertToUTGPUType(RE_GPUType t)
{
    switch (t)
    {
	case RE_GPU_UINT1: return UT_GPU_UINT1;
	case RE_GPU_UINT4: return UT_GPU_UINT4;
	case RE_GPU_UINT8: return UT_GPU_UINT8;
	case RE_GPU_UINT16: return UT_GPU_UINT16;
	case RE_GPU_UINT32: return UT_GPU_UINT32;
	    
	case RE_GPU_INT8: return UT_GPU_INT8;
	case RE_GPU_INT16: return UT_GPU_INT16;
	case RE_GPU_INT32: return UT_GPU_INT32;

	case RE_GPU_FLOAT16: return UT_GPU_FLOAT16;
	case RE_GPU_FLOAT24: return UT_GPU_FLOAT24;
	case RE_GPU_FLOAT32: return UT_GPU_FLOAT32;
	case RE_GPU_FLOAT64: return UT_GPU_FLOAT64;

	case RE_GPU_MATRIX2: return UT_GPU_MATRIX2;
	case RE_GPU_MATRIX3: return UT_GPU_MATRIX3;
	case RE_GPU_MATRIX4: return UT_GPU_MATRIX4;
    }

    UT_ASSERT(!"Unknown RE_GPUType.");
    return UT_GPU_UINT8;
}

inline RE_GPUType REconvertFromGPUType(UT_GPUType t)
{
    switch (t)
    {
	case UT_GPU_UINT1: return RE_GPU_UINT1;
	case UT_GPU_UINT4: return RE_GPU_UINT4;
	case UT_GPU_UINT8: return RE_GPU_UINT8;
	case UT_GPU_UINT16: return RE_GPU_UINT16;
	case UT_GPU_UINT32: return RE_GPU_UINT32;
	    
	case UT_GPU_INT8: return RE_GPU_INT8;
	case UT_GPU_INT16: return RE_GPU_INT16;
	case UT_GPU_INT32: return RE_GPU_INT32;

	case UT_GPU_FLOAT16: return RE_GPU_FLOAT16;
	case UT_GPU_FLOAT24: return RE_GPU_FLOAT24;
	case UT_GPU_FLOAT32: return RE_GPU_FLOAT32;
	case UT_GPU_FLOAT64: return RE_GPU_FLOAT64;

	case UT_GPU_MATRIX2: return RE_GPU_MATRIX2;
	case UT_GPU_MATRIX3: return RE_GPU_MATRIX3;
	case UT_GPU_MATRIX4: return RE_GPU_MATRIX4;
    }

    UT_ASSERT(!"Unknown UT_GPUType.");
    return RE_GPU_UINT8;
}

// defined in RE_OGLRender.C
RE_API extern const char *REgetTypeName(RE_GPUType p);

#include <PXL/PXL_Common.h>
#include <IMG/IMG_FileTypes.h>
inline RE_GPUType REconvertIMGType(IMG_DataType t)
{
    switch(t)
    {
    default:
    case IMG_INT8:	return RE_GPU_UINT8;
    case IMG_INT16:	return RE_GPU_UINT16;
    case IMG_INT32:	return RE_GPU_UINT32;
    case IMG_FLOAT32:	return RE_GPU_FLOAT32;
    case IMG_FLOAT16:	return RE_GPU_FLOAT16;
    }

    UT_ASSERT(!"Unknown type");
    return RE_GPU_UINT8;
}

inline RE_GPUType REconvertPXLType(PXL_DataFormat t)
{
    return REconvertFromGPUType(PXLconvertToUTGPUType(t));
}

// Function to get size in bits of a RE_GPUType
inline unsigned int REsizeOfGPUType(RE_GPUType gpu_type)
{
    int bpp = 32;
    
    switch(gpu_type)
    {
	case RE_GPU_UINT1:
	    bpp = 1;
	    break;
	case RE_GPU_UINT4:
	    bpp = 4;
	    break;
	case RE_GPU_UINT8:
	case RE_GPU_INT8:
	    bpp = 8;
	    break;
	case RE_GPU_FLOAT16:
	case RE_GPU_UINT16:
	case RE_GPU_INT16:
	    bpp = 16;
	    break;
        case RE_GPU_FLOAT64:
	    bpp = 64;
	    break;
	case RE_GPU_MATRIX2:
	    bpp = 32 * 4;
	    break;
	case RE_GPU_MATRIX3:
	    bpp = 32 * 9;
	    break;
	case RE_GPU_MATRIX4:
	    bpp = 32 * 16;
	    break;
	// FLOAT24 is padded to 32
	default:
	    break;
    }
    
    return bpp;
}


// RE definitions for GL primitives
enum RE_PrimType
{
    RE_PRIM_NONE,
    RE_PRIM_AS_IS = RE_PRIM_NONE,
    
    RE_PRIM_POINTS,
    
    RE_PRIM_LINES,
    RE_PRIM_LINE_STRIP,
    RE_PRIM_LINE_LOOP,

    RE_PRIM_TRIANGLES,
    RE_PRIM_TRIANGLE_STRIP,
    RE_PRIM_TRIANGLE_FAN,

    RE_PRIM_LINES_ADJACENT,		// for geometry shaders; includes
    RE_PRIM_LINE_STRIP_ADJACENT,	// extra vertices outside the primitive
    RE_PRIM_TRIANGLES_ADJACENT,		// for curvature
    RE_PRIM_TRIANGLE_STRIP_ADJACENT,

    RE_PRIM_POLYGONS,			// General N-sided polygons

    RE_PRIM_PATCHES,			// Patches sent to tessellation shaders

    RE_NUM_PRIM_TYPES
};

// defined in RE_OGLRender.C
RE_API extern int REgetGLPrim(RE_PrimType t);
RE_API extern RE_PrimType REgetREPrim(int gltype);
RE_API extern const char *REgetPrimName(RE_PrimType p);


// GLSL Shader stage types (bitfield in case a shader function can be applied
// to more than one stage type)
enum RE_ShaderType
{
    RE_SHADER_VERTEX		= 0x1,
    RE_SHADER_GEOMETRY		= 0x2,
    RE_SHADER_FRAGMENT		= 0x4,
    RE_SHADER_TESSCONTROL	= 0x8,
    RE_SHADER_TESSEVAL		= 0x10,

    RE_SHADER_COMPUTE		= 0x20, // cannot be used with other stages

    RE_SHADER_ALL		= 0x3f
};

// Shader language type - now just GLSL.
enum RE_ShaderLanguage
{
    RE_SHADER_LANGUAGE_GLSL		= 1,
    RE_SHADER_LANGUAGE_COMPUTE		= 2,
    RE_SHADER_LANGUAGE_ALL		= ~0
};

// Basic Primitive type that a shader program is designed for.
enum RE_ShaderTarget
{
    RE_SHADER_TARGET_POINT = 0,
    RE_SHADER_TARGET_LINE,
    RE_SHADER_TARGET_TRIANGLE,

    RE_SHADER_NUM_TARGETS // sentinel
};

// returns the shader target that a given prim would require.
RE_API extern RE_ShaderTarget REgetShaderTargetForPrim(RE_PrimType type);
RE_API extern RE_ShaderTarget REgetShaderTargetForGLPrim(int gltype);

// RE definitions for the various GL buffer objects
enum RE_BufferType
{
    RE_BUFFER_ATTRIBUTE     = 0x001,	// generic vertex attribute
    RE_BUFFER_ELEMENT       = 0x002,	// vertex connectivity index list
	
    RE_BUFFER_UNIFORM	    = 0x004,	// uniform buffer object
    RE_BUFFER_PIXEL_WRITE   = 0x008,	// pixels move from CPU->GPU
    RE_BUFFER_PIXEL_READ    = 0x010,	// pixels move from GPU->CPU
    RE_BUFFER_TEXTURE	    = 0x020	// For buffer textures
};

enum RE_BufferUsageHint
{
    // passing data to GPU only.
    RE_BUFFER_WRITE_ONCE,	// written once only, redrawn many
    RE_BUFFER_WRITE_FREQUENT,	// written several times, redrawn many
    RE_BUFFER_WRITE_STREAM,	// one write per redraw

    // reading data from GPU only.
    RE_BUFFER_READ_ONCE,
    RE_BUFFER_READ_FREQUENT,
    RE_BUFFER_READ_STREAM,

    // read and write (ex: rendering to a vertex list to use as geo)
    RE_BUFFER_READ_WRITE_ONCE,
    RE_BUFFER_READ_WRITE_FREQUENT,
    RE_BUFFER_READ_WRITE_STREAM
};

enum RE_BufferAccess
{
    RE_BUFFER_READ_ONLY,
    RE_BUFFER_WRITE_ONLY,
    RE_BUFFER_READ_WRITE
};

enum RE_BufferBinding
{
    RE_BUFFER_BINDING_ARRAY,
    RE_BUFFER_BINDING_ELEMENT,
    RE_BUFFER_BINDING_PIXEL_WRITE,
    RE_BUFFER_BINDING_PIXEL_READ,
    RE_BUFFER_BINDING_UNIFORM,
    RE_BUFFER_BINDING_TEXTURE,
    RE_BUFFER_BINDING_TRANSFORM_FEEDBACK,
    RE_BUFFER_BINDING_COMPUTE_INDIRECT
};

enum RE_ArrayType
{
    RE_ARRAY_VERTEX,		// sampled at vertex frequency
    RE_ARRAY_POINT,		// sampled at point frequency (default)
    RE_ARRAY_PRIMITIVE,		// sampled at primitive frequency
    RE_ARRAY_INSTANCE,		// sampled at instance frequency
    RE_ARRAY_DETAIL,		// sampled at detail frequency
    RE_ARRAY_RANDOM,		// randomly sampled
    RE_ARRAY_NULL		// placeholder only for vertex state match
};

enum RE_GenericAttribID
{
    RE_GENATTRIB_UNDEF = -2,
    RE_GENATTRIB_NONE  = -1,
    
    RE_GENATTRIB_P = 0,
    RE_GENATTRIB_CD,
    RE_GENATTRIB_ALPHA,
    RE_GENATTRIB_N,
    RE_GENATTRIB_UV,
    RE_GENATTRIB_PSCALE,
    RE_GENATTRIB_INSTANCE_INDEX,

    RE_GENATTRIB_POINT_ID,
    RE_GENATTRIB_PRIM_ID,
    RE_GENATTRIB_PRIM_INFO,
    RE_GENATTRIB_VERT_INFO,
    
    RE_GENATTRIB_POINT_SELECTION,
    RE_GENATTRIB_PRIM_SELECTION,
    RE_GENATTRIB_VERT_SELECTION,
    
    RE_GENATTRIB_INSTANCE_SELECTION,
    RE_GENATTRIB_INSTANCE_TRANSFORM,
    RE_GENATTRIB_INSTANCE_CD,
    RE_GENATTRIB_INSTANCE_TEX,
    RE_GENATTRIB_INSTANCE_ID,

    // sentinel
    RE_GENATTRIB_NUM_IDS
};

/// Vertex shader input locations for known attribute types. All others
/// attributes are assigned starting at location 'RE_GENLOC_FIRST_CUSTOM'.
enum RE_ShaderAttribLocation
{
    RE_LOC_UNDEF		= -2,
    RE_LOC_NONE			= -1,
    
    RE_LOC_P			= 0,
    RE_LOC_CD			= 1,
    RE_LOC_ALPHA		= 2,
    RE_LOC_N			= 3,
    RE_LOC_UV			= 4,
    RE_LOC_PSCALE		= 5,
    RE_LOC_POINT_SELECTION	= 6,
    RE_LOC_POINT_ID		= 7,
    RE_LOC_INSTANCE_INDEX	= 8,
    
    RE_LOC_PRIM_ID		= 9,
    RE_LOC_PRIM_SELECTION	= 10,
    RE_LOC_VERT_SELECTION	= 11,

    RE_LOC_FIRST_CUSTOM
};

#if defined(WIN32) || defined(MBSD_COCOA)
typedef void * Display;
#endif

// Default GL cache size. 32b machines get 384MB, while 64b machines get 2GB.
#ifdef AMD64
#define RE_TEXCACHE_DEFAULT_MAX_SIZE	int64(512) * 1024 * 1024
#define RE_BUFCACHE_DEFAULT_MAX_SIZE	int64(2) * 1024 * 1024 * 1024
#else
#define RE_TEXCACHE_DEFAULT_MAX_SIZE	96 * 1024 * 1024
#define RE_BUFCACHE_DEFAULT_MAX_SIZE	384 * 1024 * 1024
#endif

#define RE_DEFAULT_DPI 85.0
#define RE_SCALE_BELOW_RESOLUTION	1000
#define RE_MIN_RESOLUTION_SCALE_FACTOR	0.85f

#define MM_TO_INCHES(mm)	((mm) / 25.4)

#define RE_HALF_TONE_ALPHA		0.6f
#define RE_QUARTER_TONE_ALPHA		0.3f

enum RE_RenderBuf
{
    RE_BUFFER_UNINIT = 0,    // current buffer not known
    RE_FRONT,	    // front buffer
    RE_BACK,	    // back buffer (must be double buffered; single use front)
    RE_BACK_LEFT,   // back-left buffer for quad buffered stereo
    RE_BACK_RIGHT,  // back-right buffer for quad buffered stero

    RE_FBO_ZS,	    // for rendering to FBO depth or stencil attachments
    RE_FBO_COLOR    // for rendering to FBO attachments 0-15; add the # to
		    // this enum. 
    
    // RE_BOTH has been removed because the back buffer is no longer valid
    // after swapping, so you shouldn't need to write to both ever.
};

enum RE_MatrixMode
{
    RE_MATRIX_VIEWING,
    RE_MATRIX_PROJECTION
};

enum RE_LineStyle
{
    RE_LINE_SOLID	= 0,
    RE_LINE_DOTTED	= 1
};

enum RE_SmoothMode
{
    RE_SMOOTH_OFF	= 0,
    RE_SMOOTH_ON	= 1
};

enum RE_FaceMode
{
    RE_FACE_FRONT,
    RE_FACE_BACK,
    RE_FACE_BOTH
};

enum RE_ZFunction
{
    RE_ZNEVER,
    RE_ZLESS,
    RE_ZEQUAL,
    RE_ZLEQUAL,
    RE_ZGREATER,
    RE_ZNOTEQUAL,
    RE_ZGEQUAL,
    RE_ZALWAYS
};

enum RE_SFunction
{
    RE_SNEVER,
    RE_SLESS,
    RE_SEQUAL,
    RE_SLEQUAL,
    RE_SGREATER,
    RE_SNOTEQUAL,
    RE_SGEQUAL,
    RE_SALWAYS
};

enum RE_SOperation
{
    RE_SKEEP,
    RE_SZERO,
    RE_SREPLACE,
    RE_SINCR,
    RE_SDECR,
    RE_SINCR_WRAP,
    RE_SDECR_WRAP,
    RE_SINVERT
};

enum RE_BlendSourceFactor
{
    RE_SBLEND_ZERO,
    RE_SBLEND_DST_COLOR,
    RE_SBLEND_ONE_MINUS_DST_COLOR,
    RE_SBLEND_SRC_ALPHA,
    RE_SBLEND_ONE_MINUS_SRC_ALPHA,
    RE_SBLEND_DST_ALPHA,
    RE_SBLEND_ONE_MINUS_DST_ALPHA,
    RE_SBLEND_SRC_ALPHA_SATURATE,
    RE_SBLEND_ONE
};

enum RE_BlendDestFactor
{
    RE_DBLEND_ONE,
    RE_DBLEND_SRC_COLOR,
    RE_DBLEND_ONE_MINUS_SRC_COLOR,
    RE_DBLEND_SRC_ALPHA,
    RE_DBLEND_ONE_MINUS_SRC_ALPHA,
    RE_DBLEND_DST_ALPHA,
    RE_DBLEND_ONE_MINUS_DST_ALPHA,
    RE_DBLEND_ZERO
};

enum RE_PersistentBufferMode
{
    RE_PERSISTENT_BUFFER_DISABLE,
    RE_PERSISTENT_BUFFER_ENABLE,
    RE_PERSISTENT_BUFFER_COHERENT
};

// keep in sync with the GL barrier types (GL_*_BARRIER_BIT)
enum RE_MemoryBarrierBitfield
{
    RE_BARRIER_VERTEX_ATTRIB	  = 0x1,
    RE_BARRIER_ELEMENT_ARRAY	  = 0x2,
    RE_BARRIER_UNIFORM		  = 0x4,
    RE_BARRIER_TEXTURE_FETCH	  = 0x8,
    RE_BARRIER_SHADER_IMAGE	  = 0x20,
    RE_BARRIER_COMMAND		  = 0x40,
    RE_BARRIER_PIXEL_BUFFER	  = 0x80,
    RE_BARRIER_TEXTURE_UPDATE	  = 0x100,
    RE_BARRIER_BUFFER_UPDATE	  = 0x200,
    RE_BARRIER_FRAMEBUFFER	  = 0x400,
    RE_BARRIER_TRANSFORM_FEEDBACK = 0x800,
    RE_BARRIER_ATOMIC_COUNTER     = 0x1000,
    RE_BARRIER_SHADER_STORAGE	  = 0x2000,
    RE_BARRIER_PERSISTENT_MAP	  = 0x4000,
    RE_BARRIER_QUERY_BUFFER	  = 0x8000,

    RE_BARRIR_ALL		  = 0xFFFFFFFF
};

#define RE_MODE_MULTISAMPLE_MASK  0x60
#define RE_MODE_MULTISAMPLE_SHIFT 5

enum RE_DisplayMode
{
    // The mode components
    RE_MODE_NONE		= 0x000,
    RE_MODE_RGB			= 0x001,	// RGB mode (req'd)
    RE_MODE_DOUBLE		= 0x002,	// Double buffer
    RE_MODE_ZBUFFER		= 0x004,	// Depth buffer
    RE_MODE_ALPHA		= 0x008,        // Alpha buffer (RGBA)
    RE_MODE_STENCIL		= 0x010,	// Stencil buffer
    RE_MODE_MULTISAMPLE_LOW	= 0x020,	// 2x Full Scene AA
    RE_MODE_MULTISAMPLE_MED	= 0x040,	// 4x Full Scene AA
    RE_MODE_MULTISAMPLE_HIGH	= 0x060,	// 8x (NV) / 6x (ATI) FS AA
    RE_MODE_ACCUM		= 0x080,	// Color Accumulation buffer
    RE_MODE_ACCUM_ALPHA		= 0x100,	// Alpha accum (needs color acc)
    RE_MODE_HDR			= 0x200,	// HDR FP16 (FBO only)
    RE_MODE_HDR_FP32		= 0x400,	// HDR FP32 (FBO only)
    RE_MODE_STEREO		= 0x800,	// quad buffer stereo

    // The actual modes themselves; convenience enums.
    RE_MODE_RGB_SINGLE		= RE_MODE_RGB,
    RE_MODE_RGB_DOUBLE		= RE_MODE_RGB | RE_MODE_DOUBLE,
    
    RE_MODE_RGBA_SINGLE		= RE_MODE_RGB | RE_MODE_ALPHA,
    RE_MODE_RGBA_DOUBLE		= RE_MODE_RGB | RE_MODE_DOUBLE | RE_MODE_ALPHA,
    
    RE_MODE_RGBZ_SINGLE		= RE_MODE_RGB | RE_MODE_ZBUFFER,
    RE_MODE_RGBZ_DOUBLE		= RE_MODE_RGB | RE_MODE_DOUBLE|RE_MODE_ZBUFFER,

    RE_MODE_RGBSZ_SINGLE	= RE_MODE_RGBZ_SINGLE | RE_MODE_STENCIL,
    RE_MODE_RGBSZ_DOUBLE	= RE_MODE_RGBZ_DOUBLE | RE_MODE_STENCIL,
    
    RE_MODE_RGBAZ_SINGLE	= RE_MODE_RGB | RE_MODE_ALPHA |RE_MODE_ZBUFFER,
    RE_MODE_RGBAZ_DOUBLE	= RE_MODE_RGB | RE_MODE_DOUBLE |RE_MODE_ALPHA |
                                  RE_MODE_ZBUFFER,

    RE_MODE_RGBS_SINGLE		= RE_MODE_RGB | RE_MODE_STENCIL,
    RE_MODE_RGBS_DOUBLE		= RE_MODE_RGB | RE_MODE_DOUBLE|RE_MODE_STENCIL,

    RE_MODE_RGBAS_SINGLE	= RE_MODE_RGB | RE_MODE_ALPHA |RE_MODE_STENCIL,
    RE_MODE_RGBAS_DOUBLE	= RE_MODE_RGB | RE_MODE_DOUBLE |RE_MODE_ALPHA |
                                  RE_MODE_STENCIL,

    RE_MODE_RGB_BEST		= RE_MODE_RGB_DOUBLE,
    RE_MODE_RGBA_BEST		= RE_MODE_RGBA_DOUBLE,
    RE_MODE_RGBZ_BEST		= RE_MODE_RGBZ_DOUBLE,
    RE_MODE_RGBAZ_BEST		= RE_MODE_RGBAZ_DOUBLE,
    RE_MODE_RGBS_BEST		= RE_MODE_RGBS_DOUBLE,
    RE_MODE_RGBAS_BEST		= RE_MODE_RGBAS_DOUBLE
};

enum	RE_VisualType
{
    RE_NORMAL_VIS,
    RE_OVERLAY_VIS,
    RE_FULLSCREEN_OVERLAY_VIS
};

enum RE_Severity
{
    RE_SEVERITY_MESSAGE =0,		// ARB_debug_group only
    
    RE_SEVERITY_LOW,
    RE_SEVERITY_MEDIUM,
    RE_SEVERITY_HIGH
};

// This is a bitmask.
// *** You must update REgetDeviceName() if you change this ***
enum RE_GraphicsDevice
{
    RE_DEVICE_UNKNOWN	    = 0x0,
    
    RE_DEVICE_ATI	    = 0x1,
    RE_DEVICE_NVIDIA	    = 0x2,
    RE_DEVICE_INTEL_GMA     = 0x4,	// Intel GMA (not supported)
    RE_DEVICE_INTEL_HD	    = 0x8,	// Intel HD graphics
    RE_DEVICE_GDI	    = 0x10,	// Software OGL on Windows
    RE_DEVICE_MESA	    = 0x20,
    
    RE_DEVICE_PRO	    = 0x100,    // Professional series
    RE_DEVICE_MAC	    = 0x200,	// Mac versions of drivers
    
    RE_DEVICE_ATI_PRO	    = (RE_DEVICE_PRO|RE_DEVICE_ATI),	// FireGL/Pro
    RE_DEVICE_NVIDIA_PRO    = (RE_DEVICE_PRO|RE_DEVICE_NVIDIA),	// Quadro
    RE_DEVICE_INTEL_PRO	    = (RE_DEVICE_PRO|RE_DEVICE_INTEL_HD)// n/a?
};

RE_API extern RE_GraphicsDevice REdetermineGraphicsDevice(
				    const char *renderstring);

inline bool REisProfessionalDevice(RE_GraphicsDevice dev)
{
    return (dev & RE_DEVICE_PRO);
}

inline bool REisConsumerDevice(RE_GraphicsDevice dev)
{
    return !(dev & RE_DEVICE_PRO);
}

class UT_String;
RE_API void REgetDeviceName(RE_GraphicsDevice dev, UT_String &name);

#include <UT/UT_IntrusivePtr.h>
class RE_Material;
typedef UT_IntrusivePtr<RE_Material>	RE_MaterialPtr;
class RE_OGLBuffer;
typedef UT_IntrusivePtr<RE_OGLBuffer>	RE_OGLBufferHandle;

#include <UT/UT_StringArray.h>

typedef UT_StringArray RE_OverrideList;

class IMG_Raster;
typedef IMG_Raster	*(*RE_IconLoader)(int, int);

class PXL_Lookup;

class RE_API RE_RasterOpts
{
public:
    RE_RasterOpts() : myZoomX(-1), myZoomY(-1),
		      myColorScale(1.0f), myColorShift(0.0f),
		      myColorBlack(0.0f), myColorWhite(1.0f),
		      myAlphaScale(1.0f), myAlphaShift(0.0f),
		      myGamma(1.0f), myAspectRatio(1.0f), myVScale(1.0f),
		      myLUTSize(0), myComponent(-1), myModifyAlpha(false),
		      myDither(true), myComponentsAsGrey(true),
		      myLUTObject(0), myTransparent(0), myPremultiplyFlag(0),
		      myFragment16bit(0), myStereoMode(0), myPreBright(0),
		      myIgnoreGamma(false), myColorCorrectDisable(false)
	{ myLUT[0] = myLUT[1] = myLUT[2] = myLUT[3] = 0; }

    float myZoomX;
    float myZoomY;
    float myColorScale;
    float myColorShift;
    float myColorBlack;
    float myColorWhite;
    float myAlphaScale;
    float myAlphaShift;
    float myGamma;
    float myAspectRatio;
    float myVScale;
    float *myLUT[4];
    int	  myLUTSize;
    int	  myComponent;
    unsigned  myModifyAlpha :1,
	      myDither : 1,
	      myComponentsAsGrey :1,
	      myTransparent :1,
	      myPremultiplyFlag:1,
	      myFragment16bit : 2,
	      myStereoMode : 2,
	      myPreBright : 1,
	      myIgnoreGamma : 1,
	      myColorCorrectDisable : 1;
    PXL_Lookup *myLUTObject;
};



// stack item for blending & smoothing (which also modifies the blend state)
class RE_API re_BlendSmoothState
{
public:
    re_BlendSmoothState()
	{
	    mySmoothPushed = false;
	    mySmoothRequest = false;
	    mySmoothEnable = false;
	    myForceSmooth = 0;
	    myPrevLineWidth = -1.0f;
	    
	    myBlend = 0;
	    mySourceFactor = RE_SBLEND_ONE;
	    myDestFactor = RE_DBLEND_ZERO;
	    myAlphaSourceFactor = RE_SBLEND_ONE;
	    myAlphaDestFactor = RE_DBLEND_ZERO;
	}

    void copy(const re_BlendSmoothState &state)
    {
	    mySmoothPushed = state.mySmoothPushed;
	    mySmoothRequest = state.mySmoothRequest;
	    mySmoothEnable = state.mySmoothEnable;
	    myForceSmooth = state.myForceSmooth;
	    myPrevLineWidth = state.myPrevLineWidth;
	    
	    myBlend = state.myBlend;
	    mySourceFactor = state.mySourceFactor;
	    myDestFactor = state.myDestFactor;
	    myAlphaSourceFactor = state.myAlphaSourceFactor;
	    myAlphaDestFactor = state.myAlphaDestFactor;
    }

    void get(bool &req_smooth, int &blend,
	     RE_BlendSourceFactor &s, RE_BlendDestFactor &d,
	     RE_BlendSourceFactor &sa, RE_BlendDestFactor &da) const
	{
	    req_smooth = mySmoothRequest;
	    blend = myBlend;
	    s = mySourceFactor;
	    d = myDestFactor;
	    sa = myAlphaSourceFactor;
	    da = myAlphaDestFactor;
	}

    void invalidate()
	{ 
	    mySmoothRequest = false;
	    mySmoothEnable = -1;
	    myForceSmooth = -1;
	    myBlend = -1;
	    mySourceFactor = (RE_BlendSourceFactor)-1;
	    myDestFactor = (RE_BlendDestFactor)-1;
	    myAlphaSourceFactor = (RE_BlendSourceFactor)-1;
	    myAlphaDestFactor = (RE_BlendDestFactor)-1;
	}

    bool isValid()
	{
	    return (mySmoothEnable != -1 &&
		    myForceSmooth != -1 &&
		    myBlend != -1 &&
		    mySourceFactor != (RE_BlendSourceFactor)-1 &&
		    myDestFactor != (RE_BlendDestFactor)-1 &&
		    myAlphaSourceFactor != (RE_BlendSourceFactor)-1 &&
		    myAlphaDestFactor != (RE_BlendDestFactor)-1);
	}

    bool		mySmoothRequest;
    int			mySmoothEnable;
    int			myForceSmooth;
    bool		mySmoothPushed;
    float		myPrevLineWidth;
    int			 myBlend;
    RE_BlendSourceFactor mySourceFactor;
    RE_BlendDestFactor	 myDestFactor;
    RE_BlendSourceFactor myAlphaSourceFactor;
    RE_BlendDestFactor	 myAlphaDestFactor;
};

// Stack item for stenciling
class RE_API re_StencilState
{
public:
    re_StencilState()
	{
	    myEnable = false;
	    myClearValue = 0;
	    myWriteMask = 1;
	    myFunc = RE_SALWAYS;
	    myRef = 0;
	    myValueMask = 1;
	    myFailOperation = RE_SKEEP;
	    myPassDepthFailOperation = RE_SKEEP;
	    myPassDepthPassOperation = RE_SKEEP;
	}
    
    void set(bool	    enable,
	     int	    clearValue,
	     int	    writeMask,
	     RE_SFunction   func,
	     int	    ref,
	     int	    valueMask,
	     RE_SOperation  failOperation,
	     RE_SOperation  passDepthFailOperation,
	     RE_SOperation  passDepthPassOperation)
	{
	    myEnable = enable;
	    myClearValue = clearValue;
	    myWriteMask = writeMask;
	    myFunc = func;
	    myRef = ref;
	    myValueMask = valueMask;
	    myFailOperation = failOperation;
	    myPassDepthFailOperation = passDepthFailOperation;
	    myPassDepthPassOperation = passDepthPassOperation;
	}
    
    void get(bool	    &enable,
	     int	    &clearValue,
	     int	    &writeMask,
	     RE_SFunction   &func,
	     int	    &ref,
	     int	    &valueMask,
	     RE_SOperation  &failOperation,
	     RE_SOperation  &passDepthFailOperation,
	     RE_SOperation  &passDepthPassOperation) const
	{
	    enable = myEnable;
	    clearValue = myClearValue;
	    writeMask = myWriteMask;
	    func = myFunc;
	    ref = myRef;
	    valueMask = myValueMask;
	    failOperation = myFailOperation;
	    passDepthFailOperation = myPassDepthFailOperation;
	    passDepthPassOperation = myPassDepthPassOperation;
	}
    
    bool	    myEnable;
    int		    myClearValue;
    int		    myWriteMask;
    RE_SFunction    myFunc;
    int		    myRef;
    int		    myValueMask;
    RE_SOperation   myFailOperation;
    RE_SOperation   myPassDepthFailOperation;
    RE_SOperation   myPassDepthPassOperation;
};

// Depth stack item for depth buffer state
class RE_API re_DepthState
{
public:
    re_DepthState()
    {
	myDepthTest = -1;
	myDepthBufferWrite = -1;
	myZFunction = (RE_ZFunction) -1;
	myDepthClamp = -1;
	myZNear = 0.0;
	myZFar  = 1.0;
    }

    int		 myDepthTest;
    int		 myDepthClamp;
    int		 myDepthBufferWrite;
    RE_ZFunction myZFunction;
    double	 myZNear;
    double	 myZFar;
};

#define PATTERN_STACK_SIZE		 7
#define RE_SMOOTH_STACK_SIZE		16
#define RE_STENCIL_STACK_SIZE		16
#define RE_ALIGN_STACK_SIZE		16
#define RE_FRAMEBUFFER_STACK_SIZE	 8
#define RE_SHADER_STACK_SIZE		16
#define RE_BUFFER_STACK_SIZE		 8
#define RE_DEPTH_STACK_SIZE		16
#define RE_UNIFORM_STACK_SIZE		16


#define RE_STACK_DEPTH		256

// ============================================================================
// Platlform-specific implementation defines, typedefs and declarations.
// At the moment there is only the Qt implementation.
// ============================================================================

// We use RE_OGLContext and only cast at the last possible minute,
// to avoid problems compiling against certain OpenGL drivers.
class QOpenGLContext;
typedef QOpenGLContext *RE_OGLContext;

class RE_GLDrawable;
typedef RE_GLDrawable *OGLDrawable;

#endif
