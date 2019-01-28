//  (C) Copyright John Maddock 2001 - 2002. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

#if __IBMCPP__ <= 501
#  define HBOOST_NO_STD_ALLOCATOR
#endif

#define HBOOST_HAS_MACRO_USE_FACET
#define HBOOST_NO_STD_MESSAGES

// Apple doesn't seem to reliably defined a *unix* macro
#if !defined(CYGWIN) && (  defined(__unix__)  \
                        || defined(__unix)    \
                        || defined(unix)      \
                        || defined(__APPLE__) \
                        || defined(__APPLE)   \
                        || defined(APPLE))
#  include <unistd.h>
#endif

//  C++0x headers not yet implemented
//
#  define HBOOST_NO_CXX11_HDR_ARRAY
#  define HBOOST_NO_CXX11_HDR_CHRONO
#  define HBOOST_NO_CXX11_HDR_CODECVT
#  define HBOOST_NO_CXX11_HDR_CONDITION_VARIABLE
#  define HBOOST_NO_CXX11_HDR_FORWARD_LIST
#  define HBOOST_NO_CXX11_HDR_FUTURE
#  define HBOOST_NO_CXX11_HDR_INITIALIZER_LIST
#  define HBOOST_NO_CXX11_HDR_MUTEX
#  define HBOOST_NO_CXX11_HDR_RANDOM
#  define HBOOST_NO_CXX11_HDR_RATIO
#  define HBOOST_NO_CXX11_HDR_REGEX
#  define HBOOST_NO_CXX11_HDR_SYSTEM_ERROR
#  define HBOOST_NO_CXX11_HDR_THREAD
#  define HBOOST_NO_CXX11_HDR_TUPLE
#  define HBOOST_NO_CXX11_HDR_TYPE_TRAITS
#  define HBOOST_NO_CXX11_HDR_TYPEINDEX
#  define HBOOST_NO_CXX11_HDR_UNORDERED_MAP
#  define HBOOST_NO_CXX11_HDR_UNORDERED_SET
#  define HBOOST_NO_CXX11_NUMERIC_LIMITS
#  define HBOOST_NO_CXX11_ALLOCATOR
#  define HBOOST_NO_CXX11_ATOMIC_SMART_PTR
#  define HBOOST_NO_CXX11_SMART_PTR
#  define HBOOST_NO_CXX11_HDR_FUNCTIONAL
#  define HBOOST_NO_CXX11_HDR_ATOMIC
#  define HBOOST_NO_CXX11_STD_ALIGN
#  define HBOOST_NO_CXX11_ADDRESSOF

#if defined(__has_include)
#if !__has_include(<shared_mutex>)
#  define HBOOST_NO_CXX14_HDR_SHARED_MUTEX
#elif __cplusplus < 201402
#  define HBOOST_NO_CXX14_HDR_SHARED_MUTEX
#endif
#else
#  define HBOOST_NO_CXX14_HDR_SHARED_MUTEX
#endif

#define HBOOST_STDLIB "Visual Age default standard library"
