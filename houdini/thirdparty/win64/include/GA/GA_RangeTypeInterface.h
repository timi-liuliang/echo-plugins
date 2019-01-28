/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RangeTypeInterface.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_RangeTypeInterface__
#define __GA_RangeTypeInterface__

#include "GA_API.h"
#include "GA_Types.h"

#include <SYS/SYS_AtomicInt.h>

#include <stddef.h>


class GA_IndexMap;
class GA_IteratorState;
class GA_Range;
class GA_RangeMemberQuery;


#define GA_SPLIT_GRANULARITY	GA_PAGE_SIZE

/// @brief Abstract implementation of a range
///
/// A range specifies a selection/collection of elements in an index map.
/// The range also provides an iterator over its elements.
///
/// With operation on ranges, the user may want to parallelize operations on
/// the range.  This is done by splitting the range into multiple new range.
/// Not all ranges can be split.
///
/// @see GA_Range
class GA_API GA_RangeTypeInterface 
{
public:
	     GA_RangeTypeInterface(const GA_IndexMap &list);
	     GA_RangeTypeInterface(const GA_RangeTypeInterface &other);
    virtual ~GA_RangeTypeInterface();

    /// Query type of element
    GA_AttributeOwner	getOwner() const;

    /// Convenience method to test whether the range is divisible
    bool	is_divisible() const
		{
		    return isSplittable() && getPageCount() > 1;
		}

    /// Method to create a copy of this selection
    virtual GA_RangeTypeInterface	*copy() const = 0;

    /// Split into two existing ranges
    bool		splitIntoRanges(GA_Range &r1, GA_Range &r2) const;

    /// Split the range into 2 separate ranges.
    /// @note the split ranges @b must contain separate pages of offsets.  That
    /// is a single page cannot be split.
    virtual bool	split(GA_RangeTypeInterface *list[2]) const = 0;

    /// Return whether the RTI is capable of splitting.  If this method returns
    /// true, the range should be able to:
    ///  - Perform a binary split into two ranges if there are more than one
    ///    page of data in the range.
    ///  - Create an iterator which will iterate over the elements in a given
    ///    range of pages.
    ///
    /// For a range to be splittable, you need to implement the following four
    /// methods:
    ///	  - split()
    ///	  - getPageCount()
    ///	  - getPageElementRange()
    ///	  - getFirstOffsetInPage()
    /// @ref HDK_GA_Using_Parallel
    virtual bool	isSplittable() const = 0;

    /// For splittable ranges, return the number of pages contained in the range
    virtual GA_Size	getPageCount() const = 0;

    /// If a page is splittable, this method returns an iterator for the
    /// elements in a set of @c npages pages.
    /// The @c relative_page passed in will be between 0 and @c getPageCount()
    /// and should be considered a relative page in the range.
    /// For non-splittable ranges, you can implement the method using @code
    ///   virtual bool getPageElementRange(GA_Range &, GA_Size, GA_Size) const
    ///                      { return getPageElementRangeNonSplittable(); }
    /// @endcode
    virtual bool	getPageElementRange(GA_Range &range,
				GA_Size relative_page,
				GA_Size npages=1) const = 0;

    /// If a page is splittable, this method must return the first offset in
    /// the given page.
    /// The @c relative_page passed in will be between 0 and @c getPageCount()
    /// and should be considered a relative page in the range.
    /// For non-splittable ranges, you can implement the method using @code
    ///   virtual GA_Range getFirstOffsetInPage(GA_Size, GA_Size) const
    ///                      { return getFirstOffsetInPageNonSplittable(); }
    /// @endcode
    virtual GA_Offset	getFirstOffsetInPage(GA_Size relative_page) const = 0;

    /// Check whether the range is empty
    virtual bool	isEmpty() const = 0;

    /// @{
    /// Compare two ranges
    virtual bool	isEqual(const GA_RangeTypeInterface &src) const = 0;
    bool		operator==(const GA_RangeTypeInterface &src) const
			    {
				return ((&getIndexMap() == &src.getIndexMap())
					    && isEqual(src));
			    }
    /// @}

    /// Get an upper bound on the size of the range
    virtual GA_Size	 getMaxEntries() const = 0;

    /// Get an exact count of the size of the range
    /// @see computeEntries()
    virtual GA_Size	 getEntries() const = 0;

    /// @{
    /// Construct/destruct the iterator state.
    virtual void	iterateCreate(GA_IteratorState &state) const { }
    virtual void	iterateDestroy(GA_IteratorState &state) const { }
    /// @}

    /// Copy iterator state.  The dest state will @b not have been constructed.
    virtual void	iterateCopy(GA_IteratorState &dest,
				    const GA_IteratorState &src) const = 0;

    /// Rewind the iterator (i.e. reset the iterator state).  The start/offset
    /// should be initialized so that they define the first contiguous "block"
    /// for iteration.  If the range is empty, @c start should be greater than
    /// @c end.  No "block" is allowed to cross a page boundary.
    ///
    /// The semantics are that @c start is in the range and @end is one larger
    /// than the last element in the range.  For example, @code
    ///    start := 6
    ///    end   := 9
    ///    elements := [6, 7, 8]
    /// @endcode
    virtual void	iterateRewind(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const = 0;

    /// Choose the next contiguous range.  If the iteration is complete,
    /// @c start should be set to a value greater than @c end.  No "block"
    /// is allowed to cross a page boundary.
    virtual void	iterateNext(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const = 0;

    /// Query if the RTI is ordered according to the index.
    virtual bool	isOrdered() const;

    /// Query if it is safe to delete an element during traversal.  The method
    /// defaults to false.
    virtual bool	areTraversalDeletionsSafe() const;

    /// Query if the RTI contains duplicates. If it cannot, then optimizations
    /// can be made assuming that there are no duplicates in the range. This
    /// defaults to true.
    virtual bool	canContainDuplicates() const	{ return true; }

    /// Allocate an object to allow us to query whether individual elements
    /// are traversed by this range implementation.  The responsibility for
    /// deleting the resulting object lies with the caller.
    ///
    /// The default implementation will work for all derived classes, but
    /// more efficient implementations are often possible.
    virtual const GA_RangeMemberQuery	*allocMemberQuery() const;

    /// Accessor for the index map
    const GA_IndexMap	&getIndexMap() const { return *myIndexMap; }

    /// @{
    /// Reference tracking for RTI's, necessary for GA_Range.
    void		 incref() const;
    void		 decref() const;
    /// @}

public:
    /// @{
    /// Memory tracking for RTI's
    // In place constructor
    static void	*operator	new(size_t size, void *p)	{ return p; }
    static void	*operator	new(size_t size);
    static void	 operator	delete(void *p, size_t size);
    /// @}

protected:
    /// Method which can be called for getPageElementRange()
    bool	getPageElementRangeNonSplittable() const;

    /// Method which can be called for getFirstOffsetInPage()
    GA_Offset	getFirstOffsetInPageNonSplittable() const;

    /// This method creates an iterator and counts the entries.
    GA_Size	computeEntries() const;

private:
    const GA_IndexMap *myIndexMap;

    mutable SYS_AtomicInt32	 myRefCount;

    // GA_RTIFiltered is a friend to be able to access the single iteration
    // methods for its contained filter.
    //friend class	GA_RTIFiltered;
};
#endif
