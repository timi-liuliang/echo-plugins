/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_PolyFill.h ( RU Library, C++)
 *
 * COMMENTS:	Polygon rasterization.  Polygons are scan converted and the
 *		anti-aliased intensity is put into the resulting plane/planes.
 *
 *		Ortho rendering is performed on a window (0,0)-(1,1) (meaning
 *		that polgons should have their coordinates set appropriately).
 *
 *	Rendering process:
 *		1)  Call initialize() and append() with a list of GU_Details to
 *		    render.  The GU_Details should be made of polygons, other
 *		    primitives will be ignored.
 *		2)  Call setResolution with the appropriate sizes
 *		3)  Call doSorting()
 *
 *	Then, for each tile, simply call scanConvert().  This function is
 *	thread safe.
 */

#ifndef __RU_PolyFill__
#define __RU_PolyFill__

#include "RU_API.h"
#include <UT/UT_Endian.h>
#include <UT/UT_Lock.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Vector4.h>

#include <TIL/TIL_Tile.h>

#include <TIL/TIL_Pixel.h>

#include "RU_Feather.h"		// for RU_FeatherDropoff

enum RU_PolyFillMethod
{
    RU_POLYFILL_NORMAL = 0,
    RU_POLYFILL_FEATHER
};

class GEO_Detail;

class ru_Triangle;
class ru_Bucket;

class ru_FeatherTriangle;
class UT_BilinearSampleQuad2D;
class UT_Ramp;

class RU_API RU_PolyFill
{
public:
     RU_PolyFill(RU_PolyFillMethod method	= RU_POLYFILL_NORMAL,
		 int		   num_threads	= 1);
    
    ~RU_PolyFill();

    void			 clearGeometry();
    void			 initialize(const GEO_Detail *gdp);
    void			 append(const GEO_Detail *gdp);
    void			 initializeFeather(const GEO_Detail *gdp);
    void			 addTriangle(float x0, float y0,
					     float x1, float y1,
					     float x2, float y2);
    void			 addFeatherTriangle(float x0, float y0,
						    float x1, float y1,
						    float x2, float y2,
						    int quad_index);

    // Set the resolution, where the image size is (imagex x imagey),
    // and the size of a tile is (tilex x tiley).
    void			 setResolution(int imagex, int imagey,
					       int tilex, int tiley,
					       float tolx = 0.001F);
    void			 doSorting();

    // Set super-sampling (8x8 by default). Normal mode only.
    void			 setSampling(int xs, int ys);

    // set the dropoff function. Feather mode only (0 @edge, 1 = size pix away)
    void			 setDropoff(int size,float (*dropfunc)(float));
    void			 setDropoff(RU_FeatherDropoff dropoff)
				 { myFeatherDropoff = dropoff; }
    void			 setDropoffRamp(UT_Ramp &ramp);
    
    // The tx, ty parameters refer to the tile to render.
    void			 scanConvert(TIL_TileList *tilelist,
					     int tx, int ty,
					     const UT_Vector4 &color,
					     int thread_index);

private:
    void			 scanConvertNormal(TIL_TileList *tilelist,
						   int tx, int ty,
						   const UT_Vector4 &color,
						   int thread_index);
    void			 scanConvertFeather(TIL_TileList *tilelist,
						   int tx, int ty,
						   const UT_Vector4 &color,
						   int thread_index);

   
    // The coordinates are relative to the tile origin
    void	 (RU_PolyFill::*myWritePixel)(void *data, int x, int y,
					      int stride,  float coverage,
					      int thread);

    void writePixel8(void *data, int x, int y,int stride, float coverage,
		     int thread);
    void writePixel8F(void *data, int x, int y,int stride, float coverage,
		      int thread);
    void writePixel16(void *data, int x, int y,int stride, float coverage,
		      int thread);
    void writePixel16F(void *data, int x, int y,int stride, float coverage,
		       int thread);
    void writePixel32(void *data, int x, int y,int stride, float coverage,
		      int thread);
    void writePixel32F(void *data, int x, int y,int stride, float coverage,
		       int thread);
    void writePixelFloat16(void *data, int x, int y,int stride, float coverage,
			 int thread);
    void writePixelFloat(void *data, int x, int y,int stride, float coverage,
			 int thread);

    void			 clearAndDestroy();
    
    // Common data
    RU_PolyFillMethod		 myFillMethod;
    int				 myNumThreads;
    ru_Triangle			*myTriangles;
    ru_Bucket			*myBuckets;
    ru_FeatherTriangle		*myFeatherTriangles;
    ru_Bucket			*myFeatherBuckets;
    int				 myBucketCount;
    float			*myPLeft, *myPBottom;
    float			 myXInc, myYInc;
    float			 myXTol;
    int				 myImageX, myImageY;
    int				 myXres, myYres;	// In Buckets
    int				 myTileX, myTileY;	// Tile sizes

    // Normal method data
    int				 mySampleSize;
    float			*mySamples;
    uint64			 myFullMask;
    uint64			**myMaskBuffer;
    float			 myFullCount;
    UT_Lock			 mySampleLock;

    // Feather method data
    float			**myFillBuffer;
    UT_BilinearSampleQuad2D	*myFeatherQuads;
    int				 myFeatherSize;
    RU_FeatherDropoff		 myFeatherDropoff;

    // Fill data
    int				 myFastConvert;
    TIL_DataFormat		 myFormat;
    TIL_Pixel<unsigned char,0>	 *mySlow8;
    TIL_Pixel<unsigned char,1>	 *myFast8;
    TIL_Pixel<unsigned short,0>	 *mySlow16;
    TIL_Pixel<unsigned short,1>	 *myFast16;
    TIL_Pixel<unsigned int,0>	 *mySlow32;
    TIL_Pixel<unsigned int,1>	 *myFast32;

public: // for callback func only
    UT_FloatArray		 myFeatherLUT;
};

#endif
