//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_DETAIL_CAST_TAGS_HPP
#define HBOOST_INTERPROCESS_DETAIL_CAST_TAGS_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace hboost { namespace interprocess { namespace ipcdetail {

struct static_cast_tag {};
struct const_cast_tag {};
struct dynamic_cast_tag {};
struct reinterpret_cast_tag {};

}}}  //namespace hboost { namespace interprocess { namespace ipcdetail {

#endif   //#ifndef HBOOST_INTERPROCESS_DETAIL_CAST_TAGS_HPP
