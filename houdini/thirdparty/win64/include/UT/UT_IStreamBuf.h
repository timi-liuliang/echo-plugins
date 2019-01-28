/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_IStreamBuf.h ( UT Library, C++)
 *
 * COMMENTS:	Since the semantics of C++ streams leave a little bit to be
 *		desired, this class provides an efficient mechanism to read
 *		streams line by line.
 *
 *		However, since this class buffers the stream input itself, it's
 *		not safe to use the stream outside of the class.
 */

#ifndef __UT_IStreamBuf__
#define __UT_IStreamBuf__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_SCFCommon.h"
#include "UT_StringHolder.h"
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>
#include <fstream>		// Only needed for gcc4.4
#include <stdio.h>
#include <string.h>

#define UT_STREAMBUF_SIZE 65536

class UT_Options;
class UT_WorkBuffer;

class UT_API UT_IStreamBuf {
public:
	     UT_IStreamBuf(bool bufferable = true);

    enum {
	UT_SEEK_BEG=0,	// Seek from beginning of stream
	UT_SEEK_CUR=1,	// Seek from current location
	UT_SEEK_END=2	// Seek from the end of the stream
    };

    static UT_IStreamBuf	*nullBuf();
    static UT_IStreamBuf	*fileBuf(const UT_StringHolder &filename,
					 const UT_Options *options,
                                         bool bufferable);
    static UT_IStreamBuf	*fileBuf(FILE *fp,
                                         bool bufferable);
    static UT_IStreamBuf	*istreamBuf(std::istream &is,
                                            bool bufferable);
    static UT_IStreamBuf	*memoryBuf(const char *src, size_t len);
    static UT_IStreamBuf	*rangeBuf(UT_IStreamBuf *src,
					int64 start, int64 end);

    static UT_IStreamBuf	*zlibBuf(UT_IStreamBuf *src,
                                         bool bufferable=true);
    static UT_IStreamBuf	*gzipBuf(UT_IStreamBuf *src,
                                         bool bufferable=true);
    static UT_IStreamBuf	*bloscBuf(UT_IStreamBuf *src,
					 bool bufferable=true);
    static UT_IStreamBuf	*scBuf(UT_IStream *src,
					 bool bufferable=true);
    static UT_IStreamBuf	*blowfishBuf(UT_IStreamBuf *src,
				    const unsigned char *key, exint key_length,
                                    bool bufferable=true);
    static exint		 blowfishPlainTextLength(UT_IStreamBuf *src);

    static void		 setStdinCompatible(bool state);

    void		 bumpRef()	{ myRefCount++; }
    void		 decRef()
    {
        myRefCount--;
        if (!myRefCount)
            delete this;
    }

    /// Set the index for the stream buffer (does nothing if stream
    /// buffer does not support index)
    virtual void setIndex(const UT_CompressedBlockIndexPtr &index) {}

    // Determine if the caller tried to read past the end of file,
    // assuming that we've run out of buffer.
    bool		 isEof() const
    {
        return myBufCur == myBufEnd && endOfFileSet();
    }

    // If the stream has exhausted the buffered read (if there was a buffer
    // associated with this stream-buf), this method provides the way to read
    // directly from the stream.
    // It returns the number of bytes read.
    virtual exint read(char *buffer, exint asize=1) = 0;

    // peek returns the character read as an unsigned char cast to an
    // int, or -1 on failure.  In other words, characters with the high bit set
    // will return a number from 128 to 255, not a negative number.
    virtual int streamPeek() = 0;

    // For streams which own the underlying stream, the close method allows
    // that stream to be closed.
    virtual bool closeFile();

    // Return true for a random-access file.
    virtual bool isRandomAccessFile(UT_WorkBuffer &filename) const;

    // Query the file system type - not supported on all platforms
    virtual int getFileSystemType() const;

    // Query the file descriptor associated with an open stream, if any.  Be
    // careful what you do with it.
    virtual int		getFileDescriptor() const;

    // If there was an error on the stream, the error flag will be set
    inline bool getError() const { return myError; }
    const char *getErrorStr() const;
    void stealLoadBufferData(const char *&bufstart,
			     const char *&bufend,
			     exint maxlen);

