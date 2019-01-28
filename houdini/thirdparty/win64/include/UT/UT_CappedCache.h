/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_CappedCache.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_CappedCache__
#define __UT_CappedCache__

#include "UT_API.h"
#include "UT_Cache.h"
#include "UT_String.h"
#include "UT_IntrusivePtr.h"
#include "UT_Lock.h"
#include "UT_SpinLock.h"
#include "UT_ConcurrentHashMap.h"

class UT_CappedCache;

typedef UT_RecursiveSpinLock	UT_CappedLock;
//typedef UT_Lock	UT_CappedLock;

/// Base class for search keys for UT_CappedCache
class UT_API UT_CappedKey
    : public UT_IntrusiveRefCounter<UT_CappedKey>
{
public:
    UT_CappedKey();
    virtual ~UT_CappedKey();

    /// The duplicate() method should return a copy of the key.
    virtual UT_CappedKey	*duplicate() const = 0;
    /// Return a hash for the key
    virtual unsigned int	 getHash() const = 0;
    /// Test equality
    virtual bool		 isEqual(const UT_CappedKey &key) const = 0;
private:
};
using UT_CappedKeyHandle = UT_IntrusivePtr<UT_CappedKey>;

/// Base class for items in the UT_CappedCache
class UT_API UT_CappedItem
    : public UT_IntrusiveRefCounter<UT_CappedItem>
{
public:
    UT_CappedItem();
    virtual ~UT_CappedItem();

    /// Query the memory used by the cache item.  This should be constant over
    /// the life of the item.
    virtual int64		getMemoryUsage() const = 0;
private:
};
using UT_CappedItemHandle = UT_IntrusivePtr<UT_CappedItem>;

/// A thread safe memory limited cache
///
/// This class stores a map of UT_CappedItem objects which are queried by
/// UT_CappedKey objects.  In addition, the cache is an LRU, memory limited
/// cache.  The cache tracks memory used by each UT_CappedItem to keep memory
/// usage limited to the given threshold.
/// @note This class conforms to the UT_Cache interface.
class UT_API UT_CappedCache : public UT_Cache
{
public:
     UT_CappedCache(const char *name, int64 size_in_mb=32);
    ~UT_CappedCache();

    /// Clear the cache
    void		clear();

    /// Number of entries in the cache
    exint		entries() const	{ return myMap.size(); }

    /// Return the number of entries in the list.  This should match the
    /// entries in the map.
    exint		getListEntries() const	{ return myEntries; }

    /// Find an item in the cache
    UT_CappedItemHandle	findItem(const UT_CappedKey &key);

    /// Add an item to the cache.  The returned item may not be the item passed
    /// as an argument.  This may happen if two threads attempt to add the same
    /// item at the same time (i.e. the keys are equivalent, but the items may
    /// be different).
    UT_CappedItemHandle	addItem(const UT_CappedKey &key,
				const UT_CappedItemHandle &item);

    /// Remove an item from the cache
    void		deleteItem(const UT_CappedKey &key);

    /// @{
    /// API from UT_Cache
    virtual const char	*utGetCacheName() const		{ return myName; }
    virtual bool	 utHasMaxSize() const		{ return true; }
    virtual int64	 utGetCurrentSize() const	{ return myMemUsage; }
    virtual int64	 utGetMaxSize() const		{ return myMaxUsage; }
    virtual void	 utSetMaxSize(int64 bytes);
    virtual int64	 utReduceCacheSizeBy(int64 bytes);
    /// @}

    /// Thread safe iteration over all items in the cache.
    /// This method takes a templated class that has a () operator to process
    /// entries from the traversal.  The () operator should return a bool, true
    /// if the iteration should continue, or false if the traversal can stop.
    /// For example: @code
    ///	  class MemoryCounter
    ///	  {
    ///	  public:
    ///		MemoryCounter() : myMemory(0) {}
    ///		bool	operator()(const UT_CappedKeyHandle &key,
    ///				const UT_CappedItemHandle &handle)
    ///			{
    ///			    myMemory += item->getMemoryUsage();
    ///			    return true;
    ///			}
    ///			int64	getMemoryUsage() const { return myMemory; }
    ///			int64	myMemory;
    ///	  };
    ///   static int64 computeStoredMemory(UT_CappedCache &cache)
    ///	  {
    ///		MemoryCounter task;
    ///		cache.threadSafeTraversal(task);
    ///		return task.getMemory();
    ///	  }
    /// @endcode
    /// @note It is possible to call some methods which alter the map.
    /// However, other methods will result in a corrupt iteration.
    ///	  - @c deleteItem @n
    ///	    It is safe to call deleteItem on the current item.  However, if
    ///	    deleteItem is called, and your task takes const-references to the
    ///	    keys/items, these may become invalid.
    ///	  - @c findItem @c addItem @n
    ///	    This may have a side effect of re-ordering the list, meaning
    ///	    iteration will be corrupted.
    template <typename T>
    void	threadSafeTraversal(T &task)
		{
		    UT_AutoLockType<UT_CappedLock>	 lock(myLock);
		    ut_CappedBin			*curr=myHead;
		    ut_CappedBin			*next;
		    exint				 n = myEntries;
		    for (exint i = 0; i < n; ++i, curr = next)
		    {
			next = curr->myNext;
			if (!task(curr->getKey(), curr->getItem()))
			    break;
		    }
		}

private:
    class ut_CappedBin {
    public:
	ut_CappedBin(const UT_CappedKeyHandle &key,
		    const UT_CappedItemHandle &item)
	    : myKey(key)
	    , myItem(item)
	    , myPrev(NULL)
	    , myNext(NULL)
	    , myTimeStamp(0)
	{
	}
	~ut_CappedBin()
	{
	}

	const UT_CappedKeyHandle	&getKey() const	{ return myKey; }
	const UT_CappedItemHandle	&getItem() const { return myItem; }

	exint		getTimeStamp() const { return myTimeStamp; }
	void		setTimeStamp(exint t) { myTimeStamp = t; }

	bool		isNearHead(exint head, exint watermark) const
			{
			    return head - myTimeStamp <= watermark;
			}

    private:
	UT_CappedKeyHandle	 myKey;
	UT_CappedItemHandle	 myItem;
	ut_CappedBin		*myPrev;
	ut_CappedBin		*myNext;
	exint			 myTimeStamp;
	friend class UT_CappedCache;	// For links
    };
    class ut_cappedKeyCompare
    {
    public:
	static size_t	hash(const UT_CappedKey *a)
			    { return (size_t)a->getHash(); }
	static bool	equal(const UT_CappedKey *a, const UT_CappedKey *b)
			    { return a->isEqual(*b); }
    };
    typedef UT_ConcurrentHashMap<const UT_CappedKey *,
				ut_CappedBin *,
				ut_cappedKeyCompare>	ut_CappedMap;

private:
    void		pruneItems();
    /// @{
    /// List maintenance
    void		addItem(ut_CappedBin *bin);
    void		accessItem(ut_CappedBin *bin);
    void		removeAndDeleteItem(ut_CappedBin *bin);
    void		insertItem(ut_CappedBin *bin);
    void		unlinkItem(ut_CappedBin *bin);
    /// @}

    UT_CappedLock	 myLock;
    UT_CappedLock	 myPruneLock;
    ut_CappedMap	 myMap;
    UT_String		 myName;
    ut_CappedBin *volatile myHead;
    volatile int64	 myMemUsage;
    int64		 myMaxUsage;
    exint		 myHeadTime;
    exint		 myQuarterEntries;
    exint		 myEntries;
};

#endif
