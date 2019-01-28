/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_IStream.h ( UT Library, C++)
 *
 * COMMENTS:	Since the semantics of C++ streams leave a little bit to be
 *		desired, this class provides an efficient mechanism to read
 *		streams line by line.
 *
 *		However, since this class buffers the stream input itself, it's
 *		not safe to use the stream outside of the class.
 */

#ifndef __UT_IStream__
#define __UT_IStream__

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_IOS.h"
#include "UT_IStreamBuf.h"
#include "UT_SmallArray.h"
#include "UT_StringHolder.h"
#include "UT_UniquePtr.h"
#include "UT_WorkBuffer.h"
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>
#include <fstream>	// Only needed for gcc4.4 for handling of getc()
#include <stdio.h>
#include <string.h>
#include <string>
#include <iosfwd>

class UT_String;
class UT_Options;

typedef enum {
    UT_ISTREAM_ASCII  = UT_IOS_ASCII,		// Ascii stream
    UT_ISTREAM_BINARY = UT_IOS_BINARY
} UT_ISTREAM_READTYPE;

//
// When saving/loading binary strings, we store the run length of the string
// prior to saving the string data.  This enum specifies the minimum value
// possible for the run-length.
//
// It is somewhat dangerous to use 8 bit lengths.
//
typedef enum {
    UT_ISTREAM_8BIT_IO = 8,
    UT_ISTREAM_16BIT_IO = 16,
    UT_ISTREAM_32BIT_IO = 32,
    UT_ISTREAM_64BIT_IO = 64
} UT_ISTREAM_RLE_IO;

class UT_API UT_IStream : public UT_NonCopyable {
public:
    /// WARNING: You MUST call init before using a default-constructed
    ///          UT_IStream!
    UT_IStream() {}

    /// FIXME: Many functions will assert when bufferable is false,
    ///        because they assume that they can read directly from
    ///        the buffer.
    UT_IStream(FILE *fp, UT_ISTREAM_READTYPE binary=UT_ISTREAM_BINARY,
               bool bufferable=true)
    { init(fp, binary, bufferable); }
    UT_IStream(std::istream &is, UT_ISTREAM_READTYPE binary=UT_ISTREAM_BINARY,
               bool bufferable=true)
    { init(is, binary, bufferable); }
    UT_IStream(const char *buffer, size_t bufsize, UT_ISTREAM_READTYPE binary)
    { init(buffer, bufsize, binary); }
    UT_IStream(const UT_WorkBuffer &buffer,
               UT_ISTREAM_READTYPE binary=UT_ISTREAM_ASCII)
    { init(buffer, binary); }

    virtual ~UT_IStream();

    void init(FILE *fp, UT_ISTREAM_READTYPE binary=UT_ISTREAM_BINARY,
               bool bufferable=true);
    void init(std::istream &is, UT_ISTREAM_READTYPE binary=UT_ISTREAM_BINARY,
               bool bufferable=true);
    void init(const char *buffer, size_t bufsize, UT_ISTREAM_READTYPE binary);
    void init(const UT_WorkBuffer &buffer,
              UT_ISTREAM_READTYPE binary=UT_ISTREAM_ASCII);

    /// NOTE: This counts the memory of the UT_IStreamBuf, even though it
    ///       may be shared among multiple UT_IStream objects.
    virtual int64 getMemoryUsage(bool inclusive) const;

    /// Retrieves the filename of this stream if it was created from file
    /// returns NULL otherwise
    const char* getFilename() const;

    void setIsFile(bool isFile) { myIsFile = isFile; }

    /// It can be useful to know if this stream is a substream, particularly
    /// when this stream is flagged as a file.
    void setIsSubStream(bool is_sub_stream) { myIsSubStream = is_sub_stream; }
    bool isSubStream() const { return myIsSubStream; }

    // Typically, when reading from a non-tty stdin, istreams are able to
    // buffer the contents.  However, in some cases, when there's a
    // controlling application sending commands sporadically, it's important
    // to set compatibility on stdin.  This will result in slightly slower
    // input and should only be used when required.
    static void		setStdinCompatible(bool state);

