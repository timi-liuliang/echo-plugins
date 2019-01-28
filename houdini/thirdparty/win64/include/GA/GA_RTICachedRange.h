/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTICachedRange.h ( GA Library, C++)
 *
 * COMMENTS:	This is an RTI class that caches the data offsets visited by
 *		another RTI to allow traversal when it would not otherwise
 *		be safe with the original RTI.
 */

#ifndef __GA_RTICachedRange__
#define __GA_RTICachedRange__

#include "GA_API.h"
#include "GA_OffsetList.h"
#include "GA_RTISingle.h"
#include "GA_Types.h"


class GA_IteratorState;
class GA_Range;
class GA_RangeTypeInterface;


/// @brief Cache a given range for later traversal
class GA_API GA_RTICachedRange : public GA_RTISingle 
{
public:
    /// Iterate over the source range but in order.
    GA_RTICachedRange(const GA_RangeTypeInterface &range_to_cache);

    // Copy c-tor
    GA_RTICachedRange(const GA_RTICachedRange &src);

    virtual ~GA_RTICachedRange();

    /// @{
    /// RangeTypeInterface implementation
    virtual GA_RangeTypeInterface	*copy() const;
    virtual bool	isSplittable() const	{ return false; }
    virtual bool	isEmpty() const;
    virtual GA_Size	getMaxEntries() const;
    virtual GA_Size	getEntries() const;
    virtual bool	split(GA_RangeTypeInterface *list[2]) const;
    virtual bool	isEqual(const GA_RangeTypeInterface &src) const;
    virtual void	iterateCopy(GA_IteratorState &dest,
				    const GA_IteratorState &src) const;
    virtual bool	isOrdered() const { return myIsOrdered; }
    virtual bool	areTraversalDeletionsSafe() const { return true; }

    virtual GA_Size	getPageCount() const	{ return 0; }
    virtual bool	getPageElementRange(GA_Range &, GA_Size, GA_Size) const
			    { return getPageElementRangeNonSplittable(); }
    virtual GA_Offset	getFirstOffsetInPage(GA_Size) const
			    { return getFirstOffsetInPageNonSplittable(); }
    /// @}

    /// Accessor to offset list
    const GA_OffsetList	&getOffsetList() const	{ return myOffsetList; }

    /// Get a GA_RangeTypeInterface representing the remainder of the range
    /// after iterating over part of it, as specified by state and current.
    /// NOTE: The caller is responsible for deleting the returned
    ///       GA_RangeTypeInterface!
    virtual GA_RangeTypeInterface *getRemainder(
            const GA_IteratorState &state,
            const GA_Offset current) const;

private:
    /// @{
    /// RangeTypeInterface implementation
    virtual void	singleRewind(GA_IteratorState &state) const;
    virtual bool	singleGet(const GA_IteratorState &state,
				GA_Offset &value) const;
    virtual void	singleNext(GA_IteratorState &state) const;
    /// @}
    //
    /// Initialize the cache.
    void		init(const GA_RangeTypeInterface &range_to_cache);

    GA_OffsetList	 myOffsetList;

    /// We support using a subset of our offset list to allow sharing of the
    /// underlying data when splitting the range.  Note that the valid range
    /// is [myStart..myEnd-1], i.e., myEnd is non-inclusive.
    GA_Size		 myStart, myEnd;

    bool		 myIsOrdered;
};

#endif
