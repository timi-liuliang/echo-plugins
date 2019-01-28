/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_DualQuaternion library (C++)
 *
 * COMMENTS:	This class implements dual quaternions.
 *
 *
 */


#ifndef __UT_DualQuaternion_h__
#define __UT_DualQuaternion_h__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_Matrix3.h"
#include "UT_Matrix4.h"
#include "UT_Quaternion.h"
#include "UT_Vector3.h"
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_TypeDecorate.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>
#include <limits>


///
/// Dual quaternion class
///
/// @note This class only supports the subspace of dual quaternions for use
/// as rigid transforms. As such, its real and dual components must be
/// orthogonal.
///
template <typename T>
class UT_DualQuaternionT
{
public:
    /// Trivial constructor. If called, it initializes to all zeroes.
    SYS_FORCE_INLINE
    UT_DualQuaternionT() = default;

    /// Construct from orthogonal real and dual parts
    inline
    UT_DualQuaternionT(const UT_QuaternionT<T> &real,
		       const UT_QuaternionT<T> &dual);

    /// Construct from components of orthogonal real and dual parts
    inline
    UT_DualQuaternionT(T real_qx, T real_qy, T real_qz, T real_qw,
 		       T dual_qx, T dual_qy, T dual_qz, T dual_qw);

    /// Construct from rotation unit quaternion and translation vector
    inline
    UT_DualQuaternionT(const UT_QuaternionT<T> &r, const UT_Vector3T<T> &t);

    /// Construct from a transform matrix 
    inline
    UT_DualQuaternionT(const UT_Matrix4T<T> &xform);

    /// Default copy constructor
    SYS_FORCE_INLINE
    UT_DualQuaternionT(const UT_DualQuaternionT &dq) = default;

    /// Default assignment
    SYS_FORCE_INLINE
    UT_DualQuaternionT<T> &operator=(const UT_DualQuaternionT& v) = default;

    /// Assign from UT_Matrix4
    inline void updateFromXform(const UT_Matrix4T<T> &xform);

    /// Assign from UT_Matrix3
    inline void updateFromXform(const UT_Matrix3T<T> &xform);

    inline void updateFromRotTransPair(const UT_QuaternionT<T> &r,
				       const UT_Vector3T<T> &t);
    inline void updateFromRotTransPair(const UT_QuaternionT<T> &r,
				       T tx, T ty, T tz);

    /////////////////////////////////
    ///  Methods
    /////////////////////////////////

    /// Length squared
    inline T length2() const;

    /// Length
    inline T length() const;

    /// Normalizes the dual quaternion to have length of 1
    inline void normalize();

    /// Changes dual quaternion into its conjugate
    inline void conjugate();

    /// Invert this dual quaternion. Does not exist if getReal() is zero.
    inline void invert();

    /// Invert this dual quaternion, assuming that its already normalized.
    /// Does not exist if getReal() is zero.
    inline void invertNormalized();

    /// Change dual quaternion into an 0 rotation and 0 translation components
    inline void identity();

    /// Returns a vector that is transformed by the dual quaternion
    /// Requires this is normalized
    inline void transform(const UT_Vector3T<T> &vec, UT_Vector3T<T> &dst) const;
    
    /// Returns a vector that has only the rotation component applied
    /// Requires this is normalized 
    inline void rotate(const UT_Vector3T<T> &vec, UT_Vector3T<T> &dst) const;
    
    /// Returns a xform that is represented by the dual quaternion
    /// Requires this is normalized
    inline UT_Matrix4T<T> convertToXform() const;


    //////////////////////////////////////
    /// Operators 
    //////////////////////////////////////

    inline UT_DualQuaternionT<T> &operator*=(T scalar);
    inline UT_DualQuaternionT<T> &operator*=(const UT_DualQuaternionT<T> &dq);
    inline UT_DualQuaternionT<T> &operator+=(const UT_DualQuaternionT<T> &dq);
    inline UT_DualQuaternionT<T> &operator-=(const UT_DualQuaternionT<T> &dq);
    inline bool operator==(const UT_DualQuaternionT<T> &dq) const;
    inline bool operator!=(const UT_DualQuaternionT<T> &dq) const;
    // todo: hash

