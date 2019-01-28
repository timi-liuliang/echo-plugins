/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_DeepShadow.h ( IMG Library, C++)
 *
 * COMMENTS:	Deep shadow API
 */

#pragma once

#ifndef __IMG_DeepShadow__
#define __IMG_DeepShadow__

#include "IMG_API.h"
#include "IMG_DeepStat.h"
#include "IMG_FileTypes.h"

#include <UT/UT_Rect.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_StringHolder.h>
#include <PXL/PXL_Forward.h>

class PXL_DeepPixel;
class PXL_DeepSampleList;
class img_DeepShadow;
class IMG_DeepShadow;
class IMG_DeepShadowChannel;
class IMG_FileParms;
class IMG_Format;
class IMG_Stat;
class UT_WorkBuffer;
class UT_Options;

/// @brief Thread-safe convenience class to make reading DSM pixels easy
///
/// This is a thread-safe convenience class to wrap around reading pixels
/// which ensures that the pixel is closed properly.
///
/// @warning Beware when declaring an IMG_DeepPixelReader in the same scope
/// as an IMG_DeepShadow.  Since the pixel reader keeps a reference to the
/// file, the destruction order is important -- though calling close()
/// should avoid any problems.
///
/// The class should be used as follows: @code
///
///		IMG_DeepPixelReader	pixel(dsm);
///		for (x, y) in raster {
///		    if (pixel.open(x, y)) {
///			for (int i = 0; i < pixel.getDepth(); i++)
///			     data = pixel.getData(chp, i);
///		    }
///		}
/// @endcode
///
class IMG_API IMG_DeepPixelReader
{
public:
     IMG_DeepPixelReader(const IMG_DeepShadow &dsm);
    ~IMG_DeepPixelReader();

    /// Open a pixel for reading
    bool		open(int x, int y);
    /// Close the pixel.  This is called automatically on destruction.
    /// However, if an IMG_DeepPixelReader and IMG_DeepShadow are created in
    /// the same scope, the IMG_DeepPixelReader may hold onto references (and
    /// cause crashing errors).  Calling close() manually is a safe thing to
    /// do.
    void		close();
    /// Returns the number of z-records
    int			getDepth() const;
    /// Return the data for the channel at the given depth index.
    const float		*getData(const IMG_DeepShadowChannel &chp,
				    int depth_index);

    /// Alternately, just fill a PXL_DeepSampleList with the appropriate
    /// data.  This manages zback, coverage, flags and other special deep
    /// channels.
    bool	readPixel(int x, int y, PXL_DeepSampleList &pixel) const;


    /// After opening, you can call either of these methods to ensure that the
    /// data is in the correct form for your application.  "composited" pixels
    /// will have the data accumulated over from front to back.  "uncomposited"
    /// pixels will have the raw data for each z-record.
    ///
    /// If the force flag is false, then the state of the file will be used
    /// to determine whether the deep records need to be processed or not.
    ///
    /// These methods should be called after the pixel has been opened.
    void		composite(const IMG_DeepShadowChannel &pz,
				  const IMG_DeepShadowChannel &of,
				  bool force = false);
    void		uncomposite(const IMG_DeepShadowChannel &pz,
				    const IMG_DeepShadowChannel &of,
				    bool force = false);

private:
    class		 DeepShadowPixel;	// Forward declaration
    DeepShadowPixel	*myHandle;
};

/// @brief Thread-safe convenience class to make writing DSM pixels easy
///
/// This is a thread-safe convenience class to wrap around writing pixels
/// which ensures that the pixel is closed properly.
///
/// @warning Beware when declaring an IMG_DeepPixelReader in the same scope
/// as an IMG_DeepShadow.  Since the pixel reader keeps a reference to the
/// file, the destruction order is important -- though calling close()
/// should avoid any problems.
///
/// The class should be used as follows for sequential samples: @code
///
///		IMG_DeepPixelWriter pixel(dsm);
///		for (x, y) in raster {
///		    if (pixel.open(x, y)) {
///			for (int i = 0; i < depth; i++)
///			     data = pixel.writeOrdered(z, data, vsize);
///			pixel.close();
///		    }
///		}
/// @endcode
///
class IMG_API IMG_DeepPixelWriter
{
public:
     IMG_DeepPixelWriter(const IMG_DeepShadow &dsm);
    ~IMG_DeepPixelWriter();
    
