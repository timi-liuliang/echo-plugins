/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_IStreamDevice.h (FS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __FS_IStreamHelper__
#define __FS_IStreamHelper__

#include "FS_API.h"
#include <SYS/SYS_BoostStreams.h>
#include <UT/UT_Assert.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_IStream.h>
#include "FS_Reader.h"

/// This class is a wrapper to create a std::istream using FS to handle stream
/// I/O.
/// There are three objects involved when using this code.  The device, the
/// stream buffer and the stream itself.  You need code like: @code
///	auto reader = new FS_IStreamDevice(path);
///	if (!reader->isValid())
///	{
///	     delete reader;
///	     return false;
///	}
///	auto streambuf = new FS_IStreamDeviceBuffer(*reader);
///	auto stream = new std::istream(streambuf);
///	...
///	delete reader;
///	delete stream;
///	delete streambuf;
/// @endcode
class FS_API FS_IStreamDevice
{
public:
    using char_type = char;
    using stream_offset = bios::stream_offset;
    struct category
	: public bios::device_tag
	, public bios::seekable
	, public bios::closable_tag
    {
    };

    FS_IStreamDevice(const char *path, const UT_Options *opts=nullptr)
	: myReader(new FS_Reader(path, opts))
    {
	myStream = myReader->isGood() ? myReader->getStream() : nullptr;
    }
    FS_IStreamDevice(UT_IStream *stream)
    {
	myStream = stream;
    }
    ~FS_IStreamDevice() {}

    /// Test whether stream is valid
    bool	isValid() const	{ return myStream != nullptr; }

    /// Writing always fails
    std::streamsize	write(const char *, std::streamsize) { return -1; }
    /// Close the input stream
    void		close()
    {
	if (myReader)
	    myReader->close();
	myStream = nullptr;
    }
    /// Try to read data from the stream
    std::streamsize	read(char *buffer, std::streamsize n)
    {
	return myStream ? myStream->bread(buffer, n) : -1;
    }
    /// Putback a character
    bool		putback(char_type c)
    {
	return myStream ? myStream->ungetc() == 1 : false;
    }
    /// Seek in the stream
    stream_offset	seek(stream_offset off, std::ios_base::seekdir way)
    {
	bool	ok = false;
	if (myStream)
	{
	    if (way == std::ios_base::beg)
	    {
		ok = myStream->seekg(off, UT_IStream::UT_SEEK_BEG);
	    }
	    else if (way == std::ios_base::cur)
	    {
		ok = myStream->seekg(off, UT_IStream::UT_SEEK_CUR);
	    }
	    else
	    {
		UT_ASSERT(way == std::ios_base::end);
		ok = myStream->seekg(off, UT_IStream::UT_SEEK_END);
	    }
	}
	return ok ? myStream->tellg() : -1;
    }
private:
    UT_SharedPtr<FS_Reader>	 myReader;
    UT_IStream			*myStream;
};

using FS_IStreamDeviceBuffer = bios::stream_buffer<FS_IStreamDevice>;

#endif
