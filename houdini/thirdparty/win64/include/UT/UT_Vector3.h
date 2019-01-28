/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This class handles fpreal vectors of dimension 3.
 *
 * WARNING:
 *	This class should NOT contain any virtual methods, nor should it
 *	define more member data. The size of UT_Vector3 must always be
 *	12 bytes (3 floats).
 */

#pragma once

#ifndef __UT_Vector3_h__
#define __UT_Vector3_h__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_FixedVector.h"
#include "UT_VectorTypes.h"
#include "UT_Vector2.h"
#include <SYS/SYS_Deprecated.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <iosfwd>
#include <limits>

class UT_IStream;
class UT_JSONWriter;
class UT_JSONValue;
class UT_JSONParser;

// Free floating functions:

// Right-multiply operators (M*v) have been removed. They had previously
// been defined to return v*M, which was too counterintuitive. Once HDK
// etc. users have a chance to update their code (post 7.0) we could
// reintroduce a right-multiply operator that does a colVecMult.

template <typename T, typename S>
inline UT_Vector3T<T> operator*(const UT_Vector3T<T> &v, const UT_Matrix3T<S>  &m);
template <typename T, typename S>
inline UT_Vector3T<T> operator*(const UT_Vector3T<T> &v, const UT_Matrix4T<S>  &m);
template <typename T>
inline UT_Vector3T<T> operator+(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2);
template <typename T>
inline UT_Vector3T<T> operator-(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2);
template <typename T, typename S>
inline UT_Vector3T<T> operator+(const UT_Vector3T<T> &v, S scalar);
template <typename T, typename S>
inline UT_Vector3T<T> operator-(const UT_Vector3T<T> &v, S scalar);
template <typename T, typename S>
inline UT_Vector3T<T> operator*(const UT_Vector3T<T> &v, S scalar);
template <typename T, typename S>
inline UT_Vector3T<T> operator/(const UT_Vector3T<T> &v, S scalar);
template <typename T, typename S>
inline UT_Vector3T<T> operator+(S scalar, const UT_Vector3T<T> &v);
template <typename T, typename S>
inline UT_Vector3T<T> operator-(S scalar, const UT_Vector3T<T> &v);
template <typename T, typename S>
inline UT_Vector3T<T> operator*(S scalar, const UT_Vector3T<T> &v);
template <typename T, typename S>
inline UT_Vector3T<T>	operator/(S scalar, const UT_Vector3T<T> &v);

/// The dot and cross products between two vectors (see operator*() too)
// @{
template <typename T>
inline T dot(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2);
template <typename T>
inline UT_Vector3T<T> cross(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2);
// @}

/// The angle between two vectors in radians
// @{
template <typename T>
inline fpreal64 UTangleBetween(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2);
// @}

/// Componentwise min and maximum
template <typename T>
inline UT_Vector3T<T> SYSmin(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2);
template <typename T>
inline UT_Vector3T<T> SYSmax(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2);

/// Componentwise linear interpolation
template <typename T,typename S>
inline UT_Vector3T<T> SYSlerp(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2, S t);
template <typename T,typename S>
inline UT_Vector3T<T> SYSbilerp(const UT_Vector3T<T> &u0v0, const UT_Vector3T<T> &u1v0,
                                  const UT_Vector3T<T> &u0v1, const UT_Vector3T<T> &u1v1,
                                  S u, S v)
{ return SYSlerp(SYSlerp(u0v0, u0v1, v), SYSlerp(u1v0, u1v1, v), u); }

/// Trilinear hat function over kernel widths in s.
template <typename T>
inline T SYStrihat(const UT_Vector3T<T> &v, const UT_Vector3T<T> &s);

/// Gradient of trilinear hat function over kernel widths in s.
template <typename T>
inline UT_Vector3T<T> SYStrihatgrad(const UT_Vector3T<T> &v, const UT_Vector3T<T> &s);

/// The orthogonal projection of a vector u onto a vector v
template <typename T>
inline UT_Vector3T<T> project(const UT_Vector3T<T> &u, const UT_Vector3T<T> &v);

// TODO: make UT_Vector4 versions of these:

/// Compute the distance between two points
template <typename T>
inline T distance3d(const UT_Vector3T<T> &p1, const UT_Vector3T<T> &p2);
/// Compute the distance squared
template <typename T>
inline T distance2(const UT_Vector3T<T> &p1, const UT_Vector3T<T> &p2);
template <typename T>
inline T segmentPointDist2(const UT_Vector3T<T> &pos,
				     const UT_Vector3T<T> &pt1,
				     const UT_Vector3T<T> &pt2 );

