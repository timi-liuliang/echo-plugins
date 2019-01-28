/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTIFiltered.h ( GA Library, C++)
 *
 */

#ifndef __GA_RTIFiltered__
#define __GA_RTIFiltered__

#include "GA_API.h"
#include "GA_Range.h"
#include "GA_RangeTypeInterface.h"
#include "GA_Types.h"

#include <UT/UT_IntrusivePtr.h>

#include <SYS/SYS_AtomicInt.h>


class GA_IndexMap;
class GA_IteratorState;


/// @brief Filter to test whether an offset is valid for a GA_RTIFiltered range
///
/// This class defines a simple interface to test whether offsets are valid for
/// a given range.  The objects should be processed through the intrusive
/// pointer typedef.  For example @code
///	class MyFilter : public GA_RTIFilterObject { ... }
///	...
///	const GA_Range	&source_range;
///	GA_RTIFiltered	range(source_range, GA_RTIFilterHandle(new MyFilter));
/// @endcode
class GA_API GA_RTIFilterObject 
{
public:
    GA_RTIFilterObject();
    virtual ~GA_RTIFilterObject();

    virtual bool  filter(const GA_IndexMap &map, GA_Offset offset) const = 0;
    virtual bool  isEqual(const GA_RTIFilterObject *src) const = 0;

    void	incref()	{ myRefCount.add(1); }
    void	decref()
		{
		    if (!myRefCount.add(-1))
			delete this;
		}
private:
    SYS_AtomicInt32	myRefCount;
};

/// @{
/// Intrusive pointer typdef
static inline void intrusive_ptr_add_ref(GA_RTIFilterObject *o) { o->incref(); }
static inline void intrusive_ptr_release(GA_RTIFilterObject *o) { o->decref(); }

typedef UT_IntrusivePtr<GA_RTIFilterObject>	GA_RTIFilterHandle;
/// @}

/// @brief An implementation of a range which "filters" the range
///
/// The GA_RTIFiltered class takes a source range and a filter.  The filter is
/// applied to the source range to determine whether elements of the source
/// range belong to the filtered range.  A example might be: @code
///	class FilterOdd : GA_RTIFilterObject
///	{
///	    FilterOdd() {}
///	    virtual ~FilterOdd() {}
///
///	    virtual bool filter(const GA_IndexMap &, GA_Offset offset) const
///			{ return (offset & 1) != 0; }
///	    virtual bool isEqual(const GA_RTIFilterObject *src) const
///			{ return dynamic_cast<const FilterOdd *>(src) != NULL; }
///	};
///
///	...
///	GA_Range  all_points(new GA_RTIOffset(geo.getPointMap()));
///	GA_Range  odd_points(all_points, GA_RTIFilterHandle(new FilterOdd));
///	...
/// @endcode
///
class GA_API GA_RTIFiltered : public GA_RangeTypeInterface 
{
public:
    // Range which filters the source range given the filter specified
    GA_RTIFiltered(const GA_Range &base, const GA_RTIFilterHandle &filter);

    // Copy c-tor
    GA_RTIFiltered(const GA_RTIFiltered &src);

    virtual ~GA_RTIFiltered();

    /// @{
    /// RangeTypeInterface implementation
    virtual GA_RangeTypeInterface	*copy() const;
    virtual bool	isSplittable() const	{ return false; }
    virtual bool	isEmpty() const;
    virtual GA_Size	getMaxEntries() const;
    virtual GA_Size	getEntries() const;
    virtual bool	isEqual(const GA_RangeTypeInterface &src) const;
    virtual bool	split(GA_RangeTypeInterface *list[2]) const;
    virtual void	iterateCreate(GA_IteratorState &state) const;
    virtual void	iterateDestroy(GA_IteratorState &state) const;
    virtual void	iterateCopy(GA_IteratorState &dest,
				    const GA_IteratorState &src) const;
    virtual void	iterateRewind(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const;
    virtual void	iterateNext(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const;
    virtual bool	areTraversalDeletionsSafe() const;
    virtual GA_Size	getPageCount() const	{ return -1; }
    virtual bool	getPageElementRange(GA_Range &, GA_Size, GA_Size) const
			    { return getPageElementRangeNonSplittable(); }
    virtual GA_Offset	getFirstOffsetInPage(GA_Size) const
			    { return getFirstOffsetInPageNonSplittable(); }
    /// @}

private:
    void		init();

    GA_Range			 myRange;
    const GA_RangeTypeInterface	*myBase;	// Convenience ptr
    GA_RTIFilterHandle		 myFilter;
    GA_Size			 myComputedSize;
};

#endif
