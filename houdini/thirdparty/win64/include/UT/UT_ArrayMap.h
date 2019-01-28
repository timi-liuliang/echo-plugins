/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_ArrayMap.h (UT Library, C++)
 *
 * COMMENTS:    Flat-array-based hash map implementation.
 */

#pragma once

#ifndef __UT_ArrayMap__
#define __UT_ArrayMap__

#include "UT_Array.h"
#include "UT_ArraySet.h"
#include "UT_IteratorRange.h"
#include <iterator>
#include <stdexcept>
#include <utility>

namespace UT {

template<class KeyEqual,typename Key,typename T>
struct MapKeyEqual
{
    bool operator()(const std::pair<Key,T> &a,const std::pair<Key,T> &b) const
    {
        return KeyEqual()(a.first, b.first);
    }
};

template<typename Hash,typename Key,typename T>
struct MapKeyHash
{
    size_t operator()(const std::pair<Key,T> &a) const
    {
        return Hash()(a.first);
    }
};

template<typename S0,typename S1>
struct MapKeyClearer : public DefaultClearer<std::pair<S0,S1> >
{
    using Base = DefaultClearer<std::pair<S0,S1> >;
    using Base::clear;
    /// Only need to actually check the key,
    /// even though clear and clearConstruct will clear both.
    static bool isClear(const std::pair<S0,S1> &v)
    {
        return DefaultClearer<S0>::isClear(v.first);
    }
    /// An overload for when there's only a key.
    static bool isClear(const S0 &v)
    {
        return DefaultClearer<S0>::isClear(v);
    }
    using Base::clearConstruct;
    using Base::clearNeedsDestruction;
};

template<
    typename Key,
    typename T,
    bool MULTI=false,
    std::size_t MAX_LOAD_FACTOR_256=128,
    typename Clearer=MapKeyClearer<Key,T>,
    class Hash=std::hash<Key>,
    class KeyEqual=std::equal_to<Key>
>
class ArrayMap : public ArraySet<std::pair<Key,T>,MULTI,MAX_LOAD_FACTOR_256,Clearer,MapKeyHash<Hash,Key,T>,MapKeyEqual<KeyEqual,Key,T> >
{
public:
    typedef ArraySet<std::pair<Key,T>,MULTI,MAX_LOAD_FACTOR_256,Clearer,MapKeyHash<Hash,Key,T>,MapKeyEqual<KeyEqual,Key,T> > set_type;
    typedef ArrayMap<Key,T,MULTI,MAX_LOAD_FACTOR_256,Clearer,Hash,KeyEqual> map_type;
    typedef Key key_type;
    typedef T mapped_type;
    typedef Hash hasher;
    typedef KeyEqual key_equal;

    /// GCC and Clang can't find base class members in templated code, so we
    /// need to declare explicitly that we're inheriting them.
    using pointer = typename set_type::pointer;
    using const_pointer = typename set_type::const_pointer;
    using value_type = typename set_type::value_type;
    using size_type = typename set_type::size_type;

    /// Inherit iterator and const_iterator
    using iterator = typename set_type::iterator;
    template<bool constant_type>
    using iterator_t = typename set_type::template iterator_t<constant_type>;
    using const_iterator = typename set_type::const_iterator;

    /// Inherit the constructors from ArraySet
    using set_type::set_type;

    /// Inherit size from ArraySet.
    using set_type::size;

    /// Inherit insert from ArraySet, in addition to signatures below.
    using set_type::insert;

    std::pair<iterator, bool> insert(const Key &key, const T &val)
    {
        return insert(std::pair<Key,T>(key, val));
    }
    std::pair<iterator, bool> insert(Key &&key, T &&val)
    {
        return insert(std::make_pair(key, val));
    }

    bool operator==(const map_type &that) const
    {
        if (size() != that.size())
            return false;

        // Check if one has no allocation (both empty)
        if (!myBuckets || !that.myBuckets)
            return true;

        const const_pointer pend = myBuckets + myNumBuckets;
        for (const_pointer p = myBuckets; p != pend; ++p)
        {
            if (Clearer::isClear(*p))
                continue;
            // NOTE: Don't just use that.contains(*p), since it only checks the key!
            const_iterator it = that.find(p->first);
            if (it.atEnd())
                return false;
            if (*p != *it)
                return false;
        }
        return true;
    }
    bool operator!=(const map_type &that) const
    {
        return !(*this == that);
    }

