/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_MathCbrt.h (SYS Library, C++)
 *
 * COMMENTS:	This is taken out of SYS_Math.h to avoid including > 1200 boost
 *              header files.
 */

#ifndef __SYS_MATHCBRT_H_INCLUDED__
#define __SYS_MATHCBRT_H_INCLUDED__

#include "SYS_API.h"

#include "SYS_Types.h"
#include <hboost/math/special_functions/cbrt.hpp>
#include <hboost/math/policies/policy.hpp>

// This policy is used instead of the default error-handling policy for
// the cube root function SYScbrt.
// This is needed to ensure that no exceptions are thrown.
namespace SYS_MathCBRT
{
    typedef hboost::math::policies::policy<
        hboost::math::policies::domain_error<hboost::math::policies::ignore_error>,
	hboost::math::policies::pole_error<hboost::math::policies::ignore_error>,
	hboost::math::policies::overflow_error<hboost::math::policies::ignore_error>,
	hboost::math::policies::underflow_error<hboost::math::policies::ignore_error>,
	hboost::math::policies::denorm_error<hboost::math::policies::ignore_error>,
	hboost::math::policies::rounding_error<hboost::math::policies::ignore_error>,
	hboost::math::policies::evaluation_error<hboost::math::policies::ignore_error>,
	hboost::math::policies::indeterminate_result_error<hboost::math::policies::ignore_error>
    > policy;
}

// Use the same cbrt() implementation on all platforms, and never use
// pow(x,1.0/3.0)!
static inline fpreal32 SYScbrt(fpreal32 a)
    { return hboost::math::cbrt(a, SYS_MathCBRT::policy()); }
static inline fpreal64 SYScbrt(fpreal64 a)
    { return hboost::math::cbrt(a, SYS_MathCBRT::policy()); }
static inline fpreal64 SYScbrt(int32 a)
    { return hboost::math::cbrt(fpreal64(a), SYS_MathCBRT::policy()); }
static inline fpreal64 SYScbrt(int64 a)
    { return hboost::math::cbrt(fpreal64(a), SYS_MathCBRT::policy()); }


#endif // __SYS_MATHCBRT_H_INCLUDED__
