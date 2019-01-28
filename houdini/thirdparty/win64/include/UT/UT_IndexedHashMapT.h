/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_IndexedHashMap.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_IndexedHashMapT__
#define __UT_IndexedHashMapT__

#include "UT_String.h"
#include "UT_IndexedHashMap.h"

/// Convenience class to use strings as keys in the hash map
class UT_API UT_IndexedHashMapStringKey
{
public:
    /// This allows a key to be built automatically from a const char *.
    UT_IndexedHashMapStringKey(const char *key)
    {
	// We can use a shallow reference for the key since this is only
	// constructed when adding/finding items in the map.
	myString = key;
    }
    UT_IndexedHashMapStringKey(const UT_IndexedHashMapStringKey &src)
    {
	// The copy c-tor is called when storing the key in the map.  Thus, we
	// need to harden the string.
	myString.harden(src.myString);
    }
    const char	*getString() const	{ return myString; }

    uint	hash() const		{ return myString.hash(); }
    bool	isEqual(const UT_IndexedHashMapStringKey &src) const
		    { return strcmp(myString, src.myString) == 0; }
    bool	isLessThan(const UT_IndexedHashMapStringKey &src) const
		    { return strcmp(myString, src.myString) < 0; }
    bool	operator<(const UT_IndexedHashMapStringKey &src) const
		    { return strcmp(myString, src.myString) < 0; }

private:
    UT_String	myString;
};

/// Convenience class for items which just reference their keys.  That is, if
/// all the information is available on the key, the item class can be a simple
/// key container.
///
/// The class has a method that allows for deferred creation given the key
/// reference.
///
/// @note You will likely want to use ut_IndexedHashMapDeferItemAlloc<KEY,ITEM>
/// with this class (since the point of this item class is to allow deferred
/// allocation).
template <typename KEY>
class UT_IndexedHashMapItemKeyContainer
{
public:
    UT_IndexedHashMapItemKeyContainer(const KEY &k)
	: myKey(&k)
    {
    }
    UT_IndexedHashMapItemKeyContainer(const UT_IndexedHashMapItemKeyContainer &i)
	: myKey(i.myKey)
    {
    }
    ~UT_IndexedHashMapItemKeyContainer() { }

    UT_IndexedHashMapItemKeyContainer
		&operator=(const UT_IndexedHashMapItemKeyContainer &src)
		{
		    myKey = src.myKey;
		    return *this;
		}

    const KEY	&getKey() const { return *myKey; }
    bool	operator<(const UT_IndexedHashMapItemKeyContainer &i) const
		    { return *myKey < *i.myKey; }
private:
    const KEY	*myKey;
};

/// A typedef for a string item container.  This is an item container which
/// holds a UT_IndexedHashMapStringKey
typedef UT_IndexedHashMapItemKeyContainer<UT_IndexedHashMapStringKey>
		UT_IndexedHashMapStringItem;

/// An allocator object which dis-allows deferred allocation of items
class UT_API ut_IndexedHashMapDeferNullAlloc
{
public:
    /// Return NULL (don't allow deferred construction).
    static UT_IndexedHashMap::InternalItemT *
		newItem(const UT_IndexedHashMap::InternalKeyT *key)
    {
	UT_ASSERT(0 && "User must define an allocator for deferred allocation");
	return NULL;
    }
};

/// For ITEM objects which support a deferred constructor (for example, the key
/// container item), this can be used as a simple deferred allocator.
template <typename KEY, typename ITEM>
class ut_IndexedHashMapDeferItemAlloc
{
public:
    //// Construct an item when it doesn't already exist in the map
    static UT_IndexedHashMap::InternalItemT *
		newItem(const UT_IndexedHashMap::InternalKeyT *key)
    {
	return new ITEM(*(static_cast<const KEY *>(key)));
    }
};

/// A typedef for an allocator for the string key/item allocation.
typedef ut_IndexedHashMapDeferItemAlloc<UT_IndexedHashMapStringKey, UT_IndexedHashMapStringItem> UT_IndexedHashMapStringAlloc;

/// @brief A thread-safe hash map which stores indexed shared items
///
/// Each item in the hash map is reference counted.  That is, if objects are
/// added multiple times, only a single object will be stored in the map.
///
/// Removing an item from the hash map (by id or key) will decrement the
/// reference count on the item.  When no longer referenced, the map will
/// automatically delete the item.
///
/// As items are added to the map, each is assigned a unique id
/// (UT_IndexedHashMapItemId).  Items can then be retrieved efficiently from
/// the map using the id (UT_IndexedHashMapT::get()).
///
/// Many methods on the map are thread-safe, though some are not.  The thread
/// safety of the methods is described in the comments.
///
/// The KEY template parameter needs to have: @code
///	KEY(const KEY &src);			// The copy constructor
///	uint	hash() const;			// A hash method
///	bool	isEqual(const KEY &src) const;	// A comparison operator
/// @endcode
/// For sorting, there should be a < operator defined on items.
///
/// When adding items to an indexed hash map, only a single item is every
/// created for duplicate keys.  To optimize addition, items can be created
/// only if the item will be added to the map.  This is done through the
/// DEFER_ALLOC template parameter.  With the default argument (which doesn't
/// allow deferred allocation), ITEM objects @b must be passed in the add()
/// method.
template <typename KEY, typename ITEM,
	    typename DEFER_ALLOC=ut_IndexedHashMapDeferNullAlloc>
