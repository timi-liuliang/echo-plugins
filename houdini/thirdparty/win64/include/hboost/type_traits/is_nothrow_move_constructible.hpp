
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  (C) Copyright Eric Friedman 2002-2003.
//  (C) Copyright Antony Polukhin 2013.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_NOTHROW_MOVE_CONSTRUCTIBLE_HPP_INCLUDED
#define HBOOST_TT_IS_NOTHROW_MOVE_CONSTRUCTIBLE_HPP_INCLUDED

#include <hboost/config.hpp>
#include <hboost/type_traits/intrinsics.hpp>
#include <hboost/type_traits/integral_constant.hpp>
#include <hboost/detail/workaround.hpp>

#ifdef HBOOST_IS_NOTHROW_MOVE_CONSTRUCT

namespace hboost {

template <class T>
struct is_nothrow_move_constructible : public integral_constant<bool, HBOOST_IS_NOTHROW_MOVE_CONSTRUCT(T)>{};

template <class T> struct is_nothrow_move_constructible<volatile T> : public ::hboost::false_type {};
template <class T> struct is_nothrow_move_constructible<const volatile T> : public ::hboost::false_type{};

#elif !defined(HBOOST_NO_CXX11_NOEXCEPT) && !defined(HBOOST_NO_SFINAE_EXPR) && !HBOOST_WORKAROUND(HBOOST_GCC_VERSION, < 40800)

#include <hboost/type_traits/declval.hpp>
#include <hboost/utility/enable_if.hpp>

namespace hboost{ namespace detail{

template <class T, class Enable = void>
struct false_or_cpp11_noexcept_move_constructible: public ::hboost::false_type {};

template <class T>
struct false_or_cpp11_noexcept_move_constructible <
        T,
        typename ::hboost::enable_if_c<sizeof(T) && HBOOST_NOEXCEPT_EXPR(T(::hboost::declval<T>()))>::type
    > : public ::hboost::integral_constant<bool, HBOOST_NOEXCEPT_EXPR(T(::hboost::declval<T>()))>
{};

}

template <class T> struct is_nothrow_move_constructible
   : public integral_constant<bool, ::hboost::detail::false_or_cpp11_noexcept_move_constructible<T>::value>{};

template <class T> struct is_nothrow_move_constructible<volatile T> : public ::hboost::false_type {};
template <class T> struct is_nothrow_move_constructible<const volatile T> : public ::hboost::false_type{};
template <class T, std::size_t N> struct is_nothrow_move_constructible<T[N]> : public ::hboost::false_type{};
template <class T> struct is_nothrow_move_constructible<T[]> : public ::hboost::false_type{};

#else

#include <hboost/type_traits/has_trivial_move_constructor.hpp>
#include <hboost/type_traits/has_nothrow_copy.hpp>
#include <hboost/type_traits/is_array.hpp>

namespace hboost{

template <class T>
struct is_nothrow_move_constructible
   : public integral_constant<bool,
   (::hboost::has_trivial_move_constructor<T>::value || ::hboost::has_nothrow_copy<T>::value) && !::hboost::is_array<T>::value>
{};

#endif

template <> struct is_nothrow_move_constructible<void> : false_type{};
#ifndef HBOOST_NO_CV_VOID_SPECIALIZATIONS
template <> struct is_nothrow_move_constructible<void const> : false_type{};
template <> struct is_nothrow_move_constructible<void volatile> : false_type{};
template <> struct is_nothrow_move_constructible<void const volatile> : false_type{};
#endif
// References are always trivially constructible, even if the thing they reference is not:
template <class T> struct is_nothrow_move_constructible<T&> : public ::hboost::true_type{};
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
template <class T> struct is_nothrow_move_constructible<T&&> : public ::hboost::true_type{};
#endif

} // namespace hboost

#endif // HBOOST_TT_IS_NOTHROW_MOVE_CONSTRUCTIBLE_HPP_INCLUDED
