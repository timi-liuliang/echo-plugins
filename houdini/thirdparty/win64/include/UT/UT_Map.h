/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Map.h (UT Library, C++)
 *
 * COMMENTS:	Wrapper for an unordered map data structure.
 *
 *  UT_Map<key, entry> map;
 *  key   id;
 *  entry item;
 *
 *  map[id] = item;		// insert or replace
 *
 *  item = map[id];		// find or create new
 *
 *  UT_Map<key,entry>::iterator map_item = map.find(id);  // find only
 *  if(map_item != map.end())
 *      item = map_item->second;
 *
 *  map.erase(id);		// remove entry
 *
 *  for(auto it = map.begin(); it != map.end(); ++it)
 *  {
 *      key = it->first;	// traverse
 *      item = it->second;
 *  }
 *
 *  int num = map.size();	// #entries
 *
 *  map.clear();		// remove all entries
 */

#ifndef __UT_Map__
#define __UT_Map__

#include "UT_ContainerPrinter.h"
#include "UT_IteratorRange.h"
#include <SYS/SYS_Types.h>

#include <hboost/unordered_map.hpp>
#include <iterator>
#include <map>

template<typename K, typename V, typename H, typename P>
int64
UTgetMemoryUsage(const hboost::unordered_map<K, V, H, P> &map, bool inclusive)
{
    int64 mem = inclusive ? sizeof(map) : 0;
    // Buckets only contain a pointer to the node
    mem += map.bucket_count() * sizeof(void*);
    // Nodes contain the hash value, a pointer to the next node,
    // and the key-value pair.
    mem += map.size() * (sizeof(size_t) + sizeof(void*) + sizeof(std::pair<K,V>));
    return mem;
}

template<typename K, typename V, typename C>
int64
UTgetMemoryUsage(const std::map<K, V, C> &map, bool inclusive)
{
    int64 mem = inclusive ? sizeof(map) : 0;

    // NOTE: If the comparator object of type C owns any memory
    //       (i.e. apart from itself) when default constructed
    //       or copy constructed, that memory won't be counted.

    // NOTE: This count is for the VC++ 2010 implementation
    //       of std::map, but others should be in the ballpark.
    // Nodes contain pointers to the left, parent, and right,
    // the key-value pair, a colour in a char (red or black),
    // and a flag in a char indicating if the node is the head.
    // Round up to a multiple of 4 on the size of each node.
    mem += (map.size() + 1) * ((3*sizeof(void*) + sizeof(std::pair<const K,V>)
                               + 2*sizeof(char) + 3) & ~3);
    return mem;
}

/// Unsorted map container.
template<typename K, typename V,
	 typename H = hboost::hash<K>, typename P = std::equal_to<K> >
class UT_Map : public hboost::unordered_map<K, V, H, P>
{
public:
    // Hoisting of base types
    typedef hboost::unordered_map<K, V, H, P>	Base;
    typedef typename Base::key_type		key_type;
    typedef typename Base::mapped_type		mapped_type;
    typedef typename Base::value_type		value_type;
    typedef typename Base::hasher		hasher;
    typedef typename Base::key_equal		key_equal;
    typedef typename Base::iterator		iterator;
    typedef typename Base::const_iterator	const_iterator;
    typedef H Hasher;
    typedef P Equal;

    /// Initialize an empty map, and optionally a custom hasher and 
    /// equal compare functions.
    explicit UT_Map(const Hasher &hf = Hasher(),
                    const Equal &eql = Equal()) :
	Base(hboost::unordered::detail::default_bucket_count, hf, eql) {}

    /// Initialize the map from an iterator pair. 
    template <typename InputIt>
    UT_Map(InputIt first, InputIt last) : Base(first, last) {}

    /// Initialize the map from an iterator pair, and optionally a custom   
    /// hasher and equal compare functions.
    template <typename InputIt>
    UT_Map(InputIt first, InputIt last,
            const Hasher &hf = Hasher(),
            const Equal &eql = Equal()) :
            Base(first, last, hboost::unordered::detail::default_bucket_count,
                 hf, eql) {}