    /// Ignore the desired sample compositing state in IMG_DeepShadow.
    /// The default is to assume the incoming samples are pre-composited.
    /// Writing raw samples will ignore compression levels, since the 
    /// compressors assume pre-composited samples.
    void		writeRawSamples(bool enable);
    
    /// Open a pixel for writing
    bool		open(int x, int y);

    /// Close the pixel.  This is called automatically on destruction.
    /// However, if an IMG_DeepPixelWriter and IMG_DeepShadow are created in
    /// the same scope, the IMG_DeepPixelReader may hold onto references (and
    /// cause crashing errors).  Calling close() manually is a safe thing to
    /// do.
    bool		close();

    /// Write data to the currently open pixel.  This method inserts an 
    /// unordered data record with the given z-value and data into the 
    /// current pixel.  vsize is the number of floats that are contained
    /// in the data array.
    ///
    /// The first 3 floats of the data array must be the opacity (RGB).  Any
    /// extra channel data follows this according to the float_offset
    /// associated with the channel.  This data may be quantized to a single
    /// float depending on the options.
    ///
    /// If @c vsize doesn't contain a full record, then this method will fail.
    /// vsize must be greater or equal to 3 (depending on extra channels)
    ///
    /// Currently, mantra will always interpret the data as a rgb opacity
    /// triple.
    ///
    /// The @c flags parameter is used to pass information to the
    /// @c PXL_DeepSampleList interface.  Possible values are:
    ///   - 0x00:  No special meaning
    ///   - 0x01:  Sample represents a volume
    ///   - 0x02:  Sample is a matte surface
    /// These values are found in the @c PXL_DeepSampleList::SampleFlag enum.
    ///
    /// The @c sampleid parameter specifies which pixel sample is associated
    /// with these values (see @c sxres and @c syres on the create() method).
    ///
    /// The @c z value represents the "z-front" value for the depth sample.
    /// For hard surfaces @c dz should be 0.  For volumes @c dz should be set
    /// to the extent of the volume sample so that the "z-back" data can be set
    /// appropriately.
    bool		write(float z,
				const float *chdata, int chsize,
				int flags,
				int sampleid,
				float dz=0);

    /// Perform the same operation as write() except assume that data
    /// is pre-sorted by increasing z-value.  This method will perform
    /// slightly better than write() as data does not need to be sorted.
    bool		writeOrdered(float z,
				const float *chdata, int chsize,
				int flags,
				int sampleid,
				float dz=0)
    {
	return write(z, chdata, chsize, flags, sampleid, dz);
    }

    /// Alternatively, you can do something like: @code
    ///  PXL_DeepPixel	pixel;
    ///  pixel.startPixel(x, y);
    ///  for (rec : records) pixel.store(rec);
    ///  write(pixel.closePixel(writer.compressor()));
    /// @endcode
    bool		writePixel(int pixel_x, int pixel_y,
				    const PXL_DeepSampleListPtr &pixel);

    /// Access to the pixel compressor.
    const PXL_DeepCompressorPtr	&compressor() const { return myDeepCompressor; }

private:
    img_DeepShadow		*myFile;
    int				 myPixelX, myPixelY;
    UT_UniquePtr<PXL_DeepPixel>	 myDeepPixel;
    PXL_DeepCompressorPtr	 myDeepCompressor;
    bool			 myWriteRaw;
};