    enum {
	UT_SEEK_BEG=UT_IOS_SEEK_BEG,	// Seek from beginning of stream
	UT_SEEK_CUR=UT_IOS_SEEK_CUR,	// Seek from current location
	UT_SEEK_END=UT_IOS_SEEK_END	// Seek from the end of the stream
    };

    /// @{
    /// Query or seek to a position in the stream.  Please see documentation
    /// for standard istreams for further information.
    exint	tellg() const
    { return myStream->tellg(); }
    bool	seekg(exint pos, int seekdir=UT_SEEK_BEG);
    /// @}

    /// Check whether the stream refers to a filename and whether the file
    /// supports random seeks.
    bool	isRandomAccessFile(UT_WorkBuffer &filename) const;

    /// Return whether the stream refers to a local drive or a network path.
    typedef enum {
	UT_FSYSTEM_UNKNOWN,	// Unknown file system
	UT_FSYSTEM_LOCAL,	// Local file system
	UT_FSYSTEM_NETWORK,	// NFS or SMB file system
    } FileSystemType;
    FileSystemType	getFileSystemType() const;

    /// Returns the open file descriptor, if any.  Be careful what you do with
    /// it.
    int		getFileDescriptor() const;

    // Returns true if the stream is out of data OR if there's an error on the
    // stream.
    bool	isEof() const;

    // Return the error status.
    bool	isError() const		{ return myError; }
    void	setError(bool is_error) { myError = is_error; }

    bool	isAscii() const		{ return !myBinary; }
    bool	isBinary() const	{ return myBinary; }

    /// The version information can be used to stash version information with
    /// the stream itself rather than having to pass the version information
    /// through to all other loading methods.  This is used primarily in
    /// legacy geometry loading.
    void	setVersion(int version)	{ myVersion = version; }
    int		getVersion() const	{ return myVersion; }

    /// The label methods provide a way to give the stream a label which is
    /// used to identify the stream in error messages.  For example,
    /// UT_IFStream sets the label to the filename.
    void	 setLabel(const UT_StringHolder& label)	{ myLabel = label; }
    const UT_StringHolder &getLabel() const { return myLabel; }

    /// The read() methods use the binary state of the stream to determine
    /// whether to read ASCII or binary values.  Rather than calling this
    /// method directly, it would be better to create a UT_IStreamAutoBinary,
    /// which will automatically pop the state on its destruction.
    void	pushBinary(UT_ISTREAM_READTYPE binary);
    void	popBinary();

    /// For error handling.  If an error occurs on the stream, the operation
    /// will be added to the error message (i.e. "loading image").  Rather
    /// than calling this method directly, it would probably be better to use
    /// UT_IStreamAutoOp which automatically pops the operation.
    void	pushOperation(const char *operation);
    void	popOperation();

    // Set a sub-range of the stream.  The stream will automatically seek the
    // the start position specified.  The stream will hit EOF when nbytes have
    // been read from the stream.  Seeking should work (assertions will be
    // triggered if the seek is out of bounds).  Absolute seek positions are
    // specified based on the original stream.
    UT_IStreamBuf	*getStreamBuffer()	{ return myStream; }

    /// Returns a stream represenging a substream of the parent stream starting
    /// at 'start' byte and containin 'nbytes'.
    UT_UniquePtr<UT_IStream> getSubStream(exint start, exint nbytes);

    // The zlib, gzip, and blosc streams are available for decompression.
    // The difference between zlib and gzip is subtle, but very important.
    //	- zlib compression uses the zlib library to perform inflation of the
    //		data.  However, the bit window specified is slightly different
    //		than for gzip compression.
    //	- gzip compression uses the zlib library to perform inflation of the
    //		data.  However, it expects that the data has a gzip header
    //		before the compressed data begins.  As well, the window size is
    //		slightly different than raw zlib compression.
    UT_UniquePtr<UT_IStream> getZlibStream();
    UT_UniquePtr<UT_IStream> getGzipStream();
    UT_UniquePtr<UT_IStream> getBloscStream();
    UT_UniquePtr<UT_IStream> getSCStream();

