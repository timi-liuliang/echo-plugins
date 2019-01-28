/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimNull.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimNull__
#define __GT_PrimNull__

#include "GT_API.h"
#include "GT_Primitive.h"

/// A null primitive (no geometry, just a placeholder)
class GT_API GT_PrimNull : public GT_Primitive {
public:
	     GT_PrimNull();
    virtual ~GT_PrimNull();

    virtual const char *className() const { return "GT_PrimNull"; }
    
    /// @{
    /// API from GT_Primitive
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
					int nsegments) const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimNull(); }
    /// @}
};

#endif
