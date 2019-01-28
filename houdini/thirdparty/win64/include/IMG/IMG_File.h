/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_File.h ( IMG Library, C++)
 *
 * COMMENTS:
 *	Generic interface for loading and saving image files.
 */

#pragma once

#ifndef __IMG_File__
#define __IMG_File__

#include "IMG_API.h"
#include <iosfwd>
#include <SYS/SYS_Types.h>
#include <UT/UT_ErrorManager.h>
#include <UT/UT_FilterType.h>
#include <UT/UT_Rect.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_WorkArgs.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_StringArray.h>
#include <PXL/PXL_Forward.h>
#include "IMG_FileParms.h"
#include "IMG_Error.h"
#include "IMG_FileTypes.h"
#include "IMG_Stat.h"

class IMG_Format;
class UT_Options;
class UT_String;
class UT_WorkBuffer;
class UT_FileLUT;
class UT_IStream;
class FS_Reader;
class FS_Writer;
class IMG_FileTagList;
class img_ScanProgress;
class PXL_Raster;
class PXL_DeepSampleList;

class IMG_SaveRastersToFilesParms;

/// @brief Generic interface for reading and writing image files.
/// This class handles the reading and writing of all image formats that Houdini
/// supports. Use the static open() or create() methods to get an IMG_File
/// instance to read to or write from. Deleting this instance will close the
/// file.
class IMG_API IMG_File
{
public:
    virtual ~IMG_File();

    virtual int64 getMemoryUsage(bool inclusive) const;

    // The following methods are a public interface which is not dependent on
    // the format of the image.  Users of the library should use these methods
    // to do the loading and saving of the data.

    /// @brief Open an image file using a filename
    /// Open an image file for reading by specifying the filename and path of
    /// the file. An optional IMG_FileParms class may be passed to convert it
    /// into a specific image type (8bit, specific resolution, etc). An optional
    /// format may also be passed, which forces the library to assume the image
    /// file is of that particular format.
    /// This method returns a new IMG_File object which you read the image data 
    /// from, or NULL if an error occurred.
    static IMG_File	*open(const char *filename,
			      const IMG_FileParms *options = nullptr,
			      const IMG_Format *fmt = nullptr);
    
    /// @brief Open an image file from a stream
    /// Open an image file for reading given a stream. This is useful for 
    /// embedded images in a file. An optional IMG_FileParms class may be 
    /// passed to convert it into a specific image type (8bit, specific 
    /// resolution, etc). An optional format may also be passed, which forces
    /// the library to assume the image file is of that particular format.
    /// If 'steal_stream' is true, the image library assumes ownership of the
    /// stream and closes it when it's finished reading the file. If the stream
    /// is not seekable (cannot jump around in the file, such as with a pipe),
    /// set 'streamIsSeekable' to false.
    /// This method returns a new IMG_File object which you read the image data 
    /// from, or NULL if an error occurred.
    static IMG_File	*open(UT_IStream &is,
			      const IMG_FileParms *options = nullptr,
			      const IMG_Format *fmt=nullptr,
			      int steal_stream = 0,
			      bool streamIsSeekable = true);

    /// @brief Create a new image file
    /// Create a new image file with the pathname 'filename' according to the
    /// parameters specified in 'stat'.  You can supply an optional 'options'
    /// class to translate the image before being written (scale, flip, etc). 
    /// You can also specify an optional 'format' parameter to force the file
    /// to be written as a specific image format (regardless of the extension
    /// found in 'filename'). 
    /// NULL may be returned if the file could not be created for any reason.
    static IMG_File	*create(const char *filename,
				const IMG_Stat &stat,
				const IMG_FileParms *options = nullptr,
				const IMG_Format *fmt = nullptr);

    
    /// @brief Write an image file to an existing stream
    /// Writes a new image file to the stream 'os' according to the
    /// parameters specified in 'stat'.  You can supply an optional 'options'
    /// class to translate the image before being written (scale, flip, etc). 
    /// You can also specify an optional 'format' parameter to force the file
    /// to be written as a specific image format (regardless of the extension
    /// found in 'filename').
    /// If 'steal_stream' is true, the image library assumes ownership of the
    /// stream and closes it when it's finished writing the file. If the stream
    /// is not seekable (cannot jump around in the file, such as with a pipe),
    /// set 'streamIsSeekable' to false.
    /// NULL may be returned if the file could not be created in the stream.
    static IMG_File	*create(std::ostream &os,
	    			const IMG_Stat &stat,
				const IMG_FileParms *options = nullptr,
				const IMG_Format *fmt = nullptr,
				int steal_stream = 0,
				bool streamIsSeekable = true);

