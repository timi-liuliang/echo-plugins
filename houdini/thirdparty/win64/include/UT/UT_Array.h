/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_Array.h (UT Library, C++)
 *
 * COMMENTS:    This is the array class implementation used by
 *              almost everything in the codebase.
 *              Please be careful if you need to change it!
 */

#pragma once

#ifndef __UT_ARRAY_H_INCLUDED__
#define __UT_ARRAY_H_INCLUDED__

#include "UT_API.h"
#include "UT_ArrayHelp.h"
#include "UT_Assert.h"
#include "UT_ContainerPrinter.h"
#include "UT_IteratorRange.h"
#include "UT_Permute.h"

#include <SYS/SYS_Compiler.h>
#include <SYS/SYS_Deprecated.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_TypeTraits.h>

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <utility>

#include <string.h>


template <typename T>
class UT_Array
{
public:
    typedef T value_type;

    typedef int (*Comparator)(const T *, const T *);

    /// Copy constructor. It duplicates the data.
    /// It's marked explicit so that it's not accidentally passed by value.
    /// You can always pass by reference and then copy it, if needed.
    /// If you have a line like:
    /// UT_Array<int> a = otherarray;
    /// and it really does need to copy instead of referencing,
    /// you can rewrite it as:
    /// UT_Array<int> a(otherarray);
    explicit UT_Array(const UT_Array<T> &a);
    
    /// Move constructor. Steals the working data from the original.
    UT_Array(UT_Array<T> &&a) SYS_NOEXCEPT;
    
    /// Construct based on given capacity and size
    UT_Array(exint capacity, exint size)
    {
        myData = capacity ? allocateCapacity(capacity) : nullptr;
        if (capacity < size)
            size = capacity;
        mySize = size;
        myCapacity = capacity;
        trivialConstructRange(myData, mySize);
    }

    /// Construct based on given capacity with a size of 0
    explicit UT_Array(exint capacity = 0) : myCapacity(capacity), mySize(0)
    {
        myData = capacity ? allocateCapacity(capacity) : nullptr;
    }

    /// Construct with the contents of an initializer list
    explicit UT_Array(std::initializer_list<T> init);

    ~UT_Array();

    void	    swap(UT_Array<T> &other);

    /// Append an element to the current elements and return its index in the
    /// array, or insert the element at a specified position; if necessary,
    /// insert() grows the array to accommodate the element. The insert
    /// methods use the assignment operator '=' to place the element into the
    /// right spot; be aware that '=' works differently on objects and pointers.
    /// The test for duplicates uses the logical equal operator '=='; as with
    /// '=', the behaviour of the equality operator on pointers versus objects
    /// is not the same.
    /// Use the subscript operators instead of insert() if you are appending
    /// to the array, or if  you don't mind overwriting the element already 
    /// inserted at the given index.
    exint           append(void) { return insert(mySize); }
    exint           append(const T &t) { return appendImpl(t); }
    exint           append(T &&t) { return appendImpl(std::move(t)); }
    exint	    append(const T &t, bool check_dup)
		    {
			exint	idx;
			if (check_dup && ((idx = find(t)) != -1))
			    return idx;
			return append(t);
		    }
    void            append(const T *pt, exint count);
    void	    appendMultiple(const T &t, exint count);
    exint	    insert(exint index);
    exint	    insert(const T &t, exint i)
                        { return insertImpl(t, i); }
    exint	    insert(T &&t, exint i)
                        { return insertImpl(std::move(t), i); }

    /// Adds a new element to the array (resizing if necessary) and forwards
    /// the given arguments to T's constructor.
    /// NOTE: Unlike append(), the arguments cannot reference any existing
    /// elements in the array. Checking for and handling such cases would
    /// remove most of the performance gain versus append(T(...)). Debug builds
    /// will assert that the arguments are valid.
    template <typename... S>
    exint	    emplace_back(S&&... s);

    /// Assuming the array is sorted, it inserts item t maintaining the sorted
    /// state of the array. It returns the index of the inserted item.
    /// @note This is O(N^2) behaviour if you call it in a loop! Do not use.
    /// @{
    SYS_DEPRECATED_HDK(13.0)
    exint	    sortedInsert(const T &t, Comparator compare);

    template <typename ComparatorBool>
    SYS_DEPRECATED_HDK(13.0)
    exint	    sortedInsert(const T &t, ComparatorBool is_less);
    /// @}

    SYS_DEPRECATED_HDK(13.0)
    exint	    uniqueSortedInsert(const T &t, Comparator compare);

    template <typename ComparatorBool>
    SYS_DEPRECATED_HDK(13.0)
    exint	    uniqueSortedInsert(const T &t, ComparatorBool is_less);

