/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_FileTTMap.h ( IMG Library, C++)
 *
 * COMMENTS:	Tiled Texture Maps
 */

#ifndef __IMG_FileTTMap__
#define __IMG_FileTTMap__

#include "IMG_API.h"
#include "IMG_File.h"
#include "IMG_TextureFilter.h"

class UT_Filter;
class UT_FilterWindow;
class UT_Options;
class IMG_TTMipSampler;
class IMG_Raster;
class TBF_Channel;


/// An IMG_TTChannel is used to retrieve pixel data from the image.
/// Each image owns an array of IMG_TTChannel objects that could 
/// represent environment map rasters or mipmap levels.
class IMG_API IMG_TTChannel
{
public:
	     IMG_TTChannel();
    virtual ~IMG_TTChannel();

    virtual int64	 getMemoryUsage(bool inclusive) const = 0;

    virtual void	 filterPixels(float *rgba, int nchan,
				      int first_x, int first_y,
				      const IMG_TextureFilter &filter,
				      const float *xweights,
				      const float *yweights,
				      int usize, int vsize) = 0;

    virtual void	 getPixel(float *rgba, int nchan, int iu, int iv,
				  const IMG_TextureFilter &filter) = 0;

    /// This method returns the raw pixel data.
    virtual void	 getPixel(void *pixel, int iu, int iv) = 0;
    /// Return raw scanline data
    virtual void	 getScanline(void *scanline,
				     int y, int xres, size_t bpp);

    /// This method is called when the channel is destroyed
    virtual void 	 closeChannel() {};
};

/// A IMG_TTMap represents a tiled texture format that can be
/// sampled for texture mapping or for environment mapping.  This class 
/// supports methods for sampling and filtering the texture data.  
/// This class is pure virtual and so derived classes must override the 
/// image file and format operations.
class IMG_API IMG_FileTTMap : public IMG_File
{
public:
	     IMG_FileTTMap();
    virtual ~IMG_FileTTMap();

    virtual const char  *className() const { return "IMG_FileTTMap"; }

    /// Return true if texture evaluation of this format is enabled
    virtual bool	textureEnabled() const;

    int		 getOGLRaster(void *data, int xres, int yres,
			      IMG_ColorModel cm, IMG_DataType dt,
			      int raster_number=0);

    // Perform unfiltered evaluation at a specified uv.  This operation
    // will not even perform filtering on pixels, so the caller is
    // responsible for all filtering and antialiasing.
    void	unfilteredSample(
			float *result, int tuple_size, float u, float v,
			const IMG_TextureFilter &info,
			int channel, int raster);

    // Sample with filtering at the specified uv.
    void	sample(float *result, int tuple_size, float u, float v,
			const IMG_TextureFilter &info,
			int channel, int raster)
		{
		    float	ua[4] = {u,u,u,u};
		    float	va[4] = {v,v,v,v};

		    sample(result, tuple_size,
			    ua, va, 1, info, channel, raster);
		}


    //  Sample the map within a quadrilateral in texture space.  When
    //  specifying four corners of the map, the order of the coordinates is
    //  very important:
    //
    //      BAD                     GOOD                    GOOD
    //      2--3                    3--2                    2--3
    //       \/                     |  |                    |  |
    //       /\                     |  |                    |  |
    //      0--1                    0--1                    1--0
    void	sample(float *result, float &visible, int tuple_size,
			const float u[4], const float v[4],
			float quality,
			const IMG_TextureFilter &info,
			int channel, int raster);
    void	sample(float *result, int tuple_size,
			const float u[4], const float v[4],
			float quality,
			const IMG_TextureFilter &info,
			int channel, int raster)
		{
		    float	visible;
		    sample(result, visible,
			    tuple_size, u, v, quality,
			    info, channel, raster);
		}

    // Only call the envLookup functions if this RAT is really a cubic
    // environment map.
    void		 envLookup(float *result, int tuple_size,
        		           const UT_Vector3 &r, float angle,
        		           const IMG_TextureFilter &info, int channel);

    void		 envLookup(float *result, int tuple_size,
        		           const UT_Vector3 &r00, const UT_Vector3 &r01,
        		           const UT_Vector3 &r11, const UT_Vector3 &r10,
        		           const IMG_TextureFilter &info,
        		           int channel);

    fpreal		 getUPixelSize() const	{ return myDx; }
    fpreal		 getVPixelSize() const	{ return myDy; }

    virtual int64	 getMemoryUsage(bool inclusive) const;
    virtual int		 getRasterCount() const { return 0; }
    int			 getChannelTupleSize(int ch) const;
    virtual int		 getNumChannels() const { return getStat().getNumPlanes(); }

    // Return the channel index to use for lookups that haven't specified a
    // particular channel.
    virtual int		 getDefaultChannelIndex() const
			 { return 0; }
    int			 findChannelIndex(const char *name) const;
    
    // TODO: Remove this method and replace with calls to IMG_Stat::getImageType
    //       Add IMG_TYPE_ENVMAP to the IMG_ImageType enum.
    virtual int		 isReflMap() const { return 0; }

protected:
    void	 initialize(int mipcount,
			    const UT_ValArray<IMG_TTChannel *> &channels);

    virtual int	 closeFile();

protected:
    // Mipmap level to start reading from the file.
    int			  myMipRead;

private:
    void		  initScaleAndSize();

    //
    // Data used in lookup of pixel values.  These members MUST be 
    // initialized by derived classes before this class is used for 
    // sampling, by calling the initialize() method above.
    //
    void		  clearMips();

    // We have MIPs for each channel and raster
    int			  getMipOffset(int channel, int raster)
			  {
				channel *= getRasterCount();
				channel += raster;
				return myMipCount * channel;
			  }

    float		  myDx, myDy;	// Inverse pixel sizes

    UT_IntArray		 myColorSize;

    // Mipmap samplers are stored as an array of pointers, with each
    // entry representing one mipmap level.  If this class is an 
    // environment map, multiple sets of mipmaps may be stored sequentially
    // in this array.
    UT_ValArray<IMG_TTMipSampler *>	myMips;	// Mip Maps
    int					myMipCount;
    int					myDSMComponentCount;
    int					myDepthComplexityPlane;
    bool				myFixMipFilterWidth;
};

#endif
