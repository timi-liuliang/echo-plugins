/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_Types.h ( IMG Library, C++)
 *
 * COMMENTS:	IMG data types
 */

#ifndef __IMG_FILE_TYPES__
#define __IMG_FILE_TYPES__

#include "IMG_API.h"
#include <PXL/PXL_Common.h>

enum IMG_DataType
{
    IMG_DT_UNDEFINED = -1,

    // Allow bit masking of types so formats can specify which types they
    // support.
    IMG_DT_ANY	= 0x00,
    
    // All integer data types are unsigned.
    IMG_UCHAR	= 0x01,  IMG_INT8    = 0x01,
    IMG_USHORT	= 0x02,  IMG_INT16   = 0x02, 
    IMG_UINT	= 0x04,  IMG_INT32   = 0x04,

    IMG_FLOAT	= 0x08,  IMG_FLOAT32 = 0x08,
    IMG_HALF    = 0x10,  IMG_FLOAT16 = 0x10 
};

IMG_API const char 	*IMGgetNameFromDataType(IMG_DataType type);
IMG_API IMG_DataType	 IMGgetDataTypeFromName(const char *name);


/// How the channel data should be interpreted.
enum IMG_TypeInfo
{
    IMG_TI_UNDEFINED	= 0,
    IMG_TI_POSITION,
    IMG_TI_VECTOR,
    IMG_TI_NORMAL,
    IMG_TI_COLOR,
    IMG_TI_SAMPLE_COUNT		// Deep sample count pseudo-plane
};

IMG_API const char 	*IMGgetNameFromTypeInfo(IMG_TypeInfo type_info);
IMG_API IMG_TypeInfo	 IMGgetTypeInfoFromName(const char *name);


enum IMG_ColorModel
{
    IMG_CM_UNDEFINED = -1,

    IMG_CM_ANY	= 0,

    // basic channel configurations
    IMG_1CHAN	= 0x01,
    IMG_2CHAN   = 0x02,
    IMG_RGB	= 0x04,
    IMG_RGBA	= 0x08,

    // extension bits
    IMG_CM_REVERSED = 0x10,

    // commonly reversed formats
    IMG_BGR	= (IMG_RGB  | IMG_CM_REVERSED),
    IMG_ABGR	= (IMG_RGBA | IMG_CM_REVERSED)
};

IMG_API const char 	*IMGgetNameFromColorModel(IMG_ColorModel type_info);
IMG_API IMG_ColorModel	 IMGgetColorModelFromName(const char *name);


enum IMG_Interleave
{
    IMG_INTERLEAVE_AS_IS,
    IMG_INTERLEAVED,
    IMG_NON_INTERLEAVED
};

enum IMG_ComponentOrder
{
    IMG_COMPONENT_AS_IS,
    IMG_COMPONENT_RGBA,
    IMG_COMPONENT_ABGR
};

#define IMG_DT_ALL ((IMG_DataType)0x1F)

static inline int IMGbyteSize(IMG_DataType dt)
   {
       if(dt == IMG_INT16 || dt == IMG_FLOAT16)
	   return 2;
       else if(dt == IMG_FLOAT32 || dt == IMG_INT32)
	   return 4;
       return 1;
   }

static inline PXL_DataFormat IMGformatToPXL(IMG_DataType dt)
{
    switch(dt)
    {
    case IMG_INT8:	return PXL_INT8;
    case IMG_INT16:	return PXL_INT16;
    case IMG_INT32:	return PXL_INT32;
    case IMG_FLOAT32:	return PXL_FLOAT32;
    case IMG_FLOAT16:	return PXL_FLOAT16;
    default:		return PXL_MAX_DATA_FORMAT;
    }
    // unknown format.
    return PXL_MAX_DATA_FORMAT;
}

static inline IMG_DataType IMGformatFromPXL(PXL_DataFormat dt)
{
    switch(dt)
    {
    case PXL_INT8:	return IMG_INT8;
    case PXL_INT16:	return IMG_INT16;
    case PXL_INT32:	return IMG_INT32;
    case PXL_FLOAT32:	return IMG_FLOAT32;
    case PXL_FLOAT16:	return IMG_FLOAT16;
    default:		return IMG_DT_ANY;
    }
    // unknown format.
    return IMG_DT_ANY;
}

