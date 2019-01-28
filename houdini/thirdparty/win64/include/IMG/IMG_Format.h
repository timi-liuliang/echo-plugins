/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_Format.h ( IMG Library, C++)
 *
 * COMMENTS:	Declaration of a file format.  This class is responsible
 */

#ifndef __IMG_Format__
#define __IMG_Format__

#include "IMG_API.h"
#include "IMG_FileTypes.h"
#include <SYS/SYS_Types.h>


class UT_IStream;
class UT_String;
class IMG_File;
class IMG_Stat;
class IMG_FileTag;
class IMG_FileOptionList;
class IMG_FileTagList;

extern "C" {
    // DSO's should have this function declared.  All it has to do is create a
    // format of the new type.  This will automatically install the format.
    // The argument passed in will be a null pointer and can be ignored.
    SYS_VISIBILITY_EXPORT void	newIMGFormat(void *);
};

/// Description of a specific image file format, describing the
/// characteristics and capabilities of a specific image file format.
/// It can be used to query the capabilities of that format (resolution limits,
/// data formats supported, image orientation, etc). Image file-specific data
/// (such as resolution of a given file) can be found in IMG_Stat. @n
/// When writing a new data format, many of the virtuals will need to be 
/// overridden to properly describe its capabilities.
class IMG_API IMG_Format 
{
public:
	     IMG_Format();
    /// This is a version of the constructor that lets you avoid adding
    /// oneself to the list.  This is used by the pass throug filter and
    /// must be used if you are in multithreaded code as the underlying
    /// list is not locked.
    explicit IMG_Format(bool addtolist);
    virtual ~IMG_Format();

    // Types of access for image formats.
    enum AccessType
    {
	READ_ACCESS,
	WRITE_ACCESS
    };

    /// @{
    /// number of currently installed image formats
    static int			 getNFormats();
    /// Access to the list of currently installed image formats
    static const IMG_Format	*getFormat(int idx);
    /// @}

    /// @{
    /// Find a format given a filename and a stat - only extension matching. 
    /// Will only return formats that can read files (isReadable()). 
    static const IMG_Format	*findFormatReadable(const char *filename,
						    const IMG_Stat *stat = 0);
    /// Find a format given a filename and a stat - only extension matching. 
    /// Will only return formats that can write files (isWritable()). 
    static const IMG_Format	*findFormatWritable(const char *filename,
						    const IMG_Stat *stat = 0);
    /// @}
    static const IMG_Format	*findFormatByName(const char *format_name);
    static const IMG_Format	*findFormatByLabel(const char *format_label);
    // Find a format given a seekable istream.
    static const IMG_Format	*findFormatSeekable(UT_IStream &is);
    static const IMG_Format	*findFormatNonSeekable(UT_IStream &is);
    static const IMG_Format	*getStdoutFormat();
    IMG_File			*allocValidFile() const;

    /// @{
    /// All formats must have a unique name (like "JPEG"). 
    virtual const char	*getFormatName() const = 0;
    /// If getFormatLabel() is not overridden, the label will be the same as 
    /// the name.
    virtual const char	*getFormatLabel() const;
    /// @}

    /// This can be optionally overridden to return a short description of the
    /// format. By default, no description is given.
    virtual const char	*getFormatDescription() const;

    /// Flags whether the format is a pass through format like Flip or Scale.
    /// Specific image formats (like JPEG) are never pass-through.
    virtual bool	 isPassThrough() const { return false; }

    /// The default extension is used to determine whether the format should
    /// appear in the device/format menus.  It should not contain the '.'
    /// (i.e.  "tif" not ".tif")
    virtual const char	*getDefaultExtension() const;

    /// Describes how the format was loaded (i.e. DSO or internal formats)
    virtual const char	*getFormatLocation() const	{ return myLocation; }

    /// @{
    /// Methods to determine if this is one of our recognized files.
    ///	The extension is the first try.  If there are multiple matches,
    ///	then we resort to the magic number (when reading)
    virtual int		 checkExtension(const char *filename) const = 0;
    /// If possible, this magic numver checker will be called. This is
    /// used if the file is "seekable" (i.e. that we can rewind after checking)
    virtual int		 checkMagic(unsigned int) const;
    ///  If possible, this second magic number checker will be called.  This is
    ///  used if the file is "seekable" (i.e. that we can rewind after checking)
    virtual int		 checkMagicSeekable(UT_IStream &is) const;
    /// @}

    /// The device method gives a higher priority to device checks.  By default,
    /// this method returns 0 (no match).  This is used to allow things like
    /// a60:3.pic (which uses the abekas device).
    virtual int		 checkDevice(const char *) const;

    // ------------------------------------------------------------------
    // Image Format features
    
    /// Returns a bitfield of image types supported by this format. By default
    /// IMG_TYPE_2D is returned.
    virtual IMG_ImageType	 getSupportedImageTypes() const;

    /// Returns a bitfield of data types supported by this format.
    virtual IMG_DataType	 getSupportedTypes() const = 0;

    /// Returns a bitfield of supported color models. If IMG_CM_REVERSED is
    /// set, this format stores data in BGR/ABGR format.
    virtual IMG_ColorModel       getSupportedColorModels() const = 0;

    /// Maximum allowable resolution for the file. If a user attempts to write
    /// an image larger than this, the image will be scaled to this resolution.
    virtual void		 getMaxResolution(unsigned &x,
						  unsigned &y) const = 0;

