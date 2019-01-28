/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ThreadSafeCache.h ( UT Library, C++ )
 *
 * COMMENTS:	A fast, thread-safe cache to be used for high-performance
 *		applications.
 */

#ifndef __UT_ThreadSafeCache__
#define __UT_ThreadSafeCache__

#include "UT_API.h"
#include <map>
#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_MemoryOrder.h>
#include <SYS/SYS_Types.h>
#include "UT_LockUtil.h"
#include "UT_Lock.h"
#include "UT_StopWatch.h"

//#define ITEM_TIMING

// Explicit instantiations are provided for:
// - UT_Lock
// - UT_RecursiveSpinLock
template <class Lock>
class UT_API UT_ThreadSafeCache {
public:
    class unsafe_traverser;
    
    // A cache item stub.  You must implement a subclass to provide
    // allocation and deallocation routines.
    class UT_CacheItem
    {
    public:
		 UT_CacheItem()
		     : myPrev(0), myNext(0), myMemory(0)
		     , myRefCount(0), myTimeStamp(0)
		     , myAllocated(false) {}
	virtual ~UT_CacheItem() { UT_ASSERT(!getRefCount()); }

	// Return a string to be used in diagnostic messages about
	// different cache types.
	virtual const char	*getItemType() const = 0;

	// Retrieve the amount of memory consumed by the cache item
	int64		 getMemoryUsage() const	{ return myMemory; }

    protected:
	// Allocate an item and return the total amount of memory consumed
	// by the cache item in bytes.
	virtual int64	 allocate(void *parms) = 0;

	// Deallocate the item and free associated memory.
	virtual void	 deallocate(void *user_data) = 0;

    private:
        /// NOTE: Code seems to rely on getRefCount() using
        ///       SYS_MEMORY_ORDER_SEQ_CST, not just SYS_MEMORY_ORDER_LOAD.
        ///       With SYS_MEMORY_ORDER_LOAD, the unit test fails
        ///       about 2.7% of the time.
	int		 getRefCount() const	{ return myRefCount.load(SYS_MEMORY_ORDER_SEQ_CST); }
	int		 incRefCount()		{ return myRefCount.add(1); }
	int		 decRefCount()		{ return myRefCount.add(-1); }

	bool		 isInCache() const	{ return myNext; }

	// Unlink the item, leaving the existing next/prev unchanged (clear
	// them with clearLinks()).
	void		 unlink()
			 {
			     myPrev->myNext = myNext;
			     myNext->myPrev = myPrev;
			 }
	// Set links to 0
	void		 clearLinks()
			 {
			     myPrev = 0;
			     myNext = 0;
			 }
	// Insert the item before the given item
	void		 link(UT_CacheItem *before)
			 {
			     // NOTE: Although isInCache() is called from
			     // unlocked code, and it only checks whether
			     // myNext is non-NULL, nothing after that depends
			     // on the other values set in this function, so
			     // the other values here don't need to be set
			     // first.

			     myNext = before;
			     myPrev = before->myPrev;

			     before->myPrev->myNext = this;
			     before->myPrev = this;
			 }
	// Link to ourselves - for a circular list of size 1
	void		 selfLink()
			 {
			     myNext = this;
			     myPrev = this;
			 }

    private:
        // Force the compiler to read/write myNext and myPrev when
        // indicated, instead of saving temporary values or reordering
        // the access.  myNext is used to determine if this is cached,
        // which could be changed by another thread, so is important
        // to re-read each time.
	UT_CacheItem *volatile myNext;
	UT_CacheItem *volatile myPrev;

        // NOTE: Even myMemory and myTimeStamp must be volatile, because
        //       they get read before locking, then read again after locking,
        //       possibly having been written by another thread in between.
	volatile int64	 myMemory;
	volatile uint64	 myTimeStamp;
#ifdef ITEM_TIMING
	double		 myAllocationTime;
#endif
	SYS_AtomicInt32	 myRefCount;

        // Force the compiler to read/write myAllocated when
        // indicated, instead of saving temporary values or reordering
        // the access.  myAllocated could be changed by another thread,
        // upon allocating or deallocating, so is important to re-read
        // each time.
	volatile bool	 myAllocated;

	friend class	 UT_ThreadSafeCache;
	friend class	 UT_ThreadSafeCache::unsafe_traverser;
    };

public:
    // The destroy flag controls whether the cache will clean up its
    // allocated items on destruction.
     UT_ThreadSafeCache(bool destroy = true);
    ~UT_ThreadSafeCache();

    // Set cache size in megabytes
    void		setMaxMemory(int64 size_mb, bool prune = false);
    // Set cache size in bytes
    void		setMaxMemoryBytes(int64 size_bytes, bool prune = false);
    int64		getMaxMemoryBytes() const
			    { return myMemoryLimit; }

