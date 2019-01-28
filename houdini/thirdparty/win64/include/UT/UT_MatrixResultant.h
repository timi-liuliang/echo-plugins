/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Matrix Resultant (C++)
 *
 * COMMENTS: Calculates resultant matrices from descriptions
 * 	of the original polynomials.
 *
 */

#ifndef	__UT_MatrixResultant_H__
#define	__UT_MatrixResultant_H__

#include "UT_API.h"
#include "UT_Assert.h"

#include "UT_Vector.h"
#include "UT_Matrix.h"

class UT_API UT_MatrixResultant 
{
public:
    // Finds dixon's resultant for tensor product polynomials,
    // resulting degree is 2*m*n.
    // A, B, and C are matrices vdeg+1 by udeg+1, starting at 1,1
    void DixonTP3(UT_MatrixF &coeff, const UT_MatrixF &A,
		const UT_MatrixF &B, const UT_MatrixF &C) const;

    // Calculates Cayley's form of Bezout's resultant for two 
    // univariate polynomials.
    // F & G are vectors from 0 to deg (inclusive).
    void Cayley2(UT_MatrixF &coeff, const UT_Vector &F,
		    const UT_Vector &G) const;
};

#endif