    /// Returns an iterator to the first item matching key,
    /// or an end iterator if no items match key.
    /// @{
    iterator find(const Key &key)
    {
        UT_ASSERT_MSG_P(!Clearer::isClear(key),"Don't query a clear key!");

        const pointer pstart = myBuckets;
        if (!pstart)
            return iterator(nullptr,nullptr,false);

        const pointer pend = pstart + myNumBuckets;

        const pointer search_start = searchStart(key);
        for (pointer p = search_start; p != pend; ++p)
        {
            if (key_equal()(key,p->first))
                return iterator(p,pend,false);
            if (Clearer::isClear(*p))
                return iterator(pend,pend,false);
        }
        // Hit the end, so search back from before the start of the block.
        for (pointer p = search_start-1; p >= pstart; --p)
        {
            if (key_equal()(key,p->first))
                return iterator(p,pend,false);
            if (Clearer::isClear(*p))
                return iterator(pend,pend,false);
        }
        return iterator(pend,pend,false);
    }
    const_iterator find(const Key &key) const
    {
        UT_ASSERT_MSG_P(!Clearer::isClear(key),"Don't query a clear key!");

        const const_pointer pstart = myBuckets;
        if (!pstart)
            return const_iterator(nullptr,nullptr,false);

        const const_pointer pend = pstart + myNumBuckets;

        const_pointer search_start = searchStart(key);
        for (const_pointer p = search_start; p != pend; ++p)
        {
            if (key_equal()(key,p->first))
                return const_iterator(p,pend,false);
            if (Clearer::isClear(*p))
                return const_iterator(pend,pend,false);
        }
        // Hit the end, so search back from before the start of the block.
        for (const_pointer p = search_start-1; p >= pstart; --p)
        {
            if (key_equal()(key,p->first))
                return const_iterator(p,pend,false);
            if (Clearer::isClear(*p))
                return const_iterator(pend,pend,false);
        }
        return const_iterator(pend,pend,false);
    }
    /// @}

    /// Returns a reference to the value of the first item matching key.
    /// WARNING: This throws an exception if nothing matches key!
    /// @{
    T &at(const Key &key)
    {
        UT_ASSERT_MSG_P(!Clearer::isClear(key),"Don't query a clear key!");

        if (!myBuckets)
        {
            UT_ASSERT_MSG(0,"WARNING! An exception is about to be thrown in UT::ArrayMap::at!");
            throw std::out_of_range("Key not found in UT::ArrayMap::at(const Key &)");
        }

        const pointer pstart = myBuckets;
        const const_pointer pend = pstart + myNumBuckets;
        const pointer search_start = searchStart(key);
        for (pointer p = search_start; p != pend; ++p)
        {
            if (key_equal()(key,p->first))
                return p->second;
            if (Clearer::isClear(*p))
            {
                UT_ASSERT_MSG(0,"WARNING! An exception is about to be thrown in UT::ArrayMap::at!");
                throw std::out_of_range("Key not found in UT::ArrayMap::at(const Key &)");
            }
        }
        // Hit the end, so search back from before the start of the block.
        for (pointer p = search_start-1; p >= pstart; --p)
        {
            if (key_equal()(key,p->first))
                return p->second;
            if (Clearer::isClear(*p))
            {
                UT_ASSERT_MSG(0,"WARNING! An exception is about to be thrown in UT::ArrayMap::at!");
                throw std::out_of_range("Key not found in UT::ArrayMap::at(const Key &)");
            }
        }
        UT_ASSERT_MSG(0,"WARNING! An exception is about to be thrown in UT::ArrayMap::at!");
        throw std::out_of_range("Key not found in UT::ArrayMap::at(const Key &)");
    }
    const T &at(const Key &key) const
    {
        if (!myBuckets)
        {
            UT_ASSERT_MSG(0,"WARNING! An exception is about to be thrown in UT::ArrayMap!");
            throw std::out_of_range("Key not found in UT::ArrayMap::at(const Key &) const");
        }

        const const_pointer pstart = myBuckets;
        const const_pointer pend = pstart + myNumBuckets;
        const_pointer search_start = searchStart(key);
        for (const_pointer p = search_start; p != pend; ++p)
        {
            if (key_equal()(key,p->first))
                return p->second;
            if (Clearer::isClear(*p))
            {
                UT_ASSERT_MSG(0,"WARNING! An exception is about to be thrown in UT::ArrayMap!");
                throw std::out_of_range("Key not found in UT::ArrayMap::at(const Key &) const");
            }
        }
        // Hit the end, so search back from before the start of the block.
        for (const_pointer p = search_start-1; p >= pstart; --p)
        {
            if (key_equal()(key,p->first))
                return p->second;
            if (Clearer::isClear(*p))
            {
                UT_ASSERT_MSG(0,"WARNING! An exception is about to be thrown in UT::ArrayMap!");
                throw std::out_of_range("Key not found in UT::ArrayMap::at(const Key &) const");
            }
        }
        UT_ASSERT_MSG(0,"WARNING! An exception is about to be thrown in UT::ArrayMap!");
        throw std::out_of_range("Key not found in UT::ArrayMap::at(const Key &) const");
    }
    /// @}

