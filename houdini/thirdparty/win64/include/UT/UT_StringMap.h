/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_StringMap.h
 *
 * COMMENTS:	No UT_API here because there's no lib implementation!
 */

#pragma once

#ifndef __UT_StringMap_h__
#define __UT_StringMap_h__

#include "UT_ArrayStringMap.h" // This is only included to avoid sweeping for it right now.
#include "UT_Map.h"
#include "UT_StringHolder.h"

/// We want methods like find() to take a const UT_StringRef& instead of a
/// const UT_StringHolder& for the following reasons:
/// - This allows a const char* to be passed in without forcing a copy of the string.
/// - A UT_StringRef can be used without converting it to a UT_StringHolder
/// (which hardens the string if necessary).
/// - A UT_StringHolder can still be directly passed in.
#define UT_STRINGREF_WRAPPER(return_type, name, qualifier) \
    SYS_FORCE_INLINE return_type name(const UT_StringRef &key) qualifier \
    { \
        return Parent::name(UTmakeUnsafeRef(key)); \
    }
/// Specialization of the above macro for methods that return an iterator
/// range, since something like std::pair<iterator, iterator> is interpreted as
/// two arguments when being passed to a macro (due to the comma).
#define UT_STRINGREF_WRAPPER_RANGE(iterator_type, name, qualifier) \
    SYS_FORCE_INLINE std::pair<iterator_type, iterator_type> \
    name(const UT_StringRef &key) qualifier \
    { \
        return Parent::name(UTmakeUnsafeRef(key)); \
    }

/// UT_StringMap is a simple specialization of a UT_Map that has
/// UT_StringHolder as its key type which allows C strings to be used.
/// If you know that a string will not be destroyed during the map's
/// lifetime, UTmakeUnsafeRef can be used to insert a shallow reference.
template <typename ITEM_T>
class UT_StringMap : public UT_Map<UT_StringHolder, ITEM_T>
{
    typedef UT_Map<UT_StringHolder, ITEM_T> Parent;

public:
    typedef typename Parent::const_iterator const_iterator;
    typedef typename Parent::iterator iterator;
    typedef typename Parent::size_type size_type;

    // Expose the erase() overloads that take iterators.
    // The erase(const UT_StringHolder&) overload is hidden in favour of
    // erase(const UT_StringRef &) from UT_STRINGREF_WRAPPER.
    iterator erase(const_iterator pos) { return Parent::erase(pos); }
    iterator erase(const_iterator first, const_iterator last)
    {
        return Parent::erase(first, last);
    }

    UT_STRINGREF_WRAPPER(ITEM_T &, at, )
    UT_STRINGREF_WRAPPER(const ITEM_T &, at, const)
    UT_STRINGREF_WRAPPER(bool, contains, const)
    UT_STRINGREF_WRAPPER(size_type, count, const)
    UT_STRINGREF_WRAPPER_RANGE(iterator, equal_range, )
    UT_STRINGREF_WRAPPER_RANGE(const_iterator, equal_range, const)
    UT_STRINGREF_WRAPPER(size_type, erase, )
    UT_STRINGREF_WRAPPER(iterator, find, )
    UT_STRINGREF_WRAPPER(const_iterator, find, const)
};

/// Same as UT_StringMap, but keeps the strings in a sorted order.
template <typename ITEM_T>
class UT_SortedStringMap : public UT_SortedMap<UT_StringHolder, ITEM_T>
{
    typedef UT_SortedMap<UT_StringHolder, ITEM_T> Parent;

public:
    typedef typename Parent::const_iterator const_iterator;
    typedef typename Parent::iterator iterator;
    typedef typename Parent::size_type size_type;

    // NOTE: When we compile against a more modern standard library on OSX, the
    // signature can be changed to return an iterator to the following element
    // (the signature was changed in C++11).
    void erase(const_iterator pos) { Parent::erase(pos); }
    void erase(const_iterator first, const_iterator last)
    {
        Parent::erase(first, last);
    }

    UT_STRINGREF_WRAPPER(ITEM_T &, at, )
    UT_STRINGREF_WRAPPER(const ITEM_T &, at, const)
    UT_STRINGREF_WRAPPER(bool, contains, const)
    UT_STRINGREF_WRAPPER(size_type, count, const)
    UT_STRINGREF_WRAPPER_RANGE(iterator, equal_range, )
    UT_STRINGREF_WRAPPER_RANGE(const_iterator, equal_range, const)
    UT_STRINGREF_WRAPPER(size_type, erase, )
    UT_STRINGREF_WRAPPER(iterator, find, )
    UT_STRINGREF_WRAPPER(const_iterator, find, const)
    UT_STRINGREF_WRAPPER(iterator, lower_bound, )
    UT_STRINGREF_WRAPPER(const_iterator, lower_bound, const)
    UT_STRINGREF_WRAPPER(iterator, upper_bound, )
    UT_STRINGREF_WRAPPER(const_iterator, upper_bound, const)
};

#undef UT_STRINGREF_WRAPPER
#undef UT_STRINGREF_WRAPPER_RANGE

#endif