/// Intersect the lines p1 + v1 * t1 and p2 + v2 * t2.
/// t1 and t2 are set so that the lines intersect when
/// projected to the plane defined by the two lines.
/// This function returns a value which indicates how close
/// to being parallel the lines are. Closer to zero means
/// more parallel. This is done so that the user of this
/// function can decide what epsilon they want to use.
template <typename T>
UT_API double		intersectLines(const UT_Vector3T<T> &p1,
				       const UT_Vector3T<T> &v1,
				       const UT_Vector3T<T> &p2,
				       const UT_Vector3T<T> &v2,
				       T &t1, T &t2);

/// Returns true if the segments from p0 to p1 and from a to b intersect, and
/// t will contain the parametric value of the intersection on the segment a-b.
/// Otherwise returns false.  Parallel segments will return false.  T is
/// guaranteed to be between 0.0 and 1.0 if this function returns true.
template <typename T>
UT_API bool		intersectSegments(const UT_Vector3T<T> &p0,
	                                  const UT_Vector3T<T> &p1,
					  const UT_Vector3T<T> &a,
					  const UT_Vector3T<T> &b, T &t);

/// Returns the U coordinates of the closest points on each of the two 
/// parallel line segments
template <typename T>
UT_API UT_Vector2T<T>	segmentClosestParallel(const UT_Vector3T<T> &p0,
					       const UT_Vector3T<T> &p1,
					       const UT_Vector3T<T> &a,
					       const UT_Vector3T<T> &b);
/// Returns the U coordinates of the closest points on each of the two 
/// line segments
template <typename T>
UT_API UT_Vector2T<T>	segmentClosest(const UT_Vector3T<T> &p0,
	                               const UT_Vector3T<T> &p1,
				       const UT_Vector3T<T> &a,
				       const UT_Vector3T<T> &b);
/// Returns the U coordinate of the point on line segment p0->p1
/// that is closest to a.
template <typename T>
UT_API T segmentClosest(
    const UT_Vector3T<T> &p0,
    const UT_Vector3T<T> &p1,
    const UT_Vector3T<T> &a);

/// Returns the squared distance between two line segments: p0-p1 and a-b
template <typename T>
inline T		segmentDistance2(const UT_Vector3T<T> &p0,
					 const UT_Vector3T<T> &p1,
					 const UT_Vector3T<T> &a,
					 const UT_Vector3T<T> &b);
/// Returns the distance between two line segments: p0-p1 and a-b
template <typename T>
inline T		segmentDistance(const UT_Vector3T<T> &p0,
					const UT_Vector3T<T> &p1,
					const UT_Vector3T<T> &a,
					const UT_Vector3T<T> &b);
/// 3D Vector class.
template <typename T>
class UT_API UT_Vector3T : public UT_FixedVector<T,3,true>
{
public:
    typedef UT_FixedVector<T,3,true> Base;

    // These "using" statements are needed for operator=, operator*=, and
    // distance, so that the ones in UT_FixedVector aren't hidden by the
    // additional ones here.
    using Base::operator=;
    using Base::operator*=;
    using Base::distance;

    // These "using" statements are needed for GCC and Clang, because
    // otherwise, they ignore all members of UT_FixedVector when
    // checking validity of code in functions in this class.
    using Base::vec;
    using Base::data;
    using Base::normalize;
    using Base::length2;
    typedef T value_type;
    static const int tuple_size = 3;

    /// Default constructor.
    /// No data is initialized! Use it for extra speed.
    SYS_FORCE_INLINE UT_Vector3T() = default;

    SYS_FORCE_INLINE UT_Vector3T(const UT_Vector3T<T> &that) = default;
    SYS_FORCE_INLINE UT_Vector3T(UT_Vector3T<T> &&that) = default;

    explicit SYS_FORCE_INLINE UT_Vector3T(T v)
    {
        vec[0] = v;
        vec[1] = v;
        vec[2] = v;
    }

    SYS_FORCE_INLINE UT_Vector3T(T vx, T vy, T vz)
    {
        vec[0] = vx;
        vec[1] = vy;
        vec[2] = vz;
    }
    SYS_FORCE_INLINE UT_Vector3T(const fpreal16 v[tuple_size])
        : Base(v)
    {}
    SYS_FORCE_INLINE UT_Vector3T(const fpreal32 v[tuple_size])
        : Base(v)
    {}
    SYS_FORCE_INLINE UT_Vector3T(const fpreal64 v[tuple_size])
        : Base(v)
    {}
    SYS_FORCE_INLINE UT_Vector3T(const int32 v[tuple_size])
        : Base(v)
    {}
    SYS_FORCE_INLINE UT_Vector3T(const int64 v[tuple_size])
        : Base(v)
    {}

