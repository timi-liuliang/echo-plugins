//  (C) Copyright John Maddock 2001 - 2003. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  cygwin specific config options:

#define HBOOST_PLATFORM "Cygwin"
#define HBOOST_HAS_DIRENT_H
#define HBOOST_HAS_LOG1P
#define HBOOST_HAS_EXPM1

//
// Threading API:
// See if we have POSIX threads, if we do use them, otherwise
// revert to native Win threads.
#define HBOOST_HAS_UNISTD_H
#include <unistd.h>
#if defined(_POSIX_THREADS) && (_POSIX_THREADS+0 >= 0) && !defined(HBOOST_HAS_WINTHREADS)
#  define HBOOST_HAS_PTHREADS
#  define HBOOST_HAS_SCHED_YIELD
#  define HBOOST_HAS_GETTIMEOFDAY
#  define HBOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE
#  define HBOOST_HAS_SIGACTION
#else
#  if !defined(HBOOST_HAS_WINTHREADS)
#     define HBOOST_HAS_WINTHREADS
#  endif
#  define HBOOST_HAS_FTIME
#endif

//
// find out if we have a stdint.h, there should be a better way to do this:
//
#include <sys/types.h>
#ifdef _STDINT_H
#define HBOOST_HAS_STDINT_H
#endif

/// Cygwin has no fenv.h
#define HBOOST_NO_FENV_H

// boilerplate code:
#include <hboost/config/posix_features.hpp>

//
// Cygwin lies about XSI conformance, there is no nl_types.h:
//
#ifdef HBOOST_HAS_NL_TYPES_H
#  undef HBOOST_HAS_NL_TYPES_H
#endif
 




