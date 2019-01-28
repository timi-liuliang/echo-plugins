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

#ifndef HBOOST_CONTAINER_DETAIL_VALUE_INIT_HPP
#define HBOOST_CONTAINER_DETAIL_VALUE_INIT_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/container/detail/config_begin.hpp>
#include <hboost/container/detail/workaround.hpp>

namespace hboost {
namespace container {
namespace container_detail {

template<class T>
struct value_init
{
   value_init()
      : m_t()
   {}

   operator T &() { return m_t; }

   T m_t;
};

}  //namespace container_detail {
}  //namespace container {
}  //namespace hboost {

#include <hboost/container/detail/config_end.hpp>

#endif   //#ifndef HBOOST_CONTAINER_DETAIL_VALUE_INIT_HPP
