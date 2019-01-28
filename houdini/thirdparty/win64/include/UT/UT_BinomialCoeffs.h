/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This class contains a hard-coded matrix of binomial coefficients
 *	needed for spline evaluation. This header is meant to be included
 *	only in C files (notice the "static").
 *
 */

#ifndef __UT_BinomialCoeffs_h__
#define __UT_BinomialCoeffs_h__

#include "UT_Defines.h"

// NB: This is the same as GB_BinomialCoeffs, and should replace it!

			// Current size: 11x11.
static const float	ut_biCoeff[UT_MAXORDER][UT_MAXORDER] = {
			{ 1.0F,  0.0F,  0.0F,   0.0F,   0.0F,   0.0F,   0.0F,   0.0F,  0.0F,  0.0F, 0.0F, },
			{ 1.0F,  1.0F,  0.0F,   0.0F,   0.0F,   0.0F,   0.0F,   0.0F,  0.0F,  0.0F, 0.0F, },
			{ 1.0F,  2.0F,  1.0F,   0.0F,   0.0F,   0.0F,   0.0F,   0.0F,  0.0F,  0.0F, 0.0F, },
			{ 1.0F,  3.0F,  3.0F,   1.0F,   0.0F,   0.0F,   0.0F,   0.0F,  0.0F,  0.0F, 0.0F, },
			{ 1.0F,  4.0F,  6.0F,   4.0F,   1.0F,   0.0F,   0.0F,   0.0F,  0.0F,  0.0F, 0.0F, },
			{ 1.0F,  5.0F, 10.0F,  10.0F,   5.0F,   1.0F,   0.0F,   0.0F,  0.0F,  0.0F, 0.0F, },
			{ 1.0F,  6.0F, 15.0F,  20.0F,  15.0F,   6.0F,   1.0F,   0.0F,  0.0F,  0.0F, 0.0F, },
			{ 1.0F,  7.0F, 21.0F,  35.0F,  35.0F,  21.0F,   7.0F,   1.0F,  0.0F,  0.0F, 0.0F, },
			{ 1.0F,  8.0F, 28.0F,  56.0F,  70.0F,  56.0F,  28.0F,   8.0F,  1.0F,  0.0F, 0.0F, },
			{ 1.0F,  9.0F, 36.0F,  84.0F, 126.0F, 126.0F,  84.0F,  36.0F,  9.0F,  1.0F, 0.0F, },
			{ 1.0F, 10.0F, 45.0F, 120.0F, 210.0F, 252.0F, 210.0F, 120.0F, 45.0F, 10.0F, 1.0F  }};


#endif

