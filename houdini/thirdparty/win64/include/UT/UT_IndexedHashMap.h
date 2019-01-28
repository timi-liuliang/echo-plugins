/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_IndexedHashMap.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_IndexedHashMap__
#define __UT_IndexedHashMap__

#include "UT_API.h"
#include <SYS/SYS_AtomicInt.h>
#include "UT_Assert.h"
#include "UT_Array.h"
#include "UT_VectorTypes.h"
#include "UT_ConcurrentHashMap.h"
#include "UT_ConcurrentVector.h"
#include "UT_ConcurrentQueue.h"

/// Each item in the shared map is assigned a unique id
typedef int	UT_IndexedHashMapItemId;

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
/// (UT_IndexedHashMapItemId).  The item can then be retrieved efficiently from
/// the map using the id (UT_IndexedHashMap::get())
///
/// Many methods on the map are thread-safe.  Some methods are not (and are
/// noted in the comments).
///
/// The KEY template parameter needs to have: @code
///	KEY(const KEY &src);			// The copy constructor
///	uint	hash() const;			// A hash method
///	bool	isEqual(const KEY &src) const;	// A comparison operator
/// @endcode
class UT_API UT_IndexedHashMap
{
public:
    typedef void	InternalKeyT;	// Internal key type
    typedef void	InternalItemT;	// Internal item type

protected:
    virtual uint		 hash(const InternalKeyT *key) const = 0;
    virtual bool		 areKeysEqual(const InternalKeyT *k1,
					const InternalKeyT *k2) const = 0;
    virtual InternalKeyT	*copyKey(const InternalKeyT *key) const = 0;
    virtual void		 deleteKey(InternalKeyT *key) const = 0;

    virtual InternalItemT	*newItem(const InternalKeyT *key) const = 0;
    virtual void		 deleteItem(InternalItemT *item) const = 0;
    virtual bool		 isItemLessThan(const InternalItemT *a,
					const InternalItemT *b) const = 0;

public:
    /// If @c store_ids is true, each item stored in the map will be given a
    /// unique id of type UT_IndexedHashMapItemId.  These id's can be used to
    /// perform efficient lookup of items in the map.
    ///
    /// If the @c store_ids parameter is false, then elements in the shared map
    /// will not store id's.  The ids for the elements will always be -1
    /// (invalid).  The map won't store the structures required for indexed
    /// lookup, saving some memory and allowing some operations to be slightly
    /// more efficient.  Operations which are invalid for maps that don't store
    /// id's are noted in the comments).
    UT_IndexedHashMap(bool store_ids);
    virtual ~UT_IndexedHashMap();

    /// Clear the map
    /// @note This is @b not thread-safe
    void		clear();

    /// Return the number of entries in the map.
    /// @note This is @b not thread-safe
    exint		entries() const { return myMap.size(); }

    /// Return whether the map is empty
    bool		empty() const	{ return myMap.empty(); }

    /// Return approximate memory usage (not including key or item storage)
    int64		getMemoryUsage(bool inclusive) const;

    /// Return the maximum possible UT_IndexedHashMapItemId stored in the map.
    /// This returns an upper bound and is not exact.
    /// @note This is @b not thread-safe
    /// @note Not supported if id's are not stored in the map
    UT_IndexedHashMapItemId	getItemIdUpperBound() const
				    { return SYSmax(1, getListSize())-1; }

    /// Return the "occupancy" of the map.
    /// @note This is @b not thread-safe
    /// @note Not supported if id's are not stored in the map
    fpreal		getOccupancy() const
			{
			    exint	lsize = getListSize();
			    exint	hsize = myHoles.unsafe_size();
			    if (!lsize || !hsize)
				return 1;	// Fully occupied
			    UT_ASSERT(lsize > hsize);
			    return (fpreal)(lsize-hsize)/(fpreal)lsize;
			}

    /// Class used by compacting to map from the previous id to the new id.
    /// After compacting, this class stores a map of the old id's to the new
    /// id's
    class IdRemapping {
    public:
	 IdRemapping() {}
	~IdRemapping() {}

	/// Find the number of entries in the map
	exint	entries() const	{ return myIdMap.entries(); }

	/// Query the new id associated with the previous id
	UT_IndexedHashMapItemId	newId(UT_IndexedHashMapItemId prev) const
				{
				    if (prev >= 0 && prev < myIdMap.entries())
					return myIdMap(prev);
				    return -1;
				}

