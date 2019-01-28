/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SmallObjectAllocator.h ( UT Library, C++)
 *
 * COMMENTS:	Small object allocator.
 */

#ifndef __UT_SmallObjectAllocator__
#define __UT_SmallObjectAllocator__

#include "UT_API.h"
#include <SYS/SYS_Types.h>

class UT_WorkBuffer;

class UT_API UT_SmallObjectAllocator
{
public:
    UT_SmallObjectAllocator(bool cleanPages,
		int pageSize,
		size_t maxObjectSize,
		bool threadSafe);
    ~UT_SmallObjectAllocator();

    static void *allocate(size_t bytes);
    static void	 deallocate(void *p, size_t bytes);

    static void *aligned_allocate(size_t bytes, size_t alginment);
    static void  aligned_free(void *p, size_t bytes);

    static void	 dumpStats(UT_WorkBuffer &buf);
};

#endif
