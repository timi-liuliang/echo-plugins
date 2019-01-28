
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
//  Hinnant & John Maddock 2000.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef HBOOST_TT_REMOVE_VOLATILE_HPP_INCLUDED
#define HBOOST_TT_REMOVE_VOLATILE_HPP_INCLUDED

#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>
#include <cstddef>

namespace hboost {

   //  convert a type T to a non-cv-qualified type - remove_volatile<T>
   template <class T> struct remove_volatile{ typedef T type; };
   template <class T> struct remove_volatile<T volatile>{ typedef T type; };

#if !defined(HBOOST_NO_ARRAY_TYPE_SPECIALIZATIONS)
   template <class T, std::size_t N> struct remove_volatile<T volatile[N]>{ typedef T type[N]; };
#if !HBOOST_WORKAROUND(__BORLANDC__, < 0x600) && !defined(__IBMCPP__) &&  !HBOOST_WORKAROUND(__DMC__, HBOOST_TESTED_AT(0x840))
   template <class T> struct remove_volatile<T volatile[]>{ typedef T type[]; };
#endif
#endif


} // namespace hboost

#endif // HBOOST_TT_REMOVE_VOLATILE_HPP_INCLUDED
