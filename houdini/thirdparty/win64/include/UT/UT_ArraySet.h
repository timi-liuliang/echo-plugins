/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_ArraySet.h (UT Library, C++)
 *
 * COMMENTS:    Flat-array-based hash set implementation.
 */

#pragma once

#ifndef __UT_ArraySet__
#define __UT_ArraySet__

#include "UT_ArrayHelp.h"
#include "UT_Assert.h"
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_StaticAssert.h>
#include <SYS/SYS_Types.h>
#include <functional>
#include <limits>
#include <stdint.h>
#include <utility>

namespace UT {

template<typename T>
struct DefaultClearer;

template<typename S>
struct DefaultClearer<S*>
{
    static void clear(S*&v) { v = nullptr; }
    static bool isClear(S *v) { return v == nullptr; }
    static void clearConstruct(S**p) { clear(*p); }

    static const bool clearNeedsDestruction = false;
};

template<typename T>
struct NumericClearer
{
    static void clear(T &v)
    {
        if (std::numeric_limits<T>::has_quiet_NaN) {
            // Use QNaN as unused floating-point marker.
            // NOTE: All NaNs get treated as unused, even if they're not
            //       the representative QNaN.
            v = std::numeric_limits<T>::quiet_NaN();
        }
        else if (std::numeric_limits<T>::is_signed) {
            // Use min as unused signed integer marker.
            v = std::numeric_limits<T>::min();
        }
        else {
            // Use max as unused unsigned integer marker.
            v = std::numeric_limits<T>::max();
        }
    }
    static bool isClear(T v)
    {
        if (std::numeric_limits<T>::has_quiet_NaN) {
            // NOTE: All NaNs get treated as unused, even if they're not
            //       the representative QNaN.
            return SYSisNan(v);
        }
        if (std::numeric_limits<T>::is_signed) {
            return (v == std::numeric_limits<T>::min());
        }
        return (v == std::numeric_limits<T>::max());
    }
    static void clearConstruct(T *p) { clear(*p); }

    static const bool clearNeedsDestruction = false;
};

template<> struct DefaultClearer< int8_t > : public NumericClearer< int8_t > {};
template<> struct DefaultClearer<uint8_t > : public NumericClearer<uint8_t > {};
template<> struct DefaultClearer< int16_t> : public NumericClearer< int16_t> {};
template<> struct DefaultClearer<uint16_t> : public NumericClearer<uint16_t> {};
template<> struct DefaultClearer< int32_t> : public NumericClearer< int32_t> {};
template<> struct DefaultClearer<uint32_t> : public NumericClearer<uint32_t> {};
template<> struct DefaultClearer< int64_t> : public NumericClearer< int64_t> {};
template<> struct DefaultClearer<uint64_t> : public NumericClearer<uint64_t> {};
template<> struct DefaultClearer<float> : public NumericClearer<float> {};
template<> struct DefaultClearer<double> : public NumericClearer<double> {};
template<> struct DefaultClearer<long double> : public NumericClearer<long double> {};

/// NOTE: It may seem silly to have bool here, but things like
/// std::pair<T,bool> should work automatically, so they need some
/// default behaviour for bool.
template<>
struct DefaultClearer<bool>
{
    static void clear(bool&v) { v = false; }
    static bool isClear(bool v) { return !v; }
    static void clearConstruct(bool *p) { clear(*p); }

    static const bool clearNeedsDestruction = false;
};

template<typename S0,typename S1>
struct DefaultClearer<std::pair<S0,S1> >
{
    static void clear(std::pair<S0,S1> &v)
    {
        DefaultClearer<S0>::clear(v.first);
        DefaultClearer<S1>::clear(v.second);
    }
    static bool isClear(const std::pair<S0,S1> &v)
    {
        return DefaultClearer<S0>::isClear(v.first) && DefaultClearer<S1>::isClear(v.second);
    }
    static void clearConstruct(std::pair<S0,S1>*p) {
        DefaultClearer<S0>::clearConstruct(&p->first);
        DefaultClearer<S1>::clearConstruct(&p->second);
    }
    static const bool clearNeedsDestruction = DefaultClearer<S0>::clearNeedsDestruction || DefaultClearer<S1>::clearNeedsDestruction;
};

/// This is close to a drop-in replacement for std::unordered_set,
/// except that it uses a single array of items and empty spaces
/// marked with a dedicated "clear" value.
/// It also has a fixed maximum load factor, and doesn't store a
/// hasher, comparator, or allocator object as member data,
/// avoiding unnecessary overhead, but these differences introduce
/// some interface incompatibilities.
///
/// The incompatibility that will probably be encountered most often
/// is the lack of time guarantees.  This structure is more efficient in
/// many common cases, due to better memory coherency and fewer
/// memory allocations, but cannot guarantee as good time scaling in
/// worst or even average case as std::unordered_set guarantees for most
/// functions.
///
/// Because there is only space for one item in each bucket, if a collision
/// is hit on insertion, the structure will scan forward until a "clear"
/// bucket is found.  However, items in a contiguous block will always
/// be sorted in the order of their ideal bucket numbers.
/// If the end of the array is reached without finding a clear bucket,
/// instead of the obvious approach of wrapping to the beginning of the
/// array, the block will be shifted backward, so some items in this
/// block at the end may be earlier than their ideal bucket number.
/// This is about as complicated as wrapping, or not sorting, but avoids
/// an awkward issue when erasing while iterating where an item might get
/// visited multiple times due to the wrapping, or items might be missed
/// due to the lack of order.  Even still, unlike
/// std::unordered_set, erase may invalidate other iterators and references.
/// Also, insert may invalidate other iterators and references.
template<
    typename Key,
    bool MULTI=false,
    std::size_t MAX_LOAD_FACTOR_256=128,
    typename Clearer=DefaultClearer<Key>,
    class Hash=std::hash<Key>,
    class KeyEqual=std::equal_to<Key>
>
class ArraySet
{
public:
    typedef ArraySet<Key,MULTI,MAX_LOAD_FACTOR_256,Clearer,Hash,KeyEqual> set_type;
    typedef Key key_type;
    typedef Key value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Hash hasher;
    typedef KeyEqual key_equal;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef Clearer clearer_type;

