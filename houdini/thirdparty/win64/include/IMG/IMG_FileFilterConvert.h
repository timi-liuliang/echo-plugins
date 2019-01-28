/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_FileFilterConvert ( IMG Library, C++)
 *
 * COMMENTS:
 *
 *    Does data format conversion (int8, fp16, etc) and color model
 *    conversion (RGB, A, RGBA). 
 */
#ifndef IMG_FILE_FILTER_CONVERT_H
#define IMG_FILE_FILTER_CONVERT_H

class img_PlaneConvert;

#include <PXL/PXL_LumFuncs.h>

#include "IMG_FileFilter.h"

class IMG_API IMG_FileFilterConvert : public IMG_FileFilter
{
public:
	     IMG_FileFilterConvert(IMG_File *file_to_filter);
    virtual ~IMG_FileFilterConvert();

    virtual const char  *className() const { return "IMG_FileFilterConvert"; }
    
    void	initConversion(IMG_DataType type,
			       IMG_ColorModel model,
			       IMG_ComponentOrder order,
			       bool interleaved,
			       PXL_LumFunction func,
			       bool splitoutalpha,
			       const char *plane_name);
    
    virtual const void	*readPlaneScanline(int y, const IMG_Plane &pi);
    
    virtual int	 writePlaneScanline(const void *data, int y,
				    const IMG_Plane &pi);
private:
    void	convertPlane(int pindex,
			     const IMG_Stat &sstat,
			     bool sinterleave,
			     IMG_Stat &dstat,
			     bool dinterleave);
    
    void	convertData(void *dest, const void *source, int pindex, int y);

    void	splitAlphaFromColor(void *dest, const void *alpha, int pindex,
				    bool reversed);
    void	writeAlphaToColor(void *dest, const void *alpha, int pindex);
    
    bool	findAlphaPlane(const IMG_Stat &stat,int &aindex, int &acomp);

    void	initRead(IMG_DataType type,
			 IMG_ColorModel model,
			 IMG_ComponentOrder order,
			 bool interleaved,
			 PXL_LumFunction func,
			 bool splitoutalpha,
			 const char *plane_name);
    
    void	initWrite(IMG_DataType type,
			  IMG_ColorModel model,
			  IMG_ComponentOrder order,
			  bool interleaved,
			  PXL_LumFunction func,
			  bool splitoutalpha,
			  const char *plane_name);
    
    UT_ValArray<img_PlaneConvert *> myPlaneConvert;
    PXL_LumFunction myLumFunc;
    UT_IntArray	myAlphaCombineFlags;
};

#endif
