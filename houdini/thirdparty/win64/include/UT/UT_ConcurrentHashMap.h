/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ConcurrentHashMap.h (UT Library, C++)
 *
 * COMMENTS:	Wrapper around Intel TBB's concurrent_hash_map container.
 *		Microsoft will probably have an implementation of this in their
 *		Concurrent Runtime Parallel Patterns Library (PPL) since they
 *		already have concurrent_vector and concurrent_queue.
 *
 */

#ifndef __UT_CONCURRENTHASHMAP_H_INCLUDED__
#define __UT_CONCURRENTHASHMAP_H_INCLUDED__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include <tbb/concurrent_hash_map.h>
#include <utility>

// Due to current C++ inadequacies, we have to wrap the TBB template classes
// using a define
#define UT_ConcurrentHashMap	tbb::concurrent_hash_map

template<typename K, typename V, typename H, typename A>
int64
UTgetMemoryUsage(const UT_ConcurrentHashMap<K, V, H, A> &map, const bool inclusive)
{
    int64 mem = 0;
    if (inclusive)
        mem += sizeof(map);

    // The size of the buckets (a mutex and a pointer to the first node)
    mem += map.bucket_count()*(sizeof(intptr_t) + sizeof(void *));
    // The size of the nodes (a mutex, a pointer to the next node,
    // and an std::pair<K,V> forthe key and value)
    mem += map.size()*(sizeof(intptr_t) + sizeof(void *) + sizeof(std::pair<K,V>));

    return mem;
}

#endif // __UT_CONCURRENTHASHMAP_H_INCLUDED__