/// @brief Single channel of a Houdini DSM image
///
/// A Houdini deep shadow/camera file can have any number of image channels
/// (planes).  A DSM image contains a single channel for opacity.  A deep
/// camera image has the opacity channel along with any number of extra image
/// channels.
class IMG_API IMG_DeepShadowChannel
{
public:
    /// @param name		The unique name of the DSM channel
    /// @param float_offset
    ///		When data is read or written in deep shadow files, the pixel
    ///		data is interleaved.  That is, the data for a single pixel can
    ///		be represented as an array of floats.  Each channel has a
    ///		unique offset into the array.  The user should @b not assume
    ///		that the offsets are contiguous.  For example, the pixel data
    ///		might look like: @code
    ///			[ Of.r,		// Opacity red
    ///			  Of.g,		// Opacity green
    ///			  Of.b,		// Opacity blue
    ///			  Cf.r,		// Color (red)
    ///			  Cf.g,		// Color (green)
    ///			  Cf.b,		// Color (blue)
    ///			  s,		// s coordinate
    ///			  t,		// t coordinate
    ///			  N.x,		// Normal x
    ///			  N.y,		// Normal y
    ///			  N.z,		// Normal z
    ///			]
    ///		@endcode
    ///		Where the channels would be defined by: @code
    ///			IMG_DeepShadowChannel("Of", 0, 3);
    ///			IMG_DeepShadowChannel("Cf", 3, 3);
    ///			IMG_DeepShadowChannel("s", 6, 1);
    ///			IMG_DeepShadowChannel("t", 7, 1);
    ///			IMG_DeepShadowChannel("N", 8, 3);
    ///		@endcode
    /// @param tuple_size	Number of floats
    /// @param storage
    ///		Storage type.  This may be one of { "real16", "real32",
    ///		"real64" }.  This value represents the storage of the channel
    ///		in the file.
    /// @see IMG_DeepShadow::addExtraChannel()
     IMG_DeepShadowChannel(const char *name,
			    int float_offset,
			    int tuple_size,
			    const char *storage="real32");

    bool		   operator==(const IMG_DeepShadowChannel &s) const;

    /// @{
    /// Data acceess function
    const char		*getName() const	{ return myName.c_str(); }
    const char		*getStorage() const	{ return myStorage.c_str(); }
    int			 getOffset() const	{ return myOffset; }
    int			 getTupleSize() const	{ return myTupleSize; }
    /// @}

private:
    UT_StringHolder myName;
    UT_StringHolder myStorage;
    int		 myOffset;
    int		 myTupleSize;
};

/// @brief Class to read or write deep shadow/camera images
///
/// This class provides a creation interface for deep shadow maps.
/// The class can be used as follows: @code
///	setOption(...);
///	setOption(...);
///	...
///	open(fname, xres, yres);
///	IMG_DeepPixelWriter	writer;
///	foreach pixel loop
///		writer.open(pixel);
///		foreach record
///			writer.write(record);
///		end loop
///		writer.close();
///	end loop
///	close();
/// @endcode
///
/// To read from an existing deep shadow map, the class can be used as
/// follows: @code
///	IMG_DeepShadow		dsm;
///	IMG_DeepPixelReader	pixel;
///	dsm.open(fname);
///	foreach (x,y) in raster:
///		pixel.open(x, y)
///		for (i = 0; i < pixel.getDepth(); i++)
///			pixel.getData(chp, i);
///	close();
/// @endcode
///
/// @see
///	- TIL_TextureMap - for filtered evaluation of DSM/DCM's)
///	- IMG_DeepPixelReader - For thread-safe reading of DSM pixels
///	- IMG_DeepShadowChannel
class IMG_API IMG_DeepShadow
{
public:
	     IMG_DeepShadow();
    virtual ~IMG_DeepShadow();

    /// Certain channel names are reserved as "special" channels.  These
    /// channels have special interpretations for deep images (i.e. "Z",
    /// "ZBack", etc.).  Each format registers their special channel names
    static void		addSpecialChannel(const UT_StringHolder &channel_name,
				    IMG_DeepPlaneMask type);
    static IMG_DeepPlaneMask	isSpecialChannel(const char *name);

    /// Set a file creation option.  Options must be set before the 
    /// file is opened with open() below.
    ///
    /// Currently (and this list may be out of date), the options are:
    ///	-  "ofstorage"	 = one of {	"int8", "int16", "int32", "int64",
    ///					"uint8", "uint16", "uint32", "uint64",
    ///					"real16", "real32", "real64" }
    ///	-  "pzstorage"	 = one of { "int8" ... "real64" }
    ///	-  "ofsize"	 = one of { "1", "3" }
    ///	-  "compression	 = a value between "0" and "9" (lossyness)
    ///	-  "zbias"	 = minimum delta between z-values (float value)
    ///	-  "depth_mode"	 = one of { "nearest", "midpoint", "farthest" }
    ///	-  "depth_interp" = one of { "discrete", "continuous" }
    ///	-  "compositing"  = "0" will turn off pre-compositing of the z-records
    ///	   This is recommended when dealing with deep camera maps (i.e. if
    ///	   there are extra channels)
    void	setOption(const char *option, const UT_StringHolder &value);
    void	setOption(const char *option, fpreal value);
    void	setOption(const char *option, int64 value);
    void	setOption(const char *option, int value)
		    { setOption(option, (int64)value); }