    /// Like operator==() but using a tolerance
    inline bool isEqual(const UT_DualQuaternionT<T> &quat,
			T tol = T(SYS_FTOLERANCE)) const;

    friend std::ostream &
    operator<<(std::ostream &os, const UT_DualQuaternionT<T> &dq)
    {
	dq.outTo(os); 
	return os;
    }

    //////////////////////////////////
    /// Getters    
    //////////////////////////////////

    const T *	realData() const    { return myReal.data(); }
    T *		realData()	    { return myReal.data(); }

    const T *	dualData() const    { return myDual.data(); }
    T *		dualData()	    { return myDual.data(); }

    const UT_QuaternionT<T> &getReal() const { return myReal; }
    const UT_QuaternionT<T> &getDual() const { return myDual; }
    const UT_QuaternionT<T> &getTranslation() const { return myDual; }
    const UT_QuaternionT<T> &getRotation() const { return myReal; }


protected:
    UT_API void outTo(std::ostream &os) const;

private:

    /// Real or non-dual part of the dual quaternion.
    /// It represents the rotation component of an xform
    UT_QuaternionT<T> myReal;

    /// Dual part of the dual quaternion. 
    /// It represents the translation components
    UT_QuaternionT<T> myDual;

};


///////////////////////////////////////
/// Typedefs
///////////////////////////////////////

typedef UT_DualQuaternionT<fpreal32>	    UT_DualQuaternion;
typedef UT_DualQuaternionT<fpreal>	    UT_DualQuaternionR;
typedef UT_DualQuaternionT<fpreal32>	    UT_DualQuaternionF;
typedef UT_DualQuaternionT<fpreal64>	    UT_DualQuaternionD;

SYS_DECLARE_IS_POD(UT_DualQuaternionF);
SYS_DECLARE_IS_POD(UT_DualQuaternionD);

///////////////////////////////////////
/// Construct implementations
///////////////////////////////////////

template <typename T>
inline
UT_DualQuaternionT<T>::UT_DualQuaternionT(const UT_QuaternionT<T> &real,
					  const UT_QuaternionT<T> &dual)
    : myReal(real)
    , myDual(dual) 
{
}

template <typename T>
inline
UT_DualQuaternionT<T>::UT_DualQuaternionT(
	T real_qx, T real_qy, T real_qz, T real_qw,
	T dual_qx, T dual_qy, T dual_qz, T dual_qw)
    : myReal(real_qx, real_qy, real_qz, real_qw)
    , myDual(dual_qx, dual_qy, dual_qz, dual_qw)
{
}

template <typename T>
inline
UT_DualQuaternionT<T>::UT_DualQuaternionT(
	const UT_QuaternionT<T> &r,
	const UT_Vector3T<T> &t)
{
    updateFromRotTransPair(r, t);
}

template <typename T>
inline
UT_DualQuaternionT<T>::UT_DualQuaternionT(const UT_Matrix4T<T> &xform)
{
    updateFromXform(xform);
}

template <typename T>
inline void
UT_DualQuaternionT<T>::updateFromXform(const UT_Matrix4T<T> &xform)
{
    UT_Matrix3T<T> rotXform;
    xform.extractRotate(rotXform);
    myReal.updateFromRotationMatrix(rotXform);
    
    UT_Vector3T<T> translate;
    xform.getTranslates(translate);

    updateFromRotTransPair(myReal, translate); 
}

template <typename T>
inline void
UT_DualQuaternionT<T>::updateFromXform(const UT_Matrix3T<T> &xform)
{
    myReal.updateFromRotationMatrix(xform);
    updateFromRotTransPair(myReal, 0.0f, 0.0f, 0.0f);
}

template <typename T>
inline void
UT_DualQuaternionT<T>::updateFromRotTransPair(const UT_QuaternionT<T> &r,
					      T tx, T ty, T tz)
{
    T w = -0.5f * ( tx*r.x() + ty*r.y() + tz*r.z() );
    T x =  0.5f * ( r.w()*tx + ty*r.z() - r.y()*tz );
    T y =  0.5f * ( r.w()*ty + tz*r.x() - r.z()*tx );
    T z =  0.5f * ( r.w()*tz + tx*r.y() - r.x()*ty );
    
    myReal = r;
    myDual.assign(x, y, z, w); 
    UT_ASSERT_P(SYSequalZero(dot(myReal, myDual)));
    UT_ASSERT_P(SYSisEqual(myReal.normal(), T(1.0)));
}


