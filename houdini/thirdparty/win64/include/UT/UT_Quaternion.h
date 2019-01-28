/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library (C++)
 *
 * COMMENTS:	This class implements quaternions.
 *
 * WARNING:
 *	This class should NOT contain any virtual methods, nor should it
 *	define more member data. The size of UT_QuaternionF must always be 
 *	16 bytes (4 floats).
 *
 */

#pragma once

#ifndef __UT_Quaternion_h__
#define __UT_Quaternion_h__

#include "UT_API.h"
#include "UT_FixedVector.h"
#include "UT_Vector3.h"
#include "UT_Vector4.h"
#include "UT_VectorTypes.h"

#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>
#include <limits>
#include <stddef.h>

class UT_IStream;
class UT_JSONParser;
class UT_JSONValue;
class UT_JSONWriter;
class UT_XformOrder;

// Forward declaration
template <typename T> class UT_API UT_QuaternionT;

/// Perform component-wise SYSlerp of two quaternions
template <typename T>
inline UT_QuaternionT<T>
SYSlerp(const UT_QuaternionT<T> &q1, const UT_QuaternionT<T> &q2, T t);

/// Quaternion class
template <typename T>
class UT_API UT_QuaternionT
{
public:

    typedef T			 value_type;
    static const int		 tuple_size = 4;

				 UT_QuaternionT(T qx=0, T qy=0,
					       T qz=0, T qw=0)
				 {
				     vec[0] = qx;  vec[1] = qy;
				     vec[2] = qz;  vec[3] = qw;
				 }
				 UT_QuaternionT(const fpreal32 v[tuple_size])
				 {
				    vec[0] = v[0]; vec[1] = v[1];
				    vec[2] = v[2]; vec[3] = v[3];
				 }
				 UT_QuaternionT(const fpreal64 v[tuple_size])
				 {
				    vec[0] = v[0]; vec[1] = v[1];
				    vec[2] = v[2]; vec[3] = v[3];
				 }
				 UT_QuaternionT(const UT_Vector4T<T> &v)
				 {
				    vec[0] = v[0]; vec[1] = v[1];
				    vec[2] = v[2]; vec[3] = v[3];
				 }
                                 /// Arbitrary UT_FixedVector of the same size
                                 template <typename S>
                                 SYS_FORCE_INLINE explicit UT_QuaternionT(const UT_FixedVector<S, tuple_size> &v)
                                 {
                                     vec[0] = v[0]; vec[1] = v[1];
                                     vec[2] = v[2]; vec[3] = v[3];
                                 }
                                 inline UT_QuaternionT(T angle, 
						const UT_Vector3T<T> &axis, 
						int donormalize=1);
				 UT_QuaternionT(const UT_Vector3T<T> &rot,
					       const UT_XformOrder &order)
				 {
				     updateFromEuler(rot, order);
				 }
				 
    typedef UT_QuaternionT<T>	 ThisType;

    SYS_FORCE_INLINE		 UT_QuaternionT(const ThisType &that) = default;
    SYS_FORCE_INLINE		 UT_QuaternionT(ThisType &&that) = default;
    SYS_FORCE_INLINE ThisType	&operator=(const ThisType &that) = default;
    SYS_FORCE_INLINE ThisType	&operator=(ThisType &&that) = default;

    template <typename S> 
    inline UT_QuaternionT<T>(const UT_QuaternionT<S> &v)
    { vec[0] = v.x(); vec[1] = v.y(); vec[2] = v.z(); vec[3] = v.w(); }
    template <typename S> 
    inline UT_QuaternionT<T> &operator=(const UT_QuaternionT<S> &v)
    { vec[0] = v.x(); vec[1] = v.y(); vec[2] = v.z(); vec[3] = v.w(); return *this; }

    inline UT_QuaternionT<T>	&operator*=(const UT_QuaternionT<T> &q);
    inline UT_QuaternionT<T>	&operator*=(T scalar);
    inline UT_QuaternionT<T>	&operator/=(const UT_QuaternionT<T> &quat);
    inline UT_QuaternionT<T>	&operator/=(T scalar);
    inline UT_QuaternionT<T>	&operator+=(const UT_QuaternionT<T> &quat);
    inline bool			 operator==(const UT_QuaternionT<T> &quat) const;
    inline bool			 operator!=(const UT_QuaternionT<T> &quat) const;
    T			 	 operator()(int idx) const
				 { return vec[idx]; }
    T				&operator()(int idx)
				 { return vec[idx]; }
    T				 operator[](int idx) const
				 { return vec[idx]; }
    T				&operator[](int idx)
				 { return vec[idx]; }

