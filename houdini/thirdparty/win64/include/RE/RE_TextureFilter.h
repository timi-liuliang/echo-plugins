/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_TextureFilter.h ( RE Library, C++)
 *
 * COMMENTS:
 *	This class contains simple filter information used for textures.
 *	The GPU does the actual filtering.
 */
#ifndef RE_TextureFilter_h
#define RE_TextureFilter_h

#include "RE_API.h"
#include <UT/UT_Color.h>
#include "RE_TextureTypes.h"


class RE_API RE_TextureFilter
{
public:
    RE_TextureFilter();

    RE_TextureFilter   &operator=(const RE_TextureFilter &filter);
    bool		operator==(const RE_TextureFilter &filter) const;
    
    void		setMinFilter(RE_TexFiltType t );
    RE_TexFiltType	getMinFilter()	const;

    // Filter to use when displaying the texture at greater than 100% zoom
    void		setMagFilter(RE_TexFiltType t );
    RE_TexFiltType	getMagFilter()	const;

    // Anisotropic filtering. Acceptable values are pow2 up to the GL max.
    void		setMaxAnisotropy(int atf);
    int			getMaxAnisotropy() const;

    // Fixed bias value that is added to the level-of-detail parameter for
    // the texture before texture sampling (when mipmapping).
    void		setLodBias(float bias);
    float		getLodBias() const;

    // Texture wrapping - clamp or repeat (true). Not all dimension may be used.
    void		setTextureWrap(RE_TexClampType wrap_s,
				       RE_TexClampType wrap_t = RE_CLAMP_EDGE,
				       RE_TexClampType wrap_r = RE_CLAMP_EDGE);
    RE_TexClampType    	getWrapS() const;
    RE_TexClampType	getWrapT() const;
    RE_TexClampType	getWrapR() const;
    bool		isWrapSet() const;

    // For RE_CLAMP_BORDER, this defines the border color, normally (0,0,0,0).
    void		setBorder(RE_TextureBorder border,
				  const UT_Vector4F *color_alpha = NULL);
    const UT_Vector4F  &getBorderColor() const;

    // Sets all the filtering options to invalid values.
    void		invalidate();

private:
    RE_TexFiltType	myMinFilter;
    RE_TexFiltType	myMagFilter;
    int			myTextureAnisotropy;
    UT_Vector4F		myBorder;
    float		myLodBias;
    unsigned short	myWrapS:3, myWrapT:3, myWrapR:3;
};

inline RE_TextureFilter::RE_TextureFilter()
    : myMinFilter(RE_FILT_LINEAR),
      myMagFilter(RE_FILT_LINEAR),
      myWrapS(RE_CLAMP_REPEAT),
      myWrapT(RE_CLAMP_REPEAT),
      myWrapR(RE_CLAMP_REPEAT),
      myTextureAnisotropy(1),
      myBorder(0,0,0,0),
      myLodBias(0.0f)
{
}

inline void
RE_TextureFilter::setMinFilter(RE_TexFiltType t )
{
    myMinFilter = t;
}

inline RE_TexFiltType
RE_TextureFilter::getMinFilter() const
{
    return myMinFilter;
}

inline void
RE_TextureFilter::setMagFilter(RE_TexFiltType t )
{
    myMagFilter = t;
}

inline RE_TexFiltType
RE_TextureFilter::getMagFilter() const
{
    return myMagFilter;
}

inline void
RE_TextureFilter::setMaxAnisotropy(int ta )
{
    myTextureAnisotropy = ta;
}

inline int
RE_TextureFilter::getMaxAnisotropy() const
{
    return myTextureAnisotropy;
}

inline void
RE_TextureFilter::setLodBias(float bias)
{
    myLodBias = bias;
}

inline float
RE_TextureFilter::getLodBias() const
{
    return myLodBias;
}

inline void
RE_TextureFilter::setTextureWrap(RE_TexClampType s,
				 RE_TexClampType t,
				 RE_TexClampType r)
{
    myWrapS = s;
    myWrapT = t;
    myWrapR = r;
}

inline RE_TexClampType
RE_TextureFilter::getWrapS() const
{
    return RE_TexClampType(myWrapS);
}

inline RE_TexClampType
RE_TextureFilter::getWrapT() const
{
    return RE_TexClampType(myWrapT);
}

inline RE_TexClampType
RE_TextureFilter::getWrapR() const
{
    return RE_TexClampType(myWrapR);
}

inline RE_TextureFilter &
RE_TextureFilter::operator=(const RE_TextureFilter &filter)
{
    myWrapS = filter.myWrapS;
    myWrapT = filter.myWrapT;
    myWrapR = filter.myWrapR;
    myMinFilter = filter.myMinFilter;
    myMagFilter = filter.myMagFilter;
    myTextureAnisotropy = filter.myTextureAnisotropy;
    myBorder = filter.myBorder;

    return *this;
}

inline bool
RE_TextureFilter::operator==(const RE_TextureFilter &filter) const
{
    return (myWrapS == filter.myWrapS &&
	    myWrapT == filter.myWrapT &&
	    myWrapR == filter.myWrapR &&
	    myMinFilter == filter.myMinFilter &&
	    myMagFilter == filter.myMagFilter &&
	    myBorder == filter.myBorder &&
	    myTextureAnisotropy == filter.myTextureAnisotropy);
}

inline void
RE_TextureFilter::invalidate()
{
    myMinFilter = RE_FILT_MAX_FILTERS;
    myMagFilter = RE_FILT_MAX_FILTERS;
    myWrapS = RE_TEXTURE_BAD_WRAPPING;
    myWrapT = RE_TEXTURE_BAD_WRAPPING;
    myWrapR = RE_TEXTURE_BAD_WRAPPING;
    myBorder.assign(-1,-1,-1,-1);
    myTextureAnisotropy = 0;
}

inline bool
RE_TextureFilter::isWrapSet() const
{
    return (myWrapS != RE_TEXTURE_BAD_WRAPPING &&
	    myWrapT != RE_TEXTURE_BAD_WRAPPING &&
	    myWrapR != RE_TEXTURE_BAD_WRAPPING);
}	

inline void
RE_TextureFilter::setBorder(RE_TextureBorder border,
			    const UT_Vector4F *color)
{
    switch(border)
    {
    case RE_TEX_BORDER_ZERO:
	myBorder.assign(0,0,0,0); break;
    case RE_TEX_BORDER_BLACK:
	myBorder.assign(0,0,0,1); break;
    case RE_TEX_BORDER_WHITE:
	myBorder.assign(1,1,1,1); break;
    case RE_TEX_BORDER_COLOR:
	UT_ASSERT(color);
	myBorder = *color;
	break;
    }
}

inline const UT_Vector4F &
RE_TextureFilter::getBorderColor() const
{
    return myBorder;
}

#endif
