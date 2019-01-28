/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PageIterator.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_PageIterator__
#define __GA_PageIterator__

#include "GA_API.h"
#include "GA_Iterator.h"
#include "GA_Range.h"
#include "GA_SplittableRange.h"
#include "GA_Types.h"

#include <UT/UT_ThreadedAlgorithm.h>

#include <stddef.h>


class GA_API GA_PageIterator
{
public:
    GA_PageIterator(const GA_SplittableRange &range,
		const UT_JobInfo *jobinfo=NULL)
	: myRange(range)
	, myJobInfo(jobinfo)
	, myCurr(0)
	, myPageCount(range.getPageCount())
	, myEndIterator(false)
    {
	if (myJobInfo)
	    myCurr = myJobInfo->nextTask();
    }
    GA_PageIterator(const GA_PageIterator &pit)
	: myRange(pit.myRange)
	, myJobInfo(pit.myJobInfo)
	, myCurr(pit.myCurr)
	, myPageCount(pit.myPageCount)
	, myEndIterator(pit.myEndIterator)
    {
    }
    ~GA_PageIterator() {}

    /// Assignment operator
    GA_PageIterator	&operator=(const GA_PageIterator &src)
			{
			    myRange = src.myRange;
			    myJobInfo = src.myJobInfo;
			    myCurr = src.myCurr;
			    myPageCount = src.myPageCount;
			    myEndIterator = src.myEndIterator;
			    return *this;
			}
    /// Equality operator
    bool		operator==(const GA_PageIterator &src) const
			{
			    if (myEndIterator || src.myEndIterator)
				return atEnd() && src.atEnd();
			    return myRange == src.myRange &&
				myJobInfo == src.myJobInfo &&
				myCurr == src.myCurr &&
				myPageCount == src.myPageCount;
			}
    bool		operator!=(const GA_PageIterator &src) const
			    { return !(*this == src); }

    /// @{
    /// Return the offset for the beginning of the current page
    GA_Offset	getFirstOffsetInPage() const
		{
		    return myRange.getFirstOffsetInPage(myCurr);
		}
    GA_Offset	operator*() const	{ return getFirstOffsetInPage(); }
    /// @}

    /// @{
    /// Iterator interface
    /// @warning The post-increment operator has a side-effect since the value
    /// returned is the iterator @b after the increment (not before)
    void	rewind()	{ myCurr = 0; }
    bool	atEnd() const	{ return myCurr >= myPageCount; }
    void	advance()
		{
		    if (myJobInfo)
			myCurr = myJobInfo->nextTask();
		    else
			myCurr++;
		}
    GA_PageIterator	&operator++()		{ advance(); return *this; }
    // No post inc as it is harmful.
    /// @}

    /// @{
    /// Begin iterating over the offsets in a page
    GA_Iterator	begin() const	{ return GA_Iterator(
					myRange.getPageElementRange(myCurr)); }
    GA_Iterator	end() const	{ return myRange.end(); }
    /// @}
private:
    bool		isEnd() const { return myEndIterator || atEnd(); }

    GA_SplittableRange	 myRange;
    const UT_JobInfo	*myJobInfo;
    GA_Size		 myCurr, myPageCount;
    bool		 myEndIterator;
};

/// Define macros roughly equivalent to FOR_INFOTASKS (in
/// UT_ThreadedAlgorithm).  This will break up the full range into a load
/// balanced iteration over pages.  For example:
/// GA_FOR_INFOTASKS(info, gdp->getPointRange(), it) 
/// {
///      doSomething(it.getOrder());		// Use point number
///      doSomething(it.getOffset());	// Use point offset
/// }
#define GA_FOR_INFORANGE(info, full_range, IT_NAME)	\
    GA_SplittableRange	lcl_prange(full_range); \
    for (GA_PageIterator pit=lcl_prange.beginPages(info); \
	    !pit.atEnd(); ++pit) \
	for (GA_Iterator IT_NAME = pit.begin(); !IT_NAME.atEnd(); ++IT_NAME)

/// Like FOR_INFOTASKS_BOSS, iterate with an optional opInterrupt for every
/// block completed.
#define GA_FOR_INFORANGE_BOSS(info, full_range, IT_NAME, boss)	\
    GA_SplittableRange	lcl_prange(full_range); \
    for (GA_PageIterator pit=lcl_prange.beginPages(info); \
	    !pit.atEnd() && !boss->opInterrupt(); ++pit) \
	for (GA_Iterator IT_NAME = pit.begin(); !IT_NAME.atEnd(); ++IT_NAME)

/// Iterate over all points for the given info
#define GA_FOR_INFO_ALLPOINTS(gdp, info, IT_NAME) \
    GA_SplittableRange	lcl_prange(gdp->getPointRange()); \
    for (GA_PageIterator pit=lcl_prange.beginPages(info); \
	    !pit.atEnd(); ++pit) \
	for (GA_Iterator IT_NAME = pit.begin(); !IT_NAME.atEnd(); ++IT_NAME)

/// Iterate over all points for the given info with a UT_Interrupt
#define GA_FOR_INFO_ALLPOINTS_BOSS(gdp, info, IT_NAME, boss) \
    GA_SplittableRange	lcl_prange(gdp->getPointRange()); \
    for (GA_PageIterator pit=lcl_prange.beginPages(info); \
	    !pit.atEnd() && !boss->opInterrupt(); ++pit) \
	for (GA_Iterator IT_NAME = pit.begin(); !IT_NAME.atEnd(); ++IT_NAME)


/// Iterate over all points for the given info with a UT_Interrupt
#define GA_FOR_INFO_GROUP_POINTS_BOSS(gdp, pointgrp, info, IT_NAME, boss) \
    GA_SplittableRange	lcl_prange(gdp->getPointRange(pointgrp)); \
    for (GA_PageIterator pit=lcl_prange.beginPages(info); \
	    !pit.atEnd() && !boss->opInterrupt(); ++pit) \
	for (GA_Iterator IT_NAME = pit.begin(); !IT_NAME.atEnd(); ++IT_NAME)


#endif
