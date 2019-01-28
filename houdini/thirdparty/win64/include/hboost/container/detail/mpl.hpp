//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_CONTAINER_DETAIL_MPL_HPP
#define HBOOST_CONTAINER_CONTAINER_DETAIL_MPL_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/container/detail/config_begin.hpp>
#include <hboost/container/detail/workaround.hpp>
#include <hboost/move/detail/type_traits.hpp>
#include <hboost/intrusive/detail/mpl.hpp>

#include <cstddef>

namespace hboost {
namespace container {
namespace container_detail {

using hboost::move_detail::integral_constant;
using hboost::move_detail::true_type;
using hboost::move_detail::false_type;
using hboost::move_detail::enable_if_c;
using hboost::move_detail::enable_if;
using hboost::move_detail::enable_if_convertible;
using hboost::move_detail::disable_if_c;
using hboost::move_detail::disable_if;
using hboost::move_detail::disable_if_convertible;
using hboost::move_detail::is_convertible;
using hboost::move_detail::if_c;
using hboost::move_detail::if_;
using hboost::move_detail::identity;
using hboost::move_detail::bool_;
using hboost::move_detail::true_;
using hboost::move_detail::false_;
using hboost::move_detail::yes_type;
using hboost::move_detail::no_type;
using hboost::move_detail::bool_;
using hboost::move_detail::true_;
using hboost::move_detail::false_;
using hboost::move_detail::unvoid_ref;
using hboost::move_detail::and_;
using hboost::move_detail::or_;
using hboost::move_detail::not_;
using hboost::move_detail::enable_if_and;
using hboost::move_detail::disable_if_and;
using hboost::move_detail::enable_if_or;
using hboost::move_detail::disable_if_or;


template <class Pair>
struct select1st
{
   typedef Pair                        argument_type;
   typedef typename Pair::first_type   result_type;

   template<class OtherPair>
   const typename Pair::first_type& operator()(const OtherPair& x) const
   {  return x.first;   }

   const typename Pair::first_type& operator()(const typename Pair::first_type& x) const
   {  return x;   }
};

}  //namespace container_detail {
}  //namespace container {
}  //namespace hboost {

#include <hboost/container/detail/config_end.hpp>

#endif   //#ifndef HBOOST_CONTAINER_CONTAINER_DETAIL_MPL_HPP

