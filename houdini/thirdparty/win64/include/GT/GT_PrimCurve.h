/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimCurve.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimCurve__
#define __GT_PrimCurve__

#include "GT_API.h"
#include "GT_Primitive.h"

/// @brief A single linear or cubic patch
class GT_API GT_PrimCurve : public GT_Primitive {
public:
    GT_PrimCurve();
    GT_PrimCurve(GT_Basis basis,
		    const GT_AttributeListHandle &vertex,
		    const GT_AttributeListHandle &uniform,
		    bool wrap)
    {
	init(basis, vertex, uniform, wrap);
    }
    virtual ~GT_PrimCurve();

    virtual const char *className() const { return "GT_PrimCurve"; }
    virtual bool	save(UT_JSONWriter &w) const;

    /// @{
    /// Methods defined on GT_Primitive
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
				int nsegments) const;
    virtual void	enlargeRenderBounds(UT_BoundingBox b[], int n) const;
    virtual int		getPrimitiveType() const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    /// @}

    /// Construct a simple curve.
    /// - @c vertex @n Attribute data per vertex
    /// - @c uniform @n Single value which is uniform over the patch
    bool	init(GT_Basis basis,
		    const GT_AttributeListHandle &vertex,
		    const GT_AttributeListHandle &uniform,
		    bool wrapped);

    /// @{
    /// Accessor
    GT_Basis			getBasis() const	{ return myBasis; }
    bool			getWrap() const		{ return myWrap; }
    const GT_AttributeListHandle &getVertex() const	{ return myVertex; }
    const GT_AttributeListHandle &getUniform() const	{ return myUniform; }
    /// @}

    /// @{
    /// Access attributes
    virtual const GT_AttributeListHandle	&getVertexAttributes() const
						    { return myVertex; }
    virtual const GT_AttributeListHandle	&getUniformAttributes() const
						    { return myUniform; }
    /// @}

    /// @{
    /// Methods for GEO/GU support.
    /// Compute perimeter of a curve or an indexed curve
    virtual fpreal	computePerimeter(int seg) const;
    static fpreal	curveLength(const GT_DataArray &P,
				GT_Size nvtx=-1, GT_Offset start=0,
				const UT_Matrix4D *xform = NULL);
    /// @}

    /// Harden all attributes so there are no dangling dependencies
    virtual GT_PrimitiveHandle	doHarden() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimCurve(*this); }

    /// The virtual implementation of attribute merging
    virtual GT_PrimitiveHandle	doAttributeMerge(const GT_Primitive &src,
					const UT_StringMMPattern *vertex,
					const UT_StringMMPattern *point,
					const UT_StringMMPattern *uniform,
					const UT_StringMMPattern *detail) const;

private:
    GT_AttributeListHandle	myVertex;
    GT_AttributeListHandle	myUniform;
    GT_Basis			myBasis;
    bool			myWrap;
};

#endif
