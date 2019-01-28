/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_LRUCache.h ( UT Library, C++)
 *
 * COMMENTS:
 *	The LRU cache has a hash table for fast access and a link list for
 *	quick removal when the cache is full. Items are unique in the cache.
 */

#ifndef UT_LRU_CACHE_H
#define UT_LRU_CACHE_H

#include "UT_Assert.h"
#include "UT_IteratorRange.h"
#include "UT_NonCopyable.h"

#include <SYS/SYS_Types.h>
#include <SYS/SYS_TypeTraits.h>

#include <functional>
#include <list>
#include <type_traits>
#include <unordered_map>

namespace UT
{
    // A dummy implementation of UT_RWLock that does nothing.
    class RWNullLock
    {
    public:
	void readLock() {}
	void writeLock() {}
	void readUnlock() {}
	void writeUnlock() {}
    };
}

/// A default helper function used by UT_LRUCache to determine the size of the
/// objects it stores to help prune the storage so that it doesn't exceed the 
/// maximum given in the constructor.
template<typename V>
inline exint
UTlruGetItemSize(const V &)
{
    static_assert(!std::is_pointer<V>::value, 
		  "Pointer types need their own size function.");
    return sizeof(V);
}


/// A default helper function used by UT_LRUCache to determine whether an 
/// object is currently in use and so should not be deleted when the cache
/// gets pruned.
template<typename V>
inline bool
UTlruGetItemInUse(const V &)
{
    return false;
}



template<typename K, 
	 typename V, 
	 exint(*SizeFunc)(const V &) = UTlruGetItemSize<V>, 
	 bool(*InUseFunc)(const V &) = UTlruGetItemInUse<V>, 
	 typename L=UT::RWNullLock>
