/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PlaneImpl.h (UT Library, C++)
 *
 * COMMENTS:
 *	A 3D plane.
 *
 */

#ifndef __UT_PLANEIMPL_H_INCLUDED__
#define __UT_PLANEIMPL_H_INCLUDED__

#include "UT_Assert.h"
#include "UT_Vector3.h"
#include "UT_Vector4.h"
#include "UT_Matrix3.h"
#include "UT_Matrix4.h"
#include <SYS/SYS_Math.h>
#include <limits>


template <typename T>
const UT_Vector3T<T> UT_PlaneT<T>::theInvalidHit(
					std::numeric_limits<T>::max(),
					std::numeric_limits<T>::max(),
					std::numeric_limits<T>::max());

template <typename T>
UT_PlaneT<T>::UT_PlaneT(UT_PlaneType plane_type)
    : myPoint(0, 0, 0)
{
    setNormal(plane_type);
}

template <typename T>
UT_PlaneT<T>::UT_PlaneT(const UT_Vector3T<T> &pt, UT_PlaneType plane_type)
    : myPoint(pt)
{
    setNormal(plane_type);
}

template <typename T>
UT_PlaneT<T>::UT_PlaneT(
	const UT_Vector3T<T> &apoint,
	const UT_Vector3T<T> &norm_dir,
	bool norm
	)
    : myPoint(apoint)
    , myNormal(norm_dir)
{
    if (norm) myNormal.normalize();
}

template <typename T>
UT_PlaneT<T>::UT_PlaneT(const UT_Vector4T<T> &p)
    : myPoint(0, 0, 0)
    , myNormal(p.x(), p.y(), p.z())
{
    if (p.z())
	myPoint.z() = -p.w()/p.z();
    else if (p.y())
	myPoint.y() = -p.w()/p.y();
    else if (p.x())
	myPoint.x() = -p.w()/p.x();
}

template <typename T>
UT_PlaneT<T>::UT_PlaneT(
	const UT_Vector3T<T> &p0,
	const UT_Vector3T<T> &p1, 
	const UT_Vector3T<T> &p2
	)
    : myPoint(p0)
{
    UT_Vector3T<T> v = p1-p0;
    myNormal = p2-p0;
    myNormal.cross(v);
    myNormal.normalize();
}

template <typename T>
UT_PlaneT<T>::~UT_PlaneT()
{
    // Nothing else needed.
}

template <typename T>
UT_Vector3T<T>
UT_PlaneT<T>::intersect(const UT_Vector3T<T> &p, const UT_Vector3T<T> &v) const
{
    T tnom = dot( (myPoint-p), myNormal );
    T tden = dot( v,           myNormal );
    if (SYSequalZero(tden))
    {
	UT_ASSERT( !"Line segment does not intersect with plane" );
	return theInvalidHit;
    }
    else
    {
        return (p + tnom/tden * v);
    }
}

template <typename T>
int
UT_PlaneT<T>::intersectLine(
	const UT_Vector3T<T> &p,
	const UT_Vector3T<T> &v,
	UT_Vector3T<T> &hit
	) const
{
    T tnom = dot( (myPoint-p), myNormal );
    T tden = dot( v,           myNormal );

    // If the numerator and the denominator don't have the same sign then
    // the ray doesn't intersect with the plane.
    if (SYSequalZero(tden))
    {
	hit = theInvalidHit;
	return -1;
    }
    else
    {
        hit = (p + tnom/tden * v);
	return 0;
    }
}

template <typename T>
int
UT_PlaneT<T>::intersectRay(
	const UT_Vector3T<T> &p,
	const UT_Vector3T<T> &v,
	UT_Vector3T<T> &hit
	) const
{
    T tnom = dot( (myPoint-p), myNormal );
    T tden = dot( v,           myNormal );

    // If the numerator and the denominator don't have the same sign then
    // the ray doesn't intersect with the plane.
    if (SYSequalZero(tden) || (tnom/tden) < 0.)
    {
	hit = theInvalidHit;
	return -1;
    }
    else
    {
        hit = (p + tnom/tden * v);
	return 0;
    }
}

