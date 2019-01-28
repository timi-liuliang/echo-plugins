/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library (C++)
 *
 * COMMENTS:	There are two classes defined here.  If you're using one,
 *		you'll use the other:
 *		   TIL_TextureMap   - a texture map.  It knows how to lookup
 *					it's raster for you.
 *		   TIL_TextureCache - The cache control specifies parameters
 *					used in the internal cache for
 *					texture maps
 *				     This class is simply a verb class which
 *					accesses an internal class.
 *
 *		The only way to get at the texture cache is through a
 *		TIL_TextureMap.  This allows the cache to keep a good
 *		record of references.  The rasters are loaded on demand
 *		which means that unless you look up a color, you don't
 *		load the raster.
 *
 */

#ifndef _TIL_TextureMap_h_
#define _TIL_TextureMap_h_

#include "TIL_API.h"
#include <UT/UT_Vector4.h>
#include <UT/UT_FilterType.h>
#include <UT/UT_String.h>
#include <UT/UT_Options.h>
#include <UT/UT_SharedPtr.h>
#include <IMG/IMG_FileTypes.h>
#include <IMG/IMG_TextureFilter.h>

class TIL_Raster;
class TIL_TextureMap;
class IMG_TTFilterInfo;
class IMG_File;
class IMG_FileTTMap;
class UT_Options;
class til_Map;			// Internal structure
class til_Lookup;		// Internal structure
class UT_Filter;
class UT_OpCaller;

//
//  An enum for bump map computation type
//

enum TIL_BUMP_COMPUTE_TYPE
{
	TIL_BUMP_PRECOMPUTE,
	TIL_BUMP_COMPUTE_R,
	TIL_BUMP_COMPUTE_G,
	TIL_BUMP_COMPUTE_B,
	TIL_BUMP_COMPUTE_A,
	TIL_BUMP_COMPUTE_LUM
};

enum TIL_PtexOrient
{
    TIL_PTEX_COMPLEMENT_S	= 0x01,	// Complement the S coordinate
    TIL_PTEX_COMPLEMENT_T	= 0x02,	// Complement the T coordinate
    TIL_PTEX_SWAP_ST		= 0x04,	// Swap S/T coordinates

    // Orientation matching http://ptex.us
    TIL_PTEX_NATIVE		= 0,

    // The following are based on experimental evidence and may not be correct
    // Mudbox has the t coordinate flipped and the ST coordinats swapped
    TIL_PTEX_MUDBOX		= TIL_PTEX_SWAP_ST | TIL_PTEX_COMPLEMENT_T,
};

class TIL_API TIL_MissingTexture
{
public:
    /// Callback for missing texture
    TIL_MissingTexture() {}
    virtual ~TIL_MissingTexture();

    /// Callback invoked when a texture can't be found.  It's possible @c
    /// filename may be a @c nullptr.
    virtual void	missingTexture(const char *filename) const {};

    /// Callback invoked when a texture is converted internally to a .rat
    virtual void	convertToRat(const char *filename) const {};
};

class TIL_API TIL_TextureMapStat {
public:
    const char		*myFilename;		// Filename for the raster
    int			 myXres, myYres;	// Resolution
    int			 myRampNodes;
    int			 myImageCount;		// Image count...
    int64		 myMemory;
    int			 myOwnership;		// If it's a COP, this will be 0
};

class TIL_API TIL_TextureOptions {
public:
    TIL_TextureOptions()
	: myOptions(NULL)
	, myShared()
    { }
    TIL_TextureOptions(const UT_Options *options)
	: myOptions(options)
	, myShared()
    { }
    TIL_TextureOptions(const UT_Options *options,
                       UT_SharedPtr<UT_Options> shared)
	: myOptions(options)
	, myShared(shared)
    { }
    TIL_TextureOptions(const TIL_TextureOptions &opts)
	: myOptions(opts.myOptions)
	, myShared(opts.myShared)
    { }
    ~TIL_TextureOptions();

    const TIL_TextureOptions	&operator=(const TIL_TextureOptions &src)
				{
				    if (&src != this)
				    {
					myShared = src.myShared;
					myOptions = src.myOptions;
				    }
				    return *this;
				}

    const UT_Options	*get() const	{ return myOptions; }

private:
     const UT_Options		*myOptions;
     UT_SharedPtr<UT_Options>	 myShared;
};

