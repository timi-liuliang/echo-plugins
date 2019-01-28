/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: CE_MemoryPool.h ( CE Library, C++)
 *
 * COMMENTS: Compute Engine Memory Pool.
 */

#ifndef __CE_MemoryPool__
#define __CE_MemoryPool__

#include "CE_API.h"

#ifdef CE_ENABLED

#include <SYS/SYS_Types.h>

#include <UT/UT_Array.h>
#include <UT/UT_LRUCache.h>
#include <UT/UT_Map.h>
#include <UT/UT_Lock.h>

class CE_Context;

/// This class implements a memory pool for OpenCL buffer objects, using an
/// LRUCache to limit the size of the pool.
class CE_API CE_MemoryPool
{
public:
    /// Initialize the pool with specified maximum size.
    CE_MemoryPool(int64 size);
    virtual ~CE_MemoryPool();

    /// Empty the memory pool of all held buffers.
    void        clear();

    /// Set the maximums size of the pool.  This will prune the LRUCache as
    /// needed to fit the size.
    void        setMaxSize(int64 size) { myLRUCache.setMaxSize(size); }

    /// Returns the maximum size of the pool.
    int64       getMaxSize() const { return myLRUCache.maxSize(); }

    /// Dump memory usage statistics to stderr.
    void        reportUsage() const;

    /// Allocate a buffer for the provided context.  If a buffer of the
    /// specified size already exists in the pool, it will be returned.
    /// Otherwise a new one will be created on the device.
    cl::Buffer  allocBuffer(CE_Context *, int64 size);

    /// Check the provided buffer's reference count.  If it is about to be
    /// released, return it to the pool if it will fit, else release it.  If
    /// the reference count is higher than 1, meaning it is still active in
    /// an asynchronous operation like a kernel, put it on the inUse list to
    /// check later in sweepInUse().
    void        returnBuffer(const cl::Buffer &buf);

    /// Sweep the inUse list, looking for buffers that are no longer being used
    /// in an asynchronous operation and can be returned to the pool.
    void        sweepInUse();

private:
    // This class just holds a cl::Buffer while it's in the LRUCache.  It's also
    // responsible for alerting the CE_MemoryPool object when it's deleted
    // by pruning or other means.  Each cl::Buffer is owned by only one of
    // these objects, as maintained by being non-copyable and having move
    // semantics.
    class ut_clBuffer : public UT_NonCopyable
    {
    public:
        ut_clBuffer(CE_MemoryPool *pool, const cl::Buffer &buf, int64 size);

        ut_clBuffer(ut_clBuffer &&b);

        ~ut_clBuffer();

        ut_clBuffer &operator=(ut_clBuffer &&b);

        static exint size(const ut_clBuffer &b) { return b.mySize; }

    private:
        int64 mySize;
        cl::Buffer myBuffer;
        CE_MemoryPool *myPool;
    };
    
    friend class ut_clBuffer;

    bool 	getBufferFromPool(int64 size, cl::Buffer &buf);
    
    /// Add the buffer to the pool. Returns false if it operation fails.
    bool        appendToPool(const cl::Buffer &buf, int64 size);

    /// Remove the the buffer from the pool.
    void        releaseBuffer(const cl::Buffer &buf, int64 size);

    typedef UT_LRUCache<cl_mem, ut_clBuffer, ut_clBuffer::size> BufferCache;
    typedef UT_Map<exint, UT_Array<cl::Buffer *>> BufferTable;
    BufferCache 		myLRUCache;
    UT_Array<cl::Buffer *>   	myInUseList;
    BufferTable 		myBufferTable;
    int64                       myInUseSize;
    int64                       myTotalAllocSize;

    mutable UT_Lock		myMemoryPoolLock;
};


#endif
#endif
