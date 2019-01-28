//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2008-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_CONTAINERS_VERSION_TYPE_HPP
#define HBOOST_INTERPROCESS_CONTAINERS_VERSION_TYPE_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/interprocess/detail/config_begin.hpp>
#include <hboost/container/detail/version_type.hpp>

namespace hboost {
namespace interprocess {

using hboost::container::container_detail::version_type;
using hboost::container::container_detail::version;

}  //namespace interprocess {
}  //namespace hboost {

#include <hboost/interprocess/detail/config_end.hpp>

#endif //   #ifndef  HBOOST_INTERPROCESS_CONTAINERS_VERSION_TYPE_HPP