	/// @private Used by UT_IndexedHashMap::compactIds()
	void	prepare(exint size)
		{
		    myIdMap.entries(size);	// Grow to expected size
		    myIdMap.constant(-1);		// Initialize to -1
		}
	/// @private Used by UT_IndexedHashMap::compactIds()
	void	setId(UT_IndexedHashMapItemId prev, UT_IndexedHashMapItemId curr)
		{
		    UT_ASSERT(myIdMap(prev) == -1);
		    myIdMap(prev) = curr;
		}
    private:
	UT_Array<exint>		myIdMap;
    };

    /// Compact the list.  This fills out the integer map of old id's and their
    /// new id's.  If no compaction was done, the function returns false.
    /// @note This is @b not thread-safe
    /// @note This is a no-op if id's are not stored in the map
    bool		compactIds(IdRemapping &remapping);

    /// Sort the list of ids based on the comparator.  This method only works
    /// if the table has been compacted.  Returns false if there are no ids or
    /// the list is not compacted.
    /// @note This is @b not thread-safe
    /// @note This is a no-op if id's are not stored in the map
    bool		sortItems();

    exint               getReferenceCount(UT_IndexedHashMapItemId id) const;

protected:
    // @{
    // Internal implementation using internal types rather than exposed types.
    // None of these methods use the template types and thus are only generated
    // one time (outlined).
    void                 _replace(const UT_IndexedHashMap &src);
    InternalItemT	*_add(const InternalKeyT *key,
				InternalItemT *item=NULL,
				UT_IndexedHashMapItemId *id=NULL);
    InternalItemT	*_addReference(UT_IndexedHashMapItemId id, int inc);
    InternalItemT	*_addReference(UT_IndexedHashMapItemId id)
			    { return _addReference(id, 1); }

    InternalItemT	*_find(const InternalKeyT *key) const
			 {
			    UT_IndexedHashMapItemId id;
			    return _findItemAndId(key, id);
			 }
    exint		_findId(const InternalKeyT *key) const
			{
			    UT_IndexedHashMapItemId hid;
			    if (!_findItemAndId(key, hid))
				hid = -1;
			    return hid;
			}
    InternalItemT	*_findItemAndId(const InternalKeyT *key,
				UT_IndexedHashMapItemId &id) const;
    InternalItemT	*_get(UT_IndexedHashMapItemId id) const;
    const InternalKeyT	*_getKey(UT_IndexedHashMapItemId id) const;
    InternalItemT	*_getOrderedItem(exint index,
				    UT_IndexedHashMapItemId *id) const;
    bool		_remove(const InternalKeyT *key);
    bool		_remove(UT_IndexedHashMapItemId id);
    UT_IndexedHashMapItemId	_replaceItem(UT_IndexedHashMapItemId id,
					    const InternalKeyT *key,
					    InternalItemT *new_item=NULL);
    exint		_extractItems(
				UT_Array<UT_IndexedHashMapItemId> &ids,
				UT_Array<InternalItemT *> &items,
				exint maxitems
			    ) const;
    exint		_extractItems(
				UT_Array<UT_IndexedHashMapItemId> &ids,
				UT_Array<InternalItemT *> &items
			    ) const;
    exint		_extractItems(
				UT_Array<InternalItemT *> &items
			    ) const;
    /// @}


    // These classes need to be defined for the iterator
    // Class to store reference counted items in the map
    friend class	itemContainer;
    class itemContainer
    {
    public:
	itemContainer(const UT_IndexedHashMap &map,
			InternalItemT *item,
			exint id)
	    : myMap(map)
	    , myItem(item)
	    , myId(id)
	    , myRefCount(0)
	{}
	~itemContainer()
	{
	    myMap.deleteItem(myItem);
	}

	InternalItemT	*getItem() const	{ return myItem; }
	exint		 getId() const		{ return myId; }
	void		 setId(exint id)	{ myId = id; }
	exint		 getRef() const	{ return myRefCount; }
	void		 setRef(int d)		{ myRefCount = d; }
	int		 bumpRef(int d)
			 {
			    myRefCount += d;
			    return myRefCount;
			 }

    private:
	const UT_IndexedHashMap &myMap;
	InternalItemT		*myItem;
	exint			 myId;
	int			 myRefCount;
    };

