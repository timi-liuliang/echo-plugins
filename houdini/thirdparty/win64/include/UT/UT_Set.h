/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Set.h (UT Library, C++)
 *
 * COMMENTS:	Wrapper for an unordered set data structure.  Prefer to use
 *		this to std::set since hboost::unordered_set is more
 *		efficient when order is not needed.
 */

#ifndef __UT_Set__
#define __UT_Set__

#include "UT_ContainerPrinter.h"

#include <set>
#include <hboost/unordered_set.hpp>

template<typename K, typename H, typename P>
int64
UTgetMemoryUsage(const hboost::unordered_set<K, H, P> &set, bool inclusive)
{
    int64 mem = inclusive ? sizeof(set) : 0;
    // Buckets only contain a pointer to the node
    mem += set.bucket_count() * sizeof(void*);
    // Nodes contain the hash value, a pointer to the next node,
    // and the key.
    mem += set.size() * (sizeof(size_t) + sizeof(void*) + sizeof(K));
    return mem;
}

template<typename K, typename C>
int64
UTgetMemoryUsage(const std::set<K, C> &set, bool inclusive)
{
    int64 mem = inclusive ? sizeof(set) : 0;

    // NOTE: If the comparator object of type C owns any memory
    //       (i.e. apart from itself) when default constructed
    //       or copy constructed, that memory won't be counted.

    // NOTE: This count is for the VC++ 2010 implementation
    //       of std::map, but others should be in the ballpark.
    // Nodes contain pointers to the left, parent, and right,
    // the key, a colour in a char (red or black),
    // and a flag in a char indicating if the node is the head,
    // (in which case the key is ignored).
    // Round up to a multiple of 4 on the size of each node.
    mem += (set.size() + 1) * ((3*sizeof(void*) + sizeof(K)
                               + 2*sizeof(char) + 3) & ~3);
    return mem;
}

template<typename K,
	 typename H = hboost::hash<K>, typename P = std::equal_to<K> >
class UT_Set : public hboost::unordered_set<K, H, P>
{
public:
    typedef hboost::unordered_set<K, H, P> Base;
    typedef H Hasher;
    typedef P Equal;

    explicit UT_Set(
            const Hasher &hf = Hasher(),
            const Equal &eql = Equal()) :
	Base(hboost::unordered::detail::default_bucket_count, hf, eql) {}

    template <typename InputIt>
    UT_Set(InputIt first, InputIt last) : Base(first, last) {}

    template <typename InputIt>
    UT_Set(InputIt first, InputIt last,
            const Hasher &hf = Hasher(),
            const Equal &eql = Equal()) :
            Base(first, last, hboost::unordered::detail::default_bucket_count,
                 hf, eql) {}

    UT_Set(UT_Set const &other) : Base(other) {}

    // Note: This can be removed when C++11 support is enabled due to existence
    // of initializer lists.
    UT_Set(const K &k,
           const Hasher &hf = Hasher(),
           const Equal &eql = Equal())
	: Base(hboost::unordered::detail::default_bucket_count, hf, eql)
    {
	this->insert(k);
    }

    /// Constructs a set from an initializer list:
    /// UT_Set<int> some_set({5,123,500});
    UT_Set(std::initializer_list<K> init,
           const Hasher &hf = Hasher(),
           const Equal &eql = Equal())
	: Base(hboost::unordered::detail::default_bucket_count, hf, eql)
    {
        for (auto &&p = init.begin(); p != init.end(); ++p) {
            this->insert(*p);
        }
    }

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += UTgetMemoryUsage(*static_cast<const Base *>(this), false);
        return mem;
    }

    /// Determines membership in a set, a bit more semantically readable
    /// than count() or find()
    bool contains(const K &key) const
    { return Base::count(key) > 0; }

    bool contains(const UT_Set<K> &src) const
    { for (const_iterator it = src.begin(); it != src.end(); ++it)
	if (!contains(*it))
	    return false;
      return true; }

    /// Set-wise boolean operations.
    UT_Set<K,H,P> &operator|=(const UT_Set<K,H,P> &src)
    { for (const_iterator it = src.begin(); it != src.end(); ++it)
	this->insert(*it);
      return *this; }
    UT_Set<K,H,P> &operator&=(const UT_Set<K,H,P> &src)
    { UT_Set<K,H,P> result;
      for (const_iterator it = src.begin(); it != src.end(); ++it)
	if (contains(*it))
	    result.insert(*it);
      *this = std::move(result);
      return *this; }
    UT_Set<K,H,P> &operator-=(const UT_Set<K,H,P> &src)
    { for (const_iterator it = src.begin(); it != src.end(); ++it)
	this->erase(*it);
      return *this; }
    
