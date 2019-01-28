/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_FileFilterTile ( IMG Library, C++)
 *
 * COMMENTS:
 *
 *    Allows reading and writing files using a tile interface rather than
 *    scanlines.
 */
#ifndef IMG_FILE_FILTER_TILE_H
#define IMG_FILE_FILTER_TILE_H

#include <UT/UT_ValArray.h>
#include <UT/UT_VectorTypes.h>
#include "IMG_FileFilter.h"

class IMG_API IMG_FileFilterTile : public IMG_FileFilter
{
public:
	     IMG_FileFilterTile(IMG_File *file_to_filter);
    virtual ~IMG_FileFilterTile();

    virtual const char  *className() const { return "IMG_FileFilterTile"; }

    void	initTile(IMG_Interleave i);

    virtual bool readTile(const UT_InclusiveRect &rect,
			  void *data, const IMG_Plane *plane=0);
    
    virtual bool writeTile(const UT_InclusiveRect &rect,
			   const void *data,
			   const IMG_Plane *plane=0);
    
    virtual void	computeCommonData();
private:
    
    // These methods do not work with tiles. Do not call them.
    //
    virtual const void	*readPlaneScanline(int y, const IMG_Plane &pi);
    
    virtual int	 writePlaneScanline(const void *data, int y,
				    const IMG_Plane &pi);

    int myWidth;
    int myHeight;
    IMG_Interleave myInterleaved;
    UT_ValArray<UT_IntArray *> myProcessedPixels;
};





#endif
