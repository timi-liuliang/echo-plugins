/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_IO.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __GA_IO__
#define __GA_IO__

#include "GA_API.h"
#include "GA_Types.h"

#include <SYS/SYS_Types.h>

#include <iosfwd>


class GA_Detail;
class GA_LoadMap;
class GA_SaveMap;
class GA_Stat;

class FS_Reader;
class FS_Writer;
class UT_Options;
class UT_IStream;
class UT_JSONParser;
class UT_JSONWriter;
class UT_StringArray;
class UT_SCFReader;
class GA_SaveOptions;
class GA_LoadOptions;


/// @brief Class which defines an I/O interface to save/load geometry
///
/// Loading a GA_Detail from a disk file goes through the following process
/// @code
///  GA_Detail::load(const char *filename, const GA_LoadOptions *opts)
///	foreach (IO) {
///	    if (IO->checkFilename(filename, opts) &&
///		IO->readFile(*this, filename, opts))
///	    {
///		return true;
///	    }
///	}
///	GA_IO::io_ReadHandle	io(IO, filename, errors);
///	if (io.isValid())
///	    return load(*io.getStream(), options);
///	return false;
/// @endcode
/// Loading a GA_Detail from a stream goes through a similar process
/// @code
///  GA_Detail::load(UT_IStream &is, const GA_LoadOptions *opts):
///	uint8	magic = is.peek();
///	foreach (IO) {
///	    if (IO->checkByteMagic(magic) && IO->readFile(*this, is, opts))
///		return true;
///	}
///	return false;
/// @endcode
///
/// Saving geometry works similarly: @code
///   GA_Detail::save(const char *filename, const GA_SaveOptions *opts):
///	foreach (IO) {
///	    if (IO->checkFilename(filename, opts))
///		return IO->writeFile(*this, filename, opts);
///	}
///	return false;
///
///   GA_Detail::save(ostream &os, const GA_SaveOptions *opts) const
///	IO = selectIO(opts);
///	return IO->writeStream(*this, os, opts);
///
class GA_API GA_IO
{
public:
    GA_IO();
    virtual ~GA_IO();

    /// @{
    /// Return identifiers for the file format.
    virtual const char		*getToken() const = 0;
    virtual const char		*getLabel() const = 0;
    /// @}

    /// @{
    /// Check whether the filename could be handled by this IO handler.
    /// This is called on both load and save operations.
    virtual bool	checkFilename(const char *filename,
				    const GA_SaveOptions *opts) const = 0;
    virtual bool	checkFilename(const char *filename,
				    const GA_LoadOptions *opts) const = 0;
    /// @}

    /// When loading an un-named stream, check whether the first byte of the
    /// stream is an indicator of the file format.  This is only called for
    /// loading.
    virtual bool	checkByteMagic(uint8 first_byte,
				    const GA_LoadOptions *opts) const = 0;

    /// Stat a disk file without necessarily loading the entire geometry
    virtual bool	statFile(const char *filename,
				GA_Stat &stat, uint stat_level) const;
    /// Stat a stream
    virtual bool	statStream(UT_IStream &is,
				GA_Stat &stat, uint stat_level) const = 0;

    /// Load geometry from a disk file.  The default behaviour for this method
    /// is: @code
    ///	   GA_IO::io_ReadHandle	io(filename, true);
    ///	   return (io.isValid()) ? readStream(g, *io.getStream(), opts) : false;
    /// @endcode
    virtual bool	readFile(GA_Detail &g,
				const char *filename,
				const GA_LoadOptions *opts,
				UT_StringArray *errors) const;

    /// Load geometry from an input stream.
    virtual bool	readStream(GA_Detail &g,
				UT_IStream &is,
				const GA_LoadOptions *opts,
				UT_StringArray *errors) const = 0;

    /// Save geometry to a disk file.  The default behaviour for this method is:
    /// @code
    ///	   GA_IO::io_WriteHandle	io(filename, true);
    ///	   return io.isValid() ? writeStream(g, *io.getStream(), opts) : false;
    /// @endcode
    virtual bool	writeFile(const GA_Detail &g,
				const char *filename,
				const GA_SaveOptions *opts,
				UT_StringArray *errors) const;

    /// Write geometry to an output stream
    virtual bool	writeStream(const GA_Detail &g,
				std::ostream &os,
				bool binary,
				const GA_SaveOptions *opts,
				UT_StringArray *errors) const = 0;

    /// Create and initialize a private info attribute, stored as a JSON
    /// encoded UT_Options object in a string attribute.
    static bool		createInfoAttribute(
				    GA_Detail &g, const char *attrib_name,
				    const char *format_token,
				    const UT_Options *from_file);

    /// Extract a UT_Options object from an attribute storing a JSON encoding.
    static bool		retrieveInfo(const GA_Detail &g,
				     const char *attrib_name,
				     UT_Options &info);

