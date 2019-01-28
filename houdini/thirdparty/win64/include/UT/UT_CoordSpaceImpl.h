/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_CoordSpaceImpl.h (UT Library, C++)
 *
 * COMMENTS:
 *	A 3D system of coordinates.
 *
 */

#ifndef __UT_COORDSPACEIMPL_H_INCLUDED__
#define __UT_COORDSPACEIMPL_H_INCLUDED__

#include "UT_Assert.h"
#include "UT_Vector3.h"
#include "UT_Matrix3.h"
#include "UT_Matrix4.h"
#include <SYS/SYS_Math.h>


template <typename T>
UT_CoordSpaceT<T>::UT_CoordSpaceT(UT_PlaneType ptype)
    : myPlane(ptype)
{
    switch (ptype)
    {
	case UT_PLANE_XY:
	    myXAxis.assign(1.0, 0.0, 0.0); // world X
	    myYAxis.assign(0.0, 1.0, 0.0); // world Y
	    break;
	case UT_PLANE_YZ:
	    myXAxis.assign(0.0, 1.0, 0.0); // world Y
	    myYAxis.assign(0.0, 0.0, 1.0); // world Z
	    break;
	default:
	    UT_ASSERT(!"Invalid plane type");
            SYS_FALLTHROUGH;
	case UT_PLANE_XZ:
	    myXAxis.assign(0.0, 0.0, 1.0); // world Z
	    myYAxis.assign(1.0, 0.0, 0.0); // world X
	    break;
    }
}

template <typename T>
UT_CoordSpaceT<T>::UT_CoordSpaceT(
	const UT_Vector3T<T> &pt,
	const UT_Vector3T<T> &axaxis, 
	const UT_Vector3T<T> &ayaxis,
	const UT_Vector3T<T> *azaxis,
	bool norm
    )
    : myPlane(pt)
{
    myXAxis = axaxis;
    myYAxis = ayaxis;

    if (norm)
    {
	myXAxis.normalize();
	myYAxis.normalize();
    }

    UT_Vector3T<T> n;
    if (azaxis)
    {
	n = *azaxis;
	if (norm)
	    n.normalize();
    }
    else
    {
	n = cross(axaxis, ayaxis);
	n.normalize();
    }
    myPlane.setNormal(n);
}

template <typename T>
UT_CoordSpaceT<T>::UT_CoordSpaceT(
	const UT_Vector3T<T> &p0,
	const UT_Vector3T<T> &p1, 
	const UT_Vector3T<T> &p2
	)
    : myPlane(p0, p1, p2)
{
    myXAxis = p1 - p0;
    myXAxis.normalize();
     
    myYAxis = cross(myPlane.normal(), myXAxis);
    myYAxis.normalize();
}

template <typename T>
UT_CoordSpaceT<T>::~UT_CoordSpaceT()
{
    // nothing else needed
}

template <typename T>
void
UT_CoordSpaceT<T>::negate()
{
    UT_Vector3T<T> oldy = myYAxis;
    myYAxis = myXAxis;
    myXAxis = oldy;

    myPlane.negate();
}

template <typename T>
void	
UT_CoordSpaceT<T>::xaxis(const UT_Vector3T<T> &x, bool norm, bool allow_flip)
{
    UT_Vector3T<T> oldx = myXAxis;
    UT_Matrix3T<T> matx;

    myXAxis = x; if (norm) myXAxis.normalize();

    if (matx.dihedral(oldx, myXAxis, false) != 0)
    {
	// New x-axis is opposed to the old.  Use a rotation of 180 degrees
	// about our y-axis, if allowed.
	if (allow_flip)
	{
	    matx.identity();
	    matx.rotate(myYAxis, M_PI, /* norm */ 0);
	}
	else
	{
	    myXAxis = oldx;
	    return;
	}
    }

    myYAxis *= matx; myYAxis.normalize();
    myPlane.rotateNormal(matx, true);
}

