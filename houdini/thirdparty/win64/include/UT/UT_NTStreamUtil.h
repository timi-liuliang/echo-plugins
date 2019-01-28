
#ifndef __UT_NTStreamUtil_H__
#define __UT_NTStreamUtil_H__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_StringHolder.h"
#include "UT_TmpDir.h"
#include <SYS/SYS_Deprecated.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_Math.h>
#include <stdio.h>
#include <fstream>
#include <iosfwd>

class UT_OStream;
class UT_OStringStream;
class UT_WorkBuffer;
class UT_String;
class UT_IStream;


#ifdef WIN32
    #define NTBinaryMode    std::ios::binary
#else
    #define NTBinaryMode    ((std::ios::openmode)0)
#endif	// WIN32

#define	    UT_NoCreate	    ((std::ios::openmode)0)


typedef enum {
    UT_STRING_8BIT_IO = 8,
    UT_STRING_16BIT_IO = 16,
    UT_STRING_32BIT_IO = 32,
    UT_STRING_64BIT_IO = 64,
} UT_STRING_BINARY_IO;

typedef int UT_EnumType;


#ifdef WIN32
    
    void *getNewNamedPipe( char *pipeName );

    UT_API FILE *UnixPWrite( const char *cmd, const char *file );
    UT_API FILE *UnixPOpen( const char *file, const char *mode );
    UT_API void *UnixPOpen( const char *file, void **in, void **out,
			    void **err );
    UT_API int   UnixPClose( FILE *file, int block );
    inline FILE *popen( const char *name, const char *mode )
	    { return UnixPOpen( name, mode ); }
    inline int   pclose( FILE *f, int block = 1 )
	    { return UnixPClose( f, block ); }

    UT_API const char *UTgetSocketErrorString(int err);

#endif	// WIN32


///
/// We build our own stream buffer to work around a shortcoming
/// in the C++ standard.  pbump() and pcount() are defined to use
/// int, not streamsize, so much sadness occurs as you cross 2gb
/// boundaries.  See Bug: 45419
///
/// Note this isn't a buffered implementation - we don't invoke setpbuf
/// to update pbase, etc.  The logic is that may bring us back to the
/// broken pbump land.  Hopefully implementations use xsputn anyways.
///
/// exints are chosen rather than streamsize or int_type or pos_type
/// because I have lost all faith in how those are defined.
///
class UT_API UT_OStrStreamBuf : public std::streambuf
{
public:
    UT_OStrStreamBuf();
    explicit UT_OStrStreamBuf(exint initialsize);
    UT_OStrStreamBuf(char *pch, exint len);
    virtual ~UT_OStrStreamBuf();

protected:
    // write one character
    virtual int_type	overflow(int_type c);

    // In order to allow tellg to work, we need to provide seekoff.
    virtual pos_type	seekoff(off_type _Off,
			    std::ios_base::seekdir _Way,
			    std::ios_base::openmode /*_Mode*/ = std::ios::out);
    virtual pos_type	seekpos(pos_type _Off,
			    std::ios_base::openmode _Mode = std::ios::out);

    // write multiple characters
    virtual std::streamsize	xsputn (const char* s, std::streamsize num);

public:
    // Crappy strstreambuf equivalent functions
    char 		*str()
    {
	freeze(true);
	return myBuf;
    }

    void		 freeze(bool isfrozen)
    {
	myOwnBuf = !isfrozen;
    }

    // Note this is the current put position, not the entries!
    exint		 pcount() const
    {
	return myOffset;
    }

protected:
    bool		resizeBuf(exint minnewsize);

    exint		mySize, myEntries, myOffset;
    bool		myOwnBuf;
    char 		*myBuf;
};

/// Our portable and much improved strstream 
/// Uses an exint buffer to allow greater than 2gb streams.
class UT_API UT_OStrStream : public std::ostream
{
public:
    SYS_DEPRECATED_REPLACE(14.0, UT_OStringStream) 
    UT_OStrStream();
    
    SYS_DEPRECATED_REPLACE(14.0, UT_OStringStream) 
    explicit UT_OStrStream(exint initialsize);

    /// The stream does *not* gain ownership of this buffer!
    /// This means it will not resize if it overflows this buffer.
#if !defined(WIN32)
    SYS_DEPRECATED_REPLACE(14.0, UT_OStringStream) 
    UT_OStrStream(char *pch, exint nLength,
		  std::ios::openmode = std::ios::out);
#else
    SYS_DEPRECATED_REPLACE(14.0, UT_OStringStream) 
    UT_OStrStream(char *pch, exint nLength, int nMode = std::ios::out);
#endif