    /// Set options based on the "texture:*" options defined in the UT_Options.
    /// The UT_Options will be checked for options:
    ///	 - @c texture:ofstorage
    ///	 - @c texture:pzstorage
    ///	 - @c texture:ofsize
    ///	 - @c etc.
    void	 setOptions(const UT_Options &options);

    /// Print the current values of the file options into the work buffer
    void	 printArgs(UT_WorkBuffer &wbuf, bool json);

    /// Get a description of the available options that can be provided in
    /// setOption()
    const char	*getDescription();

    /// Add an extra channel to be stored along with Of and Pz.  Any number of
    /// channels may be added (though the file size will increase accordingly).
    ///	- The name must be unique among (and not Of or Pz)
    ///	- The float_offset is the offset into the "data" in the
    ///	  IMG_DeepPixelWriter::write() method.  Note that the opacity
    ///	  must occupy the first 3
    ///	  floats of the data array.  So, the float_offset @b must be at least 3.
    ///	- The tuple_size is the number of elements in the channel
    ///	  Currently, this must be 1, 3 or 4.
    ///	- The storage specifies the storage type (and uses the same
    ///	  tokens as the "ofstorage" option (i.e. int8, int16, int32, int64,
    ///	  uint8, uint16, uint32, uint64, real16, real32 or real64.
    /// The extra channels must be added @b before open() is called.
    bool	 addExtraChannel(const IMG_DeepShadowChannel &def,
			const UT_Options *options=0);

    /// Open a deep shadow map for writing.  This method will return false
    /// if there were errors opening the texture.
    /// The resolution of the image is specified by @c xres and @c yres, while
    /// the @c sxres and @c syres parameters specify the samples per pixel
    /// (i.e. 3x3 sampling).  The pixel_aspect is typically 1.0, while the crop
    /// can be a NULL ptr.
    ///
    /// If the sample resolution is unknown, you can set @c sxres and @c syres
    /// to 1 (the @c sampleid should be set to 0).
    /// @{
    bool	 create(const char *name, int xres, int yres,
			int sxres, int syres,
			float pixel_aspect = 1.0,
			const UT_DimRect *crop = NULL);
    bool         create(
                    const char *name,
                    const IMG_Stat &stat,
                    const IMG_FileParms *options = NULL,
                    const IMG_Format *fmt = NULL);
    ///@}

    /// Open a deep shadow map for reading.
    bool	 open(const char *name);


    /// These methods are now deprecated in favor of the
    /// IMG_DeepPixelWriter class, which allows thread-safe writes to
    /// deep images.
    /// {
    SYS_DEPRECATED(13.0)
    bool		 pixelStart(int x, int y);
    SYS_DEPRECATED(13.0)
    bool		 pixelWrite(float z, const float *data, int vsize);
    SYS_DEPRECATED(13.0)
    bool		 pixelWriteOrdered(float z, const float *data,
					    int vsize);
    SYS_DEPRECATED(13.0)
    bool		 pixelClose();
    /// }

    /// Close the deep shadow map and write it to disk.
    bool		 close();

public:

    /// Get the resolution of the deep shadow map.
    void		 resolution(int &xres, int &yres) const;

    /// Get the number of channels in the DSM
    int			 getChannelCount() const;

    /// Get a channel definition
    const IMG_DeepShadowChannel	*getChannel(int i) const;

    /// Get UT_Option structure from the file.  These options are stored with
    /// the file and may be queried in VEX using teximport().
    UT_SharedPtr<UT_Options> getTextureOptions() const;

    /// Store the UT_Options structure in the file. These options are stored with
    /// the file and may be queried in VEX using teximport() on load.
    bool		 setTextureOptions(const UT_Options &opts);

    /// @{
    /// Extract the worldToCamera transform from a UT_Options.  Returns false
    /// if there was no matrix available.
    ///
    /// This extracts the "space:world" matrix from the UT_Options.
    static bool		 getWorldToCamera(const UT_Options *options,
				UT_Matrix4F &xform);
    static bool		 getWorldToCamera(const UT_Options *options,
				UT_Matrix4D &xform);
    template <typename T>
    bool		 getWorldToCamera(UT_Matrix4T<T> &xform) const
			    { return getWorldToCamera(getTextureOptions().get(),
			                              xform); }
    /// @}

