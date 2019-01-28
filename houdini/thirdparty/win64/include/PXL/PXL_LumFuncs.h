/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_LumFuncs.h
 *
 * COMMENTS:
 * 	Utility functions used by multiple COPs.
 */
#ifndef PXL_LUM_FUNCS_H
#define PXL_LUM_FUNCS_H

#include <SYS/SYS_Types.h>
#include "PXL_API.h"
#include "PXL_Common.h"

enum PXL_LumFunction
{
    PXL_MONO_LUM=0,
    PXL_MONO_NTSC,
    PXL_MONO_MAX,
    PXL_MONO_MIN,
    PXL_MONO_AVERAGE,
    PXL_MONO_RED,
    PXL_MONO_GREEN,
    PXL_MONO_BLUE,
    PXL_MONO_COMP4
};

// This class only contains static methods.
class PXL_API PXL_LumFuncs
{
public:
    // Generic luma function.
    static void convertToLuma(PXL_DataFormat format, 
			      void *lum, PXL_LumFunction func,
			      const void *l1, const void *l2,
			      const void *l3, const void *l4, int size,
			      int sinc = 1);

    // Integer luma functions.
    static void convertToLuma(unsigned char *lum, PXL_LumFunction func,
			      const unsigned char *l1,
			      const unsigned char *l2,
			      const unsigned char *l3,
			      const unsigned char *l4,
			      int size, int sinc = 1);
    
    static void convertToLuma(unsigned short *lum, PXL_LumFunction func,
			      const unsigned short *l1,
			      const unsigned short *l2,
			      const unsigned short *l3,
			      const unsigned short *l4,
			      int size, int sinc = 1);
    
    static void convertToLuma(unsigned int *lum, PXL_LumFunction func,
			      const unsigned int *l1,
			      const unsigned int *l2,
			      const unsigned int *l3,
			      const unsigned int *l4,
			      int size, int sinc = 1);

    static void convertToLuma(fpreal16 *lum, PXL_LumFunction func,
			      const fpreal16 *l1,
			      const fpreal16 *l2,
			      const fpreal16 *l3,
			      const fpreal16 *l4,
			      int size, int sinc = 1);

    // Float luma function.
    static void convertToLuma(float *lum, PXL_LumFunction func,
			      const float *l1, const float *l2,
			      const float *l3, const float *l4, int size,
			      int sinc = 1, bool copy_if_possible = false);

};
#endif
