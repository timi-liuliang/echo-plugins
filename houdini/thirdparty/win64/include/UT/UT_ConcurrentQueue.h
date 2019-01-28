/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ConcurrentQueue.h (UT Library, C++)
 *
 * COMMENTS:	Wrapper around Intel TBB's concurrent_queue container.
 */

#ifndef __UT_CONCURRENTQUEUE_H_INCLUDED__
#define __UT_CONCURRENTQUEUE_H_INCLUDED__

#include "UT_API.h"
#include <tbb/concurrent_queue.h>
#include <SYS/SYS_Types.h>

// Due to current C++ inadequacies, we have to wrap the TBB template classes
// using a define
/// @see UT_ConcurrentVector
#define UT_ConcurrentQueue	tbb::concurrent_queue

template<typename T, typename A>
int64
UTgetMemoryUsage(const UT_ConcurrentQueue<T, A> &queue, const bool inclusive)
{
    int64 mem = 0;
    if (inclusive)
        mem += sizeof(queue);

    // NOTE: The internals of tbb::concurrent_queue are a bit of a mystery, so
    //       just count what we know is there, for now.
    mem += queue.unsafe_size()*sizeof(T);

    return mem;
}

#endif // __UT_CONCURRENTQUEUE_H_INCLUDED__
