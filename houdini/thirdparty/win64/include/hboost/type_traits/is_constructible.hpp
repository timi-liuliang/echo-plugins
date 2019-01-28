
//  (C) Copyright John Maddock 2015.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_CONSTRUCTIBLE_HPP_INCLUDED
#define HBOOST_TT_IS_CONSTRUCTIBLE_HPP_INCLUDED

#include <hboost/type_traits/integral_constant.hpp>
#include <hboost/detail/workaround.hpp>

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(HBOOST_NO_CXX11_DECLTYPE) && !HBOOST_WORKAROUND(HBOOST_MSVC, < 1800) && !HBOOST_WORKAROUND(HBOOST_GCC_VERSION, < 40500)

#include <hboost/type_traits/is_destructible.hpp>
#include <hboost/type_traits/is_default_constructible.hpp>
#include <hboost/type_traits/detail/yes_no_type.hpp>
#include <hboost/type_traits/declval.hpp>

namespace hboost{

   namespace detail{

      struct is_constructible_imp
      {
         template<typename T, typename ...TheArgs, typename = decltype(T(hboost::declval<TheArgs>()...))>
         static hboost::type_traits::yes_type test(int);
         template<typename, typename...>
         static hboost::type_traits::no_type test(...);

         template<typename T, typename Arg, typename = decltype(::new T(hboost::declval<Arg>()))>
         static hboost::type_traits::yes_type test1(int);
         template<typename, typename>
         static hboost::type_traits::no_type test1(...);

         template <typename T>
         static hboost::type_traits::yes_type ref_test(T);
         template <typename T>
         static hboost::type_traits::no_type ref_test(...);
      };

   }

   template <class T, class ...Args> struct is_constructible : public integral_constant<bool, sizeof(detail::is_constructible_imp::test<T, Args...>(0)) == sizeof(hboost::type_traits::yes_type)>{};
   template <class T, class Arg> struct is_constructible<T, Arg> : public integral_constant<bool, is_destructible<T>::value && sizeof(detail::is_constructible_imp::test1<T, Arg>(0)) == sizeof(hboost::type_traits::yes_type)>{};
   template <class Ref, class Arg> struct is_constructible<Ref&, Arg> : public integral_constant<bool, sizeof(detail::is_constructible_imp::ref_test<Ref&>(hboost::declval<Arg>())) == sizeof(hboost::type_traits::yes_type)>{};
   template <class Ref, class Arg> struct is_constructible<Ref&&, Arg> : public integral_constant<bool, sizeof(detail::is_constructible_imp::ref_test<Ref&&>(hboost::declval<Arg>())) == sizeof(hboost::type_traits::yes_type)>{};

   template <> struct is_constructible<void> : public false_type{};
   template <> struct is_constructible<void const> : public false_type{};
   template <> struct is_constructible<void const volatile> : public false_type{};
   template <> struct is_constructible<void volatile> : public false_type{};

   template <class T> struct is_constructible<T> : public is_default_constructible<T>{};

#else

#include <hboost/type_traits/is_convertible.hpp>
#include <hboost/type_traits/is_default_constructible.hpp>

namespace hboost{

   // We don't know how to implement this:
   template <class T, class U = void> struct is_constructible : public is_convertible<U, T>{};
   template <class T> struct is_constructible<T, void> : public is_default_constructible<T>{};
   template <> struct is_constructible<void, void> : public false_type{};
   template <> struct is_constructible<void const, void> : public false_type{};
   template <> struct is_constructible<void const volatile, void> : public false_type{};
   template <> struct is_constructible<void volatile, void> : public false_type{};
   template <class Ref> struct is_constructible<Ref&, void> : public false_type{};
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
   template <class Ref> struct is_constructible<Ref&&, void> : public false_type{};
#endif
#endif

} // namespace hboost

#endif // HBOOST_TT_IS_CONSTRUCTIBLE_HPP_INCLUDED
