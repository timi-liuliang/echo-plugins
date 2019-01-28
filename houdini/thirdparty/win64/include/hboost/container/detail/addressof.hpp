//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2014-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef HBOOST_CONTAINER_DETAIL_ADDRESSOF_HPP
#define HBOOST_CONTAINER_DETAIL_ADDRESSOF_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <cstddef>

namespace hboost {
namespace container {
namespace container_detail {

template <typename T>
inline T* addressof(T& obj)
{
   return static_cast<T*>(
      static_cast<void*>(
         const_cast<char*>(
            &reinterpret_cast<const char&>(obj)
   )));
}

}  //namespace container_detail {
}  //namespace container {
}  //namespace hboost {

#endif   //#ifndef HBOOST_CONTAINER_DETAIL_ADDRESSOF_HPP