//
//  The texture cache control class
//
class TIL_API TIL_TextureCache
{
public:
//
//  Limit the raster size of a texture map to the size specified
    static int		getMaxWidth(void);
    static int		getMaxHeight(void);
    static void		setMaxSize(int w=256, int h=256);

//
//  Limit the maximum number of maps in core at once
    static int		getCacheSize(void);
    static void		setCacheSize(int size=100);

//  Limit the maps to max amount of memory (in Mb)
    static int64	getCacheMemSize();
    static void		setCacheMemSize(int64 sizeinmb);

//
//  Query methods for the cache.
    static void		getImageStat(UT_Array<TIL_TextureMapStat> &stats);

    // Simple method to get memory usage
    static int64	 getMemoryUsage();

    static void		getConversionStats(exint &num_conversions,
				exint &current_memory,
				exint &peak_memory);
    
//
// The version of the cache updates everytime something in it changes
// or is removed.  This way we can clear display lists, etc.
    static int		 getVersion();

//
//  Flush all maps out of core.
    static void	clearCache(int only_out_of_date=0);

//
//  Flush a map  by name.
    static void	clearCache(const char *mapname);

//
//  Set auto-flushing of the cache.  That is, as soon as the map is
//	dereferenced completely, flush it from the cache.  This can slow
//	some applications down.
    static int		getAutoFlush(void);
    static void		setAutoFlush(int onOff);

//
// Inform the cache of any deleted rasters that it may be referencing
    static void		rasterDied(TIL_Raster *raster);
    static void		imageDataDied(void *imagedata); // for COPs 1.

//
// If a texture map is not a .rat file, exit with an error on load.
    static void		setNonRATError(bool on);
    static bool		getNonRATError();
};

class til_PtexData;

class TIL_API TIL_TextureHandle {
public:
     TIL_TextureHandle()
	 : myLookup(0)
	 , myData(0)
	 , myFirst(true) {}
     TIL_TextureHandle(const TIL_TextureHandle &h);
    ~TIL_TextureHandle();

    const TIL_TextureHandle	&operator=(const TIL_TextureHandle &h);

    bool	isPtex() const	{ return myData; }

    bool	exchangeFirstEval()
		{
		    bool	p = myFirst;
		    myFirst = false;
		    return p;
		}

    const til_Lookup	*lookup() const		{ return myLookup; }
    const til_PtexData	*ptexData() const	{ return myData; }

private:
    TIL_TextureHandle(til_Lookup *lookup);
    TIL_TextureHandle(til_PtexData *d);

    til_Lookup		*myLookup;
    til_PtexData	*myData;
    bool		 myFirst;
    friend class	 TIL_TextureMap;
};

class TIL_API TIL_TextureMap
{
public:
	     TIL_TextureMap(const char *mapname = 0, const char *relativeTo = 0,
			    TIL_Raster *rp = 0);
    virtual ~TIL_TextureMap();

    /// Expand a file containing %(UDIM)d, %(U)d or %(V)d formatting characters
    /// The @c iu and @c iv parameters passed in are the zero-based coordinates
    /// (i.e. UDIM == (1000 + iu+1+ 10*iv) and U == iu+1).
    ///
    /// For example:  expandUDIM("%s/%s_%(UDIM)%d_%(U)02d_%(V)d.rat", 3, 7)
    /// will return "%s/%s_1074_04_8.rat"
    static UT_StringHolder	expandUDIM(const char *format, int iu, int iv);

    /// Set the callback for missing textures.  This returns the previous
    /// callback.
    static const TIL_MissingTexture	*setMissingTexture(
						const TIL_MissingTexture *m);

    void		 setOpCaller(UT_OpCaller *c)	{ myCaller = c; }
    void		 setMap(const char *name, const char *relativeTo,
				TIL_Raster *rp = 0, bool always_resolve = true);

    void		 clearRaster();
    void		 forceReload(void);
    const char		*getMap(void) const;
    const TIL_Raster	*getOGLRaster();
    TIL_TextureOptions	 getTextureOptions() const;
    void		 getResolution(int &x, int &y)
			 { getResolution(getLookupHandle(), x, y); }
    int			 getNChannels() const;

    // The color channel name may be NULL representing the default channel
    void		 setColorChannelName(const char *name);
    const char		*getColorChannelName() const;

    // Set the face index
    void		 setFaceIndex(int index);
    int			 getFaceIndex() const	{ return myFaceIndex; }
    // Set the channel offset (offset into the tuple data)
    void		 setPtexChannel(int index);
    int			 getPtexChannel() const { return myPtexChannel; }