    /// @{
    /// @brief Writes a PXL_Raster to an image file.
    /// Convenience method to write an image stored in a PXL_Raster to a disk 
    /// file.
    static bool 	saveRasterAsFile(const char *filename,
				const PXL_Raster *raster,
				const IMG_SaveRastersToFilesParms &parms);
    static bool		saveRasterAsFile(const char *filename,
				const PXL_Raster *raster,
				const IMG_FileParms *fparms = nullptr,
				const IMG_Format *format = nullptr);
    /// @}

    /// @brief Writes PXL_Raster's to image files.
    /// Convenience method to write images stored in PXL_Raster's to disk
    /// files.  The @c linear_planes parameter specifies a pattern of planes
    /// which should be written to in linear space.
    static bool 	saveRastersAsFiles(const char *filename,
				const IMG_SaveRastersToFilesParms &parms);

    /// @brief Copies an existing image file to another image file.
    /// Convenience method to copy an image file from one file to another.
    /// The image date may be altered if the formats of the two files are
    /// not the same (based on filename extension), or if an optional 'options'
    /// class is passed.
    static bool		copyToFile(const char *sourcename,
				const char *destname,
				const IMG_FileParms *options = nullptr);

    static bool		copyToFileFormat(const char *sourcename,
				const char *formatname,
				UT_String *destname = nullptr,
				const IMG_FileParms *options = nullptr);

    /// @brief Copies an existing image file to a stream
    /// Convenience method to copy an image file to a stream. The image data
    /// may be altered if an optional 'options' class is passed, or if the 
    /// optional image 'format' parameter does not match the format of the 
    /// source image file.
    static bool		copyToStream(const char *sourcename,
				     std::ostream &deststream,
				     const IMG_FileParms *options = nullptr,
				     const IMG_Format *format = nullptr);

    virtual const char  *className() const { return "IMG_File"; }
 
    /// Closes the file. Called by the destructor if needed , so 'delete file'
    /// will often take the place of this call. However, you won't receive any
    /// error information in that case, so you can call it directly.
    int			 close();

    /// Returns 1 if we are reading, 0 if writing.
    int			 getReadMode() const;

    /// @{
    /// Returns the image type of the format we just opened.
    IMG_ImageType	 getImageType() const;
    /// @}

    /// @{
    /// Retrieve the file format description of the file. The format contains
    /// all information that is general to all files of this format type.
    const IMG_Format	*getFormat() const		{ return myFormat; }
    void		 setFormat(const IMG_Format *f)	{ myFormat = f; }
    /// @}

    /// Get the image description of the current image file (resolution, data 
    /// format, etc). The returned class can be queried for all of the current
    ///  image file's statistics.
    /// @{
    const IMG_Stat	&getStat() const;
	  IMG_Stat	&getStat();
    /// @}

    /// Get additional image information about the file that is specific
    /// to a particular format. Some formats do not have additional information.
    /// The information returned is human-readable in 'text'.
    virtual void	 getAdditionalInfo(UT_String &text);

    /// returns the value of an image option passed in through IMG_FileParms.
    /// Image options are specific to formats. Returns NULL if the option
    /// does not exist on the format.
    const char		*getOption(const char *name) const;

    /// Access to the list of file options this file was saved with.
    /// @{
    int			 getNumOptions() const;
    const char		*getOptionName(int i) const;
    const char		*getOptionValue(int i) const;
    /// @}

    /// @brief read a single scanline.
    /// read() returns a pointer to the scanline data
    /// in an internal buffer. Do not delete it.
    const void		*read(int scan, const IMG_Plane *from_plane = nullptr);