    /// Convenience method to perform binary search of a ascending sorted array
    /// with no duplicates.  Returns the index of the specified item, -1 if not
    /// found.
    exint	    uniqueSortedFind(const T &item, Comparator compare) const;
    template <typename ComparatorBool>
    exint	    uniqueSortedFind(const T &item, ComparatorBool is_less) const;

    /// Merge the given array into us.
    /// If direction is -1, then it assumes us and 'other' are both already
    /// sorted in descending order. Similarly, +1 means ascending.
    /// If allow_dups is false, then it further assumes that both arrays have no
    /// duplicates and will produce a result that also has no duplicates.
    /// More work will be needed if you want allow_dups to mean remove duplicates
    template <typename ComparatorBool>
    void	    merge(const UT_Array<T> &other, int direction,
			  bool allow_dups, ComparatorBool is_less);

    bool	    hasSortedSubset(const UT_Array<T> &other,
				    Comparator compare) const;

    void            sortedUnion(
                        const UT_Array<T> &other,
                        Comparator compare);
    void	    sortedUnion(
			const UT_Array<T> &other,
			UT_Array<T> &result,
			Comparator compare) const;
    void	    sortedIntersection(
			const UT_Array<T> &other,
			Comparator compare);
    void	    sortedIntersection(
			const UT_Array<T> &other,
			UT_Array<T> &result,
			Comparator compare) const;
    void	    sortedSetDifference(
			const UT_Array<T> &other,
			Comparator compare);
    void	    sortedSetDifference(
			const UT_Array<T> &other,
			UT_Array<T> &result,
			Comparator compare) const;

    /// Assuming the array is already a heap, it inserts item t maintaining
    /// the heap. It returns the index of the inserted item.
    exint	    heapPush(const T &t, Comparator compare);

    /// Assuming the array is already a heap, extracts the top (maximum)
    /// element from the heap and returns it.
    T		    heapPop(Comparator compare);

    /// Assuming the array is already a heap, return the top (maximum)
    /// element.
    const T &	    heapMax() const
    {
        UT_ASSERT_P(mySize > 0);
        return myData[0];
    }

    /// Takes another T array and concatenate it onto my end
    exint	    concat(const UT_Array<T> &a);

    /// Insert an element "count" times at the given index. Return the index.
    exint	    multipleInsert(exint index, exint count);

    /// An alias for unique element insertion at a certain index. Also used by
    /// the other insertion methods.
    exint	    insertAt(const T &t, exint index)
                        { return insertImpl(t, index); }

    /// Return true if given index is valid.
    bool	    isValidIndex(exint index) const
			{ return (index >= 0 && index < mySize); }

    /// Remove one element from the array given the element itself or its
    /// position in the list, and fill the gap by shifting the elements down
    /// by one position.  Return the index of the element remove or -1 if
    /// the value was not found.
    exint	    findAndRemove(const T &t);
    exint	    removeIndex(exint index)
    {
        return isValidIndex(index) ? removeAt(index) : -1;
    }
    SYS_FORCE_INLINE void removeLast()
    {
        if (mySize)
        {
            exint idx = --mySize;
            trivialDestruct(myData[idx]);
        }
    }

    /// Remove the range [begin_i,end_i) of elements from the array.
    void	    removeRange(exint begin_i, exint end_i);

    /// Remove the range [begin_i, end_i) of elements from this array and place
    /// them in the dest array, shrinking/growing the dest array as necessary.
    void            extractRange(exint begin_i, exint end_i,
                                 UT_Array<T>& dest);

    /// Removes all matching elements from the list, shuffling down and changing
    /// the size appropriately.
    /// Returns the number of elements left.
    template <typename IsEqual>
    exint	    removeIf(IsEqual is_equal);

    /// Remove all matching elements. Also sets the capacity of the array.
    template <typename IsEqual>
    void	    collapseIf(IsEqual is_equal)
    {
        removeIf(is_equal);
        setCapacity(size());
    }

    /// Move howMany objects starting at index srcIndex to destIndex;
    /// This method will remove the elements at [srcIdx, srcIdx+howMany) and
    /// then insert them at destIdx.  This method can be used in place of
    /// the old shift() operation.
    void	    move(exint srcIdx, exint destIdx, exint howMany);

    /// Cyclically shifts the entire array by howMany
    void	    cycle(exint howMany);

    /// Quickly set the array to a single value.
    void	    constant(const T &v);
    /// Zeros the array if a POD type, else trivial constructs if a class type.
    void	    zero();

