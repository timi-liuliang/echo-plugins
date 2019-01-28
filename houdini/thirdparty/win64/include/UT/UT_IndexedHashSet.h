/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_IndexedHashSet.h (UT Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __UT_IndexedHashSet__
#define __UT_IndexedHashSet__

#include "UT_Assert.h"
#include "UT_Array.h"
#include "UT_ConcurrentHashMap.h"
#include "UT_ConcurrentVector.h"
#include "UT_ConcurrentQueue.h"
#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>

/// Each item in the shared map is assigned a unique id
typedef exint UT_IndexedHashSetItemId;

/// @brief A thread-safe hash map which stores indexed shared items
///
/// Each item in the hash map is reference counted.  That is, if objects are
/// added multiple times, only a single object will be stored in the map.
///
/// Removing an item from the hash map (by id or key) will decrement the
/// reference count on the item.  When no longer referenced, the map will
/// delete the item.
///
/// When the map stores 'ids', each item is assigned a unique id
/// (UT_IndexedHashSetItemId).  The item can then be retrieved efficiently from
/// the map using the id (UT_IndexedHashSet::get())
///
/// Many methods on the map are thread-safe.  Some methods are not (and are
/// noted in the comments).
///
/// The KEY template parameter needs to have: @code
///	KEY(const KEY &src);			// The copy constructor
///	uint	hash() const;			// A hash method
///	bool	isEqual(const KEY &src) const;	// A comparison operator
/// @endcode
template<class T>
class UT_IndexedHashSet
{
public:
    /// Each item stored in the map will be given a unique id of type
    /// UT_IndexedHashSetItemId.  These id's can be used to perform efficient
    /// lookup of items in the map.
    UT_IndexedHashSet()
        : myListSize(0)
    {}
    ~UT_IndexedHashSet()
    {
        UT_ASSERT(entries() == 0);
    }

    /// Clear the map
    /// @note This is @b not thread-safe
    void clear();

    /// Return the number of entries in the map.
    /// @note This is @b not thread-safe
    exint entries() const { return myMap.size(); }

    /// Return whether the map is empty
    bool empty() const { return myMap.empty(); }

    /// Return approximate memory usage (not including key or item storage)
    int64 getMemoryUsage(bool inclusive) const;

    /// Return the maximum possible UT_IndexedHashSetItemId stored in the map.
    /// This returns an upper bound and is not exact.
    /// @note This is @b not thread-safe
    /// @note Not supported if id's are not stored in the map
    UT_IndexedHashSetItemId getItemIdUpperBound() const
    { return getListSize()-1; }

    /// Return the "occupancy" of the map.
    /// @note This is @b not thread-safe
    /// @note Not supported if id's are not stored in the map
    fpreal getOccupancy() const
    {
        exint lsize = getListSize();
        exint hsize = myHoles.unsafe_size();
        if (!lsize || !hsize)
            return fpreal(1);   // Fully occupied
        UT_ASSERT(lsize > hsize);
        return (fpreal)(lsize-hsize)/(fpreal)lsize;
    }

    /// Class used by compacting to map from the previous id to the new id.
    /// After compacting, this class stores a map of the old id's to the new
    /// id's
    class IdRemapping {
    public:
        /// Find the number of entries in the map
        exint entries() const { return myIdMap.entries(); }

        /// Query the new id associated with the previous id
        UT_IndexedHashSetItemId newId(UT_IndexedHashSetItemId prev) const
        {
            if (prev >= 0 && prev < myIdMap.entries())
                return myIdMap(prev);
            return -1;
        }

        /// @private Used by UT_IndexedHashSet::compactIds()
        void prepare(exint size)
        {
            myIdMap.setSizeNoInit(size);// Grow to expected size
            myIdMap.constant(-1);       // Initialize to -1
        }
        /// @private Used by UT_IndexedHashSet::compactIds()
        void setId(UT_IndexedHashSetItemId prev, UT_IndexedHashSetItemId curr)
        {
            UT_ASSERT(myIdMap(prev) == -1);
            myIdMap(prev) = curr;
        }
    private:
        UT_Array<exint> myIdMap;
    };

    /// Compact the list.  This fills out the integer map of old id's and their
    /// new id's.  If no compaction was done, the function returns false.
    /// @note This is @b not thread-safe
    /// @note This is a no-op if id's are not stored in the map
    bool compactIds(IdRemapping &remapping);

    /// Sort the list of ids based on the comparator.  This method only works
    /// if the table has been compacted.  Returns false if there are no ids or
    /// the list is not compacted.
    /// @note This is @b not thread-safe
    /// @note This is a no-op if id's are not stored in the map
    template<typename P>
    bool sortItems(const P &predicate);

    exint getReferenceCount(UT_IndexedHashSetItemId id) const;
    exint getReferenceCount(const T &key) const;

public:
    void replace(const UT_IndexedHashSet &src);
    UT_IndexedHashSetItemId add(const T &key);
    /// This may return null if inc is negative and the reference count
    /// reaches zero, or if the ID is out of range.
    const T *addReference(UT_IndexedHashSetItemId id, int inc);
    SYS_FORCE_INLINE
    const T *addReference(UT_IndexedHashSetItemId id)
    { return addReference(id, 1); }

    UT_IndexedHashSetItemId findId(const T &key) const
    {
        UT_IndexedHashSetItemId id;
        if (!findItemAndId(key, id))
            id = -1;
        return id;
    }
    const T *findItemAndId(const T &key, UT_IndexedHashSetItemId &id) const;
    SYS_FORCE_INLINE
    const T *get(UT_IndexedHashSetItemId id) const
    {
        return isValidId(id) ? &myList[id] : nullptr;
    }
    const T *getOrderedItem(exint index, UT_IndexedHashSetItemId *id = nullptr) const;
    bool remove(const T &key);
    bool remove(UT_IndexedHashSetItemId id);
    UT_IndexedHashSetItemId replaceItem(UT_IndexedHashSetItemId id,
                                        const T &key);

    template<typename ID_ARRAY, typename T_ARRAY>
    exint extractItems(ID_ARRAY &ids,
                       T_ARRAY &items,
                       exint maxitems) const;
    template<typename ID_ARRAY, typename T_ARRAY>
    exint extractItems(ID_ARRAY &ids,
                       T_ARRAY &items) const;
    template<typename T_ARRAY>
    exint extractItems(T_ARRAY &items) const;


    // Class to store item ID and reference count in the map
    class IdAndRefCount
    {
    public:
        /// WARNING: This initializes nothing; you must initialize separately
        SYS_FORCE_INLINE
        IdAndRefCount() {}
        SYS_FORCE_INLINE
        IdAndRefCount(UT_IndexedHashSetItemId id, exint refcount)
            : myId(id)
            , myRefCount(refcount)
        {}
        SYS_FORCE_INLINE
        IdAndRefCount(IdAndRefCount &&src)
            : myId(src.myId)
            , myRefCount(src.myRefCount)
        {}
        SYS_FORCE_INLINE
        IdAndRefCount(const IdAndRefCount &src)
            : myId(src.myId)
            , myRefCount(src.myRefCount)
        {}

        SYS_FORCE_INLINE
        UT_IndexedHashSetItemId getId() const { return myId; }
        SYS_FORCE_INLINE
        void setId(UT_IndexedHashSetItemId id) { myId = id; }
        SYS_FORCE_INLINE
        exint getRef() const { return myRefCount; }
        SYS_FORCE_INLINE
        void setRef(int d) { myRefCount = d; }
        SYS_FORCE_INLINE
        int bumpRef(int d)
        {
            myRefCount += d;
            return myRefCount;
        }

    private:
        UT_IndexedHashSetItemId myId;
        int myRefCount;
    };

    // Comparison class for hash map
    class keyHasher
    {
    public:
        SYS_FORCE_INLINE
        static uint hash(const T &key)
        {
            return hash_value(key);
        }
        SYS_FORCE_INLINE
        static bool equal(const T &a, const T &b)
        {
            return a == b;
        }
    };

    typedef UT_ConcurrentHashMap<T, IdAndRefCount>
                                    UT_IndexedHashSetTable;
    typedef UT_ConcurrentVector<T>  UT_IndexedHashSetVector;
    typedef UT_ConcurrentQueue<UT_IndexedHashSetItemId>
                                    UT_IndexedHashSetHoleQueue;

public:
    /// Iterate over items in the list - this is in the order they are stored
    /// in the map (i.e. by id).
    class unsafe_listiterator
    {
	unsafe_listiterator()
	    : myMap(nullptr)
	    , myIterator()
	    , mySize(0)
	    , myCurr(0)
	{ }
	unsafe_listiterator(const unsafe_listiterator &src)
	    : myMap(src.myMap)
	    , myIterator(src.myIterator)
	    , mySize(src.mySize)
	    , myCurr(src.myCurr)
	{ }
	~unsafe_listiterator()
	{ }

	/// @{
	/// Get information about the current item
	const T &getKey() const
	{ return *myIterator; }

	UT_IndexedHashSetItemId getItemId() const
	{ return UT_IndexedHashSetItemId(myCurr); }

	exint getItemShareCount() const
	{ return myMap->myMap[*myIterator].getRef(); }
	/// @}

	/// @{
	/// Implementation of iterator interface
	bool atEnd() const { return myCurr >= mySize; }
	void advance()
	{
	    do
	    {
		myCurr++;
		myIterator++;
	    } while (myCurr < mySize && UT_IndexedHashSet::isValidKey(*myIterator));
	}
	unsafe_listiterator &operator++() { advance(); return *this; }
	bool operator==(const unsafe_listiterator &it) const
	{
	    if (atEnd() && it.atEnd())
		return true;
	    return myMap == it.myMap &&
		    mySize == it.mySize &&
		    myCurr == it.myCurr;
	}
	bool operator!=(const unsafe_listiterator &it)
	{ return !(*this == it); }
	/// @}
    private:
	unsafe_listiterator(const UT_IndexedHashSet &map)
	    : myMap(&map)
	    , myIterator(map.myList.begin())
	    , myCurr(0)
	    , mySize(map.entries())
	{
	}
	const UT_IndexedHashSet			*myMap;
	typename UT_IndexedHashSetVector::const_iterator myIterator;
	exint					 mySize, myCurr;
	friend class UT_IndexedHashSet;
    };
    /// Iterate over items in the map - this is arbitrary order
    class unsafe_iterator
    {
    public:
	unsafe_iterator()
	    : myMap(NULL)
	    , myIterator()
	    , mySize(0)
	    , myCurr(0)
	{}
	unsafe_iterator(const unsafe_iterator &src)
	    : myMap(src.myMap)
	    , myIterator(src.myIterator)
	    , mySize(src.mySize)
	    , myCurr(src.myCurr)
	{}
	~unsafe_iterator() {}

	/// @{
	/// Get information about the current item
        const T &getKey() const
        { return myIterator->first; }

	UT_IndexedHashSetItemId getItemId() const
	{ return myIterator->second.getId(); }

	exint getItemShareCount() const
        { return myIterator->second.getRef();}
	/// @}

	/// @{
	/// Implementation of iterator interface
	bool atEnd() const { return myCurr >= mySize; }
	void advance()
        {
            ++myCurr;       // Move my count
            ++myIterator;   // Move my iterator
            // Assert that the iterator doesn't terminate early
            UT_ASSERT_P(myCurr >= mySize || myIterator != myMap->myMap.end());
        }
	unsafe_iterator	&operator++() { advance(); return *this; }
	// No post increment as it is dangerous.
	bool operator==(const unsafe_iterator &it) const
        {
            if (atEnd() && it.atEnd())
	        return true;
            UT_ASSERT_P(mySize == it.mySize);
            return myMap == it.myMap &&
	            myCurr == it.myCurr;
        }
	bool operator!=(const unsafe_iterator &it)
        { return !(*this == it); }
	/// @}
    private:
	unsafe_iterator(const UT_IndexedHashSet &map)
	    : myMap(&map)
	    , myIterator(map.myMap.begin())
	    , myCurr(0)
	    , mySize(map.entries())
	{
	}
	const UT_IndexedHashSet			*myMap;
	typename UT_IndexedHashSetTable::const_iterator myIterator;
	exint					 mySize, myCurr;
	friend class UT_IndexedHashSet;
    };

    unsafe_iterator begin() const
    { return unsafe_iterator(*this); }
    unsafe_iterator end() const
    { return unsafe_iterator(); }
    unsafe_listiterator beginList() const
    { return unsafe_listiterator(*this); }
    unsafe_listiterator endList() const
    { return unsafe_listiterator(); }

private:
    UT_IndexedHashSetItemId storeItemInList(const T &key);

    SYS_FORCE_INLINE
    int getListSize() const
    { return myListSize.relaxedLoad(); }

    SYS_FORCE_INLINE
    bool isValidId(UT_IndexedHashSetItemId id) const
    { return id >= 0 && id < getListSize(); }

    /// Return true if key is not the "clear" key value
    SYS_FORCE_INLINE
    static bool isValidKey(const T &key);

    /// Sets key to the "clear" key value
    SYS_FORCE_INLINE
    static void invalidateKey(T &key);

    UT_IndexedHashSetTable      myMap;
    UT_IndexedHashSetVector     myList;
    UT_IndexedHashSetHoleQueue  myHoles;
    SYS_AtomicCounter           myListSize;
};

#endif

