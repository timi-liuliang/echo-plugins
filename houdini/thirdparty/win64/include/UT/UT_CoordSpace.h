/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_CoordSpace.h (UT Library, C++)
 *
 * COMMENTS:
 *	A 3D system of coordinates.
 *
 */

#ifndef __UT_CoordSpace_h__
#define __UT_CoordSpace_h__

#include "UT_API.h"
#include "UT_Axis.h"
#include "UT_Plane.h"
#include "UT_VectorTypes.h"
#include "UT_XformOrder.h"

template <typename T>
class UT_CoordSpaceT
{
public:
    // Several constructors that define a plane as (X - point)*zaxis = 0
			    UT_CoordSpaceT(UT_PlaneType p = UT_PLANE_XY);
			    UT_CoordSpaceT(
				    const UT_Vector3T<T> &point,
				    const UT_Vector3T<T> &axaxis,
				    const UT_Vector3T<T> &ayaxis,
				    const UT_Vector3T<T> *azaxis,
				    bool norm = true);
			    UT_CoordSpaceT(
				    const UT_Vector3T<T> &p0,
				    const UT_Vector3T<T> &p1, 
				    const UT_Vector3T<T> &p2);

			    ~UT_CoordSpaceT();

    // Flip the z axis by 180 degrees:
    void		    negate();

    // Orient one axis and rotate the others accordingly, or set them all:
    void		    xaxis(const UT_Vector3T<T> &x, bool norm = true,
				  bool allow_flip = true);
    void		    yaxis(const UT_Vector3T<T> &y, bool norm = true,
				  bool allow_flip = true);
    void		    zaxis(const UT_Vector3T<T> &z, bool norm = true,
				  bool allow_flip = true);
    void		    setAxes(
				const UT_Vector3T<T> &x,
				const UT_Vector3T<T> &y,
				const UT_Vector3T<T> &z,
				bool norm = true);

    // Query the axes:
    const UT_Vector3T<T> &  xaxis() const	{ return myXAxis; }
    const UT_Vector3T<T> &  yaxis() const	{ return myYAxis; }
    const UT_Vector3T<T> &  zaxis() const	{ return myPlane.normal(); }

    // Set or query the origin:
    const UT_Vector3T<T> &  origin() const	{ return myPlane.point(); }
    void		    setOrigin(const UT_Vector3T<T> &o)
						{ myPlane.setPoint(o); }

    // Rotate by a given angle, expressed in radians:
    void		    rotate(
				UT_Vector3T<T> &axis,
				T theta,
				bool norm = true);
    void		    rotate(
				UT_Axis3::axis a,
				T theta);

    // Assume we're given a rotation matrix
    // Rotate about our own origin.
    void		    rotate(const UT_Matrix3T<T> &rotmatx);

    // Apply an invertible transformation to the plane: the method that takes
    // a non-const matrix returns the inverted and transposed matrix.
    void		    transform(const UT_Matrix4F &matx);
    void		    transform(      UT_Matrix4F &matx);
    void		    transform(const UT_Matrix4D &matx);
    void		    transform(      UT_Matrix4D &matx);

    // Set the transformation for this space to the identity matrix.
    void		    identity();

    void		    getTransformMatrix(UT_Matrix4T<T> &matx) const;

    // This version of getTransformMatrix is consistent with the transform
    // method: calling identity(), then transform(matrix), then
    // getTransformMatrixPreservingOrigin() will return the value of "matrix".
    // Doing the same but calling getTransformMatrix() will not return the
    // value of "matrix".
    void		    getTransformMatrixPreservingOrigin(
                                UT_Matrix4T<T> &matx) const;

    void		    shiftOffset(T by_factor)
			    {
				myPlane.shiftOffset(by_factor);
			    }

    void		    translate(const UT_Vector3T<T> &offset)
			    {
				myPlane.translate(offset);
			    }

    // Compute rotation from world space to us or vice-versa.
    // Return the same value "crack()" does.
    int			    fromWorldRotation(
				T &rx, T &ry, T &rz,
				const UT_XformOrder &order) const;
    int			    toWorldRotation(
				T &rx, T &ry, T &rz,
				const UT_XformOrder &order) const;

    void		    convertToWorld(
				UT_Vector3T<T> &rel) const;
    void		    convertToWorldNoOriginAdj(
				UT_Vector3T<T> &rel) const;

    // Allow const operations on the underlying plane.
    const UT_PlaneT<T> &    plane() const   { return myPlane; }

private:
    template <typename MATX>
    inline void		    changeSpace(MATX &matx) const;

    UT_PlaneT<T>	    myPlane;	// Use the plane's normal as the zaxis
    UT_Vector3T<T>	    myXAxis;	// Define the two other axes
    UT_Vector3T<T>	    myYAxis;
};

typedef UT_CoordSpaceT<fpreal>	    UT_CoordSpaceR;
typedef UT_CoordSpaceT<fpreal32>    UT_CoordSpaceF;
typedef UT_CoordSpaceT<fpreal64>    UT_CoordSpaceD;
typedef UT_CoordSpaceT<float>	    UT_CoordSpace;  // deprecated

#include "UT_CoordSpaceImpl.h"

#endif // __UT_CoordSpace_h__
