/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SCFReader.h
 *
 * COMMENTS:
 */

#ifndef UT_SCFREADER_H_
#define UT_SCFREADER_H_

#include "UT_SCFCommon.h"
#include "UT_API.h"
#include "UT_IStream.h"
#include "UT_CompressedBlockIndex.h"
#include "UT_UniquePtr.h"

/// Interface for reading Seekable Compressed Format (SCF) files.
class UT_API UT_SCFReader
{
public:
    UT_SCFReader();
    ~UT_SCFReader();

    /// Retrieves a stream that the uncompressed data can be read from.
    /// it has been given the index if available and is fully seekable.
    /// @param filename The name of the file to open
    UT_IStream *getInputStream(const char* filename);

    /// Retrieves a stream that the uncompressed data can be read from.
    /// it has been given the index if available and is fully seekable.
    /// @param filename The stream to be used as raw input
    /// @return the stream to read decompressed data from or NULL if
    /// unable to open file.
    ///
    /// The input stream needs to exist for the lifetime of the blosc stream
    /// returned.  The UT_SCFReader doesn't take ownership of the input stream
    /// passed in.
    ///
    /// The UT_SCFReader @b does maintain ownership of the stream returned.
    /// You should not clear this stream unless you steal ownership of the
    /// streams (see stealStreams()).
    UT_IStream *getInputStream(UT_IStream* stream);

    /// Provided the SCFReader has already been opened with getInputStream,
    /// this allows you to regain the filtered stream that it owns.
    UT_IStream *getFilteredInputStream() const { return myFilteredInput.get(); }

    /// The UT_SCFReader typically takes ownership of two streams
    ///  - the filter stream (which is used to read the data)
    ///  - a "sub-stream" of the compressed data from the input stream
    /// The reader usually deletes these streams on closing/destruction.  If
    /// you steal these streams, then it's safe to destroy the UT_SCFReader.
    /// Though you become responsible for deleting the streams.
    ///
    /// It's possible the stream pointers may be NULL pointers if there have
    /// been errors, or they have been stolen already.
    ///
    /// @note You cannot steal streams if you acquired the input stream by
    /// passing a filename.
    void	stealStreams(UT_UniquePtr<UT_IStream> &filter,
                             UT_UniquePtr<UT_IStream> &substream);

    /// When you open a blosc stream, there are two 

    /// Closes the reader.
    void close();

private:

    /// Creates the filtered input stream that users read from.
    /// It verifies and reads the header, metadata and index.
    bool	prepareInputStream();

    /// Reads and verifies the magic number (assumed to be 4 characters)
    /// (stream must be at corrent position)
    bool verifyMagic(const char* magic);

    /// Reads the metadata section
    bool readMetadata();

    /// Reads the index section
    bool readIndexSection();


private:
    /// Raw input from file
    UT_IStream	*myRawInput;

    /// Substream of just the compressed blocks section data
    UT_UniquePtr<UT_IStream> mySubStream;

    /// Filtered input from compressed blocks section
    UT_UniquePtr<UT_IStream> myFilteredInput;

    /// Starting position of compressed blocks
    exint	myStartOfCompressedBlocks;

    /// Ending position of compressed blocks
    exint	myEndOfCompressedBlocks;

    /// The index containing the locations of the blocks
    UT_CompressedBlockIndexPtr	myIndex;

    /// Length of metadata section
    int64	myMetadataLength;

    /// @{
    /// Flags to indicate whether the corresponding streams should be deleted
    bool	myDeleteRawInputStream;
    /// @}
};

#endif // UT_SCFREADER_H_
