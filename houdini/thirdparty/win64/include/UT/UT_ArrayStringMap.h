/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_ArrayStringMap.h
 *
 * COMMENTS:    An array-based hash map for UT_StringHolder mapping to some type.
 */

#pragma once

#ifndef __UT_ArrayStringMap_h__
#define __UT_ArrayStringMap_h__

#include "UT_ArrayMap.h"
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

/// WARNING: This class DOES NOT ALLOW EMPTY STRING as a key!
template <typename ITEM_T>
class UT_ArrayStringMap : public UT::ArrayMap<UT_StringHolder, ITEM_T>
{
public:
    typedef UT::ArrayMap<UT_StringHolder, ITEM_T> Parent;

    typedef typename Parent::const_iterator const_iterator;
    typedef typename Parent::iterator iterator;
    typedef typename Parent::size_type size_type;

    // Inherit parent class constructors
    using Parent::Parent;

    // Expose the erase() overloads that take iterators.
    // The erase(const UT_StringHolder&) overload is hidden in favour of
    // erase(const UT_StringRef &) from UT_STRINGREF_WRAPPER.
    iterator erase(iterator pos) { return Parent::erase(pos); }
    iterator erase(iterator first, iterator last)
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

    using Parent::insert;
    std::pair<iterator, bool> insert(const UT_StringRef &key, const ITEM_T &val)
    {
        auto&& iter = find(key);
        if (iter == Parent::end())
	    return insert(UT_StringHolder(key), val);
	return std::make_pair(iter, false);
    }
    std::pair<iterator, bool> insert(const UT_StringRef &key, ITEM_T &&val)
    {
        auto&& iter = find(key);
        if (iter == Parent::end())
	    return insert(UT_StringHolder(key), std::move(val));
	return std::make_pair(iter, false);
    }
    std::pair<iterator, bool> insert(UT_StringRef &&key, const ITEM_T &val)
    {
        auto&& iter = find(key);
        if (iter == Parent::end())
	    return insert(UT_StringHolder(std::move(key)), val);
	return std::make_pair(iter, false);
    }
    std::pair<iterator, bool> insert(UT_StringRef &&key, ITEM_T &&val)
    {
        auto&& iter = find(key);
        if (iter == Parent::end())
	    return insert(UT_StringHolder(std::move(key)), std::move(val));
	return std::make_pair(iter, false);
    }
    std::pair<iterator, bool> insert(const char *key, const ITEM_T &val)
    {
	return insert(UT_StringRef(key), val);
    }
    std::pair<iterator, bool> insert(const char *key, ITEM_T &&val)
    {
        return insert(UT_StringRef(key), std::move(val));
    }

};

namespace UT {
template<typename ITEM_T>
struct DefaultClearer<UT_ArrayStringMap<ITEM_T> >
    : public DefaultClearer<typename UT_ArrayStringMap<ITEM_T>::Parent>
{};
}

#undef UT_STRINGREF_WRAPPER
#undef UT_STRINGREF_WRAPPER_RANGE

#endif
