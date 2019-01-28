/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Erf.h (UT Library, C++)
 *
 * COMMENTS:
 * 	Functions for evaluating erf(), and related functions, in the complex
 * 	plane.
 */

#ifndef __UT_Erf__
#define __UT_Erf__

#include "UT_API.h"

#include <SYS/SYS_Types.h>

#include "UT_Complex.h"


/// Returns the value of Dawson's function:
///
///   F(x) = exp(-x^2) * int(exp(y^2), y, 0, x).
///
/// evaluated in the complex plane.  Accuracy increases as niters increases.
/// Note that niters must be odd.
UT_API UT_Complex	 UTdawson(const UT_Complex &z, int niters = 35);

/// Returns the evaluation of erf(x), where x is real-valued.
UT_API fpreal		 UTerf(fpreal x);

/// Returns the evaluation of erf(z) given a very small number, eps, which
/// controls the accuracy of the result, and an upper bound on the number of
/// iterations allowed, niters.  This function evaluates a continued fraction
/// representation of erf.
UT_API UT_Complex	 UTerf(
				const UT_Complex &z,
				fpreal eps = 1e-15,
				int niters = 1000);

/// Returns the evaluation of erfi(x).
UT_API UT_Complex	 UTerfi(fpreal x);

#endif

