/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_Raster.h	(Pixel Library, C++)
 *
 * COMMENTS:
 *	Defines a more generic raster for texture maps & display. 
 *	
 */
#ifndef PXL_RASTER_H
#define PXL_RASTER_H

#include <UT/UT_Rect.h>
#include "PXL_API.h"
#include "PXL_Common.h"
#include <UT/UT_SmallObject.h>
#include <UT/UT_StringHolder.h>
#include <SYS/fpreal16Limits.h>
#include <SYS/SYS_TypeTraits.h>
#include <iosfwd>
#include <limits>

class UT_IStream;

#define PXL_ROFF	16

enum PXL_StreakComponents {
    PXL_STREAK_ALL  = 0,
    PXL_STREAK_RGB,
    PXL_STREAK_A
};

class PXL_API PXL_Raster
    : public UT_SmallObject<PXL_Raster,
			    UT_SMALLOBJECT_CLEANPAGES_OFF, 250,
			    UT_SMALLOBJECT_THREADSAFE_ON>
{
public:
		 PXL_Raster();
		 PXL_Raster(const PXL_Raster &);
		 PXL_Raster(const PXL_Raster &,
			    exint x1, exint y1, exint x2, exint y2);
		 PXL_Raster(PXL_Packing p, PXL_DataFormat f,
			    exint xres = 0, exint yres = 0, int clear = 0,
			    int alloc = 1);
    virtual	~PXL_Raster();

    virtual int64	getMemoryUsage(bool inclusive) const;

    /// Copy over all the information and data from the src.  This allocates a
    /// new raster (rather than doing a shallow copy).
    virtual void	copy(const PXL_Raster &src);

    /// copies only the raster properties (res, type, etc), not the data, nor
    /// does this method allocate memory for a raster.
    virtual void	copyProperties(const PXL_Raster &src);
    
    // initializes the raster (destroys any previous image data). If you call
    // any of these methods, call init() after to reallocate the raster.
    void		setPacking(PXL_Packing p);
    void		setFormat(PXL_DataFormat f);
    void		setRes(exint x, exint y);
    void		setBWPoints(float black, float white);

    /// Colorspace that this raster is in. Changing the color space does not
    /// affect the image values stored in this raster, only how they are
    /// interpreted. custom_gamma must be specified for PXL_CS_CUSTOM_GAMMA.
    void		setColorSpace(PXL_ColorSpace space,
				      fpreal custom_gamma = 0.0);
    void		setOCIOColorSpace(const UT_StringHolder &space);
    
    void		init();
    
    // sets the raster data to r, optionally giving this class ownership of it.
    // make sure it's the right packing type, res & format!
    void		setRaster(void *r, bool give_ownership = true,
				  bool use_offset = false);

    // steals ownership from this raster. Returns NULL if this raster doesn't
    // own the image.
    void *		steal();
    
    // packing, format & xres/yres must be set for the raster to be valid.
    int			isValid() const	   { return myRaster ? 1 : 0; }
    // accessers.
    exint		getXres() const    { return myXres; }
    exint		getYres() const    { return myYres; }
    exint		getNumPixels() const { return myXres * myYres; }
    int64		getStride() const  { return myStride; }
    int64		getSize() const    { return myRasterSize; }
    PXL_DataFormat	getFormat() const  { return myFormat; }
    PXL_Packing		getPacking() const { return myPacking; }
    void		getBWPoints(float &black, float &white) const;

    /// Colorspace this raster requires for correct display
    PXL_ColorSpace	getColorSpace() const { return myColorSpace; }

    /// Gamma that must be applied to this raster to get accurate color. Only
    /// valid for PXL_CS_LINEAR, PXL_CS_GAMMA2_2, and PXL_CS_CUSTOM_GAMMA.
    fpreal		getColorSpaceGamma() const { return myColorSpaceGamma; }
    /// OCIO colorspace name, valid for PXL_CS_OCIO
    const UT_StringHolder &getColorSpaceName() const { return myColorSpaceName;}

    // data window
    void		setDataWindow(const UT_DimRect &new_rect);
    const UT_DimRect &	getDataWindow() const
			{ return myDataWindow; }
    bool		hasDataWindow() const
			{ return !myDataWindow.isEmpty(); }

    // returns the min/max values. black & white specify the black & white
    // of the raster, comp specifies an optional component to get the range
    // from. If ignore_farthest is true, 'max' will contain the 2nd largest
    // value. Good for ignoring the background.
    void		getRange(float &min, float &max,
				 float black = 0.0f, float white = 1.0f,
				 exint comp = -1,
				 bool ignore_farthest = false) const;

    // leave 2 pixels to draw grey single channels (R,G,B)
    void *		getPixels()
			{ return (unsigned char *)myRaster+myRasterOffset; }
    
    const void *	getPixels() const
			{ return (unsigned char *)myRaster+myRasterOffset; }

    // this is for drawing single greyscale R,G,B images.
    const void *	getRawPixels() const
			{ return myRaster; }

    // x/y coords, + comp# for Non-interleaved data (0-2 for RGB, 0-3 RGBA). 
    void *		getPixel(exint x, exint y, exint comp = 0);
    const void *	getPixel(exint x, exint y, exint comp = 0) const;

    void                setPixelValue(exint x, exint y, const float *vals);
    void		getPixelValue(exint x, exint y, float *vals) const;

    // returns 1 if interleaved (RGBRGB), 0 otherwise. 
    bool                isInterleaved() const;

    // returns 1 if this raster can have its components rendered separately.
    int			areComponentsRenderable() const;
    
    // if not interleaved, returbs the size of 1 channel's data. 
    int64		getNonInterleavedSize() const;
    
    // simple operations.

    // clears the raster to 0, unless color is not null; it assumes it points
    // to 8/16/32 bit int or float single/triple/quad value, depending on
    // packing type & data format. 
    void		clear(void *color = 0);
    void		clearNormal(float *color,
				    unsigned ib = 0,
				    unsigned iw = 0);


    // reads a scanline from the raster. THe buffer must be big enough to
    // hold a single scanline at the data format and packing of the raster.
    void		readFromRow(exint y, void *data) const;
    
    // Writes a scanline to the raster. The data format and packing are
    // assumed to match the raster's. 
    void		writeToRow(exint y, const void *data);
    
    // instead of writing a scanline, this writes a column of data. The
    // data format and packing are assumed to match the raster's, and
    // have yres pixels. Useful for flopping images.
    void		writeToColumn(exint x, const void *data);

    // deletes the image memory. If full = 1, everything is reset - xres/yres
    // to 0, packing and format to invalid.
    virtual void	reset(int full = 0);

    // Extracts a portion of the image 'from' into this raster, setting the
    // res, data format and packing if needed.
    void		extract(const PXL_Raster &from,
				exint x1, exint y1, exint x2, exint y2);

    // Inserts a portion of the image 'from' into this raster. The res,
    // data format and packing must match.
    void		insert(const PXL_Raster &from,exint x, exint y);

    void		applyLUT(const PXL_Raster *src, float *lptr[4], 
				 exint comp, float lstart, float lend);

    // of the raster. The rectangle can go outside of the raster.
    void		streakEdges(exint x1,exint y1, exint x2,exint y2,
		           PXL_StreakComponents streak_comps = PXL_STREAK_ALL);
    void		convert8Bit(unsigned b, unsigned w);

    void		adjustSaturation(float sat);

    bool		 load(UT_IStream &is);
    int			 save(std::ostream &os, int binary = 1) const;
    
    int		 	 getProxyIndex() const { return myProxyIndex; }
    
    void		 setDataWritten(bool written = true)
			 { myWriteFlag = written; }
    bool		 getDataWritten() const { return myWriteFlag; }

    void		 setTextureID(void *id) { myTextureID = id; }
    void		*getTextureID() const;
    void		 clearTextureID();

    // dither the image in a subdued way for mplay's render over previous.
    // If a crop region is provided, only pixels inside the crop will be
    // dimmed.
    void		dither(const UT_DimRect &crop);
    void		dither();

    bool                unpremultiply(PXL_Raster *other = nullptr);

    bool                hasAlpha() const;

    bool                clampAlpha();

    void		addInterest(void *callerData,
				    void (*changeCB)(PXL_Raster *, void *));
    void		removeInterest();

protected:
    virtual void	initialize();
    bool		allocRaster(int alloc_mem = 1);

    void		applyLUT8(const PXL_Raster *src, float *lptr[4], 
				   exint comp, float lscale, float lshift,
				   exint size);
    void		applyLUT16(const PXL_Raster *src, float *lptr[4], 
				   exint comp, float lscale, float lshift,
				   exint size);

    virtual void	attemptRealloc(int64 size);
   
    PXL_DataFormat	myFormat;
    PXL_Packing		myPacking;
    PXL_ColorSpace	myColorSpace;
    fpreal		myColorSpaceGamma;
    UT_StringHolder	myColorSpaceName;
    exint		myRasterOffset;
    exint		myXres, myYres;
    int64		myStride;
    int64		myRasterSize;
    float		myBlack, myWhite;
    void		*myRaster;
    bool	 	myAmOwner;
    int			myProxyIndex;
    bool		myWriteFlag;
    void	       *myTextureID;
    void		(*myChangeCB)(PXL_Raster *, void *);
    void		*myCallerData;

    UT_DimRect		myDataWindow;		// (0,0) is lower-left corner
};

