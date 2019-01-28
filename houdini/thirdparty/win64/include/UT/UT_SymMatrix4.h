/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef	__UT_SymMatrix4_h__
#define	__UT_SymMatrix4_h__

#include "UT_API.h"
#include <stdlib.h>
#include <iosfwd>

#include "UT_VectorTypes.h"

/// Symmetric 4x4 Matrices 
template <typename T>
class UT_API UT_SymMatrix4T 
{
public:
    /// Construct uninitialized matrix.
    UT_SymMatrix4T<T>()
    {
    }

    // Default copy constructor is fine.
    //UT_SymMatrix4(const UT_SymMatrix4T<T> &m);

    // Default assignment operator is fine.
    //UT_SymMatrix4	&operator=(const UT_SymMatrix4T<T> &m);

    void		 zero()
			{
			    q00 = 0;	q01 = 0;	q02 = 0;
			    q03 = 0;	q11 = 0;	q12 = 0;
			    q13 = 0;	q22 = 0;	q23 = 0;
			    q33 = 0;
			}

    UT_SymMatrix4T<T>	&operator+=(const UT_SymMatrix4T<T> &m)
			{
			    q00 += m.q00;	q01 += m.q01;	q02 += m.q02;
			    q03 += m.q03;	q11 += m.q11;	q12 += m.q12;
			    q13 += m.q13;	q22 += m.q22;	q23 += m.q23;
			    q33 += m.q33;
			    return *this;
			}

    UT_SymMatrix4T<T>	&operator*=(T scale)
			{
			    q00 *= scale;	q01 *= scale;	q02 *= scale;
			    q03 *= scale;	q11 *= scale;	q12 *= scale;
			    q13 *= scale;	q22 *= scale;	q23 *= scale;
			    q33 *= scale;
			    return *this;
			}

    void		addScaledIdentity(T scale)
			{
			    q00 += scale;	q11 += scale;
			    q22 += scale;	q33 += scale;
			}

    UT_SymMatrix4T<T>	&operator+=(T c)
			{
			    q00 += c;	q01 += c;	q02 += c;
			    q03 += c;	q11 += c;	q12 += c;
			    q13 += c;	q22 += c;	q23 += c;
			    q33 += c;
			    return *this;
			}


    T			getEuclideanNorm()
			{
			    return SYSsqrt(getEuclideanNorm2());
			}
    T			getEuclideanNorm2() const
    			{
			    return q00*q00 + 2*q01*q01 + 2*q02*q02 + 2*q03*q03
				    + q11*q11 + 2*q12*q12 + 2*q13*q13
				    + q22*q22 + 2*q23*q23
				    + q33*q33;
			}

    void		transpose() {}

    // Sets the matrix to v * vT
    void		outerproduct(const UT_Vector4 &v);
    void		outerproduct(fpreal64 a, fpreal64 b, fpreal64 c,
                                     fpreal64 d);

    // Updates: Matrix += v * vT
    void		outerproductUpdate(const UT_Vector4 &v);
    void		outerproductUpdate(fpreal64 a, fpreal64 b, fpreal64 c,
                                           fpreal64 d);

    // Calculates vT * Matrix * v
    T			vQv(const UT_Vector4 &v);
    T			vQv(const UT_Vector3 &v);
    
    template <typename S>
    friend class	UT_Matrix4T;

private:
    T			q00, q01, q02, q03, q11, q12, q13, q22;
    T			q23, q33;
};

// SymMatrix is traditionally doubles.
typedef UT_SymMatrix4T<fpreal64> UT_SymMatrix4;
typedef UT_SymMatrix4T<fpreal32> UT_SymMatrix4F;
typedef UT_SymMatrix4T<fpreal64> UT_SymMatrix4D;
#endif
