
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_HAS_NOTHROW_DESTRUCTOR_HPP_INCLUDED
#define HBOOST_TT_HAS_NOTHROW_DESTRUCTOR_HPP_INCLUDED

#include <hboost/type_traits/has_trivial_destructor.hpp>

#if !defined(HBOOST_NO_CXX11_NOEXCEPT) && !defined(__SUNPRO_CC) && !defined(HBOOST_MSVC)

#include <hboost/type_traits/declval.hpp>
#include <hboost/type_traits/is_destructible.hpp>

namespace hboost{

   namespace detail{

      template <class T, bool b>
      struct has_nothrow_destructor_imp : public hboost::integral_constant<bool, false>{};
      template <class T>
      struct has_nothrow_destructor_imp<T, true> : public hboost::integral_constant<bool, noexcept(hboost::declval<T*&>()->~T())>{};

   }

   template <class T> struct has_nothrow_destructor : public detail::has_nothrow_destructor_imp<T, hboost::is_destructible<T>::value>{};
   template <class T, std::size_t N> struct has_nothrow_destructor<T[N]> : public has_nothrow_destructor<T>{};
   template <class T> struct has_nothrow_destructor<T&> : public integral_constant<bool, false>{};
#if !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) 
   template <class T> struct has_nothrow_destructor<T&&> : public integral_constant<bool, false>{};
#endif
}
#else

namespace hboost {

template <class T> struct has_nothrow_destructor : public ::hboost::has_trivial_destructor<T> {};

} // namespace hboost

#endif

#endif // HBOOST_TT_HAS_NOTHROW_DESTRUCTOR_HPP_INCLUDED
