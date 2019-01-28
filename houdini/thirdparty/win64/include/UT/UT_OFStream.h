/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_OFStream.h (UT Library, C++)
 *
 * COMMENTS:	Portable replacement for std::ofstream
 */

#ifndef __UT_OFSTREAM_H_INCLUDED__
#define __UT_OFSTREAM_H_INCLUDED__

#include "UT_API.h"
#include "UT_NonCopyable.h"
#include "UT_FileBuf.h"

#include <ostream>
#include <string>


/// @brief Portable replacement for std::ofstream
///
/// @notes Files will always be opened in binary mode to avoid platform
/// differences for line endings.
class UT_API UT_OFStream : public std::ostream, UT_NonCopyable
{
public:
    typedef std::ostream	    SUPER_CLASS;
    typedef std::ios_base	    IOS_BASE;
    typedef std::ios_base::openmode OPEN_MODE;

    /// Construct with no opened file
    UT_OFStream()
	: SUPER_CLASS(&myFileBuf)
    {
    }

    /// Construct with file path
    /// @{
    explicit UT_OFStream(
		const char *filename,
		OPEN_MODE mode = IOS_BASE::out,
		UT_IOS_TYPE mode_type = UT_IOS_BINARY)
	: SUPER_CLASS(&myFileBuf)
    {
	initOpen(filename, mode, mode_type);
    }
    explicit UT_OFStream(
		const std::string &filename,
		OPEN_MODE mode = IOS_BASE::out,
		UT_IOS_TYPE mode_type = UT_IOS_BINARY)
	: SUPER_CLASS(&myFileBuf)
    {
	initOpen(filename.c_str(), mode, mode_type);
    }
    /// @}

    /// Destructor
    virtual ~UT_OFStream()
    {
    }

    /// Open with file path
    /// @{
    void open(const char *filename, OPEN_MODE mode = IOS_BASE::out,
	      UT_IOS_TYPE mode_type = UT_IOS_BINARY)
    {
	initOpen(filename, mode, mode_type);
    }
    void open(const std::string &filename, OPEN_MODE mode = IOS_BASE::out,
	      UT_IOS_TYPE mode_type = UT_IOS_BINARY)
    {
	initOpen(filename.c_str(), mode, mode_type);
    }
    /// @}

    /// Close the stream
    void close()
    {
	if (!myFileBuf.close())
	    SUPER_CLASS::setstate(IOS_BASE::failbit);
    }

    /// Return pointer to internal filebuf object
    UT_FileBuf *rdbuf() const
    {
	// std::ofstream interface requires const_cast
	return const_cast<UT_FileBuf *>(&myFileBuf);
    }

    /// Returns whether stream is currently associated to a file
    bool is_open() const
    {
	return myFileBuf.is_open();
    }

private:
    void initOpen(const char *filename, OPEN_MODE mode, UT_IOS_TYPE mode_type)
    {
	mode |= IOS_BASE::out;
	if (myFileBuf.open(filename, mode, mode_type) == 0)
	    setstate(IOS_BASE::failbit);
    }

private:
    UT_FileBuf	myFileBuf;
};

#endif // __UT_OFSTREAM_H_INCLUDED__
