/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimInstance.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimInstance__
#define __GT_PrimInstance__

#include "GT_API.h"
#include "GT_Primitive.h"
#include "GT_TransformArray.h"
#include "GT_GEODetailList.h"
#include "GT_GEOSupport.h"

class UT_StringRef;

/// A primitive which instances another primitive
///
/// The instance primitive can have attribute lists:
///  - detail: one for the whole primitive
///  - uniform: one per instance
/// The instance primitive does *not* pass these down when it refines.
class GT_API GT_PrimInstance : public GT_Primitive
{
public:
    GT_PrimInstance();
    GT_PrimInstance(const GT_PrimitiveHandle &geometry,
	    const GT_TransformArrayHandle &transforms,
	    const GT_GEOOffsetList &packed_prim_offsets=GT_GEOOffsetList(),
	    const GT_AttributeListHandle &uniform=GT_AttributeListHandle(),
	    const GT_AttributeListHandle &detail=GT_AttributeListHandle(),
	    const GT_GEODetailListHandle &source=GT_GEODetailListHandle());
    GT_PrimInstance(const GT_PrimInstance &src);
    virtual ~GT_PrimInstance();

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
						    { return myUniform; }
    virtual const GT_AttributeListHandle	&getDetailAttributes() const
						    { return myDetail; }
    /// @}

    /// Check valid
    bool		isValid() const
			    { return myGeometry && myTransforms; }

    /// Return number of instances
    GT_Size		entries() const
			    { return myTransforms ? myTransforms->entries():0; }

    /// Return geometry to be instanced
    const GT_PrimitiveHandle		&geometry() const
						{ return myGeometry; }

    /// Return transforms for instances
    const GT_TransformArrayHandle	&transforms() const
						{ return myTransforms; }
    /// Non-const transform access (for appending new transforms).
    GT_TransformArrayHandle		&transforms()
						{ return myTransforms; }
    /// Return uniform attributes
    const GT_AttributeListHandle	&uniform() const
						{ return myUniform; }
     /// Return detail attributes
    const GT_AttributeListHandle	&detail() const
						{ return myDetail; }

    const GT_GEOOffsetList		&packedPrimOffsets() const
						{ return myPackedPrimOffsets; }

    /// Return the transforms for instances, but combined with the transform
    /// for the primitive itself.
    GT_TransformArrayHandle	combinedTransforms() const;

    /// Refine geometry to create new instance primitives for each refined
    /// primitive.  For example, if you refine an instance primitive containing
    /// a GT_PrimPolygonMesh, the poly mesh will be refined to create
    /// individual polygons.  Each of these will be added to the refiner
    /// contained in an instance primitive.
    bool	refineToInstances(GT_Refine &refiner,
					const GT_RefineParms *parms) const;
    /// Refine to individual pieces of geometry.  This will create transformed
    /// copies of the geometry for each transform in the instance primitive
    bool	flattenInstances(GT_Refine &refiner,
					const GT_RefineParms *parms) const;
    /// If the geometry contained by this instance primitive is an instance
    /// primitive itself this method will expand the instances, by permuting
    /// the transform arrays, creating a single flattened array of transforms.
    GT_PrimitiveHandle	expandInstanceTransforms() const;

    /// When the generic refine method is called, if the primitive is a
    /// "simple" primitive, flattenInstances() will be called instead of
    /// refineToInstances().
    ///
    /// Default method considers the following to be "simple"
    /// - GT_PRIM_POLYGON_MESH
    /// - GT_PRIM_POLYGON
    /// - GT_PRIM_SUBDIVISION_MESH
    /// - GT_PRIM_CURVE_MESH
    /// - GT_PRIM_CURVE
    /// - GT_PRIM_POINT_MESH
    /// - GT_PRIM_NUPATCH
    /// - GT_PRIM_CIRCLE
    /// - GT_PRIM_SPHERE
    /// - GT_PRIM_TUBE
    /// - GT_PRIM_HYPERBOLOID
    /// - GT_PRIM_CIRCLE_MESH
    /// - GT_PRIM_SPHERE_MESH
    /// - GT_PRIM_TUBE_MESH
    /// - GT_PRIM_HYPERBOLOID_MESH
    virtual bool	isSimplePrimitive(int primitive_type) const;

    /// @{
    /// If the geometry being instanced comes from a GU_Detail, then the source
    /// geometry should be set to point to this geometry.  This lets viewport
    /// decorations be rendered properly.
    const GT_GEODetailListHandle	&sourceGeometry() const
					    { return mySourceGeometry; }
    void	 setSourceGeometry(const GT_GEODetailListHandle &g)
		    { mySourceGeometry = g; }
    /// @}
    
    virtual bool	updateGeoPrim(const GU_ConstDetailHandle &dtl,
				      const GT_RefineParms &refine);

protected:
    void	doEnlargeBounds(UT_BoundingBox boxes[], int nsegs,
			bool forrender) const;

    GT_PrimitiveHandle		myGeometry;
    GT_TransformArrayHandle	myTransforms;
    GT_GEOOffsetList		myPackedPrimOffsets;
    GT_AttributeListHandle	myUniform;
    GT_AttributeListHandle	myDetail;
    GT_GEODetailListHandle	mySourceGeometry;
};

#endif
