/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_Defines.h	(Pixel Library, C++)
 *
 * COMMENTS:
 *	
 */
#ifndef PXL_DEFINES_H
#define PXL_DEFINES_H

#include <limits.h>
#include "PXL_API.h"
#include <UT/UT_Assert.h>
#include <UT/UT_ComputeGPU.h>

enum PXL_DataFormat
{
    PXL_INT8 = 0,
    PXL_INT16,
    PXL_INT32,
    PXL_FLOAT32,
    PXL_FLOAT16,
    PXL_MAX_DATA_FORMAT
};

enum PXL_Packing
{
    PACK_SINGLE	=0,	// single channel
    PACK_DUAL,		// 2 channel interleaved + 1 black channel. (12B12B). 
    PACK_DUAL_NI,	// 2 channel, non-interleaved (11111....12222222...2)
    PACK_RGB,		// RGB interleaved (rgbrgb...rgb)
    PACK_RGBA,		// RGBA interleaved 
    PACK_RGB_NI,	// RGB non-interleaved (rrr...rggg...gbbb..b)
    PACK_RGBA_NI,	// RGBA non-interleaved
    
    PACK_UNKNOWN = 255
};

enum PXL_ColorSpace
{
    PXL_CS_UNKNOWN,
    
    PXL_CS_LINEAR,
    PXL_CS_GAMMA2_2,
    PXL_CS_CUSTOM_GAMMA,
    PXL_CS_OCIO,

    // For future support:
    PXL_CS_SRGB,
    PXL_CS_REC709,

    // Append all new color spaces to this list here

    PXL_CS_MAX_SPACES
};

PXL_API const char *   PXLgetColorSpaceName(PXL_ColorSpace cs);
PXL_API PXL_ColorSpace PXLgetColorSpace(const char *csname);


#define PXL_WHITE_8		UCHAR_MAX
#define PXL_WHITE_16		USHRT_MAX
#define PXL_WHITE_32		UINT_MAX

PXL_API extern const int pxlDefinesDepth[6];
PXL_API extern const int pxlDefinesPComp[256];
PXL_API extern const int pxlDefinesPDepth[256];
PXL_API extern const unsigned int pxlDefinesWhite[6];

inline UT_GPUType PXLconvertToUTGPUType(PXL_DataFormat t)
{
    switch(t)
    {
    default:
    case PXL_INT8:	return UT_GPU_UINT8;
    case PXL_INT16:	return UT_GPU_UINT16;
    case PXL_INT32:	return UT_GPU_UINT32;
    case PXL_FLOAT32:	return UT_GPU_FLOAT32;
    case PXL_FLOAT16:	return UT_GPU_FLOAT16;
    }

    UT_ASSERT(!"Unknown type");
    return UT_GPU_UINT8;
}

inline int PXLformatDepth(PXL_DataFormat d)
	{ return pxlDefinesDepth[(int) d]; }

inline int PXLpackingComponents(PXL_Packing p)
	{ return pxlDefinesPComp[(int) p]; }
    
inline int PXLpackingDepth(PXL_Packing p)
	{ return pxlDefinesPDepth[(int) p]; }

inline unsigned int PXLwhitePoint(PXL_DataFormat dt)
	{ return pxlDefinesWhite[ (int) dt]; }


// must be called before the following 3 CPU flag checks.
PXL_API void		PXLdetermineProcCaps();
PXL_API bool		PXLcpuHasMMX();
PXL_API int		PXLcpuHasSSE();
PXL_API bool		PXLcpuHasXMMX();
PXL_API bool		PXLcpuHasCondMov();

inline int PXLclampChar(int t)
{
    if(t < 0)
	t = 0;
    else if(t > 255)
	t = 255;

    return t;
}

inline int PXLclampShort(int t)
{
    if(t < 0)
	t = 0;
    else if(t > 65535)
	t = 65535;
    return t;
}


#endif
