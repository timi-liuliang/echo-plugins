/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimCircle.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimCircle__
#define __GT_PrimCircle__

#include "GT_API.h"
#include "GT_PrimQuadric.h"
#include "GT_Parametric.h"
#include "GT_Transform.h"

/// @brief A circle primitive
///
/// The circle primitive represents a parametric patch of a circle/disk
/// primitive in the XY plane.
/// The circle is parametrized so @c u (0 to 1) represents radial
/// angle, while @c v (0 to 1) is goes from the center to the circumference.
/// That is: @code
///	theta = u * 2*PI
///	x = cos(u)*v
///	y = sin(u)*v
///	z = 0
/// @endcode
class GT_API GT_PrimCircle : public GT_PrimQuadric
{
public:
    /// Default constructor (full unit circle in the xz plane, no attributes)
    GT_PrimCircle()
    {}
    /// Useful constructor
    GT_PrimCircle(const GT_AttributeListHandle &attribs,
		    const GT_TransformHandle &transform,
		    const GT_Parametric &uv=GT_Parametric())
	: GT_PrimQuadric(attribs, transform, uv)
    { }
    /// Copy (but with a different parametric range)
    GT_PrimCircle(const GT_PrimCircle &src, const GT_Parametric &uv)
	: GT_PrimQuadric(src, uv)
    {
    }
    /// Copy constructor
    GT_PrimCircle(const GT_PrimCircle &src)
	: GT_PrimQuadric(src)
    {
    }
    /// Destructor
    virtual ~GT_PrimCircle();

    virtual const char *className() const { return "GT_PrimCircle"; }
    virtual bool	save(UT_JSONWriter &w) const
			    { return saveQuadric(w, "Circle"); }
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimCircle(*this); }
    virtual GT_PrimitiveHandle	doHarden() const;
    
    /// @{
    /// Methods defined on GT_Primitive
    virtual int			getPrimitiveType() const;
    /// @}

    /// When creating a polygon mesh from a circle, we can make a triangle fan
    /// instead of a mesh.
    virtual GT_PrimitiveHandle	refineToPolygonMesh(
					const GT_RefineParms *parms) const;

    GT_PrimitiveHandle		refineToCurve(
					const GT_RefineParms *parms) const;
    /// Initialize a circle with
    /// @param attribs @n
    ///	  Attributes defined on the circle
    /// @param transform @n
    ///	  The transform attribute list should contain a single transform
    ///	  representing the 4x4 transform of the circle.
    /// @param uv @n
    ///	  The parametric range for the circle.
    ///   The circle is parametrized so @c u (0 to 1) represents radial angle,
    ///   while @c v (0 to 1) is goes from the center to the circumference.
    bool	init(const GT_AttributeListHandle &attribs,
			const GT_TransformHandle &transform,
			const GT_Parametric &uv)
		{
		    if (!GT_PrimQuadric::init(attribs, transform, uv))
			return false;
		    return true;
		}

    /// Static method to compute bounds for a circle
    static void		computeCircleBounds(UT_BoundingBox &box,
				const GT_Parametric &uv);

    /// Get a tesselation of a full circle with a given set of refinement
    /// parameters.
    static GT_PrimitiveHandle	makePolygonMesh(const GT_RefineParms *parms,
				    UT_Axis3::axis axis = UT_Axis3::XAXIS,
				    fpreal axis_trans = 0.0f,
				    fpreal radius = 1.0f);
    static GT_PrimitiveHandle	makeCurve(const GT_RefineParms *parms, 
				    UT_Axis3::axis axis = UT_Axis3::XAXIS,
				    fpreal axis_trans = 0.0f,
				    fpreal radius = 1.0f);

protected:
    virtual GT_PrimQuadric	*clone(const GT_Parametric &uv) const
				{
				    return new GT_PrimCircle(*this, uv);
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
    virtual void	adjustTesselationCounts(GT_Size &nu, GT_Size &nv) const;
    /// @}
};

#endif