    /// Initialize the map using an initializer list. The initializer list is a  
    /// list of pairs of items to add to the map. E.g:
    /// @code
    /// UT_Map<int, const char *> foo = {{1, "one"}, {2, "two"}};
    /// @endcode
    UT_Map(std::initializer_list<value_type> init_list) 
    {
	// We can't thunk down to the hboost::unordered_map initializer_list 
	// constructor, since it seems disabled when building with clang.
	this->insert(init_list.begin(), init_list.end());
    }
    
    UT_Map(UT_Map const &other) : Base(other) {}

    /// Returns the approximate size, in bytes, of the memory consumed by this map
    /// or, optionally, only the data contained within.
    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += UTgetMemoryUsage(*static_cast<const Base *>(this), false);
        return mem;
    }

    /// Returns @c true if a value with the @c key is contained in the map. 
    bool contains(const key_type &key) const
    {
	return this->find(key) != this->end();
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
    
    template<typename IT, typename T, typename DR>
    class partial_iterator_base : 
	public std::iterator<std::forward_iterator_tag, T>
    {
    public:
	typedef T& reference;
	typedef T* pointer;
	
	partial_iterator_base() : it() {}
	
	template<typename EIT, typename EDR>
	partial_iterator_base(const partial_iterator_base<EIT, T, EDR> &src) :
	    it(src.it) {}
	
	reference operator*() const { DR dr; return dr(it); }
	pointer operator->() const { DR dr; return &dr(it); }
	
	bool operator==(const partial_iterator_base<IT, T, DR> &o) const
	{ return it == o.it; }
	
	bool operator!=(const partial_iterator_base<IT, T, DR> &o) const
	{ return it != o.it; }
	
	partial_iterator_base &operator++()
	{
	    ++it;
	    return *this;
	}

	partial_iterator_base &operator=(const partial_iterator_base<IT, T, DR> &src)
	{ 
	    it = src.it;
	    return *this;
	}
    protected:
	friend class UT_Map<K, V, H, P>;
	
	partial_iterator_base(IT it) : it(it) {}
    private:
	IT it;
    };
    
public:
    using key_iterator = partial_iterator_base<iterator, key_type, 
	    deref_pair_first<iterator, key_type>>;
    using const_key_iterator = partial_iterator_base<const_iterator, const key_type, 
	deref_pair_first<const_iterator, const key_type>>;
    using mapped_iterator = partial_iterator_base<iterator, mapped_type,
	deref_pair_second<iterator, mapped_type>>;
    using const_mapped_iterator = partial_iterator_base<const_iterator, const mapped_type, 
	deref_pair_second<const_iterator, const mapped_type>>;
    
    /// Returns a range object that iterates over the map but returns only 
    /// the key values. 
    /// Example:
    /// @code
    /// UT_Map<int, const char *> foo = {{1, "one"}, {2, "two"}};
    /// for (int key : foo.key_range())
    ///    std::cout << key << "\n";
    /// @endcode
    
    UT_IteratorRange<key_iterator> key_range()  
    { return UTmakeRange(key_iterator(this->begin()), 
			 key_iterator(this->end())); }
    
    /// Returns a const range object that iterates over the map but returns 
    /// only the key values.
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
};

/// Sorted map container.
template<typename K, typename V, typename C = std::less<K> >
class UT_SortedMap : public std::map<K, V, C>
{
public:
    // Hoisting of base types.
    typedef std::map<K, V, C>	Base;
    typedef typename Base::key_type		key_type;
    typedef typename Base::mapped_type		mapped_type;
    typedef typename Base::value_type		value_type;
    typedef typename Base::key_compare		key_compare;
    typedef typename Base::iterator		iterator;
    typedef typename Base::const_iterator	const_iterator;

    typedef C LessThan;

