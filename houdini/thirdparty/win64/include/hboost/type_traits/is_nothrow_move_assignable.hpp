
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  (C) Copyright Eric Friedman 2002-2003.
//  (C) Copyright Antony Polukhin 2013.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_NOTHROW_MOVE_ASSIGNABLE_HPP_INCLUDED
#define HBOOST_TT_IS_NOTHROW_MOVE_ASSIGNABLE_HPP_INCLUDED

#include <hboost/config.hpp>
#include <hboost/type_traits/has_trivial_move_assign.hpp>
#include <hboost/type_traits/has_nothrow_assign.hpp>
#include <hboost/type_traits/is_array.hpp>
#include <hboost/type_traits/is_reference.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/type_traits/declval.hpp>

namespace hboost {

#ifdef HBOOST_IS_NOTHROW_MOVE_ASSIGN

template <class T>
struct is_nothrow_move_assignable : public integral_constant<bool, HBOOST_IS_NOTHROW_MOVE_ASSIGN(T)>{};
template <class T> struct is_nothrow_move_assignable<T const> : public false_type{};
template <class T> struct is_nothrow_move_assignable<T volatile> : public false_type{};
template <class T> struct is_nothrow_move_assignable<T const volatile> : public false_type{};
template <class T> struct is_nothrow_move_assignable<T&> : public false_type{};
#if !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) 
template <class T> struct is_nothrow_move_assignable<T&&> : public false_type{};
#endif

#elif !defined(HBOOST_NO_CXX11_NOEXCEPT) && !defined(HBOOST_NO_SFINAE_EXPR) && !HBOOST_WORKAROUND(HBOOST_GCC_VERSION, < 40700)

namespace detail{

template <class T, class Enable = void>
struct false_or_cpp11_noexcept_move_assignable: public ::hboost::false_type {};

template <class T>
struct false_or_cpp11_noexcept_move_assignable <
        T,
        typename ::hboost::enable_if_c<sizeof(T) && HBOOST_NOEXCEPT_EXPR(::hboost::declval<T&>() = ::hboost::declval<T>())>::type
    > : public ::hboost::integral_constant<bool, HBOOST_NOEXCEPT_EXPR(::hboost::declval<T&>() = ::hboost::declval<T>())>
{};

}

template <class T>
struct is_nothrow_move_assignable : public integral_constant<bool, ::hboost::detail::false_or_cpp11_noexcept_move_assignable<T>::value>{};

template <class T> struct is_nothrow_move_assignable<T const> : public ::hboost::false_type {};
template <class T> struct is_nothrow_move_assignable<T const volatile> : public ::hboost::false_type{};
template <class T> struct is_nothrow_move_assignable<T volatile> : public ::hboost::false_type{};
template <class T> struct is_nothrow_move_assignable<T&> : public ::hboost::false_type{};
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
template <class T> struct is_nothrow_move_assignable<T&&> : public ::hboost::false_type{};
#endif

#else

template <class T>
struct is_nothrow_move_assignable : public integral_constant<bool,
   (::hboost::has_trivial_move_assign<T>::value || ::hboost::has_nothrow_assign<T>::value) &&  ! ::hboost::is_array<T>::value>{};

#endif


template <> struct is_nothrow_move_assignable<void> : public false_type{};
#ifndef HBOOST_NO_CV_VOID_SPECIALIZATIONS
template <> struct is_nothrow_move_assignable<void const> : public false_type{};
template <> struct is_nothrow_move_assignable<void const volatile> : public false_type{};
template <> struct is_nothrow_move_assignable<void volatile> : public false_type{};
#endif

} // namespace hboost

#endif // HBOOST_TT_IS_NOTHROW_MOVE_ASSIGNABLE_HPP_INCLUDED