    /// @brief Reads the scanline into a buffer that you supply.
    /// The buffer passed must be big enough to fit the plane passed.
    /// getStat().bytesPerPlaneScan() will return the proper size.
    /// Returns 0 on failure.
    bool		 readIntoBuffer(int scan, void *buffer,
					const IMG_Plane *from_plane = nullptr);

    /// @brief Write a scanline to the file. 
    /// You don't need to write scanlines  in order, nor do you need to write 
    /// planes in order; the data may be cached however. Returns 0 if
    /// there was an error during the write. You should immediately stop
    /// writing if a failure occurred (many formats will behave unpredictably
    /// otherwise).
    bool		 write(int scan, const void *data,
			       const IMG_Plane *to_plane = nullptr);

    /// allocates a buffer large enough to hold the biggest plane's scanline
    /// in the file. Free this buffer with free(). (This will not hold all
    /// the planes' scanlines, only one at a time!)
    void *		 allocScanlineBuffer() const;

    /// @deprecated
    /// The following methods read/write a whole raster.  Unless the scanline
    /// increment is passed in, the stat structure will be used to determine the
    /// scanline size.  Beware if you're using translators...
    /// Use readImages() and writeImages() instead.
    ///@{
    bool		 readImage(void *data, int flip=0, int scaninc=0);
    /// @deprecated
    bool		 writeImage(void *data, int flip=0, int scaninc=0);
    ///@}

    /// @brief Reads all the image data into a list of PXL_Rasters.
    /// reads the image data into a a series of rasters. The array should be
    /// empty, as this method creates and fills the rasters for you.
    ///
    /// Note: You must delete the rasters returned by this method.
    bool		 readImages(UT_Array<PXL_Raster *> &images,
				    const char *scope = nullptr);

    /// @{
    /// @brief Writes all PXL_Rasters to the image format.
    /// This method takes a raster per IMG_Plane in the stat. The
    /// data format, color model and res of the raster must match the
    /// corresponding IMG_Plane. If the format is not a deep raster format,
    /// not all images may be written.
    /// if 'freerasters' is true, it will free the rasters for you.
    bool		 writeImages(const UT_Array<const PXL_Raster *> &imgs);
    bool		 writeImages(UT_Array<PXL_Raster *> &images,
				    bool freerasters = false);
    /// @}

    /// @deprecated
    /// For image files that contain more than one image, such as a
    /// movie file, jump to a particular frame in the movie and reset 
    /// for reading the scanlines of the new frame
    virtual bool	 jumpToFrame(int frame);

    /// When writing an image, call if interrupted or you encounter an
    /// non-image-file error and cannot complete the file write.
    virtual void	 abortWrite();
    
    ///@{
    /// orientation of the current file or format. Some formats support multiple
    /// orientations specified in the image files themselves, others simply
    /// have a fixed orientation. Call these methods rather than querying the
    /// orientation from the format, in case the format supports multiple 
    /// orientations.
    virtual int		 isTopFirst() const;
    /// If 0, scanlines are right-to-left. Default is 1 (left-to-right).
    virtual int		 isLeftFirst() const;
    ///@}

    /// call this if you won't be reading the scanlines in order, or if
    /// you'll be reading all the scanlines for a single plane first.
    virtual void	 randomReadAccessRequired();
    /// @private
    bool		 randomReadsForced() const
			 { return myForceRandomReads; }

    /// @{
    /// This is a tile interface for reading files. To use it, you must pass an
    /// IMG_FileParms object to open() with IMG_FileParms::useTileInterface()
    /// called. Otherwise, this will just return false. The x-y coords of the 
    ///tile is in pixels.
    virtual bool	 readTile(const UT_InclusiveRect &tile, void *data,
				  const IMG_Plane *plane=0);
    
    /// this is a tile interface for writing files. To use it, you must pass an
    /// IMG_FileParms object to open() with IMG_FileParms::useTileInterface()
    /// called. Otherwise, this will just return false. The x-y coords of the 
    ///tile is in pixels.
    virtual bool	 writeTile(const UT_InclusiveRect &tile,
				   const void *data,
				   const IMG_Plane *plane=0);
    /// @}

