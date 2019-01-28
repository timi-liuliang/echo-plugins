
//  (C) Copyright John Maddock 2015.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_DESTRUCTIBLE_HPP_INCLUDED
#define HBOOST_TT_IS_DESTRUCTIBLE_HPP_INCLUDED

#include <hboost/type_traits/integral_constant.hpp>
#include <hboost/detail/workaround.hpp>

#if !defined(HBOOST_NO_CXX11_DECLTYPE) && !HBOOST_WORKAROUND(HBOOST_MSVC, < 1800)

#include <hboost/type_traits/detail/yes_no_type.hpp>
#include <hboost/type_traits/declval.hpp>

namespace hboost{

   namespace detail{

      struct is_destructible_imp
      {
         template<typename T, typename = decltype(hboost::declval<T&>().~T())>
         static hboost::type_traits::yes_type test(int);
         template<typename>
         static hboost::type_traits::no_type test(...);
      };

   }

   template <class T> struct is_destructible : public integral_constant<bool, sizeof(detail::is_destructible_imp::test<T>(0)) == sizeof(hboost::type_traits::yes_type)>{};

#else

#include <hboost/type_traits/is_pod.hpp>
#include <hboost/type_traits/is_class.hpp>

namespace hboost{

   // We don't know how to implement this:
   template <class T> struct is_destructible : public integral_constant<bool, is_pod<T>::value || is_class<T>::value>{};
#endif

   template <> struct is_destructible<void> : public false_type{};
   template <> struct is_destructible<void const> : public false_type{};
   template <> struct is_destructible<void volatile> : public false_type{};
   template <> struct is_destructible<void const volatile> : public false_type{};
   template <class T> struct is_destructible<T&> : public is_destructible<T>{};
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
   template <class T> struct is_destructible<T&&> : public is_destructible<T>{};
#endif
   template <class T, std::size_t N> struct is_destructible<T[N]> : public is_destructible<T>{};
   template <class T> struct is_destructible<T[]> : public is_destructible<T>{};

} // namespace hboost

#endif // HBOOST_TT_IS_DESTRUCTIBLE_HPP_INCLUDED
