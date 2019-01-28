/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTIOffsetList.h ( GA Library, C++)
 *
 * COMMENTS:	This is similar to GA_RangeOffset, except instead of
 *		specifying a range of offsets, we specify a list of offsets.
 */

#ifndef __GA_RTIOffsetList__
#define __GA_RTIOffsetList__

#include "GA_API.h"
#include "GA_OffsetList.h"
#include "GA_RTISingle.h"
#include "GA_Types.h"

class GA_IndexMap;
class GA_IteratorState;
class GA_Range;
class GA_RangeTypeInterface;


/// @brief Define a range based on a specific offset list
class GA_API GA_RTIOffsetList : public GA_RTISingle 
{
public:
    // Iterate over the entire range of the offset list.  If end is -1, it will
    // be set to the number of entries in the offset list.
    GA_RTIOffsetList(const GA_IndexMap &list, const GA_OffsetList &offsets,
			GA_Size start=0, GA_Size end=-1);

    // Copy c-tor
    GA_RTIOffsetList(const GA_RTIOffsetList &src);

    virtual ~GA_RTIOffsetList();

    /// @{
    /// RangeTypeInterface implementation
    virtual GA_RangeTypeInterface	*copy() const;
    virtual bool	isSplittable() const;
    virtual bool	isEmpty() const;
    virtual GA_Size	getMaxEntries() const;
    virtual GA_Size	getEntries() const;
    virtual bool	split(GA_RangeTypeInterface *list[2]) const;
    virtual bool	isEqual(const GA_RangeTypeInterface &src) const;
    virtual void	iterateCopy(GA_IteratorState &dest,
				    const GA_IteratorState &src) const;
    virtual bool	areTraversalDeletionsSafe() const { return true; }
    virtual GA_Size	getPageCount() const	{ return -1; }
    virtual bool	getPageElementRange(GA_Range &, GA_Size, GA_Size) const
			    { return getPageElementRangeNonSplittable(); }
    virtual GA_Offset	getFirstOffsetInPage(GA_Size) const
			    { return getFirstOffsetInPageNonSplittable(); }
    /// @}

    /// Accessor for offset list.
    const GA_OffsetList	&getOffsetList() const	{ return myOffsetList; }

    /// Rewind the iterator (i.e. reset the iterator state).  The start/offset
    /// should be initialized so that they define the first contiguous "block"
    /// for iteration.  If the range is empty, @c start should be greater than
    /// @c end
    virtual void	iterateRewind(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const;

    /// Choose the next contiguous range.  If the iteration is complete,
    /// @c start should be set to a value greater than @c end.
    virtual void	iterateNext(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const;

private:
    /// @{
    /// RangeTypeInterface implementation
    virtual void	singleRewind(GA_IteratorState &state) const;
    virtual bool	singleGet(const GA_IteratorState &state,
				GA_Offset &value) const;
    virtual void	singleNext(GA_IteratorState &state) const;
    /// @}

    GA_OffsetList	myOffsetList;
    GA_Size		myStart, myEnd; // Range of offset list
};

#endif
