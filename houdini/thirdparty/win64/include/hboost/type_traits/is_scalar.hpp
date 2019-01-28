
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_SCALAR_HPP_INCLUDED
#define HBOOST_TT_IS_SCALAR_HPP_INCLUDED

#include <hboost/type_traits/is_arithmetic.hpp>
#include <hboost/type_traits/is_enum.hpp>
#include <hboost/type_traits/is_pointer.hpp>
#include <hboost/type_traits/is_member_pointer.hpp>
#include <hboost/config.hpp>

namespace hboost {

template <typename T>
struct is_scalar
   : public integral_constant<bool, ::hboost::is_arithmetic<T>::value || ::hboost::is_enum<T>::value || ::hboost::is_pointer<T>::value || ::hboost::is_member_pointer<T>::value>
{};

} // namespace hboost

#endif // HBOOST_TT_IS_SCALAR_HPP_INCLUDED
