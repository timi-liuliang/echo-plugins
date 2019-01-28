/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PolyRaster.h ( UT Library, C++)
 *
 * COMMENTS:	Rasterize a polygon.  The following class is a virtual class
 *		which will "rasterize" a polygon.  For each pixel that the
 *		polygon covers, a filter weight will be computed and a callback
 *		will be invoked to process the pixel.  This can be used to
 *		render polygons to an image raster or to sample texture etc.
 *
 *		Currently, only triangles are supported.
 *
 *		If a sample table is set, then the sample table will be used
 *		for computation, otherwise "perfect" filtering will be done.
 *		"perfect" filtering is more expensive than sample table
 *		evaluation.
 */

#ifndef __UT_PolyRaster__
#define __UT_PolyRaster__

#include "UT_API.h"
#include <SYS/SYS_Floor.h>
#include "UT_VectorTypes.h"
class UT_prEdge;
class UT_Filter;
class UT_FilterSampleTable;

class UT_API UT_PolyRaster {
public:
	     UT_PolyRaster();
    virtual ~UT_PolyRaster();

#if 0
    void	setSampleTable(UT_FilterSampleTable *table)
		{
		    mySampleTable = table;
		}
#endif
    void	setFilterWidth(fpreal x=1, fpreal y=1)
		{
		    myXSFilter = x * 0.5F;
		    myYSFilter = y * 0.5F;
		}
    void	setWindow(fpreal xl, fpreal xr, fpreal yb, fpreal yt)
		{
		    myXMin = xl; myXMax = xr;
		    myYMin = yb; myYMax = yt;
		}
    void	setResolution(int xsamp, int ysamp)
		{
		    myXres = xsamp;
		    myYres = ysamp;
		}

    // After all the variables are set, please call initialize.
    void	initialize();

    // The sample() method is a callback which will simply return the pixel
    // position and the weight.
    virtual void	sample(int x, int y, fpreal weight);

    // Rasterize the polygon and call the sample() method for each pixel
    // covered.  If the cliptowindow variable is 0, pixel values outside of the
    // resolution may be generated.
    void	rasterize(const float v0[2], const float v1[2],
			  const float v2[2], int cliptowindow = 1);

protected:
    float		 myXFilter, myYFilter;
    float		 myIXFilter, myIYFilter;
    float		 myXSFilter, myYSFilter;
    float		 myXMin, myXMax, myYMin, myYMax;
    int			 myXres, myYres;

private:
    void		 rasterize(UT_prEdge *head, int clipx) const;
    fpreal		 filterWeight(const float v0[2], const float v1[2],
					const float v2[2]) const;

    // This clipOneEdge will generate a polygon for the filter region
    int			 clipOneEdge(fpreal xl, fpreal xr, fpreal yb, fpreal yt,
				     float vtx[][2],
				     const UT_prEdge &edge) const;
    int			 genericClip(const float vtx[][2], int nvtx,
				     float result[][2],
				     const UT_prEdge &e0) const;
    fpreal		 genericWeight(float vtx[][2],
					int nvtx) const
			 {
			     if (nvtx < 3) return nvtx == -1;
			     return computeWeight(vtx, nvtx);
			 }
    int			 toScreenX(fpreal sx) const
			    { return (int)((sx-myXMin)*myIXSinc); }
    int			 floorScreenX(fpreal sx) const
			    { return (int)SYSfloor((sx-myXMin)*myIXSinc); }
    int			 ceilScreenX(fpreal sx) const
			    { return (int)SYSceil((sx-myXMin)*myIXSinc); }

    fpreal		 computeWeight(float vtx[][2], int nvtx) const;

    // clipOneEdge returns the filter weight for a single edge clip
    fpreal		 clipOneEdge(fpreal xl, fpreal xr, fpreal yb, fpreal yt,
				     const UT_prEdge &edge) const;
    float			 myXSinc, myIXSinc;
    float			 myYSinc, myIYSinc;
    float			 myBoxWeight;
#if 0
    UT_FilterSample		*mySampleTable;
#endif
};

#endif
