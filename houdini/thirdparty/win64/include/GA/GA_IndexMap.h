/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_IndexMap.h ( GA Library, C++)
 *
 * COMMENTS:	The index map maintains a mapping of the ordered index of a
 *		geometric element to its data offset (and vice versa).
 *
 *		The index map also maintains the list of elements
 *
 */

#pragma once

#ifndef __GA_IndexMap__
#define __GA_IndexMap__

#include "GA_API.h"
#include "GA_Range.h"
#include "GA_Types.h"
#include "GA_OffsetList.h"

#include <UT/UT_Assert.h>
#include <UT/UT_NonCopyable.h>

#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_MemoryOrder.h>
#include <SYS/SYS_Types.h>


class GA_AIFStringTuple;
class GA_AIFTuple;
class GA_Attribute;
class GA_Defragment;
class GA_Detail;
class GA_MergeMap;
class GA_MergeOffsetMap;
class UT_MemoryCounter;


/// @brief A class to manage an ordered array which has fixed offset handles
///
/// The GA_Detail keeps an @b ordered list of elements (points, vertices etc.)
/// maintained by this class.  Each ordered element (named "index") also has a
/// fixed @b offset.  The offset remains constant as elements are added/removed
/// from the detail.  The offset is used for indexing into
/// * The attribute arrays
/// * Groups
/// * Primitive lists
/// etc.
/// Since the offset is not mutable, as elements are added and deleted,
/// vacancies become available in the attribute arrays.  These vacancies
/// may then be filled as new elements get allocated.
///
/// This class (GA_IndexMap) keeps track of the vacancies and the order of
/// elements.
///
/// During some operations, temporary elements are also created.  This class
/// maintains temporary elements too.
class GA_API GA_IndexMap : public UT_NonCopyable
{
public:
     GA_IndexMap(GA_Detail &detail, GA_AttributeOwner type);
    ~GA_IndexMap();

    /// Access the detail this index map belongs to
    GA_Detail		&getDetail() const  { return myDetail; }
    /// What type of element is stored in the index
    GA_AttributeOwner	getOwner() const    { return myOwner; }

    /// Report the memory used (includes all shared memory)
    int64	getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    void	countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// The capacity for offsets.  Attribute arrays must be able to store
    /// at least this much storage.
    SYS_FORCE_INLINE
    GA_Offset	getOffsetCapacity() const    { return myOffsetCapacity; }

    /// This is an exclusive upper bound when iterating over offsets.
    /// Every active or temporary offset in this index map will be
    /// strictly less than this.
    /// It is always guaranteed that:
    /// myMaxOccupiedOffset < offsetSize() <= getOffsetCapacity()
    SYS_FORCE_INLINE
    GA_Offset	offsetSize() const
    { return myIndexFromOffset.entries(); }

    /// Return number of elements in the list.
    /// This indexSize() is always <= the offsetSize()
    SYS_FORCE_INLINE
    GA_Index	indexSize() const
    { return myIndexCount; }

    /// When accessing the map functions (i.e. mapping from order to data
    /// offsets), the map may be compacted.  This is a potentially expensive
    /// operation and should be avoided if possible.

    /// Given an ordered index, this returns the fixed @b offset associated
    /// with the index.
    /// @b NOTE: Calling the offsetFromIndex() function may invoke internal data
    /// modifications (i.e. these operations may be mutable).  These
    /// operations are potentially expensive and should be avoided if possible.
    /// @see findOffsetInIndices(), findNextOffsetInIndices()
    SYS_FORCE_INLINE
    GA_Offset	offsetFromIndex(GA_Index ordered_index) const
    {
        UT_ASSERT_P(GAisValid(ordered_index) &&
                    ordered_index < myIndexCount);

        // If the list is trivial, it will never need compacting,
        // so we avoid checking myFirstFreeIndex.  operator() checks
        // isTrivial() anyway, and the compiler seems to avoid checking
        // it twice.
        if (myOffsetFromIndex.isTrivial())
            return myOffsetFromIndex(ordered_index);

        // NOTE: We could avoid compactIndices() when
        //       ordered_index < firstFreeIndex, but we would still have to
        //       at least read-lock when accessing myOffsetFromIndex,
        //       since compactIndices calls changeSize on myOffsetFromIndex.
        compactIndicesIfNeeded();
        UT_ASSERT_P(ordered_index < myOffsetFromIndex.entries());

        return myOffsetFromIndex(ordered_index);
    }