    // Get total cache memory usage in bytes, including all allocated item
    // memory.
    int64		getMemoryUsage() const	{ return myMemoryUsed; }

    // Get number of cached items
    int64		entries() const		{ return myEntries; }

    // Set user data that is passed to allocate()/deallocate()
    void		setUserData(void *data)	{ myUserData = data; }

    // Control whether we evict items when memory limit is used. Default is
    // true.
    void		setCheckMemoryLimit(bool check)
			    { myCheckMemoryLimit = check; }
    bool		getCheckMemoryLimit() const
			    { return myCheckMemoryLimit; }

    // Access an item in the cache.  If the item is not already in the
    // cache, it will be inserted at the head.  Calling this operation
    // acquires an access lock that must be released with release().  parms
    // will be passed directly to the allocate() operation.
    void		access(UT_CacheItem *item, void *parms = 0)
    {
	item->incRefCount();

        // This prevents the memory load in item->isInCache() from occurring
        // before the store in item->incRefCount(), which is necessary for
        // the reasons outlined in freeItem().  Basically, item could be
        // deallocated after calling item->incRefCount(), but only if
        // item->incRefCount() occurs after item->isInCache() becomes false.
        // Thus, so long as item->isInCache() is read after
        // item->incRefCount(), it won't get a stale true value, which would
        // have falsely indicated that allocation isn't necessary.
        SYSmemoryFence();

	// Before moving items in the cache, check
	// whether the head has been moved a
	// sufficient number of times to make it
	// useful to move the item back to the head
	// of the cache.  This way, if the same set
	// of items are always referenced we never
	// need to modify the cache.
	if (item->isInCache() && isNearHead(item))
	{
	    return;
	}

	// Now, reorder the item in the cache
	accessLockedReorder(item, parms);
    }

    // Release the access lock on an item.  For every call to access()
    // there must be a corresponding call to release().
    void		release(UT_CacheItem *item)
			{
			    if (!item->decRefCount() && !item->isInCache())
				freeItem(item);
			}

    // Release the access lock and remove an item from the cache, if there
    // are no more references.
    void		releaseAndDeallocate(UT_CacheItem *item)
			{
			    if (!item->decRefCount())
				freeItem(item);
			}

    // Deallocates and removes an item from the cache.  This is not
    // thread-safe while anything has a reference to the item.
    void		freeItem(UT_CacheItem *item);

    // Clear all entries from the cache.
    void		freeAllItems();

    // Prune entries from the cache until we hit the given memory usage
    void		pruneItems(int64 memory_limit);

    // Prints fault statistics categorized by item type
    void		dumpStats(const char *name) const;

    class UT_API unsafe_traverser
    {
	public:
	    unsafe_traverser()
		: myCurr(NULL)
		, myHead(NULL)
	    {}
	    unsafe_traverser(const unsafe_traverser &src)
		: myCurr(src.myCurr)
		, myHead(src.myHead)
	    {}
	    ~unsafe_traverser() {}

	    const UT_CacheItem	*get() const	{ return myCurr; }
	    int		operator==(const unsafe_traverser &cmp) const
			    { return myCurr == cmp.myCurr; }
	    int		operator!=(const unsafe_traverser &cmp) const
			    { return myCurr != cmp.myCurr; }
	    bool	atEnd() const
			    { return !myCurr; }
	    void	rewind()
			    { myCurr = myHead; }
	    unsafe_traverser	&operator++()
			    {
				if (myCurr && myCurr->myNext != myHead)
				    myCurr = myCurr->myNext;
				else
				    myCurr = NULL;
				return *this;
			    }
	    const unsafe_traverser	&operator=(const unsafe_traverser &src)
			    {
				myCurr = src.myCurr;
				myHead = src.myHead;
				return *this;
			    }
	private:
	    unsafe_traverser(UT_CacheItem *head)
		: myCurr(head)
		, myHead(head)
		{}
	    const UT_CacheItem	*myCurr, *myHead;
	    friend class UT_ThreadSafeCache;
    };

    unsafe_traverser	unsafe_begin() const
			    { return unsafe_traverser(myHead); }
    unsafe_traverser	unsafe_end() const
			    { return unsafe_traverser(); }

private:
    /// Reorder the list due to access - requires locking
    void		accessLockedReorder(UT_CacheItem *item, void *parms);