    /// Note: This is old-school streams!
    /// You own the result of str() and must either free() it or invoke
    /// freeze(false)!
    char		*str() { return myBuf.str(); }
    void		 freeze(bool isfrozen) { myBuf.freeze(isfrozen); }

    exint		 pcount() const
    {
	return myBuf.pcount();
    }

    // Return this properly cast so people can get at our freeze, etc.
    UT_OStrStreamBuf	*rdbuf() { return &myBuf; }

    virtual ~UT_OStrStream() {}

private:
    UT_OStrStreamBuf	myBuf;
};

// A temporary stream that uses UT_OStrStream by default unless the buffer
// size is so big that it should be saved on disk. Tries to balance
// performance with the practicality of having large buffers stored
// in memory.
//
// NOTE: this class is not thread safe
//
class UT_API UT_TempStream
{
public:
    // construct a temporary stream
    // size_estimate - apporximate anticipated size of the buffer. This
    //    value is used to determine whether the buffer shaould be
    //    kept in memory or saved to hard drive.
     UT_TempStream(exint size_estimate);
    ~UT_TempStream();

    // accesses the output stream for writing to the stream
    std::ostream	&getStream();

    // writes out the temp stream to an out stream
    void		 copyStream( std::ostream &out );

private:
    // TODO: implement a default constructor that uses UT_OStrStream
    // by default and automatically switches to file streams
    UT_TempStream();

private:
    
    UT_OStringStream	*myStrStream;	// memory buffer delegate
    std::fstream	*myFileStream;	// file buffer delegate
    UT_String		*myFileName;	// name of the temp file
};

#define DECLARE_PORTABLE_RW(type) \
    UT_API UT_OStream &UTwrite( \
			UT_OStream &os, const type *src, int64 numelem = 1); \
    UT_API std::ostream &UTwrite( \
			std::ostream &os, const type *src, int64 numelem = 1); \
    UT_API int64 UTfread( FILE *f, type *dest, int64 numelem = 1 ); \
    UT_API int64 UTfwrite( FILE *f, const type *src, int64 numelem = 1 ); \
/**/
DECLARE_PORTABLE_RW(char)
DECLARE_PORTABLE_RW(unsigned char)
DECLARE_PORTABLE_RW(signed char)
DECLARE_PORTABLE_RW(short)
DECLARE_PORTABLE_RW(unsigned short)
DECLARE_PORTABLE_RW(int)
DECLARE_PORTABLE_RW(unsigned int)
DECLARE_PORTABLE_RW(int64)
DECLARE_PORTABLE_RW(uint64)
#undef DECLARE_PORTABLE_RW

/// Write a floating point value to given destination type, performing a
/// conversion as necessary.
// @{
#define DECLARE_CONVERTIBLE_RW(T) \
    template <typename DEST_TYPE> UT_API \
    UT_OStream & UTwrite(UT_OStream &os, const T *src, int64 cnt = 1); \
    template <typename DEST_TYPE> UT_API \
    std::ostream & UTwrite(std::ostream &os, const T *src, int64 cnt = 1); \
    template <typename DEST_TYPE> UT_API \
    int64 UTfread(FILE *f, T *dest, int64 cnt = 1); \
    template <typename DEST_TYPE> UT_API \
    int64 UTfwrite(FILE *f, const T *src, int64 cnt = 1); \
/**/
DECLARE_CONVERTIBLE_RW(int32)
DECLARE_CONVERTIBLE_RW(int64)
DECLARE_CONVERTIBLE_RW(fpreal16)
DECLARE_CONVERTIBLE_RW(fpreal32)
DECLARE_CONVERTIBLE_RW(fpreal64)
// @}

// Save a chunk of binary data that may contain null characters.  Use
// UT_IStream::readBinaryData or UT_IStream::read(std::string &) to load
// it back in.
UT_API void UTsaveDataBinary(
    UT_OStream &os, const char *data, int64 size,
    UT_STRING_BINARY_IO minbits=UT_STRING_16BIT_IO);
UT_API void UTsaveDataBinary(
    std::ostream &os, const char *data, int64 size,
    UT_STRING_BINARY_IO minbits=UT_STRING_16BIT_IO);
static inline void UTsaveDataBinary(
    std::ostream &os, const std::string &data,
    UT_STRING_BINARY_IO minbits=UT_STRING_16BIT_IO)
{ UTsaveDataBinary(os, data.c_str(), data.size(), minbits); }
static inline void UTsaveDataBinary(
    std::ostream &os, const UT_StringHolder &data,
    UT_STRING_BINARY_IO minbits=UT_STRING_16BIT_IO)
{ UTsaveDataBinary(os, data.c_str(), data.length(), minbits); }