    /// Given an element offset, this returns the ordered @b index.
    /// @b NOTE: Calling the indexFromOffset() function may invoke internal data
    /// modifications (i.e. these operations may be mutable).  These
    /// operations are potentially expensive and should be avoided if possible.
    /// @see findIndexInOffsets()
    SYS_FORCE_INLINE
    GA_Index	indexFromOffset(GA_Offset data_offset) const
    {
        UT_ASSERT_P(GAisValid(data_offset) &&
                    data_offset <= myMaxOccupiedOffset);

        // If the list is trivial, it will never need compacting,
        // so we avoid checking myFirstFreeIndex.  operator() checks
        // isTrivial() anyway, and the compiler seems to avoid checking
        // it twice.
        if (myIndexFromOffset.isTrivial())
            return myIndexFromOffset(data_offset);

        // If we have holes in the *indices*, we need to compactIndices()
        // NOTE: SYS_MEMORY_ORDER_LOAD is to force no speculative reads
        //       in later code occurring before the call to load.
        GA_Index firstFreeIndex = myFirstFreeIndex.load(SYS_MEMORY_ORDER_LOAD);
        // NOTE: It is safe to access myIndexFromOffset, because it doesn't
        //       get resized by compactIndices().
        GA_Index index = myIndexFromOffset(data_offset);
        if (GAisValid(firstFreeIndex) && index >= firstFreeIndex)
        {
            compactIndices();
            index = myIndexFromOffset(data_offset);
        }

        UT_ASSERT_P(GAisValid(index));
        return index;
    }

    /// Obtain offset of the element with the last index.
    /// Returns GA_INVALID_OFFSET if empty.
    /// NOTE: The offset will never be a temporary element, since
    ///       temporary elements have no indices.
    SYS_FORCE_INLINE
    GA_Offset	lastOffset() const
    {
        if (isMonotonicMap() && myTempCount == 0)
        {
            // NOTE: Since there are no temporaries and the map is monotonic,
            //       the maximum occupied offset is the offset of the element
            //       with the last index.
            return myMaxOccupiedOffset;
        }

        UT_ASSERT_MSG(isMonotonicMap(),
                      "Why is GA_IndexMap::lastOffset() being called between"
                      "sorting and defragmenting the offsets? It's almost"
                      "certainly a recipe for disaster.");
        GA_Index n = indexSize();
	if (n > 0)
	    return offsetFromIndex(n - 1);
	else
	    return GA_INVALID_OFFSET;
    }

    /// This is a helpful class for getting the range of elements
    /// created after such a Marker is declared.  For example,
    /// 
    /// GA_IndexMap::Marker marker(gdp->getPointMap());
    /// ... // Some code that creates points
    /// for (GA_Iterator it(marker.getRange()); !it.atEnd(); ++it)
    ///     ... // Do something to each created point
    /// 
    /// NOTE: The code doing the creating can't delete elements from
    ///       the index map before adding any new elements.
    class Marker
    {
    public:
        Marker(const GA_IndexMap &indexmap)
            : myIndexMap(indexmap)
            , myBegin(getOffset())
        {}

        /// Returns a GA_Range of (non-temporary) elements
        /// added after the creation of this Marker, so long as
        /// no elements that were already in the GA_IndexMap
        /// were deleted before adding any new elements.
        /// All non-temporary elements between getBegin() and
        /// getEnd() are in the range.
        GA_Range getRange() const
        { return GA_Range(myIndexMap, getBegin(), getEnd()); }

        /// Returns the lower-bound on the beginning of the range
        GA_Offset getBegin() const { return myBegin; }
        /// Returns the upper-bound on the end of the range
        GA_Offset getEnd() const { return getOffset(); }

    private:
        GA_Offset getOffset() const
        { return myIndexMap.lastOffset() + 1; }

    private:
        const GA_IndexMap &myIndexMap;
        const GA_Offset myBegin;
    };

