//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2014-2014.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_DETAIL_ITERATOR_HPP
#define HBOOST_CONTAINER_DETAIL_ITERATOR_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/intrusive/detail/iterator.hpp>

namespace hboost {
namespace container {

using ::hboost::intrusive::iterator_traits;
using ::hboost::intrusive::iterator_distance;
using ::hboost::intrusive::iterator_advance;
using ::hboost::intrusive::iterator;
using ::hboost::intrusive::iterator_enable_if_tag;
using ::hboost::intrusive::iterator_disable_if_tag;
using ::hboost::intrusive::iterator_arrow_result;

}  //namespace container {
}  //namespace hboost {

#endif   //#ifndef HBOOST_CONTAINER_DETAIL_ITERATORS_HPP
