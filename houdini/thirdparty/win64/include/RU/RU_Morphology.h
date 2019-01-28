/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_Morphology.h
 *
 * COMMENTS:
 * 	Does a morphological erode/dilate of an image.
 */
#ifndef RU_MORPHOLOGY_H
#define RU_MORPHOLOGY_H

#include <SYS/SYS_Types.h>

#include "RU_API.h"

class RU_API RU_Morphology
{
public:
    // The dilate, erode, and dilateErode methods all have identical 
    // signatures.  The source, dest, and temp_dest are all the same
    // size, namely w x h.  The result will be written to dest, while
    // temp_dest is basically just a temporary work buffer.  It is
    // only needed if both of the sizes are greater than 1.  Due to the
    // nature of the algorithm employed, the first size/2, and up to
    // the last 3*size/2 pixels are not written to.  The region of 
    // interest should therefore be embedded in a larger region to 
    // ensure that all data of interest is written to.
    //
    // xsize and ysize must be odd, and in the case of dilate and erode,
    // positive.  When calling dilateErode, it is the sign of the two
    // sizes which determines whether a dilate or erode occurs.  Positive
    // values are dilates, negative are erodes.
    static void	dilate(const unsigned char *source, unsigned char *dest,
		       unsigned char *temp_dest,
		       int w, int h, int xsize, int ysize);
    static void	dilate(const unsigned short *source, unsigned short *dest,
		       unsigned short *temp_dest,
		       int w, int h, int xsize, int ysize);
    static void	dilate(const unsigned int *source, unsigned int *dest,
		       unsigned int *temp_dest,
		       int w, int h, int xsize, int ysize);
    static void	dilate(const fpreal16 *source, fpreal16 *dest,
		       fpreal16 *temp_dest,
		       int w, int h, int xsize, int ysize);
    static void	dilate(const float *source, float *dest,
		       float *temp_dest,
		       int w, int h, int xsize, int ysize);

    static void	erode (const unsigned char *source, unsigned char *dest,
		       unsigned char *temp_dest,
		       int w, int h, int xsize, int ysize);
    static void	erode (const unsigned short *source, unsigned short *dest,
		       unsigned short *temp_dest,
		       int w, int h, int xsize, int ysize);
    static void	erode (const unsigned int *source, unsigned int *dest,
		       unsigned int *temp_dest,
		       int w, int h, int xsize, int ysize);
    static void	erode (const fpreal16 *source, fpreal16 *dest,
		       fpreal16 *temp_dest,
		       int w, int h, int xsize, int ysize);
    static void	erode (const float *source, float *dest,
		       float *temp_dest,
		       int w, int h, int xsize, int ysize);

    static void	dilateErode(const unsigned char *source, unsigned char *dest,
		       unsigned char *temp_dest,
		       int w, int h, int xsize, int ysize);
    static void	dilateErode(const unsigned short *source, unsigned short *dest,
		       unsigned short *temp_dest,
		       int w, int h, int xsize, int ysize);
    static void	dilateErode(const unsigned int *source, unsigned int *dest,
		       unsigned int *temp_dest,
		       int w, int h, int xsize, int ysize);
    static void	dilateErode(const fpreal16 *source, fpreal16 *dest,
		       fpreal16 *temp_dest,
		       int w, int h, int xsize, int ysize);
    static void	dilateErode(const float *source, float *dest,
		       float *temp_dest,
		       int w, int h, int xsize, int ysize);

};

#endif
