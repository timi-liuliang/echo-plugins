/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimQuadricMesh.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimQuadricMesh__
#define __GT_PrimQuadricMesh__

#include "GT_API.h"
#include "GT_Primitive.h"
#include "GT_TransformArray.h"

class GT_PrimQuadric;

/// A virtual container used by sphere, circle and tube meshes
class GT_API GT_PrimQuadricMesh : public GT_Primitive
{
public:
    /// Default c-tor
    GT_PrimQuadricMesh()
    { }

    GT_PrimQuadricMesh(const GT_PrimQuadricMesh &src)
	: GT_Primitive(src)
	, myVertex(src.myVertex)
	, myDetail(src.myDetail)
	, myTransforms(src.myTransforms)
    {
    }

    /// Construct a full list of quadrics
    GT_PrimQuadricMesh(const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &detail,
			const GT_TransformArrayHandle &xforms)
    {
	init(vertex, detail, xforms);
    }

    /// Construct a mesh from a single quadric
    GT_PrimQuadricMesh(const GT_PrimQuadric *q);

    virtual const char *className() const { return "GT_PrimQuadricMesh"; }
    bool	saveQuadricMesh(UT_JSONWriter &w, const char *name) const
		{
		    jsonWriter	j(w, name);
		    return myTransforms->save(*j) &&
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

    virtual int		getPrimitiveType() const = 0;
    /// @}

    /// Initialize the quadric mesh
    bool	init(const GT_AttributeListHandle &vertex,
		    const GT_AttributeListHandle &detail,
		    const GT_TransformArrayHandle &xforms);

    /// @{
    /// Accessors
    GT_Size				 getQuadricCount() const;
    const GT_AttributeListHandle	&getVertex() const
					    { return myVertex; }
    const GT_AttributeListHandle	&getDetail() const
					    { return myDetail; }
    const GT_TransformArrayHandle	&getTransforms() const
					    { return myTransforms; }
    const GT_TransformHandle		&getTransform(GT_Size i) const
					    { return myTransforms->get(i); }
    /// @}

    /// @{
    /// Access attributes
    virtual const GT_AttributeListHandle	&getVertexAttributes() const
						    { return myVertex; }
    virtual const GT_AttributeListHandle	&getDetailAttributes() const
						    { return myDetail; }
    /// @}

    virtual bool	updateGeoPrim(const GU_ConstDetailHandle &dtl,
				      const GT_RefineParms &refine);

    virtual bool	getBaseTransform(UT_Matrix4D &) const
			    { return false; }
    
protected:
    /// Sub-class method to compute a bounding box for a single non-transformed
    /// element.
    virtual void	getQBounds(UT_BoundingBox &box) const = 0;

    /// Method to harden attributes
    void		hardenAttributes();

    /// Sub-class method to make an individual primitive
    virtual GT_PrimitiveHandle	makeQuadric(GT_Size index,
					const GT_AttributeListHandle &h,
					const GT_TransformHandle &x,
					const GT_RefineParms *parms) const = 0;

    GT_AttributeListHandle	myVertex;
    GT_AttributeListHandle	myDetail;
    GT_TransformArrayHandle	myTransforms;
};

#endif