    ArraySet()
        : myBuckets(nullptr)
        , myNumBuckets(0)
    {
        UT_ASSERT_COMPILETIME(MAX_LOAD_FACTOR_256 <= 256);
    }
    explicit ArraySet(size_type init_bucket_count)
        : myBuckets(nullptr)
        , myNumBuckets(0)
    {
        if (init_bucket_count != 0)
            setNumBuckets(init_bucket_count);
    }

    /// Move constructor, destroying the source.
    ArraySet(set_type &&that)
        : myBuckets(that.myBuckets)
        , myNumBuckets(that.myNumBuckets)
    {
        that.myBuckets = nullptr;
        that.myNumBuckets = 0;
    }

    /// Copy constructor.
    /// Avoid accidental copying by making the constructor explicit.
    explicit ArraySet(const set_type &that)
        : myBuckets(nullptr)
        , myNumBuckets(0)
    {
        *this = that;
    }

    /// Inserts all of the items in the range [start_input,end_input).
    template<typename INPUT_ITERATOR>
    ArraySet(INPUT_ITERATOR start_input, INPUT_ITERATOR end_input, size_type init_bucket_count = 0)
        : myBuckets(nullptr)
        , myNumBuckets(0)
    {
        // Preallocate enough buckets, instead of waiting for
        // insert to potentially hit collisions.
        size_type ninput_items = std::distance(start_input, end_input);
        setNumBuckets(SYSmax(init_bucket_count, minBuckets(ninput_items)));

        // Now, just insert them all, (no need to realloc)
        for (; start_input != end_input; ++start_input)
        {
            pointer pdest;
            bool success = insertHelper<!MULTI,false>(myBuckets,myNumBuckets,*start_input,pdest);
            if (success)
                *pdest = *start_input;
            else
                --ninput_items;
        }
        *((size_type*)(myBuckets+myNumBuckets)) = ninput_items;
    }
    /// Constructs a set from an initializer list, with an optional
    /// bucket count, e.g.:
    /// UT::ArraySet<int> some_set({5,123,500}, 20);
    ArraySet(std::initializer_list<value_type> init, size_type init_bucket_count = 0)
        : myBuckets(nullptr)
        , myNumBuckets(0)
    {
        size_type bucket_count;
        if (init_bucket_count == 0 || init_bucket_count < init.size()) {
            bucket_count = minBuckets(init.size());
        }
        else {
            bucket_count = init_bucket_count;
        }
        setNumBuckets(bucket_count);
        for (auto &&p = init.begin(); p != init.end(); ++p) {
            insert(*p);
        }
    }

    set_type &operator=(const set_type &that)
    {
        if (this == &that)
            return *this;

        clear();
        setNumBuckets(that.myNumBuckets);

        if (!myBuckets)
            return *this;

        size_type size = that.size();
        *((size_type*)(myBuckets+myNumBuckets)) = size;

        if (size)
        {
            // Now, just copy them all, (no need to realloc)
            const_pointer srcp = that.myBuckets;
            pointer pend = myBuckets+myNumBuckets;
            for (pointer destp = myBuckets; destp != pend; ++destp, ++srcp)
            {
                *destp = *srcp;
            }
        }
        return *this;
    }

    set_type &operator=(std::initializer_list<value_type> init)
    {
        clear();
        insert(init.begin(), init.end());
        return *this;
    }

    set_type &operator=(set_type &&that)
    {
        destroy();
        myBuckets = that.myBuckets;
        myNumBuckets = that.myNumBuckets;
        that.myBuckets = nullptr;
        that.myNumBuckets = 0;
        return *this;
    }

