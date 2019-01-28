/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_IOJSON.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_IOJSON__
#define __GA_IOJSON__

#include "GA_API.h"
#include "GA_IO.h"

#include <SYS/SYS_Types.h>

#include <iosfwd>


class GA_Detail;
class GA_LoadMap;
class GA_SaveMap;
class GA_Stat;

class UT_IStream;
class UT_JSONParser;
class UT_JSONWriter;
class UT_Options;
class UT_StringArray;
class GA_LoadOptions;
class GA_SaveOptions;


/// @brief JSON I/O of geometry objects
class GA_API GA_IOJSON : public GA_IO 
{
public:
    GA_IOJSON();
    virtual ~GA_IOJSON();

    /// @{
    /// Methods required to implement GA_IO interface
    virtual const char	*getToken() const override;
    virtual const char	*getLabel() const override;
    virtual bool	 checkFilename(const char *filename,
				const GA_SaveOptions *opts) const override;
    virtual bool	 checkFilename(const char *filename,
				const GA_LoadOptions *opts) const override;
    virtual bool	 checkByteMagic(uint8 first_byte,
				const GA_LoadOptions *opts) const override;
    virtual bool	 statStream(UT_IStream &is,
				GA_Stat &stat, uint stat_level) const override;
    virtual bool	 readStream(GA_Detail &g,
				UT_IStream &is,
				const GA_LoadOptions *opts,
				UT_StringArray *errors) const override;
    virtual bool	 writeFile(const GA_Detail &g,
				const char *filename,
				const GA_SaveOptions *opts,
				UT_StringArray *errors) const override;
    virtual bool	 writeStream(const GA_Detail &g,
				std::ostream &os,
				bool binary,
				const GA_SaveOptions *opts,
				UT_StringArray *errors) const override;
    /// @}

    /// Stat geometry from a JSON stream
    bool		statJSON(UT_JSONParser &p,
				GA_Stat &stat,
				uint level) const;

    /// Read geometry from a JSON stream
    bool		readJSON(GA_Detail &g,
				UT_JSONParser &p,
				const GA_LoadOptions *options) const;
    /// Write geometry to a JSON stream
    bool		writeJSON(const GA_Detail &g,
				UT_JSONWriter &w,
				const GA_SaveOptions *options) const;

    /// Check whether filename uses BLOSC compression
    static bool	isScExtension(const char *filename);
    /// Check whether filename uses GZIP compression
    static bool	isGzExtension(const char *filename);

protected:
    /// @{
    /// Configuration from GA_IO
    virtual bool	readCheckStdin() const override { return true; }
    virtual bool	readCheckCompressed() const override { return true; }
    virtual bool	writeCheckStdout() const override { return true; }
    /// @}


    /// When the JSON loader hits a token it doesn't understand, it will call
    /// this method.  This allows sub-classes to load extra fields.
    virtual bool	jsonLoadExtra(GA_Detail &gdp,
				UT_JSONParser &p,
				const char *token,
				const GA_LoadMap &loadmap) const;

    /// The jsonSaveExtra method is called during writing.  This allows
    /// sub-classes to save additional information into the JSON stream.
    virtual bool	jsonSaveExtra(const GA_Detail &gdp,
				UT_JSONWriter &w,
				const GA_SaveMap &savemap) const;

    /// When the JSON loader finishes, it will call this method.  This allows
    /// sub-classes to perform any post-load processing.
    virtual bool	finishLoad(GA_Detail &gdp,
				const GA_LoadMap &) const;
};

#endif