    bool	getLine(UT_WorkBuffer &buffer, int end='\n');
    bool	skipLine(int end='\n');
    bool	getWord(UT_WorkBuffer &buffer);
    bool	getString(UT_WorkBuffer &buffer);
    bool	skipWhitespace(int64 &line_count, int64 &line_start_pos);
    bool	skipWhitespace();

    /// Read a JSON token.  This assumes that leading white space has been
    /// trimmed and will read all characters until:
    ///	   - a space (\\f, \\v, \\r, \\n, \\t, ' ')
    ///    - a separator ('/', '{', '}', '[', ']', ',', ':', '"')
    /// @note The real flag will be set if the word contains ('.', 'e', or 'E')
    bool	getJSONWord(UT_WorkBuffer &buffer, bool &real);

    /// Reads in the entire stream from the current position to the end.
    /// Returns true if anything was read in, false otherwise.  Use
    /// isError() to test error state.
    bool	getAll(UT_WorkBuffer &buffer);

    /// Does the same as getAll, except any carriage return-newline (\r\n)
    /// sequences are converted into newlines (\n).
    bool	getAllAscii(UT_WorkBuffer &buffer);

    /// Skips all the comment lines and reads the next token in the stream.
    /// Returns true if successful and false otherwise.
    bool	getNonCommentWord(UT_WorkBuffer &buffer, int comment='#');

    /// Skips all the comment lines and reads the next line in the stream.
    /// Returns the number of lines read.  Note that the resulting line will
    /// be stripped of leading white space as well.  The 
    /// strip_inline_comments argument determines whether or not the resulting
    /// line should have comments at the end, if any, stripped.
    exint	getNonCommentLine(UT_WorkBuffer &buffer, int comment='#',
				  bool strip_inline_comments = true);

    /// Reads in the next token (skipping comments) and compares it to the
    /// expected value.  Returns true if the tokens match, and false otherwise.
    bool	checkNonCommentToken(const char *expected, int comment='#')
		{
		    UT_WorkBuffer	wbuf;
		    if (!getNonCommentWord(wbuf, comment))
			return false;
		    return !strcmp(wbuf.buffer(), expected);
		}

    /// Reads in the next token and compares it to the expected value.  
    /// Returns true if the tokens match, and false otherwise.
    bool	checkToken(const char *expected)
		{
		    if (!myError)
		    {
			if (myStream->checkToken(expected))
			    return true;
			setError();
		    }
		    return false;
		}

    /// getc and peek return the character read as an unsigned char cast to an
    /// int, or -1 (EOF) on failure.  In other words, characters with the high
    /// bit set will return a number from 128 to 255, not a negative number.
    SYS_FORCE_INLINE int getc()
    { return myStream->getc(); }
    SYS_FORCE_INLINE int peek()
    { return myStream->peekc(); }

    // Returns the number of characters the stream was able to rewind.  The
    // class is guaranteed to be able to put back one character if myBufferable.
    // Otherwise, you takes your chances.  This is not true if you peek into the
    // stream.  Calling peek() may cause backup to fail.
    SYS_FORCE_INLINE exint ungetc()	{ return unwind(1); }
    SYS_FORCE_INLINE exint unwind(exint amount)
    { return myStream->unwind(amount); }

    // Read data into an array of the given type.  The data will be
    // byte-swapped according to the Houdini standard.  The size specified is
    // the number of elements in the array (not the size in bytes).
    //
    // Due to an ambiguity when reading characters from an ASCII stream,
    // there is no generic method to read characters.  Use the bread() method
    // to read raw characters, or the getWord() method to read a word.
    exint	read(bool *array, exint sz=1)
		{
		    return myBinary ? bread(array, sz) : aread(array, sz);
		}
    exint	read(int16 *array, exint sz=1)
		{
		    return myBinary ? bread(array, sz) : aread(array, sz);
		}
    exint	read(uint16 *array, exint sz=1)
		{
		    return myBinary ? bread(array, sz) : aread(array, sz);
		}
    exint	read(int32 *array, exint sz=1)
		{
		    return myBinary ? bread(array, sz) : aread(array, sz);
		}
    exint	read(uint32 *array, exint sz=1)
		{
		    return myBinary ? bread(array, sz) : aread(array, sz);
		}
    exint	read(int64 *array, exint sz=1)
		{
		    return myBinary ? bread(array, sz) : aread(array, sz);
		}
    exint	read(uint64 *array, exint sz=1)
		{
		    return myBinary ? bread(array, sz) : aread(array, sz);
		}
    bool	read(UT_String &string)
		{
		    return myBinary ? bread(string) : aread(string);
		}
    bool	read(std::string &bytes)
		{
		    return myBinary ? bread(bytes) : aread(bytes);
		}
    bool	read(UT_StringHolder &string)
		{
		    return readStringHolder(string, myBinary);
		}