    /// Search for t in one of two ways: linearly using the '==' operator, or
    /// binary using the function specified in the parameter list respectively.
    /// find() returns the index of the matching element or (exint)-1.
    /// Parameter s indicates the index the search should start at.
    exint	    find(const T &t, exint s = 0) const;
    exint	    find(const T &t, Comparator compare) const;

    /// Reverses the array by swapping elements in mirrored locations.
    void	    reverse();

    /// The fastest search possible, which does pointer arithmetic to find the
    /// index of the element. WARNING: index() does no out-of-bounds checking.
    exint	    index(const T &t) const { return &t - myData; }
    exint	    safeIndex(const T &t) const
    {
        return (&t >= myData && &t < (myData + mySize))
            ? &t - myData : -1;
    }

    /// Sort the array using a comparison function that you must provide. t1 and
    /// t2 are pointers to Thing.  The comparison function uses strcmp()
    /// semantics (i.e. -1 if less than, 0 if equal, 1 if greater).
    void	    sort(Comparator compare);

    /// Sort using std::sort.  The ComparatorBool uses the less-than semantics
    template <typename ComparatorBool>
    void	    stdsort(ComparatorBool is_less)
    {
        std::sort(myData, myData + mySize, is_less);
    }

    /// stableSort is both stable, so keeps equal elements in the same
    /// order (note this is very useful for compatibility between 
    /// compilers) and templated.
    /// Either use a bool sort function or make a utility class with
    /// bool operator()(const T a, const T b)
    /// the utility class lets you bind data to avoid globals.
    /// The comparator returns true if a must occur before b in the list.
    /// For sorting ascending, this is a less than operation.
    template<typename ComparatorBool>
    void	    stableSort(ComparatorBool is_less)
    {
        // No-op for small/empty arrays, avoiding out of
        // bounds assert on array()
        if (size() < 2)
            return;

        std::stable_sort(array(), array() + size(), is_less);
    }

    /// Comparator class for stableSortIndices
    template <typename I, typename V, typename ComparatorBool>
    class IndexedCompare
    {
    public:
        inline IndexedCompare(const UT_Array<V> &values,
                              const ComparatorBool &compare)
            : myValues(values)
            , myCompare(compare)
        {}
        inline bool operator()(I a, I b) const
        { return myCompare(myValues(a), myValues(b)); }
    private:
        const UT_Array<V> &myValues;
        const ComparatorBool &myCompare;
    };

    /// Sort indices array by the values indexed into this array using a
    /// stable sorting algorithm.  To reorder the array in such a way
    /// that it would be sorted, or another array to be reordered the
    /// same way, include UT_Permute.h and use:
    /// UTinversePermute(values.getArray(), indices.getArray(),
    ///                  values.size());
    /// The ComparatorBool uses the less-than semantics.
    /// I must be an integer type.
    template <typename I, typename ComparatorBool>
    void	    stableSortIndices(UT_Array<I> &indices,
                                      ComparatorBool is_less) const
    {
        IndexedCompare<I, T, ComparatorBool> compare(*this, is_less);
        std::stable_sort(indices.getArray(),
                         indices.getArray() + indices.size(), compare);
    }

    /// Create an index array from 0..n-1 into this array and sort
    /// it with stableSortIndices.
    /// The index array will be resized & rebuilt by this.
    template <typename I, typename ComparatorBool>
    void	    stableArgSort(UT_Array<I> &indices,
                                      ComparatorBool is_less) const
    {
	indices.setSizeNoInit(size());
	for (exint i = 0; i < size(); i++)
	    indices(i) = i;
	stableSortIndices(indices, is_less);
    }

    /// Sorts this array by an external key array.  We assume a 1:1
    /// corespondence between our own elements and those of the key
    /// array.  The comparator should be defined on the key type.
    template <typename K, typename ComparatorBool>
    void	  stableSortByKey(const UT_Array<K> &keys, ComparatorBool is_less)
    {
	UT_ASSERT(keys.size() == size());
	if (keys.size() != size())
	    return;
	UT_Array<exint>	indices;
	keys.stableArgSort(indices, is_less);
	UTinversePermute(getArray(), indices.getArray(), size());
    }

    /// Assuming this array is sorted, remove all duplicate elements.
    /// Returns the number of elements removed.
    exint	    sortedRemoveDuplicates();

    /// Assuming this array is sorted, remove all duplicate elements using the
    /// given binary predicate.
    /// Returns the number of elements removed
    template <typename CompareEqual>
    exint	    sortedRemoveDuplicatesIf(CompareEqual compare_equal);

    /// Sort remove duplicates. Requires that equal elements are adjacent after
    /// sorting.
    template<typename ComparatorBool>
    void	    sortAndRemoveDuplicates(ComparatorBool is_less)
    {
	stableSort(is_less);
	sortedRemoveDuplicates();
    }

