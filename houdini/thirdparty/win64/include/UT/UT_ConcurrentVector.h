/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ConcurrentVector.h (UT Library, C++)
 *
 * COMMENTS:	Wrapper around Intel TBB's concurrent_vector container.
 *
 */

#ifndef __UT_CONCURRENTVECTOR_H_INCLUDED__
#define __UT_CONCURRENTVECTOR_H_INCLUDED__

#include "UT_API.h"
#include <SYS/SYS_BitUtil.h>
#include <SYS/SYS_Types.h>
#include <tbb/concurrent_vector.h>

// Due to current C++ inadequacies, we have to wrap the TBB template classes
// using a define
/// @see UT_ConcurrentQueue
#define UT_ConcurrentVector	tbb::concurrent_vector

template<typename T, typename A>
int64
UTgetMemoryUsage(const UT_ConcurrentVector<T,A> &that, bool inclusive)
{
    int64 mem = inclusive ? sizeof(that) : 0;

    // The data content
    mem += that.capacity() * sizeof(T);

    // NOTE: This is based on TBB using an array of pointers to
    //       doubling-length arrays.  If the capacity is 8 or less, it uses
    //       3 pointers internal to the structure (2, 2, and 4 elements each).
    if (that.capacity() > 8)
        mem += SYSceilLog2(that.capacity()) * sizeof(void*);

    return mem;
}

#endif // __UT_CONCURRENTVECTOR_H_INCLUDED__
