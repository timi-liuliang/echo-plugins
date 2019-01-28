/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Memory.h ( GA Library, C++)
 *
 * COMMENTS:	Class to keep track of memory usage
 */

#ifndef __GA_Memory__
#define __GA_Memory__

#include "GA_API.h"
#include <UT/UT_Memory.h>
#include <SYS/SYS_Memory.h>

/// This enum is used to classify different uses for memory allocation
enum GA_MemUser
{
    GA_MEM_DEBUG = 0,		// This can be used to track down discrepancies

    GA_MEM_MISC,		// Miscellaneous allocations
    GA_MEM_ATTRIBUTE,		// Memory used by attributes
    GA_MEM_ATTRIBUTE_DATA,	// Attribute data
    GA_MEM_ATTRIBUTE_FILTER,	// Rough memory used by attribute filters
    GA_MEM_GROUP,		// Group memory
    GA_MEM_INDEX_MAP,		// Index map memory usage
    GA_MEM_PRIMITIVE,		// Memory used by primitives
    GA_MEM_TEMPORARIES,		// Temporary storage
    GA_MEM_BLOBS,		// Attribute blobs
    GA_MEM_EDITDELTA,		// Memory allocated to recording edit deltas
    GA_MEM_GBPOINTREF,		// GB point ref
    GA_MEM_RANGE,		// Ranges/RangeTypeInterfaces

    GA_MAX_MEM_USERS	// Sentinal
};

/// Keeps track of memory used by the GA library
/// @see UT_Memory, SYS_Memory
/// @see enum::GA_MemUser
class GA_API GA_Memory : public UT_Memory 
{
public:
    SYS_MEMORY_SUBCLASS(static, , theGAMemoryTable)

private:
    static SYS_MemoryTable	theGAMemoryTable;
};

#endif