    /// @{
    /// Some formats can be read and written in random order. Others require
    /// strict sequential order. If these methods return false, you should
    /// read and write scanlines in ascending order. When writing, they will
    /// be cached until they can be written sequentially. 
    virtual int			 isReadRandomAccess() const;
    /// If 0 is returned, the scanlines must be written in sequential order or
    /// they will be cached until all scanlines can be written sequentially.
    virtual int			 isWriteRandomAccess() const;
    /// @}

    /// vertical data orientation. Where (0,0) lies in the image 
    /// (top-left by default).
    virtual int			 isTopFirst() const;

    /// horizontal data orientation. Where (0,0) lies in the image 
    /// (top-left by default).
    virtual int			 isLeftFirst() const;

    /// @{
    /// Specifies if this format can read an image file. One of isReadable()
    /// or isWritable() (or both) must return true.
    virtual bool		 isReadable() const;
    /// Specifies if this format can write an image file. One of isReadable()
    /// or isWritable() (or both) must return true.
    virtual bool		 isWritable() const;
    //// @}

    // data organization.

    /// does this format support a data window or sub-area.
    virtual bool		 isDataWindowSupported() const;

    /// if true, this format only supports data windows contained within
    /// the image resolution (a crop region). 
    virtual bool		 isDataWindowCropOnly() const;

    /// does this format support saving whether the data outside the window
    /// is streaked from the window edges?
    virtual bool		 isDataWindowStreakSupported() const;

    /// does this format support deep rasters or multiple images?
    /// If not, only one plane of the color models that this format supports
    /// is allowed.
    virtual bool		 isDeepRasterSupported() const;
    
    /// does this deep raster allow RGBA in one plane (true) or split into
    /// 2 planes (RGB, A). (default false)
    virtual IMG_DeepRasterColor	 getDeepRasterRGBASupport() const;

    /// Specifies whether this format stores its data interleaved (RGBRGBRGB)
    /// or non-interleaved (RRRGGGBBB). If your format supports both, pick one.
    virtual bool		 isDataInterleaved() const;

    /// if true, planes can have different data formats & components. Otherwise,
    /// all planes must have the same data type and number of components.
    virtual bool		 canPlaneTypesDiffer() const;

    /// Returns a list of options that can be set on the format when reading or
    /// writing (like 'compression' or 'comment').
    virtual const IMG_FileOptionList	*getOptions() const;

    /// @private
    // Allocate default tags and their values
    void			 defaultTags(IMG_FileTagList &tags) const;

    /// Set an option globally for all formats. Use with extreme care.
    static void	setGlobalOption(const char *name, const char *value);
    /// Read the value of a global option. May return a NULL string.
    static void	getGlobalOption(const char *name, UT_String &value);

    /// Allows you to change an option on a specific format. This will change
    /// the option for all formats from this point on, whereas setGlobalOption
    /// would not. Returns false if the format or the option couldn't be found.
    static bool	setFormatOptionDefault(const char *format_name,
				       const char *format_option,
				       const char *defvalue);
    /// Returns the value of a format-specific option. Returns false if the 
    /// format or option does not exist.
    static bool 	getFormatOptionDefault(const char *format_name,
					       const char *format_option,
					       UT_String &defvalue);

    /// Matches a filename against a single extension
    static int	matchExtension(const char *filename, const char *ext);

    /// Matches a filename against a list of extensions (the last entry must be 
    /// NULL)
    static int	matchExtensions(const char *filename, const char *ext[]);

    /// Between these two formats, which should be preferred? returns either
    /// this or the format parameter.  This method is used to resolve 
    /// conflicts between formats when files are being created.
    virtual const IMG_Format	*resolvePriority(const IMG_Format *format) 
				    const;

    /// Some image formats have fixed color spaces, like JPEG (sRGB). The
    /// default is PXL_CS_UNKNOWN. For formats that can have colorspaces per
    /// plane, return PXL_CS_UNKNOWN and query the colorspace from IMG_Plane.
    /// formatColorSpaceGamma() is ignored unless the color space is
    /// PXL_CS_CUSTOM_GAMMA.
    /// @{
    virtual PXL_ColorSpace	formatColorSpace() const;
    virtual fpreal		formatColorSpaceGamma() const;

    /// @}

    /// Return true if the image format can store the colorspace
    virtual bool		formatStoresColorSpace() const;
    
    /// Returns a gamma correction factor when converting from a given color
    /// space to this image format's space. 'src_data_is_8bit' indicates whether
    /// data is fixed 8b, in which case it will often be converted to 2.2 gamma.
    /// If the dest_data_is_8bit is provided, it will use that flag for
    /// the destination 8bit type instead of the src_data_is_8bit.
    fpreal  adjustGammaForFormat(PXL_ColorSpace src_color_space,
				 fpreal		src_color_space_gamma,
				 bool		src_data_is_8bit,
				 bool	       *dest_data_is_8bit = NULL) const;
    
protected:
    // Returns 1 if the stat's are ok for the format specified.  For example,
    // if there's a fixed resolution for a format, the format can nullify the
    // validity of the format in the determination stage.  By default, we
    // simply check the image depth (i.e. number of images) and compare the max
    // resolution.
    virtual IMG_File	*createFile()	 const = 0;
    virtual int		 isFormatOk(const IMG_Stat &stat) const;
    void		 removeFormatFromList();

    static const IMG_Format	*findFormat(const char *filename,
					    const IMG_Stat *stat,
					    AccessType access);
    
    const IMG_FileTagList	*getGlobalTags();

private:
    static int		 getNFormatsCS();
    static void		 installBasics();
    char		*myLocation;
};

#endif

