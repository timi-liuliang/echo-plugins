/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTIPrimitiveRef.h ( GA Library, C++)
 *
 * COMMENTS:	Define the range of points or vertices referenced by a primitive
 *
 * The range of vertices owned by primitive 32
 * 	GA_RTIPrimitiveRef	i(geo, 32, GA_ATTRIB_VERTEX);
 *
 * The range of points used by primitive 14.
 * 	GA_RTIPrimitiveRef	i(geo, 14, GA_ATTRIB_POINT);
 *
 * If a primitive refers to a point more than once, the point will be visited
 * multiple times.  Behavior is undefined if vertices are added or removed
 * during traversal.
 */

#ifndef __GA_RTIPrimitiveRef__
#define __GA_RTIPrimitiveRef__

#include "GA_API.h"
#include "GA_RTISingle.h"
#include "GA_Types.h"


class GA_Detail;
class GA_IndexMap;
class GA_IteratorState;
class GA_Primitive;
class GA_Range;
class GA_RangeTypeInterface;


class GA_API GA_RTIPrimitiveRef : public GA_RTISingle 
{
public:
	     GA_RTIPrimitiveRef(const GA_Detail &geo, GA_Offset prim,
					GA_AttributeOwner what);
	     GA_RTIPrimitiveRef(const GA_RTIPrimitiveRef &i);
    virtual ~GA_RTIPrimitiveRef();

    /// @{
    /// RangeTypeInterface implementation
    virtual GA_RangeTypeInterface	*copy() const;
    virtual bool	isSplittable() const	{ return false; }
    virtual bool	isEmpty() const;
    virtual GA_Size	getMaxEntries() const;
    virtual GA_Size	getEntries() const;
    virtual bool	split(GA_RangeTypeInterface *list[2]) const;
    virtual bool	isEqual(const GA_RangeTypeInterface &src) const;
    virtual void	iterateDestroy(GA_IteratorState &state) const;
    virtual void	iterateCopy(GA_IteratorState &dest,
				    const GA_IteratorState &src) const;
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

    const GA_IndexMap	&mySourceMap;
    const GA_Primitive	*myPrimPtr;
    GA_Offset		 myPrimitive;
    GA_AttributeOwner	 myTargetType;
    bool		 myValid;
};

#endif
