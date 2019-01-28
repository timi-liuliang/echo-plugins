/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_ReaderStream.h ( FS Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __FS_ReaderStream__
#define __FS_ReaderStream__

#include "FS_API.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_WorkBuffer.h>

#include <time.h>

class UT_IStream;
class UT_Options;
class FS_IndexFile;
class FS_WriteFilterFactory;
class FS_IStreamFilterFactory;

// This class is essentially a holder for an istream object. However, it
// has a number of clever constructors.
// For most constructors, we create an istream (either an ifstream or
// istrstream) that belongs just to us. For the istrstream constructor, we take
// a data pointer, its length, and its modification time.  For the ifstream
// constructor, we take a file name.  Also, we can duplicate a source
// FS_ReaderStream.
class FS_API FS_ReaderStream
{
public:
	     // Standard constructor.
	     FS_ReaderStream();

	     // Creates a reader stream from a substream of another stream.
	     // The original stream's underlying buffer will be adopted by this
	     // reader stream, so both will share the same underlying data
	     // and current read position. Note, that usually the other
	     // (original) reader stream is not used for reading, and is usually
	     // destoryed right after constructing this reader stream, which
	     // specializes in reading a portion of the original stream.
	     //
	     // The factory is used to generate a filter for reading this
	     // stream data (starting at offset of the original stream and
	     // reading till data size is reached); it is _not_ the same as
	     // setting sub-stream filter factory via
	     // setSubStreamFilterFactory(). 
	     // If the factory is nullptr, no filtering is performed and the
	     // data is read as raw stream bytes.
	     FS_ReaderStream(FS_ReaderStream &src, int64 stream_offset, 
			     int64 stream_size, int64 data_size,
			     const FS_IStreamFilterFactory * factory);

	     // Creates a file reader stream.
	     FS_ReaderStream(const UT_StringHolder &file,
			     const UT_Options *options = nullptr);

	     // Creates a memory buffer reader stream.
	     FS_ReaderStream(const char *data, int len, int modtime,
			     const UT_StringHolder &file = UT_StringHolder());
	     // Creates a memory buffer reader stream that takes ownership
	     // of the buffer.
	     FS_ReaderStream(UT_WorkBuffer &buffer, int modtime,
			     const UT_StringHolder &file = UT_StringHolder());
	     virtual ~FS_ReaderStream();

    /// Get the amount of memory owned by this FS_ReaderStream
    virtual int64 getMemoryUsage(bool inclusive) const;

    /// Returns the actual input stream to read the data from.
    UT_IStream		*getStream() const { return myStream.get(); }
    bool		 isGood() const;

    const UT_StringHolder &getFilename() const { return myFile; }

    /// Returns the size of the data (ie, after filtering the raw stream).
    /// If no filtering is performed, this is the same as raw stream size.
    int64		 getLength() const { return myDataSize; }

    /// Accessor for modification time on the stream.
    /// @{
    void		 setModTime(time_t modtime) { myModTime = modtime; }
    time_t		 getModTime() const { return myModTime; }
    /// @}

    /// Sets the factory that should be used for this stream's substreams
    /// (but not for the whole stream). It is a way to piggyback the filter info
    /// to potential substream readers.
    void			setSubStreamReadFilterFactory(
					    FS_IStreamFilterFactory * f);
    FS_IStreamFilterFactory *	getSubStreamReadFilterFactory() const;
    void			setSubStreamWriteFilterFactory(
					    FS_WriteFilterFactory * f);
    FS_WriteFilterFactory *	getSubStreamWriteFilterFactory() const;

protected:
    // When the reader stream corresponds to the contents of the whole
    // FS_IndexFile, it is necessary to pass around the filtering information
    // (both read and write) so that the filtering can be applied to portions of
    // this stream (eg, substreams corresponding to FS_IndexFile sections).
    // The factory is not used by this class to instantiate filters, but rather
    // it is piggybacked to the parties interested in reading or writing such 
    // substreams.
    FS_IStreamFilterFactory *mySubStreamReadFilterFactory;
    FS_WriteFilterFactory   *mySubStreamWriteFilterFactory;

    UT_UniquePtr<UT_IStream> myStream;	// stream used for reading data
    UT_StringHolder	 myFile;	// file name containing original data
    int64		 myDataSize;	// size after filtering raw stream bytes
    time_t		 myModTime;	// last modification time
    UT_WorkBuffer	 myMemBuffer;	// buffer owned by this object
};

#endif

