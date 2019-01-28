/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTIElementGroup.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __GA_RTIElementGroup__
#define __GA_RTIElementGroup__

#include "GA_API.h"
#include "GA_RTISingle.h"
#include "GA_Types.h"

class GA_ElementGroup;
class GA_ElementGroupOrder;
class GA_IndexMap;
class GA_IteratorState;
class GA_Range;
class GA_RangeMemberQuery;
class GA_RangeTypeInterface;


/// @brief Iterate over a group of elements
/// Iterate over a group of elements.  If no group is specified, this is
/// is an empty range.
///
/// There are some caveats when specifying the complement of a group:
///	- The order of elements is undefined, even if the group is ordered
///	- If there is no group specified, the range will be empty
/// The first caveate may be expected, the second may be non-intuitive.
///
/// @see GA_RangeTypeInterface
class GA_API GA_RTIElementGroup : public GA_RTISingle 
{
public:
    /// Construct given a group name (may be NULL)
    ///
    /// If @c complement is false, the range will be composed of elements in
    /// the group.  If @c complement is true, the complement of the group will
    /// be contained in the range.
    GA_RTIElementGroup(const GA_IndexMap &index_map,
			const char *name,
			bool complement,
			bool ignore_order);

    /// Construct given an optional group (the group owner should match the
    /// index map owner)
    GA_RTIElementGroup(const GA_IndexMap &index_map,
			const GA_ElementGroup *group,
			bool complement,
			bool ignore_order);

    /// Copy constructor
    GA_RTIElementGroup(const GA_RTIElementGroup &i);

    /// Destructor
    virtual ~GA_RTIElementGroup();

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
    virtual bool	isOrdered() const;
    virtual bool	areTraversalDeletionsSafe() const;
    virtual bool	canContainDuplicates() const { return false; }
    virtual const GA_RangeMemberQuery	*allocMemberQuery() const;
    virtual GA_Size	getPageCount() const;
    virtual bool	getPageElementRange(GA_Range &, GA_Size, GA_Size) const;
    virtual GA_Offset	getFirstOffsetInPage(GA_Size) const;
    /// @}

    /// We partially override these iterates.  We pass down to the
    /// RTISingle for ordered groups, but unordered groups we can
    /// do a fast path.

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

    /// Get a GA_RangeTypeInterface representing the remainder of the range
    /// after iterating over part of it, as specified by state and current.
    /// NOTE: The caller is responsible for deleting the returned
    ///       GA_RangeTypeInterface!
    virtual GA_RangeTypeInterface *getRemainder(
            const GA_IteratorState &state,
            const GA_Offset current) const;

protected:
    /// Constructor used in splitting range
    GA_RTIElementGroup(const GA_ElementGroup &group,
                    GA_Offset start, GA_Offset end, bool complement);
    /// @{
    /// RangeTypeInterface implementation
    virtual void	singleRewind(GA_IteratorState &state) const;
    virtual bool	singleGet(const GA_IteratorState &state,
				GA_Offset &value) const;
    virtual void	singleNext(GA_IteratorState &state) const;
    /// @}

    /// Move to the next span
    void		nextSpan(GA_IteratorState &state) const;
    /// Move to the next offset in the current span
    void		incState(GA_IteratorState &state) const;

    /// Test if the group is ordered.  When the range specifies the complement
    /// of the group, there can be no order.
    bool		ordered() const
			    { return myOrder != NULL && !myComplement; }

private:
    const GA_ElementGroup       *myGroup;
    const GA_ElementGroupOrder	*myOrder;
    GA_Offset			 myStart, myEnd;
    bool			 myComplement;
};

#endif
