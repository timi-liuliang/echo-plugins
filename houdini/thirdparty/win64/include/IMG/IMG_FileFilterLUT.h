/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_FileFilterLUT ( IMG Library, C++)
 *
 * COMMENTS:
 *
 *    Applies a LUT and/or gamma to the scanlines.
 */
#ifndef IMG_FILE_FILTER_LUT_H
#define IMG_FILE_FILTER_LUT_H

#include <SYS/SYS_Types.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_ValArray.h>
#include <PXL/PXL_Fill.h>

#include "IMG_FileFilter.h"

class PXL_Raster;
class PXL_Lookup;

class IMG_API IMG_FileFilterLUT : public IMG_FileFilter
{
public:
    IMG_FileFilterLUT(IMG_File *file_to_filter);
    virtual ~IMG_FileFilterLUT();

    virtual const char  *className() const { return "IMG_FileFilterLUT"; }

    void     init(const char *lutname, const char *lut_scope,
		  fpreal gamma, const char *gamma_scope,
		  bool is_ocio, const char *ocio_colorspace,
		  const char *ocio_looks);

    virtual const void	*readPlaneScanline(int y, const IMG_Plane &pi);
    
    virtual int	 writePlaneScanline(const void *data, int y,
				    const IMG_Plane &pi);

    virtual void computeCommonData();

private:
    void	 applyGammaLUT(void *dest, const void *sdata,
			       const IMG_Plane &pi, bool dogamma, bool dolut);
    void	 applyOCIO(void *dest, const void *sdata,
			   const IMG_Plane &pi,
			   const UT_StringHolder &src_colorspace,
			   const UT_StringHolder &dest_colorspace,
			   const UT_StringHolder &looks);
    bool	 myIsOCIO;
    PXL_Lookup  *myLookup;
    UT_StringHolder myLUTScopeString;
    UT_StringHolder myOCIOColorspaceA; // A,B's meaning depends on read/write.
    UT_StringHolder myOCIOColorspaceB;
    UT_StringHolder myOCIOLooks;
    UT_BitArray  myLUTScope;
    
    fpreal	 myGamma;
    UT_StringHolder myGammaScopeString;
    UT_BitArray  myGammaScope;
    
    UT_ValArray<PXL_Raster *> myScanRaster;

    // We cache this so we can share the gamma table between
    // invocations.
    PXL_FillParms	myFill;
};


#endif
