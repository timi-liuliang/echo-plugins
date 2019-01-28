/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTIPrimitiveComprehension.h ( GA Library, C++)
 *
 * COMMENTS:	Define the range of points or vertices referenced by a primitive
 *
 * The range of vertices owned by primitive 32
 * 	GA_RTIPrimitiveComprehension	i(geo, 32, GA_ATTRIB_VERTEX);
 *
 * The range of points used by primitive 14.
 * 	GA_RTIPrimitiveComprehension	i(geo, 14, GA_ATTRIB_POINT);
 *
 * If a primitive refers to a point more than once, the point will be visited
 * multiple times.
 */

#ifndef __GA_RTIPrimitiveComprehension__
#define __GA_RTIPrimitiveComprehension__

#include "GA_API.h"
#include "GA_RTISingle.h"
#include "GA_OffsetList.h"
#include "GA_Types.h"


class GA_Detail;
class GA_IteratorState;
class GA_Range;
class GA_RangeTypeInterface;


/// @brief Range of all points/vertices accessed by a range of primitives
///
/// @note This allocates an offset list of all elements.
class GA_API GA_RTIPrimitiveComprehension : public GA_RTISingle 
{
public:
    /// Specify true for @c keep_prim_order if you need the points/vertices
    /// to be delivered in the order they're traversed by @c primlist.
    GA_RTIPrimitiveComprehension(const GA_Detail &geo,
				const GA_Range &primlist,
				GA_AttributeOwner what,
				bool keep_prim_order);
    GA_RTIPrimitiveComprehension(const GA_RTIPrimitiveComprehension &i,
			    GA_Offset start, GA_Offset end);
    GA_RTIPrimitiveComprehension(const GA_RTIPrimitiveComprehension &i);
    virtual ~GA_RTIPrimitiveComprehension();

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

    GA_OffsetList	 myOffsets;
    GA_AttributeOwner	 myTargetType;
};

#endif
