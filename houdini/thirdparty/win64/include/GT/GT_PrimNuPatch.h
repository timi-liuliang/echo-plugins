/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimNuPatch.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimNuPatch__
#define __GT_PrimNuPatch__

#include "GT_API.h"
#include "GT_Primitive.h"

class GT_TrimNuCurves;

/// @brief A simple representation of a NURBS surface
///
/// The NURBS patch attributes are specified by a hull/mesh of control vertices
/// and a constant attribute for the entire patch.
///
/// Given a patch with:
///  - @c uorder = The order in the U direction
///  - @c uknots = The U knot vector (with length given by @c ulen)
///  - @c vorder = The order in the V direction
///  - @c vknots = The V knot vector (with length given by @c vlen)
/// The number of control vertices is given by: @code
///   nu = len(uknots) - uorder
///   nv = len(vknots) - vorder
///   nvertices = nu * nv
/// @endcode
///
/// When specifying rational NURBS patches, the "P" attribute should always be
/// specified as the non-homogeneous 3-tuple coordinates.  An optional "Pw"
/// float array can be specified, and if it exists, the homogeneous positions
/// will be defined as @code:
///   P = (x,y,z)
///   Pw = (w)
///   P' = (w*x, w*y, w*z, w)
/// @endcode
class GT_API GT_PrimNuPatch : public GT_Primitive
{
public:
    GT_PrimNuPatch();
    GT_PrimNuPatch(const GT_PrimNuPatch &src);
    GT_PrimNuPatch(const GT_PrimNuPatch &src,
		const GT_AttributeListHandle &vertex,
		const GT_AttributeListHandle &detail);
    GT_PrimNuPatch(int uorder, const GT_DataArrayHandle &uknots,
		int vorder, const GT_DataArrayHandle &vknots,
		const GT_AttributeListHandle &vertex,
		const GT_AttributeListHandle &detail);
    virtual ~GT_PrimNuPatch();

    /// @{
    /// Methods defined on GT_Primitive
    virtual const char	*className() const { return "GT_PrimNuPatch"; }
    virtual bool	 save(UT_JSONWriter &w) const;

    virtual void	 enlargeBounds(UT_BoundingBox boxes[],
					int nsegments) const;
    virtual int		 getPrimitiveType() const;
    virtual bool	 refine(GT_Refine &refiner,
					const GT_RefineParms *parms) const;
    virtual int		 getMotionSegments() const;
    virtual int64	 getMemoryUsage() const;
    /// @}


    /// @{
    /// Accessor
    int		getNu() const	{ return myUKnots->entries() - myUOrder; }
    int		getNv() const	{ return myVKnots->entries() - myVOrder; }
    int		getUOrder() const	{ return myUOrder; }
    int		getVOrder() const	{ return myVOrder; }
    const GT_DataArrayHandle	&getUKnots() const	{ return myUKnots; }
    const GT_DataArrayHandle	&getVKnots() const	{ return myVKnots; }
    const GT_AttributeListHandle	&getVertex() const
					    { return myVertex; }
    const GT_AttributeListHandle	&getDetail() const
					    { return myDetail; }
    /// @}

    /// Check if trimmed
    bool			 isTrimmed() const { return myTrims != NULL; }

    /// Get trim curves
    const GT_TrimNuCurves	*getTrimCurves() const
				    { return myTrims; }
    /// Set trim curves.  Ownership of the curves is @b not assumed by the patch
    void			 setTrimCurves(const GT_TrimNuCurves *curves);
    /// Set trim curves, adopting ownership of the trim curves passed in
    void			 adoptTrimCurves(GT_TrimNuCurves *curves);

    /// Houdini stores it's vertices in row-major order.  This is not the case
    /// for all software.  This method will transpose the vertex into
    /// column-major order.  This operation is reversible by calling it
    /// multiple times.
    GT_PrimitiveHandle	transposeVertex() const;

    /// @{
    /// Virtual access to attributes
    virtual const GT_AttributeListHandle	&getVertexAttributes() const
						    { return myVertex; }
    virtual const GT_AttributeListHandle	&getDetailAttributes() const
						    { return myDetail; }
    /// @}

    /// Build the hull of the primitive
    GT_PrimitiveHandle	buildHull(bool copy_xform = true) const;

    /// Refine to a polygonal surface
    GT_PrimitiveHandle	buildSurface(const GT_RefineParms *parms,
				     bool copy_xform = true) const;

    /// Harden all attributes and arrays
    virtual GT_PrimitiveHandle		doHarden() const;

    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimNuPatch(*this); }

    /// The virtual implementation of attribute merging
    virtual GT_PrimitiveHandle	doAttributeMerge(const GT_Primitive &src,
					const UT_StringMMPattern *vertex,
					const UT_StringMMPattern *point,
					const UT_StringMMPattern *uniform,
					const UT_StringMMPattern *detail) const;

protected:
    void	hardenAttributes();

private:
    GT_AttributeListHandle	 myVertex;
    GT_AttributeListHandle	 myDetail;
    GT_DataArrayHandle		 myUKnots;
    GT_DataArrayHandle		 myVKnots;
    GT_TrimNuCurves		*myTrims;
    int				 myUOrder;
    int				 myVOrder;
};

#endif

