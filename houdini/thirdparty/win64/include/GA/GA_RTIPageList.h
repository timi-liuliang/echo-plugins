/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTIPageList.h ( GA Library, C++)
 *
 * COMMENTS:	This is similar to GA_RangeOffset, except instead of
 *		specifying a range of offsets, we specify a list of pages.
 */

#ifndef __GA_RTIPageList__
#define __GA_RTIPageList__

#include "GA_API.h"
#include "GA_RangeTypeInterface.h"
#include "GA_Types.h"

#include <UT/UT_SharedPtr.h>
#include <UT/UT_VectorTypes.h>


class GA_IndexMap;
class GA_IteratorState;
class GA_Range;


/// @brief Define a range based on a specific offset list
class GA_API GA_RTIPageList : public GA_RangeTypeInterface 
{
public:
    // Iterate over the entire range of the offset list
    GA_RTIPageList(GA_IndexMap &list, const UT_SharedPtr<const UT_IntArray> &pages,
		   GA_Offset start=GA_Offset(0), GA_Offset end=GA_Offset(-1));
    GA_RTIPageList(const GA_IndexMap &list, const UT_SharedPtr<const UT_IntArray> &pages,
		   GA_Offset start=GA_Offset(0), GA_Offset end=GA_Offset(-1));

    // Copy c-tor
    GA_RTIPageList(const GA_RTIPageList &src);

    virtual ~GA_RTIPageList();

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
    UT_SharedPtr<const UT_IntArray> myPages;
    GA_Offset		 myStart, myEnd;	// Range of offset list
};

#endif
