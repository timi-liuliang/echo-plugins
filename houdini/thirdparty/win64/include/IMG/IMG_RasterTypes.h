/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_Raster.h ( IMG Library, C++)
 *
 * COMMENTS:
 *	IMG_Raster is the utility class for containing a 2D raster.
 *
 *	UT_DeepRaster contains 16 bit channels and is useful for maintaining
 *	extra color precision at low alpha values.  Composite operations are
 *	provided in the IMG_Raster class for compositing both IMG_Raster's and
 *	IMG_DeepRaster's.
 */

#ifndef __IMG_RasterTypes__
#define __IMG_RasterTypes__

#include <SYS/SYS_Types.h>

template<typename T> struct UT_RGBAT;
typedef UT_RGBAT<uint8> UT_RGBA; 
typedef UT_RGBAT<uint16> UT_BIG_RGBA; 

#define IMG_SMALL_IMAGE	100	// Default size for scaled loading

#define IMG_COMPOSITE_CLEAR	0x100

enum IMG_CompositeOp {
    IMG_COMPOSITE_OVER		=  0,
    IMG_COMPOSITE_UNDER		=  1,
    IMG_COMPOSITE_ATOP		=  2,
    IMG_COMPOSITE_A_INSIDE_B	=  3,
    IMG_COMPOSITE_B_INSIDE_A	=  4 | IMG_COMPOSITE_CLEAR,
    IMG_COMPOSITE_A_OUTSIDE_B	=  5,
    IMG_COMPOSITE_B_OUTSIDE_A	=  6 | IMG_COMPOSITE_CLEAR,
    IMG_COMPOSITE_PLUS		=  7,
    IMG_COMPOSITE_A_MINUS_B	=  8,
    IMG_COMPOSITE_B_MINUS_A	=  9 | IMG_COMPOSITE_CLEAR,
    IMG_COMPOSITE_MULTIPLY	= 10 | IMG_COMPOSITE_CLEAR,
    IMG_COMPOSITE_ERASE		= 11,
    IMG_COMPOSITE_WRITE		= 12,
    IMG_COMPOSITE_READ		= 13,
    IMG_COMPOSITE_CROSS		= 14,
    IMG_COMPOSITE_MERGE		= 15,
    IMG_COMPOSITE_DIFF		= 16,
    IMG_COMPOSITE_EMBOSS	= 17,
    IMG_COMPOSITE_MULTIPLY_NC	= 18,
    IMG_COMPOSITE_MIN		= 19,
    IMG_COMPOSITE_MAX		= 20
};

enum IMG_FilterOp {
    IMG_FILTER_NONE,
    IMG_FILTER_DISSOLVE,
    IMG_FILTER_GAMMA,
    IMG_FILTER_QUANTIZE,
    IMG_FILTER_MONO,
    IMG_FILTER_BLUR,
    IMG_FILTER_EVEN,
    IMG_FILTER_ODD,
    IMG_FILTER_CHANNEL,
    IMG_FILTER_SHARPEN,
    IMG_FILTER_EDGES,
    IMG_FILTER_CHROMAKEY,
    IMG_FILTER_BUMPMAP
};

enum IMG_ScaleOp {
    IMG_SCALE_NONE = 0,		// No scaling of raster
    IMG_SCALE_SIZE,		// Re-size given a specific filter
    IMG_SCALE_FIT		// scale IMG_Raster to loaded image.
};

enum IMG_Channel {
    IMG_CHANNEL_R,
    IMG_CHANNEL_G,
    IMG_CHANNEL_B,
    IMG_CHANNEL_A,
    IMG_CHANNEL_HUE,
    IMG_CHANNEL_SAT,
    IMG_CHANNEL_VAL,
    IMG_CHANNEL_LUM
};

//
//  You can or these together and pass as second argument to setEdgeChannel
//
enum IMG_EdgeOptions {
    IMG_EDGES_HARD	= 0x00,
    IMG_EDGES_SOFT	= 0x01,
    IMG_EDGES_COLOR	= 0x02,
    IMG_EDGES_LOWSIDE	= 0x04
};