    /// Returns a copy of our offset from index list, first ensuring
    /// it is up to date.  Note this is a copy so will not remain
    /// in sync.
    GA_OffsetList getOffsetFromIndexList() const
    {
        compactIndicesIfNeeded();
        // NOTE: This returns a copy, though if the GA_OffsetList is non-trivial,
        //       the copy will reference the internal list.
	return *reinterpret_cast<GA_OffsetList *>(&myOffsetFromIndex);
    }

    /// Given an ordered index, this returns the fixed @b offset associated
    /// with the index.  This method may do an O(N) search for the result, but
    /// is guaranteed to be const.
    /// It's primarily designed for use in GA_RTIIndex.
    GA_Offset	findOffsetInIndices(GA_Index ordered_index) const;
    /// Given an offset, this skips the @b step ordered index and returns the
    /// next fixed @b offset.  This method may do an O(N) search for the
    /// result, but is guaranteed to be const.
    /// It's primarily designed for use in GA_RTIIndex.
    GA_Offset	findNextOffsetInIndices(GA_Offset data_offset,
					  GA_Index step) const;

    /// Non-compacting method to find the true order of the given data_offset.
    /// This method may need to search the non-compact mapping for holes, and
    /// so sequential calls can easily result in O(n^2) behaviour.
    GA_Index	findIndexInOffsets(GA_Offset data_offset) const;

    /// Reorder an element.  The index of the element at the given data offset
    /// will be changed to the new order (provided that the ordered position is
    /// in a valid range).
    /// @return The new index of the element, or -1 if there was an error
    GA_Index	reorder(GA_Offset data_offset, GA_Index new_order);

    /// @{
    /// Swap the order of the two specified (ordered, not transient) data
    /// offsets.
    /// @warning You @b must force defragmentation after swapping the index
    ///    order to maintain monotonicity of the index map.
    /// @return True on success, false on failure.
    bool	swapOrderByOffset(GA_Offset offset1, GA_Offset offset2);
    bool	swapOrderByIndex(GA_Index order1, GA_Index order2);
    /// @}

    /// A trivial map is one where offsetFromIndex(i) == i for all elements.
    /// That is, the offsets are in order and there are no vacancies.
    SYS_FORCE_INLINE
    bool	isTrivialMap() const
    {
        GA_Size noff = offsetSize();
        GA_Size nind = indexSize();
        if (nind != noff)
            return false;
        // NOTE: We don't need to check myFirstFreeIndex, because
        //       if it's valid, offsetSize() > indexSize().
        // NOTE: It *must* be threadsafe to call isTrivial() and
        //       compactIndices() at the same time, and compactIndices() calls
        //       GA_OffsetList::changeSize() on a non-trivial list, so
        //       GA_OffsetList::isTrivial() *must* return false, no matter
        //       what goes on in GA_OffsetList::changeSize().
        return myOffsetFromIndex.isTrivial();
    }

    SYS_FORCE_INLINE
    bool	isMonotonicMap() const
    {
        return myMonotonic;
    }

    /// Class which compares the elements at two indices
    /// Example: @code
    ///    class SortReverse : public GA_IndexMap::IndexCompare
    ///	   {
    ///            SortReverse() {}
    ///            ~SortReverse() {}
    ///            bool    initialize(const GA_IndexMap &map)
    ///                    {
    ///                        myOrder = new GA_Index[map.offsetSize()];
    ///                        for (GA_Iterator it(GA_Range(map)); !it.atEnd();
    ///                             ++it)
    ///                        {
    ///                            myOrder[it.getOffset()] =
    ///                                map.indexFromOffset(it.getOffset());
    ///                        }
    ///                    }
    ///            int     compare(const GA_IndexMap&, GA_Offset a, GA_Offset b)
    ///                    {
    ///                        if (myOrder[a] > myOrder[b])
    ///                            return -1;
    ///                        if (myOrder[a] < myOrder[b])
    ///                            return 1;
    ///                        return 0;
    ///                    }
    ///            void    finish(const GA_IndexMap &)
    ///                    {
    ///                        delete [] myOrder;
    ///                    }
    ///    };
    /// @endcode
    class GA_API IndexCompare 
    {
	public:
		     IndexCompare() {}
	    virtual ~IndexCompare() {}

