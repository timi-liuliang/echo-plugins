
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_HAS_TRIVIAL_COPY_HPP_INCLUDED
#define HBOOST_TT_HAS_TRIVIAL_COPY_HPP_INCLUDED

#include <hboost/type_traits/intrinsics.hpp>
#include <hboost/type_traits/is_pod.hpp>
#include <hboost/type_traits/is_reference.hpp>

#if (defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 409)) || defined(HBOOST_CLANG) || (defined(__SUNPRO_CC) && defined(HBOOST_HAS_TRIVIAL_COPY))
#include <hboost/type_traits/is_copy_constructible.hpp>
#define HBOOST_TT_TRIVIAL_CONSTRUCT_FIX && is_copy_constructible<T>::value
#else
#define HBOOST_TT_TRIVIAL_CONSTRUCT_FIX
#endif

#ifdef HBOOST_INTEL
#include <hboost/type_traits/add_const.hpp>
#include <hboost/type_traits/add_lvalue_reference.hpp>
#endif

namespace hboost {

template <typename T> struct has_trivial_copy 
: public integral_constant<bool, 
#ifdef HBOOST_HAS_TRIVIAL_COPY
   HBOOST_HAS_TRIVIAL_COPY(T) HBOOST_TT_TRIVIAL_CONSTRUCT_FIX
#else
   ::hboost::is_pod<T>::value
#endif
>{};
// Arrays are not explicitly copyable:
template <typename T, std::size_t N> struct has_trivial_copy<T[N]> : public false_type{};
template <typename T> struct has_trivial_copy<T[]> : public false_type{};
// Are volatile types ever trivial?  We don't really know, so assume not:
template <typename T> struct has_trivial_copy<T volatile> : public false_type{};

template <> struct has_trivial_copy<void> : public false_type{};
#ifndef HBOOST_NO_CV_VOID_SPECIALIZATIONS
template <> struct has_trivial_copy<void const> : public false_type{};
template <> struct has_trivial_copy<void volatile> : public false_type{};
template <> struct has_trivial_copy<void const volatile> : public false_type{};
#endif

template <class T> struct has_trivial_copy<T&> : public false_type{};
#if !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) 
template <class T> struct has_trivial_copy<T&&> : public false_type{};
#endif

template <class T> struct has_trivial_copy_constructor : public has_trivial_copy<T>{};

#undef HBOOST_TT_TRIVIAL_CONSTRUCT_FIX

} // namespace hboost

#endif // HBOOST_TT_HAS_TRIVIAL_COPY_HPP_INCLUDED
