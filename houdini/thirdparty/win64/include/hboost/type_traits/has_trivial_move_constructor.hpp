
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  (C) Copyright Eric Friedman 2002-2003.
//  (C) Copyright Antony Polukhin 2013.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_HAS_TRIVIAL_MOVE_CONSTRUCTOR_HPP_INCLUDED
#define HBOOST_TT_HAS_TRIVIAL_MOVE_CONSTRUCTOR_HPP_INCLUDED

#include <hboost/type_traits/intrinsics.hpp>
#include <hboost/type_traits/integral_constant.hpp>

#ifdef HBOOST_HAS_TRIVIAL_MOVE_CONSTRUCTOR

#if defined(HBOOST_MSVC) || defined(HBOOST_INTEL)
#include <hboost/type_traits/is_pod.hpp>
#include <hboost/type_traits/is_volatile.hpp>
#endif

#if defined(__GNUC__) || defined(__clang)
#include <hboost/type_traits/is_constructible.hpp>
#include <hboost/type_traits/is_volatile.hpp>
#endif


namespace hboost {

template <typename T> struct has_trivial_move_constructor : public integral_constant<bool, HBOOST_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T)>{};

#else

#ifdef __SUNPRO_CC
#include <hboost/type_traits/is_constructible.hpp>
#include <hboost/type_traits/remove_const.hpp>
#if __cplusplus >= 201103
#define SOLARIS_EXTRA_CHECK && is_constructible<typename remove_const<T>::type, typename remove_const<T>::type&&>::value
#endif
#endif

#ifndef SOLARIS_EXTRA_CHECK
#define SOLARIS_EXTRA_CHECK
#endif

#include <hboost/type_traits/is_pod.hpp>
#include <hboost/type_traits/is_volatile.hpp>

namespace hboost {

template <typename T> struct has_trivial_move_constructor 
   : public integral_constant<bool, ::hboost::is_pod<T>::value && !::hboost::is_volatile<T>::value SOLARIS_EXTRA_CHECK>{};

#undef SOLARIS_EXTRA_CHECK

#endif

template <> struct has_trivial_move_constructor<void> : public false_type{};
#ifndef HBOOST_NO_CV_VOID_SPECIALIZATIONS
template <> struct has_trivial_move_constructor<void const> : public false_type{};
template <> struct has_trivial_move_constructor<void volatile> : public false_type{};
template <> struct has_trivial_move_constructor<void const volatile> : public false_type{};
#endif
// What should we do with reference types??? The standard seems to suggest these are trivial, even if the thing they reference is not:
template <class T> struct has_trivial_move_constructor<T&> : public true_type{};
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
template <class T> struct has_trivial_move_constructor<T&&> : public true_type{};
#endif
// Arrays can not be explicitly copied:
template <class T, std::size_t N> struct has_trivial_move_constructor<T[N]> : public false_type{};
template <class T> struct has_trivial_move_constructor<T[]> : public false_type{};

} // namespace hboost

#endif // HBOOST_TT_HAS_TRIVIAL_MOVE_CONSTRUCTOR_HPP_INCLUDED
