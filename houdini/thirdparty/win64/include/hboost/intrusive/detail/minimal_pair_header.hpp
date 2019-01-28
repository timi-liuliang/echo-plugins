/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2014-2015
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef HBOOST_INTRUSIVE_DETAIL_MINIMAL_PAIR_HEADER_HPP
#define HBOOST_INTRUSIVE_DETAIL_MINIMAL_PAIR_HEADER_HPP
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif
#
#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#//Try to avoid including <utility>, as it's quite big in C++11
#if defined(HBOOST_GNU_STDLIB)
#  include <bits/stl_pair.h>
#else
#  include <utility>    //Fallback
#endif
#
#endif //HBOOST_INTRUSIVE_DETAIL_MINIMAL_PAIR_HEADER_HPP
