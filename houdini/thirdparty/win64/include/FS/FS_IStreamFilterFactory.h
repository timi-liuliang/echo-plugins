/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_IStreamFilterFactory.h ( FS Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __FS_IStreamFilterFactory__
#define __FS_IStreamFilterFactory__

#include "FS_API.h"
#include <SYS/SYS_Types.h>

class UT_IStream;

// This class provides means to create filters for the input streams that
// underline the reader stream. It is used by the FS_ReaderStream to instantiate
// a filter whenever it needs to create its input stream.
class FS_API FS_IStreamFilterFactory
{
public:
    // Standard constructor and destructor.
	     FS_IStreamFilterFactory(); 
    virtual ~FS_IStreamFilterFactory();

    /// Get the amount of memory owned by this FS_IStreamFilterFactory
    virtual int64 getMemoryUsage(bool inclusive) const = 0;

    /// This method instantiates an input stream filter (which is itself an
    /// input stream). The filter will read raw data from the 'is', then
    /// process it and present the transformed data to the user of the
    /// filter, which will see it just as a regular input stream.
    virtual UT_IStream			*createIStreamFilter(
					    UT_IStream * is) const = 0;

    /// Instantiates a identical copy of this class object. 
    virtual FS_IStreamFilterFactory	*cloneFilterFactory() const  = 0;

    /// Obtains the size of the data encoded in the is, ie. the size of the data
    /// after the raw input stream bytes have been filtered.
    virtual int				 getDataSize(
					    UT_IStream * is,
					    int stream_size) const = 0;
};

#endif

