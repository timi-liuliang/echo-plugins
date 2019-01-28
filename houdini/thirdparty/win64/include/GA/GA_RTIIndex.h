/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTIIndex.h ( GA Library, C++)
 *
 * COMMENTS:	This is like the "Python" range function.  That is, the end is
 *		non-inclusive:
 *			GA_RTIIndex(list, 0, 3) = [0, 1, 2]
 */

#ifndef __GA_RTIIndex__
#define __GA_RTIIndex__

#include "GA_API.h"
#include "GA_RTISingle.h"
#include "GA_Types.h"


class GA_IndexMap;
class GA_IteratorState;
class GA_Range;
class GA_RangeTypeInterface;


class GA_API GA_RTIIndex : public GA_RTISingle 
{
public:
    /// Iterate over the entire range of elements (in order)
    GA_RTIIndex(GA_IndexMap &list);
    GA_RTIIndex(const GA_IndexMap &list);

    /// Specify a single element by index
    GA_RTIIndex(GA_IndexMap &list, GA_Index singleton);
    GA_RTIIndex(const GA_IndexMap &list, GA_Index singleton);

    /// Iterate over a range of elements, passing a negative step will iterate
    /// backward over the range (in this case end should be < start).
    GA_RTIIndex(GA_IndexMap &list, GA_Index start, GA_Index end,
			  GA_Size step=1);
    GA_RTIIndex(const GA_IndexMap &list, GA_Index start, GA_Index end,
			  GA_Size step=1);

    /// Copy constructor
    GA_RTIIndex(const GA_RTIIndex &src);

    /// Destructor
    virtual ~GA_RTIIndex();

    /// @{
    /// RangeTypeInterface implementation
    virtual GA_RangeTypeInterface	*copy() const;
    virtual bool	isSplittable() const { return false; }
    virtual bool	isEmpty() const;
    virtual GA_Size	getMaxEntries() const;
    virtual GA_Size	getEntries() const;
    virtual bool	split(GA_RangeTypeInterface *list[2]) const;
    virtual bool	isEqual(const GA_RangeTypeInterface &src) const;
    virtual void	iterateCopy(GA_IteratorState &dest,
				    const GA_IteratorState &src) const;
    virtual void	iterateRewind(GA_IteratorState &state,
				      GA_Offset &start, GA_Offset &end) const;
    virtual void	iterateNext(GA_IteratorState &state,
				    GA_Offset &start, GA_Offset &end) const;
    virtual bool	isOrdered() const;
    virtual bool	areTraversalDeletionsSafe() const { return true; }
    virtual bool	canContainDuplicates() const { return false; }
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
    void	init(GA_Size start, GA_Size end, GA_Size step);

    GA_Index	myStart, myEnd;
    GA_Size	myStep;
};

#endif

