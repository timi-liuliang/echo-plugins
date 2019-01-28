/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_DerVector3_h__
#define __SIM_DerVector3_h__

#include "SIM_API.h"
#include <UT/UT_Matrix3.h>
#include <UT/UT_Vector3.h>

class SIM_DerScalar;

/// This class defines a 3D vector and its partial derivative w.r.t. another
/// 3D vector. It uses automatic differentiation to maintain the dependency
/// upon the derivative vector as arithmetic operations are performed.
/// The derivative of a vector-valued function is, of course, a Jacobian
/// matrix.
///
/// By performing a sequence of arithmetic operations on this
/// vector class after initializing its derivative appropriately, you can
/// easily keep track of the effect of those operations on the derivative.
/// Independent variables can be included in an equation using the
/// conventional UT_Vector3 and fpreal types, and dependent variables can
/// use the SIM_DerVector3 and SIM_DerScalar types.
///
/// It is inspired by Eitan Grinspun's class for the same purpose,
/// described in his 2003 SCA paper on Discrete Shells.
class SIM_API SIM_DerVector3
{
public:
		     SIM_DerVector3() { }
    /// Initialize to a constant vector, with no derivative.
    explicit	     SIM_DerVector3(const UT_Vector3 &v) : myV(v), myD(0.f)
		     { }
    /// Initialize to a vector with a derivative. This is particularly
    /// useful for initializing the variables themselves, where D=I.
		     SIM_DerVector3(const UT_Vector3 &v,
				    const UT_Matrix3 &D) : myV(v), myD(D)
		     { }

    // Default copy constructor is fine.
    //SIM_DerVector3(const SIM_DerVector3 &rhs);

    /// The vector v.
    const UT_Vector3&v() const
		     { return myV; }

    /// Derivative matrix, dv/dx.
    /// The entries of the matrix are laid out like a typical Jacobian:
    ///
    /// [  dv1/dx1  dv1/dx2  dv1/dx3  ]
    /// [  dv2/dx1  dv2/dx2  dv2/dx3  ]
    /// [  dv3/dx1  dv3/dx2  dv3/dx3  ]
    ///
    ///   [ dv1/dx ]   
    /// = [ dv2/dx ]
    ///   [ dv3/dx ]
    ///
    /// = [ dv/dx1  dv/dx2  dv/dx3 ]
    const UT_Matrix3&D() const
		     { return myD; }

    // Default assignment operator is fine.
    // SIM_DerVector3   operator=(const SIM_DerVector3 &rhs);

    SIM_DerVector3   operator-() const
                     {
                        return SIM_DerVector3(-v(), -D());
                     }
    SIM_DerVector3   operator+(const SIM_DerVector3 &rhs) const
                     {
                        // d(v1+v2)/dx = dv1/dx + dv2/dx
                        return SIM_DerVector3(v() + rhs.v(), D() + rhs.D());
                     }
    SIM_DerVector3   operator+(const UT_Vector3 &rhs) const
                     {
                        return SIM_DerVector3(v() + rhs, D());
                     }
    SIM_DerVector3   operator-(const SIM_DerVector3 &rhs) const
                     {
                        // d(v1-v2)/dx = dv1/dx - dv2/dx
                        return SIM_DerVector3(v() - rhs.v(), D() - rhs.D());
                     }
    SIM_DerVector3   operator-(const UT_Vector3 &rhs) const
                     {
                        return SIM_DerVector3(v() - rhs, D());
                     }
    SIM_DerVector3   operator*(const SIM_DerScalar &rhs) const;
    SIM_DerVector3   operator*(fpreal scalar) const
                     {
                        // d(v*s)/dx = s*dv/dx + v * (ds/dx)^T
                        return SIM_DerVector3(v() * scalar, D() * scalar);
                     }
    SIM_DerVector3&  operator+=(const SIM_DerVector3 &rhs)
                     { return operator=((*this) + rhs); }
    SIM_DerVector3&  operator+=(const UT_Vector3 &rhs)
                     { return operator=((*this) + rhs); }
    SIM_DerVector3&  operator-=(const SIM_DerVector3 &rhs)
                     { return operator=((*this) - rhs); }
    SIM_DerVector3&  operator-=(const UT_Vector3 &rhs)
                     { return operator=((*this) - rhs); }
    SIM_DerVector3&  operator*=(const SIM_DerScalar &rhs)
                     { return operator=((*this) * rhs); }
    SIM_DerVector3&  operator*=(const fpreal rhs)
                     { return operator=((*this) * rhs); }

