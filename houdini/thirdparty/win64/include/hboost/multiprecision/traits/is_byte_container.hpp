///////////////////////////////////////////////////////////////////////////////
//  Copyright 2015 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_IS_BYTE_CONTAINER_HPP
#define HBOOST_IS_BYTE_CONTAINER_HPP

#include <hboost/mpl/has_xxx.hpp>
#include <hboost/type_traits/is_integral.hpp>

namespace hboost{ namespace multiprecision{ namespace detail{

   HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_member_value_type, value_type, false);
   HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_member_const_iterator, const_iterator, false);

   template <class C, bool b>
   struct is_byte_container_imp
   {
      static const bool value = hboost::is_integral<typename C::value_type>::value && (sizeof(typename C::value_type) == 1);
   };

   template <class C>
   struct is_byte_container_imp<C, false> : public hboost::false_type {};

   template <class C>
   struct is_byte_container : public is_byte_container_imp<C, has_member_value_type<C>::value && has_member_const_iterator<C>::value> {};


}}} // namespaces

#endif // HBOOST_IS_BYTE_CONTAINER_HPP