    /// Does a comparison with a tolerance. This also returns true if
    /// quat.negated() is equal to us, unlike operator==().
    inline bool			 isEqual(const UT_QuaternionT<T> &quat,
					 T tol = T(SYS_FTOLERANCE)) const;

    // The rotation this quaternion represents as a matrix
    void			 getRotationMatrix(UT_Matrix3 &mat) const;
    void			 getRotationMatrix(UT_DMatrix3 &mat) const;
    void			 getInverseRotationMatrix(UT_Matrix3 &mat) const;
    void			 getInverseRotationMatrix(UT_DMatrix3 &mat) const;

    void			 getTransformMatrix(UT_Matrix4 &mat) const;
    void			 getTransformMatrix(UT_DMatrix4 &mat) const;

    /// Interpolates between this quat (t==0) and the target (t==1)
    UT_QuaternionT<T>		 interpolate(const UT_QuaternionT<T> &target,
					     T t, T b = 0.0f) const;
    /// Interpolates between the n quaternions in q, with weights w,
    /// to within tolerance tol.
    /// NOTE: The q's must be normalized, and the weights may need to sum to 1.
    void interpolate(const UT_QuaternionT<T> *q, const T *w, exint n, T tol = T(1e-6));

    /// Do component-wise lerp between this quat (t=0) and the target (t=1).
    void			 lerp(const UT_QuaternionT<T> &target, T t)
    {
	vec[0] = SYSlerp(vec[0], target.vec[0], t);
	vec[1] = SYSlerp(vec[1], target.vec[1], t);
	vec[2] = SYSlerp(vec[2], target.vec[2], t);
	vec[3] = SYSlerp(vec[3], target.vec[3], t);
    }
    /// Do component-wise lerp between this src (t=0) and dst (t=1).
    void			 lerp(const UT_QuaternionT<T> &src,
				      const UT_QuaternionT<T> &dst,
				      T t)
    {
	vec[0] = SYSlerp(src.vec[0], dst.vec[0], t);
	vec[1] = SYSlerp(src.vec[1], dst.vec[1], t);
	vec[2] = SYSlerp(src.vec[2], dst.vec[2], t);
	vec[3] = SYSlerp(src.vec[3], dst.vec[3], t);
    }

    void			 assign(T qx, T qy,
					T qz, T qw)
				    {
					vec[0] = qx;  vec[1] = qy;
					vec[2] = qz;  vec[3] = qw;
				    }
    void			 identity()
				    {
					vec[0] = vec[1] = vec[2] = 0.0f;
					vec[3] = 1.0f;
				    }
    void			 conjugate()
				    {
					vec[0] = -vec[0];
					vec[1] = -vec[1];
					vec[2] = -vec[2];
				    }
    void			 negate()
				    {
					vec[0] = -vec[0];
					vec[1] = -vec[1];
					vec[2] = -vec[2];
					vec[3] = -vec[3];
				    }
    T			 	normal() const
				    {
					return	vec[0] * vec[0] +
						vec[1] * vec[1] +
						vec[2] * vec[2] +
						vec[3] * vec[3];
				    }
    void			 normalize()
				    {
					T dn = normal();
                                        if (dn > std::numeric_limits<T>::min()
                                            && dn != 1.0)
					{
					    dn = SYSsqrt(dn);
					    *this /= dn;
					}
				    }
    T				 length() const
				    {
					return SYSsqrt(normal());
				    }
    void			 invert()
				    {
					T n = normal();
                                        if (n > std::numeric_limits<T>::min())
                                        {
                                            n = 1.0 / n;
                                            conjugate();
                                            vec[0] *= n;
                                            vec[1] *= n;
                                            vec[2] *= n;
                                            vec[3] *= n;
                                        }
				    }

    UT_QuaternionT<T> exp() const;
    UT_QuaternionT<T> ln() const;
    inline UT_QuaternionT<T> log() const
    {
        return ln();
    }

    // Form the quaternion which takes v1 and rotates it to v2. 
    // v1 and v2 are assumed normalized
    void			 updateFromVectors(const UT_Vector3T<T> &v1,
						   const UT_Vector3T<T> &v2);