    // ptexture orientation
    TIL_PtexOrient	 ptexOrient() const	{ return myPtexOrient; }
    void		 setPtexOrient(TIL_PtexOrient o) { myPtexOrient = o; }

    void		 setFilter(UT_FilterType filter, float xw, float yw);
    void		 setXFilter(UT_FilterType filter);
    void		 setYFilter(UT_FilterType filter);
    void		 setFilterWidths(float xwidth, float ywidth);
    void		 setFilterBlurs(float xblur, float yblur);
    void		 setMipInterpolate(int onoff);
    void		 setWrapMode(UT_FilterWrap uwrap, UT_FilterWrap vwrap,
					const UT_Vector4 &border);
    void		 setDeepInterp(IMG_DeepInterp interp);
    void		 setBeerLambert(bool beerlambert);
    void		 setSourceColorSpace(IMG_ColorSpace convert);
    void		 setFilterMode(IMG_TextureFilter::FilterMode m);
    void		 setZSkew(float zskew);


    const UT_Filter *getXFilter() const	{ return myFilter.getXFilter(); };
    const UT_Filter *getYFilter() const	{ return myFilter.getYFilter(); };
    float	getXFilterWidth() const { return myFilter.getXWidth(); }
    float	getYFilterWidth() const { return myFilter.getYWidth(); }

//
//  Functions used to set the color lookup behaviour of the map
    void		setColorLookup(bool interp = true,
				       bool needalpha = true)
			{
			    myInterpRequired = interp;
			    myAlphaRequired = needalpha;
			}
    void		setBumpLookup(bool interp = false,
				TIL_BUMP_COMPUTE_TYPE type=TIL_BUMP_PRECOMPUTE)
			{
			    myInterpRequired = interp;
			    myBumpType = type;
			}

    UT_FilterWrap	 getUWrap() const	 { return myFilter.getXWrap(); }
    UT_FilterWrap	 getVWrap() const	 { return myFilter.getYWrap(); }
    const UT_Vector4	&getBorder() const	 { return myFilter.getBorder(); }

    // If the texture failed to load, this method will tell us whether it
    // is using the default lookup functionality.
    bool		isDefault() const;
    int			isTTMap() const;
    int			isDSM() const;
    bool		isPtex() const;
    int			isIES() const;

//
//  Lookup a single sample of the image, interpreting the map as a color
//  When specifying four corners of the map, the order of the coordinates
//  is very important:
//
//      BAD                     GOOD                    GOOD
//      2--3                    3--2                    2--3
//       \/                     |  |                    |  |
//       /\                     |  |                    |  |
//      0--1                    0--1                    1--0

    /// When performing multiple texture evaluations, this allows the texture
    /// class to pre-compute some common values rather than performing the
    /// computation on every lookup.
    TIL_TextureHandle	getLookupHandle() const;

    /// @{
    /// Evaluate using an automatic lookup handle
    template <typename T>
    inline void		colorLookup(float u, float v, UT_Vector4T<T> &clr) const
			{
			    TIL_TextureHandle	h = getLookupHandle();
			    colorLookup(h, u, v, clr);
			}
    template <typename T>
    inline void		colorLookup(const float u[4], const float v[4],
				    UT_Vector4T<T> &clr) const
			{
			    TIL_TextureHandle	h = getLookupHandle();
			    colorLookup(h, u, v, clr);
			}
    fpreal		depthLookup(float u, float v,
				    float farvalue=1E6) const
			{
			    TIL_TextureHandle	h = getLookupHandle();
			    return depthLookup(h, u, v, farvalue);
			}
    template <typename T>
    void		bumpLookup(float u, float v,
				   T &du, T &dv) const
			{
			    TIL_TextureHandle	h = getLookupHandle();
			    bumpLookup(h, u, v, du, dv);
			}

    template <typename T>
    inline void		envLookup(const UT_Vector3T<T> &R, fpreal angle,
				  UT_Vector4T<T> &clr) const
			{
			    TIL_TextureHandle	h = getLookupHandle();
			    envLookup(h, R, angle, clr);
			}

    template <typename T>
    inline void		envLookup(const UT_Vector3T<T> &r00, const UT_Vector3T<T> &r10,
				  const UT_Vector3T<T> &r11, const UT_Vector3T<T> &r01,
				  UT_Vector4T<T> &clr) const
			{
			    TIL_TextureHandle	h = getLookupHandle();
			    envLookup(h, r00, r10, r11, r01, clr);
			}
    /// @}

