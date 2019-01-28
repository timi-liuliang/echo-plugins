/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_Generic.h (BRAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __BRAY_Generic__
#define __BRAY_Generic__

#include "BRAY_API.h"
#include "BRAY_Hit.h"
#include "BRAY_Object.h"
#include "BRAY_AttribList.h"

class BRAY_Scene;
class BRAY_VexMaterial;

class BRAY_API BRAY_Generic : public BRAY_Object
{
public:
    /// Templatized on <fpreal32> or <fpreal64>
    /// This structure is used in the intersect() function to compute an
    /// intersection with the generic primitive.
    template <typename PREC>
    struct Ray
    {
	// RayType
	const UT_Vector3T<PREC>	&org;
	const UT_Vector3T<PREC>	&dir;
	fpreal			 time;
    };
    using Ray32 = Ray<fpreal32>;
    using Ray64 = Ray<fpreal64>;

    /// This class stores information about the hit.  The information is passed
    /// back to the generic primitive when evaluating attributes.
    /// The generic object can store any data it wants in the @c u, @c v, @c w
    /// and @c geom_id fields.
    struct Hit
    {
	Hit(fpreal d=SYS_FPREAL_MAX)
	    : distance(d)
	{
	}
	fpreal		distance;	// Distance to hit
	fpreal		u, v, w;	// Parametric coordinates
	UT_Vector3	Ng;		// Geometric normal
	uint		prim_id;	// ID of primitive in generic
    };

    BRAY_Generic();
    virtual ~BRAY_Generic();

    /// @{
    /// Interface from BRAY_Object
    virtual ObjectType	 objectType() const override final { return Generic; }
    virtual const char	*className() const override { return "BRAY_Generic"; }
    /// @}

    /// Return an attribute list for the attributes supported by this primitive.
    virtual const BRAY_AttribList	*attribList() const = 0;

    /// Return the bounding box for the object at a given time
    virtual void	bounds(UT_BoundingBox &bounds, fpreal time) const = 0;

    /// @{
    /// Perform ray-intersection
    ///
    /// If the distanct to the closest intersection point for the generic
    /// object is closer than the @c distance already stored in the @c
    /// hit_info, the hit information should be replaced and the @c intersect()
    /// method should return @c true.
    ///
    /// If the distance to the closest intersection point is farther than the
    /// existing @c distance, then the @c hit_info should remain unchanged.
    virtual bool	intersect(const Ray32 &ray,
				Hit &hit_info) const = 0;
    virtual bool	intersect(const Ray64 &ray,
				Hit &hit_info) const = 0;
    /// @}

    /// Return a display color for low-quality rendering
    virtual UT_Vector3	displayColor() const { return UT_Vector3(1, 1, 1); }

    /// @{
    /// Evaluate an attribute on this primitive.  The methods should return a
    /// pointer to the attribute data.  If there isn't a direct raw pointer to
    /// the data, the buffer passed in can be used to store the data and that
    /// pointer can be returned.
    /// The @c element is the element
    virtual const int32		*attribVal(int attrib, fpreal time,
				    const Ray32 &ray, const Hit &hit_info,
				    int32 *buf, int size) const = 0;
    virtual const int64		*attribVal(int attrib, fpreal time,
				    const Ray32 &ray, const Hit &hit_info,
				    int64 *buf, int size) const = 0;
    virtual const fpreal32	*attribVal(int attrib, fpreal time,
				    const Ray32 &ray, const Hit &hit_info,
				    fpreal32 *buf, int size) const = 0;
    virtual const fpreal64	*attribVal(int attrib, fpreal time,
				    const Ray32 &ray, const Hit &hit_info,
				    fpreal64 *buf, int size) const = 0;
    virtual const UT_StringHolder *attribVal(int attrib, fpreal time,
				    const Ray32 &ray, const Hit &hit_info,
				    UT_StringHolder *buf, int size) const = 0;
    /// @}

    /// This method can be used to be notified of changes to the object
    virtual void	update(BRAY_EventType event) = 0;

    /// @{
    /// Material access
    const BRAY_VexMaterial	*material() const { return myMaterial; }
    void			 setMaterial(const BRAY_Scene &scene,
					BRAY_VexMaterial *mat);
    int				 shadingId() const { return myShadingId; }
    /// @}

protected:
    /// Interface from BRAY_Object for the @c dump() methods.  By default,
    /// nothing is printed out.
    virtual void	dumpKeys(UT_JSONWriter &w) const override;

    /// This method will be called to print out an error if the generic isn't
    /// set up properly.  You can override it to silence the errors.
    virtual void	errorMessage(const char *fmt, ...)
			    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);

protected:
    const BRAY_VexMaterial	*myMaterial;
    int				 myShadingId;
};

#endif
