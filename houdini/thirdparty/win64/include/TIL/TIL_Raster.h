/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_Raster.h	(Tile Image Library, C++)
 *
 * COMMENTS:
 *	Defines a more generic raster for texture maps & display. This class
 *	is designed to be a raster container, not a raster manipulation class. 
 */
#ifndef TIL_RASTER_H
#define TIL_RASTER_H

#include "TIL_API.h"
#include <PXL/PXL_Raster.h>
#include <UT/UT_VectorTypes.h>
#include <IMG/IMG_RasterFilter.h>

#include "TIL_Defines.h"

class IMG_Raster;
class UT_TokenString;

#define TIL_RASTER_COMPLETE	0
#define TIL_RASTER_INCOMPLETE	1
#define TIL_RASTER_CORRUPT     -1

class TIL_API TIL_Raster : public PXL_Raster
{
public:
		 TIL_Raster();
		 TIL_Raster(const TIL_Raster &);
		 TIL_Raster(const PXL_Raster &);
		 TIL_Raster(const IMG_Raster &, bool copy = true);
		 TIL_Raster(const TIL_Raster &,
			    exint x1, exint y1, exint x2, exint y2);
		 TIL_Raster(TIL_Packing p, TIL_DataFormat f,
			    exint xres = 0, exint yres = 0, int clear = 0,
			    int alloc = 1);
    virtual	~TIL_Raster();

    virtual int64	getMemoryUsage(bool inclusive) const;

    // takes ownership.
    void		  setID(UT_TokenString *token);
    const UT_TokenString *getID() const { return myID; }

    virtual void	copy(const PXL_Raster &src);

    /// copies only the raster properties (res, type, etc), not the data, nor
    /// does this method allocate memory for a raster.
    virtual void	copyProperties(const PXL_Raster &src);
    
    // deletes the image memory. If full = 1, everything is reset - xres/yres
    // to 0, packing and format to invalid.
    virtual void	reset(int full = 0);
    
    // Scale this raster to another output raster.
    // You can't do format conversions here.
    void		scaleFromRasterToSize(TIL_Raster *scaled,
					      exint xres,exint yres,
					UT_FilterType flt=UT_FILTER_BOX) const;
    
    void		scaleFromRasterToScale(TIL_Raster *scaled,
					       float xsc,float ysc,
					UT_FilterType flt=UT_FILTER_BOX) const;

    static void		scaleRasterToSize(PXL_Raster *scaled,
				    const PXL_Raster *source,
				    exint xres, exint yres,
				    UT_FilterType flt=UT_FILTER_BOX);
    static void		scaleRasterToScale(PXL_Raster *scaled,
				    const PXL_Raster *source,
				    float xsc, float ysc,
				    UT_FilterType flt=UT_FILTER_BOX);
				    


    // applyEmulation is like applyLUT but does not use a look up table and
    // instead performs the calculations on each pixel.
    // It is currently only implemented for FLOAT32 rasters, for anything else
    // you should use applyLUT.
    void		applyEmulation(const TIL_Raster *src,
				       float black, float white,
				       float scale, float shift,
				       float gamma, exint comp = -1);
    
    void		adjustGamma(float gamma,
				    bool red   = true, 
				    bool green = true, 
				    bool blue  = true, 
				    bool alpha = false);

    // Raster diff functions. Source rasters must be the same format, packing
    // and size as this raster.
    void		splitHRaster(const TIL_Raster *r1,
				     const TIL_Raster *r2, float fract);
    void		splitVRaster(const TIL_Raster *r1,
				     const TIL_Raster *r2, float fract);
    void		blendRaster(const TIL_Raster *r1,
				    const TIL_Raster *r2, float fract);
    void		diffRaster(const TIL_Raster *r1,const TIL_Raster *r2);
    void		subRaster(const TIL_Raster *r1,const TIL_Raster *r2);
    void		highlightRaster(const TIL_Raster *r1,
					const TIL_Raster *r2, float threshold);
    
    void		 setUserData(void *data) { myUserData = data; }
    void		*getUserData()		 { return myUserData; }

    // returns true if an edge > tolerance was found within distance pixels
    // of (x,y). The location of the edge is returned in (x,y). If dir and
    // pos are specified, points along the dir vector from pos are favoured.
    bool		 getClosestEdge(exint &x, exint &y, exint distance,
					float tolerance = 0.1f,
					UT_Vector2 *pos = 0,
					UT_Vector2 *dir = 0);
    
    // TIL_RASTER_COMPLETE  - raster is filled & okay
    // TIL_RASTER_INCOMPLETE- raster was user-interrupted during construction.
    // TIL_RASTER_CORRUPT   - an error occurred during construction.
    void		setIncomplete(int inc = 1);
    int			isIncomplete() const { return myIncompleteState; }
    
protected:
    virtual void	attemptRealloc(int64 size);
   
    virtual void	initialize();
    
private:
    // optional.
    UT_TokenString	*myID;
    void		*myUserData;
    int		 	 myIncompleteState;
};

#endif