/// This is a wrapper for when the PXL_DataFormat and PXL_Packing are known
/// at compile-time, e.g. constructing this inside a branch of a switch
/// statement over all types.
template<typename T, int NCOMPONENTS, bool INTERLEAVED, bool READ_ONLY>
class PXL_RasterWrapper
{
public:
    typedef typename SYS_ConstType<PXL_Raster,READ_ONLY>::type RasterType;
    typedef typename SYS_ConstType<T,READ_ONLY>::type DataType;

    SYS_FORCE_INLINE PXL_RasterWrapper(RasterType *raster = nullptr)
    {
        UT_ASSERT_COMPILETIME(NCOMPONENTS >= 1 && NCOMPONENTS <= 4);
        UT_ASSERT_MSG(!std::numeric_limits<T>::is_integer || !std::numeric_limits<T>::is_signed, "Integer types must be unsigned.");

        setRaster(raster);
    }

    SYS_FORCE_INLINE PXL_RasterWrapper<T, NCOMPONENTS, INTERLEAVED, true> makeConstant() const
    {
        return PXL_RasterWrapper<T, NCOMPONENTS, INTERLEAVED, true>(myRaster);
    }

    SYS_FORCE_INLINE RasterType *getRaster() const
    {
        return myRaster;
    }

    SYS_FORCE_INLINE exint getXres() const
    {
        return myRaster->getXres();
    }
    SYS_FORCE_INLINE exint getYres() const
    {
        return myRaster->getYres();
    }

