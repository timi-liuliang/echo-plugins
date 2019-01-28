//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2012.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_DETAIL_MIN_MAX_HPP
#define HBOOST_INTERPROCESS_DETAIL_MIN_MAX_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/interprocess/detail/config_begin.hpp>
#include <hboost/interprocess/detail/workaround.hpp>

namespace hboost {
namespace interprocess {

template<class T>
const T &max_value(const T &a, const T &b)
{  return a > b ? a : b;   }

template<class T>
const T &min_value(const T &a, const T &b)
{  return a < b ? a : b;   }

}  //namespace interprocess {
}  //namespace hboost {

#include <hboost/interprocess/detail/config_end.hpp>

#endif   //#ifndef HBOOST_INTERPROCESS_DETAIL_MIN_MAX_HPP

