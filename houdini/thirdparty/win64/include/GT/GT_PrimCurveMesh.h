/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimCurveMesh.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimCurveMesh__
#define __GT_PrimCurveMesh__

#include "GT_API.h"
#include "GT_Primitive.h"
#include "GT_CountArray.h"
#include "GT_FaceSetMap.h"

class GT_DataArray;
class GT_AttributeList;
class GT_AttributeMerge;
class GT_PrimCurve;

/// @brief A mesh of curves
class GT_API GT_PrimCurveMesh : public GT_Primitive
{
public:
    /// Default constructor
    GT_PrimCurveMesh();
    /// Copy c-tor
    GT_PrimCurveMesh(const GT_PrimCurveMesh &src)
	: GT_Primitive(src)
	, myCurveOffsets(src.myCurveOffsets)
	, myKnotOffsets(src.myKnotOffsets)
	, myKnots(src.myKnots)
	, myVertex(src.myVertex)
	, myUniform(src.myUniform)
	, myDetail(src.myDetail)
	, myUMerge(NULL)
	, myFaceSetMap(src.myFaceSetMap)
	, myBasis(src.myBasis)
	, myOrder(src.myOrder)
	, myUniformOrder(src.myUniformOrder)
	, myWrap(src.myWrap)
    {
    }
    GT_PrimCurveMesh(const GT_PrimCurveMesh &src,
	    const GT_AttributeListHandle &vattrib,
	    const GT_AttributeListHandle &uattrib,
	    const GT_AttributeListHandle &dattrib)
	: GT_Primitive(src)
	, myCurveOffsets(src.myCurveOffsets)
	, myKnotOffsets(src.myKnotOffsets)
	, myKnots(src.myKnots)
	, myVertex(vattrib)
	, myUniform(uattrib)
	, myDetail(dattrib)
	, myUMerge(NULL)
	, myFaceSetMap(src.myFaceSetMap)
	, myBasis(src.myBasis)
	, myOrder(src.myOrder)
	, myUniformOrder(src.myUniformOrder)
	, myWrap(src.myWrap)
    {
    }
    GT_PrimCurveMesh(const GT_PrimCurveMesh &src,
	    GT_Basis basis,
	    const GT_CountArray &counts,
	    const GT_AttributeListHandle &vattrib,
	    const GT_AttributeListHandle &uattrib,
	    const GT_AttributeListHandle &dattrib,
	    bool wrap,
	    const GT_FaceSetMapPtr &fsets)
	: GT_Primitive(src)
	, myCurveOffsets(counts)
	, myVertex(vattrib)
	, myUniform(uattrib)
	, myDetail(dattrib)
	, myUMerge(NULL)
	, myFaceSetMap(fsets)
	, myBasis(basis)
	, myUniformOrder(basis == GT_BASIS_LINEAR ? 2 : 4)
	, myWrap(wrap)
    {
    }
    /// Useful constructor
    GT_PrimCurveMesh(GT_Basis basis,
			const GT_DataArrayHandle &vtx_counts,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail,
			bool wrap,
			GT_IndexingMode indexing=GT_INDEXING_QUICK)
	: myUMerge(NULL)
	, myKnotOffsets()
	, myKnots()
	, myUniformOrder(basis == GT_BASIS_LINEAR ? 2 : 4)
    {
	init(basis, vtx_counts, vertex, uniform, detail, wrap, indexing);
    }
    /// Construct a curve mesh from a single GT_PrimCurve
    GT_PrimCurveMesh(const GT_PrimCurve &curve);
    
    /// Destructor
    virtual ~GT_PrimCurveMesh();

    virtual const char *className() const { return "GT_PrimCurveMesh"; }
    virtual bool	save(UT_JSONWriter &w) const;
    