typedef void	(*IMG_Compositebit_8_8)  (UT_RGBA     *dest, UT_RGBA     *op, int len );
typedef void	(*IMG_Compositebit_8_16) (UT_RGBA     *dest, UT_BIG_RGBA *op, int len );
typedef void	(*IMG_Compositebit_16_16)(UT_BIG_RGBA *dest, UT_BIG_RGBA *op, int len );
typedef void	(*IMG_Compositebit_16_8) (UT_BIG_RGBA *dest, UT_RGBA     *op, int len );

typedef void	(*IMG_UnaryOp) (UT_RGBA *pixel); // Remove this.

typedef void	(*IMG_UnaryOp16)(UT_BIG_RGBA *pixel, int length);
typedef void	(*IMG_UnaryOp8) (UT_RGBA     *pixel, int length);

//==========================================================================

#define     CHANNEL_BITS	8
#define BIG_CHANNEL_BITS	16
#define HUE_CHANNEL_BITS	15
#define SAT_CHANNEL_BITS	15
#define VAL_CHANNEL_BITS	16
#define ALP_CHANNEL_BITS	16		// HSVA alpha

#define     CHANNEL_MAX		0xFF
#define BIG_CHANNEL_MAX		0xFFFF
#define HUE_CHANNEL_MAX		0x7FFF
#define SAT_CHANNEL_MAX		0x7FFF
#define VAL_CHANNEL_MAX		0xFFFF
#define ALP_CHANNEL_MAX		0xFFFF		// HSVA alpha

#define     POINT_FIVE		0x80
#define BIG_POINT_FIVE		0x8000

#define CHANNEL_SHIFT		(BIG_CHANNEL_BITS - CHANNEL_BITS)

// HSV to SMALL_CHANNEL
#define HUE_SHIFT		(HUE_CHANNEL_BITS - CHANNEL_BITS)
#define SAT_SHIFT		(SAT_CHANNEL_BITS - CHANNEL_BITS)
#define VAL_SHIFT		(VAL_CHANNEL_BITS - CHANNEL_BITS)
#define HUE_POINT_FIVE		(1 << (HUE_SHIFT - 1))
#define SAT_POINT_FIVE		(1 << (SAT_SHIFT - 1))
#define VAL_POINT_FIVE		(1 << (VAL_SHIFT - 1))
#define HUE_TO_SMALL_CHANNEL(v)	(((v) + HUE_POINT_FIVE) >> HUE_SHIFT)
#define SAT_TO_SMALL_CHANNEL(v)	(((v) + SAT_POINT_FIVE) >> SAT_SHIFT)
#define VAL_TO_SMALL_CHANNEL(v)	(((v) + VAL_POINT_FIVE) >> VAL_SHIFT)

// HSV to BIG_CHANNEL
#define BIG_HUE_SHIFT		(BIG_CHANNEL_BITS - HUE_CHANNEL_BITS)
#define BIG_SAT_SHIFT		(BIG_CHANNEL_BITS - SAT_CHANNEL_BITS)
#define BIG_VAL_SHIFT		(BIG_CHANNEL_BITS - VAL_CHANNEL_BITS)
#define HUE_ROUND(v)		((v) >>((HUE_CHANNEL_BITS<<1)-BIG_CHANNEL_BITS))
#define SAT_ROUND(v)		((v) >>((SAT_CHANNEL_BITS<<1)-BIG_CHANNEL_BITS))
#define VAL_ROUND(v)		((v) >>((VAL_CHANNEL_BITS<<1)-BIG_CHANNEL_BITS))
#define HUE_TO_BIG_CHANNEL(v)	(((v) << BIG_HUE_SHIFT) | HUE_ROUND(v))
#define SAT_TO_BIG_CHANNEL(v)	(((v) << BIG_SAT_SHIFT) | SAT_ROUND(v))
#define VAL_TO_BIG_CHANNEL(v)	(((v) << BIG_VAL_SHIFT) | VAL_ROUND(v))

#define BIG_TO_SMALL_CHANNEL(v)	((v) >> CHANNEL_SHIFT)
#define SMALL_TO_BIG_CHANNEL(v)	((v) << CHANNEL_SHIFT | (v))

#define MONO_WEIGHTR	76	// .299
#define MONO_WEIGHTG	151	// .587
#define MONO_WEIGHTB	29	// .114

#define     NTSC_LUM(r, g, b)	((((r) * MONO_WEIGHTR) \
			        + ((g) * MONO_WEIGHTG) \
				+ ((b) * MONO_WEIGHTB)) >> 8)

#endif