    SYS_FORCE_INLINE void setRaster(RasterType *raster)
    {
        myRaster = raster;
        if (!raster)
            return;

        UT_ASSERT(PXLpackingComponents(raster->getPacking()) == NCOMPONENTS);
        UT_ASSERT(NCOMPONENTS==1 || raster->isInterleaved() == INTERLEAVED);
        UT_ASSERT(sizeof(T) == PXLformatDepth(raster->getFormat()));
        UT_ASSERT(std::numeric_limits<T>::is_integer == (
            raster->getFormat() == PXL_INT8 ||
            raster->getFormat() == PXL_INT16 ||
            raster->getFormat() == PXL_INT32));
        UT_ASSERT(raster->getRawPixels() != nullptr);
    }

    DataType *getPixel(exint x, exint y, exint comp = 0) const
    {
        UT_ASSERT_P(myRaster);
        if (!myRaster)
            return nullptr;
        const exint xres = myRaster->getXres();
        const exint yres = myRaster->getYres();
        if (x < 0 || y < 0 || x >= xres || y >= yres || !myRaster->getRawPixels())
            return nullptr;

        const exint stride = myRaster->getStride();
        int64 offset;
        if (NCOMPONENTS==1)
            offset = x*sizeof(T) + y * stride;
        else if (INTERLEAVED)
            offset = (comp+x*NCOMPONENTS)*sizeof(T) + y * stride;
        else
            offset = x *sizeof(T) + (y + comp*yres) * stride;

        return (DataType *)(((typename SYS_ConstType<unsigned char,READ_ONLY>::type *)myRaster->getPixels()) + offset);
    }

