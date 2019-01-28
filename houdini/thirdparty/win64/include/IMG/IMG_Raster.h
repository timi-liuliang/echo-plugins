/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_Raster.h ( IMG Library, C++)
 *
 * COMMENTS:
 *	IMG_Raster is the utility class for containing a 2D raster.
 *
 *	UT_DeepRaster contains 16 bit channels and is useful for maintaining
 *	extra color precision at low alpha values.  Composite operations are
 *	provided in the IMG_Raster class for compositing both IMG_Raster's and
 *	IMG_DeepRaster's.
 */

#ifndef __IMG_Raster__
#define __IMG_Raster__

#include "IMG_API.h"
#include "IMG_RasterTypes.h"
#include <UT/UT_FilterType.h>
#include <UT/UT_Raster.h>
#include <iosfwd>

#define DEEP_OPS

class UT_Color;
class UT_IStream;
class UT_ThreadSet;
class IMG_RasterHSVA;
class img_ioState;
class img_lineptrs;

class IMG_API IMG_EMBOSS_PARAMS {
public:
    float        lightPos[3];		// Light position (should be +Z)
    float        lightColot[3];		// Light color (currently not used)

    float        ambient;		// Ambient contribution
    float        diffuse;		// Diffuse coefficient
    float        specular;		// Specular coefficient
    float        shiny;			// Phong coefficient

    float        bumpScale;		// Scale of embossing
};

//==========================================================================

class IMG_API IMG_Raster : public UT_Raster
{
public:
	     IMG_Raster();
	     IMG_Raster(short xres, short yres, UT_RasterDepth depth);
	     IMG_Raster(short xres, short yres, UT_RGBA *image = 0);
	     IMG_Raster(short xres, short yres, UT_BIG_RGBA *image);
	     IMG_Raster(const IMG_Raster &r);
    virtual ~IMG_Raster();

    void		 setAlpha(float alpha);
    void		 setCompositeOp(IMG_CompositeOp op);
    IMG_CompositeOp	 getCompositeOp() const { return compositeOp; }

    void		 compositePixel(UT_RGBA *dest, UT_RGBA *op)
			 { (*compositebit_8_8)(dest, op, 1); }

    void		 compositePixel(UT_RGBA *dest, UT_BIG_RGBA *op)
			 { (*compositebit_8_16)(dest, op, 1); }

    void		 compositePixel(UT_BIG_RGBA *dest, UT_BIG_RGBA *op)
			 { (*compositebit_16_16)(dest, op, 1); }

    void		 compositePixel(UT_BIG_RGBA *dest, UT_RGBA *op)
			 { (*compositebit_16_8)(dest, op, 1); }

    void		 composite(IMG_Raster    *r, short x = 0, short y = 0);

    void		 filter(IMG_FilterOp op,
				float arg = 0.5,
				short x = 0,
				short y = 0,
				short w = 0,		// Implies clip_w
				short h = 0);		// Implies clip_h
    // Apply a filter in-place...
    void		 convolveFilter(UT_FilterType xfilter, float xwidth,
					UT_FilterType yfilter, float ywidth);
//
//  Calling crop actually changes the size of the raster to the cropped size.
//
    void		 size(short w, short h, void *newRaster = 0);
    void		 crop(short x, short y, short w, short h);
    void		 rgbaScale(float rrange[2],
				   float grange[2],
				   float brange[2],
                                   float arange[2],
				   short x=0, short y=0, short w=0, short h=0);

//
//  Currently the channel mask only applies to blur operations.
//  Non zero arguments enable the corresponding channel.
//
    void		 setChannelMask(int r, int g, int b, int a);
    void		 setEdgeChannel(IMG_Channel chan, unsigned options = 0);
    void		 setChromaKey(const UT_HSVA c1,
				      const UT_HSVA c2,
				      const UT_HSVA delta,
				      int alpha);
    void		 setEmbossParams(const IMG_EMBOSS_PARAMS &bp);
    void		 getEmbossParams(IMG_EMBOSS_PARAMS *bp) const;

    virtual UT_RGBA	*getPixel(short x, short y) const;
    virtual UT_BIG_RGBA	*getDeepPixel(short x, short y) const;
    virtual void	*getVoidPixel(short x, short y) const;

			 // Scale the raster
    void		 resize(short w, short h,
				UT_FilterType filter = UT_FILTER_POINT);
    void		 resizeCopy(IMG_Raster *src, short w, short h,
				    UT_FilterType filt = UT_FILTER_POINT,
				    float filter_width = 0.0F)
			 {
			     resizeCopy(src, w, h, filt, filter_width,
						   filt, filter_width);
			 }
    void		 resizeCopy(IMG_Raster *src, short w, short h,
				    UT_FilterType xfilter, float xwidth,
				    UT_FilterType yfilter, float ywidth);

