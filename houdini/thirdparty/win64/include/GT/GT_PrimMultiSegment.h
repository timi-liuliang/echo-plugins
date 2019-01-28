/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimMultiSegment.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimMultiSegment__
#define __GT_PrimMultiSegment__

#include "GT_API.h"
#include "GT_Primitive.h"

/// A primitive which contains a list of primitives for motion segments
/// This takes a list of primitives and creates a single primitive with
/// multiple segments.
class GT_API GT_PrimMultiSegment : public GT_Primitive
{
public:
    typedef UT_Array<GT_PrimitiveHandle>	PrimList;

    GT_PrimMultiSegment(const PrimList &list);
    GT_PrimMultiSegment(const GT_PrimMultiSegment &src);
    virtual ~GT_PrimMultiSegment();

    /// @{
    /// Method defined on GT_Primitive
    virtual const char	*className() const;
    virtual void	 enlargeBounds(UT_BoundingBox boxes[], int nsegs) const;
    virtual void	 enlargeRenderBounds(UT_BoundingBox b[], int n) const;
    virtual bool	 refine(GT_Refine &refiner,
				const GT_RefineParms *parms=NULL) const;
    virtual int		 getMotionSegments() const;
    virtual int64	 getMemoryUsage() const;
    virtual bool	 save(UT_JSONWriter &w) const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimMultiSegment(*this); }
    /// @}

    /// Return the primitive for a given segment
    const GT_PrimitiveHandle	&prim(int i) const	{ return myList(i); }

private:
    PrimList	myList;
};

#endif