    /// @{
    /// Get the camera's projection transform from the file options.  Returns
    /// false if there was no matrix available.
    /// If @c fit_z is true, the projection will attempt to fit the z
    /// coordinates to the NDC near/far to the range (0,1).  If the camera:clip
    /// option is not saved, this will not be possible (z coordinates will
    /// remain unchanged).  The method will @b not fail if there is no
    /// camera:clip saved, but the z coordinates will have unexpected values
    ///
    /// The method gets the values for the projection matrix from the following
    /// options:
    ///  - float camera:zoom
    ///  - int camera:projection (optional, defaults to 0)
    ///  - float image:pixelaspect (optional, defaults to 1)
    ///  - vector4 image:window (optional, defaults to [0, 1, 0, 1])
    ///  - vector2 camera:clip (used iff @c fit_z is true, defaults to (0,1))
    ///  - float camera:orthowidth (required iff @c projection != 0)
    static bool		getCameraToNDC(const UT_Options *options,
				UT_Matrix4F &xform,
				int xres, int yres,
				bool fit_z=true);
    static bool		getCameraToNDC(const UT_Options *options,
				UT_Matrix4D &xform,
				int xres, int yres,
				bool fit_z=true);
    template <typename T>
    bool		getCameraToNDC(UT_Matrix4T<T> &xform,
				bool fit_z=true) const
			{
			    int		xres, yres;
			    resolution(xres, yres);
			    return getCameraToNDC(getTextureOptions().get(),
					xform, xres, yres, fit_z);
			}
    /// @}

    /// @{
    /// Get the world to camera NDC space transform using the file options.
    /// Returns false if there is no matrix available.
    /// If @c fit_z is true, the projection will attempt to fit the z
    /// coordinates to the NDC near/far to the range (0,1).  If the camera:clip
    /// option is not saved, this will not be possible (z coordinates will
    /// remain unchanged).
    /// @note This simply returns getWorldToCamera()*getCameraToNDC()
    static bool		getWorldToNDC(const UT_Options *options,
				UT_Matrix4F &xform,
				int xres, int yres, bool fit_z);
    static bool		getWorldToNDC(const UT_Options *options,
				UT_Matrix4D &xform,
				int xres, int yres, bool fit_z);
    template <typename T>
    bool		getWorldToNDC(UT_Matrix4T<T> &xform,
				bool fit_z) const
			{
			    int	xres, yres;
			    resolution(xres, yres);
			    return getWorldToNDC(getTextureOptions().get(),
					xform, xres, yres, fit_z);
			}
    /// @}

    /// Return deep stats 
    UT_SharedPtr<IMG_DeepStat>	getDeepStat() const;

    float		ofBias() const;
    float		zBias() const;

    /// Return the PXL_DeepChannelList being written to the DSM
    const PXL_DeepChannelListPtr	&deepChannels() const;

    /// Return the number of samples per pixel
    int					 deepSampleListsPerPixel() const;

    /// Metadata to send to deep image file
    void setWriteTag(
        const char *tag,
        int datasize,
        const char *const *data);

    bool copyImageTextureOptions(const IMG_DeepShadow &src, bool clear_existing);

protected:
    /// Perform all computation for creating an image, but don't actually open
    /// the IMG_File.  This can be done when you want to make sure all the deep
    /// information is set up properly, but don't actually want to create the
    /// image.  One example of this might be when doing distributed rendering.
    ///
    /// This takes the same arguments as @c create().
    bool		createStat(IMG_Stat &stat,
				    const char *name, int xres, int yres,
				    int sxres, int syres,
				    float pixel_aspect,
				    const UT_DimRect *crop);

private:
    friend class	IMG_DeepPixelReader;
    friend class	IMG_DeepPixelWriter;

    bool		openPixelRead(IMG_DeepPixelReader &h, int x, int y);
    void		closePixelRead(IMG_DeepPixelReader &h);

private:
    friend class	 img_DeepShadow;

private:
    UT_UniquePtr<IMG_DeepPixelWriter>	 myWriter;
    img_DeepShadow			*myFile;
};

#endif
