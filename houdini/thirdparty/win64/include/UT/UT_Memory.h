/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Memory.h ( UT Library, C++)
 *
 * COMMENTS:	Memory tracking utilities
 */

#ifndef __UT_Memory__
#define __UT_Memory__

#include "UT_API.h"
#include <SYS/SYS_Memory.h>

enum UT_MemoryUser {
    UT_MEM_DEBUG = 0,

    UT_MEM_SYMBOLTABLE,
    UT_MEM_ARRAY,
    UT_MEM_FMALLOC,
    UT_MEM_FMALLOC_PAGE,
    UT_MEM_RINGBUFFER,
    UT_MEM_TAGLIST,
    UT_MEM_SPLINE,
    UT_MEM_JSON,

    UT_MEM_MAX_ENTRIES		// Sentinal
};

class UT_WorkBuffer;

class UT_API UT_Memory {
public:
    SYS_MEMORY_SUBCLASS(static, , theUTMemoryTable)

    /// Print the memory in a "nice" (well at least standard) format.  The
    /// buffer passed in should be at least 80 characters long.
    static void	printMemory(char *buffer, const char *label,
					size_t used, size_t peak,
					int indent=3);

    /// Print an entire table of memory users.  This printer assumes that
    /// each user's label will fit into a 15 character buffer (for formatting)
    /// The total memory accounted for is returned.
    static size_t	printMemory(UT_WorkBuffer &buffer,
				    SYS_MemoryTable &table,
				    int indent=3,
				    bool sort_by_peak=false);


private:
    static SYS_MemoryTable	theUTMemoryTable;
};

#endif