    /// When writing an image, the checkpoint() method can be used to
    /// checkpoint data (see TIFFCheckpointDirectory() for an example).
    /// Generally, this writes the image data currently passed and this results
    /// in a usable, partially written file.
    /// By default, nothing is done.
    virtual void	 checkpoint();

    /// This method converts the "standard" bit depth option to an IMG_DataType.
    /// This is used by the -b option in mantra as well as many other places.
    /// When dealing with floating point arguments, there's an optional flag to
    /// "denormalize" the floating point values when writing.  This basically
    /// removes tiny precision errors.  It's up to the user to deal with the
    /// denormalization.
    ///	- char, byte, 8	= uint8
    ///	- short, 16	= uint16
    ///	- int		= uint32
    ///	- FLOAT		= fpreal32 (denormalize=true)
    ///	- HALF		= fpreal16 (denormalize=true)
    ///	- float, 32	= fpreal32
    ///	- half		= fpreal16
    static IMG_DataType		mapStorageOption(const char *option,
					    bool *denormalize_flag = nullptr);

    /// Turns error reporting on or off. Returns the previous setting.
    /// Errors are reported to the error manager, which will either show up
    /// in the node information or the Houdini status bar (if not loaded within
    /// a node).
    ///
    static int		 doErrors( int newDoErrors );

    bool		 hasError() const
			 { return getBaseFile() ? (getBaseFile()->hasError() ||
						   myErrorFlag) :myErrorFlag; }

    /// Returns information about the alpha channel if the IMG_FileParms passed
    /// to open() had detectAlphaDetails() specified. 
    virtual IMG_FileAlphaInfo getAlphaInfo(float &) const
			      { return IMG_ALPHA_VARYING; }
    
    /// returns the raw file at the top of the filter stack.
    IMG_File		*getUnfilteredFile();
    const IMG_File	*getUnfilteredFile() const;
    virtual IMG_File	*getBaseFile() const { return 0; }

    /// Copy the texture image options from another IMG_File
    bool		copyImageTextureOptions(const IMG_File &src,
				bool clear_existing);

    /// Some image formats support "texture" options.  Formats which do not
    /// support options should return a NULL pointer.
    /// This method calls @c getImageTextureOptions() by default
    virtual UT_SharedPtr<UT_Options>	imageTextureOptions() const;

    /// This method is deprecated.  Please implement @c imageTextureOptions()
    SYS_DEPRECATED(12.0) virtual const UT_Options *getImageTextureOptions() const;

    /// For image formats which support texture options, this will clear out
    /// all texture options.
    virtual void	clearImageTextureOptions();

    /// For image formats which support texture options, merge the UT_Options
    /// into the current options.
    virtual bool	setImageTextureOptions(const UT_Options &options);

    /// When being created through a tile device (i.e. rendering, etc.), the
    /// writer may send additional information to the file writer after the
    /// image has been created.
    virtual void	setWriteTag(const char *tagname,
				    int num_values,
				    const char *const *values);

    /// Read the image options (@see getOption()) to extract the world to
    /// camera transform.  If the data isn't available, this method returns
    /// false.  This will likely only work when the image is being created from
    /// mantra.
    bool	getWorldToCamera(UT_Matrix4D &xform) const;

    /// Read the image options (@see getOption()) to extract the camera to NDC
    /// transform.  If the data isn't available in the options, this method
    /// returns false.  This will likely only work when the image is being
    /// created from mantra.
    /// If the @c fit_z option is true, the near/far range will be set to
    /// (0,1).  This will not be possible if the "camera:clip" setting wasn't
    /// passed in the options.
    bool	getCameraToNDC(UT_Matrix4D &xform, int xres, int yres, bool
		    fit_z = true) const;
    /// Read the image options (@see getOption()) to extract the world to NDC
    /// transform.  If the data isn't available in the options, this method
    /// returns false.  This will likely only work when the image is being
    /// created from mantra.
    /// If the @c fit_z option is true, the near/far range will be set to
    /// (0,1).  This will not be possible if the "camera:clip" setting wasn't
    /// passed in the options.
    bool	getWorldToNDC(UT_Matrix4D &xform, int xres, int yres, bool
		    fit_z = true) const;