    /// Form the quaternion from the rotation component of an
    /// arbitrary 3x3 matrix.
    void			 updateFromArbitraryMatrix(const UT_Matrix3 &);
    void			 updateFromArbitraryMatrix(const UT_Matrix3D &);

    /// Form the quaternion from a rotation matrix
    /// WARNING: This will produce incorrect results if given
    ///          a non-rotation matrix!  Use updateFromArbitraryMatrix
    ///          if you may have a non-rotation matrix.
    void			 updateFromRotationMatrix(const UT_Matrix3 &);
    void			 updateFromRotationMatrix(const UT_Matrix3D &);

    // Form the quaternion from an angle/axis
    void			 updateFromAngleAxis(T angle, 
						     const UT_Vector3T<T> &axis,
						     int normalize=1);

    void			 getAngleAxis(T &angle,
					      UT_Vector3T<T> &axis) const;

    void			 updateFromLogMap(const UT_Vector3T<T> &v);
    void			 getLogMap(UT_Vector3T<T> &v) const;

    // Form the quaternion from euler rotation angles (given in radians)
    void			 updateFromEuler(const UT_Vector3T<T> &rot,
						 const UT_XformOrder &order);

    // Given the angular velocity omega, compute our derivative into q_prime
    void			 computeDerivative(const UT_Vector3T<T> &omega,
						   UT_QuaternionT<T> &q_prime);

    // Returns the angular velocity required to move from the rotation of
    // this quaternion to the destination quaternion in a given time.
    UT_Vector3T<T>		 computeAngVel(const UT_QuaternionT<T> &dest,
					       T time) const;

    // Integrates this quaternion by the given angular velocity and time
    // step.  There are two appraoches to doing this.  For small
    // angular velocity/timesteps, one can compute the derivative
    // implied by the angular velocity, apply linearly, and renormalize.
    // Alternatively, one can construct the proper quaternion for the given
    // angular velocity and rotate by that.  Which method is controlled
    // by the accurate flag.
    void			 integrate(const UT_Vector3T<T> &angvel, 
					   T timestep,
					   bool accurate = true);

    // Returns the rx/ry/rz euler rotation representation of the quaternion.
    // The returned rotations are in radians.
    UT_Vector3T<T>		 computeRotations(const UT_XformOrder &) const;

    /// Rotates a vector by this quaternion
    /// Requires that this is normalized.
    inline UT_Vector3T<T>	 rotate(const UT_Vector3T<T> &) const;

    /// rotates a vector by the inverse of this quaternion.
    /// Requires that this is normalized.
    inline UT_Vector3T<T>	 rotateInverse(const UT_Vector3T<T> &) const;

    // Multiply this quarternion's "real world" Euler angles by the given
    // scalar s.  That is, if this quaternion is 
    // [ cos(a), n1 sin(a), n2 sin(a), n3 sin(a) ]  it is replaced by
    // [ cos(a*s), n1 sin(a*s), n2 sin(a*s), n3 sin(a*s) ] 
    void			multAngle( T s );

    T				&x() { return vec[0]; }
    T				&y() { return vec[1]; }
    T				&z() { return vec[2]; }
    T				&w() { return vec[3]; }

    T				x() const { return vec[0]; }
    T				y() const { return vec[1]; }
    T				z() const { return vec[2]; }
    T				w() const { return vec[3]; }

    void			save(std::ostream &os, int binary=0) const;
    bool			load(UT_IStream &is);

    /// @{
    /// Methods to serialize to a JSON stream.  The vector is stored as an
    /// array of 4 reals.
    bool		save(UT_JSONWriter &w) const;
    bool		save(UT_JSONValue &v) const;
    bool		load(UT_JSONParser &p);
    /// @}

    const T	*data(void) const	{ return &vec[0]; }
    T		*data(void)		{ return &vec[0]; }

    static int	 entries()		{ return tuple_size; }

    /// Compute a hash
    unsigned	hash() const	{ return SYSvector_hash(data(), tuple_size); }

protected:
    void			 initialize(T qx = 0, T qy = 0, 
					    T qz = 0, T qw = 0)
				     {
					 vec[0] = qx; vec[1] = qy; 
					 vec[2] = qz; vec[3] = qw;
				     }
private:
    // I/O friends:
    friend std::ostream	&operator<<(std::ostream &os, const UT_QuaternionT<T> &v)
			{
			    v.save(os);
			    return os;
			}
    T			vec[tuple_size];
};

