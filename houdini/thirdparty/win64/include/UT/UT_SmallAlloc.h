/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SmallAlloc.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_SmallAlloc__
#define __UT_SmallAlloc__

#include "UT_API.h"
#include <stddef.h>
#include <string.h>
#include <SYS/SYS_Types.h>

/// UT_SmallAlloc manages the allocation of small (and possibly variable
/// sized) blocks of memory.  Memory is stored in pages of a specified size
/// with one wasted pointer at the beginning of the block storing a link
/// to the next page.  Memory is deallocated on destruction.
class UT_API UT_SmallAlloc {
public:
     UT_SmallAlloc(size_t page_size = 1024);
    ~UT_SmallAlloc();
   
    // Allocates an array with size elements of type T
    template <typename T>
    T			*allocArray(exint size)
			 { return static_cast<T *>(alloc(size*sizeof(T))); }

    // Creates a copy of the given string
    const char		*allocString(const char *str)
			 {
			     char *copy = allocArray<char>(strlen(str)+1);
			     strcpy(copy, str);
			     return copy;
			 }

    // Raw interface to allocate a block of the given number of bytes
    void		*alloc(size_t bytes);

    // Clear all memory currently allocated by this object
    void		 clear();

    int64		 getMemoryUsage() const	{ return myTotalUsage; }

private:
    void		 init();

private:
    size_t		 myPageSize;
    size_t		 myHalfPageSize;
    size_t		 myHeadOffset;
    void		*myHead;
    void		*myTail;
    size_t		 myTotalUsage;
};

#endif

