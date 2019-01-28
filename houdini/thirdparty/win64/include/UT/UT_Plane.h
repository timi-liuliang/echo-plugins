/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Plane.h (UT Library, C++)
 *
 * COMMENTS:
 *	A 3D plane.
 *
 */

#ifndef __UT_Plane_h__
#define __UT_Plane_h__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_Axis.h"
#include "UT_Vector3.h"
#include "UT_VectorTypes.h"
#include <SYS/SYS_Types.h>

// The 3 classic types of planes. Evidently, a UT_PlaneT object can be
// an arbitrary plane, but it helps to have some defaults.
enum UT_PlaneType
{
    UT_PLANE_XY,
    UT_PLANE_YZ,
    UT_PLANE_XZ
};

template <typename T>
class UT_PlaneT
{
public:

    // Several constructors that define a plane as (X - point)*normal = 0
			    UT_PlaneT(
				UT_PlaneType plane_type = UT_PLANE_XY);
			    UT_PlaneT(
				const UT_Vector3T<T> &pt,
				UT_PlaneType plane_type = UT_PLANE_XY);
			    UT_PlaneT(
				const UT_Vector3T<T> &apoint,
				const UT_Vector3T<T> &norm_dir,
				bool norm = true);
			    UT_PlaneT(
				const UT_Vector3T<T> &p0,
				const UT_Vector3T<T> &p1,
				const UT_Vector3T<T> &p2);
			    UT_PlaneT(
				const UT_Vector4T<T> &p);

			    ~UT_PlaneT();

    // Intersect a line defined parametrically as 'p + t*v' with this plane.
    // If the line runs parallel to the plane, both methods return "max"
    // as the intersection point. The second method also returns -1; if not
    // parallel, it returns 0.
    UT_Vector3T<T>	    intersect(
				const UT_Vector3T<T> &p,
				const UT_Vector3T<T> &v) const;
    int			    intersectLine(
				const UT_Vector3T<T> &p,
				const UT_Vector3T<T> &v, 
				UT_Vector3T<T> &hit) const;
    int			    intersectRay(
				const UT_Vector3T<T> &p,
				const UT_Vector3T<T> &v, 
				UT_Vector3T<T> &hit) const;

    // We occasionally need to find the intersection of a line with this
    // plane shifted to another position (ie. myPoint changes but myNormal
    // doesn't). offset_pt is a point on the shifted plane.
    int			    intersectLine(
				const UT_Vector3T<T> &offset_pt,
				const UT_Vector3T<T> &p,
				const UT_Vector3T<T> &v,
				UT_Vector3T<T> &hit) const;
    int	    		    intersectRay(
				const UT_Vector3T<T> &offset_pt,
				const UT_Vector3T<T> &p,
				const UT_Vector3T<T> &v,
				UT_Vector3T<T> &hit) const;

    // Find the projection of point p on the plane. The second method
    // returns the signed distance to the plane.
    UT_Vector3T<T>	    project(const UT_Vector3T<T> &p) const;
    T			    project(
				const UT_Vector3T<T> &p,
				UT_Vector3T<T> &projection) const;

    // Find the point symmetric to p and store it in p:
    void		    symmetry(UT_Vector3T<T> &p) const;

    // Find out if the plane contains the point p and return true or false.
    bool		    contains(const UT_Vector3T<T> &p) const;

    // Finds if the given point is on one side or the other.  The side
    // in direction of the normal is 1, the other is 0.
    int			    side(const UT_Vector3T<T> &p) const;

    // Query the point and the normal;
    const UT_Vector3T<T> &  point() const
				{ return myPoint; }
    void		    setPoint(const UT_Vector3T<T> &p)
				{ myPoint = p; }
    const UT_Vector3T<T> &  normal() const
				{ return myNormal; }
    void		    setNormal(const UT_Vector3T<T> &n)
			    {
				myNormal = n;
				UT_ASSERT(SYSisEqual(myNormal.length2(), 1));
			    }
    void		    setVectorAsNormal(const UT_Vector3T<T> &n)
			    {
				myNormal = n;
				myNormal.normalize();
			    }
    void		    setNormal(UT_PlaneType plane_type)
			    {
				switch (plane_type)
				{
				    case UT_PLANE_XY:
					myNormal.assign(0.0, 0.0, 1.0);
					break;
				    case UT_PLANE_YZ:
					myNormal.assign(1.0, 0.0, 0.0);
					break;
				    default:
					UT_ASSERT(!"Invalid plane type");
                                        SYS_FALLTHROUGH;
				    case UT_PLANE_XZ:
					myNormal.assign(0.0, 1.0, 0.0);
					break;
				}
			    }

    // Flip by 180 degrees:
    void		    negate();

    // Rotate by a given angle, expressed in radians:
    void		    rotate(
				UT_Vector3T<T> &axis,
				T theta,
				bool norm = true);
    void		    rotate(UT_Axis3::axis a, T theta);

    // Only rotate the point or normal
    void		    rotatePoint(const UT_Matrix3T<T> &rot_matx)
			    {
				myPoint *= rot_matx;
			    }
    void		    rotateNormal(const UT_Matrix3T<T> &rot_matx,
					 bool norm = true)
			    {
				myNormal *= rot_matx;
				if (norm)
				    myNormal.normalize();
			    }

    // Apply an invertible transformation to the plane: the method that takes
    // a non-const matrix returns the inverted and transposed matrix.
    void		    transform(const UT_Matrix4F &matx);
    void		    transform(      UT_Matrix4F &matx);
    void		    transform(const UT_Matrix4D &matx);
    void		    transform(      UT_Matrix4D &matx);

    // Translate the plane along the normal by the given factor.
    void		    shiftOffset(T by_factor)
			    {
				myPoint += (myNormal * by_factor);
			    }

    void		    translate(const UT_Vector3T<T> &offset)
			    {
				myPoint += offset;
			    }

private:
    static const UT_Vector3T<T> theInvalidHit;

    // The defining point and normal:
    UT_Vector3T<T>	    myPoint;
    UT_Vector3T<T>	    myNormal;
};

typedef UT_PlaneT<fpreal>   UT_PlaneR;
typedef UT_PlaneT<fpreal32> UT_PlaneF;
typedef UT_PlaneT<fpreal64> UT_PlaneD;
typedef UT_PlaneT<float>    UT_Plane;	// deprecated

#include "UT_PlaneImpl.h"

#endif // __UT_Plane_h__
