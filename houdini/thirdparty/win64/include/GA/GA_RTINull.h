/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTINull.h ( GA Library, C++)
 *
 * COMMENTS:	This is like the "Python" range function.  That is, the end is
 *		non-inclusive:
 *			GA_RTINull(list, 0, 3) = [0, 1, 2]
 */

#ifndef __GA_RTINull__
#define __GA_RTINull__

#include "GA_API.h"
#include "GA_RangeTypeInterface.h"
#include "GA_Types.h"


class GA_IndexMap;
class GA_IteratorState;
class GA_Range;


class GA_API GA_RTINull : public GA_RangeTypeInterface 
{
public:
    // Iterate over the entire range of the element list
    GA_RTINull(GA_IndexMap &list);
    GA_RTINull(const GA_IndexMap &list);

    // Copy c-tor
    GA_RTINull(const GA_RTINull &src);

    virtual ~GA_RTINull();

    /// @{
    /// RangeTypeInterface implementation
    virtual GA_RangeTypeInterface	*copy() const;
    virtual bool	isSplittable() const	{ return true; }
    virtual bool	isEmpty() const		{ return true; }
    virtual GA_Size	getMaxEntries() const	{ return 0; }
    virtual GA_Size	getEntries() const	{ return 0; }
    virtual bool	isEqual(const GA_RangeTypeInterface &src) const;
    virtual void	iterateCopy(GA_IteratorState &dest,
				    const GA_IteratorState &src) const;
    virtual void	iterateRewind(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const;
    virtual void	iterateNext(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const;
    virtual bool	isOrdered() const { return true; }
    virtual bool	areTraversalDeletionsSafe() const { return true; }
    virtual bool	split(GA_RangeTypeInterface *list[2]) const;
    virtual GA_Size	getPageCount() const	{ return -1; }
    virtual bool	getPageElementRange(GA_Range &, GA_Size, GA_Size) const
			    { return getPageElementRangeNonSplittable(); }
    virtual GA_Offset	getFirstOffsetInPage(GA_Size) const
			    { return getFirstOffsetInPageNonSplittable(); }
    /// @}

private:
};

#endif
