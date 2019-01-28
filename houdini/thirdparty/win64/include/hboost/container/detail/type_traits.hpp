//////////////////////////////////////////////////////////////////////////////
// (C) Copyright John Maddock 2000.
// (C) Copyright Ion Gaztanaga 2005-2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
// The alignment and Type traits implementation comes from
// John Maddock's TypeTraits library.
//
// Some other tricks come from Howard Hinnant's papers and StackOverflow replies
//////////////////////////////////////////////////////////////////////////////
#ifndef HBOOST_CONTAINER_CONTAINER_DETAIL_TYPE_TRAITS_HPP
#define HBOOST_CONTAINER_CONTAINER_DETAIL_TYPE_TRAITS_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/move/detail/type_traits.hpp>

namespace hboost {
namespace container {
namespace container_detail {

using ::hboost::move_detail::enable_if;
using ::hboost::move_detail::enable_if_and;
using ::hboost::move_detail::is_same;
using ::hboost::move_detail::is_different;
using ::hboost::move_detail::is_pointer;
using ::hboost::move_detail::add_reference;
using ::hboost::move_detail::add_const;
using ::hboost::move_detail::add_const_reference;
using ::hboost::move_detail::remove_const;
using ::hboost::move_detail::remove_reference;
using ::hboost::move_detail::make_unsigned;
using ::hboost::move_detail::is_floating_point;
using ::hboost::move_detail::is_integral;
using ::hboost::move_detail::is_enum;
using ::hboost::move_detail::is_pod;
using ::hboost::move_detail::is_empty;
using ::hboost::move_detail::is_trivially_destructible;
using ::hboost::move_detail::is_trivially_default_constructible;
using ::hboost::move_detail::is_trivially_copy_constructible;
using ::hboost::move_detail::is_trivially_move_constructible;
using ::hboost::move_detail::is_trivially_copy_assignable;
using ::hboost::move_detail::is_trivially_move_assignable;
using ::hboost::move_detail::is_nothrow_default_constructible;
using ::hboost::move_detail::is_nothrow_copy_constructible;
using ::hboost::move_detail::is_nothrow_move_constructible;
using ::hboost::move_detail::is_nothrow_copy_assignable;
using ::hboost::move_detail::is_nothrow_move_assignable;
using ::hboost::move_detail::is_nothrow_swappable;
using ::hboost::move_detail::alignment_of;
using ::hboost::move_detail::aligned_storage;
using ::hboost::move_detail::nat;
using ::hboost::move_detail::max_align_t;

}  //namespace container_detail {
}  //namespace container {
}  //namespace hboost {

#endif   //#ifndef HBOOST_CONTAINER_CONTAINER_DETAIL_TYPE_TRAITS_HPP