static inline IMG_ColorModel IMGcolorModelFromPXL(PXL_Packing pack)
{
    switch(pack)
    {
    case PACK_SINGLE:	    return IMG_1CHAN;
    case PACK_DUAL:
    case PACK_DUAL_NI:	    return IMG_2CHAN;
    case PACK_RGB:
    case PACK_RGB_NI:	    return IMG_RGB;
    case PACK_RGBA:
    case PACK_RGBA_NI:	    return IMG_RGBA;
    default:		    return IMG_CM_ANY;
    }
}

static inline PXL_Packing IMGcolorModelToPXL(IMG_ColorModel cm,
					     bool interleave = true)
{
    switch(cm)
    {
    case IMG_1CHAN:	    return PACK_SINGLE;
    case IMG_2CHAN:	    return interleave ? PACK_DUAL : PACK_DUAL_NI;
    case IMG_RGB:	    return interleave ? PACK_RGB  : PACK_RGB_NI;
    case IMG_RGBA:	    return interleave ? PACK_RGBA : PACK_RGBA_NI;
    default:		    return PACK_UNKNOWN;
    }
}

static inline IMG_Interleave IMGinterleaveFromPXL(PXL_Packing pack)
{
    switch(pack)
    {
    case PACK_DUAL:
    case PACK_RGB:
    case PACK_RGBA:	return IMG_INTERLEAVED;
    case PACK_DUAL_NI:
    case PACK_RGB_NI:
    case PACK_RGBA_NI:	return IMG_NON_INTERLEAVED;
    default:		return IMG_INTERLEAVE_AS_IS;
    }
}

// 
// ------------------------------------------------------------------------
//

// size mask
#define IMG_CM_SIZE 0x0F
#define IMG_CM_ALL  ((IMG_ColorModel)0x0F)

static inline int IMGvectorSize(IMG_ColorModel cm)
   {
       int num;
       int sb = (cm & IMG_CM_SIZE);
       
       if(sb == IMG_1CHAN)
	   num=1;
       else if(sb == IMG_2CHAN)
	   num=2;
       else if(sb == IMG_RGB)
	   num=3;
       else if(sb == IMG_RGBA)
	   num=4;
       else
	   num=0;
       return num;
   }

static inline bool IMGisReversed(IMG_ColorModel cm)
   { return ((bool)(cm & IMG_CM_REVERSED)); }

static inline IMG_ColorModel
IMGcolorModelFromVectorSize(int vsize)
{
    switch(vsize)
    {
    case 1: return IMG_1CHAN;
    case 2: return IMG_2CHAN;
    case 3: return IMG_RGB;
    case 4: return IMG_RGBA;
    }
    return IMG_CM_UNDEFINED;
}

//
// ------------------------------------------------------------------------
//

// Image Orientation enums. 
enum IMG_XOrientation
{
    IMG_ORIENT_X_NONE,
    IMG_ORIENT_LEFT_FIRST,
    IMG_ORIENT_RIGHT_FIRST
};
    
enum IMG_YOrientation
{
    IMG_ORIENT_Y_NONE,
    IMG_ORIENT_TOP_FIRST,
    IMG_ORIENT_BOTTOM_FIRST
};

// Deep Raster RGBA preference
enum IMG_DeepRasterColor
{
    IMG_DEEP_RGBA_SPLIT,
    IMG_DEEP_RGBA_COMBINED,
    IMG_DEEP_RGBA_DONT_CARE
};

// return enum for the IMG_File::getAlphaInfo() method, triggered by
// IMG_FileParms::detectTrivialAlpha()
enum IMG_FileAlphaInfo
{
    IMG_ALPHA_OPAQUE,		// constant 1
    IMG_ALPHA_CONSTANT,		// constant valued
    IMG_ALPHA_CUTOUT,		// either 1.0 or 0.0
    IMG_ALPHA_VARYING,		// 
    IMG_ALPHA_OUT_OF_BOUNDS,	// < 0.0 or > 1.0
    IMG_ALPHA_NONE		// no alpha channel
};

/// Type of image we want to create or have opened.
enum IMG_ImageType
{
    IMG_TYPE_2D		= 1 << 0,	///> The image is a 2D image.
    IMG_TYPE_DEEP_PIXEL	= 1 << 1	///> The image is a deep pixel image.
};


// Depth interpolation settings for deep pixel images.
enum IMG_DepthInterp
{
    IMG_DI_UNDEFINED 	= -1,
    IMG_DI_DISCRETE 	= 0,
    IMG_DI_LINEAR 	= 1
};

IMG_API const char 	*IMGgetNameFromDepthInterp(IMG_DepthInterp type);
IMG_API IMG_DepthInterp  IMGgetDepthInterpFromName(const char *name);


#endif