    /// Partitions the array into values greater than or less than
    /// the Nth element, returns the resulting partition number.
    /// idx == 0 will get the minimum value, idx == size()-1 the
    /// maximum value.  This does modify this array!
    template <typename ComparatorBool>
    T		    selectNthLargest(exint idx, ComparatorBool is_less);

    /// Set the capacity of the array, i.e. grow it or shrink it. The
    /// function copies the data after reallocating space for the array.
    void            setCapacity(exint newcapacity);
    void            setCapacityIfNeeded(exint mincapacity)
    {
        if (capacity() < mincapacity)
            setCapacity(mincapacity);
    }
    /// If the capacity is smaller than mincapacity, expand the array
    /// to at least mincapacity and to at least a constant factor of the
    /// array's previous capacity, to avoid having a linear number of
    /// reallocations in a linear number of calls to bumpCapacity.
    void            bumpCapacity(exint mincapacity)
    {
        if (capacity() >= mincapacity)
            return;
        // The following 4 lines are just
        // SYSmax(mincapacity, UTbumpAlloc(capacity())), avoiding SYSmax
        exint bumped = UTbumpAlloc(capacity());
        exint newcapacity = mincapacity;
        if (bumped > mincapacity)
            newcapacity = bumped;
        setCapacity(newcapacity);
    }

    /// First bumpCapacity to ensure that there's space for newsize,
    /// expanding either not at all or by at least a constant factor
    /// of the array's previous capacity,
    /// then set the size to newsize.
    void            bumpSize(exint newsize)
    {
        bumpCapacity(newsize);
        setSize(newsize);
    }
    /// NOTE: bumpEntries() will be deprecated in favour of bumpSize() in a
    ///       future version.
    void            bumpEntries(exint newsize)
    {
        bumpSize(newsize);
    }

    /// Query the capacity, i.e. the allocated length of the array.
    /// NOTE: capacity() >= size().
    exint           capacity() const { return myCapacity; }
    /// Query the size, i.e. the number of occupied elements in the array.
    /// NOTE: capacity() >= size().
    exint           size() const     { return mySize; }
    /// Alias of size().  size() is preferred.
    exint           entries() const  { return mySize; }
    /// Returns true iff there are no occupied elements in the array.
    bool            isEmpty() const  { return mySize==0; }

    /// Returns the amount of memory used by this UT_Array.
    /// If inclusive is false, it only counts the memory of the array.
    /// This is often necessary to avoid double-counting, e.g. if this
    /// UT_Array is a member variable of a class whose memory is already
    /// being counted by the caller.
    int64	    getMemoryUsage(bool inclusive=false) const
    { return (inclusive ? sizeof(*this) : 0) + capacity()*sizeof(T); }

    /// Set the size, the number of occupied elements in the array.
    /// NOTE: This will not do bumpCapacity, so if you call this
    ///       n times to increase the size, it may take
    ///       n^2 time.
    void            setSize(exint newsize)
    {
        if (newsize < 0)
            newsize = 0;
        if (newsize == mySize)
            return;
        setCapacityIfNeeded(newsize);
        if (mySize > newsize)
            trivialDestructRange(myData + newsize, mySize - newsize);
        else // newsize > mySize
            trivialConstructRange(myData + mySize, newsize - mySize);
        mySize = newsize;
    }
    /// Alias of setSize().  setSize() is preferred.
    void            entries(exint newsize)
    {
        setSize(newsize);
    }
    /// Set the size, but unlike setSize(newsize), this function
    /// will not initialize new POD elements to zero. Non-POD data types
    /// will still have their constructors called.
    /// This function is faster than setSize(ne) if you intend to fill in
    /// data for all elements.
    void            setSizeNoInit(exint newsize)
    {
        if (newsize < 0)
            newsize = 0;
        if (newsize == mySize)
            return;
        setCapacityIfNeeded(newsize);
        if (mySize > newsize)
            trivialDestructRange(myData + newsize, mySize - newsize);
        else if (!isPOD()) // newsize > mySize
            trivialConstructRange(myData + mySize, newsize - mySize);
        mySize = newsize;
    }

    /// Decreases, but never expands, to the given maxsize.
    void            truncate(exint maxsize)
    {
        if (maxsize >= 0 && size() > maxsize)
            setSize(maxsize);
    }
    /// Resets list to an empty list.
    void            clear() {
        // Don't call setSize(0) since that would require a valid default
        // constructor.
        trivialDestructRange(myData, mySize);
        mySize = 0;
    }