///////////////////////////////////////
/// Method implementations
///////////////////////////////////////

template <typename T>
inline void 
UT_DualQuaternionT<T>::transform(
	const UT_Vector3T<T> &vec, UT_Vector3T<T> &dst) const 
{
    // The actual equation is p' = qpq* where p' and p are quaternions that
    // represent the points while q and q* are conjugate quaternions of each
    // other q = qt x qr, which is the operation rotation followed by
    // translation

    UT_Vector3T<T> real_vec( myReal.x(), myReal.y(), myReal.z() );
    UT_Vector3T<T> dual_vec( myDual.x(), myDual.y(), myDual.z() );

    UT_Vector3T<T> cross_vec = cross(real_vec, dual_vec);
    UT_Vector3T<T>
	t = (dual_vec*myReal.w() - real_vec*myDual.w() + cross_vec) * T(2.0);

    dst = myReal.rotate(vec) + t;
}

template <typename T>
inline void 
UT_DualQuaternionT<T>::rotate(
	const UT_Vector3T<T> &vec, UT_Vector3T<T> &dst) const
{
    UT_ASSERT(SYSisEqual(myReal.normal(), T(1.0)));
    UT_Quaternion rot_quat = myReal;
    dst = rot_quat.rotate(vec);
}

template <typename T>
inline UT_Matrix4T<T> 
UT_DualQuaternionT<T>::convertToXform() const
{
    UT_Matrix3T<T> rotMat;
    myReal.getRotationMatrix(rotMat);
   
    UT_Matrix4T<T> xform;
    xform = rotMat;

    UT_QuaternionT<T> real_conjugate = myReal;
    real_conjugate.conjugate();
    UT_QuaternionT<T> t_quat = myDual * real_conjugate; 
    t_quat *= T(2.0);

    xform.setTranslates(UT_Vector3T<T>(t_quat.x(), t_quat.y(), t_quat.z()));

    return xform; 
}

template <typename T>
inline void
UT_DualQuaternionT<T>::updateFromRotTransPair(const UT_QuaternionT<T> &r,
					      const UT_Vector3T<T> &t)
{
    updateFromRotTransPair(r, t.x(), t.y(), t.z());    
}

template <typename T>
inline T
UT_DualQuaternionT<T>::length2() const
{
    UT_ASSERT_P(SYSequalZero(dot(myReal, myDual)));
    return myReal.normal();
}

template <typename T>
inline T
UT_DualQuaternionT<T>::length() const
{
    return SYSsqrt(length2());
}

template <typename T>
inline void 
UT_DualQuaternionT<T>::normalize()
{
    // This assertion doesn't seem to hold true in general when we're doing a
    // weighted average of dual quaternions. This kind of brings up a problem
    // as to whether this is correct thing at all. But the operations here
    // follow what is done in "Skinning with Dual Quaternions" by Ladislav et
    // al. They give a rationale why it's ok to do the normalize() this way in
    // the paper nut I'm not sure I buy it.
    //UT_ASSERT_P(SYSequalZero(dot(myReal, myDual)));

    T norm = myReal.normal();
    if(norm > std::numeric_limits<T>::min() &&
       norm != 1.0)
    {
	norm = SYSsqrt(norm);
	myReal /= norm;
	myDual /= norm;
    }
}

template <typename T>
inline void 
UT_DualQuaternionT<T>::conjugate()
{
    myReal.conjugate();
    myDual.conjugate();
}

template <typename T>
inline void
UT_DualQuaternionT<T>::invert()
{
    UT_ASSERT_P(!SYSequalZero(myReal.normal()));
    UT_ASSERT_P(SYSequalZero(dot(myReal, myDual)));
    myReal.invert();
    myDual = (myReal * myDual * myReal) * T(-1.0);
}

template <typename T>
inline void
UT_DualQuaternionT<T>::invertNormalized()
{
    UT_ASSERT_P(SYSisEqual(myReal.normal(), 1.0));
    UT_ASSERT_P(SYSequalZero(dot(myReal, myDual)));
    myReal.conjugate();
    myDual = (myReal * myDual * myReal) * T(-1.0);
}