    void		pruneItemsNoLock(int64 memory_limit);
    void		addToCache(UT_CacheItem *item);
    void		removeFromCache(UT_CacheItem *item);
    void		allocateItem(UT_CacheItem *item, void *parms)
    {
	if (!item->myAllocated)
	{
	    UT_AutoObjLockType<Lock>
		lock(myObjLock, item);
	    if (!item->myAllocated)
	    {
#ifdef ITEM_TIMING
		UT_StopWatch	timer;
		timer.start();
#endif
		item->myMemory = item->allocate(
			parms ? parms : myUserData);

                // Ensure that the data allocated are written out
                // to main memory before setting myAllocated to true,
                // else something could read myAllocated, see that it's
                // true, read an out-of-date data pointer, probably still
                // NULL, and crash dereferencing it.
                SYSstoreFence();

		item->myAllocated = true;

                // NOTE: The automatic unlock will do a second SYSstoreFence,
                //       ensuring that myAllocated will be true before the
                //       lock is released.

#ifdef ITEM_TIMING
		item->myAllocationTime = timer.lap();
#endif
	    }
	}

        // Ensure that code in the calling function doesn't
        // speculatively read data from item before the read of
        // item->myAllocated above.  Without it, what could happen is:
        // 0) item->myAllocated starts false, and some subclass member,
        //    p, starts NULL.
        // 1) This thread (in the calling function after this call)
        //    speculatively reads p, getting NULL.
        // 2) Another thread sets p to a valid address, and does a store
        //    fence, to ensure it's written out.
        // 3) The other thread sets myAllocated to true.
        // 4) The other thread unlocks, doing a store fence just before
        //    unlocking, to ensure myAllocated is written out.
        // 5) This thread (at the top of this function) reads myAllocated,
        //    getting true.
        // 6) The calling function already has a value for p, and myAllocated
        //    is true, so it dereferences p, and crashes.
        SYSloadFence();
    }

    // Heuristic to decide when an item is close to the head.  Items can
    // only be moved when this method returns false.
    bool		isNearHead(const UT_CacheItem *item) const
			{ return myHeadTime - item->myTimeStamp <=
			    myQuarterEntries; }

private:

    typedef typename Lock::Scope    LockScope;

    // This class keeps track of fault statistics for a given type of item
    // in the cache.
    class FaultStats {
    public:
	FaultStats()
	    : myFaults(0)
	    , myTotalMemory(0)
	    , myTotalTime(0) {}

    public:
	int64	myFaults;
	int64	myTotalMemory;
	double	myTotalTime;
    };

    typedef std::map<const char *, FaultStats>	FaultMap;

    UT_ObjLockType<Lock>	 myObjLock;
    Lock			 myThreadLock;
    UT_CacheItem		*myHead;
    int64			 myMemoryLimit;
    int64			 myMemoryUsed;
    int64			 myEntries;
    int64			 myQuarterEntries;
    FaultMap			 myFaultMap;
    uint64			 myHeadTime;
    void			*myUserData;
    bool			 myDestroy;
    bool			 myCheckMemoryLimit;
};

/// This can be used as a unified cache for a process.  The cache will not
/// destroy any items left in the cache when the process exits.
class UT_API UT_UnifiedCache 
{
public:
    static UT_ThreadSafeCache<UT_Lock> &get() { return theCache; }

    /// Set the amount of memory to be used by the cache, in MB
    static void		setMaxMemory(int size_mb)
			{
			    theCache.setMaxMemory(size_mb);
			    theMemorySet = true;
			}

    /// Check whether the memory limit has been set
    static bool		isMaxMemorySet()
			{ return theMemorySet; }

    static int64	getMemoryUsage()
			{ return theCache.getMemoryUsage(); }

private:
    static UT_ThreadSafeCache<UT_Lock>	theCache;
    static bool		theMemorySet;
};

/// Scoped accessor object for accessing unified cache items. 
template<typename T>
class UT_UnifiedCacheAccessor
{
public:
    /// Default initializer. Can be used to initialize the accessor to a cache
    /// item right away. Otherwise call @c reset to set the item to access.
    UT_UnifiedCacheAccessor(T *item = 0, void *parm = 0)
    {
	access(item, parm);
    }

    /// Release the currently accessed cache item, if any.
    ~UT_UnifiedCacheAccessor()
    {
	release();
    }

    /// Returns @c true if no item is being accessed.
    bool empty() const { return myItem == 0; }
    
    /// Returns the current item being accessed
    T *item() const { return myItem; }
    
    /// Indirection operator for the current item being accessed. It is the
    /// caller's responsibility to ensure that the item is valid, e.g. by
    /// calling @c empty on it first.
    T *operator->() const { return myItem; }

    /// Reset the currently accessed cached item to a new item. If the item
    /// given is the same as the one currently being accessed, this call is a
    /// no-op, even if a new @c param value is given. Reset the item to @c
    /// nullptr and back to the wanted item if a change in @c parm value is
    /// required.
    void reset(T *item, void *parm = 0)
    {
	if (myItem != item)
	{
	    release();
	    access(item, parm);
	}
    }

private:
    void access(T *item, void *parm)
    {
	myItem = item;
	if (myItem)
	    UT_UnifiedCache::get().access(item, parm);
    }
    void release()
    {
	if (myItem)
	{
	    UT_UnifiedCache::get().release(myItem);
	    myItem = 0;
	}
    }

    T 	*myItem;
};

#endif
