/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_FilterArea.h ( RU Library, C++)
 *
 * USAGE:
 *	1) First, set the wrapping modes and the filter type and width.
 *	2) Then set the texture coordinates.  Internally, this will compute
 *	   the floating point bounds of the input region required.
 *	3) Next, query the filter to find out the extents of the input.
 *	   Build the input data needed, then
 *	4) Call the appropriate filter method to filter your arrays.
 *
 * COMMENTS:
 *	Filter's a set of "texture" coordinates of an input to the output
 *	region.  Note that for straight scaling of images, there are much
 *	better ways of filtering.  This is a generic filtering call which
 *	will be more expensive, but provide reasonable filtering of arbitrary
 *	regions.  This does assume that there is some continuity to the
 *	texture coordinates.  If the texture coordinates are random, this
 *	will still filter, but be very expensive.
 *
 *	This code assumes that the texture coordinates are in pixel space
 *	That is the 0 to XRes-1 and 0 to YRes-1.  The texture coordinates map
 *	to the center of pixels (not the edges).  All of this is actually
 *	very important information.
 *
 *	This code only works on floating point data.  It's not really an
 *	RU_Algorithm, but it can be used by RU_Algorithms if required.
 *
 * TODO:
 *	- We should be smart and pre-process the deltas on the texture
 *	  coordinates.  If the minimum delta is bigger than 2, we can do a
 *	  quick prefiltering (i.e. making a MIP) then use a smaller filter
 *	  width.
 */

#ifndef __RU_FilterArea__
#define __RU_FilterArea__

#include "RU_API.h"
#include <UT/UT_FilterType.h>
#include <UT/UT_VectorTypes.h>

class UT_Filter;

typedef enum {
    RU_FILTER_REPEAT = 0,
    RU_FILTER_CLAMP  = 1,
    RU_FILTER_BORDER = 2,
    RU_FILTER_MIRROR = 3
} RU_FILTER_WRAP;

class RU_API RU_FilterArea
{
public:
		 RU_FilterArea();
    virtual	~RU_FilterArea();

    // First, set up the filter parameters (use either setFilter func)
    void		 setFilter(UT_FilterType xfilter, float xsize,
				   UT_FilterType yfilter, float ysize);
    void		 setFilter(UT_Filter *xfilter, float xsize,
				   UT_Filter *yfilter, float ysize);
    void		 setWrap(int xwrap, int ywrap);
    void		 setBorder(float border[4]);


    // Second, set the texture coordinates (using 1 of the following)
    void		 setTextureCoords(const float *u, const float *v,
					  int rows, int cols,
					  int in_xres, int in_yres,
					  const char *mask = 0);
    void		 setFourCornerCoords(const float *u0, const float *v0,
					     const float *u1, const float *v1,
					     int rows, int cols,
					     int in_xres, int in_yres,
					     const char *mask = 0);

    // Third, get the extents of the input region required
    void		 getInputRegion(int &x0, int &x1, int &y0, int &y1);

    // Fourth, call the sample methods as many times as you like.  All of the
    // sample routines assume that the input is set to the input region
    // specified above.  The input region is inclusive (meaning there are
    // (x1-x0+1)*(y1-y0+1) pixels.
    void	 sample1(float *o,	const float *i);

    void	 sample3(UT_Vector3 *o, const float *i0, const float *i1,
					const float *i2);

    void	 sample4(UT_Vector4 *o, const float *i0, const float *i1,
					const float *i2, const float *i3);

    // Here are some methods to query the state of the filter
    RU_FILTER_WRAP	 getXWrap()	{ return myXWrap; }
    RU_FILTER_WRAP	 getYWrap()	{ return myYWrap; }
    UT_FilterType	 getXFilter()	{ return myXFilterType; }
    UT_FilterType	 getYFilter()	{ return myYFilterType; }
    float		 getXSize()	{ return myXSize; }
    float		 getYSize()	{ return myYSize; }

private:
    // This will return 1 if the filter is valid
    void		 setupWindow(float umin, float umax,
				     float vmin, float vmax,
				     int &xsize, int &ysize);
    // Parameters.
    UT_FilterType	 myXFilterType, myYFilterType;
    UT_Filter		*myXFilter, *myYFilter;
    float		 myXSize, myYSize;
    RU_FILTER_WRAP	 myXWrap, myYWrap;
    float		 myBorder[4];

    const float		*myUCoord;
    const float		*myVCoord;
    const char		*myMask;
    int			 myRows, myCols;
    int			 mySize;
    float		*myDU, *myDV;
    int			 myMaxSize;

    // Computed data
    float		 myXSupport, myYSupport;	// Real filter size
    float		 myMinU, myMaxU;
    float		 myMinV, myMaxV;
    int			 myInX0, myInX1;
    int			 myInY0, myInY1;
    int			 myInXRes, myInYRes;

    // Data used in filtering
    float		*myXWindow, *myYWindow;
    int			*myXPixels, *myYPixels;
    int			 myXWindowSize, myYWindowSize;
};

#endif
