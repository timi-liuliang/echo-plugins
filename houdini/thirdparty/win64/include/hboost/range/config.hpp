// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_CONFIG_HPP
#define HBOOST_RANGE_CONFIG_HPP

#include <hboost/detail/workaround.hpp>

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/config.hpp>

#ifdef HBOOST_RANGE_DEDUCED_TYPENAME
#error "macro already defined!"
#endif

#if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
# define HBOOST_RANGE_DEDUCED_TYPENAME typename
#else
#define HBOOST_RANGE_DEDUCED_TYPENAME HBOOST_DEDUCED_TYPENAME
#endif

#ifdef HBOOST_RANGE_NO_ARRAY_SUPPORT
#error "macro already defined!"
#endif

#if HBOOST_WORKAROUND( __MWERKS__, <= 0x3003 )
#define HBOOST_RANGE_NO_ARRAY_SUPPORT 1
#endif

#ifdef HBOOST_RANGE_NO_ARRAY_SUPPORT
#define HBOOST_RANGE_ARRAY_REF() (hboost_range_array)
#define HBOOST_RANGE_NO_STATIC_ASSERT
#else
#define HBOOST_RANGE_ARRAY_REF() (&hboost_range_array)
#endif

#if defined(__GNUC__) && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)))
#  define HBOOST_RANGE_UNUSED __attribute__((unused))
#else
#  define HBOOST_RANGE_UNUSED
#endif



#endif

