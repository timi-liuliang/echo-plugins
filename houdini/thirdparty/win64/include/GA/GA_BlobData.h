/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_BlobData.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_BlobData__
#define __GA_BlobData__

#include "GA_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_IntrusivePtr.h>

#include <stddef.h>

class GA_LoadMap;
class GA_SaveMap;
class UT_JSONParser;
class UT_JSONWriter;
class UT_MemoryCounter;
class UT_WorkBuffer;


/// Class representing an opaque blob of data stored in an attribute
/// @see GA_BlobRef, GA_BlobContainer, GA_ATIBlob
class GA_API GA_BlobData 
    : public UT_IntrusiveRefCounter<GA_BlobData>
{
public:
	     GA_BlobData();
    virtual ~GA_BlobData();

    /// @{
    /// The new & delete operators are overloaded to help with memory tracking.
    // Allocat new object
    static void	*operator	new(size_t size);
    // In-place construction
    static void	*operator	new(size_t size, void *p);
    // Deletion of allocated object
    static void  operator	delete(void *p, size_t size);
    // Get memory statistics
    static void			getAllocationStats(int64 &used, int64 &peak);
    /// @}

    /// Create a hash
    virtual uint	hash() const = 0;

    /// Return true if this blob is equal to the other blob
    virtual bool	isEqual(const GA_BlobData &blob) const = 0;

    /// Save blob data to a JSON stream.  By default, this saves a Null.
    virtual bool	jsonSave(UT_JSONWriter &w, const GA_SaveMap *s) const;
    /// Load blob data from a JSON stream.  By default, this skips the next
    /// block of data.
    virtual bool	jsonLoad(UT_JSONParser &p, const GA_LoadMap *m);

    /// Call jsonSave on the work buffer to create a simple representation of
    /// the blob data.
    void		repr(UT_WorkBuffer &wbuf) const;

    /// Report approximate memory usage
    virtual int64	getMemoryUsage(bool inclusive) const = 0;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    virtual void	countMemory(UT_MemoryCounter &counter, bool inclusive) const = 0;

protected:
    /// This is needed by subclasses int order to implement countMemory
    exint		getRefCount() const { return use_count(); }
};

using GA_BlobRef = UT_IntrusivePtr<GA_BlobData>;

#endif
