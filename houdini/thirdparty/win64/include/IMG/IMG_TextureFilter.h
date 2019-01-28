/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_TextureFilter.h ( IMG Library, C++)
 *
 * COMMENTS:	This class holds filter information required for texture
 *		evaluation.
 */

#ifndef __IMG_TextureFilter__
#define __IMG_TextureFilter__

#include "IMG_API.h"
#include <UT/UT_Vector4.h>
#include <UT/UT_String.h>
#include <UT/UT_FilterType.h>
#include <UT/UT_Filter.h>

class UT_Filter;
class UT_FilterWindow;

enum IMG_DeepInterp
{
    IMG_DEEP_DISCRETE = 0,	// Step interpolation of z samples
    IMG_DEEP_LINEAR = 1		// Linear interpolation of z samples
};

/// Modes for how to translate colors in images for texture lookups
enum IMG_ColorSpace
{
    IMG_COLORSPACE_AUTO,	// Determine automatically
    IMG_COLORSPACE_LINEAR	// Direct linear translation
};

/// Modes for filtering
class IMG_API IMG_TextureFilter
{
public:
    enum FilterMode
    {
	IMG_UTFILTER,	// Use UT_Filter for evaluation
	IMG_BILINEAR,		// Bilinear interpolation
	IMG_BIQUADRATIC,	// Biquadratic interpolation
	IMG_BICUBIC,		// Bicubic interpolation
    };

     IMG_TextureFilter();
     IMG_TextureFilter(const IMG_TextureFilter &rhs);
    ~IMG_TextureFilter();


    /// Get the channel name (i.e. the plane name)
    const char		*getChannelName() const	{ return myChannelName; }
    /// Get the channel index (i.e. the plane index).  The index is considered
    /// uninitialized if its value is < 0.
    int			 getChannelIndex() const { return myChannelIndex; }
    /// Set the channel name.  If this is different than the previous value,
    /// the channel index will be reset to -1.
    void		 setChannelName(const char *channel);
    /// Once the channel index has be resolved, stash the value here.
    void		 setChannelIndex(int i)	{ myChannelIndex = i; }

    void		 setFilter(UT_FilterType ftype,
				   float x, float y);
    void		 setXFilter(UT_FilterType ftype);
    void		 setYFilter(UT_FilterType ftype);
    void		 setFilterWidths(float x, float y);
    void		 setFilterBlurs(float x, float y);
    void		 setMipInterpolate(int onoff);
    void		 setWrap(UT_FilterWrap uwrap, UT_FilterWrap vwrap,
				 const UT_Vector4 &border);
    void		 setDeepInterp(IMG_DeepInterp interp)
			 { myDeepInterp = interp; }
    void		 setSourceColorSpace(IMG_ColorSpace convert)
			 { mySourceColorSpace = convert; }
    void		 setZSkew(float zskew) { myZSkew = zskew; }
    void		 setZWidth(float z) { myZWidth = z; }
    void		 swapBorder(UT_Vector4 &b, int ncomponents=4);

    FilterMode		 filterMode() const { return myFilterMode; }
    void		 setFilterMode(FilterMode m) { myFilterMode = m; }
    const UT_Filter	*getXFilter() const { return myXFilter; }
    const UT_Filter	*getYFilter() const { return myYFilter; }

    int			 getMipInterpolate() const { return myInterp; }
    float		 getXWidth() const	{ return myXWidth; }
    float		 getYWidth() const	{ return myYWidth; }
    float		 getXBlur() const	{ return myXBlur; }
    float		 getYBlur() const	{ return myYBlur; }

    UT_FilterWrap	 getXWrap() const	{ return myXWrap; }
    UT_FilterWrap	 getYWrap() const	{ return myYWrap; }
    const UT_Vector4	&getBorder() const	{ return myBorder; }

    float		 getXsupport() const { return myXFilter->getSupport(); }
    float		 getYsupport() const { return myYFilter->getSupport(); }
    float		 zskew() const { return myZSkew; }
    float		 zwidth() const	{ return myZWidth; }

    void		 setBeerLambert(bool b) { myBeerLambert = b; }
    bool		 beerLambert() const { return myBeerLambert; }

    void	dump() const;

public:
    UT_String		 myChannelName;
    UT_Filter		*myXFilter, *myYFilter;
    float		 myXWidth, myYWidth;
    UT_FilterWrap	 myXWrap, myYWrap;
    UT_Vector4		 myBorder;
    int			 myInterp;
    IMG_DeepInterp	 myDeepInterp;
    IMG_ColorSpace	 mySourceColorSpace;
    FilterMode		 myFilterMode;
    int			 myChannelIndex;
    float		 myXBlur, myYBlur;
    UT_Vector3		 myZEqn;
    float		 myZSkew;
    float		 myZWidth;
    bool		 myBeerLambert;
    bool		 myOwnFilter;
};

#endif