    // Class to search for items in the map
    friend class keyContainer;
    class keyContainer
    {
    public:
	// Here, we just hold a reference to the key rather than duplicating
	// the key.
	explicit keyContainer(const UT_IndexedHashMap &map,
				const InternalKeyT *key)
	    : myMap(map)
	    , myKey(key)
	    , myOwn(false)
	{}
	keyContainer(const keyContainer &src)
	    : myMap(src.myMap)
	    , myKey(src.myKey ? src.myMap.copyKey(src.myKey) : NULL)
	    , myOwn(true)
	{}
	~keyContainer()
	{
	    if (myOwn)
		myMap.deleteKey(const_cast<InternalKeyT *>(myKey));
	}
	keyContainer	&operator=(const keyContainer &src)
			{
			    UT_ASSERT(0);
			    if (myKey != src.myKey)
			    {
				if (myOwn)
				    myMap.deleteKey(const_cast<InternalKeyT *>(myKey));
				// Make a hard copy
				myKey = src.myKey ?
					src.myMap.copyKey(src.myKey) : NULL;
			    }
			    return *this;
			}
	const InternalKeyT	*getKey() const
				{
				    UT_ASSERT(myOwn);
				    return myKey;
				}
	uint		hash() const
			{
			    UT_ASSERT(myKey);
			    return myMap.hash(myKey);
			}
	bool		isEqual(const keyContainer &b) const
			{
			    UT_ASSERT(myKey && b.myKey);
			    return myMap.areKeysEqual(myKey, b.myKey);
			}
    private:
	const UT_IndexedHashMap &myMap;
	const InternalKeyT	*myKey;
	bool			 myOwn;
    };

    // Class to store items in the indexed list
    class listContainer
    {
    public:
	listContainer()
	    : myItem(NULL)
	    , myKey(NULL)
	{}
	listContainer(itemContainer *i, const InternalKeyT *k)
	    : myItem(i)
	    , myKey(k)
	{}
	listContainer(const listContainer &src)
	    : myItem(src.myItem)
	    , myKey(src.myKey)
	{}
	listContainer	&operator=(const listContainer &src)
			{
			    myItem = src.myItem;
			    myKey = src.myKey;
			    return *this;
			}
	bool		isValid() const	{ return myItem; }

	InternalItemT		*getItem() const
				 {
				     return myItem ? myItem->getItem() : NULL;
				 }
	const InternalKeyT	*getKey() const { return myKey; }

	void			 setId(exint id) { myItem->setId(id); }
	exint			 getId() const { return myItem->getId(); }
	exint			 getRef() const
				    { return myItem ? myItem->getRef() : -1; }

	itemContainer		*getItemContainer() { return myItem; }

    private:
	itemContainer		*myItem;
	const InternalKeyT	*myKey;
    };

    // Comparison class for hash map
    class keyCompare
    {
    public:
	static uint	hash(const keyContainer &key)
			{
			    return key.hash();
			}
	static bool	equal(const keyContainer &a, const keyContainer &b)
			{
			    return a.isEqual(b);
			}
    };

    // Class used to sort items
    friend class itemCompare;
    class itemCompare
    {
    public:
	itemCompare(const UT_IndexedHashMap &map)
	    : myMap(map)
	{}
	bool	operator()(const listContainer &a, const listContainer &b) const
		{
		    return myMap.isItemLessThan(a.getItem(), b.getItem());
		}
    private:
	const UT_IndexedHashMap	&myMap;
    };

    typedef UT_ConcurrentHashMap<keyContainer, itemContainer *, keyCompare>
						UT_IndexedHashMapTable;
    typedef UT_ConcurrentVector<listContainer>  UT_IndexedHashMapVector;
    typedef UT_ConcurrentQueue<UT_IndexedHashMapItemId>
						UT_IndexedHashMapHoleQueue;

public:
    /// Iterate over items in the list - this is in the order they are stored
    /// in the map (i.e. by id).
    class UT_API unsafe_listiterator
    {
	unsafe_listiterator()
	    : myMap(NULL)
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
	const InternalKeyT	*getKey() const
				    { return myIterator->getKey(); }
	InternalItemT		*getItem() const
				    { return myIterator->getItem(); }
	UT_IndexedHashMapItemId	 getItemId() const
				    { return myIterator->getId(); }
	exint			 getItemShareCount() const
				    { return myIterator->getRef(); }
	template <typename T> const T	*keyAs() const
				{ return static_cast<const T *>(getKey()); }
	template <typename T> const T	*itemAs() const
				{ return static_cast<const T *>(getItem()); }
	/// @}

