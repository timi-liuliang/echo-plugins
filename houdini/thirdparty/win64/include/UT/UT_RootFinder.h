/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Root finding (C++)
 *
 * COMMENTS:
 *
 */

#ifndef _UT_RootFinder_h_
#define _UT_RootFinder_h_

#include "UT_API.h"

#include "UT_MatrixFwd.h"
#include "UT_VectorFwd.h"
#include "UT_VectorTypes.h"
#include "UT_Complex.h"
#include <SYS/SYS_Types.h>

template <typename T>
class UT_API UT_RootFinderT
{
public:
    // 1D root finders:
    //

    // Finds the real roots of a polynomial.  The coefficients of the
    // polynomial are specified in 'cf'.  'cf(i)' should contain the
    // coefficient for x^i.
    //
    // The roots are appended to 'roots'
    static void laguerre(const UT_ValArray<T> &cf, UT_ValArray<T> &roots,
			 int maxIters = 200);

    // Returns number of real roots.  The roots are found in v0 & v1.
    //	This is quite a stable solver handling many bad cases.
    //
    //	Note: if a, b and c are all zero, this function will return
    //	zero roots, even though the equation is zero for all values of v.
    static int	quadratic(T a, T b, T c, T &v0, T &v1);

    // Returns number of real roots but calculates all of the roots,
    // the real returned in r_roots and imaginary in i_roots
    static int	quadratic(T a, T b, T c,
			  UT_Vector2T<T> &r_roots, UT_Vector2T<T> &i_roots);

    // Returns number of real roots.  The roots are found in v0, v1, & v2.
    // a*v^3 + b*v^2 + c*v + d = 0
    //
    //	Note: if a, b, c and d are all zero, this function will return
    //	zero roots, even though the equation is zero for all values of v.
    static int  cubic(T a, T b, T c, T d, T &v0, T &v1, T &v2);

    // Returns number of real roots.  Finds all the roots, however,
    // real parts in r_roots and imaginary in i_roots
    //
    //	Note: if a, b, c and d are all zero, this function will return
    //	zero roots, even though the equation is zero for all values of v.
    static int  cubic(T a, T b, T c, T d,
		      UT_Vector3T<T> &r_roots, UT_Vector3T<T> &i_roots);

    // Returns number of real roots.  The user must provide an interval [x1,x2]
    // which is to contain the roots they're interested in.  It is possible that
    // roots virtually on the end points might be missed, so increase your
    // interval to compensate.
    static int  cubic(T a, T b, T c, T d, T x1, T x2, T r[3]);

    // Returns number of real roots.  The user must provide an interval [x1,x2]
    // which is to contain the roots they're interested in.  It is possible that
    // roots virtually on the end points might be missed, so increase your
    // interval to compensate.
    static int  quartic(T a, T b, T c, T d, T e, T x1, T x2, T r[4]);


    // No derivative is needed. Root bracketed by [x1,x2].
    static int brent(T (*func)(T x, void *data), void *data,
	      T x1, T x2, T tol, T &result, int maxIter=200);

    // Derivative is needed. Root bracketed by [x1,x2].
    static int newton(void (*func)(T x, T &val, T &der, void *data), void *data,
               T x1, T x2, T tol, T &result, int maxIter=200);

    // Multi-dimensional root finders:
    //

    // Jacobian is needed. Specify initial guess in result.
    template <typename S>
    static int newton(void (*func)(const UT_VectorT<S> &x, UT_VectorT<S> &val, UT_MatrixT<T> &jacobi, void *data),
	       void *data,
	       S tolx, S tolf,
	       UT_VectorT<S> &result, int maxIter=200);

    // Jacobian is needed. Specify initial guess in result.
    // [x1,x2] specify the boundary that the func is defined within.
    template <typename S>
    static int newton(void (*func)(const UT_VectorT<S> &x, UT_VectorT<S> &val, UT_MatrixT<T> &jacobi, void *data),
	       void *data,
	       const UT_VectorT<S> &x1, const UT_VectorT<S> &x2,
	       S tolx, S tolf,
	       UT_VectorT<S> &result, int maxIter=200);
};

typedef UT_RootFinderT<fpreal>   UT_RootFinderR;
typedef UT_RootFinderT<fpreal32> UT_RootFinderF;
typedef UT_RootFinderT<fpreal64> UT_RootFinderD;
typedef UT_RootFinderT<float>    UT_RootFinder;

#endif
