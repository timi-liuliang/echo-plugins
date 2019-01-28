/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_BloscCompressionFilter.C (C++)
 *
 * COMMENTS:	.
 */

#ifndef UT_BLOSCCOMPRESSIONFILTER_H_
#define UT_BLOSCCOMPRESSIONFILTER_H_

#include <SYS/SYS_BoostStreams.h>
#include <cstdio>
#include <iostream>
#include <blosc.h>
#include "UT_SCFCommon.h"

#include "UT_Assert.h"


///
/// The blosc compression filter is an implementation of boost multicharacter output filter
/// It can be used in boost output streams to compress data using blosc on write.
///
class UT_BloscCompressionFilter : public bios::multichar_output_filter
{
    /// The number of bytes for the atomic type in the binary input stream
    size_t myTypeSize;

    /// Block size for compression
    size_t myBlockSize;

    /// The BLOSC compression level (0-9)
    int myCompressionLevel;

    /// Whether or not to do the shuffling
    bool myDoShuffle;

    /// Buffer for holding input and output
    char* myInputBuffer; // Size = blockSize
    char* myOutputBuffer; // size = blockSize+BLOSC_MAX_OVERHEAD
    size_t myInputBufferUsage;

    /// Current location in compressed file
    int64 currentLocation;

    /// Location of compressed blocks
    UT_CompressedBlockIndex index;

public:
    typedef char char_type;

    /// Blosc Compression Filter
    /// Construct and push onto boost stream.
    /// @param[in] typeSize The byte size of the atomic type in the binary stream
    /// @param[in] blockSize The block size to force blosc to use.
    /// @param[in] doShuffle True to run the shuffler. Default = true
    /// @param[in] compressionLevel The compression level to give blosc
    /// possible values are [1-9] with 9 being the most compression.
    explicit UT_BloscCompressionFilter(size_t typeSize, size_t blockSize,
	    bool doShuffle = true, int compressionLevel = 9);
    virtual ~UT_BloscCompressionFilter();

    /// Retrieves the list of locations in the file where the blocks are
    UT_CompressedBlockIndex getIndex();

    /// Retrieves the current location in the file
    exint getCurrentLocation();

    /// Compresses all data available in the input buffer. Writing
    /// it to the output buffer. Returns the compressed size if the
    /// data put in the output buffer.
    std::streamsize doCompression();

    template<typename Sink>
    std::streamsize write(Sink& dest, const char* sourceBuffer,
	    std::streamsize sourceBufferSize);

    template<typename Sink>
    void close(Sink& dest);
};

inline UT_BloscCompressionFilter::UT_BloscCompressionFilter(
	size_t typeSize, size_t blockSize, bool doShuffle, int compressionLevel) :
	myTypeSize(typeSize), myBlockSize(blockSize), myCompressionLevel(
		compressionLevel), myDoShuffle(doShuffle), myInputBuffer(NULL), myOutputBuffer(
		NULL), myInputBufferUsage(0), currentLocation(0), index()
{
}

inline UT_BloscCompressionFilter::~UT_BloscCompressionFilter()
{
    delete[] myInputBuffer;
    delete[] myOutputBuffer;
}

inline UT_CompressedBlockIndex UT_BloscCompressionFilter::getIndex()
{
    return index;
}

inline exint UT_BloscCompressionFilter::getCurrentLocation()
{
    return currentLocation;
}

inline std::streamsize UT_BloscCompressionFilter::doCompression()
{
    // Compress buffer and write to output buffer
    std::streamsize compressedSize = blosc_compress_ctx(myCompressionLevel,
	    myDoShuffle ? 1 : 0, myTypeSize, myInputBufferUsage, myInputBuffer,
	    myOutputBuffer, myBlockSize + BLOSC_MAX_OVERHEAD, "lz4", myBlockSize, 1);
    // Clear the input buffer
    myInputBufferUsage = 0;
    // Blosc returns -1 on failure to compress
    // Returns 0 if not enough buffer to compress
    if (compressedSize == -1)
    {
	std::cerr << "Problem compressing, blosc returned: " << compressedSize
		<< std::endl;
	return -1; // Give up (EOF)
    }
    return compressedSize;
}

template<typename Sink>
inline std::streamsize UT_BloscCompressionFilter::write(Sink& dest,
	const char* sourceBuffer, std::streamsize sourceBufferSize)
{
    // Allocate input and output buffer if not allocated yet
    // This is done because BOOST copies us around a bit before
    // using the filter.
    if (!myInputBuffer) {
	UT_ASSERT(myOutputBuffer == NULL);
	myInputBuffer = new char[myBlockSize];
	myOutputBuffer = new char[myBlockSize + BLOSC_MAX_OVERHEAD];
    }

    // Keep original buffer size as we need to return it
    std::streamsize totalRead = sourceBufferSize;
    // While we have stuff to read
    while (sourceBufferSize > 0)
    {
	// Read all available characters until input buffer is full
	// or we run out of characters
	std::streamsize ammountToCopy = std::min(sourceBufferSize,
		(std::streamsize) ((myBlockSize - myInputBufferUsage)));
	memcpy(myInputBuffer + myInputBufferUsage, sourceBuffer, ammountToCopy);
	myInputBufferUsage += ammountToCopy;
	// Modify size available to read and increment source buffer
	sourceBufferSize -= ammountToCopy;
	sourceBuffer += ammountToCopy;
	// If our buffer is full
	if (myInputBufferUsage == myBlockSize)
	{
	    // Run compression
	    std::streamsize compressedSize = doCompression();
	    // -1 indicates an error occoured so return -1 (signal eof)
	    if (compressedSize == -1)
	    {
		return -1;
	    }
	    // Write the compressed block to the stream
	    bios::write<Sink>(dest, myOutputBuffer, compressedSize);

	    // Increment our counter of file location
	    currentLocation += compressedSize;

	    // Write out location of next block
	    index.append(currentLocation);
	}
    }
    UT_ASSERT(sourceBufferSize == 0);
    return totalRead;
}

template<typename Sink>
inline void UT_BloscCompressionFilter::close(Sink& dest)
{
    // Compress the rest of the data we have
    if (myInputBufferUsage > 0)
    {
	// Remember the uncompressed size so we can give to index
	std::streamsize uncompressedSize = myInputBufferUsage;

	// Run compression
	std::streamsize compressedSize = doCompression();

	// Write the final block to the sink
	bios::write<Sink>(dest, myOutputBuffer, compressedSize);

	// Increment our current location so we can return an
	// accurate final size
	currentLocation += compressedSize;

	// The index needs to know how big the final block is
	index.setBlockSize(myBlockSize, uncompressedSize);

	// NOTE: We don't write out the final index here because
	// we write out the index in advance!
    } else
    {
	// In this case (where we have a perfect multiple of blocksize)
	// we write an index location that was not actually present.
	// So we remove it.
	index.removeLast();

	// and we call the last block to be normal sized
	index.setBlockSize(myBlockSize, myBlockSize);
    }
}

#endif /* UT_BLOSCCOMPRESSIONFILTER_H_ */
