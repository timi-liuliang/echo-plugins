//  (C) Copyright Artyom Beilis 2010.  
//  Use, modification and distribution are subject to the  
//  Boost Software License, Version 1.0. (See accompanying file  
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) 

#ifndef HBOOST_CONFIG_PLATFORM_VMS_HPP 
#define HBOOST_CONFIG_PLATFORM_VMS_HPP 

#define HBOOST_PLATFORM "OpenVMS" 

#undef  HBOOST_HAS_STDINT_H 
#define HBOOST_HAS_UNISTD_H 
#define HBOOST_HAS_NL_TYPES_H 
#define HBOOST_HAS_GETTIMEOFDAY 
#define HBOOST_HAS_DIRENT_H 
#define HBOOST_HAS_PTHREADS 
#define HBOOST_HAS_NANOSLEEP 
#define HBOOST_HAS_CLOCK_GETTIME 
#define HBOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE 
#define HBOOST_HAS_LOG1P 
#define HBOOST_HAS_EXPM1 
#define HBOOST_HAS_THREADS 
#undef  HBOOST_HAS_SCHED_YIELD 

#endif 