    /// Assign array a to this array by copying each of a's elements with
    /// memcpy for POD types, and with copy construction for class types.
    UT_Array<T> &   operator=(const UT_Array<T> &a);

    /// Replace the contents with those from the initializer_list ilist
    UT_Array<T> &   operator=(std::initializer_list<T> ilist);

    /// Move the contents of array a to this array. 
    UT_Array<T> &   operator=(UT_Array<T> &&a);

    /// Compare two array and return true if they are equal and false otherwise.
    /// Two elements are checked against each other using operator '==' or
    /// compare() respectively.
    /// NOTE: The capacities of the arrays are not checked when
    ///       determining whether they are equal.
    bool            operator==(const UT_Array<T> &a) const;
    bool            operator!=(const UT_Array<T> &a) const;
    int             isEqual(const UT_Array<T> &a, Comparator compare) const;
     
    /// Subscript operator
    /// NOTE: This does NOT do any bounds checking unless paranoid
    ///       asserts are enabled.
    T &		    operator()(exint i)
    {
        UT_ASSERT_P(i >= 0 && i < mySize);
        return myData[i];
    }
    /// Const subscript operator
    /// NOTE: This does NOT do any bounds checking unless paranoid
    ///       asserts are enabled.
    const T &	    operator()(exint i) const
    {
	UT_ASSERT_P(i >= 0 && i < mySize);
	return myData[i];
    }

    /// Subscript operator
    /// NOTE: This does NOT do any bounds checking unless paranoid
    ///       asserts are enabled.
    T &		    operator[](exint i)
    {
        UT_ASSERT_P(i >= 0 && i < mySize);
        return myData[i];
    }
    /// Const subscript operator
    /// NOTE: This does NOT do any bounds checking unless paranoid
    ///       asserts are enabled.
    const T &	    operator[](exint i) const
    {
	UT_ASSERT_P(i >= 0 && i < mySize);
	return myData[i];
    }
    
    /// forcedRef(exint) will grow the array if necessary, initializing any
    /// new elements to zero for POD types and default constructing for
    /// class types.
    T &             forcedRef(exint i)
    {
        UT_ASSERT_P(i >= 0);
        if (i >= mySize)
            bumpSize(i+1);
        return myData[i];
    }

    /// forcedGet(exint) does NOT grow the array, and will return default
    /// objects for out of bound array indices.
    T               forcedGet(exint i) const
    {
        return (i >= 0 && i < mySize) ? myData[i] : T();
    }

    T &		    last()
    {
        UT_ASSERT_P(mySize);
        return myData[mySize-1];
    }
    const T &	    last() const
    {
        UT_ASSERT_P(mySize);
        return myData[mySize-1];
    }

    /// Apply a user-defined function to each element of the array
    /// as int as the function returns zero. If applyFct returns
    /// 1, apply() stops traversing the list and returns the current
    /// index; otherwise, apply() returns the size.
    exint	    apply(int (*applyFct)(T &t, void *d), void *d);

    template <typename BinaryOp>
    T		    accumulate(const T &init_value, BinaryOp add) const;

    T *		    getArray() const		    { return myData; }
    const T *	    getRawArray() const		    { return myData; }

    T *		    array()			    { return myData; }
    const T *	    array() const		    { return myData; }

    T *		    data()			    { return myData; }
    const T *	    data() const		    { return myData; }

    /// This method allows you to swap in a new raw T array, which must be
    /// the same size as myCapacity. Use caution with this method.
    T *		    aliasArray(T *newdata)
    { T *data = myData; myData = newdata; return data; }

