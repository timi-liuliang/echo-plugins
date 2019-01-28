/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTIRepeater.h ( GA Library, C++)
 *
 * COMMENTS:	This is like the "Python" range function.  That is, the end is
 *		non-inclusive:
 *			GA_RTIRepeater(list, 0, 3) = [0, 1, 2]
 */

#ifndef __GA_RTIRepeater__
#define __GA_RTIRepeater__

#include "GA_API.h"
#include "GA_RangeTypeInterface.h"
#include "GA_Types.h"


class GA_IndexMap;
class GA_IteratorState;
class GA_Range;


class GA_API GA_RTIRepeater : public GA_RangeTypeInterface 
{
public:
    // Range which repeats the @c offset for @c count times
    GA_RTIRepeater(const GA_IndexMap &list, GA_Offset offset, GA_Size count);

    // Copy c-tor
    GA_RTIRepeater(const GA_RTIRepeater &src);
    virtual ~GA_RTIRepeater();

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
    virtual void	iterateRewind(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const;
    virtual void	iterateNext(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const;
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

    GA_Offset		myOffset;
    GA_Size		myCount;
};

#endif
