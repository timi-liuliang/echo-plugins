//  (C) Copyright Jim Douglas 2005. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  QNX specific config options:

#define HBOOST_PLATFORM "QNX"

#define HBOOST_HAS_UNISTD_H
#include <hboost/config/posix_features.hpp>

// QNX claims XOpen version 5 compatibility, but doesn't have an nl_types.h
// or log1p and expm1:
#undef  HBOOST_HAS_NL_TYPES_H
#undef  HBOOST_HAS_LOG1P
#undef  HBOOST_HAS_EXPM1

#define HBOOST_HAS_PTHREADS
#define HBOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE

#define HBOOST_HAS_GETTIMEOFDAY
#define HBOOST_HAS_CLOCK_GETTIME
#define HBOOST_HAS_NANOSLEEP





