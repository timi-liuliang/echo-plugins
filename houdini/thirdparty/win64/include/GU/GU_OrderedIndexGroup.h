/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_OrderedIndexGroup.h ( GU Library, C++)
 *
 * COMMENTS:	This is a helper class for GU_Selection derivatives to keep
 * 		selection as an ordered group of indices, rather than offsets,
 * 		so that selection can remain GA_Detail invariant.
 */

#ifndef __GU_OrderedIndexGroup__
#define __GU_OrderedIndexGroup__

#include <UT/UT_Array.h>
#include <UT/UT_Map.h>
#include <iterator>

// Tombstone value for the index type, so that we can mark a dead value as  
// distinct from a potentially valid value.
template<typename INDEX>
inline INDEX
tombstone() { return INDEX(); }

template<typename INDEX>
class GU_OrderedIndexGroup
{
    typedef UT_Map<INDEX, exint> IndexMap;
    typedef UT_Array<INDEX> IndexOrder;
    
public:
    typedef INDEX type;

    GU_OrderedIndexGroup() : myCompact(true) {}
    
    void append(const INDEX &idx)
    {
	typename IndexMap::iterator it = myIndices.find(idx);

	if (it == myIndices.end())
	{
	    exint order = myOrder.append(idx);
	    myIndices.insert(std::make_pair(idx, order));
	}
	else
	{
	    // Just update the order. The newly added entry gets put last.
	    myOrder(it->second) = tombstone<INDEX>();
	    it->second = myOrder.append(idx);
	}
    }
    
    bool erase(const INDEX &idx)
    {
	typename IndexMap::const_iterator it = myIndices.find(idx);
	
	if (it == myIndices.end())
	    return false;
	
	exint order = it->second;
	myOrder(order) = tombstone<INDEX>();

	myIndices.erase(it);
	myCompact = false;
	
	return true;
    }
    
    void clear()
    {
	myIndices.clear();
	myOrder.clear();
	myCompact = true;
    }
    
    bool contains(const INDEX &idx) const 
    { 
	// The order doesn't matter for is-member search.
	return myIndices.find(idx) != myIndices.end(); 
    }
    
    bool contains(const INDEX &idx, INDEX &contained_idx) const 
    { 
	// The order doesn't matter for is-member search.
	typename IndexMap::const_iterator it = myIndices.find(idx);

	if (it != myIndices.end())
	{
	    contained_idx = myOrder(it->second);
	    return true;
	}

	return false;
    }
    
    // For sizing, the index list is the most authoritative one.
    size_t size() const { return myIndices.size(); }
    
    bool empty() const { return myIndices.empty(); }
    
    // Simple adapter to skip over tombstones when iterating over indices
    class const_iterator : public std::iterator<std::forward_iterator_tag, const INDEX>
    {
    public:
	const INDEX &operator *() { return *myIt; }
	
	bool operator==(const const_iterator &it) { return myIt == it.myIt; } 
	bool operator!=(const const_iterator &it) { return myIt != it.myIt; }
	
	const_iterator &operator++() 
	{
	    // Skip over tombstones so our user doesn't see them.
	    do { ++myIt; } while(myIt != myEnd && *myIt == tombstone<INDEX>());
	    return *this;
	}
    protected:
	friend class GU_OrderedIndexGroup;
	const_iterator(const typename IndexOrder::const_iterator &it, 
	               const typename IndexOrder::const_iterator &end) 
	    : myIt(it), myEnd(end) 
	{
	    // Fast forward until we're no longer looking at tombstones
	    while(myIt != myEnd && *myIt == tombstone<INDEX>())
		++myIt;
	}
    private:
	typename IndexOrder::const_iterator myIt, myEnd;
    };
    typedef const_iterator iterator;
    
    const_iterator begin() const
    {
	return const_iterator(myOrder.begin(), myOrder.end());
    }
    const_iterator end() const
    {
	return const_iterator(myOrder.end(), myOrder.end());
    }
    
    void dump(std::ostream &os) const 
    {
	os << "Indices: ";
	for(const typename IndexMap::value_type &oi : myIndices)
	    os << "[ " << oi.first << ", " << oi.second << "] ";
	os << "\n";
	os << "Order: " << myOrder << "\n";
    }
    
    
    void swap(GU_OrderedIndexGroup &group)
    {
	myIndices.swap(group.myIndices);
	myOrder.swap(group.myOrder);
	std::swap(myCompact, group.myCompact);
    }
    
    void compact()
    {
	if (myCompact)
	    return;
	
	// Create a remapping array for the new order. We use this to move 
	// items in the order list into their new positions and also to modify
	// the order stored in the index list.
	UT_Array<exint> order_remap;
	
	exint		order = 0;
	order_remap.setSizeNoInit(myOrder.size());
	for (int i = 0, ie = myOrder.size(); i < ie; i++)
	{
	    if (myOrder(i) == tombstone<INDEX>())
		order_remap(i) = -1;
	    else
	    {
		// Shift entries down in the order array.
		myOrder(order) = myOrder(i);
		order_remap(i) = order++;
	    }
	}
	
	myOrder.truncate(order);
	
	// Now map the order stored in the index list to the new order as
	// represented in the order array.
	// Note that unordered_set iterators are always constant, but since the
	// order field doesn't contribute to equivalence, it's mutable and we
	// can change it at will without affecting the order of the set.
	for(typename IndexMap::value_type &oi : myIndices)
	{
	    oi.second = order_remap(oi.second);
	}
	
	myCompact = true;
    }

    int64 getMemoryUsage(bool inclusive) const
    {
	int64 mem = inclusive ? sizeof(*this) : 0;
	// Compute size of unordered_multiset, see UT_Set.h for original.
	mem += myIndices.bucket_count() * sizeof(void*);
	mem += myIndices.size() * (sizeof(size_t) + sizeof(void*) + sizeof(INDEX));
	mem += myOrder.getMemoryUsage(false);
	return mem;
    }
    
private:
    IndexMap		myIndices;
    IndexOrder		myOrder;
    bool		myCompact;
};

#endif // __GU_OrderedIndexGroup__