    /// @{
    /// Methods defined on GT_Primitive
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
						int nseg) const;
    virtual void	enlargeRenderBounds(UT_BoundingBox b[], int n) const;
    virtual int		getPrimitiveType() const;
    virtual bool	refine(GT_Refine &refiner,
				const GT_RefineParms *parms) const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    virtual const GT_ViewportRefineOptions	&viewportRefineOptions() const;
    /// @}

    /// Initialize the mesh
    /// - @c vtx_counts @n
    ///   An integer array, representing the number of vertices in each curve.
    ///   The length of the array determines the number of curves in the mesh.
    /// - @c vertex @n
    ///   Unique data per vertex.  The length of these attribute arrays should
    ///   be the sum of the vertex_counts array.
    /// - @c uniform @n
    ///   Attribute per curve.  There should be one entry for each entry in the
    ///   @c vtx_counts array.
    /// - @c indexing @n
    ///   What type of indexing is required
    void		init(GT_Basis basis,
			    const GT_DataArrayHandle &vtx_counts,
			    const GT_AttributeListHandle &vertex,
			    const GT_AttributeListHandle &uniform,
			    const GT_AttributeListHandle &detail,
			    bool wrap,
			    GT_IndexingMode indexing=GT_INDEXING_QUICK);

    /// Set knot vector.  Some conditions must be met for this to work properly.
    ///
    /// - The basis must be GT_BASIS_BSPLINE
    /// - The storage for the knots must be floating point
    /// - The curve will have periodicity turned off.  To create a periodic
    ///   curve, you must duplicate the first @c (order-1) vertices and adjust
    ///   the knot vector so the knot spacing for the last @c order knots
    ///   matches the spacing for the first @c order knots.
    /// - The length of the knot vector must be @n
    ///     <tt>sum(getVertexCount(i)) + sum(getOrder(i))</tt> @n
    ///   That is, the length of the knot vector for each curve must be the
    ///   number of vertices in the curve plus its order.
    ///
    ///   @note In Houdini curves, the first vertex is implicitly duplicated
    ///   for periodic NURBS curves.  This is @b not the case for GT, where the
    ///   first vertex must be explicitly duplicated.
    bool		setKnots(const GT_DataArrayHandle &knots);

    /// @{
    /// Set the basis & order for the curves
    /// Basis Type              Min Order       Max Order
    /// GT_BASIS_LINEAR         2               2
    /// GT_BASIS_BEZIER         2               31
    /// GT_BASIS_BSPLINE        2               arbitrary
    /// GT_BASIS_CATMULLROM     4               4
    /// GT_BASIS_HERMITE        4               4
    /// GT_BASIS_POWER          2               arbitrary
    /// @note Changing the order will clear the knot vector (if one exists)
    bool	setBasis(GT_Basis basis, int order=4);
    bool	setBasis(GT_Basis basis, const GT_DataArrayHandle &order);
    /// @}

    /// @{
    /// Set the curve order
    bool	setOrder(int order)
		    { return setBasis(getBasis(), order); }
    bool	setOrder(const GT_DataArrayHandle &order)
		    { return setBasis(getBasis(), order); }
    /// @}


    /// Return the number of faces in the mesh
    GT_Size			getCurveCount() const;
    /// Return a pointer to the individual face
    GT_PrimitiveHandle		getCurve(GT_Offset i) const;

    /// Refine to linear curves
    GT_PrimitiveHandle	refineToLinear(const GT_RefineParms *parms=NULL,
					fpreal lod_scale = -1) const;
    /// @{
    /// Accessors
    GT_Basis			 getBasis() const
				    { return myBasis; }
    const GT_CountArray		&getCurveCountArray() const
				    { return myCurveOffsets; }
    const GT_AttributeListHandle &getVertex() const
				    { return myVertex; }
    const GT_AttributeListHandle &getUniform() const
				    { return myUniform; }
    const GT_AttributeListHandle &getDetail() const
				    { return myDetail; }
    bool			 getWrap() const
				    { return myWrap; }
    const GT_DataArrayHandle	knots() const { return myKnots; }
    GT_Offset			knotOffset(GT_Offset curve) const
    {
	return myKnots ? myKnotOffsets.getOffset(curve) : 0;
    }
    /// @}

    /// Return the order for a given curve
    int		getOrder(GT_Offset curve) const
		{
		    return (myUniformOrder > 0) ? myUniformOrder
						: myOrder->getI32(curve);
		}
    /// Check to see whether all curves have the same order
    bool	isUniformOrder() const	{ return myUniformOrder > 0; }

    /// Return the order for all curves.   If the curves have varying orders,
    /// the uniform order will be less than zero.
    int				uniformOrder() const { return myUniformOrder; }

    /// Return the varying orders (may be NULL)
    const GT_DataArrayHandle	&varyingOrders() const	{ return myOrder; }

    /// Harden all attributes so there are no dangling dependencies
    virtual GT_PrimitiveHandle	doHarden() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimCurveMesh(*this); }

    /// The virtual implementation of attribute merging
    virtual GT_PrimitiveHandle	doAttributeMerge(const GT_Primitive &src,
					const UT_StringMMPattern *vertex,
					const UT_StringMMPattern *point,
					const UT_StringMMPattern *uniform,
					const UT_StringMMPattern *detail) const;

    /// @{
    /// Access to the face sets (curve sets)
    const GT_FaceSetMapPtr	&faceSetMap() const	{ return myFaceSetMap; }
    void			 setFaceSetMap(const GT_FaceSetMapPtr &v)
					{ myFaceSetMap = v; }
    void			 addFaceSet(const char *name,
					const GT_FaceSetPtr &set);
    /// @}

    /// @{
    /// Access attributes
    virtual const GT_AttributeListHandle	&getVertexAttributes() const
						    { return myVertex; }
    virtual const GT_AttributeListHandle	&getUniformAttributes() const
						    { return myUniform; }
    virtual const GT_AttributeListHandle	&getDetailAttributes() const
						    { return myDetail; }
    /// @}

    /// Return the offset into the vertex list for the given face
    GT_Offset		getVertexOffset(GT_Offset face) const
			    { return myCurveOffsets.getOffset(face); }
    /// Return the length of the vertex list for the given face
    GT_Size		getVertexCount(GT_Offset face) const
			    { return myCurveOffsets.getCount(face); }

    /// Return an array containing the face counts per-face
    /// @warning This is expensive and will create a separate numeric array
    const GT_DataArrayHandle
		getCurveCounts(GT_IndexingMode mode=GT_INDEXING_QUICK) const;

    /// GEO/GU support to compute perimeter of curve
    virtual fpreal	computePerimeter(int seg) const;

private:
    void		makeMerge();

    GT_CountArray		 myCurveOffsets;
    GT_CountArray		 myKnotOffsets;
    GT_DataArrayHandle		 myKnots;
    GT_DataArrayHandle		 myOrder;
    GT_AttributeListHandle	 myVertex;
    GT_AttributeListHandle	 myUniform;
    GT_AttributeListHandle	 myDetail;
    GT_FaceSetMapPtr		 myFaceSetMap;
    GT_AttributeMerge		*myUMerge;
    GT_Basis			 myBasis;
    int				 myUniformOrder;
    bool			 myWrap;
};

#endif
