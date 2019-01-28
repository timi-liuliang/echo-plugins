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

#ifndef HBOOST_INTRUSIVE_DETAIL_SIMPLE_DISPOSERS_HPP
#define HBOOST_INTRUSIVE_DETAIL_SIMPLE_DISPOSERS_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace hboost {
namespace intrusive {
namespace detail {

class null_disposer
{
   public:
   template <class Pointer>
   void operator()(Pointer)
   {}
};

template<class NodeAlgorithms>
class init_disposer
{
   typedef typename NodeAlgorithms::node_ptr node_ptr;

   public:
   void operator()(const node_ptr & p)
   {  NodeAlgorithms::init(p);   }
};

}  //namespace detail{
}  //namespace intrusive{
}  //namespace hboost{

#endif //HBOOST_INTRUSIVE_DETAIL_SIMPLE_DISPOSERS_HPP