    SYS_FORCE_INLINE DataType *getPixelFast(void *pixels, exint x, exint y/*, exint comp = 0*/) const
    {
        UT_ASSERT_P(myRaster);
        const exint stride = myRaster->getStride();
        int64 offset;
        if (NCOMPONENTS==1)
            offset = x*sizeof(T) + y * stride;
        else if (INTERLEAVED)
            offset = (/*comp+*/x*NCOMPONENTS)*sizeof(T) + y * stride;
        else
            offset = x *sizeof(T) + (y /*+ comp*myRaster->getYres()*/) * stride;
        return (DataType *)(((typename SYS_ConstType<unsigned char,READ_ONLY>::type *)pixels) + offset);
    }

    SYS_FORCE_INLINE void getPixelValueFast(const void *pixels, exint x, exint y, float *vals) const
    {
        // const even if READ_ONLY is false
        const DataType *data = getPixelFast(pixels, x, y);
        if (INTERLEAVED || NCOMPONENTS==1)
        {
            for (int i = 0; i < NCOMPONENTS; ++i)
            {
                if (std::numeric_limits<T>::is_integer)
                    vals[i] = float(data[i])/float(std::numeric_limits<T>::max());
                else
                    vals[i] = float(data[i]);
            }
        }
        else
        {
            const exint yres_stride = myRaster->getYres()*myRaster->getStride();
            for (int i = 0; i < NCOMPONENTS; ++i,
                data = (const DataType *)(((const unsigned char *)data) + yres_stride))
            {
                if (std::numeric_limits<T>::is_integer)
                    vals[i] = float(*data)/float(std::numeric_limits<T>::max());
                else
                    vals[i] = float(*data);
            }
        }
    }

    SYS_FORCE_INLINE void zeroPixelValueFast(void *pixels, exint x, exint y) const
    {
        UT_ASSERT_P(!READ_ONLY);

        DataType *data = getPixelFast(pixels, x, y);
        if (INTERLEAVED || NCOMPONENTS==1)
        {
            for (int i = 0; i < NCOMPONENTS; ++i)
                data[i] = T(0);
        }
        else
        {
            const exint yres_stride = myRaster->getYres()*myRaster->getStride();
            for (int i = 0; i < NCOMPONENTS; ++i,
                data = (DataType *)(((typename SYS_ConstType<unsigned char,READ_ONLY>::type *)data) + yres_stride))
            {
                *data = T(0);
            }
        }
    }

    static SYS_FORCE_INLINE T convertFromFloat(const float val)
    {
        if (std::numeric_limits<T>::is_integer)
            return T(val * std::numeric_limits<T>::max());
        else
            return T(val);
    }

    SYS_FORCE_INLINE void setPixelValueFast(void *pixels, exint x, exint y, const T *vals) const
    {
        UT_ASSERT_P(!READ_ONLY);

        DataType *data = getPixelFast(pixels, x, y);
        if (INTERLEAVED || NCOMPONENTS==1)
        {
            for (int i = 0; i < NCOMPONENTS; ++i)
                data[i] = vals[i];
        }
        else
        {
            const exint yres_stride = myRaster->getYres()*myRaster->getStride();
            for (int i = 0; i < NCOMPONENTS; ++i,
                data = (DataType *)(((typename SYS_ConstType<unsigned char,READ_ONLY>::type *)data) + yres_stride))
            {
                *data = vals[i];
            }
        }
    }

private:
    RasterType *myRaster;
};

#endif