	/// @{
	/// Implementation of iterator interface
	bool		atEnd() const	{ return myCurr >= mySize; }
	void		advance()
			{
			    do
			    {
				myCurr++;
				myIterator++;
			    } while (myCurr < mySize && !myIterator->isValid());
			}
	unsafe_listiterator	&operator++()	{ advance(); return *this; }
	bool		operator==(const unsafe_listiterator &it) const
			{
			    if (atEnd() && it.atEnd())
				return true;
			    return myMap == it.myMap &&
				    mySize == it.mySize &&
				    myCurr == it.myCurr;
			}
	bool		operator!=(const unsafe_listiterator &it)
			    { return !(*this == it); }
	/// @}
    private:
	unsafe_listiterator(const UT_IndexedHashMap &map)
	    : myMap(&map)
	    , myIterator(map.myList.begin())
	    , myCurr(0)
	    , mySize(map.entries())
	{
	}
	const UT_IndexedHashMap			*myMap;
	UT_IndexedHashMapVector::const_iterator	 myIterator;
	exint					 mySize, myCurr;
	friend class UT_IndexedHashMap;
    };
    /// Iterate over items in the map - this is arbitrary order
    class UT_API unsafe_iterator
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
	const InternalKeyT	*getKey() const
				    { return myIterator->first.getKey(); }
	InternalItemT		*getItem() const
				    { return myIterator->second->getItem(); }
	UT_IndexedHashMapItemId	 getItemId() const
				    { return myMap->_findId(getKey()); }
	exint			 getItemShareCount() const
				    { return myIterator->second->getRef();}

	template <typename T> const T	*keyAs() const
				{ return static_cast<const T *>(getKey()); }
	template <typename T> const T	*itemAs() const
				{ return static_cast<const T *>(getItem()); }
	/// @}

	/// @{
	/// Implementation of iterator interface
	bool		atEnd() const		{ return myCurr >= mySize; }
	void		advance()
			{
			    ++myCurr;		// Move my count
			    ++myIterator;	// Move my iterator
			    // Assert that the iterator doesn't terminate early
			    UT_ASSERT(myCurr >= mySize ||
					myIterator != myMap->myMap.end());
			}
	unsafe_iterator	&operator++()		{ advance(); return *this; }
	// No post increment as it is dangerous.
	bool		operator==(const unsafe_iterator &it) const
			{
			    if (atEnd() && it.atEnd())
				return true;
			    return myMap == it.myMap &&
				    mySize == it.mySize &&
				    myCurr == it.myCurr;
			}
	bool		operator!=(const unsafe_iterator &it)
			    { return !(*this == it); }
	/// @}
    private:
	unsafe_iterator(const UT_IndexedHashMap &map)
	    : myMap(&map)
	    , myIterator(map.myMap.begin())
	    , myCurr(0)
	    , mySize(map.entries())
	{
	}
	const UT_IndexedHashMap			*myMap;
	UT_IndexedHashMapTable::const_iterator	 myIterator;
	exint					 mySize, myCurr;
	friend class UT_IndexedHashMap;
    };
    unsafe_iterator	begin() const
			    { return unsafe_iterator(*this); }
    unsafe_iterator	end() const
			    { return unsafe_iterator(); }
    unsafe_listiterator	beginList() const
			    { return unsafe_listiterator(*this); }
    unsafe_listiterator	endList() const
			    { return unsafe_listiterator(); }

private:
    UT_IndexedHashMapItemId	storeItemInList(itemContainer *item,
					const InternalKeyT *key);

    int			getListSize() const
			    { return myListSize.relaxedLoad(); }
    bool		isValidId(UT_IndexedHashMapItemId id) const
			    { return id >= 0 && id < getListSize(); }

    UT_IndexedHashMapTable	myMap;
    UT_IndexedHashMapVector	myList;
    UT_IndexedHashMapHoleQueue	myHoles;
    SYS_AtomicInt32		myListSize;
    bool			myStoreIds;
};

#endif

