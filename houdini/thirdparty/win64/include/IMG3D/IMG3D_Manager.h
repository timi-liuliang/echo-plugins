/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG3D_Manager.h ( IMG3D Library, C++)
 *
 * COMMENTS:	API into the 3D texture library.
 */

#ifndef __IMG3D_Manager__
#define __IMG3D_Manager__

#include "IMG3D_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_BoundingBox.h>

class UT_String;

/// Callback function for evaluating the values of a 3D texture at a particular
/// position in space.  The callback takes:
/// - nvalues	= the number of positions to evaluate
/// - pos	= an array which is nvalues long containing the position to
///		  evaluate.
/// - result	= an array whose size is <tt>nvalues * sum(ch_size)</tt> long.
///		  The resulting values should be interleaved.  That is, all the
///		  channel values for the first position, then all the channel
///		  values for the second position, etc.
/// - ch_name	= The array of channel names being evaluated
/// - ch_size	= The array of channel sizes being evaluated.
/// - for_aa	= Whether the samples being evaluated are primary or
///		  anti-aliasing samples.
typedef void	(*IMG3D_TextureEval)(int nvalues,
			const UT_Vector3 *pos,
			fpreal32 *result[],
			const char *names[],
			int sizes[],
			int for_aa);

typedef void	(*IMG3D_Manager_Eval)(int nvalues,
			const fpreal32 *pos,
			fpreal32 *result[],
			const char *names[],
			int sizes[],
			int for_aa);

/// @brief Class to handle reading/writing 3D texture images
///
/// A 3D texture file is similar to a 2D texture file, but with an added
/// dimension.  The 3D texture is broken down into 3D tiles.  Each tile is
/// stored independently in the file.  This allows for partial loading of the
/// 3D texture (without having to load the entire image at one time).
///
/// However, the tiled approach makes it difficult to create images since the
/// writer works on a single tile at a time.
///
/// Just like image files, the 3D texture file can have an arbitrary number of
/// channels, each with its own name, storage and tuple size.
///
/// In order to read an 3D texture, you have to open both an image and a
/// channel: @code
///	IMG3D_Manager	fp;
///	int		channel_index;
///	fp.openTexture(filename);
///	fp.openChannel(channel);
/// @endcode
class IMG3D_API IMG3D_Manager {
public:
     IMG3D_Manager();
    ~IMG3D_Manager();

    /// Close the file (whether opened for read or write).
    /// This is done automatically on destruction.
    int		 closeTexture();

public:
    /// Open a texture for reading.  Returns 0 if there was an error opening
    /// the file.
    int		openTexture(const char *filename);

    // Query functions, require the texture to be open (and specified
    // channel indices to exist)

    // Size of the bounding box of the i3d.  
    // Null will be returned if the file has not been opened.

    /// Returns an array of 3 floats for xmin(), ymin(), zmin() representing
    /// the left, bottom, front corner of the 3D texture.
    const float		*getCorner() const;	// Left, bottom, front corner
    /// Returns an array of 3 floats for xsize(), ysize(), zsize() representing
    /// the size of the bounding box.
    const float		*getSize() const;	// width, height, depth.
    /// Returns the number of voxels in x, y, z.
    const int		*getRes() const;	// Raw size in voxels. w, h, d

    /// Number of channels in the texture image
    int		getNChannels() const;

    /// Query channel names
    const char *getChannelName(int idx) const;

    /// Return the number of floats in the given channel
    int		getChannelSize(int idx) const;

    /// Find the index of a named channel.  Returns -1 if not found.
    int		getChannelNumber(const char *name) const;

    /// Once a texture is open, you must open a handle to a channel to be able
    /// to access data in the channel.  Returns 0 on failure or 1 on success.
    /// @note Only a single channel can be open per manager.
    int		openChannel(const char *channel_name);

    /// Set the evaluation filter.  This filter filter can be any one of the
    /// standard Houdini filter types.  The filtering is use for evaluation and
    /// integration of the texture.  Returns 0 on failure.
    /// @see UT_Filter.
    int		setFilter(const char *filter_name, fpreal filter_width);

