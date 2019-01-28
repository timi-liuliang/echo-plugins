//  (C) Copyright Jessica Hamilton 2014.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Haiku specific config options:

#define HBOOST_PLATFORM "Haiku"

#define HBOOST_HAS_UNISTD_H
#define HBOOST_HAS_STDINT_H

#ifndef HBOOST_DISABLE_THREADS
#  define HBOOST_HAS_THREADS
#endif

#define HBOOST_NO_CXX11_HDR_TYPE_TRAITS
#define HBOOST_NO_CXX11_ATOMIC_SMART_PTR
#define HBOOST_NO_CXX11_STATIC_ASSERT
#define HBOOST_NO_CXX11_VARIADIC_MACROS

//
// thread API's not auto detected:
//
#define HBOOST_HAS_SCHED_YIELD
#define HBOOST_HAS_GETTIMEOFDAY

// boilerplate code:
#include <hboost/config/posix_features.hpp>