public:
    // Lowering of base types
    typedef typename Base::key_type		key_type;
    typedef typename Base::value_type		value_type;
    typedef typename Base::hasher		hasher;
    typedef typename Base::key_equal		key_equal;
    typedef typename Base::iterator		iterator;
    typedef typename Base::const_iterator	const_iterator;
};



template<typename K, typename C = std::less<K> >
class UT_SortedSet : public std::set<K, C>
{
public:
    typedef std::set<K, C> Base;
    typedef C LessThan;

    UT_SortedSet() : Base() {}

    explicit UT_SortedSet(const LessThan& lt) : Base(lt) {}

    template<typename InputIt>
    UT_SortedSet(InputIt first, InputIt last) : Base(first, last) {}

    template<typename InputIt>
    UT_SortedSet(InputIt first, InputIt last, const LessThan &lt) :
	Base(first, last, lt) {}

    UT_SortedSet(const UT_SortedSet &other) : Base(other) {}

    // Note: This can be removed when C++11 support is enabled due to existence
    // of initializer lists.
    UT_SortedSet(const K &k) : Base()
    {
	this->insert(k);
    }

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += UTgetMemoryUsage(*static_cast<const Base *>(this), false);
        return mem;
    }

    /// Determines membership in a set, a bit more semantically readable
    /// than count() or find()
    bool contains(const K &key) const
    { return this->count(key) > 0; }

    bool contains(const UT_SortedSet<K> &src) const
    { for (const_iterator it = src.begin(); it != src.end(); ++it)
	if (!contains(*it))
	    return false;
      return true; }

    /// Set-wise boolean operations.
    UT_SortedSet<K,C> &operator|=(const UT_SortedSet<K,C> &src)
    { for (const_iterator it = src.begin(); it != src.end(); ++it)
	this->insert(*it);
      return *this; }
    UT_SortedSet<K,C> &operator&=(const UT_SortedSet<K,C> &src)
    { UT_SortedSet<K,C> result;
      for (const_iterator it = src.begin(); it != src.end(); ++it)
	if (contains(*it))
	    result.insert(*it);
      *this = std::move(result);
      return *this; }
    UT_SortedSet<K,C> &operator-=(const UT_SortedSet<K,C> &src)
    { for (const_iterator it = src.begin(); it != src.end(); ++it)
	this->erase(*it);
      return *this; }
    
public:
    // Lowering of base types.
    typedef typename Base::key_type		key_type;
    typedef typename Base::value_type		value_type;
    typedef typename Base::key_compare		key_compare;
    typedef typename Base::iterator		iterator;
    typedef typename Base::const_iterator	const_iterator;
};

template<typename OS, typename S>
inline OS &
operator<<(OS &os, const UT_Set<S> &d)
{
    os << "UT_Set" << UT_ContainerPrinter<UT_Set<S> >(d);
    return os;
}

template<typename OS, typename S>
inline OS &
operator<<(OS &os, const UT_SortedSet<S> &d)
{
    os << "UT_SortedSet" << UT_ContainerPrinter<UT_SortedSet<S> >(d);
    return os;
}


#endif
