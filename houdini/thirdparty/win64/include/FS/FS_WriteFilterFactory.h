/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_WriteFilterFactory.h ( FS Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __FS_WriteFilterFactory__
#define __FS_WriteFilterFactory__

#include "FS_API.h"
#include <SYS/SYS_Types.h>

class UT_StreamBufferFilter;
class FS_IStreamFilterFactory;

// This class provides means to create filters that are applied when the index
// file writes itself out to the output stream.
class FS_API FS_WriteFilterFactory
{
public:
    // Standard constructor and destructor.
	     FS_WriteFilterFactory(); 
    virtual ~FS_WriteFilterFactory();

    /// Get the amount of memory owned by this FS_WriteFilterFactory
    virtual int64 getMemoryUsage(bool inclusive) const = 0;

    /// This method instantiates an write filter. The filter is used on each
    /// section, when the index file writes them out to the output stream.
    virtual UT_StreamBufferFilter	*createBufferFilter() const = 0;

    /// Instantiates a identical copy of this class object. 
    virtual FS_WriteFilterFactory	*cloneFilterFactory() const  = 0;

    /// Returns the size of filtered data based on its original unfiltred size. 
    virtual int		 getFilteredSize( int size );

    /// Returns true if this write filter matches the specified read filter.
    virtual bool	 matches(const FS_IStreamFilterFactory *f) const = 0;
};

#endif