    SIM_DerScalar    dot(const SIM_DerVector3 &rhs) const;
    SIM_DerScalar    dot(const UT_Vector3 &rhs) const;
    SIM_DerVector3   cross(const SIM_DerVector3 &rhs) const;
    SIM_DerVector3   cross(const UT_Vector3 &rhs) const;
    SIM_DerScalar    length2() const;
    SIM_DerScalar    length() const;
    SIM_DerVector3   normalize() const;


    // Matrix corresponding to a vector cross-product.
    //   a x b = S(a) * b
    // The matrix is skew-symmetric.
    static UT_Matrix3 S(const UT_Vector3 &v)
                     {
                        return UT_Matrix3( 0, -v.z(), v.y(),
                                           v.z(), 0, -v.x(),
                                           -v.y(), v.x(), 0);
                     }

private:
    UT_Vector3	 myV;
    UT_Matrix3	 myD;
};

#include "SIM_DerScalar.h"

inline
SIM_DerVector3	 operator+(const UT_Vector3 &lhs, const SIM_DerVector3 &rhs);
inline
SIM_DerVector3	 operator-(const UT_Vector3 &lhs, const SIM_DerVector3 &rhs);
inline
SIM_DerVector3 	 operator*(const SIM_DerScalar &s, const SIM_DerVector3 &v);
inline
SIM_DerVector3	 operator*(fpreal s, const SIM_DerVector3 &v);
inline
SIM_DerVector3	 operator/(const SIM_DerVector3 &v, const SIM_DerScalar &s);
inline
SIM_DerVector3	 operator/(const SIM_DerVector3 &v, fpreal s);
inline
SIM_DerScalar	 dot(const SIM_DerVector3 &lhs, const SIM_DerVector3 &rhs);
inline
SIM_DerScalar	 dot(const SIM_DerVector3 &lhs, const UT_Vector3 &rhs);
inline
SIM_DerScalar	 dot(const UT_Vector3 &lhs, const SIM_DerVector3 &rhs);
inline
SIM_DerVector3	 cross(const SIM_DerVector3 &lhs, const SIM_DerVector3 &rhs);
inline
SIM_DerVector3	 cross(const SIM_DerVector3 &lhs, const UT_Vector3 &rhs);
inline
SIM_DerVector3	 cross(const UT_Vector3 &lhs, const SIM_DerVector3 &rhs);



inline SIM_DerVector3
SIM_DerVector3::operator*(const SIM_DerScalar &rhs) const
{
    // d(v*s)/dx = s*dv/dx + v * (ds/dx)^T
    UT_Matrix3 newD(D());
    newD *= rhs.v();
    newD.outerproductUpdate(1, v(), rhs.D());

    return SIM_DerVector3(v() * rhs.v(), newD);
}

inline SIM_DerScalar
SIM_DerVector3::dot(const SIM_DerVector3 &rhs) const
{
    // d(v1.v2)/dx = v1^T * dv2/dx + v2^T * dv1/dx
    // Note the rowvector*matrix multiplication.
    return SIM_DerScalar(::dot(v(), rhs.v()),
			 ::rowVecMult(rhs.v(), D()) +
			 ::rowVecMult(v(), rhs.D()));
}

inline SIM_DerScalar
SIM_DerVector3::dot(const UT_Vector3 &rhs) const
{
    return SIM_DerScalar(::dot(v(), rhs),
			 ::rowVecMult(rhs, D()));
}

