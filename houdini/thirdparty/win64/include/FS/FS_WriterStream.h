/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_WriterStream.h ( FS Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __FS_WriterStream__
#define __FS_WriterStream__

#include "FS_API.h"
#include <iosfwd>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_String.h>
#include <UT/UT_UniquePtr.h>

// This class is essentially a holder for an ostream object. However, it
// can be overridden with a custom implementation if some cleanup code needs
// to be run for a particular FS_Writer implementation.
// This class also knows how to write to index file section (see FS_IndexFile),
// if the file name contains the section name separator, '?', by creating a
// temporary file (myTempFile) for the section, and then embedding that section
// in the final index file in destroy() method.

class FS_API FS_WriterStream : UT_NonCopyable
{
public:
    /// Default constructor, sets myStream to NULL.
	     FS_WriterStream();
    /// Convenience constructor for files on disk. Basically just calls init().
	     FS_WriterStream(const char *file);
    /// Destructor. Calls destroy() virtual, but since this is a destructor, 
    /// any overriding destroy() methods in derived classes won't be called 
    /// and should be explicitly invoked in derived class's destructor.
    virtual ~FS_WriterStream();

    /// Get the amount of memory owned by this FS_WriterStream
    virtual int64 getMemoryUsage(bool inclusive) const;

    /// Obtains the output stream.
    std::ostream	*getStream() const;

    /// Deletes the file, if possible.
    bool		 removeFile();

protected:
    /// Sets myFile member to file argument and creates myStream output stream.
    /// If file refers to a section in an index file (ie, using standard
    /// convention, the file name contains the '?' section separator character),
    /// then it creates a temporery file
    /// (whose path is stored in myTempFile) and a stream for that temporary
    /// file (represented by myStream), where the section data is being written
    /// out. In destroy(), the temp file is embedded into the final destination
    /// index file and then removed.
    virtual bool	 init(const char *file);
    
    /// Destroys the output stream and removes file. 
    /// If the stream refers to a section in an index file, the data is flushed
    /// out and the new section is embedded in the index file.
    virtual bool	 destroy(bool removefile);

protected:
    /// File path for the output stream. It can be an empty string if 
    /// stream does not refer to a file.
    UT_String		 myFile;

    /// Path to the temporary file, if the original file path refers to a
    /// section in an index file. This temp file holds new data written to that
    /// section.
    UT_String		 myTempFile;

    /// The stream object used for writing out data. This class creates
    /// default stream for disk files, but derived classes can override init()
    /// and destroy() to create custom output streams.
    std::ostream	*myStream;
#ifdef WIN32
    /// This is a write buffer for Windows, because it doesn't seem to use a
    /// large enough one by default.
    UT_UniquePtr<char[]> myTempBuf;
#endif
};

#endif

