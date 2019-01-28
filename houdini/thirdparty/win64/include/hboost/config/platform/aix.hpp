//  (C) Copyright John Maddock 2001 - 2002. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  IBM/Aix specific config options:

#define HBOOST_PLATFORM "IBM Aix"

#define HBOOST_HAS_UNISTD_H
#define HBOOST_HAS_NL_TYPES_H
#define HBOOST_HAS_NANOSLEEP
#define HBOOST_HAS_CLOCK_GETTIME

// This needs support in "hboost/cstdint.hpp" exactly like FreeBSD.
// This platform has header named <inttypes.h> which includes all
// the things needed.
#define HBOOST_HAS_STDINT_H

// Threading API's:
#define HBOOST_HAS_PTHREADS
#define HBOOST_HAS_PTHREAD_DELAY_NP
#define HBOOST_HAS_SCHED_YIELD
//#define HBOOST_HAS_PTHREAD_YIELD

// boilerplate code:
#include <hboost/config/posix_features.hpp>