    bool operator==(const set_type &that) const
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
            // NOTE: Don't just use that.contains(*p), in case it doesn't check the whole item contents!
            const_iterator it = that.find(*p);
            if (it.atEnd())
                return false;
            if (*p != *it)
                return false;
        }
        return true;
    }
    bool operator!=(const set_type &that) const
    {
        return !(*this == that);
    }

    /// Swaps another set with this one
    void swap(set_type &that)
    {
        pointer temp_buckets = that.myBuckets;
        size_type temp_numbuckets = that.myNumBuckets;
        that.myBuckets = myBuckets;
        that.myNumBuckets = myNumBuckets;
        myBuckets = temp_buckets;
        myNumBuckets = temp_numbuckets;
    }

    ~ArraySet()
    {
        destroy();
    }

    /// Returns true iff there are no items in the set
    bool empty() const
    {
        return !myBuckets || (*((const size_type*)(myBuckets+myNumBuckets)) == 0);
    }

    /// Returns the number of items in the set
    size_type size() const
    {
        return myBuckets ? *(const size_type*)(myBuckets+myNumBuckets) : 0;
    }

    /// This only exists because the standard requires it.
    /// The only hard limit is what memory will allow.
    static size_type max_size()
    {
        return std::numeric_limits<size_type>::max();
    }

    /// Removes all items from the set, but does not free the memory.
    /// Call destroy() to free all the memory.
    void clear()
    {
        if (!myBuckets)
            return;
        pointer p = myBuckets;
        pointer pend = p+myNumBuckets;
        if (*((const size_type*)pend) == 0)
            return;
        while (p != pend)
        {
            Clearer::clear(*p);
            ++p;
        }
        *((size_type*)pend) = 0;
    }

    /// Removes all items from the set and frees all the memory.
    void destroy()
    {
        if (!myBuckets)
            return;
        if (!std::is_trivially_destructible<value_type>::value && (Clearer::clearNeedsDestruction || !empty()))
        {
            pointer p = myBuckets;
            const_pointer pend = p+myNumBuckets;
            while (p != pend)
            {
                // I'm assuming that in some cases, it's faster to check isClear than to call the destructor,
                // even if it's only because of a better chance of inlining.
                if (Clearer::clearNeedsDestruction || !Clearer::isClear(*p))
                    p->~value_type();
                ++p;
            }
        }
        free(myBuckets);
        myBuckets = nullptr;
        myNumBuckets = 0;
    }

    /// Returns the current number of buckets.
    size_type bucket_count() const
    {
        return myNumBuckets;
    }

    /// This only exists because the standard requires it.
    /// The only hard limit is what memory will allow.
    static size_type max_bucket_count()
    {
        return std::numeric_limits<size_type>::max();
    }

    /// This only exists because the standard requires it.
    /// This function doesn't really make sense for this implementation,
    /// but it returns 1 if the bucket is occupied, and 0 if it is not.
    size_type bucket_size(size_type i) const
    {
        if (i >= myNumBuckets) {
            return 0;
        }
        if (Clearer::isClear(myBuckets[i])) {
            return 0;
        }
        return 1;
    }

    /// Returns the portion of buckets that need to be occupied before
    /// reallocation occurs.
    /// Unlike in the standard, the maximum load factor is a constant
    /// in this implementation, so max_load_factor() is a static function.
    static float max_load_factor()
    {
        return float(MAX_LOAD_FACTOR_256)/256;
    }

    /// Returns the current portion of buckets that are occupied.
    float load_factor() const
    {
        return float(size())/float(myNumBuckets);
    }

    /// Sets the number of buckets to larger of new_num_buckets and the
    /// required minimum number of buckets for size() items, unless
    /// that number is the same as the current number of buckets.
    void rehash(size_type new_num_buckets)
    {
        size_type min_buckets = minBuckets(size());
        if (new_num_buckets < min_buckets) {
            new_num_buckets = min_buckets;
        }
        if (myNumBuckets == new_num_buckets) {
            return;
        }
        setNumBuckets(min_buckets);
    }

    /// Sets the number of buckets to the required minimum number of buckets
    /// for the larger of num_items and size().
    void reserve(size_type num_items)
    {
        size_type min_buckets = minBuckets(SYSmax(size(),num_items));
        setNumBuckets(min_buckets);
    }

    /// Sets the number of buckets to new_num_buckets, regardless of the
    /// required minimum number of buckets for size() items, though
    /// you probably shouldn't be calling it with num_new_buckets < size(),
    /// except that if num_new_buclets==0, this calls destroy().
    void setNumBuckets(size_type new_num_buckets)
    {
        if (new_num_buckets == myNumBuckets)
            return;
        if (new_num_buckets == 0)
        {
            // Assume that the caller wants to delete everything
            destroy();
            return;
        }

        size_type old_size = size();
        if (new_num_buckets < old_size)
        {
            UT_ASSERT_MSG(0,"What does the caller expect to happen when setting the number of buckets lower than the size, but not to zero?\nThis class doesn't support multiple items per bucket.");
            new_num_buckets = old_size;
        }

        // NOTE: I'm assuming that the caller, in explicitly calling
        // setNumBuckets, doesn't want to be limited by MAX_LOAD_FACTOR_256,
        // so don't check against it.

        pointer new_buckets = (pointer)malloc(new_num_buckets*sizeof(value_type) + sizeof(size_type));
        pointer new_end = new_buckets + new_num_buckets;
        for (pointer p = new_buckets; p < new_end; ++p)
        {
            Clearer::clearConstruct(p);
        }
        
        if (old_size)
        {
            // Move items over and clear them from myBuckets
            pointer pend = myBuckets+myNumBuckets;
            for (pointer srcp = myBuckets; srcp < pend; ++srcp)
            {
                if (!Clearer::isClear(*srcp))
                {
                    value_type v;
                    Clearer::clear(v);
                    // Need to make sure that *p is set to clear,
                    // as opposed to whatever std::move leaves it as,
                    // so swap with clear.
                    std::swap(*srcp, v);
                    pointer destp;
                    insertHelper<false,false>(new_buckets, new_num_buckets, v, destp);
                    *destp = std::move(v);
                }
            }

            // Temporarily set the size to 0, so that destroy() won't re-destruct them.
            *((size_type*)pend) = 0;
        }
        // Don't worry; this will just destruct the current buckets
        destroy();

        myBuckets = new_buckets;
        myNumBuckets = new_num_buckets;
        *((size_type*)new_end) = old_size;
    }

    /// This increases the number of buckets, if needed, for
    /// the specified number of items
    void bumpNumBuckets(size_type new_num_items)
    {
        size_type num_buckets = minBuckets(new_num_items);
        if (num_buckets <= myNumBuckets)
            return;
        // Although UT_Array normally bumps up by about 12.5% each time,
        // and so does UTbumpAllocToPrime, to reduce significant rehashing
        // overhead, we bump to at least twice the previous size.
	num_buckets = SYSmax(num_buckets, 2*myNumBuckets);
        num_buckets = UTbumpAllocToPrime(num_buckets);
        setNumBuckets(num_buckets);
    }

    /// Set iterator class
    template<bool constant_type>
    class iterator_t
    {
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;
        typedef typename std::conditional<constant_type, const typename ArraySet::value_type, typename ArraySet::value_type>::type value_type;
        typedef typename std::conditional<constant_type, const value_type, value_type>::type& reference;
        typedef typename std::conditional<constant_type, typename set_type::const_pointer, typename set_type::pointer>::type pointer;

        iterator_t() = default;
        iterator_t(pointer current, const_pointer end, bool scan=true)
            : myCurrent(current)
            , myEnd(end)
        {
            UT_ASSERT_MSG_P(scan || current == end || !Clearer::isClear(*current), "An iterator can't point to a clear item!");
            if (scan)
            {
                while (myCurrent != myEnd && Clearer::isClear(*myCurrent))
                {
                    ++myCurrent;
                }
            }
        }
        iterator_t(const iterator_t &) = default;
        iterator_t(iterator_t &&) = default;
        iterator_t &operator=(const iterator_t &) = default;
        iterator_t &operator=(iterator_t &&) = default;
        bool operator==(const iterator_t &that) const
        {
            UT_ASSERT_MSG_P(myEnd == that.myEnd, "Probably unintentional: Comparing against iterator from different set or iterator invalidated by reallocation!");
            return (myCurrent == that.myCurrent);
        }
        bool operator!=(const iterator_t &that) const
        {
            UT_ASSERT_MSG_P(myEnd == that.myEnd, "Probably unintentional: Comparing against iterator from different set or iterator invalidated by reallocation!");
            return (myCurrent != that.myCurrent);
        }
        bool atEnd() const
        {
            return myCurrent == myEnd;
        }
        iterator_t<constant_type> &operator++()
        {
            UT_ASSERT_MSG_P(myCurrent < myEnd, "Incrementing end iterator!");
            if (myCurrent == myEnd) {
                return *this;
            }
            do {
                ++myCurrent;
            } while (myCurrent != myEnd && Clearer::isClear(*myCurrent));
            return *this;
        }

	// This is intentionally not implemented to force its non-use but we
	// keep it here just in case there's weird C++ rules on iterator
	// requirements
        iterator_t<constant_type> operator++(int);

        reference operator*() const
        {
            UT_ASSERT_MSG_P(myCurrent < myEnd, "Dereferencing end iterator!");
            return *myCurrent;
        }
        pointer operator->() const
        {
            UT_ASSERT_MSG_P(myCurrent < myEnd, "Dereferencing end iterator!");
            return myCurrent;
        }
        operator pointer() const
        {
            UT_ASSERT_MSG_P(myCurrent <= myEnd, "Dereferencing iterator past end!");
            return myCurrent;
        }
        pointer getCurrent() const
        {
            UT_ASSERT_MSG_P(myCurrent <= myEnd, "Dereferencing iterator past end!");
            return myCurrent;
        }
        const_pointer getEnd() const
        {
            return myEnd;
        }
    private:
        friend class iterator_t<!constant_type>;

        pointer myCurrent;
        const_pointer myEnd;
    };

    /// Iterator type for iterating over non-constant elements.
    typedef iterator_t<false> iterator;

    /// Iterator type for iterating over constant elements.
    /// It may be useful to convert an iterator to a const_iterator,
    /// so making this a subclass lets us add a constructor for that.
    class const_iterator : public iterator_t<true>
    {
    public:
        using iterator_t<true>::iterator_t;

        const_iterator() = default;
        const_iterator(const iterator &non_const_it)
            : iterator_t<true>(non_const_it.getCurrent(), non_const_it.getEnd(), false)
        {}
    };

    /// Returns a non-const iterator for the beginning of the set.
    iterator begin()
    {
        return iterator(myBuckets,myBuckets+myNumBuckets,true);
    }
    /// Returns a const iterator for the beginning of the set.
    const_iterator cbegin() const
    {
        return const_iterator(myBuckets,myBuckets+myNumBuckets,true);
    }
    /// Returns a const iterator for the beginning of the set.
    const_iterator begin() const
    {
        return const_iterator(myBuckets,myBuckets+myNumBuckets,true);
    }
    /// Returns a non-const end iterator for the set.
    iterator end()
    {
        pointer pend = myBuckets+myNumBuckets;
        return iterator(pend,pend,false);
    }
    /// Returns a const end iterator for the set.
    const_iterator cend() const
    {
        const_pointer pend = myBuckets+myNumBuckets;
        return const_iterator(pend,pend,false);
    }
    /// Returns a const end iterator for the set.
    const_iterator end() const
    {
        const_pointer pend = myBuckets+myNumBuckets;
        return const_iterator(pend,pend,false);
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
            if (key_equal()(key,*p))
                return iterator(p,pend,false);
            if (Clearer::isClear(*p))
                return iterator(pend,pend,false);
        }
        // Hit the end, so search back from before the start of the block.
        for (pointer p = search_start-1; p >= pstart; --p)
        {
            if (key_equal()(key,*p))
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
            if (key_equal()(key,*p))
                return const_iterator(p,pend,false);
            if (Clearer::isClear(*p))
                return const_iterator(pend,pend,false);
        }
        // Hit the end, so search back from before the start of the block.
        for (const_pointer p = search_start-1; p >= pstart; --p)
        {
            if (key_equal()(key,*p))
                return const_iterator(p,pend,false);
            if (Clearer::isClear(*p))
                return const_iterator(pend,pend,false);
        }
        return const_iterator(pend,pend,false);
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
            if (key_equal()(key,*p))
            {
                if (!MULTI)
                    return 1;
                ++count;
            }
            else if (Clearer::isClear(*p))
                return MULTI ? count : 0;
        }
        // Hit the end, so search back from before the start of the block.
        for (const_pointer p = search_start-1; p >= pstart; --p)
        {
            if (key_equal()(key,*p))
            {
                if (!MULTI)
                    return 1;
	        ++count;
            }
            else if (Clearer::isClear(*p))
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
            if (key_equal()(key,*p))
                return true;
            if (Clearer::isClear(*p))
                return false;
        }
        // Hit the end, so search back from before the start of the block.
        for (const_pointer p = search_start-1; p >= pstart; --p)
        {
            if (key_equal()(key,*p))
                return true;
            if (Clearer::isClear(*p))
                return false;
        }
        return false;
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
                if (key_equal()(key,*p))
                    return std::make_pair(const_iterator(p,pend,false), const_iterator(p+1,pend,true));
                if (Clearer::isClear(*p))
                    return std::make_pair(const_iterator(pend,pend,false), const_iterator(pend,pend,false));
            }
            // Hit the end, so search back from before the start of the block.
            for (const_pointer p = search_start-1; p >= pstart; --p)
            {
                if (key_equal()(key,*p))
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
            if (key_equal()(key,*p))
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
            if (!key_equal()(key,*p))
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
                if (key_equal()(key,*p))
                    return std::make_pair(iterator(p,pend,false), iterator(p+1,pend,true));
                if (Clearer::isClear(*p))
                    return std::make_pair(iterator(pend,pend,false), iterator(pend,pend,false));
            }
            // Hit the end, so search back from before the start of the block.
            for (pointer p = search_start-1; p >= pstart; --p)
            {
                if (key_equal()(key,*p))
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
            if (key_equal()(key,*p))
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
            if (!key_equal()(key,*p))
            {
                return std::make_pair(iterator(p+1,pend,false), iterator(end_found,pend,true));
            }
        }
        return std::make_pair(iterator(pstart,pend,false), iterator(end_found,pend,true));
    }
    /// @}

    /// Inserts the given value into the set, unless MULTI is false and
    /// there's already an item for which key_equal()(value,other)
    /// returns true.  Returns a pair of iterator to the inserted value
    /// (or the existing item if not inserted), and a bool that's true
    /// iff the item was inserted.
    /// @{
    std::pair<iterator, bool> insert(const value_type &value)
    {
        pointer p;
        bool success = insertHelper(myBuckets, myNumBuckets, value, p);
        if (success)
        {
            *p = value;
            ++*((size_type*)(myBuckets+myNumBuckets));
        }
        return std::make_pair(iterator(p,myBuckets+myNumBuckets,false),success);
    }
    std::pair<iterator, bool> insert(value_type &&value)
    {
        pointer p;
        bool success = insertHelper(myBuckets, myNumBuckets, value, p);
        if (success)
        {
            *p = std::move(value);
            ++*((size_type*)(myBuckets+myNumBuckets));
        }
        return std::make_pair(iterator(p,myBuckets+myNumBuckets,false),success);
    }
    /// @}

    /// Inserts all of the items in the range [start_input,end_input).
    template<typename INPUT_ITERATOR>
    void insert(INPUT_ITERATOR start_input, INPUT_ITERATOR end_input)
    {
        // Preallocate enough buckets, instead of waiting for
        // insert to potentially hit collisions.
        size_type ninput_items = std::distance(start_input, end_input);
        if (!ninput_items)
            return;

        size_type new_size = size() + ninput_items;
        if (myNumBuckets < minBuckets(new_size))
            bumpNumBuckets(new_size);

        // Now, just insert them all, (no need to realloc)
        for (; start_input != end_input; ++start_input)
        {
            pointer pdest;
            bool success = insertHelper<!MULTI,false>(myBuckets,myNumBuckets,*start_input,pdest);
            if (success)
                *pdest = *start_input;
            else
                --new_size;

        }
        *((size_type*)(myBuckets+myNumBuckets)) = new_size;
    }
    /// Inserts all of the items from the initializer_list.
    void insert(std::initializer_list<value_type> list)
    {
        // Just delegate to the iterator range insert function.
        insert(list.begin(),list.end());
    }

    /// Inserts an element constructed with the given arguments.
    /// Maybe the standard implementation can somehow get a speedup
    /// from this, but we need the item's hash before inserting,
    /// so it needs to be constructed first.
    template <typename... Args>
    std::pair<iterator,bool> emplace(Args&&... args)
    {
        return insert(value_type(std::forward<Args>(args)...));
    }

    /// Removes the item at the specified iterator location,
    /// returning an iterator to the next item.
    ///
    /// Unlike std::unordered_set::erase(const_iterator), this may
    /// invalidate iterators and references to other items in the
    /// map, if they are in a contiguous block of non-empty items
    /// with the item that iter points to.  Also, this only
    /// accepts an iterator, instead of a const_iterator.
    /// (Why would std::unordered_set::erase accept a const_iterator?)
    iterator erase(iterator iter)
    {
        const pointer pstart = myBuckets;
        const pointer pend = pstart + myNumBuckets;

        const pointer p = (pointer)iter;

        UT_ASSERT_MSG_P(p >= pstart && p < pend && !Clearer::isClear(*p), "An iterator to erase can't point to a clear item!");

        // Actually erase the item
        Clearer::clear(*p);
        size_type new_size = --*((size_type*)(myBuckets+myNumBuckets));

        if (new_size == 0)
            return iterator(pend,pend,false);

        // Check for the common, simple case, where there's nothing after p in
        // the block of possible conflicts, and p isn't at the end.
        pointer pnext = p+1;
        bool end_case = (pnext == pend);
        if (!end_case)
        {
            if (Clearer::isClear(*pnext))
                return iterator(pnext,pend,true);

            // Now, for the hard case... -_-

            // If we're not right at the end of the array already,
            // find the end of the block, and check if it's the end of the array.
            // pnext is already clear, else we'd have returned above.
            do
            {
                ++pnext;
                end_case = (pnext == pend);
            } while (!end_case && !Clearer::isClear(*pnext));
        }

        if (end_case && (p != pstart) && !Clearer::isClear(*(p-1)))
        {
            // If this is a block right at the end of the array, there may be earlier
            // items that are too early, in which case, we need to shift
            // everything after that down one, into the empty spot.

            // Find the start of the block.
            pointer pprev = p-1;
            while (pprev != pstart && !Clearer::isClear(*(pprev-1)))
            {
                --pprev;
            }

            // Find the earliest item that should go at or after
            // its current bucket.
            do
            {
                // Check where this item ideally belongs.
                size_type hash = hasher()(*pprev);
                size_type ideal_bucket_num = hash%myNumBuckets;
                if (ideal_bucket_num > (pprev-pstart))
                {
                    // Shift everything later by one and return.
                    for (pointer pdest = p; pdest != pprev; --pdest)
                    {
                        *pdest = std::move(*(pdest-1));
                    }
                    Clearer::clear(*pprev);
                    return iterator(p+1,pend,true);
                }

                ++pprev;
            } while (pprev != p);
        }

        if (p+1 != pend)
        {
            // We're not at the end of the array, or there were no items before
            // this block to move into the empty space, but there is at least
            // one item after p, so shift items backward until there's one
            // that doesn't belong earlier than it is now.

            size_type empty_bucket_num = p-pstart;

            // NOTE: pnext points to the end of this block.
            for (pointer pdest = p; pdest+1 != pnext; ++pdest)
            {
                // Check where this item ideally belongs.
                size_type hash = hasher()(*(pdest+1));
                size_type ideal_bucket_num = hash%myNumBuckets;
                if (ideal_bucket_num > empty_bucket_num)
                {
                    Clearer::clear(*pdest);
                    return iterator(p+(pdest==p),pend,false);
                }

                *pdest = std::move(*(pdest+1));
                ++empty_bucket_num;
            }
            Clearer::clear(*(pnext-1));
            // p now contains an item that was previously after it.
            return iterator(p,pend,false);
        }

        // p is still empty, but there's an item (or the end pointer)
        // right after it.
        return iterator(p+1,pend,false);
    }

    /// Removes all items in the range [start_iter,end_iter),
    /// and returns an iterator immediately following that range.
    ///
    /// Unlike std::unordered_set::erase(const_iterator,const_iterator),
    /// this may invalidate iterators and references to other items in the
    /// map, if they are in a contiguous block of non-empty items
    /// with the items in the erased range.  Also, this only
    /// accepts iterator's, instead of const_iterator's.
    /// (Why would std::unordered_set::erase accept a const_iterator?)
    iterator erase(iterator start_iter, iterator end_iter) {
        if (start_iter == end_iter)
            return end_iter;

        const pointer pstart = myBuckets;
        const const_pointer pend = pstart + myNumBuckets;

        const pointer prange_start = (pointer)start_iter;
        const pointer prange_end = (pointer)end_iter;

        UT_ASSERT_MSG_P(prange_start >= pstart && prange_start < prange_end && prange_end <= pend && !Clearer::isClear(*prange_start), "An iterator to erase can't point to a clear item!");

        // Actually erase the items
        size_type nremoved_items = 1;
        size_type nitems_in_last_block = 1;
        Clearer::clear(*prange_start);
        for (pointer p = prange_start+1; p != prange_end; ++p)
        {
            if (!Clearer::isClear(*p))
            {
                Clearer::clear(*p);
                ++nitems_in_last_block;
                ++nremoved_items;
            }
            else
            {
                nitems_in_last_block = 0;
            }
        }

        size_type new_size = (*((size_type*)(myBuckets+myNumBuckets)) -= nremoved_items);

        if (new_size == 0)
        {
            // This must be pointing to the end of the array.
            return end_iter;
        }

        pointer block_end = prange_end;

        if (prange_end != pend)
        {
            // NOTE: If prange_end is not pend, it is always occupied, because
            // it came from an iterator, which should be dereferenceable.
            // However, if there was a gap between the last removed item
            // and prange_end, we can still exit early.
            if (nitems_in_last_block == 0)
                return end_iter;

            // Find the end of the block
            do
            {
                ++block_end;
            } while (block_end != pend && !Clearer::isClear(*block_end));
        }
        else {
            // If end_iter is at the array end, and there's at least one empty
            // spot in the given range, we're done, because the last block
            // was completely eliminated.
            if (nitems_in_last_block != prange_end-prange_start)
                return end_iter;
        }

        UT_ASSERT_P(nitems_in_last_block > 0);

        // If the end of the block is the array end and
        // nitems_in_last_block == prange_end-prange_start (no gaps),
        // find the beginning of the block, and if there are any items that
        // belong in later buckets:
        // 1) shift everything after to the end of the clear block.
        // 2) move them backward to where they belong, as available.
        // 3) if there is still an item in the last cleared spot, return.
        if (block_end == pend && nitems_in_last_block == prange_end-prange_start && prange_start != pstart && !Clearer::isClear(*(prange_start-1)))
        {
            // Find the start of the block.
            pointer block_start = prange_start;
	    do
            {
                --block_start;
            } while (block_start != pstart && !Clearer::isClear(*(block_start-1)));

            UT_ASSERT_MSG(0,"FIXME: Implement this!!!");
        }

        // Move items backward until there's one that doesn't
        // need to be moved backward.
        UT_ASSERT_MSG(0,"FIXME: Implement this!!!");
        return end_iter; // This is probably incorrect, but just to get it to compile
    }

    /// Removes all items matching key and returns
    /// the number of items removed.
    size_type erase(const Key &key)
    {
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

    /// NOTE: std::unordered_set::hash_function() isn't static,
    ///       but here, we're not storing a hasher as
    ///       a data member, so it's independent of the instance.
    static hasher hash_function()
    {
        return hasher();
    }

    /// NOTE: std::unordered_set::key_eq() isn't static,
    ///       but here, we're not storing a key_equal as
    ///       a data member, so it's independent of the instance.
    static key_equal key_eq()
    {
        return key_equal();
    }

    /// Returns the amount of memory used by this, excluding any memory that might be
    /// allocated by any of the items, themselves.
    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += sizeof(value_type)*myNumBuckets;
        return mem;
    }

    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEach(FUNCTOR &&functor) const
    {
        const_pointer p = myBuckets;
        const const_pointer pend = p + myNumBuckets;
        for (; p != pend; ++p)
        {
            if (!Clearer::isClear(*p))
                functor(*p);
        }
    }

