/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimQuadric.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimQuadric__
#define __GT_PrimQuadric__

#include "GT_API.h"
#include "GT_Primitive.h"
#include "GT_Parametric.h"
#include "GT_Transform.h"

class GT_API GT_PrimQuadric : public GT_Primitive
{
public:
    GT_PrimQuadric()
	: myAttributes()
	, myUV()
    {}
    GT_PrimQuadric(const GT_AttributeListHandle &attribs,
		    const GT_TransformHandle &transform,
		    const GT_Parametric &uv=GT_Parametric())
	: myAttributes(attribs)
    {
        setPrimitiveTransform(transform);
    }
    GT_PrimQuadric(const GT_PrimQuadric &src)
	: myAttributes(src.myAttributes)
	, myUV(src.myUV)
	, GT_Primitive(src)
    {
    }
    /// Copy the quadric, but with a different parametric range
    GT_PrimQuadric(const GT_PrimQuadric &src, const GT_Parametric &uv)
	: myAttributes(src.myAttributes)
	, myUV(uv)
	, GT_Primitive(src)
    {
    }
    virtual ~GT_PrimQuadric();

    virtual const char *className() const { return "GT_PrimQuadric"; }
    bool		saveQuadric(UT_JSONWriter &w, const char *name) const
			{
			    jsonWriter	j(w, name);
			    return myUV.save(*j) &&
				    getPrimitiveTransform()->save(*j) &&
				    saveAttributeLists(*j);
			}
    
    /// @{
    /// Methods defined on GT_Primitive
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
					int nsegments) const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    /// @}

    /// Initialize the quadric.
    /// - @c attribs @n The attributes attached to the sphere
    /// - @c transfrom @n An optional transform attribute.
    /// The transform should be a 4x4 matrix.
    bool	init(const GT_AttributeListHandle &attribs,
			const GT_TransformHandle &transform,
			const GT_Parametric &uv);

    /// @{
    /// Query state
    const GT_AttributeListHandle	&getAttributes() const
					    { return myAttributes; }
    const GT_Parametric			&getParametric() const
					    { return myUV; }
    /// @}

    /// @{
    /// Access attributes
    virtual const GT_AttributeListHandle	&getDetailAttributes() const
						    { return myAttributes; }
    /// @}

    /// @{
    /// Split in U or V
    GT_PrimitiveHandle	splitU()
			{
			    GT_Parametric	right = myUV.splitU();
			    return GT_PrimitiveHandle(clone(right));
			}
    GT_PrimitiveHandle	splitV()
			{
			    GT_Parametric	top = myUV.splitV();
			    return GT_PrimitiveHandle(clone(top));
			}
    /// @}

    /// Method defined on GT_Primitive
    virtual bool	refine(GT_Refine &refiner,
				const GT_RefineParms *parms) const;

    /// Viewport refinement options
    virtual const GT_ViewportRefineOptions	&viewportRefineOptions() const;

    /// Convenience method to refine to a polygon mesh
    virtual GT_PrimitiveHandle	refineToPolygonMesh(
				    const GT_RefineParms *parms) const;

    /// @{
    /// Convert from parametric coordinates to position/normal.  These methods
    /// invoke the optimized "row" methods.
    void		getQP(UT_Vector3F &P, fpreal u, fpreal v) const
			    { fillQP(&P, 1, &u, v); }
    void		getQP(UT_Vector3D &P, fpreal u, fpreal v) const
			    { fillQP(&P, 1, &u, v); }
    void		getQN(UT_Vector3F &N, fpreal u, fpreal v) const
			    { fillQN(&N, 1, &u, v); }
    void		getQN(UT_Vector3D &N, fpreal u, fpreal v) const
			    { fillQN(&N, 1, &u, v); }
    /// @}

    /// ------------- Sub-class methods ---------------------
    /// Sub-class method to get the bounding box
    virtual void		getQBounds(UT_BoundingBox &box) const = 0;

    /// @{
    /// Evaluate a "row" of position/normal data.  The @c u data will have been
    /// processed by @c prepareParametricU().
    virtual void		fillQP(UT_Vector3F *P, GT_Size n,
					const fpreal *u, fpreal v) const=0;
    virtual void		fillQP(UT_Vector3D *P, GT_Size n,
					const fpreal *u, fpreal v) const=0;
    virtual void		fillQN(UT_Vector3F *P, GT_Size n,
					const fpreal *u, fpreal v) const=0;
    virtual void		fillQN(UT_Vector3D *P, GT_Size n,
					const fpreal *u, fpreal v) const=0;
    /// @}

    /// Given the u/v tesselation parameters, the sub-class can alter them if
    /// it chooses.
    virtual void		adjustTesselationCounts(GT_Size &nu,
							GT_Size &nv) const;

protected:
    /// Create a split primitive
    virtual GT_PrimQuadric	*clone(const GT_Parametric &uv) const = 0;

    /// Harden attributes etc.
    void		hardenAttributes();

    /// This method is called by the quadric refiner and may be called if you
    /// choose to override the refine() method on the sub-class.
    virtual GT_PrimitiveHandle	createMesh(GT_Size nu, GT_Size nv,
					fpreal u0, fpreal u1,
					fpreal v0, fpreal v1,
					bool create_normals=true,
					bool create_uv=true,
					GT_Storage Pstorage=GT_STORE_REAL32,
					GT_Storage Nstorage=GT_STORE_REAL32) const;

    /// Extract refinement parameters
    static void		extractRefineParms(const GT_RefineParms *parms,
				    GT_Size &nu, GT_Size &nv,
				    fpreal &u0, fpreal &u1,
				    fpreal &v0, fpreal &v1);

    GT_AttributeListHandle	myAttributes;	// Uniform attributes
    GT_Parametric		myUV;
};

#endif
