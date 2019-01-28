
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_VOID_HPP_INCLUDED
#define HBOOST_TT_IS_VOID_HPP_INCLUDED

#include <hboost/type_traits/integral_constant.hpp>

namespace hboost {

template <class T>
struct is_void : public false_type {};

template<> struct is_void<void> : public true_type {};
template<> struct is_void<const void> : public true_type{};
template<> struct is_void<const volatile void> : public true_type{};
template<> struct is_void<volatile void> : public true_type{};

} // namespace hboost

#endif // HBOOST_TT_IS_VOID_HPP_INCLUDED