template <typename T>
UT_API size_t format(char *buf, size_t buf_size, const UT_QuaternionT<T> &q);

template <typename T>
inline
UT_QuaternionT<T>::UT_QuaternionT(T angle, const UT_Vector3T<T> &axis,
			     int donormalize)
{
    updateFromAngleAxis(angle, axis, donormalize);
}

template <typename T>
inline bool
UT_QuaternionT<T>::operator==(const UT_QuaternionT<T> &quat) const
{
    return (vec[0] == quat.vec[0] &&
	    vec[1] == quat.vec[1] &&
	    vec[2] == quat.vec[2] &&
	    vec[3] == quat.vec[3]);
}

template <typename T>
inline bool
UT_QuaternionT<T>::operator!=(const UT_QuaternionT<T> &quat) const
{
    return !(*this == quat);
}

template <typename T>
inline bool
UT_QuaternionT<T>::isEqual(const UT_QuaternionT<T> &quat, T tol) const
{
    // Two quaternions are equal if all values are equal, or if all values
    // are equal magnitude but opposite sign. Both sets of values represent
    // the same overall rotation.
    return ((SYSisEqual(vec[0], quat.vec[0], tol) &&
	     SYSisEqual(vec[1], quat.vec[1], tol) &&
	     SYSisEqual(vec[2], quat.vec[2], tol) &&
	     SYSisEqual(vec[3], quat.vec[3], tol)) ||
	    (SYSisEqual(-vec[0], quat.vec[0], tol) &&
	     SYSisEqual(-vec[1], quat.vec[1], tol) &&
	     SYSisEqual(-vec[2], quat.vec[2], tol) &&
	     SYSisEqual(-vec[3], quat.vec[3], tol)));
}

template <typename T>
inline UT_QuaternionT<T>
operator*(const UT_QuaternionT<T> &q1, const UT_QuaternionT<T> &q2)
{
    UT_QuaternionT<T>	 product = q1;

    product *= q2;

    return UT_QuaternionT<T>(product);
}

template <typename T>
inline UT_QuaternionT<T>
operator+(const UT_QuaternionT<T> &q1, const UT_QuaternionT<T> &q2)
{
    return UT_QuaternionT<T>(q1.x() + q2.x(),
   			 q1.y() + q2.y(),
			 q1.z() + q2.z(),
			 q1.w() + q2.w());
}

template <typename T>
inline UT_QuaternionT<T> &
UT_QuaternionT<T>::operator+=(const UT_QuaternionT<T> &quat)
{
    vec[0] += quat.vec[0];
    vec[1] += quat.vec[1];
    vec[2] += quat.vec[2];
    vec[3] += quat.vec[3];

    return *this;
}

template <typename T>
inline UT_QuaternionT<T>
operator-(const UT_QuaternionT<T> &q1, const UT_QuaternionT<T> &q2)
{
    return UT_QuaternionT<T>(q1.x() - q2.x(),
   			 q1.y() - q2.y(),
			 q1.z() - q2.z(),
			 q1.w() - q2.w());
}

template <typename T>
inline UT_QuaternionT<T>
operator-(const UT_QuaternionT<T> &q)
{
    return UT_QuaternionT<T>(-q.x(),
   			 -q.y(),
			 -q.z(),
			 -q.w());
}

template <typename T>
inline UT_QuaternionT<T>
operator*(const UT_QuaternionT<T> &q, T scalar)
{
    return UT_QuaternionT<T>(q.x() * scalar,
			 q.y() * scalar,
			 q.z() * scalar,
			 q.w() * scalar);
}

template <typename T>
inline UT_QuaternionT<T>
operator*(T scalar, const UT_QuaternionT<T> &q)
{
    return UT_QuaternionT<T>(q.x() * scalar,
			 q.y() * scalar,
			 q.z() * scalar,
			 q.w() * scalar);
}

template <typename T>
inline UT_QuaternionT<T> &
UT_QuaternionT<T>::operator*=(const UT_QuaternionT<T> &q)
{
    UT_Vector3T<T>	 v1(vec[0], vec[1], vec[2]);
    UT_Vector3T<T>	 v2(q.vec[0], q.vec[1], q.vec[2]);
    UT_Vector3T<T>	 v3;
    T			 s1 = vec[3], s2 = q.vec[3];

    vec[3] = s1*s2 - v1.dot(v2);
    v3 = s1*v2 + s2*v1 + cross(v1, v2);
    vec[0] = v3[0];
    vec[1] = v3[1];
    vec[2] = v3[2];

    return *this;
}

