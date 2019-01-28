//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2008-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_DETAIL_VARIADIC_TEMPLATES_TOOLS_HPP
#define HBOOST_INTERPROCESS_DETAIL_VARIADIC_TEMPLATES_TOOLS_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/container/detail/variadic_templates_tools.hpp>

namespace hboost {
namespace interprocess {
namespace ipcdetail {

using hboost::container::container_detail::tuple;
using hboost::container::container_detail::build_number_seq;
using hboost::container::container_detail::index_tuple;
using hboost::container::container_detail::get;

}}}   //namespace hboost { namespace interprocess { namespace ipcdetail {

#endif   //#ifndef HBOOST_INTERPROCESS_DETAIL_VARIADIC_TEMPLATES_TOOLS_HPP
