/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_LocalDiskCache.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_LocalDiskCache__
#define __UT_LocalDiskCache__

#include "UT_API.h"
#include "UT_CappedCache.h"
#include "UT_ConcurrentQueue.h"
#include "UT_String.h"
#include <stdio.h>

/// @brief Class used to lookup cache items
class UT_API UT_LocalDiskCacheKey : public UT_CappedKey
{
public:
    UT_LocalDiskCacheKey()
	: UT_CappedKey()
    {}
    virtual ~UT_LocalDiskCacheKey();

    /// @{
    /// Pure virtual methods which need to be implemented in the subclass
    virtual UT_CappedKey	*duplicate() const = 0;
    virtual unsigned		getHash() const = 0;
    virtual bool		isEqual(const UT_CappedKey &key) const=0;
    /// @}

private:
    /// The getTotalMemory() method should @b not be implemented in the
    /// sub-class.  It is important that this always return "1".
    virtual int64	getTotalMemory() const;
};

/// @brief A disk cache to store expensive data on a local disk
///
/// In some cases, accessing data can be expensive:
///	- Loading over a saturated network
///	- An expensive computation
/// This cache provides a means to store this data on disk.  The disk file is
/// automatically cleaned up when the program completes.
///
/// This cache makes a reasonable secondary cache.
class UT_API UT_LocalDiskCache
{
public:
    /// The cache is used to store data in a temporary disk file
    /// @c max_item_size is the maximum size for each item put into the cache
    /// @c max_item_count is the maximum number of items to be stored in the
    /// cache
    /// It's fairly obvious that the maximum size for the disk file will
    /// be @c max_item_size*max_item_count.
    UT_LocalDiskCache(const char *cache_name,
			int64 max_item_size,
			int64 max_item_count);
    ~UT_LocalDiskCache();

    /// Clear entire disk cache.  This is not thread safe and should only be
    /// called when the cache isn't under contention.
    void	clearCache(bool reset_file=true);

    /// Extract from the disk cache.  Once extracted, the item will no longer
    /// exist in the cache.
    ///
    /// The @remove_from_cache flag determines whether the object will be
    /// removed from the disk cache after extraction.  Since this cache is
    /// usually used as a secondary cache, it's usually the primary cache which
    /// extracts the data from the disk cache (keeping the object in the
    /// primary cache after).
    bool	extractFromCache(const UT_LocalDiskCacheKey &key,
				void *buffer, int64 &buffer_size,
				bool remove_from_cache = true);

    /// Store in the cache
    void	storeInCache(const UT_LocalDiskCacheKey &key,
				const void *buffer, int64 buffer_size);

    /// @private Used internally
    void	itemFreed(int64 o, int64 s);

    /// Get memory used
    int64	getMemoryUsed() const;

    /// Get the file size
    int64	getFileSize() const	{ return myFileSize; }

    /// Traverse the cache
    template <typename T> void	traverseCache(T &task)
				{
				    myCache.threadSafeTraversal(task);
				}

    /// Const access to the cache
    const UT_CappedCache	&getCache() const	{ return myCache; }

private:
    bool	 lockAndLoad(int64 offset, void *data, int64 size);
    bool	 lockAndSave(int64 offset, const void *data, int64 size);
    void	 growFile(int64 size);
    void	 initQueue();

    UT_CappedCache		 myCache;
    UT_ConcurrentQueue<int64>	 myQueue;
    FILE			*myFp;
    UT_Lock			 myLock;
    int64			 myMaxItemSize, myMaxItemCount;
    int64			 myFileSize;
};

#endif