    SYS_FORCE_INLINE explicit UT_Vector3T(const UT_Vector2T<T> &v);
    SYS_FORCE_INLINE explicit UT_Vector3T(const UT_Vector4T<T> &v);

    /// Our own type of any given value_type.
    template <typename S>
    SYS_FORCE_INLINE UT_Vector3T(const UT_Vector3T<S> &v)
        : Base(v)
    {}

    /// Arbitrary UT_FixedVector of the same size
    template <typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE UT_Vector3T(const UT_FixedVector<S,tuple_size,S_INSTANTIATED> &v)
        : Base(v)
    {}

    SYS_FORCE_INLINE UT_Vector3T<T> &operator=(const UT_Vector3T<T> &that) = default;
    SYS_FORCE_INLINE UT_Vector3T<T> &operator=(UT_Vector3T<T> &&that) = default;

    template <typename S>
    SYS_FORCE_INLINE UT_Vector3T<T> &operator=(const UT_Vector3T<S> &v)
    { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; return *this; }

    /// Assignment operator that truncates a V4 to a V3.
    /// TODO: remove this. This should require an explicit UT_Vector3()
    /// construction, since it's unsafe.
    SYS_DEPRECATED_HDK_REPLACE(16.0,explicit UT_Vector3 constructor to avoid implicit conversion from UT_Vector4)
    UT_Vector3T<T>	&operator=(const UT_Vector4T<T> &v);

    /// It's unclear why this is needed, given UT_FixedVector::operator-(),
    /// but the compiler seems not to accept not having it.
    SYS_FORCE_INLINE UT_Vector3T<T> operator-() const
    {
        return UT_Vector3T<T>(-vec[0], -vec[1], -vec[2]);
    }

    void	clampZero(T tol = T(0.00001f))
		{
    		    if (vec[0] >= -tol && vec[0] <= tol) vec[0] = 0;
    		    if (vec[1] >= -tol && vec[1] <= tol) vec[1] = 0;
    		    if (vec[2] >= -tol && vec[2] <= tol) vec[2] = 0;
		}

    SYS_FORCE_INLINE
    void	 multiplyComponents(const UT_Vector3T<T> &v)
		 {
		     vec[0] *= v.vec[0];
		     vec[1] *= v.vec[1];
		     vec[2] *= v.vec[2];
		 }

    /// If you need a multiplication operator that left multiplies the vector
    /// by a matrix (M * v), use the following colVecMult() functions. If
    /// you'd rather not use operator*=() for right-multiplications (v * M),
    /// use the following rowVecMult() functions. The methods that take a 4x4
    /// matrix first extend this vector to 4D by adding an element equal to 1.0.
    /// @internal These are implemented in UT_Matrix3.h and UT_Matrix4.h
    // @{
    SYS_FORCE_INLINE void rowVecMult(const UT_Matrix3F &m);
    SYS_FORCE_INLINE void rowVecMult(const UT_Matrix4F &m);
    SYS_FORCE_INLINE void rowVecMult(const UT_Matrix3D &m);
    SYS_FORCE_INLINE void rowVecMult(const UT_Matrix4D &m);
    SYS_FORCE_INLINE void colVecMult(const UT_Matrix3F &m);
    SYS_FORCE_INLINE void colVecMult(const UT_Matrix4F &m);
    SYS_FORCE_INLINE void colVecMult(const UT_Matrix3D &m);
    SYS_FORCE_INLINE void colVecMult(const UT_Matrix4D &m);
    // @}


    /// This multiply will not extend the vector by adding a fourth element.
    /// Instead, it converts the Matrix4 to a Matrix3.  This means that
    /// the translate component of the matrix is not applied to the vector
    /// @internal These are implemented in UT_Matrix4.h
    // @{
    SYS_FORCE_INLINE void rowVecMult3(const UT_Matrix4F &m);
    SYS_FORCE_INLINE void rowVecMult3(const UT_Matrix4D &m);
    SYS_FORCE_INLINE void colVecMult3(const UT_Matrix4F &m);
    SYS_FORCE_INLINE void colVecMult3(const UT_Matrix4D &m);
    // @}



    /// The *=, multiply, multiply3 and multiplyT routines are provided for
    /// legacy reasons. They all assume that *this is a row vector. Generally,
    /// the rowVecMult and colVecMult methods are preferred, since they're
    /// more explicit about the row vector assumption.
    // @{
    template <typename S>
    SYS_FORCE_INLINE UT_Vector3T<T> &operator*=(const UT_Matrix3T<S> &m);
    template <typename S>
    SYS_FORCE_INLINE UT_Vector3T<T> &operator*=(const UT_Matrix4T<S> &m);

