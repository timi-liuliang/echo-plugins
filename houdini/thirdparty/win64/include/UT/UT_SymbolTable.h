/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_SymbolTable.h
 *
 * COMMENTS:
 */

#ifndef __UT_SymbolTable_h__
#define __UT_SymbolTable_h__

#include "UT_API.h"
#include <stdio.h>
#include <SYS/SYS_Types.h>
#include "UT_ContainerPrinter.h"
#include "UT_IteratorRange.h"
#include "UT_StringMap.h"

#include <iterator>
#include <iosfwd>

/// A UT_SymbolMap contains a UT_Map but provides the interface
/// required by what was UT_SymbolTable.  Strings are passed as <tt>const char
/// *</tt>.
template <typename ITEM_T>
class UT_SymbolMap
{
public:
    typedef UT_StringMap<ITEM_T>                MapType;
    typedef typename MapType::const_iterator	const_map_iterator;
    typedef typename MapType::iterator		map_iterator;
    typedef ITEM_T value_type;

    /// Constructor
    explicit UT_SymbolMap()
	: myMap()
    {
    }
    /// Destructor
    ~UT_SymbolMap()
    {
	clear();
    }

    /// Add a symbol to the map
    void	addSymbol(const UT_StringHolder &symbol, const ITEM_T &data)
		{
                    (*this)[symbol] = data;
		}
    /// Find a symbol in the map.  If the symbol is found, the data pointer
    /// passed in will be assigned to the ITEM_T in the map.
    bool	findSymbol(const UT_StringRef &symbol, ITEM_T *datap) const
		{
		    const_map_iterator it = myMap.find(symbol);
		    if (it != myMap.end())
		    {
			if (datap)
			    *datap = it->second;
			return true;
		    }
		    return false;
		}

    /// Find a symbol and return an iterator.  Returns map_end() if the symbol
    /// is not found in the map.
    /// @{
    map_iterator	find(const UT_StringRef &symbol)
			{
			    return myMap.find(symbol);
			}
    const_map_iterator	find(const UT_StringRef &symbol) const
			{
			    return myMap.find(symbol);
			}
    /// @}

    /// Check if a symbol exists
    bool	contains(const UT_StringRef &symbol) const
		    { return myMap.count(symbol) > 0; }
    /// Find out how many times a symbol exists (this will be either 0 or 1).
    exint	count(const UT_StringRef &symbol) const
		    { return myMap.count(symbol); }

    /// Erase a symbol from the map
    bool	deleteSymbol(const UT_StringRef &symbol)
		{
		    myMap.erase(symbol);
		    return true;
		}
    /// @{
    /// Erase a symbol from the map
    exint	erase(const UT_StringRef &symbol)
		{
		    return myMap.erase(symbol);
		}
    map_iterator erase(const_map_iterator it)
		{
		    return myMap.erase(it);
		}
    /// @}
    /// Clear the map
    void	clear()
		{
		    myMap.clear();
		}

    /// Add a symbol and return the string reference
    const char	*addSymbolAndGetReference(const UT_StringHolder &symbol,
			    const ITEM_T &data)
		{
                    addSymbol(symbol, data);

		    UT_ASSERT_P(myMap.find(symbol) != myMap.end());
		    return myMap.find(symbol)->first.c_str();
		}


    /// @{
    /// Query the number of elements in the map
    exint	 entries() const	{ return myMap.size(); }
    exint	 size() const		{ return myMap.size(); }
    /// @}

    /// @{
    /// Information about the buckets
    exint	bucket_count() const	{ return myMap.bucket_count(); }
    exint	capacity() const
		{ return exint(myMap.bucket_count()*myMap.max_load_factor()); }
    /// @}

    /// Check if the map is empty
    bool	 empty() const { return myMap.empty(); }