class UT_IndexedHashMapT : public UT_IndexedHashMap
{
protected:
    /// The indexed hash map maintains all operations on internal types.  This
    /// allows the bulk of the code to be shared between template
    /// instantiations.
    SYS_FORCE_INLINE KEY	*castKEY(InternalKeyT *key) const
				    { return static_cast<KEY *>(key); }
    SYS_FORCE_INLINE const KEY	*castKEY(const InternalKeyT *key) const
				    { return static_cast<const KEY *>(key); }
    SYS_FORCE_INLINE ITEM	*castITEM(InternalItemT *item) const
				    { return static_cast<ITEM *>(item); }
    SYS_FORCE_INLINE const ITEM	*castITEM(const InternalItemT *item) const
				    { return static_cast<const ITEM *>(item); }

    /// Methods for keys
    virtual uint	hash(const InternalKeyT *key) const
			    { return castKEY(key)->hash(); }
    virtual bool	areKeysEqual(const InternalKeyT *k1,
				    const InternalKeyT *k2) const
			    { return castKEY(k1)->isEqual(*castKEY(k2)); }
    virtual InternalKeyT *copyKey(const InternalKeyT *key) const
			    { return new KEY(*castKEY(key)); }
    virtual void	 deleteKey(InternalKeyT *key) const
			    { delete castKEY(key); }

    virtual InternalItemT *newItem(const InternalKeyT *key) const
			    {
				return DEFER_ALLOC::newItem(key);
			    }
    virtual void	 deleteItem(InternalItemT *item) const
			    { delete castITEM(item); }
    virtual bool	 isItemLessThan(const InternalItemT *a,
					const InternalItemT *b) const
			    { return *castITEM(a) < *castITEM(b); }

public:
    UT_IndexedHashMapT(bool store_ids=true)
	: UT_IndexedHashMap(store_ids)
    {}
    virtual ~UT_IndexedHashMapT()
    {
	clear();	// Clear the map
    }

