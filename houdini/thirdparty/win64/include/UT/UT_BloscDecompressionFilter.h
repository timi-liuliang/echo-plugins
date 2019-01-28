/*
 * UT_BloscDecompressionFilter.h
 *
 *  Created on: May 1, 2014
 *      Author: cspeller
 */

#ifndef UT_BLOSCDECOMPRESSIONFILTER_H_
#define UT_BLOSCDECOMPRESSIONFILTER_H_

// Use UT_IStreamBuf blosc filter
#if 0

#include <SYS/SYS_BoostStreams.h>
#include <cstdio>
#include <blosc.h>


///
/// The blosc decompression filter is an implementation of boost multicharacter input filter
/// It can be used in boost input streams to read data compressed with blosc
///
class UT_BloscDecompressionFilter : public bios::multichar_input_filter
{
    /// Buffer for storing read blocks
    char* myInputBuffer;
    size_t myInputBufferSize;

    /// Buffer for result from blosc
    char* myOutputBuffer;
    size_t myOutputBufferSize;
    size_t myOutputBufferFlushPosition;
    size_t myOutputBufferUsedSize;

public:
    UT_BloscDecompressionFilter();
    virtual ~UT_BloscDecompressionFilter();

    template<typename Source>
    std::streamsize read(Source& src, char* destinationBuffer,
	    std::streamsize destinationSize);

    template<typename Source>
    void close(Source&) {}
};

inline UT_BloscDecompressionFilter::UT_BloscDecompressionFilter() :
	myInputBuffer(NULL), myInputBufferSize(0), myOutputBuffer(NULL),
	myOutputBufferSize(0), myOutputBufferFlushPosition(0), myOutputBufferUsedSize(0)
{
}

inline UT_BloscDecompressionFilter::~UT_BloscDecompressionFilter()
{
    if (myInputBuffer)
    {
	delete myInputBuffer;
    }
    if (myOutputBuffer)
    {
	delete myOutputBuffer;
    }
}

template<typename Source>
inline std::streamsize UT_BloscDecompressionFilter::read(Source& src,
	char* destinationBuffer, std::streamsize destinationSize)
{
    // Allocate input buffer if not allocated yet
    // This is done because BOOST copies us around a bit before
    // using the filter.
    if (!myInputBuffer)
    {
	myInputBuffer = new char[BLOSC_MIN_HEADER_LENGTH];
        myInputBufferSize = BLOSC_MIN_HEADER_LENGTH;
    }

    // If there is nothing to flush
    if (myOutputBufferUsedSize == myOutputBufferFlushPosition)
    {
	// Reset flush position
	myOutputBufferFlushPosition = 0;
	myOutputBufferUsedSize = 0;

	// Read the blosc header
	if (bios::read(src, myInputBuffer, BLOSC_MIN_HEADER_LENGTH) == -1)
	{
	    return -1;
	}

	// Retrieve metadata about the current blosc block
	size_t uncompressedSize;
	size_t compressedSize;
	size_t blockSize;
	blosc_cbuffer_sizes(myInputBuffer, &uncompressedSize, &compressedSize,
		&blockSize);

	// Make sure buffer is big enough to hold all we are going to read
	if (myInputBufferSize < BLOSC_MIN_HEADER_LENGTH + compressedSize)
	{
	    char* oldBuffer = myInputBuffer;
	    myInputBuffer = new char[blockSize + BLOSC_MAX_OVERHEAD];
	    memcpy(myInputBuffer, oldBuffer, BLOSC_MIN_HEADER_LENGTH);
	    delete oldBuffer;
	    myInputBufferSize = blockSize + BLOSC_MAX_OVERHEAD;
	}

	// Make sure output buffer is big enough for all possible output data
	if (myOutputBufferSize < uncompressedSize)
	{
	    if (myOutputBuffer)
	    {
		delete myOutputBuffer;
	    }
	    myOutputBuffer = new char[uncompressedSize];
	    myOutputBufferSize = uncompressedSize;
	}

	// Read compressed chunk into input buffer
	bios::read(src, myInputBuffer + BLOSC_MIN_HEADER_LENGTH,
		compressedSize - BLOSC_MIN_HEADER_LENGTH);

	// Uncompress chunk into destination buffer
	int finalBlocksize = blosc_decompress(myInputBuffer, myOutputBuffer,
		myOutputBufferSize);

	// Error check
	if (finalBlocksize <= 0)
	{
	    std::cerr << "Unable to decompress block." << std::endl;
	    std::cerr << "Compressed Size: " << compressedSize << std::endl;
	    std::cerr << "Uncompressed Size: " << uncompressedSize << std::endl;
	    std::cerr << "Block Size: " << blockSize << std::endl;
	    std::cerr << "Error Code: " << finalBlocksize << std::endl;
	    return -1;
	}

	// Add block size to ammount in output buffer
	myOutputBufferUsedSize += finalBlocksize;
    }
    // Flush output if available
    size_t ammountToFlush = std::min((size_t) (destinationSize),
	    myOutputBufferUsedSize - myOutputBufferFlushPosition);
    memcpy(destinationBuffer, myOutputBuffer + myOutputBufferFlushPosition,
	    ammountToFlush);
    myOutputBufferFlushPosition += ammountToFlush;
    // Return how many bytes we wrote to destination
    return ammountToFlush;
}

#endif

#endif /* UT_BLOSCDECOMPRESSIONFILTER_H_ */