// d(v1 x v2)/dx = dv1/dx x v2 + v1 x dv2/dx
//               = -v2 x dv1/dx + v1 x dv2/dx
//               = S(-v2) dv1/dx + S(v1) dv2/dx
inline SIM_DerVector3
SIM_DerVector3::cross(const SIM_DerVector3 &rhs) const
{
    return SIM_DerVector3(::cross(v(), rhs.v()),
			  S(-rhs.v()) * D() + S(v()) * rhs.D());
}

// d(v1 x v2)/dx = dv1/dx x v2 + v1 x dv2/dx
//               = -v2 x dv1/dx + v1 x dv2/dx
//               = S(-v2) dv1/dx + S(v1) dv2/dx
inline SIM_DerVector3
SIM_DerVector3::cross(const UT_Vector3 &rhs) const
{
    return SIM_DerVector3(::cross(v(), rhs), S(-rhs) * D());
}

// d|v|^2/dx = d|v.v|/dx
//           = 2 * v * dv/dx
inline SIM_DerScalar
SIM_DerVector3::length2() const
{
    return SIM_DerScalar(v().length2(),
			 2 * ::rowVecMult(v(), D()));
}

// d|v|/dx = d((v.v)^.5)/dx
//         = .5 / |v| * d(v.v)/dx
//         = v / |v| * dv/dx

// Because it includes a square root, there is a discontinuity at the
// origin. Like the square root, I approximate using a zero derivative at
// the origin.
inline SIM_DerScalar
SIM_DerVector3::length() const
{
    const fpreal tol = 1e-5;
    const fpreal len = v().length();
    if( len < tol )
	return SIM_DerScalar(len);
    else
	return SIM_DerScalar(len, ::rowVecMult(v() / len, D()));
}

inline SIM_DerVector3
SIM_DerVector3::normalize() const
{
    // TODO: we can make this more efficient... can't we?
    return (*this)/length();
}





inline SIM_DerVector3
operator+(const UT_Vector3 &lhs, const SIM_DerVector3 &rhs)
{
    return rhs + lhs;
}

inline SIM_DerVector3
operator-(const UT_Vector3 &lhs, const SIM_DerVector3 &rhs)
{
    return SIM_DerVector3(lhs - rhs.v(), -rhs.D());
}

inline SIM_DerVector3
operator*(const SIM_DerScalar &s, const SIM_DerVector3 &v)
{
    return v * s;
}

inline SIM_DerVector3
operator*(fpreal s, const SIM_DerVector3 &v)
{
    return v * s;
}

inline SIM_DerVector3
operator/(const SIM_DerVector3 &v, const SIM_DerScalar &s)
{
    return v * s.inverse();
}

inline SIM_DerVector3
operator/(const SIM_DerVector3 &v, fpreal s)
{
    return v * (1/s);
}

inline SIM_DerScalar
dot(const SIM_DerVector3 &lhs, const SIM_DerVector3 &rhs)
{
    return lhs.dot(rhs);
}

inline SIM_DerScalar
dot(const SIM_DerVector3 &lhs, const UT_Vector3 &rhs)
{
    return lhs.dot(rhs);
}

inline SIM_DerScalar
dot(const UT_Vector3 &lhs, const SIM_DerVector3 &rhs)
{
    return rhs.dot(lhs);
}

inline SIM_DerVector3
cross(const SIM_DerVector3 &lhs, const SIM_DerVector3 &rhs)
{
    return lhs.cross(rhs);
}

inline SIM_DerVector3
cross(const SIM_DerVector3 &lhs, const UT_Vector3 &rhs)
{
    return lhs.cross(rhs);
}

inline SIM_DerVector3
cross(const UT_Vector3 &lhs, const SIM_DerVector3 &rhs)
{
    // d(v1 x v2)/dx = dv1/dx x v2 + v1 x dv2/dx
    //               = -v2 x dv1/dx + v1 x dv2/dx
    //               = S(-v2) dv1/dx + S(v1) dv2/dx
    return SIM_DerVector3(::cross(lhs, rhs.v()),
			  SIM_DerVector3::S(lhs) * rhs.D());
}
#endif