template <typename T>
void	
UT_CoordSpaceT<T>::yaxis(const UT_Vector3T<T> &y, bool norm, bool allow_flip)
{
    UT_Vector3T<T> oldy = myYAxis;
    UT_Matrix3T<T> matx;

    myYAxis = y; if (norm) myYAxis.normalize();

    if (matx.dihedral(oldy, myYAxis, false) != 0)
    {
	// New y-axis is opposed to the old.  Use a rotation of 180 degrees
	// about our z-axis, if allowed.
	if (allow_flip)
	{
	    matx.identity();
            UT_Vector3T<T> nml = myPlane.normal();
	    matx.rotate(nml, M_PI, /* norm */ 0);
	}
	else
	{
	    myYAxis = oldy;
	    return;
	}
    }

    myXAxis *= matx; myXAxis.normalize();
    myPlane.rotateNormal(matx, true);
}

template <typename T>
void	
UT_CoordSpaceT<T>::zaxis(const UT_Vector3T<T> &z, bool norm, bool allow_flip)
{
    UT_Vector3T<T> oldz = myPlane.normal();
    UT_Matrix3T<T> matx;

    if (norm)
	myPlane.setVectorAsNormal(z);
    else
	myPlane.setNormal(z);

    UT_Vector3T<T> n = myPlane.normal();
    if (matx.dihedral(oldz, n, false) != 0)
    {
	// New z-axis is opposed to the old.  Use a rotation of 180 degrees
	// about our y-axis, if allowed.
	if (allow_flip)
	{
	    matx.identity();
	    matx.rotate(myYAxis, M_PI, /* norm */ 0);
	}
	else
	{
	    myPlane.setNormal(oldz);
	    return;
	}
    }

    myXAxis *= matx; myXAxis.normalize();
    myYAxis *= matx; myYAxis.normalize();
}

template <typename T>
void	
UT_CoordSpaceT<T>::setAxes(
	const UT_Vector3T<T> &x,
	const UT_Vector3T<T> &y,
	const UT_Vector3T<T> &z,
	bool norm)
{
    myXAxis = x;
    myYAxis = y;

    if (norm)
    {
	myXAxis.normalize();
	myYAxis.normalize();
	myPlane.setVectorAsNormal(z);
    }
    else
    {
	myPlane.setNormal(z);
    }
}

template <typename T>
void
UT_CoordSpaceT<T>::rotate(UT_Vector3T<T> &axis, T theta, bool norm)
{
    UT_Matrix3T<T> rmatx;
    rmatx = UT_Matrix3T<T>::rotationMat(axis, theta, norm);
    myPlane.rotatePoint(rmatx);

    rmatx.invert();
    rmatx.transpose();

    myXAxis *= rmatx; myXAxis.normalize();
    myYAxis *= rmatx; myYAxis.normalize();
    myPlane.rotateNormal(rmatx, true);
}

template <typename T>
void
UT_CoordSpaceT<T>::rotate(UT_Axis3::axis a, T theta)
{
    UT_Matrix3T<T> rmatx;
    rmatx = UT_Matrix3T<T>::rotationMat(a, theta);
    myPlane.rotatePoint(rmatx);

    rmatx.invert();
    rmatx.transpose();

    myXAxis *= rmatx; myXAxis.normalize();
    myYAxis *= rmatx; myYAxis.normalize();
    myPlane.rotateNormal(rmatx, true);
}

template <typename T>
void
UT_CoordSpaceT<T>::rotate(const UT_Matrix3T<T> &rotmatx)
{
    UT_Matrix4T<T>	matx;
    UT_Matrix4T<T>	tmp;

    tmp = rotmatx;

    matx.identity();
    matx.translate(-origin().x(), -origin().y(), -origin().z()); 
    matx *= tmp;
    matx.translate( origin().x(),  origin().y(),  origin().z()); 
    transform((UT_Matrix4T<T>&) matx);
}

template <typename T>
void
UT_CoordSpaceT<T>::transform(const UT_Matrix4F &matx)
{
    myPlane.transform(matx);

    UT_Matrix4F xform(matx);

    xform.invert();
    xform.transpose();

    myXAxis *= xform; myXAxis.normalize();
    myYAxis *= xform; myYAxis.normalize();
}

template <typename T>
void
UT_CoordSpaceT<T>::transform(UT_Matrix4F &matx)
{
    myPlane.transform(matx); // returned already inverted and transposed

    myXAxis *= matx; myXAxis.normalize();
    myYAxis *= matx; myYAxis.normalize();
}