    // API for raw deep pixel access.  Methods will fail if the image is not
    // a deep pixel image. The deep pixel functions can be accessed from
    // multiple threads simultaneously.

    // Returns the number of deep pixel samples for the given pixel location.
    // Returns -1 if the sample count cannot be read, or if the image is not
    // a deep pixel image.
    virtual int		 getDeepPixelSamples(int x, int y);

    // Read the deep pixel samples for the given plane. The size of the data
    // array should be at least the same size as the number of samples returned
    // by getDeepPixelSamples multiplied by the component count of the plane
    // given by IMG_Plane::getComponentCount.
    // Returns false if the data cannot be read, or if the image is not a deep
    // pixel image.
    virtual bool	 getDeepPixelPlaneData(int x, int y,
                	                       const IMG_Plane &plane,
                	                       float *data);

    // Read the deep pixel samples for all components at a given pixel location.
    // The length of the pointer array should at least be the same as the
    // number of samples returned from getDeepPixelSamples(). The size of
    // each array pointed to should be at least the same size as the number
    // of components returned from getDeepPixelComponentCount().
    // Returns false if the data cannot be read, of if the image is not a deep
    // pixel image.
    virtual bool	 getDeepPixelData(int x, int y, float * const *data);

    /// Read a deep pixel
    virtual bool	 readDeepPixelData(int x, int y,
					PXL_DeepSampleList &pixel);

    /// @{
    /// A method to write deep pixel data for all channels, at a given pixel
    /// location. The function should be given an array of pointers to float
    /// arrays, where each float array contains the value of all image
    /// components for a given sample level.
    ///
    /// Returns false if the data cannot be written, or if the image is not
    /// a deep pixel image.
    virtual bool	 writeDeepPixelData(int x, int y,
					    const PXL_DeepSampleListPtr &pixel);
    /// @}


    /// users of IMG_File can select only certain planes to read using
    /// IMG_FileParms::selectPlane..(). Call selectPlanesToRead() first,
    /// usually at the end of open(), and then call isPlaneSelected() to
    /// determine if the plane was selected for read.
    /// @{
    void		 selectPlanesToRead();
    bool		 isPlaneSelected(const IMG_Plane &plane) const;
    /// @}

protected:
    IMG_File();		/// Only called by format class

    // If the check_magic flag is 0, then the first 4 bytes of the file have
    // alread.  They are passed in as the magic number for verification.
    virtual int		 open();
    virtual int		 openPostMagic();
    virtual int		 openFile(const char *filename);

    virtual int		 readScanline(int scanline, void *data) = 0;
    virtual const void  *readPlaneScanline(int y, const IMG_Plane &pi);

    // Create an image
    virtual int		 createFile(const char *filename,const IMG_Stat &stat);
    virtual int		 create(const IMG_Stat &stat);
    
    virtual int		 writeScanline(int scanline, const void *data) = 0;
    virtual int		 writePlaneScanline(const void *data, int y,
					    const IMG_Plane &pi);

    virtual int		 closeFile() = 0;

    /// Run after the output stream has been closed.
    /// Be careful, since some things have been destructed.
    virtual void         postCloseAction();

    // add any image flips, scales, data translations, etc. Returns the
    // end of the filter chain.
    // 
    virtual bool	 isPassThrough() const { return false; }
    
    IMG_File		*makeReadFilters(const IMG_FileParms &parms);
    IMG_File		*makeWriteFilters(const IMG_FileParms &parms);


    // if you override this, you must call this class's version.
    virtual void	 computeCommonData();
    
    void		*getScanlineBuffer(int y);
    void		*getPlaneBuffer(int y, const IMG_Plane &pi);
    
    virtual void	 finishedScanline(int scan);
    
    static void		 imgError( int code, const char *msg = nullptr );
    static void		 imgWarning( int code, const char *msg = nullptr );
    static void		 setOptions(const IMG_FileParms *options,
				    IMG_File *file);

    /// @{
    /// Set option to control behaviour of the file.  For example, the
    /// compression level when creating the image.
    int		setOption(const char *token, const char *value);
    int		setOption(const char *token, fpreal64 val);
    int		setOption(const char *token, int64 val);
    int		setOption(const char *option, int value)
			{ return setOption(option, (int64)value); }
    /// @}