    template <typename S>
    SYS_FORCE_INLINE void multiply3(const UT_Matrix4T<S> &mat);
    // @}

    /// This multiply will multiply the (row) vector by the transpose of the
    ///	matrix instead of the matrix itself.  This is faster than
    ///	transposing the matrix, then multiplying (as well there's potentially
    ///	less storage requirements).
    // @{
    template <typename S>
    SYS_FORCE_INLINE void multiplyT(const UT_Matrix3T<S> &mat);
    template <typename S>
    SYS_FORCE_INLINE void multiply3T(const UT_Matrix4T<S> &mat);
    // @}

    /// The following methods implement multiplies (row) vector by a matrix,
    /// however, the resulting vector is specified by the dest parameter
    /// These operations are safe even if "dest" is the same as "this".
    // @{
    template <typename S>
    SYS_FORCE_INLINE void multiply3(UT_Vector3T<T> &dest,
				    const UT_Matrix4T<S> &mat) const;
    template <typename S>
    SYS_FORCE_INLINE void multiplyT(UT_Vector3T<T> &dest,
				    const UT_Matrix3T<S> &mat) const;
    template <typename S>
    SYS_FORCE_INLINE void multiply3T(UT_Vector3T<T> &dest,
				     const UT_Matrix4T<S> &mat) const;
    template <typename S>
    SYS_FORCE_INLINE void multiply(UT_Vector3T<T> &dest,
				   const UT_Matrix4T<S> &mat) const;
    template <typename S>
    SYS_FORCE_INLINE void multiply(UT_Vector3T<T> &dest,
				   const UT_Matrix3T<S> &mat) const;
    // @}

    SYS_FORCE_INLINE void cross(const UT_Vector3T<T> &v)
		 {
		    operator=(::cross(*this, v));
		 }

    SYS_FORCE_INLINE void normal(const UT_Vector3T<T> &va, const UT_Vector3T<T> &vb)
		 {
		    vec[0] += (va.vec[2]+vb.vec[2])*(vb.vec[1]-va.vec[1]);
		    vec[1] += (va.vec[0]+vb.vec[0])*(vb.vec[2]-va.vec[2]);
		    vec[2] += (va.vec[1]+vb.vec[1])*(vb.vec[0]-va.vec[0]);
		 }

    /// Finds an arbitrary perpendicular to v, and sets this to it.
    void	 arbitraryPerp(const UT_Vector3T<T> &v);
    /// Makes this orthogonal to the given vector.  If they are colinear,
    /// does an arbitrary perp
    void	 makeOrthonormal(const UT_Vector3T<T> &v);

    /// These allow you to find out what indices to use for different axes
    // @{
    int		 findMinAbsAxis() const
		 {
		    T	ax = SYSabs(x()), ay = SYSabs(y());
		    if (ax < ay)
			return (SYSabs(z()) < ax) ? 2 : 0;
		    else
			return (SYSabs(z()) < ay) ? 2 : 1;
		 }
    int		 findMaxAbsAxis() const
		 {
		    T	ax = SYSabs(x()), ay = SYSabs(y());
		    if (ax >= ay)
			return (SYSabs(z()) >= ax) ? 2 : 0;
		    else
			return (SYSabs(z()) >= ay) ? 2 : 1;
		 }
    // @}

    /// Given this vector as the z-axis, get a frame of reference such that the
    /// X and Y vectors are orthonormal to the vector.  This vector should be
    /// normalized.
    void	 getFrameOfReference(UT_Vector3T<T> &X, UT_Vector3T<T> &Y) const
		 {
			 if (SYSabs(x()) < 0.6F) Y = UT_Vector3T<T>(1, 0, 0);
		    else if (SYSabs(z()) < 0.6F) Y = UT_Vector3T<T>(0, 1, 0);
		    else			Y = UT_Vector3T<T>(0, 0, 1);
		    X = ::cross(Y, *this);
		    X.normalize();
		    Y = ::cross(*this, X);
		 }

    /// Calculates the orthogonal projection of a vector u on the *this vector 
    UT_Vector3T<T>	 project(const UT_Vector3T<T> &u) const;

    /// Create a matrix of projection onto this vector: the matrix transforms
    /// a vector v into its projection on the direction of (*this) vector,
    /// ie. dot(*this, v) * this->normalize();
    /// If we need to be normalized, set norm to non-false.
    template <typename S>
    UT_Matrix3T<S> project(bool norm=true);