template <typename T>
inline UT_QuaternionT<T> &
UT_QuaternionT<T>::operator*=(T scalar)
{
    vec[0] *= scalar;
    vec[1] *= scalar;
    vec[2] *= scalar;
    vec[3] *= scalar;

    return *this;
}

template <typename T>
inline UT_QuaternionT<T>
operator/(const UT_QuaternionT<T> &a, const UT_QuaternionT<T> &b)
{
    UT_QuaternionT<T>	a1 = a;
    UT_QuaternionT<T>	b1 = b;

    b1.invert();
    a1 *= b1;

    return UT_QuaternionT<T>(a1);
}

template <typename T>
inline UT_QuaternionT<T>
operator/(const UT_QuaternionT<T> &q, T scalar)
{
    T d = 1.0/scalar;
    return UT_QuaternionT<T>(q.x()*d, q.y()*d, q.z()*d, q.w()*d);
}

template <typename T>
inline UT_QuaternionT<T> &
UT_QuaternionT<T>::operator/=(const UT_QuaternionT<T> &quat)
{
    UT_QuaternionT<T> q = quat;

    q.invert();
    operator*=(q);

    return *this;
}

template <typename T>
inline UT_QuaternionT<T> &
UT_QuaternionT<T>::operator/=(T scalar)
{
    T d = 1.0F/scalar;

    vec[0] *= d;
    vec[1] *= d;
    vec[2] *= d;
    vec[3] *= d;

    return *this;
}

template <typename T>
inline UT_Vector3T<T>
UT_QuaternionT<T>::rotate(const UT_Vector3T<T> &v) const
{
    UT_QuaternionT<T> q = (*this) *
		      UT_QuaternionT<T>(v.x(), v.y(), v.z(), 0.0f) * 
		      UT_QuaternionT<T>(-vec[0], -vec[1], -vec[2], vec[3]);
    return UT_Vector3T<T>(q.x(), q.y(), q.z());
}

template <typename T>
inline UT_Vector3T<T>
UT_QuaternionT<T>::rotateInverse(const UT_Vector3T<T> &v) const
{
    UT_QuaternionT<T> q = UT_QuaternionT<T>(-vec[0], -vec[1], -vec[2], vec[3]) *
		      UT_QuaternionT<T>(v.x(), v.y(), v.z(), 0.0f) * 
		      (*this);
    return UT_Vector3T<T>(q.x(), q.y(), q.z());
}

template <typename T>
inline T
dot( const UT_QuaternionT<T> &q1, const UT_QuaternionT<T> &q2 )
{
    return q1.x()*q2.x() + q1.y()*q2.y() + q1.z()*q2.z() + q1.w()*q2.w();
}

template <typename T>
inline size_t
hash_value(const UT_QuaternionT<T> &val)
{
    return val.hash();
}

typedef UT_QuaternionT<fpreal32>	UT_Quaternion;
typedef UT_QuaternionT<fpreal>		UT_QuaternionR;
typedef UT_QuaternionT<fpreal16>	UT_QuaternionH;
typedef UT_QuaternionT<fpreal32>	UT_QuaternionF;
typedef UT_QuaternionT<fpreal64>	UT_QuaternionD;

template<typename T>
struct UT_FixedVectorTraits<UT_QuaternionT<T> >
{
    typedef UT_FixedVector<T,4> FixedVectorType;
    typedef T DataType;
    static const exint TupleSize = 4;
    static const bool isVectorType = true;
};

///////////////////////////////////////////////////////////////////////////////
//
// Implementations
//

template <typename T>
inline UT_QuaternionT<T>
SYSlerp(const UT_QuaternionT<T> &q1, const UT_QuaternionT<T> &q2, T t)
{
    return UT_QuaternionT<T>( SYSlerp(q1.x(), q2.x(), t)
			    , SYSlerp(q1.y(), q2.y(), t)
			    , SYSlerp(q1.z(), q2.z(), t)
			    , SYSlerp(q1.w(), q2.w(), t)
			    );
}

#endif