    void		 resizeCropCopy(IMG_Raster *src, short sl, short sb,
					short sw, short sh, short w, short h);

    int			 save(const char *pathname, bool quiet = false);
    int			 save(const char *pathname, int fullHeight, void *tag,
			       int (*more)(IMG_Raster *r,void *,int,int),
			       unsigned margin = 0);
    int			 save(std::ostream &os, int binary = 1) const;

    void		*openSave(const char *pathname, int fullHeight);
    void		*saveMore(void *handle);
    void		 abortSave(void *handle);

    int			 load(const char *pathname,
			      IMG_ScaleOp   scaled = IMG_SCALE_NONE,
			      UT_FilterType filter = UT_FILTER_POINT,
			      short	    size    = 100,
			      bool 	    watermarked = false,
			      bool	    topremult = true);
    int			 load(const char *pathname,
			      short	  xres,
			      short	  yres,
			      IMG_ScaleOp   scaled = IMG_SCALE_SIZE,
			      UT_FilterType filter = UT_FILTER_BOX,
			      short	  yJump	 = 0,
			      short	  yThrow = 0,
			      short	  xJump  = 0,
			      short	  xThrow = 0,
			      short	  yHave  = 0);
    bool		 load(UT_IStream &is);

    int			 stat(const char *pathname,
			      short *xres,
			      short *yres);

    void  setClipOffset(short x, short y);
    void  setClipSize  (short w, short h);
    short getClipX() const { return clip_x; }
    short getClipY() const { return clip_y; }
    short getClipW() const { return clip_w; }
    short getClipH() const { return clip_h; }
    void  getClippedOp(short *x, short *y, short *w, short *h) const;

    static void  mult_8_8(  UT_RGBA *pixel, unsigned int alpha,
			    unsigned int *a, unsigned int *b,
			    unsigned int *g, unsigned int *r);
    static void	 mult_8_16( UT_RGBA *pixel, unsigned int alpha,
			    unsigned int *a, unsigned int *b,
			    unsigned int *g, unsigned int *r,
			    unsigned int point_five);
    static void	 mult_16_8( UT_BIG_RGBA *pixel, unsigned int alpha,
			    unsigned int *a, unsigned int *b,
			    unsigned int *g, unsigned int *r,
			    unsigned int point_five);
    static void	 mult_16_16(UT_BIG_RGBA *pixel, unsigned int alpha,
			    unsigned int *a, unsigned int *b,
			    unsigned int *g, unsigned int *r);

    void	operator=(const IMG_Raster &r);
    void	operator=(const IMG_RasterHSVA &r);

protected:
    void		    blurOp_8 (float weight = 0.5);
    void		    blurOp_16(float weight = 0.5);
    void		    bumpOp_8 (float weight = 0.5);
    void		    bumpOp_16(float weight = 0.5);
    void		    edgeOp_8 (float weight = 0.5);
    void		    edgeOp_16(float weight = 0.5);
    void		 sharpenOp_8 (float weight = 0.5);
    void		 sharpenOp_16(float weight = 0.5);

    static unsigned short	 ut_chanR_8(UT_RGBA *p);
    static unsigned short 	 ut_chanG_8(UT_RGBA *p);
    static unsigned short 	 ut_chanB_8(UT_RGBA *p);
    static unsigned short 	 ut_chanA_8(UT_RGBA *p);
    static unsigned short 	 ut_chanL_8(UT_RGBA *p);
    static unsigned short 	 ut_chanH_8(UT_RGBA *p);
    static unsigned short 	 ut_chanS_8(UT_RGBA *p);
    static unsigned short 	 ut_chanV_8(UT_RGBA *p);

    static unsigned short	 ut_chanR_16(UT_BIG_RGBA *p);
    static unsigned short 	 ut_chanG_16(UT_BIG_RGBA *p);
    static unsigned short 	 ut_chanB_16(UT_BIG_RGBA *p);
    static unsigned short 	 ut_chanA_16(UT_BIG_RGBA *p);
    static unsigned short 	 ut_chanL_16(UT_BIG_RGBA *p);
    static unsigned short 	 ut_chanH_16(UT_BIG_RGBA *p);
    static unsigned short 	 ut_chanS_16(UT_BIG_RGBA *p);
    static unsigned short 	 ut_chanV_16(UT_BIG_RGBA *p);