    exint bread(char *buffer, exint size)
    {
        if (myError)
            return 0;

        // Fast path for size 1, because it's being hit often.
        if (size == 1 && myBufCur != myBufEnd)
        {
            *buffer = *myBufCur;
            ++myBufCur;
            return 1;
        }

        if (!myBufferable)
            return read(buffer, size);

        exint nread = 0;
        exint left = size;
        while (left > 0)
        {
            if (myBufCur == myBufEnd)
            {
                // If we're reading a small amount of data, buffer it.
                if (left >= UT_STREAMBUF_SIZE)
                {
		    // Clear the buffer in this case because the position of
		    // the underlying stream will be past the buffer.  If we
		    // try to seek back into the buffer and then read past it
		    // the underlying stream will be in the wrong place.
                    myBufStartPos = -1;
		    myBufStart = NULL;
		    myBufCur = NULL;
		    myBufEnd = NULL;
                    break;
                }
                if (!loadBuffer())
                    break;
            }

            // Load first portion from my buffer
	    // How much can I read
            exint n = SYSmin(left, exint(myBufEnd - myBufCur));
            memcpy(buffer+nread, myBufCur, n);
            myBufCur += n;
            left -= n;
            nread += n;
        }
        if (left)
            nread += read(buffer+nread, left);

        return nread;
    }

    exint tellg() const
    {
        if (myBufCur != NULL)
            return myBufStartPos + (myBufCur - myBufStart);

        return tellPos();
    }

    bool seekg(exint pos, int seekdir)
    {
        myError = false;
        if (myBufCur != NULL && seekdir != UT_SEEK_END)
        {
            // Get the position relative to the buffer start
            exint relpos;
            if (seekdir == UT_SEEK_CUR)
                relpos = pos + (myBufCur - myBufStart);
            else
            {
                UT_ASSERT_P(seekdir == UT_SEEK_BEG);
                relpos = pos - myBufStartPos;
            }
            // If we're seeking inside the buffer, just move myBufCur.
            if (relpos >= 0 && relpos < (myBufEnd - myBufStart))
            {
                myBufCur = myBufStart + relpos;
                return true;
            }

            // The stream has already been read until the end of the buffer,
            // so any seeks relative to the current position need to be
            // adjusted to be relative to the end of the buffer.
            if (seekdir == UT_SEEK_CUR)
                pos -= (myBufEnd - myBufCur);
        }

        // Clear the buffer since we're seeking to a new place in the stream
        myBufStartPos = -1;
        myBufStart = NULL;
        myBufCur = NULL;
        myBufEnd = NULL;
        return seekPos(pos, seekdir);
    }

    void	clearLoadBuffer();
    bool	getLine(UT_WorkBuffer &buffer, int end='\n');
    bool	skipLine(int end='\n');
    bool	getJSONWord(UT_WorkBuffer &buffer, bool &real);
    bool	getWord(UT_WorkBuffer &buffer);
    bool	getString(UT_WorkBuffer &buffer);
    bool	getNumber(UT_WorkBuffer &buffer, bool &real);
    bool	skipWhitespace();
    bool	skipWhitespace(int64 *line_count, int64 *line_start_pos);
    bool	getAll(UT_WorkBuffer &buffer);
    bool	getAllAscii(UT_WorkBuffer &buffer);

    SYS_FORCE_INLINE bool
    checkToken(const char *match)
    {
	UT_ASSERT_MSG(isBufferable(),"Unbuffered checkToken() not implemented");

	if (!skipWhitespace())
	    return false;

	// Check token against match
	for ( ; *match; ++myBufCur, ++match)
	{
	    if (myBufCur == myBufEnd && !loadBuffer())
		return false;
	    if (*match != *myBufCur)
		return false;
	}
	// Check one more character to see if it's whitespace (space or newline)
	if (myBufCur == myBufEnd && !loadBuffer())
	    return !myError;
	return !myError && SYSisspace(*myBufCur);
    }

