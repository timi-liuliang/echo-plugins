/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FileBuf.h (UT Library, C++)
 *
 * COMMENTS:	Portable replacement for std::filebuf
 */

#ifndef __UT_FILEBUF_H_INCLUDED__
#define __UT_FILEBUF_H_INCLUDED__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_IOS.h"
#include "UT_NonCopyable.h"
#include "UT_NTStreamUtil.h"

#include <SYS/SYS_Stdio.h>

#include <stdio.h>
#include <streambuf>
#include <string>


class UT_API UT_FileBuf : public std::streambuf, UT_NonCopyable
{
public:
    typedef std::streambuf		SUPER_CLASS;
    typedef std::ios_base		IOS_BASE;
    typedef std::ios_base::openmode	OPEN_MODE;
    typedef traits_type::int_type	int_type;
    typedef traits_type::pos_type	pos_type;
    typedef traits_type::off_type	off_type;


    UT_FileBuf()
	: myFile(NULL)
    {
    }

    virtual ~UT_FileBuf()
    {
	if (myFile)
	    close();
    }

    /// Test if there exists an opened file
    bool is_open() const
    {
	return (myFile != NULL);
    }

    /// Open the given file with fopen and return a pointer to result.  The
    /// returned pointer is valid until the next open operation, the next close
    /// operation, or this object destructs.
    /// @{
    FILE* open(const char *filename, OPEN_MODE mode,
	       UT_IOS_TYPE mode_type = UT_IOS_BINARY);

    FILE* open(const std::string &filename, OPEN_MODE mode,
	       UT_IOS_TYPE mode_type = UT_IOS_BINARY)
	    { return open(filename.c_str(), mode, mode_type); }
    /// @}

    /// @brief Create filebuf from UTgetTmpFile() for read/write.
    /// @note This is a SESI-specific extension.
    FILE* openTmpFile();

    /// Close the currently opened file (if any)
    UT_FileBuf* close();

    /// @brief Attempt to set the opened (writable) file as sparse.
    /// The exact behaviour is platform-specific depending on the underlying
    /// file system. So this function provides a hint at best.
    /// @note This is a SESI-specific extension. This works on most filesystems
    /// on Linux, Windows and OSX post-Sierra.
    bool setFileAsSparse();

protected:

    // [BUFFER] Set the buffer for the stream
    virtual SUPER_CLASS*
    setbuf(char* buf, std::streamsize n)
    {
	if (!myFile)
	    return NULL;
	int mode = (buf == NULL && n == 0) ? _IONBF : _IOFBF;
	return (::setvbuf(myFile, buf, mode, n) == 0) ? this : NULL;
    }

    // [BUFFER] Set internal position to relative position
    virtual pos_type
    seekoff(off_type off, IOS_BASE::seekdir way,
	    OPEN_MODE /*which*/ = IOS_BASE::in | IOS_BASE::out)
    {
	int whence = SEEK_CUR;
	UT_ASSERT(myFile);
	switch (way)
	{
	    case IOS_BASE::beg:
		whence = SEEK_SET;
		break;
	    case IOS_BASE::end:
		whence = SEEK_END;
		break;
	    case IOS_BASE::cur:
	    default:
		whence = SEEK_CUR;
		break;
	}
	return (SYSfseek(myFile, off, whence) == 0)
		? pos_type(SYSftell(myFile))
		: pos_type(off_type(-1));
    }

    // [BUFFER] Set internal position to absolute position
    virtual pos_type
    seekpos(pos_type off, OPEN_MODE /*which*/ = IOS_BASE::in | IOS_BASE::out)
    {
	UT_ASSERT(myFile);
	return (SYSfseek(myFile, off, SEEK_SET) == 0)
		? pos_type(SYSftell(myFile))
		: pos_type(off_type(-1));
    }

    // [BUFFER] Flush buffer to disk
    virtual int
    sync()
    {
	// ostream::flush calls this indiscriminately regardless of the
	// stream's error state.
	if (!myFile)
	    return -1;
	
	return (::fflush(myFile) == 0) ? 0 : -1;
    }

    // [OUTPUT] Write single character
    virtual int_type overflow(int_type ch = traits_type::eof())
    {
	if (::putc(ch, myFile) == ch)
	    return ch;
	return traits_type::eof();

    }
    // [OUTPUT] Write multiple cahracters
    virtual std::streamsize
    xsputn(const char* buf, std::streamsize n) 
    {
	UT_ASSERT(myFile && n >= 0);
	return ::fwrite(buf, sizeof(char), n, myFile);
    }

    // [INPUT] Read multiple characters at once
    virtual std::streamsize
    xsgetn(char *buf, std::streamsize n)
    {
	UT_ASSERT(myFile && n >= 0);
	return ::fread(buf, sizeof(char), n, myFile);
    }
    // [INPUT] Read a character, without advancing the position
    virtual int_type
    underflow()
    {
	int_type ch = ::getc(myFile);
	if (ch != EOF)
	{
	    if (::ungetc(ch, myFile) == ch)
		return ch;
	}
	return traits_type::eof();
    }
    // [INPUT] Read a character and advance the position
    virtual int_type
    uflow()
    {
	int_type ch = ::getc(myFile);
	if (ch != EOF)
	    return ch;
	return traits_type::eof();
    }
    // [INPUT] Put back a character
    virtual int_type
    pbackfail(int_type ch = traits_type::eof())
    {
	UT_ASSERT(myFile);
	if (::ungetc(ch, myFile) == ch)
	    return ch;
	return traits_type::eof();
    }

private:
    FILE *			myFile;

    static const char *const	theBinaryCharModes[];
    static const char *const	theAsciiCharModes[];
    static const OPEN_MODE	theOpenModes[];
};

#endif // __UT_FILEBUF_H_INCLUDED__
