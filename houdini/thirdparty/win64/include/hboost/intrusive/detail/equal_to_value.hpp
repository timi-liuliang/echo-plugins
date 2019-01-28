/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2014-2014
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTRUSIVE_DETAIL_EQUAL_TO_VALUE_HPP
#define HBOOST_INTRUSIVE_DETAIL_EQUAL_TO_VALUE_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/intrusive/detail/workaround.hpp>

namespace hboost {
namespace intrusive {
namespace detail {

//This functor compares a stored value
//and the one passed as an argument
template<class ConstReference>
class equal_to_value
{
   ConstReference t_;

   public:
   equal_to_value(ConstReference t)
      :  t_(t)
   {}

   HBOOST_INTRUSIVE_FORCEINLINE bool operator()(ConstReference t)const
   {  return t_ == t;   }
};

}  //namespace detail{
}  //namespace intrusive{
}  //namespace hboost{

#endif //HBOOST_INTRUSIVE_DETAIL_EQUAL_TO_VALUE_HPP