    // Helpers for reading sz elements from the stream, but only storing
    // the first max_elem number of elements.
    template <typename SOURCE_TYPE, typename T>
    exint	read(T *array, exint sz, exint max_elem)
		{
		    exint n = (max_elem <= sz) ? max_elem : sz;
		    exint i = read<SOURCE_TYPE>(array, n);
		    if (i <= max_elem)
			return i;
		    SOURCE_TYPE dummy;
		    for (i = max_elem; i < sz; i++)
		    {
			if (!read<SOURCE_TYPE>(&dummy))
			    break;
		    }
		    return i;
		}
    exint	read(bool *array, exint sz, exint max_elem)
		{
		    exint n = (max_elem <= sz) ? max_elem : sz;
		    exint i = read(array, n);
		    if (i <= max_elem)
			return i;
		    bool dummy;
		    for (i = max_elem; i < sz; i++)
		    {
			if (!read(&dummy))
			    break;
		    }
		    return i;
		}

    // Read a string that was saved by first writing the number of bytes in the
    // stream, followed by the string itself.  The length was stored using as
    // few bytes as possible, and startbits specifies the minimum number of
    // size bits it can expect.  Note that a UT_String cannot contain data with
    // null's, so use readBinaryData below or read(std::string &) for binary
    // data.
    bool	readBinaryString(UT_String &str, UT_ISTREAM_RLE_IO startbits);
    bool	readBinaryString(UT_StringHolder &string,
				UT_ISTREAM_RLE_IO startbits);

    // Read a chunk of binary data, returning NULL if the data could not be
    // read for some reason.  The exint size of the data is returned in the
    // "size" parameter.  If add_null is true, the data is null-terminated
    // and "size" includes the space for the null terminator.  The caller
    // is responsible for calling free() on the result.  It is safe to adopt
    // the result into a UT_String.
    char *readBinaryData(
	exint &size, UT_ISTREAM_RLE_IO startbits, bool add_null=false);

    bool	readBool(bool &result)
		{
		    return (read(&result) == 1);
		}
    bool	readChar(char &result)
		{
		    if (isBinary())
			return bread(&result) == 1;
		    return areadChar(result);
		}
    bool	areadChar(char &result)
		{
		    if (!skipWhitespace())
			return false;
		    return bread(&result, 1) == 1;
		}

    /// Read a floating point value of the specified source type into the given
    /// destination variable, performing a conversion as necessary.
    // @{
    template <typename SOURCE_TYPE> exint read(int32 *val, exint sz=1)
	{ return myBinary ? bread<SOURCE_TYPE>(val, sz) : aread(val, sz); }
    template <typename SOURCE_TYPE> exint read(int64 *val, exint sz=1)
	{ return myBinary ? bread<SOURCE_TYPE>(val, sz) : aread(val, sz); }
    template <typename SOURCE_TYPE> exint read(fpreal16 *val, exint sz=1)
	{ return myBinary ? bread<SOURCE_TYPE>(val, sz) : aread(val, sz); }
    template <typename SOURCE_TYPE> exint read(fpreal32 *val, exint sz=1)
	{ return myBinary ? bread<SOURCE_TYPE>(val, sz) : aread(val, sz); }
    template <typename SOURCE_TYPE> exint read(fpreal64 *val, exint sz=1)
	{ return myBinary ? bread<SOURCE_TYPE>(val, sz) : aread(val, sz); }

