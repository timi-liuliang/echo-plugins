/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_Writer.h ( FS Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __FS_Writer__
#define __FS_Writer__

#include "FS_API.h"
#include <UT/UT_NonCopyable.h>
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>
#include <iosfwd>
#include "FS_WriterStream.h"

class FS_WriterHelper;

/// Class for writing files
class FS_API FS_Writer : UT_NonCopyable
{
public:
    /// Use this constructor to create a new writer. The specified file is
    /// created immediately.
	     FS_Writer(const char *dest);

    virtual ~FS_Writer();

    /// Get the amount of memory owned by this FS_Writer
    virtual int64 getMemoryUsage(bool inclusive) const;

    /// Closes the stream. After calling this function, the getStream()
    /// function will always return @c NULL.
    void		 close();

    /// This function retrieves the C++ stream object which you can interact
    /// with in the normal ways. If this function returns @c NULL, the file
    /// specified in the constructor could not be created.
    std::ostream	*getStream() const;

    bool		 removeFile();

    // Functions for adding and removing helpers.
    static void		 addWriterHelper(FS_WriterHelper *helper);
    static void		 removeWriterHelper(FS_WriterHelper *helper);

private:
    void		 createStreamFromDest(const char *dest);

    static UT_ValArray<FS_WriterHelper *>	&getHelpers();

    FS_WriterStream				*myStream;
};

/// This class provides a plug-in method for adding a custom "file-system"
/// @see FS_ReaderHelper, FS_InfoHelper
class FS_API FS_WriterHelper
{
public:
	     FS_WriterHelper()
	     { FS_Writer::addWriterHelper(this); }
    virtual ~FS_WriterHelper()
	     { FS_Writer::removeWriterHelper(this); }

    /// Return an FS_WriterStream if the helper is able to open the source
    /// filename.
    virtual FS_WriterStream	*createStream(const char *source) = 0;
};

#endif
