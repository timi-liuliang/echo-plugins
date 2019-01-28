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

#ifndef HBOOST_CONTAINER_DETAIL_ALGORITHM_HPP
#define HBOOST_CONTAINER_DETAIL_ALGORITHM_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/intrusive/detail/algorithm.hpp>

namespace hboost {
namespace container {

using hboost::intrusive::algo_equal;
using hboost::intrusive::algo_lexicographical_compare;

}  //namespace container {
}  //namespace hboost {

#endif   //#ifndef HBOOST_CONTAINER_DETAIL_ALGORITHM_HPP