    /// Vector p (representing a point in 3-space) and vector v define
    /// a line. This member returns the projection of "this" onto the
    /// line (the point on the line that is closest to this point).
    UT_Vector3T<T>	 projection(const UT_Vector3T<T> &p,
			    const UT_Vector3T<T> &v) const;

    /// Projects this onto the line segement [a,b].  The returned point
    /// will lie between a and b.
    UT_Vector3T<T>	 projectOnSegment(const UT_Vector3T<T> &va,
				  const UT_Vector3T<T> &vb) const;
    /// Projects this onto the line segment [a, b].  The fpreal t is set
    /// to the parametric position of intersection, a being 0 and b being 1.
    UT_Vector3T<T>	 projectOnSegment(const UT_Vector3T<T> &va, const UT_Vector3T<T> &vb,
				  T &t) const;

    /// Create a matrix of symmetry around this vector: the matrix transforms 
    /// a vector v into its symmetry around (*this), ie. two times the 
    /// projection of v onto (*this) minus v.
    /// If we need to be normalized, set norm to non-false.
    UT_Matrix3	 symmetry(bool norm=true);

    /// This method stores in (*this) the intersection between two 3D lines,
    /// p1+t*v1 and p2+u*v2. If the two lines do not actually intersect, we
    /// shift the 2nd line along the perpendicular on both lines (along the
    /// line of min distance) and return the shifted intersection point; this
    /// point thus lies on the 1st line.
    /// If we find an intersection point (shifted or not) we return 0; if
    /// the two lines are parallel we return -1; and if they intersect 
    /// behind our back we return -2. When we return -2 there still is a
    /// valid intersection point in (*this).
    int		lineIntersect(const UT_Vector3T<T> &p1, const UT_Vector3T<T> &v1,
			      const UT_Vector3T<T> &p2, const UT_Vector3T<T> &v2);

    /// Compute the intersection of vector p2+t*v2 and the line segment between
    /// points pa and pb. If the two lines do not intersect we shift the 
    /// (p2, v2) line along the line of min distance and return the point 
    /// where it intersects the segment. If we find an intersection point 
    /// along the stretch between pa and pb, we return 0. If the lines are
    /// parallel we return -1. If they intersect before pa we return -2, and 
    /// if after pb, we return -3. The intersection point is valid with 
    /// return codes 0,-2,-3.
    int		segLineIntersect(const UT_Vector3T<T> &pa,  const UT_Vector3T<T> &pb,
				 const UT_Vector3T<T> &p2, const UT_Vector3T<T> &v2);

    /// Determines whether or not the points p0, p1 and "this" are collinear.
    /// If they are t contains the parametric value of where "this" is found
    /// on the segment from p0 to p1 and returns true.  Otherwise returns
    /// false.  If p0 and p1 are equal, t is set to
    /// std::numeric_limits<T>::max() and true is returned.
    bool	areCollinear(const UT_Vector3T<T> &p0, const UT_Vector3T<T> &p1,
			     T *t = 0, T tol = 1e-5) const;

    /// Compute (homogeneous) barycentric co-ordinates of this point
    /// relative to the triangle defined by t0, t1 and t2. (The point is
    /// projected into the triangle's plane.)
    UT_Vector3T<T>	getBary(const UT_Vector3T<T> &t0, const UT_Vector3T<T> &t1,
			const UT_Vector3T<T> &t2, bool *degen = NULL) const;


    /// Compute the signed distance from us to a line.
    T	distance(const UT_Vector3T<T> &p1, const UT_Vector3T<T> &v1) const;
    /// Compute the signed distance between two lines.
    T	distance(const UT_Vector3T<T> &p1, const UT_Vector3T<T> &v1,
			 const UT_Vector3T<T> &p2, const UT_Vector3T<T> &v2) const;

    /// Return the components of the vector. The () operator does NOT check
    /// for the boundary condition.
    /// @{
    SYS_FORCE_INLINE T &x(void)		{ return vec[0]; }
    SYS_FORCE_INLINE T  x(void) const	{ return vec[0]; }
    SYS_FORCE_INLINE T &y(void)		{ return vec[1]; }
    SYS_FORCE_INLINE T  y(void) const	{ return vec[1]; }
    SYS_FORCE_INLINE T &z(void)		{ return vec[2]; }
    SYS_FORCE_INLINE T  z(void) const	{ return vec[2]; }
    SYS_FORCE_INLINE T &r(void) 	{ return vec[0]; }
    SYS_FORCE_INLINE T  r(void) const	{ return vec[0]; }
    SYS_FORCE_INLINE T &g(void) 	{ return vec[1]; }
    SYS_FORCE_INLINE T  g(void) const	{ return vec[1]; }
    SYS_FORCE_INLINE T &b(void) 	{ return vec[2]; }
    SYS_FORCE_INLINE T  b(void) const	{ return vec[2]; }
    SYS_FORCE_INLINE T &operator()(unsigned i)
		 {
		     UT_ASSERT_P(i < tuple_size);
		     return vec[i];
		 }
    SYS_FORCE_INLINE T	 operator()(unsigned i) const
		 {
		     UT_ASSERT_P(i < tuple_size);
		     return vec[i];
		 }
    /// @}