    /// Destroy the private info attribute created by createInfoAttribute().
    static bool		destroyInfoAttribute(GA_Detail &g,
					     const char *attrib_name);

protected:
    /// If the io_ReadHandle should scan for filenames starting with "stdin",
    /// this method should return true.
    virtual bool	readCheckStdin() const		{ return false; }
    /// If the io_ReadHandle should scan for filenames ending with
    /// extensions that indicate it is compressed then this method
    /// should return true
    virtual bool	readCheckCompressed() const = 0;
    /// If the io_WriteHandle should scan for filenames starting with "stdout",
    /// this method should return true.
    virtual bool	writeCheckStdout() const	{ return false; }

    bool		isStdout(const char *filename) const;

    /// Class to hold information about a read stream.  This class is used
    /// solely to hold the istream handles and clean up properly.
    /// Destruction of the class will close the stream
    /// @note The io_ReadHandle will automatically search the
    ///   HOUDINI_GEOMETRY_PATH when scanning for disk files.
    /// @note If the filename ends in .gz, or .sc the appropriate decompression
    /// stream wil be allocated
    class GA_API io_ReadHandle 
    {
    public:
	io_ReadHandle()
	    : myStream(nullptr)
	    , myBaseStream(nullptr)
	    , myReader(nullptr)
            , myIsStdin(false)
	    , myScfReader(nullptr)
	    , myIsScf(false)
	{}
	io_ReadHandle(const GA_IO &io,
			const char *filename,
			UT_StringArray *errors)
	    : myStream(nullptr)
	    , myBaseStream(nullptr)
	    , myReader(nullptr)
            , myIsStdin(false)
	    , myScfReader(nullptr)
	    , myIsScf(false)
	{
	    open(io, filename, errors);
	}
	~io_ReadHandle()
	{
	    close();
	}

	/// Test whether the read handle is valid
	bool		 isValid() const	{ return myStream != nullptr; }
        /// Test whether the read handle reads from stdin
        bool             isStdin() const        { return myIsStdin; }
	/// Get the UT_IStream associated with the handle
	UT_IStream	*getStream() const	{ return myStream; }
	/// Open a file for reading.  If @c check_stdin is true and the
	/// filename starts with "stdin", the stream returned will be stdin.
	/// If the open fails, errors will be appended to the error array.
	UT_IStream	*open(const GA_IO &io, const char *filename,
				UT_StringArray *errors);
	/// Close the stream
	void		 close();

	/// Provide access to the FS_Reader (this may be nullptr)
	const FS_Reader	*reader() const { return myReader; }
    private:
	UT_IStream	*myStream;
	UT_IStream	*myBaseStream;
	FS_Reader	*myReader;
        bool             myIsStdin;
	UT_SCFReader	*myScfReader;
	bool		 myIsScf;
    };

    /// Class to hold information about a write stream.  This class is used
    /// solely to hold the ostream handles and clean up properly.
    /// Destruction of the class will close the stream.
    class GA_API io_WriteHandle 
    {
    public:
	 io_WriteHandle()
	    : myStream(nullptr)
	    , myWriter(nullptr)
	{
	}
	 io_WriteHandle(const GA_IO &io, const char *filename,
			UT_StringArray *errors)
	    : myStream(nullptr)
	    , myWriter(nullptr)
	{
	    open(io, filename, errors);
	}
	~io_WriteHandle()
	{
	    close();
	}

	/// Check if the stream is valid
	bool		 isValid() const { return myStream != nullptr; }
	/// Get the output stream
	std::ostream	*getStream() const { return myStream; }
	/// Open a file for reading.  If @c check_stdout is true and the
	/// filename starts with "stdout", the stream returned will be stdout.
	/// If the open fails, errors will be appended to the error array.
	std::ostream	*open(const GA_IO &io, const char *filename,
				UT_StringArray *errors);
	/// Close the stream
	void		 close();
    private:
	std::ostream	*myStream;
	FS_Writer	*myWriter;
    };

    /// Provide access to private portions of the GA_Detail class for loading
    /// Tell the detail that the loading will be adding @c npoints, @c nvertex
    /// and @c nprimitive primitives.  This will adjust the load map so that
    /// the proper index/offset mappings can be created.  It also adjusts the
    /// index maps.
    /// The method will return false if there was an error.
    bool		setLoadCounts(GA_Detail &gdp,
					GA_Size npoints,
					GA_Size nvertex,
					GA_Size nprimitive,
					GA_LoadMap &loadmap) const;

    /// @{
    /// Save and load the vertex point-reference array using JSON
    /// @warning: After calling jsonLoadPointReference() you must rebuild the
    /// topology attributes.
    bool	jsonSavePointReference(const GA_Detail &gdp,
					UT_JSONWriter &w,
					const GA_SaveMap &savemap) const;
    bool	jsonLoadPointReference(GA_Detail &gdp,
					UT_JSONParser &p,
					const GA_LoadMap &loadmap) const;
    /// @}

    /// After the geometry has been loaded, the sub-class may need to call the
    /// method to rebuild the topology attributes on the detail.  The caller
    /// may also request that full topology attributes are created after load.
    ///
    /// If the method fails, it's because the geometry failed the sanity check
    /// on topology.  In this case, the geometry is cleared.  The user should
    /// probably add an error in this case.
    //
    /// @warning This method is for advanced users.
    bool		rebuildTopology(GA_Detail &gdp,
					bool create_attributes=false) const;
};

#endif

