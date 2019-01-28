/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimPatchMesh.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimPatchMesh__
#define __GT_PrimPatchMesh__

#include "GT_API.h"
#include "GT_DataArray.h"
#include "GT_CountArray.h"
#include "GT_Primitive.h"

class GT_PrimPatch;

/// A virtual container used by sphere, circle and tube meshes
class GT_API GT_PrimPatchMesh : public GT_Primitive
{
public:
    /// Default c-tor
    GT_PrimPatchMesh()
	: myUMerge(NULL)
    { }

    /// Construct a full list of patches.  All patches have the same basis.
    /// For @c npatches, the lengths of each array should be
    ///  - @nu, @uwrap, @nv, @vwrap @n
    ///    @c npatches (i.e. one for each patch)
    ///  - @vertex @n
    ///    One for each vertex in each patch.  That is sum(nu*nv)
    ///  - @uniform @n
    ///    One for each patch.  There is currently no way to specify an
    ///    attribute per-face for each patch in the patch mesh.  You can use a
    ///    collection of GT_PrimPatch objects if you need this behaviour.
    ///  - @detail @n
    ///    A single value
    GT_PrimPatchMesh(GT_Basis basis,
			const GT_DataArrayHandle &nu,
			const GT_DataArrayHandle &uwrap,
			const GT_DataArrayHandle &nv,
			const GT_DataArrayHandle &vwrap,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail)
	: myUMerge(NULL)
    {
	init(basis, nu, uwrap, nv, vwrap, vertex, uniform, detail);
    }

    GT_PrimPatchMesh(const GT_PrimPatchMesh &src)
	: GT_Primitive(src)
	, myNu(src.myNu)
	, myUWrap(src.myUWrap)
	, myNv(src.myNv)
	, myVWrap(src.myVWrap)
	, myVertexOffsets(src.myVertexOffsets)
	, myVertex(src.myVertex)
	, myUniform(src.myUniform)
	, myDetail(src.myDetail)
	, myUMerge(NULL)	// Uniform/Detail merge
	, myBasis(src.myBasis)
    {
    }
    GT_PrimPatchMesh(const GT_PrimPatchMesh &src,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail)
	: myNu(src.myNu)
	, myUWrap(src.myUWrap)
	, myNv(src.myNv)
	, myVWrap(src.myVWrap)
	, myVertexOffsets(src.myVertexOffsets)
	, myVertex(vertex)
	, myUniform(uniform)
	, myDetail(detail)
	, myUMerge(NULL)	// Uniform/Detail merge
	, myBasis(src.myBasis)
    {
    }

    virtual ~GT_PrimPatchMesh();

    /// Construct a mesh from a single patch
    GT_PrimPatchMesh(const GT_PrimPatch *q);

    virtual const char *className() const { return "GT_PrimPatchMesh"; }
    virtual bool	save(UT_JSONWriter &w) const
			{
			    jsonWriter	j(w, "PatchMesh");
			    return myNu->save(*j) &&
				    myNv->save(*j) &&
				    myUWrap->save(*j) &&
				    myVWrap->save(*j) &&
				    myVertexOffsets.save(*j) &&
				    saveAttributeLists(*j);
			}
    
    /// @{
    /// Methods defined on GT_Primitive
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
				    int nsegments) const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    virtual bool	refine(GT_Refine &refiner,
				const GT_RefineParms *parms) const;

    virtual int		getPrimitiveType() const;
    /// @}

    /// Initialize the quadric mesh
    bool	init(GT_Basis basis,
			const GT_DataArrayHandle &nu,
			const GT_DataArrayHandle &uwrap,
			const GT_DataArrayHandle &nv,
			const GT_DataArrayHandle &vwrap,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail);

    /// @{
    /// Accessors
    GT_Size				 getPatchCount() const
					    { return myNu ? myNu->entries():0; }
    GT_Basis				 getBasis() const
					    { return myBasis; }
    const GT_DataArrayHandle		&getNu() const
					    { return myNu; }
    const GT_DataArrayHandle		&getUWrap() const
					    { return myUWrap; }
    const GT_DataArrayHandle		&getNv() const
					    { return myNv; }
    const GT_DataArrayHandle		&getVWrap() const
					    { return myVWrap; }
    const GT_AttributeListHandle	&getVertex() const
					    { return myVertex; }
    const GT_AttributeListHandle	&getUniform() const
					    { return myUniform; }
    const GT_AttributeListHandle	&getDetail() const
					    { return myDetail; }
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

    /// Construct a single patch
    GT_PrimitiveHandle	makePatch(exint patch,
					const GT_RefineParms *parms) const;

    /// Harden all attributes so there are no dangling dependencies
    virtual GT_PrimitiveHandle	doHarden() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimPatchMesh(*this); }

    /// The virtual implementation of attribute merging
    virtual GT_PrimitiveHandle	doAttributeMerge(const GT_Primitive &src,
					const UT_StringMMPattern *vertex,
					const UT_StringMMPattern *point,
					const UT_StringMMPattern *uniform,
					const UT_StringMMPattern *detail) const;
protected:
    void		makeMergeMap();

    GT_DataArrayHandle		myNu;
    GT_DataArrayHandle		myUWrap;
    GT_DataArrayHandle		myNv;
    GT_DataArrayHandle		myVWrap;
    GT_CountArray		myVertexOffsets;
    GT_AttributeListHandle	myVertex;
    GT_AttributeListHandle	myUniform;
    GT_AttributeListHandle	myDetail;
    GT_AttributeMerge		*myUMerge;	// Uniform/Detail merge
    GT_Basis			myBasis;
};

#endif
