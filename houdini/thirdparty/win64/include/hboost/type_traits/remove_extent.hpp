
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000-2005.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_REMOVE_EXTENT_HPP_INCLUDED
#define HBOOST_TT_REMOVE_EXTENT_HPP_INCLUDED

#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>
#include <cstddef>

namespace hboost {

template <class T> struct remove_extent{ typedef T type; };

#if !defined(HBOOST_NO_ARRAY_TYPE_SPECIALIZATIONS)
template <typename T, std::size_t N> struct remove_extent<T[N]> { typedef T type; };
template <typename T, std::size_t N> struct remove_extent<T const[N]> { typedef T const type; };
template <typename T, std::size_t N> struct remove_extent<T volatile [N]> { typedef T volatile type; };
template <typename T, std::size_t N> struct remove_extent<T const volatile [N]> { typedef T const volatile type; };
#if !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610)) && !defined(__IBMCPP__) &&  !HBOOST_WORKAROUND(__DMC__, HBOOST_TESTED_AT(0x840))
template <typename T> struct remove_extent<T[]> { typedef T type; };
template <typename T> struct remove_extent<T const[]> { typedef T const type; };
template <typename T> struct remove_extent<T volatile[]> { typedef T volatile type; };
template <typename T> struct remove_extent<T const volatile[]> { typedef T const volatile type; };
#endif
#endif

} // namespace hboost

#endif // HBOOST_TT_REMOVE_BOUNDS_HPP_INCLUDED