/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This is meant to be included by UT_MatrixSolver.h and includes
 *	the template implementations needed by external code.
 */

#pragma once

#ifndef __UT_MATRIXSOLVERIMPL_H_INCLUDED__
#define __UT_MATRIXSOLVERIMPL_H_INCLUDED__

#include "UT_Interrupt.h"


template <typename T>
template <typename AMult, typename AtASolve>
T
UT_MatrixIterSolverT<T>::PCGLS(const AMult &amult, const AtASolve &atasolve,
			       int rows, int cols,
			       UT_VectorT<T> &x, const UT_VectorT<T> &b,
			       fpreal tol, int norm_type, int max_iter)
{
    UT_AutoInterrupt boss("Solving PCGLS");

    fpreal64 alpha, abs_new, residual_norm2, rhs_norm2;

    UT_VectorT<T> r(0, rows - 1), tmp(0, rows - 1);
    UT_VectorT<T> p(0, cols - 1), s(0, cols - 1), z(0, cols - 1);


    // s is the normal residual.
    amult(b, s, 1);		// s = A^t * b

    if (norm_type == 2)
	rhs_norm2 = dot(s, s);
    else
    {
	rhs_norm2 = s.norm(norm_type);
	rhs_norm2 *= rhs_norm2;
    }

    if (rhs_norm2 == 0.0)
    {
	x.zero();
	return 0;
    }

    fpreal64 threshold = tol * tol * rhs_norm2;

    if (max_iter < 0)
	max_iter = 2 * (rows + cols);

    amult(x, r, 0);      	// Residual r = b - Ax
    r.negPlus(b);

    amult(r, s, 1);		// s = A^t * r

    if (norm_type == 2)
	residual_norm2 = dot(s, s);
    else
    {
	residual_norm2 = s.norm(norm_type);
	residual_norm2 *= residual_norm2;
    }

    if (residual_norm2 < threshold)
    {
	return SYSsqrt(residual_norm2 / rhs_norm2);
    }

    atasolve(s, p);
    abs_new = dot(s, p);
    p = p;

    for (exint iter = 0; iter < max_iter; ++iter)
    {
	if (!(iter % 256) && boss.wasInterrupted())
	    break;

	amult(p, tmp, 0);
	alpha = abs_new / dot(tmp, tmp);
	x.addScaledVec(alpha, p);
	r.addScaledVec(-alpha, tmp);
	amult(r, s, 1);			// s = A^t * r

	if (norm_type == 2)
	    residual_norm2 = dot(s, s);
	else
	{
	    residual_norm2 = s.norm(norm_type);
	    residual_norm2 *= residual_norm2;
	}
	if (residual_norm2 < threshold)
	    break;

	atasolve(s, z);
	fpreal64 absOld = abs_new;
	abs_new = dot(s, z);
	p.scaleAddVec(abs_new / absOld, z);
    }

    return residual_norm2 / rhs_norm2;
}


#endif // __UT_MATRIXSOLVERIMPL_H_INCLUDED__