    /// Evaluate the 3D texture at the given position.  Returns 0 on failure.
    /// Uses the filter set by setFilter().
    /// @note The @c result buffer should have enough storage to hold
    ///   getChannelSize().
    int		sample(const UT_Vector3 &pos, float *result);

    /// Evaluate the gradient of the 3D texture at the given position.
    /// Uses the filter set by setFilter().
    /// @note The gradient can only be computed on scalar channels.
    /// @note 3 floats will be returned for a scalar channel.
    int		gradient(const UT_Vector3 &pos, float *result);

    /// Integrate from p0 to p1 and return the integration result.
    ///	Optional controls are:
    ///  @param p0 Start of integration
    ///  @param p1 End of integration
    ///  @param result
    ///		Integration result.  This should large enough to hold
    ///		getChannelSize() floats.
    ///	 @param limit_max	if this value of the integration exceeds
    ///				this value, then the integration will be
    ///				terminated early.
    ///	 @param value_scale	scale the integration value by this amount.
    ///				This scaling is applied BEFORE limit checks.
    ///	 @param accuracy
    ///		A scaling on stepping.  Increasing the accuracy will perform
    ///		more sub-steps on the integration, yielding a more accurate
    ///		result (at a higher cost).  Increasing the accuracy can have a
    ///		significant effect on performance.
    ///
    /// As a result of integration, p1 will be clipped to the bounds of the
    /// texture.  As well, p0 will be moved to the point where the limit was
    /// reached (if the limit was reached).
    int		integrate(UT_Vector3 &p0, UT_Vector3 &p1, float *result,
			  fpreal limit_max=1, fpreal value_scale=1,
			  fpreal accuracy=0);

    /// Find the intersection against the iso-surface defined by density
    /// specified.  The accuracy is how close f(p0) will be to zero.
    /// If an intersection is found, the function will return 1.
    /// p0 will be moved to the intersection point (or to p1 if there was no
    /// intersection found).
    int		intersect(UT_Vector3 &p0, const UT_Vector3 &p1,
			  fpreal density, fpreal accuracy = 1e-5F);

    /// Load a channel as a flat list of floats.  The data is allocated using
    /// malloc().  It's the users responsibility for freeing this memory using
    /// free().  Given the getRes() function returning W, H, D (width, height,
    /// depth), the array returned will be <tt>W*H*D * getChannelSize() *
    /// sizeof(float)</tt> bytes long.
    /// Given an index (ix, iy, iz), the channel data can be indexed by:
    ///	<tt>ix + iy*W + iz*W*H</tt>
    float	*loadUntiledChannel();

    /// Same as loadUntitledChannel() but fills a user-allocated array.
    int		 loadUntiledChannel(float *data);

public:
    /// This method allows the creation of a 3D texture.  The bounding box
    /// defines the are where the map is sampled.
    ///
    /// Compression is the level of gzip compression to use.  Valid values are
    /// between 0 (no compression) and 9 (maximum compression).
    int			createTexture(
			    const char			*filename,
			    const UT_BoundingBox	&box,
			    int				xres,
			    int				yres,
			    int				zres,
			    int				compression=5);

    /// Once the texture is created, please call the following code to generate
    /// the values for the texture.
    ///	 @param num_channels	The number of channels
    ///	 @param channel_names	An array of channel names
    ///	 @param channel_sizes	The number of floats in each channel.
    ///	 @param evaluator	Callback to evaluate channel positions
    ///	 @param max_samples	Anti-aliasing samples
    ///	 @param variance	Variance threshhold for anti-aliasing to occur
    ///	 @param filter_width	Anti-aliasing filter width
    ///	 @param jitter		Amount of jitter for anti-aliasing
    /// @note currently, only box filtering is supported for anti-aliasing
    /// @return 0 if the process fails.
    int			 fillTexture(
			    int			 num_channels,
			    const char		*channel_names[],
			    int			 channel_sizes[],
			    IMG3D_TextureEval	 evaluator,
			    int			 max_samples=4,
			    fpreal		 variance=0.005,
			    fpreal		 filter_width=1,
			    fpreal		 jitter=1);

