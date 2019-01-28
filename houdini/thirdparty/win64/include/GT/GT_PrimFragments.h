/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimFragments.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimFragments__
#define __GT_PrimFragments__

#include "GT_API.h"
#include "GT_Primitive.h"
#include "GT_TransformArray.h"
#include "GT_GEODetailList.h"

class UT_StringRef;


/// A primitive made of multiple fragments that are coalesced into one geometry
///
/// For each fragment, you can query:
///	- The transform for the fragment
///	- The untransformed bounding box
///	- The transformed bounding box
///	- The fragment's __primitive_id
///	- The fragment's __point_id
/// These properties are only used for information, not for refinement or
/// rendering.  The single geometry is composed of the transformed fragments.
///
/// Attributes are:
///  - detail: one for the whole primitive
/// The instance primitive does *not* pass these down when it refines.
class GT_API GT_PrimFragments : public GT_Primitive
{
public:
    GT_PrimFragments();
    GT_PrimFragments(const GT_PrimitiveHandle &geometry,
	    const GT_GEODetailListHandle &source=GT_GEODetailListHandle());
    GT_PrimFragments(const GT_PrimFragments &src);
    virtual ~GT_PrimFragments();

    /// @{
    /// Interface from GT_Primitive
    virtual const char *className() const;
    virtual int		getPrimitiveType() const;
    virtual void	enlargeBounds(UT_BoundingBox boxes[], int nsegs) const
			    { doEnlargeBounds(boxes, nsegs, false); }
    virtual void	enlargeRenderBounds(UT_BoundingBox boxes[], int nsegs) const
			    { doEnlargeBounds(boxes, nsegs, true); }
    virtual void	getVelocityRange(UT_Vector3 &vmin, UT_Vector3 &vmax,
				const UT_StringRef &attribute_name) const;
    virtual bool	refine(GT_Refine &refiner,
					const GT_RefineParms *parms=NULL) const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    virtual bool	save(UT_JSONWriter &w) const;
    virtual fpreal	computePerimeter(int seg=0) const;
    virtual fpreal	computeSurfaceArea(int seg=0) const;
    virtual fpreal	computeVolume(const UT_Vector3 &ref_P, int seg=0) const;
    virtual GT_PrimitiveHandle	 doHarden() const;
    virtual GT_PrimitiveHandle	 doSoftCopy() const;
    virtual const GT_ViewportRefineOptions	&viewportRefineOptions() const;
    virtual const GT_AttributeListHandle	&getUniformAttributes() const
						    { return myAttributes; }
    /// @}

    /// Set up __primitive_id, __point_id and transforms based on fragment
    /// primitives.  This assumes that the primitives are all packed primitives
    /// of some kind (and have a single point).
    ///
    /// The centroids should likely be in the center of the transformed
    /// bounding box.
    void	setupFragments(const GT_GEOOffsetList &primitives,
			    const GT_GEOOffsetList &vertices,
			    const GT_GEOOffsetList &points,
			    const GT_DataArrayHandle &centroids);

    /// Return number of fragments
    GT_Size		entries() const
			    { return myCentroids ? myCentroids->entries():0; }

    /// Return combined geometry
    const GT_PrimitiveHandle		&geometry() const
						{ return myGeometry; }

    /// Return fragment centroids (where to render decorations)
    const GT_DataArrayHandle		&centroids() const
						{ return myCentroids; }
    /// @{
    /// If the geometry being instanced comes from a GU_Detail, then the source
    /// geometry should be set to point to this geometry.  This lets viewport
    /// decorations be rendered properly.
    const GT_GEODetailListHandle	&sourceGeometry() const
					    { return mySourceGeometry; }
    void	 setSourceGeometry(const GT_GEODetailListHandle &g)
		    { mySourceGeometry = g; }
    /// @}

protected:
    /// Set up __primitive_id, __point_id and __vertex_id explicitly
    /// The arrays can be NULL.
    void	setFragmentAttributes(const GT_DataArrayHandle &prim_ids,
				const GT_DataArrayHandle &vertex_ids,
				const GT_DataArrayHandle &point_ids);

    void	doEnlargeBounds(UT_BoundingBox boxes[], int nsegs,
			bool forrender) const;
    GT_PrimitiveHandle		myGeometry;
    GT_AttributeListHandle	myAttributes;
    GT_GEODetailListHandle	mySourceGeometry;
    GT_DataArrayHandle		myCentroids;
};

#endif
