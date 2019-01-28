//  Copyright John Maddock 2007.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_MATH_TRUNC_HPP
#define HBOOST_MATH_TRUNC_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <hboost/math/special_functions/math_fwd.hpp>
#include <hboost/math/tools/config.hpp>
#include <hboost/math/policies/error_handling.hpp>
#include <hboost/math/special_functions/fpclassify.hpp>

namespace hboost{ namespace math{ namespace detail{

template <class T, class Policy>
inline typename tools::promote_args<T>::type trunc(const T& v, const Policy& pol, const mpl::false_&)
{
   HBOOST_MATH_STD_USING
   typedef typename tools::promote_args<T>::type result_type;
   if(!(hboost::math::isfinite)(v))
      return policies::raise_rounding_error("hboost::math::trunc<%1%>(%1%)", 0, static_cast<result_type>(v), static_cast<result_type>(v), pol);
   return (v >= 0) ? static_cast<result_type>(floor(v)) : static_cast<result_type>(ceil(v));
}

template <class T, class Policy>
inline typename tools::promote_args<T>::type trunc(const T& v, const Policy&, const mpl::true_&)
{
   return v;
}

}

template <class T, class Policy>
inline typename tools::promote_args<T>::type trunc(const T& v, const Policy& pol)
{
   return detail::trunc(v, pol, mpl::bool_<detail::is_integer_for_rounding<T>::value>());
}
template <class T>
inline typename tools::promote_args<T>::type trunc(const T& v)
{
   return trunc(v, policies::policy<>());
}
//
// The following functions will not compile unless T has an
// implicit convertion to the integer types.  For user-defined
// number types this will likely not be the case.  In that case
// these functions should either be specialized for the UDT in
// question, or else overloads should be placed in the same 
// namespace as the UDT: these will then be found via argument
// dependent lookup.  See our concept archetypes for examples.
//
template <class T, class Policy>
inline int itrunc(const T& v, const Policy& pol)
{
   HBOOST_MATH_STD_USING
   typedef typename tools::promote_args<T>::type result_type;
   result_type r = hboost::math::trunc(v, pol);
   if((r > (std::numeric_limits<int>::max)()) || (r < (std::numeric_limits<int>::min)()))
      return static_cast<int>(policies::raise_rounding_error("hboost::math::itrunc<%1%>(%1%)", 0, static_cast<result_type>(v), 0, pol));
   return static_cast<int>(r);
}
template <class T>
inline int itrunc(const T& v)
{
   return itrunc(v, policies::policy<>());
}

template <class T, class Policy>
inline long ltrunc(const T& v, const Policy& pol)
{
   HBOOST_MATH_STD_USING
   typedef typename tools::promote_args<T>::type result_type;
   result_type r = hboost::math::trunc(v, pol);
   if((r > (std::numeric_limits<long>::max)()) || (r < (std::numeric_limits<long>::min)()))
      return static_cast<long>(policies::raise_rounding_error("hboost::math::ltrunc<%1%>(%1%)", 0, static_cast<result_type>(v), 0L, pol));
   return static_cast<long>(r);
}
template <class T>
inline long ltrunc(const T& v)
{
   return ltrunc(v, policies::policy<>());
}

#ifdef HBOOST_HAS_LONG_LONG

template <class T, class Policy>
inline hboost::long_long_type lltrunc(const T& v, const Policy& pol)
{
   HBOOST_MATH_STD_USING
   typedef typename tools::promote_args<T>::type result_type;
   result_type r = hboost::math::trunc(v, pol);
   if((r > (std::numeric_limits<hboost::long_long_type>::max)()) || (r < (std::numeric_limits<hboost::long_long_type>::min)()))
      return static_cast<hboost::long_long_type>(policies::raise_rounding_error("hboost::math::lltrunc<%1%>(%1%)", 0, v, static_cast<hboost::long_long_type>(0), pol));
   return static_cast<hboost::long_long_type>(r);
}
template <class T>
inline hboost::long_long_type lltrunc(const T& v)
{
   return lltrunc(v, policies::policy<>());
}

#endif

}} // namespaces

#endif // HBOOST_MATH_TRUNC_HPP