    /// Returns the number of entries matching key.
    /// If MULTI is false, this will only return either 0 or 1.
    size_type count(const Key &key) const
    {
        UT_ASSERT_MSG_P(!Clearer::isClear(key),"Don't query a clear key!");

        if (!myBuckets)
            return 0;

        const const_pointer pstart = myBuckets;
        const const_pointer pend = pstart + myNumBuckets;
        const_pointer search_start = searchStart(key);
        size_type count = 0;
        for (const_pointer p = search_start; p != pend; ++p)
        {
            if (key_equal()(key,p->first))
            {
                if (!MULTI)
                    return 1;
                ++count;
            }
            if (Clearer::isClear(*p))
                return MULTI ? count : 0;
        }
        // Hit the end, so search back from before the start of the block.
        for (const_pointer p = search_start-1; p >= pstart; --p)
        {
            if (key_equal()(key,p->first))
            {
                if (!MULTI)
                    return 1;
                ++count;
            }
            if (Clearer::isClear(*p))
                return MULTI ? count : 0;
        }
        return MULTI ? count : 0;
    }

    /// Returns true iff the set contains the given key.
    /// This should be faster than count() if MULTI is true.
    bool contains(const Key &key) const
    {
        UT_ASSERT_MSG_P(!Clearer::isClear(key),"Don't query a clear key!");

        if (!myBuckets)
            return false;

        const const_pointer pstart = myBuckets;
        const const_pointer pend = pstart + myNumBuckets;
        const_pointer search_start = searchStart(key);
        for (const_pointer p = search_start; p != pend; ++p)
        {
            if (key_equal()(key,p->first))
                return true;
            if (Clearer::isClear(*p))
                return false;
        }
        // Hit the end, so search back from before the start of the block.
        for (const_pointer p = search_start-1; p >= pstart; --p)
        {
            if (key_equal()(key,p->first))
                return true;
            if (Clearer::isClear(*p))
                return false;
        }
        return false;
    }

    /// Returns a reference to the first value that is mapped-to from a key
    /// matching key, inserting if none exist.
    /// NOTE: If you use this, key cannot match the key of a pair
    ///       cleared by Clearer, else insertHelper will assert.
    T &operator[](const Key &key)
    {
        UT_ASSERT_MSG_P(!Clearer::isClear(key),"Don't query a clear key!");

        iterator it = find(key);
        if (!it.atEnd())
            return it->second;

        // NOTE: Must use value initialization (not to be confused with default
        //       constructor!) for second, so that, e.g. ints will be
        //       initialized to zero and pointers will be nullptr.
        value_type value(key, mapped_type());

        pointer dest;
        set_type::template insertHelper<false,true>(myBuckets,myNumBuckets,value,dest);
        *dest = std::move(value);
        ++*((size_type*)(myBuckets+myNumBuckets));
        return dest->second;
    }

    /// Returns a reference to the first value that is mapped-to from a key
    /// matching key, inserting if none exist.
    /// NOTE: If you use this, key cannot match the key of a pair
    ///       cleared by Clearer, else insertHelper will assert.
    T &operator[](Key &&key)
    {
        UT_ASSERT_MSG_P(!Clearer::isClear(key),"Don't query a clear key!");

        iterator it = find(key);
        if (!it.atEnd())
            return it->second;

        // NOTE: Must use value initialization (not to be confused with default
        //       constructor!) for second, so that, e.g. ints will be
        //       initialized to zero and pointers will be nullptr.
        value_type value(std::move(key), mapped_type());

        pointer dest;
        set_type::template insertHelper<false,true>(myBuckets,myNumBuckets,value,dest);
        *dest = std::move(value);
        ++*((size_type*)(myBuckets+myNumBuckets));
        return dest->second;
    }

