//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2014-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef HBOOST_CONTAINER_DETAIL_ALLOC_TRAITS_HPP
#define HBOOST_CONTAINER_DETAIL_ALLOC_TRAITS_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

// move
#include <hboost/move/adl_move_swap.hpp>
#include <hboost/move/utility_core.hpp>

namespace hboost {
namespace container {
namespace container_detail {

template<class AllocatorType>
inline void swap_alloc(AllocatorType &, AllocatorType &, container_detail::false_type)
   HBOOST_NOEXCEPT_OR_NOTHROW
{}

template<class AllocatorType>
inline void swap_alloc(AllocatorType &l, AllocatorType &r, container_detail::true_type)
{  hboost::adl_move_swap(l, r);   }

template<class AllocatorType>
inline void assign_alloc(AllocatorType &, const AllocatorType &, container_detail::false_type)
   HBOOST_NOEXCEPT_OR_NOTHROW
{}

template<class AllocatorType>
inline void assign_alloc(AllocatorType &l, const AllocatorType &r, container_detail::true_type)
{  l = r;   }

template<class AllocatorType>
inline void move_alloc(AllocatorType &, AllocatorType &, container_detail::false_type)
   HBOOST_NOEXCEPT_OR_NOTHROW
{}

template<class AllocatorType>
inline void move_alloc(AllocatorType &l, AllocatorType &r, container_detail::true_type)
{  l = ::hboost::move(r);   }

}  //namespace container_detail {
}  //namespace container {
}  //namespace hboost {

#endif   //#ifndef HBOOST_CONTAINER_DETAIL_ALLOC_TRAITS_HPP
