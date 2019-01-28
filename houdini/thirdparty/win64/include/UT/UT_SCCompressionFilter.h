/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SCCompressionFilter.C (C++)
 *
 * COMMENTS:	.
 */

#ifndef UT_SCCOMPRESSIONFILTER_H_
#define UT_SCCOMPRESSIONFILTER_H_

#include <SYS/SYS_BoostStreams.h>
#include <cstdio>
#include <iostream>
#include "UT_SCFCommon.h"
#include "UT_SCFWriter.h"

/// The SC compression filter is an implementation of boost multicharacter
/// output filter.  It can be used with boost output streams to write a .sc
/// file to disk.
class UT_SCCompressionFilter : public bios::multichar_output_filter
{
public:
    typedef char char_type;

    static const size_t	theDefaultTypeSize = 4;
    static const size_t	theDefaultBlockSize = (1<<20);

    /// SC Compression Filter
    explicit UT_SCCompressionFilter(size_t typesize,
				size_t blocksize,
				bool doShuffer = true,
				int compressionLevel = 9);
    virtual ~UT_SCCompressionFilter();

    template<typename Sink>
    std::streamsize write(Sink& dest, const char* sourceBuffer,
				std::streamsize sourceBufferSize);

    template<typename Sink>
    void close(Sink& dest);

private:
    // Stuff to pass on to blosc
    size_t	myTypeSize;
    size_t	myBlockSize;
    int		myCompressionLevel;
    bool	myDoShuffle;

    UT_SCFWriter*		myOutputWriter;
    bios::filtering_ostream*	myOutputStream;
    std::ostream*		myOutputAdapter;
};


inline
UT_SCCompressionFilter::UT_SCCompressionFilter(
	size_t typeSize,
	size_t blockSize,
	bool doShuffle,
	int compressionLevel)
    : myTypeSize(typeSize)
    , myBlockSize(blockSize)
    , myCompressionLevel(compressionLevel)
    , myDoShuffle(doShuffle)
    , myOutputWriter(NULL)
    , myOutputStream(NULL)
{}

inline
UT_SCCompressionFilter::~UT_SCCompressionFilter()
{
    // This might not be nessisary can we be destroyed without a close?
    delete myOutputWriter;
}

template<typename Sink>
inline std::streamsize
UT_SCCompressionFilter::write(Sink& dest, const char* sourceBuffer,
	std::streamsize sourceBufferSize)
{
    if (!myOutputWriter)
    {
	myOutputWriter = new UT_SCFWriter(myDoShuffle, myTypeSize, myBlockSize,
					  UT_COMPRESSION_TYPE_BLOSC,
					  myCompressionLevel);
	myOutputAdapter = new std::ostream(&dest);
	myOutputStream = myOutputWriter->getOutputStream(myOutputAdapter);
    }

    if(!myOutputStream->write((const char*)sourceBuffer, sourceBufferSize))
    {
	return 0;
    }

    return sourceBufferSize;
}

template<typename Sink>
inline void
UT_SCCompressionFilter::close(Sink& dest)
{
    // Check for close without write
    if (myOutputWriter)
    {
	myOutputWriter->close();
	delete myOutputWriter;
	delete myOutputAdapter;
    }
    myOutputWriter = NULL;
}

#endif // UT_SCCOMPRESSIONFILTER_H_
