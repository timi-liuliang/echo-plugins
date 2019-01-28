/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_ArrayStringSet.h
 *
 * COMMENTS:    An array-based hash set for UT_StringHolder.
 */

#pragma once

#ifndef __UT_ArrayStringSet_h__
#define __UT_ArrayStringSet_h__

#include "UT_ArraySet.h"
#include "UT_StringHolder.h"

/// We want methods like find() to take a const UT_StringRef& instead of a
/// const UT_StringHolder& for the following reasons:
/// - This allows a const char* to be passed in without forcing a copy of the string.
/// - A UT_StringRef can be used without converting it to a UT_StringHolder
/// (which hardens the string if necessary).
/// - A UT_StringHolder can still be directly passed in.
#define UT_STRINGREF_WRAPPER(return_type, name, qualifier) \
    inline return_type name(const UT_StringRef &key) qualifier \
    { \
        return Parent::name(UTmakeUnsafeRef(key)); \
    }
/// Specialization of the above macro for methods that return an iterator
/// range, since something like std::pair<iterator, iterator> is interpreted as
/// two arguments when being passed to a macro (due to the comma).
#define UT_STRINGREF_WRAPPER_RANGE(iterator_type, name, qualifier) \
    inline std::pair<iterator_type, iterator_type> \
    name(const UT_StringRef &key) qualifier \
    { \
        return Parent::name(UTmakeUnsafeRef(key)); \
    }

/// UT_ArrayStringSet is a simple specialization of a UT::ArraySet that has
/// UT_StringHolder as its key type which allows C strings to be used.
/// If you know that a string will not be destroyed during the set's
/// lifetime, UTmakeUnsafeRef can be used to insert a shallow reference.
/// WARNING: This class DOES NOT ALLOW EMPTY STRING as a key!
class UT_ArrayStringSet : public UT::ArraySet<UT_StringHolder>
{
public:
    typedef UT::ArraySet<UT_StringHolder> Parent;

    typedef Parent::const_iterator const_iterator;
    typedef Parent::iterator iterator;
    typedef Parent::size_type size_type;

    // Inherit parent class constructors
    using Parent::Parent;

    iterator erase(iterator pos) { return Parent::erase(pos); }
    iterator erase(iterator first, iterator last)
    {
        return Parent::erase(first, last);
    }

    UT_STRINGREF_WRAPPER(size_type, count, const)
    UT_STRINGREF_WRAPPER_RANGE(iterator, equal_range, )
    UT_STRINGREF_WRAPPER_RANGE(const_iterator, equal_range, const)
    UT_STRINGREF_WRAPPER(size_type, erase, )
    UT_STRINGREF_WRAPPER(const_iterator, find, const)

    bool contains(const UT_StringRef &ref) const
    { return count(ref) > 0; }

    bool contains(const UT_ArrayStringSet &src) const
    { for (const_iterator it = src.begin(); it != src.end(); ++it)
        if (!contains(*it))
            return false;
    return true; }

    /// Set-wise boolean operations.
    UT_ArrayStringSet &operator|=(const UT_ArrayStringSet &src)
    { for (const_iterator it = src.begin(); it != src.end(); ++it)
        insert(*it);
    return *this; }
    UT_ArrayStringSet &operator&=(const UT_ArrayStringSet &src)
    { UT_ArrayStringSet result;
    for (const_iterator it = src.begin(); it != src.end(); ++it)
        if (contains(*it))
            result.insert(*it);
    *this = std::move(result);
    return *this; }
    UT_ArrayStringSet &operator-=(const UT_ArrayStringSet &src)
    { for (const_iterator it = src.begin(); it != src.end(); ++it)
        erase(*it);
    return *this; }

    using Parent::insert;
    std::pair<iterator, bool> insert(const UT_StringRef &key)
    {
        auto&& iter = Parent::find(UTmakeUnsafeRef(key));
        if (iter == end())
	    return insert(UT_StringHolder(key));
	return std::make_pair(iter, false);
    }
    std::pair<iterator, bool> insert(UT_StringRef &&key)
    {
        auto&& iter = Parent::find(UTmakeUnsafeRef(key));
        if (iter == end())
	    return insert(UT_StringHolder(std::move(key)));
	return std::make_pair(iter, false);
    }
    std::pair<iterator, bool> insert(const char *key)
    {
	return insert(UT_StringRef(key));
    }
};

namespace UT {
template<>
struct DefaultClearer<UT_ArrayStringSet>
    : public DefaultClearer<typename UT_ArrayStringSet::Parent>
{};
}

#undef UT_STRINGREF_WRAPPER
#undef UT_STRINGREF_WRAPPER_RANGE

#endif
