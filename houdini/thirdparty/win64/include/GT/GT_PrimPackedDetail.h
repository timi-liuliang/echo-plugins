/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_Primitive.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimPackedDetail__
#define __GT_PrimPackedDetail__

#include "GT_API.h"
#include "GT_PrimInstance.h"
#include <GU/GU_DetailHandle.h>

/// @brief A primitive which contains it's own GU_Detail with a transform
///
/// This is very similar to an instance.  The key differences are that the GT
/// geometry is defined by the embedded GU_Detail in the primitive.  The other
/// difference is that by default, this primitive has a single instance
/// containing a single identity transform.
class GT_API GT_PrimPackedDetail : public GT_PrimInstance
{
public:
    GT_PrimPackedDetail();
    GT_PrimPackedDetail(const GU_DetailHandle &gdp,
	    const GT_AttributeListHandle &uniform=GT_AttributeListHandle(),
	    const GT_AttributeListHandle &detail=GT_AttributeListHandle());
    GT_PrimPackedDetail(const GT_PrimPackedDetail &src);
    virtual ~GT_PrimPackedDetail();

    /// @{
    /// Interface from GT_Primitive
    virtual const char *className() const;
    virtual int64	getMemoryUsage() const;
    virtual bool	save(UT_JSONWriter &w) const;
    virtual GT_PrimitiveHandle	 doHarden() const;
    virtual GT_PrimitiveHandle	 doSoftCopy() const;
    /// @}

    /// @{
    /// Provide read only access to the detail
    GU_ConstDetailHandle detailHandle() const
			    { return GU_ConstDetailHandle(myDetailHandle); }

    /// Set the detail.  This will automatically set the GT representations
    const GU_DetailHandle	&setDetail(const GU_DetailHandle &gdp);

    /// Set the detail (taking ownership of the GU_Detail* passed in)
    const GU_DetailHandle	&setDetail(GU_Detail *gdp);

private:
    GU_DetailHandle	 myDetailHandle;
};

#endif