	    /// Called prior to the sort.  If the method returns false, the
	    /// sort is aborted.  If the sort is aborted, the finish() method
	    /// will @b not be called.
	    virtual bool	initialize(const GA_IndexMap &map) = 0;

	    /// Compare two elements using strcmp() semantics
	    virtual int		compare(const GA_IndexMap &map,
					GA_Offset item1,
					GA_Offset item2) = 0;

	    /// Called after the sort is complete.  At this point, the index
	    /// will be ordered according to the sort.
	    virtual void	finish(const GA_IndexMap &map) = 0;
    };

    /// Convenience class which can be used to sort an index map based on an
    /// attribute value.  The attribute should either provide an AIFTuple or
    /// AIFStringTuple interface.  If neither interfaces are provided, this
    /// class won't do any sorting.
    /// Example: @code
    ///		GA_Detail	&gdp;
    ///		GA_IndexMap	&points = gdp.getIndexMap(GA_ATTRIB_POINT);
    ///
    ///		// Sort by P.z
    ///		GA_IndexMap::AttributeCompare	cmpPz(points, "P", 2);
    ///		points.sortIndices(cmpPz);
    /// @endcode
    class GA_API AttributeCompare : public IndexCompare 
    {
	public:
	    /// Sort the index map based on the values in the attribute.  For
	    /// tuple attributes, compare the value for the given index.
	    AttributeCompare(const GA_Attribute *attribute, int tuple_index=0);
	    /// Construct given an index map and an attribute name
	    AttributeCompare(const GA_IndexMap &map,
			const char *attribute_name,
			int tuple_index=0);
	    /// Destructor
	    virtual ~AttributeCompare();

	    /// @{
	    /// Methods defined on IndexCompare
	    virtual bool	initialize(const GA_IndexMap &map);
	    virtual int		compare(const GA_IndexMap &map,
					GA_Offset item1, GA_Offset item2);
	    virtual void	finish(const GA_IndexMap &map);
	    /// @}

	protected:
	    const GA_Attribute		*getAttribute() const
					    { return myAttribute; }
	    const GA_AIFTuple		*getAIFTuple() const
					    { return myTuple; }
	    const GA_AIFStringTuple	*getAIFStringTuple() const
					    { return mySTuple; }
	    int				getTupleIndex() const
					    { return myIndex; }
	    bool			getIsFloat() const
					    { return myFloat; }
	private:
	    const GA_Attribute		*myAttribute;
	    const GA_AIFTuple		*myTuple;
	    const GA_AIFStringTuple	*mySTuple;
	    int				 myIndex;
	    bool			 myFloat;
    };

    /// Sort the index order using a comparator
    bool	sortIndices(IndexCompare &compare, bool stable=true);

    /// Sort a selection of elements
    bool	sortIndices(IndexCompare &compare, const GA_Range &range,
			bool stable=true);

    /// Sort an array of GA_Offset data according to a comparator
    bool	sortOffsetArray(GA_OffsetArray &offsets, IndexCompare &compare,
			bool stable=true) const;

    /// @{
    /// Shift/cycle elements in the list.
    bool	cycleIndices(GA_Size offset);
    bool	cycleIndices(GA_Size offset, const GA_Range &range);
    /// @}

    /// @{
    /// Reverse a selection of elements
    bool	reverseIndices();
    bool	reverseIndices(const GA_Range &range);
    /// @}

    //------------------------------------------------------------
    // List management
    /// Add a new element to the list, returning the data offset of the new
    /// element.
    GA_Offset	addElement()	{ return addElementBlock(1); }
    /// Add new elements to the list, returning the first offset of the
    /// contiguous block
    GA_Offset	addElementBlock(GA_Size nelements);
    /// Returns the start offset that the next call to addElementBlock
    /// would return, (assuming it doesn't exceed the hard limit.)
    GA_Offset   nextNewElementOffset() const;
    /// Delete an element from the list by specifying the order
    void	destroyIndex(GA_Index where);
    /// Delete an element from the list by the data offset
    void	destroyOffset(GA_Offset where);

    /// Returns whether the offset is in the valid range of offsets for this
    /// index map.
    bool	isOffsetInRange(GA_Offset offset) const
		    { return GAisValid(offset) && offset < offsetSize(); }

