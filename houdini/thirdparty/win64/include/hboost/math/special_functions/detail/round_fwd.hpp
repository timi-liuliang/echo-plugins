// Copyright John Maddock 2008.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_MATH_SPECIAL_ROUND_FWD_HPP
#define HBOOST_MATH_SPECIAL_ROUND_FWD_HPP

#include <hboost/config.hpp>
#include <hboost/math/tools/promotion.hpp>

#ifdef _MSC_VER
#pragma once
#endif

namespace hboost
{
   namespace math
   { 

   template <class T, class Policy>
   typename tools::promote_args<T>::type trunc(const T& v, const Policy& pol);
   template <class T>
   typename tools::promote_args<T>::type trunc(const T& v);
   template <class T, class Policy>
   int itrunc(const T& v, const Policy& pol);
   template <class T>
   int itrunc(const T& v);
   template <class T, class Policy>
   long ltrunc(const T& v, const Policy& pol);
   template <class T>
   long ltrunc(const T& v);
#ifdef HBOOST_HAS_LONG_LONG
   template <class T, class Policy>
   hboost::long_long_type lltrunc(const T& v, const Policy& pol);
   template <class T>
   hboost::long_long_type lltrunc(const T& v);
#endif
   template <class T, class Policy>
   typename tools::promote_args<T>::type round(const T& v, const Policy& pol);
   template <class T>
   typename tools::promote_args<T>::type round(const T& v);
   template <class T, class Policy>
   int iround(const T& v, const Policy& pol);
   template <class T>
   int iround(const T& v);
   template <class T, class Policy>
   long lround(const T& v, const Policy& pol);
   template <class T>
   long lround(const T& v);
#ifdef HBOOST_HAS_LONG_LONG
   template <class T, class Policy>
   hboost::long_long_type llround(const T& v, const Policy& pol);
   template <class T>
   hboost::long_long_type llround(const T& v);
#endif
   template <class T, class Policy>
   T modf(const T& v, T* ipart, const Policy& pol);
   template <class T>
   T modf(const T& v, T* ipart);
   template <class T, class Policy>
   T modf(const T& v, int* ipart, const Policy& pol);
   template <class T>
   T modf(const T& v, int* ipart);
   template <class T, class Policy>
   T modf(const T& v, long* ipart, const Policy& pol);
   template <class T>
   T modf(const T& v, long* ipart);
#ifdef HBOOST_HAS_LONG_LONG
   template <class T, class Policy>
   T modf(const T& v, hboost::long_long_type* ipart, const Policy& pol);
   template <class T>
   T modf(const T& v, hboost::long_long_type* ipart);
#endif

   }
}

#undef HBOOST_MATH_STD_USING
#define HBOOST_MATH_STD_USING HBOOST_MATH_STD_USING_CORE\
   using hboost::math::round;\
   using hboost::math::iround;\
   using hboost::math::lround;\
   using hboost::math::trunc;\
   using hboost::math::itrunc;\
   using hboost::math::ltrunc;\
   using hboost::math::modf;


#endif // HBOOST_MATH_SPECIAL_ROUND_FWD_HPP