protected:
    static size_type minBuckets(size_type size)
    {
        // If there are only 0-2 elements, there might as well only be
        // that many buckets.
        if (size <= 2)
            return size;

        // Simpler case if MAX_LOAD_FACTOR_256 evenly divides 256
        if ((256%MAX_LOAD_FACTOR_256) == 0)
            // +1 because otherwise it's guaranteed to be composite.
            return size * (256/MAX_LOAD_FACTOR_256) + 1;

        return (size*256 + MAX_LOAD_FACTOR_256-1)/MAX_LOAD_FACTOR_256;
    }
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

    template<bool fail_on_equal=!MULTI,bool check_realloc=true>
    bool insertHelper(pointer pstart, size_type nbuckets, const value_type &value,pointer &destp)
    {
        UT_ASSERT_P(!Clearer::isClear(value));

        if (check_realloc && nbuckets == 0)
        {
            setNumBuckets(1);
            destp = myBuckets;
            return true;
        }

        const pointer pend = pstart + nbuckets;
        const size_type hash = hasher()(value);
        const size_type ideal_bucket = (hash%nbuckets);
        pointer init_searchp = pstart + ideal_bucket;
        pointer searchp = init_searchp;

        if (Clearer::isClear(*searchp))
        {
            // Don't bother checking MAX_LOAD_FACTOR_256 if exact match is clear,
            // since there's no harm in perfect hashes... unless it fills up and
            // people do a lot of querying keys that aren't in the set.
            // If that ends up being a bottleneck, feel free to change this.
            destp = searchp;
            return true;
        }

        if (fail_on_equal && key_equal()(*searchp,value))
        {
            destp = searchp;
            return false;
        }

        if (!fail_on_equal)
        {
            // We're always adding one.
            size_type new_size = size() + 1;
            if (check_realloc && nbuckets < minBuckets(new_size)) {
                bumpNumBuckets(new_size);
                // Delegate to version that doesn't check for realloc.
                return insertHelper<fail_on_equal,false>(myBuckets,myNumBuckets,value,destp);
            }
            // We're not reallocating in this case.
        }

        // Find the place where our ideal bucket is less than the next
        // item's ideal bucket.
        if ((hasher()(*searchp)%nbuckets) <= ideal_bucket)
        {
            while (true)
            {
                ++searchp;
                if (searchp == pend || Clearer::isClear(*searchp))
                    break;

                const size_type other_hash = hasher()(*searchp);
                if (fail_on_equal && other_hash==hash && key_equal()(*searchp,value))
                {
                    destp = searchp;
                    return false;
                }
                if (ideal_bucket < (other_hash%nbuckets))
                    break;
            }
        }
        pointer insertp = searchp;

        // Find the end of the block
        while (searchp != pend && !Clearer::isClear(*searchp))
        {
            ++searchp;
        }

        if (searchp != pend)
        {
            // Common case: block end is not the array end.

            if (fail_on_equal)
            {
                // We're always adding one.
                size_type new_size = *((const size_type*)pend) + 1;
                if (check_realloc && nbuckets < minBuckets(new_size))
                {
                    bumpNumBuckets(new_size);
                    // Delegate to version that doesn't check for realloc.
                    return insertHelper<fail_on_equal,false>(myBuckets,myNumBuckets,value,destp);
                }
                // We're not reallocating in this case.
            }

            // Move items later by one.
            while (searchp != insertp)
            {
                *searchp = std::move(*(searchp-1));
                --searchp;
            }
            destp = insertp;
            return true;
        }

        // Less common case: block end is the array end.
        // insertp may actually need to go earlier, because
        // we can't move items later, else they'd be past the end.
        if (insertp == init_searchp) 
        {
            // There should always be a space somewhere if !fail_on_equal,
            // because reallocation was handled above.
            UT_ASSERT_P(insertp-1 >= pstart || (fail_on_equal && insertp == pstart));
            while ((!fail_on_equal || insertp != pstart) && !Clearer::isClear(*(insertp-1)))
            {
                const size_type other_hash = hasher()(*(insertp-1));
                if (fail_on_equal && other_hash == hash && key_equal()(*(insertp-1),value))
                {
                    destp = insertp-1;
                    return false;
                }
                if ((other_hash%nbuckets) <= ideal_bucket)
                    break;
                --insertp;
                UT_ASSERT_P(insertp-1 >= pstart || (fail_on_equal && insertp == pstart));
            }
        }

        if (fail_on_equal)
        {
            // We're always adding one.
            size_type new_size = *((const size_type*)pend) + 1;
            if (check_realloc && nbuckets < minBuckets(new_size))
            {
                bumpNumBuckets(new_size);
                // Delegate to version that doesn't check for realloc.
                return insertHelper<fail_on_equal,false>(myBuckets,myNumBuckets,value,destp);
            }
            // We're not reallocating in this case.
        }

        pointer blockstart = init_searchp;
        // NOTE: blockstart shouldn't hit pstart, because myNumBuckets > size()
        UT_ASSERT_P(blockstart-1 >= pstart);
        while (!Clearer::isClear(*(blockstart-1)))
        {
            --blockstart;
            UT_ASSERT_P(blockstart-1 >= pstart);
        }
        // Move items earlier by one.
        while (blockstart != insertp)
        {
            *(blockstart-1) = std::move(*blockstart);
            ++blockstart;
        }
        destp = insertp-1;
        return true;
    }

