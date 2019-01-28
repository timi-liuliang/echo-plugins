/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTIOffset.h ( GA Library, C++)
 *
 * COMMENTS:	This is like the "Python" range function.  That is, the end is
 *		non-inclusive:
 *			GA_RTIOffset(list, 0, 3) = [0, 1, 2]
 */

#ifndef __GA_RTIOffset__
#define __GA_RTIOffset__

#include "GA_API.h"
#include "GA_RangeTypeInterface.h"
#include "GA_Types.h"


class GA_IndexMap;
class GA_IteratorState;
class GA_Range;
class GA_RangeMemberQuery;


/// Including temporaries will also include temporary vertex/point indices in
/// the range.  Most ranges are only interested in the visible elements.
/// However, some operations need to be applied to *all* elements.
template <bool IncludeTemporaries>
class GA_RTIOffset_T : public GA_RangeTypeInterface 
{
public:
    // Range containing the entire range of the element list
    GA_RTIOffset_T(const GA_IndexMap &list);

    // Range containing a single element
    GA_RTIOffset_T(const GA_IndexMap &list, GA_Offset singleton);

    // Range of a subset of elements
    GA_RTIOffset_T(const GA_IndexMap &list, GA_Offset start, GA_Offset end);

    // Copy c-tor
    GA_RTIOffset_T(const GA_RTIOffset_T &src);

    virtual ~GA_RTIOffset_T();

    /// @{
    /// RangeTypeInterface implementation
    virtual GA_RangeTypeInterface	*copy() const;
    virtual bool	isSplittable() const	{ return true; }
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
    virtual bool	isOrdered() const { return true; }
    virtual bool	areTraversalDeletionsSafe() const { return true; }
    virtual bool	canContainDuplicates() const { return false; }
    virtual const GA_RangeMemberQuery	*allocMemberQuery() const;

    virtual GA_Size	getPageCount() const;
    virtual bool	getPageElementRange(GA_Range &range,
			    GA_Size relative_page, GA_Size npages) const;
    virtual GA_Offset	getFirstOffsetInPage(GA_Size relative_page) const;
    /// @}

private:
    void	init(GA_Size start, GA_Size end);
    void	extractRange(GA_Offset &curr,
			GA_Offset &start, GA_Offset &end) const;

    // State information
    GA_Offset		myStart, myEnd;
    GA_Size		myComputedSize;
};

typedef GA_RTIOffset_T<false>	GA_RTIOffset;
typedef GA_RTIOffset_T<true>	GA_RTIOffsetComplete;

#endif
