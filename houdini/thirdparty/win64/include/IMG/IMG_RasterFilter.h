/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_Filter.h ( IMG Library, C++)
 *
 * COMMENTS:	Code to apply a simple 2D symmetric separable filter to an
 *		image.  This code allows for re-sizing of the image as well as
 *		filtering to the same resolution.
 */

#ifndef __IMG_Filter__
#define __IMG_Filter__

#include "IMG_API.h"
#include <UT/UT_Filter.h>
#include "IMG_FileTypes.h"

// Private to this class
class img_FilterStorage;
class SPUT_ThreadSet;

#ifdef CELLBE
#include <SPU/SPU_FastBlur.h>
#endif

class IMG_API IMG_RasterFilter
{
public:
    typedef exint	(*fetchFunc)(void *tag, void *buffer, exint scanline);

    IMG_RasterFilter(
		exint inpXres,	exint inpYres,
		exint outXres,	exint outYres,
		fetchFunc	inpFetch,

		UT_FilterType	xfilter = UT_FILTER_BOX,
		float		xwidth = 1,

		UT_FilterType	yfilter = UT_FILTER_BOX,
		float		ywidth = 1,

		IMG_DataType	srcType = IMG_UCHAR,
		IMG_DataType	destType = IMG_UCHAR,

		// We do not do color model conversions in this code
		IMG_ColorModel	colorModel = IMG_RGBA);
    
    IMG_RasterFilter(
		exint inpXres,	exint inpYres,
		exint outXres,	exint outYres,
		fetchFunc	inpFetch,

		UT_Filter *	xfilter,
		float		xwidth,

		UT_Filter *	yfilter,
		float		ywidth,

		IMG_DataType	srcType = IMG_UCHAR,
		IMG_DataType	destType = IMG_UCHAR,

		// We do not do color model conversions in this code
		IMG_ColorModel	colorModel = IMG_RGBA);

    
    ~IMG_RasterFilter();

    void	restartFilter();
    exint		nextScanline(exint y, void *buf, void *tag);

    
    // This is the constructor for fast approximations of Box, Cone and Gauss
    // filters. You must use fastApproxBlur() for this version instead of
    // nextScanline(). restartFilter() is not needed before reusing the filter.
    //	- only single channel data is supported
    //  - different X/Y filter types are not supported (though sizes are)
    //  - data conversion is not supported
    IMG_RasterFilter( exint xres,		exint yres,
		      fetchFunc		inpFetch,
		      UT_FilterType	filter = UT_FILTER_BOX,
		      float		xwidth = 1.0f,
		      float		ywidth = 1.0f,
		      IMG_DataType	type = IMG_UCHAR);

    static void	     getApproxBlurBound(UT_FilterType type,
					float xsize,float ysize,
					exint ix1,exint iy1, exint ix2, exint iy2,
					exint &x1,exint &y1, exint &x2, exint &y2);

    // this method does a fast gaussian filter by using 4 successive box
    // filters (or a cone filter with 2, and an exact box filter). This
    // is most optimal for larger filter sizes. Small filter sizes have a
    // considerably more error (<10). 'buf' is a pointer to the entire image
    // (xres x yres, Format = type, from the constructor)
    int		fastApproxBlur(void *buf, void *tag);

    // These only apply to the CELLBE version of fast approx blur. They have
    // no effect on other platforms.
    bool	isRunning();
    void	close();

private:
    exint		 myInXres, myInYres;
    exint		 myOutXres, myOutYres;
    fetchFunc	 myFetch;

    exint		 myYWindow;		// Size of filter window in Y.
    float	 myYRadius;		// Filter radius as a float
    float	 myXScale, myYScale;

    IMG_DataType	myInType;
    IMG_DataType	myOutType;
    exint			myWPP;		// Channels per pixel (words)

    UT_Filter		*myYFilter;
    UT_FilterWindow	*myXWeight;
    UT_FilterWindow	 myYWeight;

    void	 *myInputBuffer;
    float	 *myFinal;
    float	 *myLoader;
    float	**myActive;
    exint		 *myLineBuf;

    // Extra stuff for the Fast Approximate blur.
    void	  filterVerticalFastApprox(exint x, float *tmp1, float *tmp2);
    
    bool	  myFastApprox;
    int		  myNumPasses;
    
    exint		  myXRadius;
    float	  myXRadiusWeight;
    float	  myXArea;
    exint		  myXOffset;
    
    exint		  myYIntRadius;
    float	  myYRadiusWeight;
    float	  myYArea;
    exint		  myYOffset;
};

#endif