    /// Returns a pair of iterators representing the range of values matching
    /// key, as [first,second).
    /// @{
    std::pair<const_iterator,const_iterator> equal_range(const Key &key) const
    {
        UT_ASSERT_MSG_P(!Clearer::isClear(key),"Don't query a clear key!");

        const const_pointer pstart = myBuckets;
        const const_pointer pend = pstart + myNumBuckets;
        if (!pstart)
            return std::make_pair(const_iterator(nullptr,nullptr,false), const_iterator(nullptr,nullptr,false));

        const const_pointer search_start = searchStart(key);
        if (!MULTI)
        {
            for (const_pointer p = search_start; p != pend; ++p)
            {
                if (key_equal()(key,p->first))
                    return std::make_pair(const_iterator(p,pend,false), const_iterator(p+1,pend,true));
                if (Clearer::isClear(*p))
                    return std::make_pair(const_iterator(pend,pend,false), const_iterator(pend,pend,false));
            }
            // Hit the end, so search back from before the start of the block.
            for (const_pointer p = search_start-1; p >= pstart; --p)
            {
                if (key_equal()(key,p->first))
                    return std::make_pair(const_iterator(p,pend,false), const_iterator(p+1,pend,true));
                if (Clearer::isClear(*p))
                    return std::make_pair(const_iterator(pend,pend,false), const_iterator(pend,pend,false));
            }
            return std::make_pair(const_iterator(pend,pend,false), const_iterator(pend,pend,false));
        }

        // MULTI is true, so we might have multiple matches
        const_pointer first_found = nullptr;
        const_pointer end_found = pend;
        for (const_pointer p = search_start; p != pend; ++p)
        {
            if (key_equal()(key,p->first))
            {
                if (!first_found)
                    first_found = p;
            }
            else if (first_found)
            {
                if (first_found != search_start)
                    return std::make_pair(const_iterator(first_found,pend,false), const_iterator(p,pend,true));

                end_found = p;
                break;
            }
            else if (Clearer::isClear(*p))
            {
                // NOTE: first_found must be false
                return std::make_pair(const_iterator(pend,pend,false), const_iterator(pend,pend,false));
            }
        }

        // Hit the end, so search back from before the start of the block.
        for (const_pointer p = search_start-1; p >= pstart; --p)
        {
            if (!key_equal()(key,p->first))
            {
                return std::make_pair(const_iterator(p+1,pend,false), const_iterator(end_found,pend,true));
            }
        }
        return std::make_pair(const_iterator(pstart,pend,false), const_iterator(end_found,pend,true));
    }
    std::pair<iterator,iterator> equal_range(const Key &key)
    {
        UT_ASSERT_MSG_P(!Clearer::isClear(key),"Don't query a clear key!");

        const pointer pstart = myBuckets;
        const pointer pend = pstart + myNumBuckets;
        if (!pstart)
            return std::make_pair(iterator(nullptr,nullptr,false), iterator(nullptr,nullptr,false));

        const pointer search_start = searchStart(key);
        if (!MULTI)
        {
            for (pointer p = search_start; p != pend; ++p)
            {
                if (key_equal()(key,p->first))
                    return std::make_pair(iterator(p,pend,false), iterator(p+1,pend,true));
                if (Clearer::isClear(*p))
                    return std::make_pair(iterator(pend,pend,false), iterator(pend,pend,false));
            }
            // Hit the end, so search back from before the start of the block.
            for (pointer p = search_start-1; p >= pstart; --p)
            {
                if (key_equal()(key,p->first))
                    return std::make_pair(iterator(p,pend,false), iterator(p+1,pend,true));
                if (Clearer::isClear(*p))
                    return std::make_pair(iterator(pend,pend,false), iterator(pend,pend,false));
            }
            return std::make_pair(iterator(pend,pend,false), iterator(pend,pend,false));
        }

        // MULTI is true, so we might have multiple matches
        pointer first_found = nullptr;
        pointer end_found = pend;
        for (pointer p = search_start; p != pend; ++p)
        {
            if (key_equal()(key,p->first))
            {
                if (!first_found)
                    first_found = p;
            }
            else if (first_found)
            {
                if (first_found != search_start)
                    return std::make_pair(iterator(first_found,pend,false), iterator(p,pend,true));

                end_found = p;
                break;
            }
            else if (Clearer::isClear(*p))
            {
                // NOTE: first_found must be false
                return std::make_pair(iterator(pend,pend,false), iterator(pend,pend,false));
            }
        }

        // Hit the end, so search back from before the start of the block.
        for (pointer p = search_start-1; p >= pstart; --p)
        {
            if (!key_equal()(key,p->first))
            {
                return std::make_pair(iterator(p+1,pend,false), iterator(end_found,pend,true));
            }
        }
        return std::make_pair(iterator(pstart,pend,false), iterator(end_found,pend,true));
    }
    /// @}

