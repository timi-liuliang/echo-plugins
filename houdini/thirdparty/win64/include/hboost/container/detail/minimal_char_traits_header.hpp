/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2014-2015
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef HBOOST_CONTAINER_DETAIL_MINIMAL_CHAR_TRAITS_HEADER_HPP
#define HBOOST_CONTAINER_DETAIL_MINIMAL_CHAR_TRAITS_HEADER_HPP
#
#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif
#
#//Try to avoid including <string>, as it's quite big
#if defined(_MSC_VER) && defined(HBOOST_DINKUMWARE_STDLIB)
   #include <iosfwd>   //Dinkum libraries for MSVC define std::char_traits there
#elif defined(HBOOST_GNU_STDLIB)
   #include <bits/char_traits.h>
#else
   #include <string>  //Fallback
#endif

#endif //HBOOST_CONTAINER_DETAIL_MINIMAL_CHAR_TRAITS_HEADER_HPP