template <typename T>
int
UT_PlaneT<T>::intersectLine(
	const UT_Vector3T<T> &offset_pt,
	const UT_Vector3T<T> &p, 
	const UT_Vector3T<T> &v,
	UT_Vector3T<T> &hit
	) const
{
    T tnom = dot( (offset_pt-p), myNormal );
    T tden = dot( v,             myNormal );

    if (SYSequalZero(tden))
    {
	hit = theInvalidHit;
	return -1;
    }
    else
    {
        hit = (p + tnom/tden * v);
	return 0;
    }
}

template <typename T>
int
UT_PlaneT<T>::intersectRay(
	const UT_Vector3T<T> &offset_pt,
	const UT_Vector3T<T> &p, 
	const UT_Vector3T<T> &v,
	UT_Vector3T<T> &hit
	) const
{
    T tnom = dot( (offset_pt-p), myNormal );
    T tden = dot( v,             myNormal );

    // If the numerator and the denominator don't have the same sign then
    // the ray doesn't intersect with the plane.
    if (SYSequalZero(tden) || (tnom/tden) < 0.)
    {
	hit = theInvalidHit;
	return -1;
    }
    else
    {
        hit = (p + tnom/tden * v);
	return 0;
    }
}

template <typename T>
void
UT_PlaneT<T>::negate()
{
    myNormal.negate();
}

template <typename T>
void
UT_PlaneT<T>::symmetry(UT_Vector3T<T> &p) const
{
    T tnom = dot( (myPoint-p), myNormal );
    p += 2.0 * tnom/myNormal.length2() * myNormal;
}

template <typename T>
UT_Vector3T<T>
UT_PlaneT<T>::project(const UT_Vector3T<T> &p) const
{
    T tnom = dot( (myPoint-p), myNormal );
    return (p + tnom/myNormal.length2() * myNormal);
}

template <typename T>
T
UT_PlaneT<T>::project(const UT_Vector3T<T> &p, UT_Vector3T<T> &projection) const
{
    T tnom = dot( (myPoint-p), myNormal );
    projection = p + tnom/myNormal.length2() * myNormal;
    return tnom;
}

template <typename T>
void
UT_PlaneT<T>::rotate(UT_Vector3T<T>  &axis, T theta, bool norm)
{
    UT_Matrix3 rMat;
    rMat = UT_Matrix3::rotationMat(axis, theta, norm);
    myPoint  *= rMat;
    rMat.invert(); rMat.transpose();
    myNormal *= rMat;
    myNormal.normalize();
}

template <typename T>
void
UT_PlaneT<T>::rotate(UT_Axis3::axis a, T theta)
{
    UT_Matrix3 rMat; 
    rMat = UT_Matrix3::rotationMat(a, theta);
    myPoint  *= rMat;
    rMat.invert(); rMat.transpose();
    myNormal *= rMat;
    myNormal.normalize();
}

template <typename T>
void
UT_PlaneT<T>::transform(const UT_Matrix4F &matx)
{
    myPoint  *= matx;
    UT_Matrix4F	nml_matx(matx);
    nml_matx.invert(); nml_matx.transpose();
    myNormal *= nml_matx;
    myNormal.normalize();
}

template <typename T>
void
UT_PlaneT<T>::transform(UT_Matrix4F &matx)
{
    myPoint  *= matx;
    matx.invert(); matx.transpose();
    myNormal *= matx;
    myNormal.normalize();
}

template <typename T>
void
UT_PlaneT<T>::transform(const UT_Matrix4D &matx)
{
    myPoint  *= matx;
    UT_Matrix4D	nml_matx(matx);
    nml_matx.invert(); nml_matx.transpose();
    myNormal *= nml_matx;
    myNormal.normalize();
}

template <typename T>
void
UT_PlaneT<T>::transform(UT_Matrix4D &matx)
{
    myPoint  *= matx;
    matx.invert(); matx.transpose();
    myNormal *= matx;
    myNormal.normalize();
}

template <typename T>
bool
UT_PlaneT<T>::contains(const UT_Vector3T<T> &p) const
{
    return SYSequalZero(dot(p-myPoint, myNormal)) ? 1 : 0;
}

template <typename T>
int
UT_PlaneT<T>::side(const UT_Vector3T<T> &p) const
{
    return (dot(p-myPoint, myNormal) > 0) ? 1 : 0;
}

#endif // __UT_PLANEIMPL_H_INCLUDED__
