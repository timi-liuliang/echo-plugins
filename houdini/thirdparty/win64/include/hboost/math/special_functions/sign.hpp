//  (C) Copyright John Maddock 2006.
//  (C) Copyright Johan Rade 2006.
//  (C) Copyright Paul A. Bristow 2011 (added changesign).

//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_MATH_TOOLS_SIGN_HPP
#define HBOOST_MATH_TOOLS_SIGN_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <hboost/math/tools/config.hpp>
#include <hboost/math/special_functions/math_fwd.hpp>
#include <hboost/math/special_functions/detail/fp_traits.hpp>

namespace hboost{ namespace math{ 

namespace detail {

  // signbit

#ifdef HBOOST_MATH_USE_STD_FPCLASSIFY
    template<class T> 
    inline int signbit_impl(T x, native_tag const&)
    {
        return (std::signbit)(x);
    }
#endif

    // Generic versions first, note that these do not handle
    // signed zero or NaN.

    template<class T>
    inline int signbit_impl(T x, generic_tag<true> const&)
    {
        return x < 0;
    }

    template<class T> 
    inline int signbit_impl(T x, generic_tag<false> const&)
    {
        return x < 0;
    }

#if defined(__GNUC__) && (LDBL_MANT_DIG == 106)
    //
    // Special handling for GCC's "double double" type, 
    // in this case the sign is the same as the sign we
    // get by casting to double, no overflow/underflow
    // can occur since the exponents are the same magnitude
    // for the two types:
    //
    inline int signbit_impl(long double x, generic_tag<true> const&)
    {
       return (hboost::math::signbit)(static_cast<double>(x));
    }
    inline int signbit_impl(long double x, generic_tag<false> const&)
    {
       return (hboost::math::signbit)(static_cast<double>(x));
    }
#endif

    template<class T>
    inline int signbit_impl(T x, ieee_copy_all_bits_tag const&)
    {
        typedef HBOOST_DEDUCED_TYPENAME fp_traits<T>::type traits;

        HBOOST_DEDUCED_TYPENAME traits::bits a;
        traits::get_bits(x,a);
        return a & traits::sign ? 1 : 0;
    }

    template<class T> 
    inline int signbit_impl(T x, ieee_copy_leading_bits_tag const&)
    {
        typedef HBOOST_DEDUCED_TYPENAME fp_traits<T>::type traits;

        HBOOST_DEDUCED_TYPENAME traits::bits a;
        traits::get_bits(x,a);

        return a & traits::sign ? 1 : 0;
    }

    // Changesign
    
    // Generic versions first, note that these do not handle
    // signed zero or NaN.

    template<class T>
    inline T (changesign_impl)(T x, generic_tag<true> const&)
    {
        return -x;
    }

    template<class T>
    inline T (changesign_impl)(T x, generic_tag<false> const&)
    {
        return -x;
    }
#if defined(__GNUC__) && (LDBL_MANT_DIG == 106)
    //
    // Special handling for GCC's "double double" type, 
    // in this case we need to change the sign of both
    // components of the "double double":
    //
    inline long double (changesign_impl)(long double x, generic_tag<true> const&)
    {
       double* pd = reinterpret_cast<double*>(&x);
       pd[0] = hboost::math::changesign(pd[0]);
       pd[1] = hboost::math::changesign(pd[1]);
       return x;
    }
    inline long double (changesign_impl)(long double x, generic_tag<false> const&)
    {
       double* pd = reinterpret_cast<double*>(&x);
       pd[0] = hboost::math::changesign(pd[0]);
       pd[1] = hboost::math::changesign(pd[1]);
       return x;
    }
#endif

    template<class T>
    inline T changesign_impl(T x, ieee_copy_all_bits_tag const&)
    {
        typedef HBOOST_DEDUCED_TYPENAME fp_traits<T>::sign_change_type traits;

        HBOOST_DEDUCED_TYPENAME traits::bits a;
        traits::get_bits(x,a);
        a ^= traits::sign;
        traits::set_bits(x,a);
        return x;
    }

    template<class T>
    inline T (changesign_impl)(T x, ieee_copy_leading_bits_tag const&)
    {
        typedef HBOOST_DEDUCED_TYPENAME fp_traits<T>::sign_change_type traits;

        HBOOST_DEDUCED_TYPENAME traits::bits a;
        traits::get_bits(x,a);
        a ^= traits::sign;
        traits::set_bits(x,a);
        return x;
    }


}   // namespace detail

template<class T> int (signbit)(T x)
{ 
   typedef typename detail::fp_traits<T>::type traits;
   typedef typename traits::method method;
   // typedef typename hboost::is_floating_point<T>::type fp_tag;
   typedef typename tools::promote_args_permissive<T>::type result_type;
   return detail::signbit_impl(static_cast<result_type>(x), method());
}

template <class T>
inline int sign HBOOST_NO_MACRO_EXPAND(const T& z)
{
   return (z == 0) ? 0 : (hboost::math::signbit)(z) ? -1 : 1;
}

template <class T> typename tools::promote_args_permissive<T>::type (changesign)(const T& x)
{ //!< \brief return unchanged binary pattern of x, except for change of sign bit. 
   typedef typename detail::fp_traits<T>::sign_change_type traits;
   typedef typename traits::method method;
   // typedef typename hboost::is_floating_point<T>::type fp_tag;
   typedef typename tools::promote_args_permissive<T>::type result_type;

   return detail::changesign_impl(static_cast<result_type>(x), method());
}

template <class T, class U>
inline typename tools::promote_args_permissive<T, U>::type 
   copysign HBOOST_NO_MACRO_EXPAND(const T& x, const U& y)
{
   HBOOST_MATH_STD_USING
   typedef typename tools::promote_args_permissive<T, U>::type result_type;
   return (hboost::math::signbit)(static_cast<result_type>(x)) != (hboost::math::signbit)(static_cast<result_type>(y)) 
      ? (hboost::math::changesign)(static_cast<result_type>(x)) : static_cast<result_type>(x);
}

} // namespace math
} // namespace hboost


#endif // HBOOST_MATH_TOOLS_SIGN_HPP


