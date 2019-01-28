/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_Memory.h ( GT Library, C++)
 *
 * COMMENTS:	Class to keep track of memory usage
 */

#ifndef __GT_Memory__
#define __GT_Memory__

#include "GT_API.h"
#include <UT/UT_Memory.h>

enum GT_MemUser {
    GT_MEM_DEBUG = 0,		// This can be used to track down discrepancies

    GT_MEM_CACHE,

    GT_MEM_STRUCTS,		// Book-keeping structures

    GT_MEM_DATA,		// Memory used by data arrays
    GT_MEM_PRIMITIVE,		// Memory used by primitives
    GT_MEM_ATTRIBLIST,		// Attribute list memory

    GT_MEM_REFINE,		// Memory acquired during refinement

    GT_MEM_GEO,			// Geometry library memory users
    GT_MEM_USER,		// User primitive memory use

    GT_MEM_TRIM,		// Trim curves

    GT_MEM_XFORM,		// Transforms

    GT_MEM_MISC,		// Other memory usage

    GT_MAX_MEM_USERS	// Sentinal
};

class GT_API GT_Memory : UT_Memory {
public:
    SYS_MEMORY_SUBCLASS(static, , theGTMemoryTable)

private:
    static SYS_MemoryTable	theGTMemoryTable;
};

#endif