    /// Returns true if the specified offset is not in use (i.e. vacant)
    /// NOTE: Inactive != Vacant.  There are temporaries.
    bool	isOffsetVacant(GA_Offset offset) const;

    /// Returns true if the specified offset is referenced by an ordered element
    /// No bounds checking is done.
    bool	isOffsetActiveFast(GA_Offset offset) const
		    { return GAisValid(myIndexFromOffset(offset)); }


    /// Returns the first offset in [start, end) which is inactive.
    /// If none are inactive, return end.
    /// No bounds checking is done.
    SYS_FORCE_INLINE
    GA_Offset   findInactiveOffset(GA_Offset start, GA_Offset end) const
	    { return myIndexFromOffset.findInvalidInRange(start, end); }
    /// Returns the first offset in [start, end) which is active
    /// If none are vacant, return end.
    /// No bounds checking is done.
    SYS_FORCE_INLINE
    GA_Offset   findActiveOffset(GA_Offset start, GA_Offset end) const
	    { return myIndexFromOffset.findValidInRange(start, end); }

    /// Returns true if the specified offset is referenced by an ordered element
    bool	isOffsetActive(GA_Offset offset) const
		    { return isOffsetInRange(offset) &&
			     isOffsetActiveFast(offset); }

    /// Returns true if the specified offset is being used for a temporary
    /// element.
    /// @see GA_WorkVertexBuffer
    bool	isOffsetTransient(GA_Offset offset) const;

    /// Calls functor on every active offset in this index map.
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachOffset(FUNCTOR &&functor) const
    {
        if (isTrivialMap())
        {
            const GA_Offset end = GA_Offset(GA_Size(myIndexCount));
            for (GA_Offset off(0); off != end; ++off)
            {
                functor(off);
            }
        }
        else
        {
            const GA_Offset veryend(myMaxOccupiedOffset+1);
            GA_Offset off(0);
            while (true)
            {
                off = findActiveOffset(off, veryend);
                GA_Offset end = findInactiveOffset(off, veryend);
                if (off == end)
                    break;
                do
                {
                    functor(off);
                    ++off;
                } while (off != end);
            }
        }
    }

    /// Calls functor on every active offset in this index map,
    /// until functor returns false for some element.
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachOffsetBreak(FUNCTOR &&functor) const
    {
        if (isTrivialMap())
        {
            const GA_Offset end = GA_Offset(GA_Size(myIndexCount));
            for (GA_Offset off(0); off != end; ++off)
            {
                if (!functor(off))
                    return;
            }
        }
        else
        {
            const GA_Offset veryend(myMaxOccupiedOffset+1);
            GA_Offset off(0);
            while (true)
            {
                off = findActiveOffset(off, veryend);
                GA_Offset end = findInactiveOffset(off, veryend);
                if (off == end)
                    break;
                do
                {
                    if (!functor(off))
                        return;
                    ++off;
                } while (off != end);
            }
        }
    }

    /// @private Clear the index map
    /// @param	clear_capacity	If true, the offset capacity value 
    ///				is reset to zero. Else, it is left unchanged.
    void	clear(bool clear_capacity);

    /// @private Allocate a temporary element
    /// @see GA_WorkVertexBuffer
    GA_Offset	addTemporary();

    /// @private
    /// Query the number of temporary elements
    GA_Size	getTempCount() const	{ return myTempCount; }

    /// @private Method called by detail during merge operation
    void	merge(GA_MergeMap &map);
    /// @private Method called to initialize the merge offset maps
    void	mergeInitOffsetMap(GA_MergeMap &map, GA_MergeOffsetMap &omap);

    /// @private Used by GA_RTIOffset to skip over vacancies.  Skips to first
    /// non-vacant/non-temporary element.
    void	skipToFirstActive(GA_Offset &curr, GA_Offset end,
			bool fwd=true) const;
    /// @private Used by GA_RTIOffset to extract a contiguous range
    /// of offsets.  Stops at first vacant or temporary element.
    void	extractActiveRange(GA_Offset &curr, GA_Offset end,
			bool fwd=true) const;