template <typename T>
inline void 
UT_DualQuaternionT<T>::identity()
{
    myReal.identity();
    myDual.assign(0, 0, 0, 0);
}


///////////////////////////////////////
/// Operator implementations 
///////////////////////////////////////

template <typename T>
inline UT_DualQuaternionT<T> &
UT_DualQuaternionT<T>::operator+=(const UT_DualQuaternionT<T> &dq)
{
    myDual += dq.getDual();
    myReal += dq.getReal();
    return (*this);
}

template <typename T>
inline UT_DualQuaternionT<T> &
UT_DualQuaternionT<T>::operator*=(T scalar)
{
    myDual *= scalar;
    myReal *= scalar;
    return (*this);
}

template <typename T>
inline UT_DualQuaternionT<T> &
UT_DualQuaternionT<T>::operator*=(const UT_DualQuaternionT<T> &dq)
{
    myDual = myReal * dq.getDual() + myDual * dq.getReal();
    myReal *= dq.getReal();
    return (*this);
}

template <typename T>
inline UT_DualQuaternionT<T> &
UT_DualQuaternionT<T>::operator-=(const UT_DualQuaternionT<T> &dq)
{
    myDual = myDual - dq.getDual();
    myReal = myReal - dq.getReal();
    return (*this);
}

template <typename T>
inline bool
UT_DualQuaternionT<T>::operator==(const UT_DualQuaternionT<T> &dq) const
{
    return ( myDual == dq.getDual() ) && 
	   ( myReal == dq.getReal() );    
}

template <typename T>
inline bool
UT_DualQuaternionT<T>::isEqual(const UT_DualQuaternionT<T> &dq, T tol) const
{
    return     SYSisEqual(myDual(0), dq.getDual()(0), tol)
	    && SYSisEqual(myDual(1), dq.getDual()(1), tol)
	    && SYSisEqual(myDual(2), dq.getDual()(2), tol)
	    && SYSisEqual(myDual(3), dq.getDual()(3), tol)
	    && SYSisEqual(myReal(0), dq.getReal()(0), tol)
	    && SYSisEqual(myReal(1), dq.getReal()(1), tol)
	    && SYSisEqual(myReal(2), dq.getReal()(2), tol)
	    && SYSisEqual(myReal(3), dq.getReal()(3), tol);
}

template <typename T>
inline bool
UT_DualQuaternionT<T>::operator!=(const UT_DualQuaternionT<T> &dq) const
{
    return !(*this == dq);
}

///////////////////////////////////////
/// External operators
///////////////////////////////////////

template <typename T>
inline UT_DualQuaternionT<T> 
operator-(const UT_DualQuaternionT<T> &dq1, const UT_DualQuaternionT<T> &dq2)
{
    return UT_DualQuaternionT<T>(dq1.getReal()-dq2.getReal(),
				 dq1.getDual()-dq2.getDual());
}

template <typename T>
inline UT_DualQuaternionT<T>
operator+(const UT_DualQuaternionT<T> &dq1, const UT_DualQuaternionT<T> &dq2)
{
    return UT_DualQuaternionT<T>(dq1.getReal()+dq2.getReal(),
				 dq1.getDual()+dq2.getDual());
}

template <typename T>
inline UT_DualQuaternionT<T>
operator*(const UT_DualQuaternionT<T> &dq1, const T scalar)
{
    return UT_DualQuaternionT<T>(dq1.getReal()*scalar,
				 dq1.getDual()*scalar);
}

template <typename T>
inline UT_DualQuaternionT<T>
operator*(const UT_DualQuaternionT<T> &dq1, const UT_DualQuaternionT<T> &dq2)
{
    return UT_DualQuaternionT<T>(
	dq1.getReal() * dq2.getReal(),
	dq1.getReal() * dq2.getDual() + dq1.getDual() * dq2.getReal());
}

template<> UT_API
void UT_DualQuaternionT<fpreal32>::outTo(std::ostream &os) const;
template<> UT_API
void UT_DualQuaternionT<fpreal64>::outTo(std::ostream &os) const;

#endif // __UT_DualQuaternion_h__