    /// Return approximate memory usage
    /// NOTE: *Not* including KEY or ITEM storage, even though the destructor
    ///       destroys both the KEY and ITEM objects, because the caller should know
    ///       better what KEY and ITEM are.  If they can't vary in size, just
    ///       multiply by entries(); if they can, use begin() to iterate through.
    int64		getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += UT_IndexedHashMap::getMemoryUsage(false);
        return mem;
    }

    /// Add a new item to the map, returning the item actually stored in the
    /// map.  This may not be the item passed in if the map already contains
    /// the object.
    ///
    /// If the item passed in is NULL, an item will be allocated using the
    /// deferred allocator (see the DEFER_ALLOC template argument).  The
    /// default allocator does @b not allow for deferred construction and
    /// requires the item to be passed in.
    /// @note This is thread-safe
    ITEM	*add(const KEY &key, ITEM *item=NULL,
			UT_IndexedHashMapItemId *id=NULL)
		    { return castITEM(_add(&key, item, id)); }

    /// Add reference to an existing item.
    /// @param id   Item index
    /// @param inc  The number of references to add. 
    ///		    It defaults to 1.  If negative, the item may be
    ///             removed in which case it returns 0.
    /// @note This item is thread-safe provided the item is exists in the map
    /// @{
    ITEM	*addReference(UT_IndexedHashMapItemId id, int inc)
		    { return castITEM(_addReference(id, inc)); }
    ITEM	*addReference(UT_IndexedHashMapItemId id)
		    { return castITEM(_addReference(id, 1)); }
    /// @}

    /// Find an item in the map.
    /// @note This is thread-safe
    ITEM	*find(const KEY &key) const
		    { return castITEM(_find(&key)); }

    /// Given a key, find an item's id (or -1 if not found)
    /// @note This is thread-safe
    UT_IndexedHashMapItemId	findId(const KEY &key) const
				    { return _findId(&key); }

    /// Find an item and its id.  This is more efficient than calling find()
    /// and findId() separately.  Calling find() and findId() is also not
    /// always thread-safe.
    /// @note This is thread-safe
    ITEM	*find(const KEY &key, UT_IndexedHashMapItemId &id) const
		    { return castITEM(_findItemAndId(&key, id)); }

    /// Get the item which has the given id
    /// @note This is thread-safe
    ITEM	*get(UT_IndexedHashMapItemId id) const
		    { return castITEM(_get(id)); }

    /// Get the n'th item in the list.  This skips over holes and will return
    /// items for a contiguous list of integers.  For example: @code
    ///   for (int i = 0; ; ++i)
    ///   {
    ///      UT_IndexedHashMapItemId	id;
    ///      ITEM *item = map.getOrderedItem(i, &id);
    ///      if (!item)
    ///           break;
    ///   }
    /// @endcode
    /// The @c UT_IndexedHashMapItemId's returned should be monotonic, but not
    /// contiguous.  If the @c id parameter is non-null, the id for the item
    /// will be stored there.
    /// @note This is @b not thread-safe
    ITEM	*getOrderedItem(exint index,
				UT_IndexedHashMapItemId *id = NULL) const
		    { return castITEM(_getOrderedItem(index, id)); }

    /// Get the key associated with the given id
    /// @note This is thread-safe
    const KEY	*getKey(UT_IndexedHashMapItemId id) const
		    { return castKEY(_getKey(id)); }

    /// Remove an item from the map.  This dereferences the item in the map and
    /// returns true if the item was deleted from the map.  The method will
    /// return false if the item was shared and is still in the map.
    /// @note This is thread-safe
    bool	remove(const KEY &key)
		    { return _remove(&key); }

    /// Remove the item with the given id from the map.
    /// @note This is thread-safe
    bool	remove(UT_IndexedHashMapItemId id)
		    { return _remove(id); }

    /// Replace an item at a given id.
    ///
    /// The existing item will be deleted from the table and the new item will
    /// replace it.
    /// The method will fail if there is currently no item at the given
    /// location.
    ///
    /// The method will return the new id for the item.  Usually, this will be
    /// the same as the existing id, unless the new item already exists in the
    /// map.
    ///
    /// If the item passed in is NULL, a new item will be called using newItem()
    ///
    /// @warning If the returned @c id does not match the @c id passed in, it's
    /// the user's responsibility to update any references to the new id.
    /// @note This is thread-safe
    UT_IndexedHashMapItemId	replaceItem(UT_IndexedHashMapItemId id,
					const KEY &key, ITEM *item=NULL)
				    { return _replaceItem(id, &key, item); }

    /// Replace the object given by the @c old_key with the value given by the
    /// @c new_key.
    ///
    /// The @c old_key will be removed by the map.
    ///
    /// If the map stores id's, the @c new_key will be assigned the same id as
    /// the @c old_key unless the @c new_key already exists in the map.
    ///
    /// The @c new_key object will inherit all references of the @c old_key.
    ///
    /// This method returns the id of the new item.  This will be the same id
    /// as the old object unless the @c new_key already exists in the map.
    /// @note This is thread-safe
    UT_IndexedHashMapItemId	replaceItem(const KEY &old_key,
					const KEY &new_key)
				    { return _replaceItem(&old_key, &new_key); }

    /// Extract the items into a list of ids and pointers to items.  Items will
    /// be appended to the lists.  The function returns the number of items
    /// added.
    /// @note This is @b not thread-safe
    exint	extractItems(
		    UT_Array<UT_IndexedHashMapItemId> &ids,
		    UT_Array<ITEM *> &items
		) const
		{
		    return _extractItems(ids,
			reinterpret_cast< UT_Array<InternalItemT *> & >(items));
		}
    exint	extractItems(
		    UT_Array<UT_IndexedHashMapItemId> &ids,
		    UT_Array<ITEM *> &items,
		    exint maxitems
		) const
		{
		    return _extractItems(ids,
			reinterpret_cast< UT_Array<InternalItemT *> & >(items),
			maxitems);
		}

    /// Extract an array of pointers to items.
    ///
    /// If there are id's associated with the items, the items will be stored
    /// at their indexed location.  The list of items may contain NULL pointers
    /// for unused index entries.
    ///
    /// If there are no id's, the items will be extracted in arbitrary order
    /// into a packed array.
    ///
    /// The function returns the number of items in the array.
    /// @note This is @b not thread-safe
    exint	extractItems(UT_Array<ITEM *> &items) const
		{
		    return _extractItems(
			reinterpret_cast< UT_Array<InternalItemT *> & >(items));
		}


    /// @{
    /// Convenience operators
    ITEM	*operator[](UT_IndexedHashMapItemId id) const
				    { return get(id); }
    ITEM	*operator[](const KEY &k) const
				    { return find(k); }
    ITEM	*operator()(UT_IndexedHashMapItemId id) const
				    { return get(id); }
    ITEM	*operator()(const KEY &k) const
				    { return find(k); }
    /// @}

    /// Replaces the content of this with the content of src.
    void replace(const UT_IndexedHashMapT<KEY, ITEM, DEFER_ALLOC> &src)
    {
        _replace(src);
    }
};

/// A string map
typedef UT_IndexedHashMapT<UT_IndexedHashMapStringKey,
			    UT_IndexedHashMapStringItem,
			    UT_IndexedHashMapStringAlloc> UT_IndexedStringMap;

#endif
