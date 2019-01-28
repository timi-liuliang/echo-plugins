/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_FileFilterFlip ( IMG Library, C++)
 *
 * COMMENTS:
 *
 *    Read/Write from a memory buffer.
 *    Supports vertical and horizontal flips, as well as flopping (90' rot). 
 */

#pragma once

#ifndef IMG_FILE_FILTER_FLIP_H
#define IMG_FILE_FILTER_FLIP_H

class PXL_Raster;

#include <UT/UT_BitArray.h>
#include <UT/UT_ValArray.h>

#include "IMG_FileFilter.h"

class IMG_API IMG_FileFilterFlip : public IMG_FileFilter
{
public:
	     IMG_FileFilterFlip(IMG_File *file_to_filter);
    virtual ~IMG_FileFilterFlip();

    virtual const char  *className() const { return "IMG_FileFilterFlip"; }
    
    void	 init(bool flipx, bool flipy, bool flop);

    virtual const void	*readPlaneScanline(int y, const IMG_Plane &pi);
    
    virtual int	 writePlaneScanline(const void *data, int y,
				    const IMG_Plane &pi);

    virtual int	 isTopFirst() const;
    virtual void computeCommonData();
    
    virtual int		 getDeepPixelSamples(int x, int y);
    virtual bool	 getDeepPixelPlaneData(int x, int y,
					const IMG_Plane &plane,
					float *data);
    virtual bool	 getDeepPixelData(int x, int y, float * const *data);
    virtual bool	 readDeepPixelData(int x, int y,
                	                    PXL_DeepSampleList &p);
    virtual bool	 writeDeepPixelData(int x, int y,
					const PXL_DeepSampleListPtr &p);

private:
    
    const void	*extractScanline(int y, const PXL_Raster *p);
    bool	 fillRasters();
    virtual void	 finishedScanline(int scan);

    void	 flipHorizontal(void *dest, const void *source,
				const IMG_Plane &pi);
    
    void	 computeXY(int &x, int &y);
    
    bool	 myXFlip;
    bool	 myYFlip;
    bool	 myFlop;
    bool	 myUseRasters;
    bool	 myRastersFilled;
    
    UT_ValArray<PXL_Raster *>	 myPlaneImage;
    void       *myRasterBuffer;
};

#endif
