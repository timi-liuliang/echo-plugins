/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOPrimList.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOPrimList__
#define __GT_GEOPrimList__

#include "GT_API.h"
#include "GT_Primitive.h"
#include "GT_GEOSupport.h"
#include <GEO/GEO_Primitive.h>

class UT_StringRef;


/// Class to contain a range of GEO primitives
class GT_API GT_GEOPrimList : public GT_Primitive
{
public:
    GT_GEOPrimList(const GU_ConstDetailHandle &gdp,
			const GT_GEOOffsetList &prims);
    GT_GEOPrimList(const GT_GEOPrimList &src);
    virtual ~GT_GEOPrimList();

    /// @{
    /// GT_Primitive interface
    virtual const char *className() const;
    virtual int		getPrimitiveType() const { return GT_GEO_PRIMLIST; }
    virtual void	enlargeBounds(UT_BoundingBox boxes[], int nseg) const;
    virtual void	enlargeRenderBounds(UT_BoundingBox boxes[], int nseg) const;
    virtual void	getVelocityRange(UT_Vector3 &vmin, UT_Vector3 &vmax,
				const UT_StringRef &attribute_name) const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    virtual bool	save(UT_JSONWriter &w) const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_GEOPrimList(*this); }
    virtual GT_PrimitiveHandle	doHarden() const
				    { return GT_PrimitiveHandle(); }
    /// @}

    /// Refining fails for this primitive.
    virtual bool	refine(GT_Refine &refiner,
				const GT_RefineParms *parms=NULL) const;

    /// Add a primitive
    void		append(const GEO_Primitive &prim);

    /// Access to the GU_Detail
    const GU_ConstDetailHandle	&detail() const	{ return myGdp; }

    /// Access the list of primitives
    GA_Range		getPrimitiveRange() const;

    /// Query primitive type
    /// If the list contains a heterogeneous mix of primitive types, the type
    /// will be invalid.
    const GA_PrimitiveTypeId	&getTypeId() const	{ return myType; }

private:
    GU_ConstDetailHandle	myGdp;
    GT_GEOOffsetList		myPrimitives;
    GA_PrimitiveTypeId		myType;
};

#endif
