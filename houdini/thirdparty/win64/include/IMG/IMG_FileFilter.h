/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_FileFilterFlip ( IMG Library, C++)
 *
 * COMMENTS:
 *
 *     This is the base class for all filters that support IMG_File.
 *  A filter transforms the image data in some way during the load/save
 *  process. Some filters are read/write, while others only affect one.
 *  
 */

#pragma once

#ifndef IMG_FILE_FILTER_H
#define IMG_FILE_FILTER_H

#include "IMG_File.h"

class IMG_API IMG_FileFilter : public IMG_File
{
public:
	     IMG_FileFilter(IMG_File *file_to_filter);
    virtual ~IMG_FileFilter();

    // All filters are pass through. None of the image formats are.
    //
    virtual bool isPassThrough() const { return true; }
    virtual int  isTopFirst() const { return myFileToFilter->isTopFirst(); }
    virtual int  isLeftFirst() const { return myFileToFilter->isLeftFirst(); }

    virtual IMG_File *getBaseFile() const { return myFileToFilter; }
    
    virtual void	 randomReadAccessRequired();

    void    setFileToFilter(IMG_File *file) { myFileToFilter = file; }

    virtual void	 computeCommonData()
			{ IMG_File::computeCommonData(); }

    virtual int		 getDeepPixelSamples(int x, int y);
    virtual bool	 getDeepPixelPlaneData(int x, int y,
                	                       const IMG_Plane &plane,
                	                       float *data);
    virtual bool	 getDeepPixelData(int x, int y, float * const *data);
    virtual bool	 readDeepPixelData(int x, int y,
                	                    PXL_DeepSampleList &p);
    virtual bool	 writeDeepPixelData(int x, int y,
                	                    const PXL_DeepSampleListPtr &p);
    
 protected:
    // called when the user calls closeFile().
    //
    virtual int  finishFilter() { return 1; }

    // Don't override these methods; override the read/writePlaneScanline()
    // methods instead.
    //
    virtual int	 readScanline(int scanline, void *data);
    virtual int	 writeScanline(int scanline, const void *data);

    virtual void checkpoint();
    virtual int	 closeFile();

    // Utility functions for filters.
    const IMG_Stat &getInputStat() const { return myFileToFilter->getStat(); } 
    IMG_Stat	   &getInputStat()	 { return myFileToFilter->getStat(); } 
    
    const void * fetchScanline(int y, const IMG_Plane &pi);
    bool	 writeToScanline(int y, const void *data,
				 const IMG_Plane &pi);

    void	*fetchWriteScanline(int y, const IMG_Plane &pi);

    virtual void getAdditionalInfo(UT_String &text)
		 { myFileToFilter->getAdditionalInfo(text); }
	    
    /// @{
    /// Image texture options
    virtual UT_SharedPtr<UT_Options>	imageTextureOptions() const;
    virtual void	clearImageTextureOptions();
    virtual bool	setImageTextureOptions(const UT_Options &options);
    virtual void	setWriteTag(const char *tag, int n, const char *const *v);
    /// @}
private:
    IMG_File	*myFileToFilter;

};









#endif