protected:
    pointer myBuckets;
    size_type myNumBuckets;
};

template<
    typename Key,
    bool MULTI,
    std::size_t MAX_LOAD_FACTOR_256,
    typename Clearer,
    class Hash,
    class KeyEqual
>
struct DefaultClearer<ArraySet<Key,MULTI,MAX_LOAD_FACTOR_256,Clearer,Hash,KeyEqual> >
{
    using TheType = ArraySet<Key,MULTI,MAX_LOAD_FACTOR_256,Clearer,Hash,KeyEqual>;
    SYS_FORCE_INLINE
    static void clear(TheType&v) { v.destroy(); }
    SYS_FORCE_INLINE
    static bool isClear(TheType &v) { return v.bucket_count() == 0; }
    SYS_FORCE_INLINE
    static void clearConstruct(TheType *p) { new((void *)p) TheType(); }

    static const bool clearNeedsDestruction = false;
};

} // namespace UT

template<typename Key,
    bool MULTI = false,
    int MAX_LOAD_FACTOR_256 = 128,
    typename Clearer = UT::DefaultClearer<Key>,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key> >
using UT_ArraySet = UT::ArraySet<Key,MULTI,MAX_LOAD_FACTOR_256,Clearer,Hash,KeyEqual>;

namespace std {
    template<typename Key,
    bool MULTI,
    int MAX_LOAD_FACTOR_256,
    typename Clearer,
    class Hash,
    class KeyEqual>
    void swap(
        UT::ArraySet<Key,MULTI,MAX_LOAD_FACTOR_256,Clearer,Hash,KeyEqual> &a,
        UT::ArraySet<Key,MULTI,MAX_LOAD_FACTOR_256,Clearer,Hash,KeyEqual> &b)
    {
        a.swap(b);
    }
}

#endif