class UT_LRUCache : 
    public UT_NonCopyable
{
    using ValueType = std::pair<const K, V>;
    using ValueList = std::list<ValueType> ;
    using KeyIteratorMap = std::unordered_map<K, typename ValueList::iterator>;
    
    template<typename PROXIED, typename TYPE>
    friend class iterator_base;

    ValueList myValueList;
    KeyIteratorMap myKeyMap;
    exint myMaxSize;
    exint myCurrentSize;
    std::function<exint(const V &)> mySizeFunc;
    std::function<bool(const V &)> myInUseFunc;
    mutable L *myLock;
    
protected:
    /// This iterator is just a simple proxy around either the regular iterator
    /// or the const_iterator on the internal list.
    template<typename PROXIED, typename TYPE> 
    class iterator_base :
	public std::iterator<std::bidirectional_iterator_tag, TYPE>
    {
	using Base = 
	    typename std::iterator<std::bidirectional_iterator_tag, TYPE>; 
	using proxied_iterator = PROXIED; 
    public:
	using typename Base::pointer;
	using typename Base::reference;
	
	iterator_base() : myLock(nullptr) {}
	
	iterator_base(const iterator_base &other)
	{
	    other.myLock->readLock();
	    myIt = other.myIt;
	    myLock = other.myLock;
	}
	
	iterator_base(iterator_base &&other)
	{
	    myIt = std::move(other.myIt);
	    other.myIt = proxied_iterator();
	    myLock = other.myLock;
	    other.myLock = nullptr;
	}
	
	iterator_base &operator=(const iterator_base &other)
	{
	    other.myLock->readLock();
	    myIt = other.myIt;
	    myLock = other.myLock;
	    return *this;
	}
	
	iterator_base &operator=(iterator_base &&other)
	{
	    myIt = std::move(other.myIt);
	    other.myIt = proxied_iterator();
	    myLock = other.myLock;
	    other.myLock = nullptr;
	    return *this;
	}
	
	~iterator_base()
	{
	    // Release the read lock on destroy.
	    if (myLock)
		myLock->readUnlock();
	}
	
	iterator_base &operator++() { ++myIt; return *this; }
	iterator_base &operator--() { --myIt; return *this; }
	
	pointer operator->() const { return &(*myIt); }
	reference operator*() const { return *myIt; }
	
	bool operator==(const iterator_base &other) const
	{
	    return myIt == other.myIt;
	}
	bool operator!=(const iterator_base &other) const
	{
	    return myIt != other.myIt;
	}
	
    protected:
	friend class UT_LRUCache<K, V, SizeFunc, InUseFunc, L>;
	
	iterator_base(proxied_iterator it, L *lock) : 
	    myIt(it), myLock(lock) { }
	
    private:
	proxied_iterator myIt;
	L *myLock;
    };
    
public:
    /// A iterator pointing to mutable values. 
    using iterator = iterator_base<typename ValueList::iterator, std::pair<const K, V>>;
    
    /// A const iterator pointing to immutable values.
    using const_iterator = iterator_base<typename ValueList::const_iterator, const std::pair<const K, V>>; 
    
    /// Construct a new LRU cache of a given @c max_size. By default the cache
    /// is not thread-safe, but by setting the template argument @c L to 
    /// @c UT_RWLock, the cache is automatically thread-safe. This incurs some  
    /// overhead due to the locking, however, so only do that if absolutely 
    /// required.
    UT_LRUCache(exint max_size = SYS_EXINT_MAX) :
	myMaxSize(max_size), 
	myCurrentSize(0),
	mySizeFunc(SizeFunc),
	myInUseFunc(InUseFunc),
	myLock(nullptr)
    {
	if (!std::is_same<L, UT::RWNullLock>::value)
	    myLock = new L;
    }
    
    /// Sets the new maximum size for the cache. If the new maximum is smaller
    ///  than the current maximum, the cache will be pruned to fit the new 
    /// maximums size.
    void setMaxSize(exint max_size)
    {
	if (max_size >= myMaxSize)
	{
	    myMaxSize = max_size;
	}
	else
	{
	    myMaxSize = max_size;
	    if (myLock) 
		myLock->writeLock();
	    
	    prune(nullptr);
	    
	    if (myLock) 
		myLock->writeUnlock();
	}
    }
    
    /// Returns the current maximum size of the cache. 
    exint maxSize() const { return myMaxSize; }
    
    /// Returns the current size of the cache, in arbitrary units. 
    /// If the cache is thread-safe this value may not be the most up-to-date.
    exint currentSize() const 
    { 
	if (myLock) 
	    myLock->readLock();
	
	exint size = myCurrentSize;
	
	if (myLock) 
	    myLock->readUnlock();
	return size;
    }
    
    /// Returns the number of items in the cache. 
    exint count() const 
    {
	if (myLock) 
	    myLock->readLock();
	
	exint size = myValueList.size();
	
	if (myLock) 
	    myLock->readUnlock();
	return size;
    }
    
    /// Checks for the presence of the item with the given @c key. Does not
    /// affect their ordering, like @c find.
    bool contains(const K &key) const
    {
	if (myLock) 
	    myLock->readLock();
	
	bool found = (myKeyMap.find(key) != myKeyMap.end());
	
	if (myLock) 
	    myLock->readUnlock();
	return found;
    }
    
    /// Find the item in the cache with the given key. If nothing is found  
    /// then the iterator will point to the end.
    /// There's no const version, since looking up the key modifies the LRU
    /// cache's state and the iterator holds a read lock, if the cache is 
    /// thread-safe. 
    /// @note Since the iterator holds a lock, do 
    iterator find(const K &key)
    {
	if (myLock) 
	    myLock->readLock();
	
	auto itK = myKeyMap.find(key);
	if (itK == myKeyMap.end())
	{
	    if (myLock) 
		myLock->readUnlock();
	    return iterator(myValueList.end(), nullptr);
	}
	
	// Move this item to the top of the list.
	if (itK->second != myValueList.begin())
	{
	    myValueList.splice(itK->second, myValueList, myValueList.begin());
	    itK->second = myValueList.begin();
	}
	
	return iterator(itK->second, myLock);
    }
    
    /// Insert a new item into the cache. The item is automatically marked as 
    /// the most recently used. The cache is pruned beforehand, to avoid 
    /// evicting the item immediately. Returns a pair of an iterator and a 
    /// @c bool. The iterator points to the item inserted, or the existing item 
    /// if not evicted. The @c bool value indicates whether the item got 
    /// inserted or not.
    std::pair<iterator, bool>
    insert(const K &key, V &&value, bool evict=false)
    {
	if (myLock)
	{
	    // Get both read and write locks. Upon exit, we relinquish the
	    // write lock, but the read lock gets carried out through the
	    // iterator.
	    myLock->writeLock();
	    myLock->readLock();
	}
	
	std::pair<iterator, bool> result;
	auto itK = myKeyMap.find(key);
	if (itK != myKeyMap.end() && !evict)
	{
	    // The item already exists and we don't want to evict the existing 
	    // item.
	    result = std::make_pair(iterator(itK->second, myLock), false); 
	}
	else
	{
	    if (itK != myKeyMap.end())
	    {
		// If we're replacing an item, take that away from the current 
		// size before updating it with the new value.
		myCurrentSize -= mySizeFunc((itK->second)->second);
		myCurrentSize += mySizeFunc(value);
	    
		prune(&itK->second);
	    
		// We're just updating an existing item. Move it to the  
		// beginning and move the contents of the existing object onto 
		// the old one. 
		myValueList.splice(itK->second, myValueList, 
				   myValueList.begin());
		
		myValueList.begin()->second = std::move(value);
		itK->second = myValueList.begin();
		
		result = std::make_pair(iterator(itK->second, myLock), true);
	    }
	    else
	    {
		myCurrentSize += mySizeFunc(value);
		
		prune(nullptr);
		
		myValueList.push_front(std::make_pair(key, std::move(value)));
		myKeyMap.insert(std::make_pair(key, myValueList.begin()));
		
		result = std::make_pair(iterator(myValueList.begin(), myLock), 
					true);
	    }
	}
	
	if (myLock) 
	    myLock->writeUnlock();
	
	return result;
    }
    
    
    /// Removes the item matching the @c key. If the item existed and was 
    /// successfully removed, then this function returns @c true. 
    bool erase(const K &key)
    {
	bool updated = false;
	if (myLock) 
	    myLock->writeLock();
	
	auto itK = myKeyMap.find(key);
	if (itK != myKeyMap.end())
	{
	    myCurrentSize -= mySizeFunc((itK->second)->second);
	    myValueList.erase(itK->second);
	    myKeyMap.erase(itK);
	    updated = true;
	}
	
	if (myLock) 
	    myLock->writeUnlock();
	
	return updated;
    }
    
    /// Steals an item from the cache. This is functionally equivalent to 
    /// @c erase with a key, except the item in the cache also gets hoisted 
    /// outside into @c value, if it exists.
    bool steal(const K &key, V &&value)
    {
	bool updated = false;
	if (myLock) 
	    myLock->writeLock();
	
	auto itK = myKeyMap.find(key);
	if (itK != myKeyMap.end())
	{
	    myCurrentSize -= mySizeFunc((itK->second)->second);
	    
	    // Move the contents of the cache item into the outside item.
	    value = std::move((itK->second)->second);
	    
	    myValueList.erase(itK->second);
	    myKeyMap.erase(itK);
	    updated = true;
	}
	
	if (myLock) 
	    myLock->writeUnlock();
	
	return updated;
    }
    
    /// Clears the cache completely.
    void clear()
    {
	if (myLock) myLock->writeLock();
	
	myKeyMap.clear();
	myValueList.clear();
	myCurrentSize = 0;
	
	if (myLock) myLock->writeUnlock();
    }
    
    /// Returns an iterator to the front-most item in the LRU cache. 
    iterator begin()
    {
	if (myLock) myLock->readLock();
	return iterator(myValueList.begin(), myLock); 
    }
    
    /// Returns an iterator to the end of the LRU list. 
    iterator end()
    { 
	// The iterator will unlock the read lock when it destroys.
	return iterator(myValueList.end(), nullptr); 
    }

    /// Returns a const iterator to the front-most item in the LRU cache. 
    const_iterator begin() const
    {
	if (myLock) myLock->readLock();
	return const_iterator(myValueList.begin(), myLock); 
    }
    
    /// Returns a const iterator to the end of the LRU list. 
    const_iterator end() const
    { 
	// The iterator will unlock the read lock when it destroys.
	return const_iterator(myValueList.end(), nullptr); 
    }
    
private:
    /// Prune the list, from the last to the first. We skip objects that define
    /// inUse() which returns @c true.
    /// @note This function assumes that the caller holds the write lock. 
    void prune(typename ValueList::iterator *itSkipV = nullptr)
    {
	for (auto ritV = myValueList.rbegin(); 
	    myCurrentSize > myMaxSize && ritV != myValueList.rend(); ++ritV)
	{
	    if ((itSkipV && ritV.base() == *itSkipV) || 
		myInUseFunc(ritV->second))
	    {
		continue;
	    }
	    
	    myCurrentSize -= mySizeFunc(ritV->second);
	    
	    auto itK = myKeyMap.find(ritV->first);
	    myKeyMap.erase(itK);
	    // Delicate dance because erase takes a forward iterator, but we're 
	    // using a reverse iterator.
	    auto itV = myValueList.erase(std::prev(ritV.base())); 
	    ritV = typename ValueList::reverse_iterator(itV);
	}
    }
};

#endif