    /// @{
    /// Evaluate using a lookup handle
    void		colorLookup(TIL_TextureHandle &h,
				    float u, float v, UT_Vector4F &clr) const;
    void		colorLookup(TIL_TextureHandle &h,
				    float u, float v, UT_Vector4D &clr) const;
    void		colorLookup(TIL_TextureHandle &h,
				    const float u[4], const float v[4],
				    UT_Vector4F &clr) const;
    void		colorLookup(TIL_TextureHandle &h,
				    const float u[4], const float v[4],
				    UT_Vector4D &clr) const;
    float		depthLookup(TIL_TextureHandle &h,
				    float u, float v,
				    float farvalue=1E6) const;
    void		bumpLookup(TIL_TextureHandle &h,
				   float u, float v,
				   fpreal32 &du, fpreal32 &dv) const;
    void		bumpLookup(TIL_TextureHandle &h,
				   float u, float v,
				   fpreal64 &du, fpreal64 &dv) const;

    void		envLookup(TIL_TextureHandle &h,
				  const UT_Vector3F &R, fpreal angle,
				  UT_Vector4F &clr) const;
    void		envLookup(TIL_TextureHandle &h,
				  const UT_Vector3D &R, fpreal angle,
				  UT_Vector4D &clr) const;

    void		envLookup(TIL_TextureHandle &h,
			      const UT_Vector3F &r00, const UT_Vector3F &r10,
			      const UT_Vector3F &r11, const UT_Vector3F &r01,
			      UT_Vector4 &clr) const;
    void		envLookup(TIL_TextureHandle &h,
			      const UT_Vector3D &r00, const UT_Vector3D &r10,
			      const UT_Vector3D &r11, const UT_Vector3D &r01,
			      UT_Vector4D &clr) const;
    void		getResolution(const TIL_TextureHandle &h,
				      int &x, int &y);
    /// @}

    /// @{
    /// Deep shadow lookup will fall back to regular depth map lookups
    /// if this map is not a deep shadow map.
    void		deepLookup(TIL_TextureHandle &h,
				   UT_Vector3F &clr, 
				   const UT_Vector3F &p,
				   const UT_Vector3F &du,
				   const UT_Vector3F &dv,
				   float quality, int maxsamples);
    void		deepLookup(TIL_TextureHandle &h,
				   UT_Vector3D &clr, 
				   const UT_Vector3D &p,
				   const UT_Vector3D &du,
				   const UT_Vector3D &dv,
				   float quality, int maxsamples);
    /// @}

    /// Query filter information
    const IMG_TextureFilter	&getFilter() const	{ return myFilter; }
    IMG_TextureFilter		&getFilter()		{ return myFilter; }

    // Adjust scales should only be called internally.
    TIL_BUMP_COMPUTE_TYPE	 getBumpCompute() const
				 {
				     return myBumpType;
				 }

    bool	 getInterpRequired() const	{ return myInterpRequired; }
    bool	 getAlphaRequired() const	{ return myAlphaRequired; }

    // Returns true if the texture is volatile (ie, sourced from a COP). It
    // needs to be checked every time it's accessed to se if it's been updated.
    bool	 isVolatileTexture() const;

    // Returns true if a volatile texture has changed, and optionally returns
    // the new raster in the newraster parm.
    bool	 hasTextureChanged(const TIL_Raster **newraster = 0);

    // Return the IMG File used (for TT maps). This may contain filters.
    IMG_File		*getFile();

    // Returns the unfiltered TT map.
    IMG_FileTTMap	*getTTMap();

    // Look up the color channel index
    int			 getColorChannelIndex(IMG_FileTTMap *map) const;

private:
    float		shadowLookup(const UT_Vector3 &p,
				     const UT_Vector3 &du,
				     const UT_Vector3 &dv,
				     float quality, int maxsamples) const;

    // This version of the shadow lookup is used when there are no 
    // derivatives available (for non-shading contexts)
    float		shadowLookup(const UT_Vector3 &Ps,
				     float du, float dv, 
				     float quality, int maxsamples) const;

    TIL_TextureHandle	getPtexHandle() const;
    
private:
    til_Map			*myMap;
    UT_OpCaller			*myCaller;
    IMG_TextureFilter		 myFilter;
    UT_String			 myColorChannel;

    int				 myPtexFilter;
    int				 myFaceIndex;
    int				 myPtexChannel;
    TIL_BUMP_COMPUTE_TYPE	 myBumpType;
    TIL_PtexOrient		 myPtexOrient;
    bool			 myInterpRequired;
    bool			 myAlphaRequired;

    friend class		 ut_Map;
};

#endif