    /// Compute a hash
    unsigned	hash() const	{ return SYSvector_hash(data(), tuple_size); }

    // TODO: eliminate these methods. They're redundant, given good inline
    // constructors.
    /// Set the values of the vector components
    void	 assign(T xx = 0.0f, T yy = 0.0f, T zz = 0.0f)
		 {
		     vec[0] = xx; vec[1] = yy; vec[2] = zz;
		 }
    /// Set the values of the vector components
    void	 assign(const T *v)
		 {
		     vec[0]=v[0]; vec[1]=v[1]; vec[2]=v[2];
		 }

    /// Express the point in homogeneous coordinates or vice-versa
    // @{
    void	 homogenize(void)
		 {
		     vec[0] *= vec[2];
		     vec[1] *= vec[2];
		 }
    void	 dehomogenize(void)
		 {
		     if (vec[2] != 0)
		     {
			 T denom = 1.0f / vec[2];
			 vec[0] *= denom;
			 vec[1] *= denom;
		     }
		 }
    // @}

    /// assuming that "this" is a rotation (in radians, of course), the
    /// equivalent set of rotations which are closest to the "base" rotation
    /// are found. The equivalent rotations are the same as the original
    /// rotations +2*n*PI
    void	 roundAngles(const UT_Vector3T<T> &base);

    /// conversion between degrees and radians
    // @{
    void	 degToRad();
    void	 radToDeg();
    // @}

    /// It seems that given any rotation matrix and transform order, 
    /// there are two distinct triples of rotations that will result in
    /// the same overall rotation. This method will find the closest of
    /// the two after finding the closest using the above method.
    void	 roundAngles(const UT_Vector3T<T> &b, const UT_XformOrder &o);

    /// Return the dual of the vector
    /// The dual is a matrix which acts like the cross product when
    /// multiplied by other vectors.
    /// The following are equivalent:
    /// a.getDual(A);  c = colVecMult(A, b)
    /// c = cross(a, b)
    template <typename S>
    void	 getDual(UT_Matrix3T<S> &dual) const;

    /// Protected I/O methods
    // @{
    void	 save(std::ostream &os, bool binary = false) const;
    bool	 load(UT_IStream &is);
    // @}

    /// @{
    /// Methods to serialize to a JSON stream.  The vector is stored as an
    /// array of 3 reals.
    bool		save(UT_JSONWriter &w) const;
    bool		save(UT_JSONValue &v) const;
    bool		load(UT_JSONParser &p);
    /// @}

    /// @{
    /// Method to return the angle (in radians) between this and another vector
    fpreal64 angleTo(const UT_Vector3T<T> &v) const
    {
	return ::UTangleBetween(*this, v);
    }
    /// @}

    /// Returns the vector size
    static int		entries()	{ return tuple_size; }

private:
    /// I/O friends
    // @{
    friend std::ostream	&operator<<(std::ostream &os, const UT_Vector3T<T> &v)
			{
			    v.save(os);
			    return os;
			}
    // @}
};

// Required for constructor
#include "UT_Vector2.h"
#include "UT_Vector4.h"

template <typename T>
SYS_FORCE_INLINE UT_Vector3T<T>::UT_Vector3T(const UT_Vector2T<T> &v)
{
    vec[0] = v.x();
    vec[1] = v.y();
    vec[2] = T(0);
}
template <typename T>
SYS_FORCE_INLINE UT_Vector3T<T>::UT_Vector3T(const UT_Vector4T<T> &v)
{
    vec[0] = v.x();
    vec[1] = v.y();
    vec[2] = v.z();
}

// Free floating functions:
template <typename T>
inline
UT_Vector3T<T>	operator+(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2)
{
    return UT_Vector3T<T>(v1.x()+v2.x(), v1.y()+v2.y(), v1.z()+v2.z());
}

template <typename T>
inline
UT_Vector3T<T>	operator-(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2)
{
    return UT_Vector3T<T>(v1.x()-v2.x(), v1.y()-v2.y(), v1.z()-v2.z());
}