    /// getc and peek return the character read as an unsigned char cast to an
    /// int, or -1 (EOF) on failure.  In other words, characters with the high
    /// bit set will return a number from 128 to 255, not a negative number.
    SYS_FORCE_INLINE int getc()
    {
        if (myBufCur != myBufEnd || loadBuffer())
        {
            char c = *myBufCur;
            ++myBufCur;
            return (int)(uchar)c;
        }
        char data;
        exint nread = read(&data, 1);
        if (nread == 0)
            return -1;
        return (int)(uchar)data;
    }
    SYS_FORCE_INLINE int peekc()
    {
        if (myBufCur != myBufEnd || loadBuffer())
        {
            char c = *myBufCur;
            return (int)(uchar)c;
        }
        return streamPeek();
    }
    SYS_FORCE_INLINE exint unwind(exint len)
    {
        len = SYSmin(exint(myBufCur - myBufStart), len);
        myBufCur -= len;
        return len;
    }

    virtual int64 getMemoryUsage(bool inclusive) const;

    bool isBufferable() const
    { return myBufferable; }

protected:
    virtual	~UT_IStreamBuf();
    void	 setError(const char *msg=0);

    // Extract an alpha-numeric word, with an optional prefix
    bool	getAlNumWord(UT_WorkBuffer &buffer, const char *prefix="");

    // Read data into a buffer and fill in the start and end pointers.
    // The file position of the start of the buffer is put into bufstartpos.
    // When the end of the buffer is reached, call loadBuffer() again,
    // to get more data.
    virtual void loadBuffer(exint &bufstartpos, const char *&bufstart, const char *&bufend, bool keepeofif0) = 0;

    virtual exint tellPos() const = 0;
    virtual bool seekPos(exint pos, int dir) = 0;
    // Determine if the caller tried to read past the end of file,
    // assuming that we've run out of buffer.
    // The equivalent to this as a public method is isEof().
    virtual bool	 endOfFileSet() const = 0;

    // If loadBuffer() is being used as if it were read(), and not enough
    // has already been read to count as a successful read from the outside
    // perspective, a failure to read anything here should keep the eof
    // flag set instead of clearing it.
    // The eof flag is never kept set if loadBuffer reads some non-zero
    // amount of data, because it logically only read up to the end, not past.
    bool loadBuffer(bool keepeofif0 = false);

    void appendDecodedEscapeSequence(UT_WorkBuffer &buffer);

private:
    int64        myBufStartPos;
    const char  *myBufStart;
    const char  *myBufCur;
    const char  *myBufEnd;
    char	*myErrorStr;
    exint	 myRefCount;
    int		 myErrorNum;
    bool	 myError;
    bool         myBufferable;
};

SYS_FORCE_INLINE bool
UT_IStreamBuf::skipWhitespace()
{
    // FIXME: Unbuffered UT_IStreamBuf::skipWhitespace is not implemented!!!
    UT_ASSERT_MSG(isBufferable(),
	    "Unbuffered skipWhitespace() not implemented");

    // Return false if we're already at the EOF
    if (myBufCur == myBufEnd && !loadBuffer(true))
	return false;

    do {
	for ( ; myBufCur < myBufEnd; ++myBufCur)
	{
	    if (!SYSisspace(*myBufCur))
		return true;
	}
    } while (loadBuffer(false));

    // Return true if we skipped to the EOF so that the caller can determine
    // whether it's valid.
    return true;
}

inline bool
UT_IStreamBuf::skipLine(int newline)
{
    // FIXME: Unbuffered UT_IStreamBuf::skipLine is not implemented!!!
    UT_ASSERT_MSG(isBufferable(),
	    "Unbuffered skipLine not implemented");

    // Return false if we're already at the EOF
    if (myBufCur == myBufEnd && !loadBuffer(true))
	return false;

    do {
	for ( ; myBufCur < myBufEnd; ++myBufCur)
	{
	    if (*myBufCur == newline)
	    {
		++myBufCur; // move over the newline
		return true;
	    }
	}
    } while (loadBuffer(false));

    // Return true if we skipped to the EOF so that the caller can determine
    // whether it's valid.
    return true;
}

#endif