    /// Save flat untiled data to the file.  This method perfoms the inverse
    /// operation of loadUntiledChannel, but for multiple channels.
    ///	 @param num_channels	- The number of channels
    ///	 @param channel_names	- An array of channel names
    ///	 @param channel_sizes	- The number of floats in each channel
    ///	 @param data		- Source data for each channel
    ///  @param variance	- Ignored
    ///
    /// Given the @c getRes() function returning W, H, D (width, height,
    /// depth), the data array for channel i must be
    /// <tt>W*H*D * channel_sizes[i] * sizeof(float) </tt> bytes long.
    /// Given an index (ix, iy, iz), the channel data can be indexed by:
    ///	<tt>ix + iy*W + iz*W*H</tt>
    ///
    /// @return 0 if the process fails.
    int			 fillUntiledTexture(
			    int			 num_channels,
			    const char		*channel_names[],
			    int			 channel_sizes[],
			    const float		*data[],
			    fpreal		 variance=0.005);

    /// @{
    ///
    /// It's possible store and retrieve tags (based on the read/write mode of
    /// the file).  These tags can be queried using texinfo() in VEX.
    ///
    bool	exportTag(const char *name, int value);
    bool	exportTag(const char *name, float value);
    bool	exportTag(const char *name, const UT_Vector3 &value);
    bool	exportTag(const char *name, const UT_Vector4 &value);
    bool	exportTag(const char *name, const UT_Matrix3 &value);
    bool	exportTag(const char *name, const UT_Matrix4 &value);
    bool	exportTag(const char *name, const char *value);
    /// @}

    /// @{
    /// When importing, if the type doesn't match, casting will be done (where
    /// possible).  Importing a string is always possible.  If the import
    /// fails, the value will be untouched and the import will return false.
    bool	importTag(const char *name, int &value);
    bool	importTag(const char *name, float &value);
    bool	importTag(const char *name, UT_Vector3 &value);
    bool	importTag(const char *name, UT_Vector4 &value);
    bool	importTag(const char *name, UT_Matrix3 &value);
    bool	importTag(const char *name, UT_Matrix4 &value);
    bool	importTag(const char *name, UT_String &value);
    /// @}

public:
    // Deprecated methods that take floating point parameters.  You should
    // use the new methods above.
    /// @private - Please use the UT_Vector3 version
    int		SYS_DEPRECATED(8.2) sample(const float *pos, float *result)
		{ return sample(*(UT_Vector3 *)pos, result); }
    /// @private - Please use the UT_Vector3 version
    int		SYS_DEPRECATED(8.2) gradient(const float *pos, float *result)
		{ return gradient(*(UT_Vector3 *)pos, result); }
    /// @private - Please use the UT_Vector3 version
    int		SYS_DEPRECATED(8.2) integrate(float *p0, float *p1, float *result,
			  fpreal limit_max=1, fpreal value_scale=1,
			  fpreal accuracy=0)
		{ return integrate(*(UT_Vector3 *)p0, *(UT_Vector3 *)p1,
			result, limit_max, value_scale, accuracy); }
    /// @private - Please use the UT_Vector3 version
    int		SYS_DEPRECATED(8.2) intersect(float *p0, const float *p1,
			fpreal density, fpreal accuracy = 1e-5F)
		{ return intersect(*(UT_Vector3 *)p0, *(UT_Vector3 *)p1,
			density, accuracy); }
    /// @private - Please use the UT_BoundingBox version
    int		SYS_DEPRECATED(8.2) createTexture(
		    const char		*filename,
		    const float		 box[3][2],
		    int			xres,
		    int			yres,
		    int			zres,
		    int			compression=5)
		{
		    return createTexture(filename, UT_BoundingBox(
				box[0][0], box[1][0], box[2][0],
				box[0][1], box[1][1], box[2][1]), 
			    xres, yres, zres, compression);
		}
    /// @private - Please use the IMG3D_TextureEval version
    int		SYS_DEPRECATED(8.2) fillTexture(
		    int			 num_channels,
		    const char		*channel_names[],
		    int			 channel_sizes[],
		    IMG3D_Manager_Eval	 evaluator,
		    int			 max_samples=4,
		    fpreal		 variance=0.005,
		    fpreal		 filter_width=1,
		    fpreal		 jitter=1);

protected:
    void	*myData;	// Internal data structure
};

#endif
