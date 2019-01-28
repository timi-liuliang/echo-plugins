/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_SplittableRange.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_SplittableRange__
#define __GA_SplittableRange__

#include "GA_API.h"
#include "GA_Range.h"
#include "GA_Types.h"

#include <UT/UT_ParallelUtil.h>		// for UT_Split

#include <stddef.h>


class GA_PageIterator;
class GA_RangeTypeInterface;

class UT_JobInfo;


/// A range which can be split into multiple ranges for threading
///
/// A GA_SplittableRange can be split into separate ranges.  All split ranges
/// are guaranteed to not cross page boundaries, so each split range can be
/// worked on by independent threads.
///
/// The range conforms to tbb::range semantics, but also provides methods for
/// UT_ThreadedAlgorithm.
class GA_API GA_SplittableRange : public GA_Range
{
public:
    /// Construct a splittable range from any arbitrary range
    GA_SplittableRange(const GA_Range &range);
    /// Copy c-tor
    GA_SplittableRange(const GA_SplittableRange &range);
    ~GA_SplittableRange()
    {
    }

    /// @{
    /// UT_ParallelUtil interface
    GA_SplittableRange(GA_SplittableRange &range, UT_Split)
	: GA_Range()
    {
	range.split(range, *this);
    }
    bool	is_divisible() const
		{
		    return myRange->is_divisible();
		}
    /// @}

    /// Convenience method to test whether we can multi-thread
    bool	canMultiThread() const	{ return is_divisible(); }

    /// Return the number of pages
    GA_Size		getPageCount() const
			    { return myRange->getPageCount(); }
    /// Get the offset of the first element in a page
    GA_Offset		getFirstOffsetInPage(GA_Size relative_page) const
			    { return myRange->getFirstOffsetInPage(relative_page); }
    /// Get an iterator over the elements in a given page
    GA_Range		getPageElementRange(GA_Size relative_page) const;

    /// @{
    /// Create an iterator over the pages in the range
    GA_PageIterator	beginPages() const;
    GA_PageIterator	beginPages(const UT_JobInfo &jobinfo) const;
    /// @}

    /// Get a range of elements for a given job
    GA_Range	getJobElementRange(const UT_JobInfo &info) const;

    /// Static method to divide work for UT_ThreadedAlgorithm.
    static inline GA_Range divideWork(const GA_Range &r, const UT_JobInfo &j)
			{
			    return GA_SplittableRange(r).getJobElementRange(j);
			}

    /// Split this range into two new ranges by splitting the RTI
    bool		split(GA_SplittableRange &d1,
				GA_SplittableRange &d2) const
			{
			    GA_RangeTypeInterface	*list[2];
			    if (!myRange->is_divisible() || !myRange->split(list))
				return false;
			    d1.setRange(list[0]);
			    d2.setRange(list[1]);
			    return true;
			}
private:
};

/// This is needed by the tbb wrapper in UT_ParallelUtility
/// It returns an upper bound for the actual number of work items
/// in the range.
inline GA_API size_t UTestimatedNumItems(const GA_SplittableRange& range)
{
    return range.getMaxEntries();    
}

#endif

