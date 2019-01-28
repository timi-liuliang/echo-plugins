/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_Composite.h
 *
 * COMMENTS:
 *	Does a composite between two images.
 */
#ifndef RU_COMPOSITE_H
#define RU_COMPOSITE_H

#include "RU_API.h"
enum RU_CompositeType
{
    RU_COMP_OVER	= 0,
    RU_COMP_UNDER	= 1,
    RU_COMP_ATOP	= 2,
    RU_COMP_INSIDE	= 3,
    RU_COMP_OUTSIDE	= 4,
    RU_COMP_SCREEN	= 5,
    RU_COMP_ADD		= 6,
    RU_COMP_SUBTRACT	= 7,
    RU_COMP_DIFF	= 8,
    RU_COMP_MULTIPLY	= 9,
    RU_COMP_MIN		= 10,
    RU_COMP_MAX		= 11,
    RU_COMP_AVERAGE	= 12,	
    RU_COMP_XOR		= 13,
    RU_COMP_SUBTRACT2	= 14
};


class RU_API RU_Composite
{
public:
    static void	composite(RU_CompositeType op, int xsize, int ysize,
			  float *bcolor,       const float *balpha,
			  const float *fcolor, const float *falpha);
    
    static void	composite(RU_CompositeType op, int xsize, int ysize,
			  unsigned char *bcolor, const unsigned char *balpha,
			  const unsigned char *fcolor,
			  const unsigned char *falpha);
    static void	composite(RU_CompositeType op, int xsize, int ysize,
			  unsigned char *bcolor, const unsigned short *balpha,
			  const unsigned char *fcolor,
			  const unsigned short *falpha);
    static void	composite(RU_CompositeType op, int xsize, int ysize,
			  unsigned char *bcolor, const float *balpha,
			  const unsigned char *fcolor,
			  const float *falpha);
    
    static void	composite(RU_CompositeType op, int xsize, int ysize,
			  unsigned short *bcolor, const unsigned short *balpha,
			  const unsigned short *fcolor,
			  const unsigned short *falpha);
    static void	composite(RU_CompositeType op, int xsize, int ysize,
			  unsigned short *bcolor, const unsigned char *balpha,
			  const unsigned short *fcolor,
			  const unsigned char *falpha);
    static void	composite(RU_CompositeType op, int xsize, int ysize,
			  unsigned short *bcolor, const float *balpha,
			  const unsigned short *fcolor,
			  const float *falpha);
    
    static void	composite(RU_CompositeType op, int xsize, int ysize,
			  float *bcolor, const unsigned short *balpha,
			  const float *fcolor,
			  const unsigned short *falpha);
    static void	composite(RU_CompositeType op, int xsize, int ysize,
			  float *bcolor, const unsigned char *balpha,
			  const float *fcolor,
			  const unsigned char *falpha);
    
    static void	compositeGeo(RU_CompositeType op, int xsize, int ysize,
			     float *bcolor,	  const float *balpha,
			     const float *fcolor, const float *falpha,
			     const float *fore[4]);
    
    static void	compositeAlpha(RU_CompositeType op, int xsize, int ysize,
			       float *balpha, const float *falpha);
    static void	compositeAlpha(RU_CompositeType op, int xsize, int ysize,
			       unsigned char *balpha,
			       const unsigned char *falpha);
    static void	compositeAlpha(RU_CompositeType op, int xsize, int ysize,
			       unsigned short *balpha,
			       const unsigned short *falpha);
};


#endif