    template <typename IT, bool FORWARD>
    class base_iterator : 
	public std::iterator<std::random_access_iterator_tag, T, exint> 
    {
        public:
	    typedef IT&		reference;
	    typedef IT*		pointer;
	
	    // Note: When we drop gcc 4.4 support and allow range-based for
	    // loops, we should also drop atEnd(), which means we can drop
	    // myEnd here.
	    base_iterator() : myCurrent(nullptr), myEnd(nullptr) {}
	    
	      // Allow iterator to const_iterator conversion
	    template<typename EIT>
	    base_iterator(const base_iterator<EIT, FORWARD> &src)
		: myCurrent(src.myCurrent), myEnd(src.myEnd) {}
	    
	    pointer	operator->() const 
			{ return FORWARD ? myCurrent : myCurrent - 1; }
	    
	    reference	operator*() const
			{ return FORWARD ? *myCurrent : myCurrent[-1]; }

	    reference	item() const
			{ return FORWARD ? *myCurrent : myCurrent[-1]; }
	    
	    reference	operator[](exint n) const
			{ return FORWARD ? myCurrent[n] : myCurrent[-n - 1]; } 

	    /// Pre-increment operator
            base_iterator &operator++()
			{
        		    if (FORWARD) ++myCurrent; else --myCurrent;
			    return *this;
			}
	    /// Post-increment operator
	    base_iterator operator++(int)
			{
			    base_iterator tmp = *this;
        		    if (FORWARD) ++myCurrent; else --myCurrent;
        		    return tmp;
			}
	    /// Pre-decrement operator
	    base_iterator &operator--()
			{
			    if (FORWARD) --myCurrent; else ++myCurrent;
			    return *this;
			}
	    /// Post-decrement operator
	    base_iterator operator--(int)
			{
			    base_iterator tmp = *this;
			    if (FORWARD) --myCurrent; else ++myCurrent;
			    return tmp;
			}

	    base_iterator &operator+=(exint n)   
			{
			    if (FORWARD)
				myCurrent += n;
			    else
				myCurrent -= n;
			    return *this;
			}
            base_iterator operator+(exint n) const
			{
			    if (FORWARD)
				return base_iterator(myCurrent + n, myEnd);
			    else
				return base_iterator(myCurrent - n, myEnd);
			}
	    
            base_iterator &operator-=(exint n)
        		{ return (*this) += (-n); }
            base_iterator operator-(exint n) const
			{ return (*this) + (-n); }
            
	    bool	 atEnd() const		{ return myCurrent == myEnd; }
	    void	 advance()		{ this->operator++(); }
	    
	    // Comparators
	    template<typename ITR, bool FR>
	    bool 	 operator==(const base_iterator<ITR, FR> &r) const
			 { return myCurrent == r.myCurrent; }
	    
	    template<typename ITR, bool FR>
	    bool 	 operator!=(const base_iterator<ITR, FR> &r) const
			 { return myCurrent != r.myCurrent; }
	    
	    template<typename ITR>
	    bool	 operator<(const base_iterator<ITR, FORWARD> &r) const
	    {
		if (FORWARD) 
		    return myCurrent < r.myCurrent;
		else
		    return r.myCurrent < myCurrent;
	    }
	    
	    template<typename ITR>
	    bool	 operator>(const base_iterator<ITR, FORWARD> &r) const
	    {
		if (FORWARD) 
		    return myCurrent > r.myCurrent;
		else
		    return r.myCurrent > myCurrent;
	    }

	    template<typename ITR>
	    bool	 operator<=(const base_iterator<ITR, FORWARD> &r) const
	    {
		if (FORWARD) 
		    return myCurrent <= r.myCurrent;
		else
		    return r.myCurrent <= myCurrent;
	    }

	    template<typename ITR>
	    bool	 operator>=(const base_iterator<ITR, FORWARD> &r) const
	    {
		if (FORWARD) 
		    return myCurrent >= r.myCurrent;
		else
		    return r.myCurrent >= myCurrent;
	    }
	    
	    // Difference operator for std::distance
	    template<typename ITR>
	    exint	 operator-(const base_iterator<ITR, FORWARD> &r) const
	    {
		if (FORWARD) 
		    return exint(myCurrent - r.myCurrent);
		else
		    return exint(r.myCurrent - myCurrent);
	    }
	    
	    
        protected:
	    friend class UT_Array<T>;
	    base_iterator(IT *c, IT *e) : myCurrent(c), myEnd(e) {}
	private:

	    IT			*myCurrent;
	    IT			*myEnd;
    };
    
    typedef base_iterator<T, true>		iterator;
    typedef base_iterator<const T, true>	const_iterator;
    typedef base_iterator<T, false>		reverse_iterator;
    typedef base_iterator<const T, false>	const_reverse_iterator;
    typedef const_iterator	traverser; // For backward compatibility

    /// Begin iterating over the array.  The contents of the array may be 
    /// modified during the traversal.
    iterator		begin()
			{
			    return iterator(myData, myData + mySize);
			}
    /// End iterator.
    iterator		end()
			{
			    return iterator(myData + mySize,
					    myData + mySize);
			}

    /// Begin iterating over the array.  The array may not be modified during
    /// the traversal.
    const_iterator	begin() const
			{
			    return const_iterator(myData, myData + mySize);
			}
    /// End const iterator.  Consider using it.atEnd() instead.
    const_iterator	end() const
			{
			    return const_iterator(myData + mySize,
						  myData + mySize);
			}
    
    /// Begin iterating over the array in reverse. 
    reverse_iterator	rbegin()
			{
			    return reverse_iterator(myData + mySize,
			                            myData);
			}
    /// End reverse iterator.
    reverse_iterator	rend()
			{
			    return reverse_iterator(myData, myData);
			}
    /// Begin iterating over the array in reverse. 
    const_reverse_iterator rbegin() const
			{
			    return const_reverse_iterator(myData + mySize,
							  myData);
			}
    /// End reverse iterator.  Consider using it.atEnd() instead.
    const_reverse_iterator rend() const
			{
			    return const_reverse_iterator(myData, myData);
			}
    
