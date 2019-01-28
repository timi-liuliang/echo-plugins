
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_OBJECT_HPP_INCLUDED
#define HBOOST_TT_IS_OBJECT_HPP_INCLUDED

#include <hboost/config.hpp>
#include <hboost/type_traits/is_reference.hpp>
#include <hboost/type_traits/is_void.hpp>
#include <hboost/type_traits/is_function.hpp>

namespace hboost {

template <class T> struct is_object
   : public 
      integral_constant<
         bool, 
         ! ::hboost::is_reference<T>::value && ! ::hboost::is_void<T>::value && ! ::hboost::is_function<T>::value > 
{};

} // namespace hboost

#endif // HBOOST_TT_IS_OBJECT_HPP_INCLUDED
