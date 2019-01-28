/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_DerScalar_h__
#define __SIM_DerScalar_h__

#include "SIM_API.h"
#include <UT/UT_Vector3.h>

class SIM_DerVector3;

/// This class defines a scalar and its derivative w.r.t. a 3D vector.
/// It uses automatic differentiation to maintain the dependency
/// upon the derivative vector as arithmetic operations are performed.
///
/// By performing a sequence of arithmetic operations on this
/// class after initializing its derivative appropriately, you can
/// easily keep track of the effect of those operations on the derivative.
/// Independent variables can be included in an equation using the
/// conventional UT_Vector3 and fpreal types, and dependent variables can
/// use the SIM_DerVector3 and SIM_DerScalar types.
///
/// It is inspired by Eitan Grinspun's class for the same purpose,
/// described in his 2003 SCA paper on Discrete Shells.
class SIM_API SIM_DerScalar
{
public:
		     SIM_DerScalar() { }
    /// Initialize to a constant vector, with no derivative.
    explicit	     SIM_DerScalar(fpreal v) : myV(v), myD(0.f, 0.f, 0.f)
		     { }
    /// Initialize to a vector with a derivative. This is particularly
    /// useful for initializing the variables themselves, where D=I.
		     SIM_DerScalar(fpreal v, const UT_Vector3 &D)
			 : myV(v), myD(D)
		     { }

    // Default copy constructor is fine.
    //SIM_DerScalar(const SIM_DerScalar &rhs);

    // The scalar v.
    fpreal	     v() const
		     { return myV; }
    // The derivative, dv/dx = [ dv/dx1  dv/dx2  dv/dx3 ]^T
    const UT_Vector3&D() const
		     { return myD; }

    // Default assignment operator is fine.
    // SIM_DerScalar   operator=(const SIM_DerScalar &rhs);
    SIM_DerScalar   &operator=(fpreal rhs)
		     { return operator=(SIM_DerScalar(rhs)); }

    SIM_DerScalar    operator-() const
                     {
                        // d(-v)/dx = -dv/dx
                        return SIM_DerScalar(-v(), -D());
                     }
    SIM_DerScalar    operator+(const SIM_DerScalar &rhs) const
                     {
                        // d(v1+v2)/dx = dv1/dx + dv2/dx
                        return SIM_DerScalar(v() + rhs.v(), D() + rhs.D());
                     }
    SIM_DerScalar    operator+(fpreal rhs) const
                     {
                        return SIM_DerScalar(v() + rhs, D());
                     }
    SIM_DerScalar    operator-(const SIM_DerScalar &rhs) const
                     {
                        // d(v1-v2)/dx = dv1/dx - dv2/dx
                        return SIM_DerScalar(v() - rhs.v(), D() - rhs.D());
                     }
    SIM_DerScalar    operator-(fpreal rhs) const
                     {
                        return SIM_DerScalar(v() - rhs, D());
                     }
    SIM_DerScalar    operator*(const SIM_DerScalar &rhs) const
                     {
                        // d(v1*v2)/dx = v1 * dv2/dx + v2 * dv1/dx
                        return SIM_DerScalar(v() * rhs.v(),
                                             v() * rhs.D() + rhs.v() * D());
                     }
    SIM_DerScalar    operator*(fpreal rhs) const
                     {
                        // d(v1*v2)/dx = v1 * dv2/dx + v2 * dv1/dx
                        return SIM_DerScalar(v() * rhs, rhs * D());
                     }
    SIM_DerScalar    sqr() const
                     {
                         return SIM_DerScalar(v() * v(), (2 * v()) * D());
                     }
    SIM_DerScalar    operator/(const SIM_DerScalar &rhs) const
		     { return operator*(rhs.inverse()); }
    SIM_DerScalar    operator/(fpreal rhs) const
		     { return operator*(1/rhs); }
    SIM_DerScalar    operator+=(const SIM_DerScalar &rhs)
		     { return operator=(*this + rhs); }
    SIM_DerScalar    operator+=(fpreal rhs)
		     { return operator=(*this + rhs); }
    SIM_DerScalar    operator-=(const SIM_DerScalar &rhs)
		     { return operator=(*this - rhs); }
    SIM_DerScalar    operator-=(fpreal rhs)
		     { return operator=(*this - rhs); }
    SIM_DerScalar    operator*=(const SIM_DerScalar &rhs)
		     { return operator=(*this * rhs); }
    SIM_DerScalar    operator*=(fpreal rhs)
		     { return operator=(*this * rhs); }
    SIM_DerScalar    operator/=(const SIM_DerScalar &rhs)
		     { return operator=(*this / rhs); }
    SIM_DerScalar    operator/=(fpreal rhs)
		     { return operator=(*this * (1/rhs)); }
    SIM_DerScalar    inverse() const
                     {
                        // d(v^-1)/dx = -dv/dx * v^-2
                        UT_ASSERT_P(v() == v());
                        UT_ASSERT_P(D() == D());
                        return SIM_DerScalar(1/v(), D() / (-v() * v()));
                     }
    // The derivative for this is tricky near zero. It's discontinuous at
    // zero (switches from real to imaginary), and approaches infinity as
    // we get close to zero.
    // I approximate it as being equal to zero within a tolerance of the
    // origin.
    SIM_DerScalar    sqrt() const
                     {
                        const fpreal tol = 1e-5;
                        // d(sqrt(v))/dx = .5 / sqrt(v) * dv/dx
                        UT_ASSERT_P(v() == v());
                        UT_ASSERT_P(D() == D());

                        UT_ASSERT_P(v() > -tol);
                        fpreal newVal = SYSsqrt(SYSmax(0.f, v()));
                        if( newVal < tol  )
                        {
                            return SIM_DerScalar(newVal);
                        }
                        else
                        {
                            return SIM_DerScalar(newVal, D() / (2*newVal));
                        }
                     }

private:
    fpreal	 myV;
    UT_Vector3	 myD;
};

inline  SIM_DerScalar operator+(fpreal lhs, const SIM_DerScalar &rhs);
inline  SIM_DerScalar operator-(fpreal lhs, const SIM_DerScalar &rhs);
inline  SIM_DerScalar operator*(fpreal lhs, const SIM_DerScalar &rhs);
inline  SIM_DerScalar operator/(fpreal lhs, const SIM_DerScalar &rhs);

inline SIM_DerScalar
operator+(fpreal lhs, const SIM_DerScalar &rhs)
{
    return rhs + lhs;
}

inline SIM_DerScalar
operator-(fpreal lhs, const SIM_DerScalar &rhs)
{
    return SIM_DerScalar(lhs - rhs.v(), -rhs.D());
}

inline SIM_DerScalar
operator*(fpreal lhs, const SIM_DerScalar &rhs)
{
    return rhs * lhs;
}

inline SIM_DerScalar
operator/(fpreal lhs, const SIM_DerScalar &rhs)
{
    return lhs * rhs.inverse();
}
#endif
