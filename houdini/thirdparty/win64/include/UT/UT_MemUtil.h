/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_MemUtil.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *	Memory utility functions, for doing various low-level memory
 *      operations. Currently only implemented for Linux.
 *	
 */
#ifndef UT_MEM_UTIL_H
#define UT_MEM_UTIL_H

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include <iosfwd>
#include <stddef.h>

enum UTmemAdvice {
    UT_MEM_ADVICE_NONE,		// no advice.
    UT_MEM_ADVICE_RANDOM,	// random order access - don't fetch ahead.
    UT_MEM_ADVICE_SEQ,		// sequential access - fetch ahead aggresively.
    UT_MEM_ADVICE_NEEDSOON,	// fetch ahead.
    UT_MEM_ADVICE_DONE		// done with the memory for now.
};

UT_API int UTmemAdvise(void *mem, size_t bytes, UTmemAdvice advice);

// like memset, except it'll work with any arbitrary byte size of data.
UT_API void UTmemsetChunk(void *dest, const void *value, exint valuesize,
			  exint ncopies);

// attempts to return the actual # of bytes that malloc would return for a
// block of 'size' bytes.
UT_API size_t UTmallocSize(size_t size);

UT_API void UTdumpHeap();
UT_API void UTdumpMemory(void *base);

UT_API void UTdumpHeapSize(std::ostream &os);

/// Attempt to free unused heap allocated memory.
UT_API void UTtrimUnusedHeapMemory();

#endif