    /// Inherit erase from ArraySet, in addition to signatures below.
    using set_type::erase;

    /// Removes all items matching key and returns
    /// the number of items removed.
    size_type erase(const Key &key)
    {
        UT_ASSERT_MSG_P(!Clearer::isClear(key),"Don't query a clear key!");

        if (!MULTI)
        {
            iterator it = find(key);
            if (it.atEnd())
                return 0;
            erase(it);
            return 1;
        }

        std::pair<iterator,iterator> its = equal_range(key);
        size_type count = std::distance(its.first, its.second);
        erase(its.first, its.second);
        return count;
    }

    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachKey(FUNCTOR &&functor) const
    {
        const_pointer p = myBuckets;
        const const_pointer pend = p + myNumBuckets;
        for (; p != pend; ++p)
        {
            if (!Clearer::isClear(*p))
                functor(p->first);
        }
    }

    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachValue(FUNCTOR &&functor) const
    {
        const_pointer p = myBuckets;
        const const_pointer pend = p + myNumBuckets;
        for (; p != pend; ++p)
        {
            if (!Clearer::isClear(*p))
                functor(p->second);
        }
    }

    template<bool constant_type>
    class ordered_iterator_t
    {
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;
        typedef typename std::conditional<constant_type, const typename ArrayMap::value_type, typename ArrayMap::value_type>::type value_type;
        typedef typename std::conditional<constant_type, const value_type, value_type>::type& reference;
        typedef typename std::conditional<constant_type, typename set_type::const_pointer, typename set_type::pointer>::type pointer;
        typedef typename std::conditional<constant_type, const typename ArrayMap::map_type, typename ArrayMap::map_type>::type map_type;

        template<typename COMPARATOR>
        ordered_iterator_t(map_type &map, const COMPARATOR &comparator)
            : myMap(map)
            , myI(0)
        {
            myKeys.setCapacity(map.size());
            map.forEachKey([this](const Key &key) {
                myKeys.append(key);
            });
            myKeys.stdsort(comparator);
        }
        ordered_iterator_t(ordered_iterator_t<constant_type> &&that)
            : myMap(that.myMap)
            , myKeys(std::move(that.myKeys))
            , myI(that.myI)
        {}
        reference operator*() const
        {
            return *myMap.find(myKeys[myI]);
        }
        pointer operator->() const
        {
            return &*myMap.find(myKeys[myI]);
        }
        operator pointer() const
        {
            return &*myMap.find(myKeys[myI]);
        }
        void operator++()
        {
            ++myI;
        }
        bool atEnd() const
        {
            return myI >= myKeys.size();
        }
    private:
        /// Use atEnd(), not ==
        bool operator==(const ordered_iterator_t<constant_type> &) const = delete;
        bool operator!=(const ordered_iterator_t<constant_type> &) const = delete;

        map_type &myMap;
        UT_Array<Key> myKeys;
        exint myI;
    };

    template<typename COMPARATOR>
    ordered_iterator_t<true> ordered_begin(const COMPARATOR &comparator) const
    {
        return ordered_iterator_t<true>(*this, comparator);
    }

    template<typename COMPARATOR>
    ordered_iterator_t<true> ordered_cbegin(const COMPARATOR &comparator) const
    {
        return ordered_iterator_t<true>(*this, comparator);
    }

