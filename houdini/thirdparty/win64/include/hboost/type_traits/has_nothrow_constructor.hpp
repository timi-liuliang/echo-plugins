
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_HAS_NOTHROW_CONSTRUCTOR_HPP_INCLUDED
#define HBOOST_TT_HAS_NOTHROW_CONSTRUCTOR_HPP_INCLUDED

#include <hboost/type_traits/intrinsics.hpp>
#include <hboost/type_traits/integral_constant.hpp>

#ifdef HBOOST_HAS_NOTHROW_CONSTRUCTOR

#if defined(HBOOST_MSVC) || defined(HBOOST_INTEL)
#include <hboost/type_traits/has_trivial_constructor.hpp>
#endif
#if defined(__GNUC__ ) || defined(__SUNPRO_CC) || defined(__clang__)
#include <hboost/type_traits/is_default_constructible.hpp>
#endif

namespace hboost {

template <class T> struct has_nothrow_constructor : public integral_constant<bool, HBOOST_HAS_NOTHROW_CONSTRUCTOR(T)>{};

#elif !defined(HBOOST_NO_CXX11_NOEXCEPT)

#include <hboost/type_traits/is_default_constructible.hpp>
#include <hboost/type_traits/remove_all_extents.hpp>

#ifdef HBOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4197) // top-level volatile in cast is ignored
#endif

namespace hboost { namespace detail{

   template <class T, bool b> struct has_nothrow_constructor_imp : public hboost::integral_constant<bool, false>{};
   template <class T> struct has_nothrow_constructor_imp<T, true> : public hboost::integral_constant<bool, noexcept(T())>{};
   template <class T, std::size_t N> struct has_nothrow_constructor_imp<T[N], true> : public has_nothrow_constructor_imp<T, true> {};
}

template <class T> struct has_nothrow_constructor : public detail::has_nothrow_constructor_imp<T, is_default_constructible<T>::value>{};

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#else

#include <hboost/type_traits/has_trivial_constructor.hpp>

namespace hboost {

template <class T> struct has_nothrow_constructor : public ::hboost::has_trivial_constructor<T> {};

#endif

template<> struct has_nothrow_constructor<void> : public false_type {};
#ifndef HBOOST_NO_CV_VOID_SPECIALIZATIONS
template<> struct has_nothrow_constructor<void const> : public false_type{};
template<> struct has_nothrow_constructor<void const volatile> : public false_type{};
template<> struct has_nothrow_constructor<void volatile> : public false_type{};
#endif

template <class T> struct has_nothrow_default_constructor : public has_nothrow_constructor<T>{};

} // namespace hboost

#endif // HBOOST_TT_HAS_NOTHROW_CONSTRUCTOR_HPP_INCLUDED
