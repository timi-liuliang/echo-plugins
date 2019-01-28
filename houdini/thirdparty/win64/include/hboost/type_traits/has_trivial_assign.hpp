
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_HAS_TRIVIAL_ASSIGN_HPP_INCLUDED
#define HBOOST_TT_HAS_TRIVIAL_ASSIGN_HPP_INCLUDED

#include <hboost/type_traits/detail/config.hpp>
#include <hboost/type_traits/intrinsics.hpp>
#include <hboost/type_traits/integral_constant.hpp>

#if !defined(HBOOST_HAS_TRIVIAL_ASSIGN) || defined(HBOOST_MSVC) || defined(__GNUC__) || defined(HBOOST_INTEL) || defined(__SUNPRO_CC) || defined(__clang__)
#include <hboost/type_traits/is_pod.hpp>
#include <hboost/type_traits/is_const.hpp>
#include <hboost/type_traits/is_volatile.hpp>
#include <hboost/type_traits/is_assignable.hpp>
#endif

namespace hboost {

   template <typename T>
   struct has_trivial_assign : public integral_constant < bool,
#ifdef HBOOST_HAS_TRIVIAL_ASSIGN
      HBOOST_HAS_TRIVIAL_ASSIGN(T)
#else
      ::hboost::is_pod<T>::value && !::hboost::is_const<T>::value && !::hboost::is_volatile<T>::value
#endif
   > {};

   template<> struct has_trivial_assign<void> : public false_type{};
#ifndef HBOOST_NO_CV_VOID_SPECIALIZATIONS
   template<> struct has_trivial_assign<void const> : public false_type{};
   template<> struct has_trivial_assign<void const volatile> : public false_type{};
   template<> struct has_trivial_assign<void volatile> : public false_type{};
#endif
   template <class T> struct has_trivial_assign<T volatile> : public false_type{};
   template <class T> struct has_trivial_assign<T&> : public false_type{};
#if !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)
   template <class T> struct has_trivial_assign<T&&> : public false_type{};
#endif
   // Arrays are not explictly assignable:
   template <typename T, std::size_t N> struct has_trivial_assign<T[N]> : public false_type{};
   template <typename T> struct has_trivial_assign<T[]> : public false_type{};

} // namespace hboost

#endif // HBOOST_TT_HAS_TRIVIAL_ASSIGN_HPP_INCLUDED
