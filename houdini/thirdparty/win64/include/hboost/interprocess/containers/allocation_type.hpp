//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2008-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_CONTAINERS_ALLOCATION_TYPE_HPP
#define HBOOST_INTERPROCESS_CONTAINERS_ALLOCATION_TYPE_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/interprocess/detail/config_begin.hpp>
#include <hboost/container/detail/allocation_type.hpp>

namespace hboost {
namespace interprocess {

#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)
typedef int allocation_type;
#endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED
static const allocation_type allocate_new       = hboost::container::allocate_new;
static const allocation_type expand_fwd         = hboost::container::expand_fwd;
static const allocation_type expand_bwd         = hboost::container::expand_bwd;
static const allocation_type shrink_in_place    = hboost::container::shrink_in_place;
static const allocation_type try_shrink_in_place= hboost::container::try_shrink_in_place;
static const allocation_type nothrow_allocation = hboost::container::nothrow_allocation;
static const allocation_type zero_memory        = hboost::container::zero_memory;

}  //namespace interprocess {
}  //namespace hboost {

#include <hboost/interprocess/detail/config_end.hpp>

#endif //   #ifndef  HBOOST_INTERPROCESS_CONTAINERS_ALLOCATION_TYPE_HPP
