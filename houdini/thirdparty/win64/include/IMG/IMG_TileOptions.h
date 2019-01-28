/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_TileOptions.h ( IMG Library, C++)
 *
 * COMMENTS:
 */

#ifndef __IMG_TileOptions__
#define __IMG_TileOptions__

#include "IMG_API.h"
#include "IMG_FileTypes.h"
#include "IMG_Format.h"
#include <UT/UT_Options.h>
#include <UT/UT_ValArray.h>

class UT_StringArray;
class UT_JSONWriter;

class IMG_API IMG_TileOptions
{
public:
    /// If the format is a null pointer, it will be inferred from the filename
	     IMG_TileOptions() = default;
    virtual ~IMG_TileOptions() = default;

    /// Create a new copy of myself
    virtual IMG_TileOptions	*clone() const;

    void	setPlaneInfo(const UT_StringHolder &filename,
			const UT_StringHolder &channel,
			const UT_StringHolder &format,
			IMG_DataType dtype = IMG_DT_ANY,
			IMG_ColorModel cmodel = IMG_CM_ANY,
			IMG_TypeInfo tinfo = IMG_TI_COLOR);
    void	setPlaneInfo(const char *filename,
			const char *channel,
			const char *format,
			IMG_DataType dtype = IMG_DT_ANY,
			IMG_ColorModel cmodel = IMG_CM_ANY,
			IMG_TypeInfo tinfo = IMG_TI_COLOR)
    {
	setPlaneInfo(UT_StringHolder(filename),
			UT_StringHolder(channel),
			UT_StringHolder(format),
			dtype, cmodel, tinfo);
    }

    fpreal	gamma() const		{ return myGamma; }
    void	setGamma(fpreal gamma)	{ myGamma = gamma; }
    PXL_ColorSpace	colorSpace() const;

    // Change the filename extension to match the format
    bool	changeFileExtension();

    /// There are sevaral ways to set format options:
    ///   1)  A sting (which is parsed into token/value pairs)
    ///   2)  A set of token/value pairs specified as a linear array
    void	clearOptions();
    void	setFormatOptions(const char *string);
    void	setFormatOptions(int argc, const char *const argv[]);
    void	setFormatOptions(const UT_StringArray &options);
    void	setFormatOption(const char *token, const char *value);
    void	setFormatOptions(const UT_Options &options);

    /// Get the IMG_Format associated with these options.  This will possibly
    /// return a null ptr.
    virtual const IMG_Format	*getFormat(IMG_Format::AccessType 
				    access=IMG_Format::READ_ACCESS) const;

    const char			*getFilename() const;
    const char			*getChannel() const;
    const char			*getFormatName() const;
    IMG_DataType		 getDataType() const;
    IMG_ColorModel		 getColorModel() const;
    IMG_TypeInfo		 getTypeInfo() const;
    const UT_Options		&getLocalOptions() const;

    /// Getting the option will return true/false based on whether the option
    /// was defined.  If the option was not defined, the value will not be
    /// touched (i.e. it will remain unchanged).
    virtual bool	getOption(const char *token, UT_String &value) const;

    /// Texture options are only output for specific file formats.  These
    /// texture options are used to pass information from the calling
    /// application to the device (where required).
    /// The application can write directly to the UT_Options, which will
    /// then be output in the file.
    virtual void	setTextureOptions(UT_Options &options);

    /// Pack all options into a single string.  Returns the number of options
    /// packed.
    virtual int		packOptionsString(UT_String &result) const;

    /// @{
    /// Dump options
    void	dump(UT_JSONWriter &w) const;
    void	dump() const;
    /// @}

protected:
    // Copy from the option source
    void		copy(const IMG_TileOptions &src);

    UT_StringHolder	myFilename;
    UT_StringHolder	myChannel;
    UT_StringHolder	myFormatName;
    UT_Options		myOptions;
    IMG_DataType	myDataType = IMG_DT_ANY;
    IMG_ColorModel	myColorModel = IMG_CM_ANY;
    IMG_TypeInfo	myTypeInfo = IMG_TI_UNDEFINED;
    fpreal		myGamma = 1;
};

class IMG_API IMG_TileOptionList
{
public:
     IMG_TileOptionList();
    ~IMG_TileOptionList();

    /// WARNING:  Using this class will delete all the contents, so you must
    /// append clones if you're creating single lists from existing options.
    int		entries() const			{ return myOptions.entries(); }
    void	append(IMG_TileOptions *opt)	{ myOptions.append(opt); }
    IMG_TileOptions	*operator()(int i)	{ return myOptions(i); }

    /// @{
    /// Dump option list
    void	dump(UT_JSONWriter &w) const;
    void	dump() const;
    /// @}
private:
    UT_Array<IMG_TileOptions *>	myOptions;
};

#endif
