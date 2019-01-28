
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
//  Hinnant & John Maddock 2000.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef HBOOST_TT_IS_UNION_HPP_INCLUDED
#define HBOOST_TT_IS_UNION_HPP_INCLUDED

#include <hboost/type_traits/intrinsics.hpp>
#include <hboost/type_traits/integral_constant.hpp>

namespace hboost {

#ifdef HBOOST_IS_UNION
template <class T> struct is_union : public integral_constant<bool, HBOOST_IS_UNION(T)> {};
#else
template <class T> struct is_union : public integral_constant<bool, false> {};
#endif

template <class T> struct is_union<T const> : public is_union<T>{};
template <class T> struct is_union<T volatile const> : public is_union<T>{};
template <class T> struct is_union<T volatile> : public is_union<T>{};

} // namespace hboost

#endif // HBOOST_TT_IS_UNION_HPP_INCLUDED
