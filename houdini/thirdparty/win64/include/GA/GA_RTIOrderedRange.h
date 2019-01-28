/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTIOrderedRange.h ( GA Library, C++)
 *
 * COMMENTS:	This is an RTI class that sorts and orders another RTI for
 *		iteration or traversal.
 */

#ifndef __GA_RTIOrderedRange__
#define __GA_RTIOrderedRange__

#include "GA_API.h"
#include "GA_OffsetList.h"
#include "GA_RTISingle.h"
#include "GA_Types.h"

class GA_IteratorState;
class GA_Range;
class GA_RangeTypeInterface;


class GA_API GA_RTIOrderedRange : public GA_RTISingle 
{
public:
    /// Iterate over the source range but in order.
    /// @param	descending  If true, the index order is going to be in
    ///		    a descending order. If false, it's ascending.
    GA_RTIOrderedRange(const GA_RangeTypeInterface &src, 
		       bool descending = false);

    /// Copy constructor
    GA_RTIOrderedRange(const GA_RTIOrderedRange &src);

    /// Destructor
    virtual ~GA_RTIOrderedRange();

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
    virtual bool	isOrdered() const;
    virtual bool	areTraversalDeletionsSafe() const { return true; }
    virtual GA_Size	getPageCount() const	{ return -1; }
    virtual bool	getPageElementRange(GA_Range &, GA_Size, GA_Size) const
			    { return getPageElementRangeNonSplittable(); }
    virtual GA_Offset	getFirstOffsetInPage(GA_Size) const
			    { return getFirstOffsetInPageNonSplittable(); }
    /// @}

private:
    /// @{
    /// RangeTypeInterface implementation
    virtual void	singleRewind(GA_IteratorState &state) const;
    virtual bool	singleGet(const GA_IteratorState &state,
				GA_Offset &value) const;
    virtual void	singleNext(GA_IteratorState &state) const;
    /// @}

    /// Initialize the ordered range object.
    void		init(const GA_RangeTypeInterface &src,
			     bool descending = false);

    /// Finds an index that has a valid offset that is equal or greater
    /// than the index given by the argument. Returns -1 if no such valid 
    /// index can be found.
    GA_Size		findValidOffsetIndexAtOrAfter(GA_Size index) const;

    /// The array that contains a permutation of offsets that orders
    /// them according to the index 
    GA_OffsetList	myOffsetOrder;

    /// The iteration step, usually +1 or -1 to indicate traversal direction.
    int			myStep;
};

#endif