    UT_IteratorRange<iterator> range()
			{ return UT_IteratorRange<iterator>(begin(), end()); }
    UT_IteratorRange<const_iterator> range() const
			{ return UT_IteratorRange<const_iterator>(begin(), end()); }
    
    UT_IteratorRange<reverse_iterator> rrange()
			{ return UT_IteratorRange<reverse_iterator>(rbegin(), rend()); }
    UT_IteratorRange<const_reverse_iterator> rrange() const
			{ return UT_IteratorRange<const_reverse_iterator>(rbegin(), rend()); }

    /// Remove item specified by the reverse_iterator.
    void		removeItem(const reverse_iterator &it)
			{
			    removeAt(&it.item() - myData);
			}


    /// Very dangerous methods to share arrays.
    /// The array is not aware of the sharing, so ensure you clear
    /// out the array prior a destructor or setCapacity operation.
    void	    unsafeShareData(UT_Array<T> &src)
		    {
			myData = src.myData;
			myCapacity = src.myCapacity;
			mySize = src.mySize;
		    }
    void	    unsafeShareData(T *src, exint srcsize)
		    {
			myData = src;
			myCapacity = srcsize;
			mySize = srcsize;
		    }
    void	    unsafeShareData(T *src, exint size, exint capacity)
		    {
			myData = src;
			mySize = size;
			myCapacity = capacity;
		    }
    void	    unsafeClearData()
		    {
			myData = nullptr;
			myCapacity = 0;
			mySize = 0;
		    }

    /// Returns true if the data used by the array was allocated on the heap.
    inline bool	    isHeapBuffer() const
		    {
			return (myData != (T *)(((char*)this) + sizeof(*this)));
		    }
    inline bool	    isHeapBuffer(T* data) const
		    {
			return (data != (T *)(((char*)this) + sizeof(*this)));
		    }

protected:
    // Check whether T may have a constructor, destructor, or copy
    // constructor.  This test is conservative in that some POD types will
    // not be recognized as POD by this function. To mark your type as POD,
    // use the SYS_DECLARE_IS_POD() macro in SYS_TypeDecorate.h.
    static constexpr SYS_FORCE_INLINE bool isPOD()
    {
        return SYSisPOD<T>();
    }

    /// Implements both append(const T &) and append(T &&) via perfect
    /// forwarding. Unlike the variadic emplace_back(), its argument may be a
    /// reference to another element in the array.
    template <typename S>
    exint           appendImpl(S &&s);

    /// Similar to appendImpl() but for insertion.
    template <typename S>
    exint           insertImpl(S &&s, exint index);

    /// In debug builds, verifies that the arguments to emplace_back() will not
    /// be invalidated when realloc() is called.
    template <typename First, typename... Rest>
    void validateEmplaceArgs(First &&first, Rest&&... rest) const
    {
        UT_ASSERT_MSG_P(
            static_cast<const void *>(&first) <
		static_cast<const void *>(myData) ||
            static_cast<const void *>(&first) >=
		static_cast<const void *>(myData + mySize),
            "Argument cannot reference an existing element in the array.");

        validateEmplaceArgs(std::forward<Rest>(rest)...);
    }

    /// Base case for validateEmplaceArgs().
    void validateEmplaceArgs() const
    {
    }

    // Construct the given type
    template <typename... S>
    static void	    construct(T &dst, S&&... s)
		    {
                        new (&dst) T(std::forward<S>(s)...);
		    }

    // Copy construct the given type
    static void	    copyConstruct(T &dst, const T &src)
		    {
			if (isPOD())
			    dst = src;
			else
			    new (&dst) T(src);
		    }
    static void	    copyConstructRange(T *dst, const T *src, exint n)
		    {
			if (isPOD())
                        {
                            if (n > 0)
                            {
                                ::memcpy((void *)dst, (const void *)src,
                                         n * sizeof(T));
                            }
                        }
			else
			{
			    for (exint i = 0; i < n; i++)
				new (&dst[i]) T(src[i]);
			}
		    }

