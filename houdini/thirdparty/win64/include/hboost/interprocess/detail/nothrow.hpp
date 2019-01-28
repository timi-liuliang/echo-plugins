//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2014-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef HBOOST_INTERPROCESS_DETAIL_NOTHROW_HPP
#define HBOOST_INTERPROCESS_DETAIL_NOTHROW_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace std {   //no namespace versioning in clang+libc++

struct nothrow_t;

}  //namespace std {

namespace hboost{ namespace interprocess {

template <int Dummy = 0>
struct nothrow
{
   static const std::nothrow_t &get()   {  return *pnothrow;  }
   static std::nothrow_t *pnothrow;
};

template <int Dummy>
std::nothrow_t *nothrow<Dummy>::pnothrow;

}}  //namespace hboost{ namespace interprocess {

#endif //#ifndef HBOOST_INTERPROCESS_DETAIL_NOTHROW_HPP
