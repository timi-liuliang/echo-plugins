//  (C) Copyright John Maddock 2001. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  BeOS specific config options:

#define HBOOST_PLATFORM "BeOS"

#define HBOOST_NO_CWCHAR
#define HBOOST_NO_CWCTYPE
#define HBOOST_HAS_UNISTD_H

#define HBOOST_HAS_BETHREADS

#ifndef HBOOST_DISABLE_THREADS
#  define HBOOST_HAS_THREADS
#endif

// boilerplate code:
#include <hboost/config/posix_features.hpp>
 