    void	composite_8_8   (IMG_Raster *, short x, short y);
    void	composite_8_16  (IMG_Raster *, short x, short y);
    void	composite_16_8  (IMG_Raster *, short x, short y);
    void	composite_16_16 (IMG_Raster *, short x, short y);

    static	void	*compositeThreadMgr( void *data );

    void	setCompositeOp_8_8  (IMG_CompositeOp op);
    void	setCompositeOp_8_16 (IMG_CompositeOp op);
    void	setCompositeOp_16_8 (IMG_CompositeOp op);
    void	setCompositeOp_16_16(IMG_CompositeOp op);

    IMG_Compositebit_8_8	compositebit_8_8;
    IMG_Compositebit_8_16	compositebit_8_16;
    IMG_Compositebit_16_16	compositebit_16_16;
    IMG_Compositebit_16_8	compositebit_16_8;

    IMG_CompositeOp		compositeOp;

    short		 channelMask;
    short		 clip_x;	// Active raster area
    short		 clip_y;
    short		 clip_w;
    short		 clip_h;
    short		 op_x;		// Clip region of most recent composite
    short		 op_y;
    short		 op_w;
    short		 op_h;

    img_ioState		*io;

public:
    // Leave these two typedefs here if you want this program to compile
    // under C++ >= 3.0.1. (Cristin)
    typedef void	(*img_3SumFunc_8)(
			    int		 sums[],
			    UT_RGBA	*above,
			    UT_RGBA	*center,
			    UT_RGBA	*below);
    typedef void	(*img_3SumFunc_16)(
			    int		 sums[],
			    UT_BIG_RGBA	*above,
			    UT_BIG_RGBA	*center,
			    UT_BIG_RGBA	*below);

    typedef void 	(*img_3x3FilterFunc_8)(
			    UT_RGBA *out,
			    UT_RGBA *p, int r, int g, int b, int a);
    typedef void 	(*img_3x3FilterFunc_16)(
			    UT_BIG_RGBA *out,
			    UT_BIG_RGBA *p, int r, int g, int b, int a);

    typedef void	(*img_HpassFunc_8)(
			    img_3x3FilterFunc_8	 do_pixel,
			    img_3SumFunc_8	 do_sum,
			    UT_RGBA		*output,
			    int			 length,
			    UT_RGBA		*above,
			    UT_RGBA		*center,
			    UT_RGBA		*below);
    typedef void	(*img_HpassFunc_16)(
			    img_3x3FilterFunc_16	 do_pixel,
			    img_3SumFunc_16	 do_sum,
			    UT_BIG_RGBA		*output,
			    int			 length,
			    UT_BIG_RGBA		*above,
			    UT_BIG_RGBA		*center,
			    UT_BIG_RGBA		*below);
private:

    void		 apply3x3_8(img_HpassFunc_8	hpass,
				    img_3x3FilterFunc_8	doit,
				    img_3SumFunc_8	do_sum,
				    UT_RGBA		*r);
    void		 apply3x3_16(img_HpassFunc_16	hpass,
				     img_3x3FilterFunc_16	doit,
				     img_3SumFunc_16	do_sum,
				     UT_BIG_RGBA	*r);

    void		 initImg();

    static void		*doVerticalPass_8 ( void* );	// thread callable
    static void		*doVerticalPass_16( void* );	// thread callable
    static void		*doHorizontalPass_8 ( void* );	// thread callable
    static void		*doHorizontalPass_16( void* );	// thread callable
    void		 initBlurTables_8 (float weight);
    void		 initBlurTables_16(float weight);

    void		 clearOutsideOpArea_8();
    void		 clearOutsideOpArea_16();

    void		 rgbaScale_8( float rrange[2],
				      float grange[2],
				      float brange[2],
                                      float arange[2],
				      short x=0,short y=0,short w=0, short h=0);

    void		 rgbaScale_16(float rrange[2],
				      float grange[2],
				      float brange[2],
                                      float arange[2],
				      short x=0,short y=0,short w=0, short h=0);
    void		 copy(IMG_Raster *src, short w, short h,
				void *dst, UT_RasterDepth dst_depth,
				UT_FilterType filt)
			 {
			     copy(src, w, h, dst, dst_depth,
				  filt, 0.0F, filt, 0.0F);
			 }
    void		 copy(IMG_Raster *src, short w, short h,
				void *dst, UT_RasterDepth dst_depth,
				UT_FilterType xfilter, float xwidth,
				UT_FilterType yfilter, float ywidth);
    
    UT_ThreadSet	 *getThreadSet();
};

#endif
