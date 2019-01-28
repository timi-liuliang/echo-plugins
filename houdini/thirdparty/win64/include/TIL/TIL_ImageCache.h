/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_ImageCache.h (IMGUI library, C++)
 *
 * COMMENTS:
 *	Manages a cache of TIL_Raster images for viewing.
 */
#ifndef TIL_IMAGE_CACHE_H
#define TIL_IMAGE_CACHE_H

#include "TIL_API.h"
#include <iosfwd>
#include <UT/UT_Cache.h>
#include <UT/UT_FilterType.h>

#include "TIL_Defines.h"

class TIL_Image;
class TIL_ImageState;
class TIL_Look;
class TIL_ImageSource;
class TIL_Raster;

enum TIL_AlphaOption {
    TIL_NO_ALPHA,
    TIL_USE_ALPHA,
    TIL_SOLID_ALPHA
};

struct TIL_GetImageOptions
{
    TIL_GetImageOptions() :
	    myZoom(1), myIZoom(1),
	    myU1(0), myV1(0), myU2(1), myV2(1),
	    myScaled(false),
	    myAlpha(TIL_NO_ALPHA),
	    myOnlyIfCached(false),
	    myOverrideRes(false),
	    myOverX(-1), myOverY(-1),
	    myDataFormat(TILE_MAX_DATA_FORMAT),
	    myHalfFloatSupport(true),
	    myUseAspectRatio(true)
    {}

    void 		setCrop(float u1, float v1, float u2, float v2)
			{ myU1 = u1; myV1 = v1; myU2 = u2; myV2 = v2; }

    void		setResolution(exint resx, exint resy)
			{ myOverrideRes = true; myOverX = resx; myOverY = resy; }

    float 		myZoom;
    float 		myIZoom;
    float 		myU1, myV1;
    float 		myU2, myV2;
    bool 		myScaled;
    TIL_AlphaOption 	myAlpha;
    bool 		myOnlyIfCached;
    bool 		myOverrideRes;
    exint 		myOverX, myOverY;
    TIL_DataFormat 	myDataFormat;
    bool 		myHalfFloatSupport;
    bool 		myUseAspectRatio;
};

class TIL_API TIL_ImageCache : public UT_Cache
{
public:
			 TIL_ImageCache();
			~TIL_ImageCache();

    void		 setCacheLimit(bool enable);
    void		 setCacheSize(unsigned size_in_kb);

    int64		 getCurrentSize() const { return mySize; }
    int64		 getMaxSize() const { return myMaxSize; }

    static int64	 getSystemMaxSize();

    void		 addToCache(TIL_Image *image);
    void		 returnToCache(TIL_Image *&image);
    void		 returnToCache(const TIL_Raster *r);
    
    void		 removeSourceFromCache(TIL_ImageSource *source);
    void		 removeFrameFromCache(TIL_ImageSource *source, int fr);

    // the tokenstring, frame index and node path should be filled in.
    // returns 1 and fills in the raster if found (0 otherwise). 
    TIL_Image	*	 getImageFromCache(TIL_ImageState *match,
					   bool usedscaled = false,
					   UT_FilterType f = UT_FILTER_BOX,
					   bool bump = true);

    // Do not set force_all to true unless calling in a UT_Exit callback.
    void		 clearImageCache(bool force_all = false);

    int			 getImage(TIL_Image	   *&image,
				  TIL_ImageSource *source,
				  const char	    *plane,
				  exint		     planeindex,
				  float		     t,
				  void		    *look,
				  bool		     updateflag,
				  const TIL_GetImageOptions &opts =
						      TIL_GetImageOptions());

    static void		 setUpdateCallback(void (*callback)(void *,
							    TIL_Raster *,
							    int,int,
							    float,float));

    void		 dumpCache(std::ostream &os);
    
    virtual int64	utReduceCacheSizeBy(int64 amount);
protected:
    // the UT_Cache interface
    virtual const char *utGetCacheName() const { return "COP Flipbook Cache";}
    virtual int64	utGetCurrentSize() const { return getCurrentSize(); }
    virtual bool	utHasMaxSize() const { return true; }
    virtual int64	utGetMaxSize() const { return getMaxSize()*1024; }
    virtual void	utSetMaxSize(int64 size); 

private:
    int			 fetchImage(TIL_Image	   *&image,
				    TIL_ImageState *state,
				    float	    t,
				    void	   *look,
				    bool	    updateflag,
				    bool	    scaled,
				    bool	    include_alpha,
				    bool	    interactive,
				    exint		    fxres,
				    exint		    fyres,
				    bool	    onlyifcached,
				    bool	    use_software_emul,
				    bool	    halffloatsupport);
    
    void		 removeReference(TIL_Image *&image);
    bool		 removeImage(TIL_Image *image);
    
    void		 pruneCache();

    void		 pruneAnyObsoleteImages();
    void		 unlinkImage(TIL_Image *image);

    void		 lockCache(bool lock = true);
    
    TIL_Image		*myHead;
    TIL_Image		*myTail;
    int64		 mySize;
    int64		 myMaxSize;
    exint			 myNumImages;
    exint			 myNumObsolete;
    unsigned		 myCacheLimit : 1,
			 my8BitEmulOnly :1;
    TIL_FastLock	 myLock;
};

TIL_API TIL_ImageCache *TILgetCache();

#endif
