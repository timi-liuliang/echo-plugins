//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2009-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_CONTAINERS_CONTAINERS_FWD_HPP
#define HBOOST_INTERPROCESS_CONTAINERS_CONTAINERS_FWD_HPP

#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

//////////////////////////////////////////////////////////////////////////////
//                        Standard predeclarations
//////////////////////////////////////////////////////////////////////////////

#include <hboost/interprocess/detail/config_begin.hpp>
#include <hboost/container/container_fwd.hpp>

namespace hboost {
namespace interprocess {

using hboost::container::ordered_range;
using hboost::container::ordered_unique_range;

}  //namespace interprocess {
}  //namespace hboost {

#include <hboost/interprocess/detail/config_end.hpp>

#endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

#endif //   #ifndef  HBOOST_INTERPROCESS_CONTAINERS_CONTAINERS_FWD_HPP
