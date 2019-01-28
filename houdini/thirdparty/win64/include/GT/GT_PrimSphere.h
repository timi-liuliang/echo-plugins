/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimSphere.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimSphere__
#define __GT_PrimSphere__

#include "GT_API.h"
#include "GT_PrimQuadric.h"
#include "GT_Parametric.h"
#include "GT_Transform.h"

/// @brief A sphere primitive
///
/// The sphere primitive represents a parametric patch of a sphere primitive
/// The sphere is parametrized so @c u (0 to 1) represents longitude, while
/// @c v (0 to 1) is goes from pole to pole.
/// That is: @code
///	theta = u * 2*PI
///	phi = v * PI
///	x = cos(theta)*cos(phi)
///	y = sin(theta)*cos(phi)
///	z = sin(phi)
/// @endcode
class GT_API GT_PrimSphere : public GT_PrimQuadric
{
public:
    /// Default constructor (full unit sphere, no attributes)
    GT_PrimSphere()
	: GT_PrimQuadric()
    {}
    /// Useful constructor
    GT_PrimSphere(const GT_AttributeListHandle &attribs,
		    const GT_TransformHandle &transform,
		    const GT_Parametric &uv=GT_Parametric())
	: GT_PrimQuadric(attribs, transform, uv)
    { }
    /// Copy constructor with different parametric range
    GT_PrimSphere(const GT_PrimSphere &src, const GT_Parametric &uv)
	: GT_PrimQuadric(src, uv)
    {
    }
    /// Copy constructor
    GT_PrimSphere(const GT_PrimSphere &src)
	: GT_PrimQuadric(src)
    {
    }
    /// Destructor
    virtual ~GT_PrimSphere();

    virtual const char *className() const { return "GT_PrimSphere"; }
    virtual bool	save(UT_JSONWriter &w) const
			    { return saveQuadric(w, "Sphere"); }
    
    /// @{
    /// Methods defined on GT_Primitive
    virtual int			getPrimitiveType() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimSphere(*this); }
    virtual GT_PrimitiveHandle	doHarden() const;
    /// @}

    /// Initialize a sphere with
    /// @param attribs @n
    ///	  Attributes defined on the sphere
    /// @param transform @n
    ///	  The transform attribute list should contain a single transform
    ///	  representing the 4x4 transform of the sphere.
    /// @param uv @n
    ///	  The parametric range for the sphere.  The sphere is parametrized so
    ///	  @c u (0 to 1) represents longitude, while @c v (0 to 1) is goes from
    ///	  pole to pole.
    bool	init(const GT_AttributeListHandle &attribs,
			const GT_TransformHandle &transform,
			const GT_Parametric &uv)
		{
		    if (!GT_PrimQuadric::init(attribs, transform, uv))
			return false;
		    return true;
		}

    /// Static method to compute bounds for a sphere
    static void		computeSphereBounds(UT_BoundingBox &box,
				const GT_Parametric &uv);

    /// Get a tesselation of a full sphere with a given set of refinement
    /// parameters.
    static GT_PrimitiveHandle	makePolygonMesh(const GT_RefineParms *parms);

protected:
    virtual GT_PrimQuadric	*clone(const GT_Parametric &uv) const
				{
				    return new GT_PrimSphere(*this, uv);
				}
    /// @{
    /// Methods defined on GT_PrimQuadric
    virtual void	getQBounds(UT_BoundingBox &box) const;
    virtual void	fillQP(UT_Vector3F *P, GT_Size n,
				const fpreal *u, fpreal v) const;
    virtual void	fillQP(UT_Vector3D *P, GT_Size n,
				const fpreal *u, fpreal v) const;
    virtual void	fillQN(UT_Vector3F *P, GT_Size n,
				const fpreal *u, fpreal v) const;
    virtual void	fillQN(UT_Vector3D *P, GT_Size n,
				const fpreal *u, fpreal v) const;
    /// @}
};

#endif
