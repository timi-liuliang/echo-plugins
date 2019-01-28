/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_FileFilterScale ( IMG Library, C++)
 *
 * COMMENTS:
 *
 *    Scales a file from one res to another.
 *
 */
#ifndef IMG_FILE_FILTER_SCALE_H
#define IMG_FILE_FILTER_SCALE_H

class img_PlaneScale;

#include <UT/UT_ValArray.h>
#include <UT/UT_FilterType.h>

#include "IMG_FileFilter.h"

class IMG_API IMG_FileFilterScale : public IMG_FileFilter
{
public:
	     IMG_FileFilterScale(IMG_File *file_to_filter);
    virtual ~IMG_FileFilterScale();

    virtual const char  *className() const { return "IMG_FileFilterScale"; }
    
    void	initScale(int xres, int yres, UT_FilterType filter);

    virtual const void	*readPlaneScanline(int y, const IMG_Plane &pi);
    
    virtual int	 writePlaneScanline(const void *data, int y,
				    const IMG_Plane &pi);
private:
    bool	 grabScan(void *buffer, int y, int pi, int comp);
    virtual void finishedScanline(int scan);
    
    UT_ValArray<img_PlaneScale *> myPlaneScale;
    int mySupport;
    
    friend class img_PlaneScale;
};

#endif