// Save a string in a binary format.
UT_API void  UTsaveStringBinary(
    std::ostream &os, const char *str, UT_STRING_BINARY_IO minbits);
UT_API void  UTsaveStringBinary(
    UT_OStream &os, const char *str, UT_STRING_BINARY_IO minbits);

// Return the number of bytes required by UTsaveStringBinary() to save a string
// of the specified length.
UT_API uint64 UTgetNBytesToSaveStringBinary(
    uint64 stringlength, UT_STRING_BINARY_IO minbits);

UT_API void UTbuildDOSCommandLine(char *dest, const char * const *args);
UT_API void UTbuildDOSCommandLine(UT_WorkBuffer &buf, const char * const *args);

// Abstract base class for filtering the data before it is sent to the output
// stream.
class UT_API UT_StreamBufferFilter
{
public:
    // virtual destructor for derived classes
    virtual	   ~UT_StreamBufferFilter()
		    {};

    // Often the filtering can efficently be performed only in multiple-byte
    // chunks, or the data may grow as a result of the filtering requiring that
    // the data is passed in smaller chunks than the actual buffer allocation
    // size. 
    // This method returns the the maximum input size (no larger than the
    // specified buffer size) with which the filter can work, given the buffer
    // size (sizes in bytes).
    virtual int	    getDataChunkLength( int buffer_size )
		    { return buffer_size; }

    // Before the the data is sent to a filter, the filter may need to
    // initialize itself and/or write out some preamble.
    // This method lets the filter write the preamble data to the
    // buffer (up to a buffer_size), and return the data size actually written.
    // The stream_size is the total size of the stream that is going to be
    // filtered chunk by chunk.
    virtual int	    beginFilter(char * /*buffer*/, int /*buffer_size*/,
				int /*stream_size*/)
		    { return 0; }

    // Performs the filtering of the data in the buffer. The data size is
    // 'data_size' which may grow, shrink, or stay the same after filtering.
    // The filtered data length is returned by the method.
    virtual int	    doFilterChunk( char *buffer, int data_size, 
				   int buffer_size ) = 0;

    // After all the data has been sent to a filter, the filter may need
    // to return some remaining filtered data, which it did not have a chance
    // to process.
    // This method lets the filter write the filtered data to the
    // buffer (up to a buffer_size), and return the data size actually written.
    virtual int	    endFilter( char * /*buffer*/, int /*buffer_size*/ )
		    { return 0; }
};

// Writes out the buffer to the output stream, but before it is written out
// the buffer is filtered using the given filter. The 'size' specifies the 
// length of the data to be filtered and written out.
UT_API bool UTwriteFiltered( std::ostream &os, const char *data, int size,
			     UT_StreamBufferFilter * filter );

// Copies the entire contents of the input stream into the output stream.
// Up to 'maxlen' bytes are read from the input stream. If the filter is
// specified, then the bytes read from the input stream are filtered before
// writing them to the output stream. If the filter is applied then the number 
// of written bytes may be different than the number of bytes read.
UT_API bool UTcopyStreamToStream(std::istream &is, std::ostream &os,
				 size_t maxlen = ((size_t)-1),
				 UT_StreamBufferFilter * filter = NULL);
UT_API bool UTcopyStreamToStream(UT_IStream &is, std::ostream &os,
				 size_t maxlen = ((size_t)-1),
				 UT_StreamBufferFilter * filter = NULL);

// Creates a unique temp file name. This name may conflict with an existing
// file, but the name will be unique among all _running_ Houdinis. In other
// words, it is safe to mindlessly overwrite the file with the given name.
UT_API void UTcreateTempFileName(UT_String &tempfile, const char *matchExt = 0);
UT_API void UTcreateTempFileName(UT_WorkBuffer &tempfile, const char *matchExt = 0);
UT_API void UTcreateTempFileName(UT_StringHolder &tempfile, const char *matchExt = 0);

// Tries to create the directory that will be required to hold the specified
// file.  Returns true if the dirctory exists of was created.
UT_API bool UTcreateDirectoryForFile(const char *filename);

// This function tells you whether or not you would block if you tried to
// read data from a file descriptor/the standard input.
UT_API bool UTisDataAvailableOnFD(int fd);
UT_API bool UTisDataAvailableOnStdin();
UT_API bool UTisStdinValid();

// This function will wait until either data is available on a file descriptor
// /stdin or until the specified timeout has passed.  The type of argument to
// the timeout is the same as the type of argument to sginap (10ms per clock
// tick).
UT_API void UTwaitForTimeoutOrDataOnFD(long clock_ticks, int fd);
UT_API void UTwaitForTimeoutOrDataOnStdin(long clock_ticks);

#endif 	// __UT_NTStreamUtil_H__