    template <typename SOURCE_TYPE> exint bread(int32 *buffer, exint asize=1);
    template <typename SOURCE_TYPE> exint bread(int64 *buffer, exint asize=1);
    template <typename SOURCE_TYPE> exint bread(fpreal16 *buffer, exint asize=1);
    template <typename SOURCE_TYPE> exint bread(fpreal32 *buffer, exint asize=1);
    template <typename SOURCE_TYPE> exint bread(fpreal64 *buffer, exint asize=1);
    // @}

    // Platform consistent binary reads of data.  The read methods return the
    // number of elements read.
    inline exint bread(bool *buffer, exint size=1)
    {
        UT_ASSERT_COMPILETIME(sizeof(bool) == sizeof(char));
        return bread(reinterpret_cast<char *>(buffer), size);
    }
    exint	bread(char *buffer, exint asize=1);
    exint	bread(unsigned char *buffer, exint asize=1)
		    { return bread((char *)buffer, asize); }
    exint	bread(signed char *buffer, exint asize=1)
		    { return bread((char *)buffer, asize); }
    exint	bread(int16 *buffer, exint asize=1);
    exint	bread(uint16 *buffer, exint asize=1)
		    { return bread((int16 *)buffer, asize); }
    exint	bread(int32 *buffer, exint asize=1);
    exint	bread(uint32 *buffer, exint asize=1)
		    { return bread((int32 *)buffer, asize); }
    exint	bread(int64 *buffer, exint asize=1);
    exint	bread(uint64 *buffer, exint asize=1)
		    { return bread((int64 *)buffer, asize); }
    bool	bread(UT_String &str)	{ return readString(str, true); }
    bool	bread(std::string &bytes)
		    { return readStdString(bytes, true); }
    bool	bread(UT_StringHolder &bytes)
		    { return readStringHolder(bytes, true); }

    // Read ASCII values into the buffers.  The process is that we read a word,
    // then run atoi() or atof() on the token.  This does NOT set an error bit
    // if the word is not an integer.
    //
    // The aread(char *) method does not exist due to an ambiguity.
    // The following code:
    //	  char	chr;
    //	  is.read(&chr);
    // would skip whitespace when reading from an ASCII stream.  Since this is
    // error prone (i.e. you might expect the next character of the stream to
    // be read), there is no implicit ASCII read method for an array of
    // characters.  To read a single character, please call readChar() or
    // areadChar().
    //
    exint	aread(bool *buffer, exint asize=1);
    exint	aread(int16 *buffer, exint asize=1);
    exint	aread(uint16 *buffer, exint asize=1);
    exint	aread(int32 *buffer, exint asize=1);
    exint	aread(uint32 *buffer, exint asize=1);
    exint	aread(int64 *buffer, exint asize=1);
    exint	aread(uint64 *buffer, exint asize=1);
    exint	aread(fpreal16 *buffer, exint asize=1);
    exint	aread(fpreal32 *buffer, exint asize=1);
    exint	aread(fpreal64 *buffer, exint asize=1);
    bool	aread(UT_String &str)	{ return readString(str, false); }
    bool	aread(std::string &bytes)
		    { return readStdString(bytes, false); }
    bool	aread(UT_StringHolder &bytes)
		    { return readStringHolder(bytes, false); }

protected:
    // Protected constructors (used either internally or by sub-classes)

    // If you can't access this, you want a UT_IFStream subclass
    UT_IStream(const UT_StringHolder &filename, const UT_Options *open_options,
               bool bufferable);
    UT_IStream(UT_IStreamBuf *sbuf, UT_ISTREAM_READTYPE binary);

protected:
    void	initCommon(UT_ISTREAM_READTYPE binary);
    bool	readString(UT_String &str, bool binary);
    bool	readStdString(std::string &bytes, bool binary);
    bool	readStringHolder(UT_StringHolder &bytes, bool binary);
    void	setError();
    inline void clearError()
    { myError = false; }

    // In practice, myBinaryStack has a max size of 2 for any given session
    UT_SmallArray<int,sizeof(int)*4> myBinaryStack;

