
//  (C) Copyright John Maddock 2005.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef HBOOST_TT_HAS_VIRTUAL_DESTRUCTOR_HPP_INCLUDED
#define HBOOST_TT_HAS_VIRTUAL_DESTRUCTOR_HPP_INCLUDED

#include <hboost/type_traits/intrinsics.hpp>
#include <hboost/type_traits/integral_constant.hpp>

namespace hboost {

#ifdef HBOOST_HAS_VIRTUAL_DESTRUCTOR
   template <class T> struct has_virtual_destructor : public integral_constant<bool, HBOOST_HAS_VIRTUAL_DESTRUCTOR(T)>{};
#else
   template <class T> struct has_virtual_destructor : public integral_constant<bool, false>{};
#endif

} // namespace hboost

#endif // HBOOST_TT_IS_MEMBER_FUNCTION_POINTER_HPP_INCLUDED