    bool		 canWriteStatWithoutConversion() const;

    // Access to input and output streams
    //	peekByte() will peek at the next input byte.  -1 returned on EOF
    //	readSomeBytes() will read data into the buffer specified.  The number
    //		of bytes actually read is returned.
    //	writeSomeBytes() will write data from the buffer.  The number
    //		of bytes actually written is returned.
    //	flushBytes() will flush the output stream
    //	readBytes() is a convenience method when all data needs to be read
    //	writeBytes() is a convenience method when all data needs to be written
    // 
    int			 peekByte();
    int			 readSomeBytes(void *buffer, int size);
    int			 writeSomeBytes(const void *buffer, int size);
    bool		 flushBytes();

    bool		 readBytes(void *buffer, int size)
			 { return readSomeBytes(buffer, size) == size; }
    bool		 writeBytes(const void *buffer, int size)
			 { return writeSomeBytes(buffer, size) == size; }


    // For the odd format which requires data to be read as ASCII lines, this
    // method exists.
    bool		 readAsciiLine(UT_WorkBuffer &wbuf);

    // Seeks the stream to the start + offset.  Returns true if successful
    // and false otherwise.
    bool		 seekFromBeginning(int64 offset);
    bool		 seekFromCurrent(int64 offset);
    bool		 seekFromEnd(int64 offset);
    int64		 tellCurrentPosition();

    // Copies our input stream to the given stream.
    void		 copyInputToStream(std::ostream &os);

    // Deletes our input stream.
    void		 deleteStream();

    /////////////////////////////////////////////////////////////////////////
    // Do not use these methods.  They are for special cases only.
    UT_IStream		*getUTIStream() { return myIStream; }
    void		 setIStream(UT_IStream *is)	{ myIStream = is; }
    /////////////////////////////////////////////////////////////////////////

    // Image description.
    IMG_Stat		 myStat;
    bool		 myReadMode;
    std::ostream	*myOS;
    int64		 myStreamStartPos;
    int64		 myStreamLength;
    unsigned		 myCreateIStream:1,
		 	 myCreateOStream:1,
			 myForceRandomReads:1,
			 myHasRandomReads:1,
			 myFileOpen:1,
			 myErrorFlag:1,
			 myContinueOnErrorFlag:1;
    
private:
    class img_ScanProgress
    {
    public:
			 img_ScanProgress()
			     : myNumPlanes(0)
			 {
			 }
	void		 init(const IMG_Stat &stat);
	int64		 getMemoryUsage(bool inclusive) const;
	void		 processed(int scan, const IMG_Plane *plane);
	void		 processAll(int scan);
	bool		 isComplete(int scan, const IMG_Plane *plane) const;
	bool		 isComplete(int scan) const;
	int		 remaining(int scan) const;
	int		 numScans() const { return myCounts.entries(); }
    private:
	int		 bitIndex(int scan, int plane) const;
    private:
	int		 myNumPlanes;
	UT_BitArray	 myProcessed;
	UT_ValArray<int> myCounts;
    };

private:
    // Initialize tags to default values
    void		 initializeTags();
    void		 makeScanlineBuffers();
    void		 deallocScanlineBuffer(int scan);
    const void		*returnErrorScan(int scan, const IMG_Plane &pi);

    IMG_File		*makeDataConversion(const IMG_FileParms &parms,
					    IMG_File *file);
    IMG_File		*makeFlip(const IMG_FileParms &parms,
				  IMG_File *file);
    

    const IMG_Format	*myFormat;
    IMG_FileTagList	*myTagList;
    void		*myScanlineBuffer;
    int			 myCurrentImageScanline;

    img_ScanProgress	 myScanProgress;
    UT_ValArray<void *>	 myImageScanline;
    UT_BitArray		 mySelectPlane;


    FS_Reader		*myReader;
    FS_Writer		*myWriter;
    UT_IStream		*myIStream;

    // only IMG_Format can call initializeTags().
    friend class IMG_Format;
    friend class IMG_FileFilter;
    friend class IMG_GZip;
    friend class IMG_FileBuffer;
    friend class MV_ReadPRISMS;
};

#endif