    template<typename COMPARATOR>
    ordered_iterator_t<false> ordered_begin(const COMPARATOR &comparator)
    {
        return ordered_iterator_t<false>(*this, comparator);
    }

protected:
    template<typename VIT, typename VT>
    struct deref_pair_first
    {
        VT &operator()(const VIT &v) const { return v->first; }
    };
    template<typename VIT, typename VT>
    struct deref_pair_second
    {
        VT &operator()(const VIT &v) const { return v->second; }
    };

    template<typename IT, typename VT, typename DR>
    class partial_iterator_base :
        public std::iterator<std::forward_iterator_tag, VT>
    {
    public:
        typedef VT& reference;
        typedef VT* pointer;

        partial_iterator_base() : it() {}

        template<typename EIT, typename EDR>
        partial_iterator_base(const partial_iterator_base<EIT, VT, EDR> &src) :
            it(src.it) {}

        reference operator*() const { DR dr; return dr(it); }
        pointer operator->() const { DR dr; return &dr(it); }

        bool operator==(const partial_iterator_base<IT, VT, DR> &o) const
        { return it == o.it; }

        bool operator!=(const partial_iterator_base<IT, VT, DR> &o) const
        { return it != o.it; }

        partial_iterator_base &operator++()
        {
            ++it;
            return *this;
        }

        partial_iterator_base &operator=(const partial_iterator_base<IT, VT, DR> &src)
        { 
            it = src.it;
            return *this;
        }
    protected:
        friend class ArrayMap;

        partial_iterator_base(IT it) : it(it) {}
    private:
        IT it;
    };

public:
    using const_key_iterator = partial_iterator_base<const_iterator, const key_type, 
        deref_pair_first<const_iterator, const key_type>>;
    using mapped_iterator = partial_iterator_base<iterator, mapped_type,
        deref_pair_second<iterator, mapped_type>>;
    using const_mapped_iterator = partial_iterator_base<const_iterator, const mapped_type, 
        deref_pair_second<const_iterator, const mapped_type>>;

    /// Returns a const range object that iterates over the map but returns
    /// only the key values.
    /// Example:
    /// @code
    /// UT::ArrayMap<int, const char *> foo = {{1, "one"}, {2, "two"}};
    /// for (int key : foo.key_range())
    ///    std::cout << key << "\n";
    /// @endcode
    UT_IteratorRange<const_key_iterator> key_range() const
    { return UTmakeRange(const_key_iterator(this->begin()),
        const_key_iterator(this->end())); }

    /// Returns a range object that iterates over the map but returns only
    /// the mapped values.
    UT_IteratorRange<mapped_iterator> mapped_range()
    { return UTmakeRange(mapped_iterator(this->begin()),
        mapped_iterator(this->end())); }

    /// Returns a const range object that iterates over the map but returns
    /// only the mapped values.
    UT_IteratorRange<const_mapped_iterator> mapped_range() const
    { return UTmakeRange(const_mapped_iterator(this->begin()),
        const_mapped_iterator(this->end())); }

private:
    /// GCC and Clang can't find base class members in templated code, so we
    /// need to declare explicitly that we're inheriting them.
    using set_type::myBuckets;
    using set_type::myNumBuckets;

    pointer searchStart(const Key &key)
    {
        const size_type hash = hasher()(key);
        return myBuckets + (hash%myNumBuckets);
    }
    const_pointer searchStart(const Key &key) const
    {
        const size_type hash = hasher()(key);
        return myBuckets + (hash%myNumBuckets);
    }
};

template<
    typename Key,
    typename T,
    bool MULTI,
    std::size_t MAX_LOAD_FACTOR_256,
    typename Clearer,
    class Hash,
    class KeyEqual
>
struct DefaultClearer<ArrayMap<Key,T,MULTI,MAX_LOAD_FACTOR_256,Clearer,Hash,KeyEqual> >
    : public DefaultClearer<typename ArrayMap<Key,T,MULTI,MAX_LOAD_FACTOR_256,Clearer,Hash,KeyEqual>::set_type>
{};

} // namespace UT

template<typename Key,
    typename T,
    bool MULTI = false,
    int MAX_LOAD_FACTOR_256 = 128,
    typename Clearer = UT::MapKeyClearer<Key,T>,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key> >
using UT_ArrayMap = UT::ArrayMap<Key,T,MULTI,MAX_LOAD_FACTOR_256,Clearer,Hash,KeyEqual>;

#endif
