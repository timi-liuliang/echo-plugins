/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Raster.h ( UT Library, C++)
 *
 * COMMENTS:
 *	UT_Raster is the utility class for containing a 2D raster.
 *	This is intended as a base class for IMG_Raster (see the IMG library)
 *	It's almost useless without it...
 */

#ifndef __UT_Raster__
#define __UT_Raster__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include "UT_Pixel.h"

class UT_API UT_Raster
{
public:
    enum UT_RasterDepth { UT_RASTER_8, UT_RASTER_16 };
    enum UT_ColorModel 	 { UT_MODEL_RGBA, UT_MODEL_ABGR };

	     UT_Raster();
    virtual ~UT_Raster();

    short Xres() const { return xres; }
    short Yres() const { return yres; }

    void		 clear(const UT_RGBA     &pixel);
    void		 clear(const UT_BIG_RGBA &pixel);
    void		 clear(const UT_Color &color, fpreal alpha = 1.0f);
    void		 clear(int x1, int y1, int x2, int y2,
			       const UT_RGBA &color);

    void		 clear(int x1, int y1, int x2, int y2,
			       const UT_BIG_RGBA &color);

    UT_RasterDepth	 getRasterDepth( void ) const { return( pixelDepth ); }
    void		 setRasterDepth( UT_RasterDepth depth );

    UT_ColorModel	 getColorModel(void)	const { return myModel; }
    void		 setColorModel( UT_ColorModel model );
    void		 byteSwapRaster();	// Swap bytes in the raster

    UT_RGBA		*getRaster() const;
    UT_BIG_RGBA		*getDeepRaster() const;
    void		*getVoidRaster() const;
    int			 getBytesPerPixel( void );

    void		 setNewRaster(UT_RGBA     *r);
    void		 setNewRaster(UT_BIG_RGBA *r);

    //  Phantom scanlines (bottom, top)
    //  No functional value (currently) but can be used to remember when the
    //  raster is really part of a larger raster

    void                 setPhantom( short b, short t)  { pBot = b; pTop = t; }
    short                getPbot( void )                { return( pBot); }
    short                getPtop( void )                { return( pTop ); }

    static UT_RGBA	 zeroPixel;
    static UT_RGBA	 onePixel;

protected:
    UT_BIG_RGBA		*raster16;
    UT_RGBA		*raster;
    UT_RGBA	       **scanlines;
    UT_RasterDepth	 pixelDepth;
    UT_ColorModel	 myModel;	// My color model

    void		 setBaseSize(short width, short height,
				     void *newRaster=0);

    void		*allocateMemory(int size);
    void		*reAllocateMemory(void *mem, int size);
    void		 freeMemory(void *mem);

    short		 xres;
    short		 yres;
    short                pBot;
    short                pTop;

    void		 init();
};

#endif