    /// Retrieve an entry in the table by name, creating it if it doesn't
    /// already exist.
    ITEM_T	&operator[](const UT_StringHolder &symbol)
		 {
                     return myMap[symbol];
		 }
    ITEM_T	&insert(const UT_StringHolder &name, const ITEM_T &item)
		 {
		    // Assign and return the reference in the map
		    return ((*this)[name] = item);
		 }

    /// Call this method when you're adding entries to a table inside a loop
    /// and you know how many entries you'll be adding.  This will create at
    /// least @c size buckets but also ensure the load factor is less than the
    /// maximum load factor.
    void	 reserveTableSize(exint size)
		    { myMap.rehash(size); }

    /// Get current load factor
    float	getLoadFactor() const
		    { return myMap.load_factor(); }

    /// @{
    /// Adjust the maximum/minimum load factors
    /// The table will automatically rebuild itself to keep the load factor
    /// within the specified range.  A minimum load factor of 0 means that
    /// the table will never shrink when elements are deleted from it.
    float	getMaxLoadFactor() const
		    { return myMap.max_load_factor(); }
    void	setMaxLoadFactor(float f)
		    { myMap.max_load_factor(f); }
    float	getMinLoadFactor() const
		    { return 0.; }
    void	setMinLoadFactor(float) {}
    /// @}

    /// Merge the specified table with ourselves.
    void	mergeTable(const UT_SymbolMap &table)
		{
		    const_map_iterator	it, last;
		    last = table.map_end();
		    for (it = table.map_begin(); it != last; ++it)
			(*this)[it->first] = it->second;
		}

    /// "getStringReference" returns a pointer to the actual string that
    /// is maintained in the symbol table. This can be used to implement
    /// a shared string symbol table.
    const char	*getStringReference(const UT_StringRef &symbol)
		{
		    const_map_iterator	it = myMap.find(symbol);
		    if (it == map_end())
		    {
			// If we don't have an entry, we create one.  Duh.
			return addSymbolAndGetReference(symbol, ITEM_T());
		    }
		    return it->first.c_str();
		}

    /// Returns a unique key based using the passed-in string as a prefix.  
    UT_StringHolder makeUniqueKey(const char *name) const
    {
	if (!UTisstring(name))
	    return UT_StringHolder();
	    
	UT_String s(name);
	while (myMap.find(s) != myMap.end())
	    s.incrementNumberedName();
	
	UT_StringHolder n;
	n.adoptFromString(s);
	return n;
    }
    