template <typename T>
void
UT_CoordSpaceT<T>::transform(const UT_Matrix4D &matx)
{
    myPlane.transform(matx);

    UT_Matrix4D	xform(matx);

    xform.invert();
    xform.transpose();

    myXAxis *= xform; myXAxis.normalize();
    myYAxis *= xform; myYAxis.normalize();
}

template <typename T>
void
UT_CoordSpaceT<T>::transform(UT_Matrix4D &matx)
{
    myPlane.transform(matx); // returned already inverted and transposed

    myXAxis *= matx; myXAxis.normalize();
    myYAxis *= matx; myYAxis.normalize();
}

template <typename T>
void
UT_CoordSpaceT<T>::identity()
{
    // Set our transform to the identity matrix.
    myPlane.setPoint(UT_Vector3T<T>(0.0, 0.0, 0.0));
    myPlane.setNormal(UT_PLANE_XY);
    myXAxis.assign(1.0, 0.0, 0.0);
    myYAxis.assign(0.0, 1.0, 0.0);
}

// Multiples the input matrix by the change in coordinate space from world
// space into this one.
template <typename T>
template <typename MATX>
inline void
UT_CoordSpaceT<T>::changeSpace(MATX &matx) const
{
    UT_Vector3T<T> world_x(1.0, 0.0, 0.0);
    UT_Vector3T<T> world_y(0.0, 1.0, 0.0);

    matx.changeSpace(world_x, world_y,
		     const_cast<UT_CoordSpaceT<T> *>(this)->myXAxis,
		     const_cast<UT_CoordSpaceT<T> *>(this)->myYAxis,
		     /*norm*/ false);
}

template <typename T>
void
UT_CoordSpaceT<T>::getTransformMatrix(UT_Matrix4T<T> &matx) const
{
    matx.identity();
    matx.translate(origin().x(), origin().y(), origin().z());
    changeSpace(matx);
}

template <typename T>
void
UT_CoordSpaceT<T>::getTransformMatrixPreservingOrigin(
    UT_Matrix4T<T> &matx) const
{
    matx.identity();
    changeSpace(matx);
    matx.translate(origin().x(), origin().y(), origin().z());
}

template <typename T>
int
UT_CoordSpaceT<T>::fromWorldRotation(T &rx, T &ry, T &rz,
				 const UT_XformOrder &order) const
{
    UT_Vector3T<T>		rvals;
    UT_Matrix3T<T>		rmatx;

    rmatx.identity();
    changeSpace(rmatx);
    int ret = rmatx.crack(rvals, order);

    rx = rvals[0]; ry = rvals[1]; rz = rvals[2];

    return ret;
}

template <typename T>
int
UT_CoordSpaceT<T>::toWorldRotation(T &rx, T &ry, T &rz,
			       const UT_XformOrder &order) const
{
    UT_Vector3T<T>		rvals;
    UT_Matrix3T<T>		rmatx;

    rmatx.identity();
    changeSpace(rmatx);
    rmatx.invert();
    int ret = rmatx.crack(rvals, order);

    rx = rvals[0]; ry = rvals[1]; rz = rvals[2];

    return ret;
}

template <typename T>
void
UT_CoordSpaceT<T>::convertToWorld(UT_Vector3T<T> &rel) const
{
    UT_Vector3T<T> r;
    r.x() = rel.x()*xaxis().x()+rel.y()*yaxis().x()+rel.z()*zaxis().x();
    r.y() = rel.x()*xaxis().y()+rel.y()*yaxis().y()+rel.z()*zaxis().y();
    r.z() = rel.x()*xaxis().z()+rel.y()*yaxis().z()+rel.z()*zaxis().z();
    r    += origin();
    rel   = r;
}

template <typename T>
void
UT_CoordSpaceT<T>::convertToWorldNoOriginAdj(UT_Vector3T<T> &rel) const
{
    UT_Vector3T<T> r;
    r.x() = rel.x()*xaxis().x()+rel.y()*yaxis().x()+rel.z()*zaxis().x();
    r.y() = rel.x()*xaxis().y()+rel.y()*yaxis().y()+rel.z()*zaxis().y();
    r.z() = rel.x()*xaxis().z()+rel.y()*yaxis().z()+rel.z()*zaxis().z();
    rel   = r;
}

#endif // __UT_COORDSPACEIMPL_H_INCLUDED__