template <typename T, typename S>
inline
UT_Vector3T<T>	operator+(const UT_Vector3T<T> &v, S scalar)
{
    return UT_Vector3T<T>(v.x()+scalar, v.y()+scalar, v.z()+scalar);
}

template <typename T>
inline
UT_Vector3T<T>	operator*(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2)
{
    return UT_Vector3T<T>(v1.x()*v2.x(), v1.y()*v2.y(), v1.z()*v2.z());
}

template <typename T>
inline
UT_Vector3T<T>	operator/(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2)
{
    return UT_Vector3T<T>(v1.x()/v2.x(), v1.y()/v2.y(), v1.z()/v2.z());
}

template <typename T, typename S>
inline
UT_Vector3T<T>	operator+(S scalar, const UT_Vector3T<T> &v)
{
    return UT_Vector3T<T>(v.x()+scalar, v.y()+scalar, v.z()+scalar);
}

template <typename T, typename S>
inline
UT_Vector3T<T>	operator-(const UT_Vector3T<T> &v, S scalar)
{
    return UT_Vector3T<T>(v.x()-scalar, v.y()-scalar, v.z()-scalar);
}

template <typename T, typename S>
inline
UT_Vector3T<T>	operator-(S scalar, const UT_Vector3T<T> &v)
{
    return UT_Vector3T<T>(scalar-v.x(), scalar-v.y(), scalar-v.z());
}

template <typename T, typename S>
inline
UT_Vector3T<T>	operator*(const UT_Vector3T<T> &v, S scalar)
{
    return UT_Vector3T<T>(v.x()*scalar, v.y()*scalar, v.z()*scalar);
}

template <typename T, typename S>
inline
UT_Vector3T<T>	operator*(S scalar, const UT_Vector3T<T> &v)
{
    return UT_Vector3T<T>(v.x()*scalar, v.y()*scalar, v.z()*scalar);
}

template <typename T, typename S>
inline
UT_Vector3T<T> operator/(const UT_Vector3T<T> &v, S scalar)
{
    if (UT_StorageNum<T>::theIsFloat)
    {
        // This has to be T because S may be int for "v = v/2" code
        // For the same reason we must cast the 1
        T inv = ((T)1) / scalar;
        return UT_Vector3T<T>(v.x()*inv, v.y()*inv, v.z()*inv);
    }
    return UT_Vector3T<T>(v.x()/scalar, v.y()/scalar, v.z()/scalar);
}

template <typename T, typename S>
inline
UT_Vector3T<T>	operator/(S scalar, const UT_Vector3T<T> &v)
{
    return UT_Vector3T<T>(scalar/v.x(), scalar/v.y(), scalar/v.z());
}

template <typename T>
inline
T		dot(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2)
{
    return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}

template <typename T>
inline
UT_Vector3T<T>	cross(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2)
{
    // compute the cross product:
    return UT_Vector3T<T>(
	v1.y()*v2.z() - v1.z()*v2.y(),
	v1.z()*v2.x() - v1.x()*v2.z(),
	v1.x()*v2.y() - v1.y()*v2.x()
    );
}

template <typename T>
inline
fpreal64	UTangleBetween(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2)
{
    UT_Vector3T<fpreal64> v1crossv2 = cross(v1, v2);
    fpreal v1dotv2 = dot(v1, v2);
    return SYSatan2(v1crossv2.length(), v1dotv2);
}

template <typename T>
inline
UT_Vector3T<T>	SYSmin(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2)
{
    return UT_Vector3T<T>(
	    SYSmin(v1.x(), v2.x()),
	    SYSmin(v1.y(), v2.y()),
	    SYSmin(v1.z(), v2.z())
    );
}

template <typename T>
inline
UT_Vector3T<T>	SYSmax(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2)
{
    return UT_Vector3T<T>(
	    SYSmax(v1.x(), v2.x()),
	    SYSmax(v1.y(), v2.y()),
	    SYSmax(v1.z(), v2.z())
    );
}

template <typename T,typename S>
inline
UT_Vector3T<T> SYSlerp(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2, S t)
{
    return UT_Vector3T<T>(
	    SYSlerp(v1.x(), v2.x(), t),
	    SYSlerp(v1.y(), v2.y(), t),
	    SYSlerp(v1.z(), v2.z(), t));
}

template <typename T>
inline
T SYStrihat(const UT_Vector3T<T> &v, const UT_Vector3T<T> &s)
{
    return SYShat(v.x(), s.x()) * SYShat(v.y(), s.y()) * SYShat(v.z(), s.z());
}

