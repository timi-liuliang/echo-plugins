
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_STATELESS_HPP_INCLUDED
#define HBOOST_TT_IS_STATELESS_HPP_INCLUDED

#include <hboost/type_traits/has_trivial_constructor.hpp>
#include <hboost/type_traits/has_trivial_copy.hpp>
#include <hboost/type_traits/has_trivial_destructor.hpp>
#include <hboost/type_traits/is_class.hpp>
#include <hboost/type_traits/is_empty.hpp>
#include <hboost/config.hpp>

namespace hboost {

template <typename T>
struct is_stateless
 : public integral_constant<bool,  
      (::hboost::has_trivial_constructor<T>::value
      && ::hboost::has_trivial_copy<T>::value
      && ::hboost::has_trivial_destructor<T>::value
      && ::hboost::is_class<T>::value
      && ::hboost::is_empty<T>::value)>
{};

} // namespace hboost

#endif // HBOOST_TT_IS_STATELESS_HPP_INCLUDED
