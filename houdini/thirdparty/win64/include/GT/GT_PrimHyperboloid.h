/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimHyperboloid.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimHyperboloid__
#define __GT_PrimHyperboloid__

#include "GT_API.h"
#include "GT_PrimQuadric.h"
#include <UT/UT_VectorTypes.h>

/// Generic hyperboloid primitive
///
/// The hyperboloid primitive is a parametric surface defined by revolving a
/// line segment around the z azis.
/// Given the hyperboloid is defined by points <em>p0</em>, <em>p1</em> and
/// parametric coordinates <em>u</em> and <em>v</em>: @code
///    fpreal	    theta = u * 2 * PI;
///    UT_Vector3D  P = lerp(p0, p1, v);
///    x = P.x() * cos(theta) - P.y() * sin(theta);
///    y = P.z() * sin(theta) + P.y() * cos(theta);
///    z = P.z();
/// @endcode
///
/// Examples:
/// - A tube @code
///	PrimHyperboloid( UT_Vector3(1, 0, -.5), UT_Vector3(1, 0, .5), ...);
/// @endcode
/// - A cone @code
///	PrimHyperboloid( UT_Vector3(0, 0, -.5), UT_Vector3(1, 0, .5), ...);
/// @endcode
/// - A circle @code
///	PrimHyperboloid( UT_Vector3(0, 0, 0), UT_Vector3(1, 0, 0), ...);
/// @endcode
/// 
class GT_API GT_PrimHyperboloid : public GT_PrimQuadric
{
public:
    GT_PrimHyperboloid()
    { }
    GT_PrimHyperboloid(const UT_Vector3D &p0,
			const UT_Vector3D &p1,
			const GT_AttributeListHandle &attribs,
			const GT_TransformHandle &transform,
			const GT_Parametric &uv=GT_Parametric())
	: GT_PrimQuadric(attribs, transform, uv)
	, myP0(p0)
	, myDP(p1 - p0)
    { }
    GT_PrimHyperboloid(const GT_PrimHyperboloid &src)
	: GT_PrimQuadric(src)
	, myP0(src.myP0)
	, myDP(src.myDP)
    { }
    GT_PrimHyperboloid(const GT_PrimHyperboloid &src, const GT_Parametric &uv)
	: GT_PrimQuadric(src, uv)
	, myP0(src.myP0)
	, myDP(src.myDP)
    { }
    virtual ~GT_PrimHyperboloid();

    virtual const char *className() const { return "GT_PrimHyperboloid"; }
    virtual bool	save(UT_JSONWriter &w) const
			{
			    jsonWriter	j(w, "Hyperboloid");
			    bool	ok = true;
			    ok = ok && getP0().save(w);
			    ok = ok && getP1().save(w);
			    ok = ok && myUV.save(w);
			    ok = ok && getPrimitiveTransform()->save(w);
			    return ok && saveAttributeLists(w);
			}
    
    /// @{
    /// Methods defined on GT_Primitive
    virtual int		getPrimitiveType() const;
    virtual GT_PrimitiveHandle	doHarden() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimHyperboloid(*this); }
    /// @}

    /// Initialize hyperboloid with
    ///	@param p0 @n
    ///	  First point defining hyperboloid axis
    ///	@param p1 @n
    ///	  Second point defining hyperboloid axis
    /// @param attribs @n
    ///	  Attributes defined on the hyperboloid
    /// @param transform @n
    ///	  Transform on the hyperboloid
    /// @param uv @n
    ///	  The parametric range for the hyperboloid.  For the hyperboloid, @b u
    ///	  coordinate represents the rotation, while @b v represents the
    ///	  interpolant between @c p0 and @c p1.
    bool	init(const UT_Vector3D &p0,
		     const UT_Vector3D &p1,
		     const GT_AttributeListHandle &attribs,
		     const GT_TransformHandle &transform,
		     const GT_Parametric &uv=GT_Parametric())
		{
		    if (!GT_PrimQuadric::init(attribs, transform, uv))
			return false;
		    myP0 = p0;
		    myDP = p1 - p0;
		    return true;
		}

    /// @{
    /// Accessor
    const UT_Vector3D	getP0() const	{ return myP0; }
    const UT_Vector3D	getP1() const	{ return myP0 + myDP; }
    const UT_Vector3D	getDP() const	{ return myDP; }
    /// @}

protected:
    /// Clone a new hyperbolic sheet with a different parametric range
    virtual GT_PrimQuadric	*clone(const GT_Parametric &uv) const
				{
				    return new GT_PrimHyperboloid(*this, uv);
				}

    /// Hyperboloids can be evaluated with just 2 rows of data
    virtual void	adjustTesselationCounts(GT_Size &nu, GT_Size &nv) const;
    /// @{
    /// Methods defined on GT_PrimQuadric
    virtual void	getQBounds(UT_BoundingBox &box) const;
    virtual void	fillQP(UT_Vector3F *P, GT_Size n,
				    const fpreal *u, fpreal v) const;
    virtual void	fillQP(UT_Vector3D *P, GT_Size n,
				    const fpreal *u, fpreal v) const;
    virtual void	fillQN(UT_Vector3F *N, GT_Size n,
				    const fpreal *u, fpreal v) const;
    virtual void	fillQN(UT_Vector3D *N, GT_Size n,
				    const fpreal *u, fpreal v) const;
    /// @}

private:
    UT_Vector3D		myP0;
    UT_Vector3D		myDP;
};

#endif