    UT_SortedMap() : Base() {}

    explicit UT_SortedMap(const LessThan &lt) : Base(lt) {}

    template<typename InputIt>
    UT_SortedMap(InputIt first, InputIt last) : Base(first, last) {}

    template<typename InputIt>
    UT_SortedMap(InputIt first, InputIt last, const LessThan &lt) :
	Base(first, last, lt) {}

    UT_SortedMap(const UT_SortedMap &other) : Base(other) {}

    /// Initialize the map using an initializer list. The initializer list is a  
    /// list of pairs of items to add to the map. E.g:
    /// @code
    /// UT_Map<int, const char *> foo = {{1, "one"}, {2, "two"}};
    /// @endcode
    UT_SortedMap(std::initializer_list<value_type> init_list) 
    {
	this->insert(init_list.begin(), init_list.end());
    }

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += UTgetMemoryUsage(*static_cast<const Base *>(this), false);
        return mem;
    }

    bool contains(const key_type &key) const
    {
	return this->find(key) != this->end();
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
    
    template<typename IT, typename T, typename DR>
    class partial_iterator_base : 
	public std::iterator<std::forward_iterator_tag, T>
    {
    public:
	typedef T& reference;
	typedef T* pointer;
	
	partial_iterator_base() : it() {}
	
	template<typename EIT, typename EDR>
	partial_iterator_base(const partial_iterator_base<EIT, T, EDR> &src) :
	    it(src.it) {}
	
	reference operator*() const { DR dr; return dr(it); }
	pointer operator->() const { DR dr; return &dr(it); }
	
	bool operator==(const partial_iterator_base<IT, T, DR> &o) const
	{ return it == o.it; }
	
	bool operator!=(const partial_iterator_base<IT, T, DR> &o) const
	{ return it != o.it; }
	
	partial_iterator_base &operator++()
	{
	    ++it;
	    return *this;
	}

	partial_iterator_base &operator=(const partial_iterator_base<IT, T, DR> &src)
	{ 
	    it = src.it;
	    return *this;
	}
    protected:
	friend class UT_SortedMap<K, V, C>;
	
	partial_iterator_base(IT it) : it(it) {}
    private:
	IT it;
    };
    
public:
    using key_iterator = partial_iterator_base<iterator, key_type, 
	    deref_pair_first<iterator, key_type>>;
    using const_key_iterator = partial_iterator_base<const_iterator, const key_type, 
	deref_pair_first<const_iterator, const key_type>>;
    using mapped_iterator = partial_iterator_base<iterator, mapped_type,
	deref_pair_second<iterator, mapped_type>>;
    using const_mapped_iterator = partial_iterator_base<const_iterator, const mapped_type, 
	deref_pair_second<const_iterator, const mapped_type>>;
    
    /// Returns a range object that iterates over the map but returns only 
    /// the key values.
    UT_IteratorRange<key_iterator> key_range()  
    { return UTmakeRange(key_iterator(this->begin()), 
			 key_iterator(this->end())); }
    
    /// Returns a const range object that iterates over the map but returns 
    /// only the key values.
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
};

namespace std
{
    // This helper needs to live in the 'std' namespace for argument-dependent 
    // lookup to succeed.
    template<typename OS, typename K, typename V>
    inline OS &
    operator<<(OS &os, const pair<K, V> &v)
    {
	os << "<" << v.first << ", " << v.second << ">";
	return os;
    }
}

template<typename OS, typename K, typename V>
inline OS &
operator<<(OS &os, const UT_Map<K, V> &d)
{
    os << "UT_Map" << UT_ContainerPrinter<UT_Map<K, V> >(d);
    return os;
}

template<typename OS, typename K, typename V>
inline OS &
operator<<(OS &os, const UT_SortedMap<K, V> &d)
{
    os << "UT_SortedMap" << UT_ContainerPrinter<UT_SortedMap<K, V> >(d);
    return os;
}

#endif
