/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimPatch.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimPatch__
#define __GT_PrimPatch__

#include "GT_API.h"
#include "GT_Primitive.h"

/// @brief A mesh of linear or cubic patches
class GT_API GT_PrimPatch : public GT_Primitive
{
public:
    GT_PrimPatch();
    GT_PrimPatch(const GT_PrimPatch &src)
	: GT_Primitive(src)
	, myVertex(src.myVertex)
	, myUniform(src.myUniform)
	, myDetail(src.myDetail)
	, myNu(src.myNu)
	, myNv(src.myNv)
	, myBasis(src.myBasis)
	, myUWrap(src.myUWrap)
	, myVWrap(src.myVWrap)
    {
    }
    GT_PrimPatch(const GT_PrimPatch &src,
		    const GT_AttributeListHandle &vertex,
		    const GT_AttributeListHandle &uniform,
		    const GT_AttributeListHandle &detail)
	: GT_Primitive(src)
	, myVertex(vertex)
	, myUniform(uniform)
	, myDetail(detail)
	, myNu(src.myNu)
	, myNv(src.myNv)
	, myBasis(src.myBasis)
	, myUWrap(src.myUWrap)
	, myVWrap(src.myVWrap)
    {
    }
    GT_PrimPatch(GT_Basis basis, int nu, bool uwrap, int nv, bool vwrap,
		    const GT_AttributeListHandle &vertex,
		    const GT_AttributeListHandle &uniform,
		    const GT_AttributeListHandle &detail)
    {
	init(basis, nu, uwrap, nv, vwrap, vertex, uniform, detail);
    }
    virtual ~GT_PrimPatch();

    virtual const char *className() const { return "GT_PrimPatch"; }
    virtual bool	save(UT_JSONWriter &w) const;
    
    /// @{
    /// Methods defined on GT_Primitive
    virtual void		enlargeBounds(UT_BoundingBox boxes[],
					int nsegments) const;
    virtual int			getPrimitiveType() const;
    virtual bool		refine(GT_Refine &refiner,
					const GT_RefineParms *parms) const;
    virtual int			getMotionSegments() const;
    virtual int64		getMemoryUsage() const;
    /// @}

    /// Construct a simple patch:
    /// - @c nu @n Number of columns
    /// - @c nv @n Number of rows
    /// - @c uwrap @n
    ///   Whether the patch is connected between the last and first columns.
    /// - @c vwrap @n
    ///	  Whether the patch is connected between the last and first rows.
    /// - @c vertex @n Attribute data per vertex (nu * nv elements)
    /// - @c uniform @n Uniform values for patch faces.  The number of
    /// entries should be:
    ///  - Linear:  (nu + uwrapped - 1) * (nv + vwrapped - 1)
    bool	init(GT_Basis basis,
		    int nu, bool uwrap,
		    int nv, bool vwrap,
		    const GT_AttributeListHandle &vertex,
		    const GT_AttributeListHandle &uniform,
		    const GT_AttributeListHandle &detail);

    /// @{
    /// Construct a curve mesh from a patch of vertices
    static GT_PrimitiveHandle	makePatchRows(GT_Basis basis,
				    int nu, bool uwrap,
				    int nv, bool vwrap,
				    const GT_AttributeListHandle &vertex,
				    const GT_AttributeListHandle &detail);
    static GT_PrimitiveHandle	makePatchCols(GT_Basis basis,
				    int nu, bool uwrap,
				    int nv, bool vwrap,
				    const GT_AttributeListHandle &vertex,
				    const GT_AttributeListHandle &detail);
    static GT_PrimitiveHandle	makePatchRowsCols(GT_Basis basis,
				    int nu, bool uwrap,
				    int nv, bool vwrap,
				    const GT_AttributeListHandle &vertex,
				    const GT_AttributeListHandle &detail);
    static GT_PrimitiveHandle	makePatchTriangles(
				    int nu, bool uwrap,
				    int nv, bool vwrap,
				    const GT_AttributeListHandle &vertex,
				    const GT_AttributeListHandle &detail);
    static GT_PrimitiveHandle	makePatchAlternatingTriangles(
				    int nu, bool uwrap,
				    int nv, bool vwrap,
				    const GT_AttributeListHandle &vertex,
				    const GT_AttributeListHandle &detail);
    static GT_PrimitiveHandle	makePatchReverseTriangles(
				    int nu, bool uwrap,
				    int nv, bool vwrap,
				    const GT_AttributeListHandle &vertex,
				    const GT_AttributeListHandle &detail);
    /// @}

    /// @{
    /// Accessor
    GT_Basis		getBasis() const	{ return myBasis; }
    int			getNu() const		{ return myNu; }
    int			getNv() const		{ return myNv; }
    bool		getUWrap() const	{ return myUWrap; }
    bool		getVWrap() const	{ return myVWrap; }
    const GT_AttributeListHandle &getVertex() const	{ return myVertex; }
    const GT_AttributeListHandle &getUniform() const	{ return myUniform; }
    const GT_AttributeListHandle &getDetail() const	{ return myDetail; }
    int			getNuWrapped() const { return myUWrap ? myNu+1 : myNu; }
    int			getNvWrapped() const { return myVWrap ? myNv+1 : myNv; }
    /// @}

    /// Harden all attributes so there are no dangling dependencies
    virtual GT_PrimitiveHandle	doHarden() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimPatch(*this); }

    /// The virtual implementation of attribute merging
    virtual GT_PrimitiveHandle	doAttributeMerge(const GT_Primitive &src,
					const UT_StringMMPattern *vertex,
					const UT_StringMMPattern *point,
					const UT_StringMMPattern *uniform,
					const UT_StringMMPattern *detail) const;

    /// @{
    /// Access attributes
    virtual const GT_AttributeListHandle	&getVertexAttributes() const
						    { return myVertex; }
    virtual const GT_AttributeListHandle	&getUniformAttributes() const
						    { return myUniform; }
    virtual const GT_AttributeListHandle	&getDetailAttributes() const
						    { return myDetail; }
    /// @}

    /// Refine to a single polygon mesh.
    bool	refineToPolyMesh(GT_Refine &refine,
			    const GT_RefineParms *parms=NULL) const;
private:
    GT_AttributeListHandle	myVertex;
    GT_AttributeListHandle	myUniform;
    GT_AttributeListHandle	myDetail;
    int				myNu, myNv;
    GT_Basis			myBasis;
    bool			myUWrap, myVWrap;
};

#endif
