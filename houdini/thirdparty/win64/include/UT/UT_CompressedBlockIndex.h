/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_CompressedBlockIndex.h
 *
 * COMMENTS:
 */

#ifndef UT_COMPRESSEDBLOCKINDEX_H_
#define UT_COMPRESSEDBLOCKINDEX_H_

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_IntrusivePtr.h"

class UT_IStream;

typedef int64 BlockLocationType;

class UT_API UT_CompressedBlockIndex
    : public UT_IntrusiveRefCounter<UT_CompressedBlockIndex>
{
public:
    UT_CompressedBlockIndex();
    ~UT_CompressedBlockIndex();

    /// Sets the blocksize and the size of the last block.
    /// Last block size used for end of stream calculations.
    void setBlockSize(exint blockSize, exint lastBlockSize);

    /// Adds entry to the end of the index
    void append(BlockLocationType entry);

    /// Removes the last entry
    void removeLast();

    /// Get's the total uncompressed size of the stream with this block index
    exint getTotalUncompressedSize();

    /// Retrieves the location of the start of the block containing
    /// the given offset in uncompressed file
    BlockLocationType getCompressedBlockOffset(exint uncompressedOffset);

    /// Retrieves the location of the start of the block containing
    /// the given offset (given from the end of the file) in uncompressed file
    BlockLocationType getCompressedBlockOffsetFromEnd(exint uncompressedOffsetFromEnd);

    /// Get the block number given offset in compressed file
    exint getBlockNumberCompressedOffset(BlockLocationType compressedOffset);

    /// Get the length in the file of the entire index section (for seeking)
    int64 getLength();

    /// Get the block size of the compressed blocks
    exint getBlockSize();

    /// Get the size of the last compressed block
    exint getLastBlockSize();


    /// Write index to stream
    void writeToStream(std::ostream* out);

    /// Read index from stream
    void readFromStream(UT_IStream* in, int64 indexLength);

    /// Dump information
    void	dump(const char *msg="") const;

private:
    UT_Array<BlockLocationType> myIndex;

    exint myBlockSize;

    exint myLastBlockSize;
};

typedef UT_IntrusivePtr<UT_CompressedBlockIndex> UT_CompressedBlockIndexPtr;

#endif // UT_COMPRESSEDBLOCKINDEX_H_