template <typename T>
inline
UT_Vector3T<T> SYStrihatgrad(const UT_Vector3T<T> &v, const UT_Vector3T<T> &s)
{
    const T xhat = SYShat(v.x(), s.x());
    const T yhat = SYShat(v.y(), s.y());
    const T zhat = SYShat(v.z(), s.z());
    return UT_Vector3T<T>(SYSdhat(v.x(), s.x()) * yhat * zhat,
			 xhat * SYSdhat(v.y(), s.y()) * zhat,
			 xhat * yhat * SYSdhat(v.z(), s.z()));
}

template <typename T>
inline
UT_Vector3T<T>	project(const UT_Vector3T<T> &u, const UT_Vector3T<T> &v)
{
    return dot(u, v) / v.length2() * v;
}

template <typename T>
inline
T	distance3d(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2)
{
    return (v1 - v2).length();
}
template <typename T>
inline
T	distance2(const UT_Vector3T<T> &v1, const UT_Vector3T<T> &v2)
{
    return (v1 - v2).length2();
}

// calculate distance squared of pos to the line segment defined by pt1 to pt2
template <typename T>
inline
T	segmentPointDist2(const UT_Vector3T<T> &pos, 
			  const UT_Vector3T<T> &pt1, const UT_Vector3T<T> &pt2 )
{
    UT_Vector3T<T>	vec;
    T	proj_t;
    T	veclen2;

    vec = pt2 - pt1;
    proj_t = vec.dot( pos - pt1 );
    veclen2 = vec.length2();

    if( proj_t <= (T)0.0 )
    {
	// in bottom cap region, calculate distance from pt1
	vec = pos - pt1;
    }
    else if( proj_t >= veclen2 )
    {
	// in top cap region, calculate distance from pt2
	vec = pos - pt2;
    }
    else
    {
	// middle region, calculate distance from projected pt
	proj_t /= veclen2;
	vec = (pt1 + (proj_t * vec)) - pos;
    }

    return dot(vec, vec);
}

// TODO: review the effiency of the following routine, there is a faster
// way to get just the distance.
template <typename T>
inline
T	 segmentDistance2(const UT_Vector3T<T> &p0, const UT_Vector3T<T> &p1,
			  const UT_Vector3T<T> &q0, const UT_Vector3T<T> &q1)
{
    UT_Vector2 t = segmentClosest(p0, p1, q0, q1);
    UT_Vector3 a = p0 + (p1 - p0) * t[0];
    UT_Vector3 b = q0 + (q1 - q0) * t[1];
    return distance2(a, b);
}

template <typename T>
inline
T 	segmentDistance(const UT_Vector3T<T> &p0, const UT_Vector3T<T> &p1,
			const UT_Vector3T<T> &q0, const UT_Vector3T<T> &q1)
{
    return SYSsqrt(segmentDistance2(p0, p1, q0, q1));
}

/// Given a 3D position, input, and a 3D parallelpiped with corner p0 and
/// directions du, dv, and dw, finds the 0 or 1 locations in the parameter
/// space of that parallelpiped that correspond with the input position.
/// Only a parameter location approximately between 0 and 1
/// is accepted.  The return value is the number of accepted parameter locations,
/// i.e. 0 or 1.
template <typename T>
int UTinverseTrilerpFlat(const UT_Vector3T<T> &input,
    const UT_Vector3T<T> &p0,
    const UT_Vector3T<T> &du, const UT_Vector3T<T> &dv, const UT_Vector3T<T> &dw,
    UT_Vector3T<T> &output)
{
    const UT_Vector3T<T> orig = input - p0;

    const UT_Matrix3T<T> matrix(
        du.x(), dv.x(), dw.x(),
        du.y(), dv.y(), dw.y(),
        du.z(), dv.z(), dw.z()
    );

    bool failed = matrix.solve(orig.x(), orig.y(), orig.z(), output);
    return !failed &&
        SYSisGreaterOrEqual(output.x(), 0) && SYSisLessOrEqual(output.x(), 1) &&
        SYSisGreaterOrEqual(output.y(), 0) && SYSisLessOrEqual(output.y(), 1) &&
        SYSisGreaterOrEqual(output.z(), 0) && SYSisLessOrEqual(output.z(), 1);
}

template <typename T>
inline size_t hash_value(const UT_Vector3T<T> &val)
{
    return val.hash();
}

// Overload for custom formatting of UT_Vector3T<T> with UTformat.
template<typename T>
UT_API size_t format(char *buffer, size_t buffer_size, const UT_Vector3T<T> &v);


template<typename T>
struct UT_FixedVectorTraits<UT_Vector3T<T> >
{
    typedef UT_FixedVector<T,3> FixedVectorType;
    typedef T DataType;
    static const exint TupleSize = 3;
    static const bool isVectorType = true;
};

#endif