    UT_Array<const char *>	 myOperationStack;

    UT_IStreamBuf		*myStream;
    UT_StringHolder		 myLabel;
    int				 myVersion;
    bool			 myBinary, myEof, myError;
    bool 			 myIsFile;
    bool			 myIsSubStream;
};

inline bool
UT_IStream::skipLine(int newline)
{
    if (!myError)
    {
	if (myStream->skipLine(newline))
	    return true;
	setError();
    }
    return false;
}

inline bool
UT_IStream::skipWhitespace()
{
    if (!myError)
    {
	if (myStream->skipWhitespace())
	    return true;
	setError();
    }
    return false;
}

inline bool
UT_IStream::skipWhitespace(int64 &line_count, int64 &line_start_pos)
{
    if (!myError)
    {
	if (myStream->skipWhitespace(&line_count, &line_start_pos))
	    return true;
	setError();
    }
    return false;
}

//
// Simple wrapper class for file streams that handles open and close of the
// file automatically.
class UT_API UT_IFStream : public UT_IStream {
public:
	     UT_IFStream();

    /// Open a file for input
    ///
    /// Known Options
    ///	  cache_mode  "random" or "sequential" hint
    ///	  ascii       true or false (performs CRLF translation on Windows)
    /// @{
	    UT_IFStream(
		const UT_StringHolder &filename,
		UT_ISTREAM_READTYPE binary=UT_ISTREAM_BINARY,
		const UT_Options *options=NULL,
		bool bufferable=true);
    bool    open(
		const UT_StringHolder &filename,
		UT_ISTREAM_READTYPE binary=UT_ISTREAM_BINARY,
		const UT_Options *options=NULL,
		bool bufferable=true);
    /// @}

    virtual ~UT_IFStream();

    bool	close();
};

//
// Compressed data input.
// Please see the getZlib and getGzip stream methods on UT_IStream.
class UT_API UT_IZStream : public UT_IStream {
public:
	     UT_IZStream(UT_IStream &src);
    virtual ~UT_IZStream();
};

class UT_API UT_IGZStream : public UT_IStream {
public:
	     UT_IGZStream(UT_IStream &src);
    virtual ~UT_IGZStream();
};

// Data input encrypted with a blowfish algorithm.
class UT_API UT_IBlowfishStream : public UT_IStream {
public:
	     UT_IBlowfishStream(UT_IStream &src, const unsigned char * key, 
				exint key_size,
				UT_ISTREAM_READTYPE binary=UT_ISTREAM_BINARY);
    virtual ~UT_IBlowfishStream();

    /// Obtains the length of data after it has been decrypted from the stream.
    static exint	    getPlainTextLength(UT_IStream &src);
};

//
// Turn the stream into binary or ASCII while scoped
class UT_API UT_IStreamAutoBinary {
public:
    explicit UT_IStreamAutoBinary(UT_IStream &is, UT_ISTREAM_READTYPE binary)
	: myIs(is)
    {
	myIs.pushBinary(binary);
    }
    explicit UT_IStreamAutoBinary(UT_IStream &is, bool binary)
	: myIs(is)
    {
	myIs.pushBinary(binary ? UT_ISTREAM_BINARY : UT_ISTREAM_ASCII);
    }
    ~UT_IStreamAutoBinary()
    {
	myIs.popBinary();
    }
private:
    UT_IStream	&myIs;
};

//
// Push operations being performed on the stream.  This is for error
// reporting.
class UT_API UT_IStreamAutoOp {
public:
    // Typically the strings will be compile time defined, which means they
    // don't need to be hardened.  The UT_IStream expects the operation string
    // to be around for the duration of the operation.
    explicit UT_IStreamAutoOp(UT_IStream &is, const char *operation,
			bool harden = false)
	    : myIs(is),
	      myOperation(operation, harden ? 1 : 0)
    {
	myIs.pushOperation(myOperation);
    }
    ~UT_IStreamAutoOp()
    {
	myIs.popOperation();
    }
private:
    UT_IStream	&myIs;
    UT_String	 myOperation;
};

#endif
