/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_FileFilterWindow ( IMG Library, C++)
 *
 * COMMENTS:
 *
 *    Implements data windowing abstraction for files.
 *
 */

#pragma once

#ifndef IMG_FILE_FILTER_WINDOW_H
#define IMG_FILE_FILTER_WINDOW_H

class PXL_FillParms;

#include <UT/UT_Rect.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>

#include "IMG_FileFilter.h"

class IMG_API IMG_FileFilterWindow : public IMG_FileFilter
{
public:
	     IMG_FileFilterWindow(IMG_File *file_to_filter);
    virtual ~IMG_FileFilterWindow();

    virtual const char  *className() const { return "IMG_FileFilterWindow"; }
    
    void	initWindow(const UT_DimRect &user_area);

    virtual const void	*readPlaneScanline(int y, const IMG_Plane &pi);
    
    virtual int	 writePlaneScanline(const void *data, int y,
				    const IMG_Plane &pi);

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
    
    void	 fillBG(void *buffer, const IMG_Plane &pi,
			PXL_FillParms *parms);
    void	 fillScan(void *buffer, const void *input,
			  const IMG_Plane &pi,
			  IMG_Stat &stat, IMG_Stat &istat);
    void	 clearScan(void *buffer, const IMG_Plane &pi);
    
    virtual void finishedScanline(int scan);

    bool	 computeXY(int &x, int &y);
    
    UT_DimRect	myInWindow;
    UT_DimRect  myOutWindow;
    bool	myDisjointFlag;
    fpreal	myBGColor[4];
    bool	myStreakFlag;
    
    UT_ValArray<PXL_FillParms *> myPlaneFill;
    UT_ValArray<PXL_FillParms *> myPlaneFillLeft;
    UT_ValArray<PXL_FillParms *> myPlaneFillRight;
    UT_IntArray			 myPlaneScanline;
};

#endif
