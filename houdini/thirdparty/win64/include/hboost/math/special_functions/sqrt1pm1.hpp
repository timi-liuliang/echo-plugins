//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_MATH_SQRT1PM1
#define HBOOST_MATH_SQRT1PM1

#ifdef _MSC_VER
#pragma once
#endif

#include <hboost/math/special_functions/math_fwd.hpp>
#include <hboost/math/special_functions/log1p.hpp>
#include <hboost/math/special_functions/expm1.hpp>

//
// This algorithm computes sqrt(1+x)-1 for small x:
//

namespace hboost{ namespace math{

template <class T, class Policy>
inline typename tools::promote_args<T>::type sqrt1pm1(const T& val, const Policy& pol)
{
   typedef typename tools::promote_args<T>::type result_type;
   HBOOST_MATH_STD_USING

   if(fabs(result_type(val)) > 0.75)
      return sqrt(1 + result_type(val)) - 1;
   return hboost::math::expm1(hboost::math::log1p(val, pol) / 2, pol);
}

template <class T>
inline typename tools::promote_args<T>::type sqrt1pm1(const T& val)
{
   return sqrt1pm1(val, policies::policy<>());
}

} // namespace math
} // namespace hboost

#endif // HBOOST_MATH_SQRT1PM1