    /// Element Constructor
    static void	    trivialConstruct(T &dst)
		    {
			if (!isPOD())
			    new (&dst) T();
			else
			    memset((void *)&dst, 0, sizeof(T));
		    }
    static void	    trivialConstructRange(T *dst, exint n)
		    {
			if (!isPOD())
			{
			    for (exint i = 0; i < n; i++)
				new (&dst[i]) T();
			}
			else if (n == 1)
			{
			    // Special case for n == 1. If the size parameter
			    // passed to memset is known at compile time, this
			    // function call will be inlined. This results in
			    // much faster performance than a real memset
			    // function call which is required in the case
			    // below, where n is not known until runtime.
			    // This makes calls to append() much faster.
			    memset((void *)dst, 0, sizeof(T));
			}
			else
			    memset((void *)dst, 0, sizeof(T) * n);
		    }

    /// Element Destructor
    static SYS_FORCE_INLINE void trivialDestruct(T &dst)
		    {
			if (!isPOD())
			    dst.~T();
		    }
    static void	    trivialDestructRange(T *dst, exint n)
		    {
			if (!isPOD())
			{
			    for (exint i = 0; i < n; i++)
				dst[i].~T();
			}
		    }

private:
    /// Pointer to the array of elements of type T
    T *myData;

    /// The number of elements for which we have allocated memory
    exint myCapacity;

    /// The actual number of valid elements in the array
    exint mySize;

    // The guts of the remove() methods.
    exint	    removeAt(exint index);

    T *		    allocateCapacity(exint num_items);

    template<typename OS, typename S>
    friend OS &operator<<(OS &os, const UT_Array<S> &d);

    /// Friend specialization of std::swap() to use UT_String::swap()
    /// @internal This is needed because standard std::swap() implementations
    /// will try to copy the UT_String objects, causing hardened strings to
    /// become weak.
    friend void	swap(UT_Array<T>& a, UT_Array<T>& b) { a.swap(b); }
};

// Assigns src to dest, using the default C++ conversion operator.
template <typename T, typename S>
void
UTconvertArray(UT_Array<T> &dest, const UT_Array<S> &src)
{
    exint n = src.size();
    dest.setCapacity(n);
    dest.setSize(n);
    for (exint i = 0; i < n; i++)
	dest(i) = T(src(i));
}
template <typename T, typename S>
void
UTconvertArray(UT_Array<T> &dest, const S *src, exint n)
{
    dest.setCapacity(n);
    dest.setSize(n);
    for (exint i = 0; i < n; i++)
	dest(i) = T(src[i]);
}
template <typename T, typename S>
void
UTconvertArray(T *dest, const UT_Array<S> &src)
{
    // We assume here that dest has enough memory for src.size()
    exint n = src.size();
    for (exint i = 0; i < n; i++)
	dest[i] = T(src(i));
}
template <typename T, typename S>
void
UTconvertArray(T *dest, const S *src, int64 n)
{
    // We assume here that dest has enough memory for n elements
    for (int64 i = 0; i < n; i++)
	dest[i] = T(src[i]);
}

#include "UT_ArrayImpl.h"


template<typename OS, typename S>
inline OS &
operator<<(OS &os, const UT_Array<S> &d)
{
    os << "UT_Array" << UT_ContainerPrinter<UT_Array<S> >(d);
    return os;
}

// Overload for custom formatting of a UT_StringArray with UTformat.
template <typename T> UT_API size_t
format(char *buffer, size_t bufsize, const UT_Array<T> &v);

/// Unlike UT_Array::getMemoryUsage(), this also calls getMemoryUsage() on the
/// its elements
template <template <typename> class ArrayT, typename T>
static inline int64
UTarrayDeepMemoryUsage(const ArrayT<T> &arr, bool inclusive)
{
    int64 mem = inclusive ? sizeof(arr) : 0;
    mem += arr.getMemoryUsage(false);
    for (auto &&item : arr)
	mem += item.getMemoryUsage(false);
    return mem;
}

/// Utility to sort array using operator<
template <typename T>
static inline void
UTsort(UT_Array<T> &arr)
{
    arr.stdsort([](const T &a, const T &b) { return a < b; });
}

/// Utility to sort array using operator< and remove duplicates
template <typename T>
static inline void
UTsortAndRemoveDuplicates(UT_Array<T> &arr)
{
    arr.sortAndRemoveDuplicates([](const T &a, const T &b) { return a < b; });
}

// For UT::ArraySet.
namespace UT
{
template <typename T>
struct DefaultClearer;

template <typename T>
struct DefaultClearer<UT_Array<T>>
{
    static void clear(UT_Array<T> &v) { v.setCapacity(0); }
    static bool isClear(const UT_Array<T> &v) { return v.capacity() == 0; }
    static void clearConstruct(UT_Array<T> *p)
    {
        new ((void *)p) UT_Array<T>();
    }
    static const bool clearNeedsDestruction = false;
};
} // namespace UT

#endif // __UT_ARRAY_H_INCLUDED__