    /// @private Used by GA_RTIOffsetComplete to skip over vacancies (but
    /// including temporaries).
    void	skipToFirstOccupied(GA_Offset &curr, GA_Offset end) const;
    /// @private Used by GA_RTOffsetComplete to extract a contiguous range of
    /// occupied (includes active and temporaries).
    void	extractOccupiedRange(GA_Offset &curr, GA_Offset end) const;

    /// @private Used by GA_RTIIndex to extract a contiguous ordered range of
    /// offsets.  Stops at first vacant, temporary, or out of order element.
    void	extractActiveOrderedRange(GA_Offset &curr, GA_Offset end) const;

    /// @private - Used only by GA_Detail
    void	defragment(const GA_Defragment &defrag);

    /// @private - Possibly decrease the getOffsetCapacity()/offsetSize() due to
    /// deletions
    void	shrinkOffsets();

    /// @private - defragment to make this a trivial map
    void	makeMonotonic();

    /// @private - access to internal offset from index array used in
    /// defragmenting.
    const GA_ListType<GA_Offset, GA_Index> &getIndexFromOffsetMap() const
						{ return myIndexFromOffset; }

    /// Returns true iff the index maps are trivial and equal, or
    /// non-trivial and share the exact same data.  This does not
    /// fully check for equality!
    bool        isSame(const GA_IndexMap &that) const
    {
        if (this == &that)
            return true;

        // This shouldn't be called while defragmenting or sorting
        UT_ASSERT_P(myMonotonic && that.myMonotonic);

        // Detail index maps are all the same
        if (myOwner == GA_ATTRIB_DETAIL)
        {
            UT_ASSERT_P(myIndexCount == 1);
            return (that.myOwner == GA_ATTRIB_DETAIL);
        }

        // Check index count first, for speedy false,
        // then check for same index->offset, next most likely false,
        // then change count, and everything else.
        // Assume false if indices not compacted or temporaries present,
        // for simplicity, and we also don't need to check offset->index
        // that way.
        // NOTE: For some reason, detail index maps may have different
        //       change counts, even though they're always the same.
        return (myIndexCount == that.myIndexCount) &&
            myOffsetFromIndex.isSame(that.myOffsetFromIndex) &&
            myOwner == that.myOwner &&
            myOffsetCapacity == that.myOffsetCapacity &&
            myFreeHead == that.myFreeHead &&
            myFreeTrailerBase == that.myFreeTrailerBase &&
            myMaxOccupiedOffset == that.myMaxOccupiedOffset &&
            myMaxTouchedOffset == that.myMaxTouchedOffset &&
            myFirstFreeIndex.load(SYS_MEMORY_ORDER_LOAD) == GA_INVALID_INDEX && that.myFirstFreeIndex.load(SYS_MEMORY_ORDER_LOAD) == GA_INVALID_INDEX &&
            myTempCount == 0 && that.myTempCount == 0;
    }

    /// This is used as a quick check in defragmentation, in case
    /// this index map is actually defragmented, but not trivial
    /// for some reason.
    void checkTrivial()
    {
        if (!isTrivialMap() &&
            GA_Size(indexSize()) == GA_Size(offsetSize()) &&
            isMonotonicMap())
        {
            myIndexFromOffset.setTrivial(GA_Index(0), offsetSize());
            myOffsetFromIndex.setTrivial(GA_Offset(0), indexSize());
        }
    }

private:
    /// @private Used by merging to copy the data from the source
    void	copyFrom(const GA_IndexMap &src);

    /// Compact the ordered list.  This removes any temporary vacancies in the
    /// ordered list.
    void	compactIndices() const;

    /// Compact the ordered list if necessary, and avoid locking unless
    /// compacting is necessary.
    void	compactIndicesIfNeeded() const
    {
        // If we have holes in the *indices*, we need to compactIndices()
        // NOTE: SYS_MEMORY_ORDER_LOAD is to force no speculative reads
        //       in later code occurring before the call to load.
        GA_Index firstFreeIndex(myFirstFreeIndex.load(SYS_MEMORY_ORDER_LOAD));
        if (GAisValid(firstFreeIndex))
            compactIndices();
        UT_ASSERT_P(GA_Index(myFirstFreeIndex.relaxedLoad()) == GA_INVALID_INDEX);
    }

