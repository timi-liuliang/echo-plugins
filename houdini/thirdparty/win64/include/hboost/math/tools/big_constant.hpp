
//  Copyright (c) 2011 John Maddock
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_MATH_TOOLS_BIG_CONSTANT_HPP
#define HBOOST_MATH_TOOLS_BIG_CONSTANT_HPP

#include <hboost/math/tools/config.hpp>
#ifndef HBOOST_MATH_NO_LEXICAL_CAST
#include <hboost/lexical_cast.hpp>
#endif
#include <hboost/type_traits/is_convertible.hpp>

namespace hboost{ namespace math{ 

namespace tools{

template <class T>
struct numeric_traits : public std::numeric_limits< T > {};

#ifdef HBOOST_MATH_USE_FLOAT128
typedef __float128 largest_float;
#define HBOOST_MATH_LARGEST_FLOAT_C(x) x##Q
template <>
struct numeric_traits<__float128>
{
   static const int digits = 113;
   static const int digits10 = 33;
   static const int max_exponent = 16384;
   static const bool is_specialized = true;
};
#else
typedef long double largest_float;
#define HBOOST_MATH_LARGEST_FLOAT_C(x) x##L
#endif

template <class T>
inline HBOOST_CONSTEXPR_OR_CONST T make_big_value(largest_float v, const char*, mpl::true_ const&, mpl::false_ const&) HBOOST_MATH_NOEXCEPT(T)
{
   return static_cast<T>(v);
}
template <class T>
inline HBOOST_CONSTEXPR_OR_CONST T make_big_value(largest_float v, const char*, mpl::true_ const&, mpl::true_ const&) HBOOST_MATH_NOEXCEPT(T)
{
   return static_cast<T>(v);
}
#ifndef HBOOST_MATH_NO_LEXICAL_CAST
template <class T>
inline T make_big_value(largest_float, const char* s, mpl::false_ const&, mpl::false_ const&)
{
   return hboost::lexical_cast<T>(s);
}
#endif
template <class T>
inline HBOOST_MATH_CONSTEXPR const char* make_big_value(largest_float, const char* s, mpl::false_ const&, mpl::true_ const&) HBOOST_MATH_NOEXCEPT(T)
{
   return s;
}

//
// For constants which might fit in a long double (if it's big enough):
//
#define HBOOST_MATH_BIG_CONSTANT(T, D, x)\
   hboost::math::tools::make_big_value<T>(\
      HBOOST_MATH_LARGEST_FLOAT_C(x), \
      HBOOST_STRINGIZE(x), \
      mpl::bool_< (is_convertible<hboost::math::tools::largest_float, T>::value) && \
      ((D <= hboost::math::tools::numeric_traits<hboost::math::tools::largest_float>::digits) \
          || is_floating_point<T>::value \
          || (hboost::math::tools::numeric_traits<T>::is_specialized && \
          (hboost::math::tools::numeric_traits<T>::digits10 <= hboost::math::tools::numeric_traits<hboost::math::tools::largest_float>::digits10))) >(), \
      hboost::is_convertible<const char*, T>())
//
// For constants too huge for any conceivable long double (and which generate compiler errors if we try and declare them as such):
//
#define HBOOST_MATH_HUGE_CONSTANT(T, D, x)\
   hboost::math::tools::make_big_value<T>(0.0L, HBOOST_STRINGIZE(x), \
   mpl::bool_<is_floating_point<T>::value || (hboost::math::tools::numeric_traits<T>::is_specialized && hboost::math::tools::numeric_traits<T>::max_exponent <= hboost::math::tools::numeric_traits<hboost::math::tools::largest_float>::max_exponent && hboost::math::tools::numeric_traits<T>::digits <= hboost::math::tools::numeric_traits<hboost::math::tools::largest_float>::digits)>(), \
   hboost::is_convertible<const char*, T>())

}}} // namespaces

#endif