    /// Return an approximation of how much memory we use
    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myMap.getMemoryUsage(false);
	// The keys store copies of the strings if SAFE_STRINGS, so we need to count them
	for (const_map_iterator it = myMap.begin(); it != myMap.end(); ++it)
	    mem += it->first.getMemoryUsage(false);
        return mem;
    }

    /// @{
    /// "traverse" invokes the given function with every member of the symbol
    /// table. The order of traversal is undefined. If the function returns a
    /// non-zero value the traversal continues. If it returns zero then
    /// traversal stops. The return value is zero if the traversal was
    /// interupted and one otherwise.
    ///
    /// Call traverseConst() if you will not be removing things from the
    /// table while traversing, and traverse() if you might be.  It is only
    /// safe to remove the current item when using traverse() -- not other
    /// items. If you call traverse() and add something to the table, you
    /// may or may not encounter it later in the traversal.
    int		traverseConst(
			  int (*function)(ITEM_T &, const char *, void *),
			  void *data) const
		{
		    for (const_map_iterator it = map_begin(); it != map_end(); ++it)
		    {
			if (!function(const_cast<ITEM_T &>(it->second),
				    it->first.c_str(), data))
			{
			    return 0;
			}
		    }
		    return 1;
		}
    int		traverse(int (*function)(ITEM_T &, const char *, void *),
			  void *data)
		{
		    for (map_iterator it = map_begin(); it != map_end(); )
		    {
			// Increment before calling the functor to ensure
			// that 'it' remains valid even if we delete the
			// current element.  It's still unsafe to delete
			// the next element.
			map_iterator curr = it++;
			if (!function(curr->second, curr->first.c_str(), data))
			{
			    return 0;
			}
		    }
		    return 1;
		}
    /// @}

    /// Iterator conforming to Houdini's atEnd(), advance() interface
    template<typename T, typename IT>
    class base_iterator : 
	public std::iterator<std::forward_iterator_tag, T>
    {
	public:
	    typedef T&		reference;
	    typedef T*		pointer;
	    
	    base_iterator()
		: myIt()
		, myEnd()
	    {}

	    // Converting constructor to const_iterator
	    template<typename ET, typename EIT>
	    base_iterator(const base_iterator<ET, EIT> &src)
		: myIt(src.myIt), myEnd(src.myEnd) {}
	    
	    reference value() const { return const_cast<T &>(myIt->second); }
	    reference operator*() const	{ return thing(); }
	    pointer operator->() const { return &thing(); }

	    reference thing() const { return value(); }
	    
	    const UT_StringHolder &key() const
				 { return myIt->first; }
	    const char		*name() const
				 { return key().c_str(); }

	    bool	operator==(const base_iterator<T, IT> &cmp) const
			{
			    return myIt == cmp.myIt;
			}
	    bool	operator!=(const base_iterator<T, IT> &cmp) const
			{ return !(*this == cmp); }

	    bool	atEnd() const
			{
			    return myIt == myEnd;
			}

	    base_iterator &operator++()
			{
			    ++myIt;
			    return *this;
			}

	    base_iterator &operator=(const base_iterator<T, IT> &src)
			 {
			     myIt = src.myIt;
			     myEnd = src.myEnd;
			     return *this;
			 }

	private:
	    base_iterator(const IT &it,
	                  const IT &end)
		: myIt(it)
		, myEnd(end)
	    {}

	    IT myIt, myEnd;

	    friend class UT_SymbolMap<ITEM_T>;
    };

    typedef base_iterator<ITEM_T, map_iterator> iterator;
    typedef base_iterator<const ITEM_T, const_map_iterator> const_iterator;
    
    typedef const_iterator	traverser;

    iterator	begin() { return iterator(myMap.begin(), myMap.end()); }
    iterator	end() { return iterator(myMap.end(), myMap.end()); }
    const_iterator begin() const { return const_iterator(myMap.begin(), myMap.end()); }
    const_iterator end() const { return const_iterator(myMap.end(), myMap.end()); }

    const_map_iterator	map_begin() const	{ return myMap.begin(); }
    const_map_iterator	map_end() const		{ return myMap.end(); }
    map_iterator	map_begin()		{ return myMap.begin(); }
    map_iterator	map_end()		{ return myMap.end(); }
    
    UT_IteratorRange<const_map_iterator> map_range() const 
			{ return UTmakeRange(myMap.begin(), myMap.end()); }  
    UT_IteratorRange<map_iterator> map_range() 
			{ return UTmakeRange(myMap.begin(), myMap.end()); }  

    /// Write out some statistics pertaining to the underlying map container to
    /// the output stream given.
    void outputStats(std::ostream &os) const;
protected:
    template<typename OS, typename T>
    friend OS &operator<<(OS &os, const UT_SymbolMap<T> &d);
    
    MapType		myMap;
};

template <typename T>
void	 
UT_SymbolMap<T>::outputStats(std::ostream &os) const
{
    os << "     Table Size: " << size() << "\n";
    os << "   Bucket Count: " << bucket_count() << "\n";
    os << "       Capacity: " << capacity() << "\n";
    os << "    Load Factor: " << getLoadFactor() << "\n";
    os << "Max Load Factor: " << getMaxLoadFactor() << "\n";
}

template<typename OS, typename T>
inline OS &
operator<<(OS &os, const UT_SymbolMap<T> &d)
{
    os << d.myMap;
    return os;
}


#endif