    /// Compact the ordered list if necessary, and avoid locking unless
    /// compacting is necessary.
    void	compactIndicesIfNeeded()
    {
        // If we have holes in the *indices*, we need to compactIndices()
        // NOTE: Since we're non-const, we can use relaxedLoad().
        // WARNING: If this function is ever made non-private, consider
        //          changing this to load(SYS_MEMORY_ORDER_LOAD), since
        //          we may get a non-const GA_IndexMap from a detail that
        //          we have multiple threads writing to.
        GA_Index firstFreeIndex(myFirstFreeIndex.relaxedLoad());
        if (GAisValid(firstFreeIndex))
            compactIndices();
        UT_ASSERT_P(GA_Index(myFirstFreeIndex.relaxedLoad()) == GA_INVALID_INDEX);
    }

    /// Prune any entries in the free list greater than or equal to the
    /// supplied upper bound.
    void	pruneFreeList(GA_Offset upper_bound);

    /// Element initialization code in common between addTemporary
    /// and addElementBlock
    void        initElementBlock(GA_Offset startoff, GA_Offset nelements);

    /// Delete the element at the given offset
    void	destroyElement(GA_Offset offset);

    /// Compute whether we're trivial or not.  If we are trivial, then this
    /// operation may eliminate storage for maps.
    void	computeTrivial();

    /// Compute whether the map is monotonic or not
    bool	computeMonotonic();

private:
    /// Owner of this index map
    GA_Detail		&myDetail;

    /// Index->Offset map
    mutable GA_OffsetListType<GA_Index>		myOffsetFromIndex;

    /// Offset->Index map
    mutable GA_ListType<GA_Offset, GA_Index>	myIndexFromOffset;

    /// Capacity of attributes in myDetail.  This can be larger than
    /// myIndexFromOffset.entries(), and gets bumpAlloc'd to avoid having
    /// to set the capacity of a large number of attributes too often.
    GA_Offset		 myOffsetCapacity;

    /// This is the offset of the head of the linked list of free offsets
    /// for use when adding temporary elements.
    GA_Offset		 myFreeHead;

    /// myFreeTrailerBase is used to track a block of free offsets at the end
    /// of the offsets.  It may not be optimal (i.e. it may not be
    /// myMaxOccupiedOffset+1) if there are offsets stuck in the linked-list
    /// after myMaxOccupiedOffset.  It can take O(n) time to check for them,
    /// so we don't always when deleting.
    GA_Offset		 myFreeTrailerBase;

    /// myMaxOccupiedOffset is the largest offset that is used, whether by
    /// an ordered or temporary element.
    GA_Offset		 myMaxOccupiedOffset;

    /// myMaxTouchedOffset is the largest offset that has, at some point,
    /// been used, but it is always strictly less than myOffsetCapacity.
    GA_Offset		 myMaxTouchedOffset;

    /// When an element is deleted, instead of shifting down all of
    /// myOffsetFromIndex immediately, we wait until someone queries it before
    /// calling compactIndices().  If nothing has been deleted, myFirstFreeIndex
    /// is GA_INVALID_INDEX.  Otherwise, it refers to the lowest (former) index
    /// of a freed element.
    mutable SYS_AtomicCounter myFirstFreeIndex;

    /// The number of temporary elements in the index map.
    GA_Size		 myTempCount;

    /// The true count of indices, i.e. myOffsetFromIndex.entries() minus
    /// the number of holes.
    GA_Index		 myIndexCount;

    /// The type of element this index map is for (point, vertex, prim, detail)
    GA_AttributeOwner	 myOwner;

    /// This is true if the offset corresponding with any index is always
    /// greater than the offset corresponding with a lower index, i.e.
    /// if the offsets are monotone increasing.
    /// The ONLY case where this is false is immediately before defragmentation
    /// inside of a GA_IndexMap function, so users of GA_IndexMap, apart from
    /// the GA_Detail::defragment and GA_Defragment code, *can* rely on the
    /// index map being monotone increasing.
    bool		 myMonotonic;

    /// NOTE: These are friends so that they can call compactIndicesIfNeeded.
    friend class GA_Detail;
    friend class GU_DetailHandleRef;

    /// NOTE: This friend is so that GA_AttributeSet::replace can call copyFrom.
    friend class GA_AttributeSet;
};

#endif
