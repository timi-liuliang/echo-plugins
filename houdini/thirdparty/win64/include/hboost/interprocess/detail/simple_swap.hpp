#ifndef HBOOST_INTERPROCESS_DETAIL_SWAP_HPP
#define HBOOST_INTERPROCESS_DETAIL_SWAP_HPP
//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2014-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace hboost { namespace interprocess {

template<class T>
void simple_swap(T&x, T&y)
{  T tmp(x); x = y; y = tmp;  }

}}  //namespace hboost{ namespace interprocess {

#endif //#ifndef HBOOST_INTERPROCESS_DETAIL_SWAP_HPP
