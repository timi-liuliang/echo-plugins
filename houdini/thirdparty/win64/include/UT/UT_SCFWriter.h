/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SCFWriter.h
 *
 * COMMENTS:
 */

#ifndef UT_SCFWRITER_H_
#define UT_SCFWRITER_H_

#include <SYS/SYS_BoostStreams.h>

#include "UT_SCFCommon.h"
#include "UT_API.h"
#include "UT_String.h"
#include "UT_Compression.h"

// Forward dels
class UT_BloscCompressionFilter;

/// Interface for writing Seekable Compressed Format (SCF) files.
class UT_API UT_SCFWriter
{
public:
    UT_SCFWriter(bool doShuffleing = true,
		    exint typeSize = 4,
		    exint blockSize = 65536,
		    UT_CompressionType type = UT_COMPRESSION_TYPE_BLOSC,
		    int compressionlevel = UT_SCF_BLOSC_COMPRESSION_LEVEL);
    ~UT_SCFWriter();

    /// Sets the compression type for the compressed blocks section
    /// of the SCF.
    void setCompressionType(UT_CompressionType type);

    /// Set's the size of each block in the SCF.
    /// The default block size is: <TODO: Fill this in with default block size>
    void setBlockSize(exint blockSize);

    /// Retrieves an output stream to write uncompressed data into
    /// the file. This is equivalent to open().
    /// @param filename Filename to write to.
    bios::filtering_ostream* getOutputStream(const char* filename);

    /// Retrieves an output stream to write uncompressed data into
    /// the file. This is equivalent to open().
    /// @param outStream Stream to write to.
    bios::filtering_ostream* getOutputStream(std::ostream* outStream);

    /// Closes the file.
    void close();

private:

    /// Writes the header and metadata then opens the compressed
    /// stream for writing
    void initalizeOutput();

    /// Opens a filtering outstream with current options
    void openCompressedBlocksStream();

    /// Writes header information to stream (also writes metadata)
    /// Header is | Magic # = scf1 | Metadata Length |
    void writeHeader();

    /// Writes the metadata section to stream
    void writeMetadata();

    /// Writes the index including the footer data
    void writeBlocksIndex(const UT_CompressedBlockIndex& index);

    /// Writes the footer data
    /// Footer is | Index Length | Magic # = 1fcs |
    void writeFooterData(int64 indexLength);

private:
    /// Flag set once stream is open
    bool myIsActive;

    /// Compression type to use for compressed blocks section
    UT_CompressionType myCompressionType;

    /// The block size for the compressed blocks section
    exint myBlockSize;

    /// True if we should do shuffling (blosc only)
    bool myDoShuffleing;

    /// Typesize for shuffling (blosc only)
    exint myTypeSize;

    /// Compression level.
    int myCompressionLevel;

    /// Our filtered output stream to compressed blocks section
    bios::filtering_ostream* myFilteredOutput;

    /// Our raw output stream
    std::ostream* myRawOutput;

    /// We somtimes create the output stream ourselfs, if we do
    /// it is assigned here to be deleted.
    std::ostream* myOutputToDelete;
};

#endif // UT_SCFWRITER_H_
