/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_IndexedHashSetImpl.h (UT Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __UT_IndexedHashSetImpl__
#define __UT_IndexedHashSetImpl__

#include "UT_IndexedHashSet.h"
#include "UT_ArraySet.h" // Just for UT::DefaultClearer
#include "UT_Assert.h"
#include <vector>

template<typename T>
void
UT_IndexedHashSet<T>::clear()
{
    myList.clear();
    myHoles.clear();
    myMap.clear();
    myListSize.relaxedStore(0);
}

template<typename T>
bool
UT_IndexedHashSet<T>::isValidKey(const T &key)
{
    return !UT::DefaultClearer<T>::isClear(key);
}

template<typename T>
void
UT_IndexedHashSet<T>::invalidateKey(T &key)
{
    UT::DefaultClearer<T>::clear(key);
}

template<typename T>
void
UT_IndexedHashSet<T>::replace(const UT_IndexedHashSet &src)
{
    if (this == &src)
        return;

    clear();

    // All of the IDs and reference counts need to be the same as in src.
    // Just in case, the order of the holes should be the same, too.

    exint listsize = src.myList.size();
    myList.resize(listsize);
    myListSize.relaxedStore(listsize);
    myMap.rehash(src.myMap.size());

    for (exint id = 0; id < listsize; ++id)
    {
        const T &srckey = src.myList[id];
        if (!isValidKey(srckey))
        {
            invalidateKey(myList[id]);
            continue;
        }
        myList[id] = srckey;

        exint refcount;
        {
            typename UT_IndexedHashSetTable::const_accessor a;
            src.myMap.find(a, srckey);
            refcount = a->second.getRef();
        }
        // There shouldn't be any conflicts, since we cleared above.
        myMap.insert(std::pair<T,IdAndRefCount>(srckey, IdAndRefCount(id, refcount)));
    }

    if (!src.myHoles.empty())
    {
        // Unfortunately, tbb::concurrent_queue::assign isn't accessible,
        // so let's destruct and copy construct in-place.
        myHoles.~UT_IndexedHashSetHoleQueue();
        new (&myHoles) UT_IndexedHashSetHoleQueue(src.myHoles);
    }
}

template<typename T>
const T *
UT_IndexedHashSet<T>::getOrderedItem(exint idx,
    UT_IndexedHashSetItemId *id_store) const
{
    if (myHoles.empty())
    {
	if (id_store)
	    *id_store = idx;
	return get((UT_IndexedHashSetItemId)idx);
    }
    exint find = 0;
    for (unsafe_listiterator it = beginList(); !it.atEnd(); ++it, ++find)
    {
	if (find == idx)
	{
	    if (id_store)
		*id_store = it.getItemId();
	    return &it.getKey();
	}
    }
    return nullptr;
}

template<typename T>
const T *
UT_IndexedHashSet<T>::addReference(UT_IndexedHashSetItemId id, int inc)
{
    if (!isValidId(id))
        return nullptr;

    // We need to "lock" this item since bumpRef() is not thread-safe
    typename UT_IndexedHashSetTable::accessor a;
    if (myMap.find(a, myList[id]))
    {
        // Check to see if this brought us to zero.
        if (!a->second.bumpRef(inc))
        {
	    // Last reference to the item

	    // Clear the key
	    UT_IndexedHashSetItemId id = a->second.getId();
	    UT_ASSERT(isValidId(id));
	    invalidateKey(myList[id]);
	    myHoles.push(id);	// Now safe to use

	    // Erase from the map
	    myMap.erase(a);

            // We definitely don't want to return item now!
            return nullptr;
        }

        return &myList[id];
    }

    // Looking up ID for clear key.
    return nullptr;
}

template<typename T>
exint
UT_IndexedHashSet<T>::getReferenceCount(UT_IndexedHashSetItemId id) const
{
    if (!isValidId(id))
        return 0;

    // We need to "lock" this item since getRef() is not thread-safe
    typename UT_IndexedHashSetTable::accessor a;
    if (myMap.find(a, myList[id]))
    {
        return a->second.getRef();
    }

    return 0;
}

template<typename T>
exint
UT_IndexedHashSet<T>::getReferenceCount(const T &key) const
{
    // We need to "lock" this item since getRef() is not thread-safe
    typename UT_IndexedHashSetTable::accessor a;
    if (myMap.find(a, key))
    {
        return a->second.getRef();
    }

    return 0;
}

template<typename T>
UT_IndexedHashSetItemId
UT_IndexedHashSet<T>::storeItemInList(const T &key)
{
    UT_IndexedHashSetItemId offset;
    if (myHoles.try_pop(offset))
    {
	UT_ASSERT(offset >= 0 && offset < myList.size());
	myList[offset] = key;
    }
    else
    {
	offset = myList.push_back(key) - myList.begin();
	myListSize.maximum(offset+1);
    }
    return offset;
}

template<typename T>
UT_IndexedHashSetItemId
UT_IndexedHashSet<T>::add(const T &key)
{
    if (UT::DefaultClearer<T>::isClear(key))
        return UT_IndexedHashSetItemId(-1);
    typename UT_IndexedHashSetTable::accessor a;
    if (myMap.insert(a, key))
    {
        // Create an item and store it in the map.
        // Use the key from the map, since it has the hash computed.
        UT_IndexedHashSetItemId id = storeItemInList(a->first);
        a->second.setId(id);
        a->second.setRef(1);
    }
    else
    {
        // Increment the reference count on the container (not the item)
        a->second.bumpRef(1);
    }
    return a->second.getId();
}

template<typename T>
const T *
UT_IndexedHashSet<T>::findItemAndId(
    const T &key,
    UT_IndexedHashSetItemId &id) const
{
    typename UT_IndexedHashSetTable::const_accessor a;
    if (myMap.find(a, key))
    {
        id = a->second.getId();
        return &a->first;
    }
    return nullptr;
}

template<typename T>
bool
UT_IndexedHashSet<T>::remove(const T &key)
{
    typename UT_IndexedHashSetTable::accessor a;
    if (myMap.find(a, key))
    {
        // We have write access to object
        if (!a->second.bumpRef(-1))
        {
            // Last reference to the item

            // Clear the pointer
            UT_IndexedHashSetItemId id = a->second.getId();
            UT_ASSERT(isValidId(id));
            invalidateKey(myList[id]);
            myHoles.push(id); // Now safe to use

            // Erase from the map
            myMap.erase(a);

            // true means removed
            return true;
        }
    }
    return false;
}

template<typename T>
bool
UT_IndexedHashSet<T>::remove(UT_IndexedHashSetItemId id)
{
    if (!isValidId(id))
        return false;

    const T &key = myList[id];
    if (isValidKey(key))
    {
        return remove(key);
    }
    return false;
}

template<typename T>
UT_IndexedHashSetItemId
UT_IndexedHashSet<T>::replaceItem(
    UT_IndexedHashSetItemId id,
    const T &key)
{
    if (!isValidId(id) || !isValidKey(myList[id]))
        return -1;

    typename UT_IndexedHashSetTable::accessor a;
    // Find the item in the map so that we can replace it
    const T &okey = myList[id];
    if (!myMap.find(a, okey))
    {
        UT_ASSERT(0 && "Missing object that's in the map!");
        return -1;
    }

    // Get reference count of the current item
    exint irefcount = a->second.getRef();

    // Assert the object is in the location we expect.
    UT_ASSERT(a->second.getId() == id);

    // Remove the previous item from the list/map and delete it.
    invalidateKey(myList[id]);  // Clear out item from list
    myMap.erase(a);

    if (UT::DefaultClearer<T>::isClear(key))
    {
        // Since we've cleaned out the previous entry, and there isn't one
        // for the clear value, just add the id to the hole list.
        myHoles.push(id); // We can write into this list

        return UT_IndexedHashSetItemId(-1);
    }

    // Now, insert the new item
    if (myMap.insert(a, key))
    {
        // Store a new item in the map
        a->second.setId(id);
        a->second.setRef(irefcount);
        // Use the key from the map, since it has the hash computed
        myList[id] = a->first;
    }
    else
    {
        // Hey! There's already an item which matches the key

        // Since we've cleaned out the previous entry, we can now write to the
        // object in the list, so add the id to the hole list.
        myHoles.push(id); // We can write into this list

        // It's up to user to update references
        a->second.bumpRef(irefcount);

        // Get the appropriate id
        id = a->second.getId();
    }
    return id;
}

template<typename T>
bool
UT_IndexedHashSet<T>::compactIds(IdRemapping &remapping)
{
    if (myHoles.empty())
	return false;
    exint n = getListSize();

    remapping.prepare(n);
    exint d = 0;
    for (exint s = 0; s < n; ++s)
    {
        const T &skey = myList[s];
        if (!isValidKey(skey))
            continue;

        remapping.setId(s, d);
        if (s != d)
        {
            {
                typename UT_IndexedHashSetTable::accessor a;
                UT_VERIFY_P(myMap.find(a, skey));
                a->second.setId(d);
            }
            myList[d] = skey;
        }
        d++;
    }
    myList.resize(d);	// Shrink to 'd' elements
    myListSize.relaxedStore(myList.size());
    myHoles.clear();	// No holes
    return true;
}

template<typename T>
template<typename P>
bool
UT_IndexedHashSet<T>::sortItems(const P &predicate)
{
    if (!myHoles.empty())
	return false;

    // There's no sort on concurrent list, so we need to throw the items into a
    // normal list for sorting.
    UT_ASSERT(myListSize.relaxedLoad() == myList.size());
    exint nitems = myListSize.relaxedLoad();
    std::vector<T> items;
    items.reserve(nitems);
    for (exint i = 0; i < nitems; ++i)
    {
	items.push_back(myList[i]);
    }
    std::stable_sort(items.begin(), items.end(), predicate);
    // Now, throw back into my concurrent list
    for (exint i = 0; i < nitems; ++i)
    {
	myList[i] = items[i];
	myMap[items[i]].setId(i); // Set new location
    }
    return true;
}

template<typename T>
template<typename ID_ARRAY, typename T_ARRAY>
exint
UT_IndexedHashSet<T>::extractItems(
    ID_ARRAY &ids,
    T_ARRAY &items,
    exint maxitems) const
{
    if (maxitems == 0)
        return 0;

    UT_ASSERT(myListSize.relaxedLoad() == myList.size());
    exint nitems = myListSize.relaxedLoad();
    for (exint i = 0; i < nitems; ++i)
    {
        if (isValidKey(myList[i]))
        {
            // NOTE: GA_ATIBlob::extractBlobs() and
            //       GA_ATIBlobArray::extractBlobs rely on ids
            //       being in ascending order.  (See Question #71439.)
            ids.append(i);
            items.append(myList[i]);

            if (items.size() >= maxitems)
                break;
        }
    }
    return items.size();
}

template<typename T>
template<typename ID_ARRAY, typename T_ARRAY>
exint
UT_IndexedHashSet<T>::extractItems(
    ID_ARRAY &ids,
    T_ARRAY &items) const
{
    UT_ASSERT(myListSize.relaxedLoad() == myList.size());
    exint nitems = myListSize.relaxedLoad();
    for (exint i = 0; i < nitems; ++i)
    {
        if (isValidKey(myList[i]))
        {
            // NOTE: GA_ATIBlob::extractBlobs() and
            //       GA_ATIBlobArray::extractBlobs rely on ids
            //       being in ascending order.  (See Question #71439.)
            ids.append(i);
            items.append(myList[i]);
        }
    }
    return items.size();
}

template<typename T>
template<typename T_ARRAY>
exint
UT_IndexedHashSet<T>::extractItems(
    T_ARRAY &items) const
{
    exint nitems = myListSize.relaxedLoad();
    for (exint i = 0; i < nitems; ++i)
    {
        if (isValidKey(myList[i]))
            items.append(myList[i]);
    }
    return items.size();
}

template<typename T>
int64
UT_IndexedHashSet<T>::getMemoryUsage(bool inclusive) const
{
    int64 mem = inclusive ? sizeof(*this) : 0;

    UT_ASSERT(myListSize.relaxedLoad() == myList.size());
    mem += UTgetMemoryUsage(myList, false);
    mem += UTgetMemoryUsage(myMap, false);
    mem += UTgetMemoryUsage(myHoles, false);
    return mem;
}

#endif
