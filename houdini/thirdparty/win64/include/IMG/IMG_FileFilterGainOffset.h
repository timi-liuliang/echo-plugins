/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_FileFilterGainOffset ( IMG Library, C++)
 *
 * COMMENTS:
 *
 *    Apply gain and offset to image in that order.
 */
#ifndef IMG_FILE_FILTER_GAIN_OFFSET_H
#define IMG_FILE_FILTER_GAIN_OFFSET_H

#include <SYS/SYS_Types.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_ValArray.h>
#include <PXL/PXL_Fill.h>

#include "IMG_FileFilter.h"

class PXL_Raster;
class PXL_Lookup;

class IMG_API IMG_FileFilterGainOffset : public IMG_FileFilter
{
public:
	     IMG_FileFilterGainOffset(IMG_File *file_to_filter);
    virtual ~IMG_FileFilterGainOffset();

    virtual const char  *className() const { return "IMG_FileFilterGainOffset"; }

    void     init(fpreal gain, fpreal offset);

    virtual const void	*readPlaneScanline(int y, const IMG_Plane &pi);
    
    virtual int	 writePlaneScanline(const void *data, int y,
				    const IMG_Plane &pi);

private:
    void applyGainOffset(void *dest, const void *sdata,
			 const IMG_Plane &pi);

    fpreal	 myGain;
    fpreal	 myOffset;
};


#endif
