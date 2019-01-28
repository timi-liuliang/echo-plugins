/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RangeMemberQuery.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_RangeMemberQuery__
#define __GA_RangeMemberQuery__

#include "GA_API.h"
#include "GA_Types.h"

class GA_RangeTypeInterface;
class GA_ElementGroup;

/// @brief Abstract base class for a range membership query object
///
/// A range member query object permits the testing of individual element
/// offsets for membership in their parent range.  This is typically used
/// where testing a set of elements for membership is more efficient than
/// searching for each individual member one after another.
///
/// @see GA_Range
class GA_API GA_RangeMemberQuery
{
public:
    virtual ~GA_RangeMemberQuery();

    virtual bool	 contains(GA_Offset) const = 0;

    static const GA_RangeMemberQuery	*alloc();
    static const GA_RangeMemberQuery	*alloc(const GA_RangeTypeInterface &);
    static const GA_RangeMemberQuery	*alloc(const GA_ElementGroup &);
};
#endif
