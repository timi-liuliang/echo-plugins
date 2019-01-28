//  (C) Copyright Christopher Jefferson 2011.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  config for libc++
//  Might need more in here later.

#if !defined(_LIBCPP_VERSION)
#  include <ciso646>
#  if !defined(_LIBCPP_VERSION)
#      error "This is not libc++!"
#  endif
#endif

#define HBOOST_STDLIB "libc++ version " HBOOST_STRINGIZE(_LIBCPP_VERSION)

#define HBOOST_HAS_THREADS

#ifdef _LIBCPP_HAS_NO_VARIADICS
#    define HBOOST_NO_CXX11_HDR_TUPLE
#endif

// HBOOST_NO_CXX11_ALLOCATOR should imply no support for the C++11
// allocator model. The C++11 allocator model requires a conforming
// std::allocator_traits which is only possible with C++11 template
// aliases since members rebind_alloc and rebind_traits require it.
#if defined(_LIBCPP_HAS_NO_TEMPLATE_ALIASES)
#    define HBOOST_NO_CXX11_ALLOCATOR
#endif

#if __cplusplus < 201103
#  define HBOOST_NO_CXX11_HDR_ARRAY
#  define HBOOST_NO_CXX11_HDR_CODECVT
#  define HBOOST_NO_CXX11_HDR_CONDITION_VARIABLE
#  define HBOOST_NO_CXX11_HDR_FORWARD_LIST
#  define HBOOST_NO_CXX11_HDR_INITIALIZER_LIST
#  define HBOOST_NO_CXX11_HDR_MUTEX
#  define HBOOST_NO_CXX11_HDR_RANDOM
#  define HBOOST_NO_CXX11_HDR_RATIO
#  define HBOOST_NO_CXX11_HDR_REGEX
#  define HBOOST_NO_CXX11_HDR_SYSTEM_ERROR
#  define HBOOST_NO_CXX11_HDR_THREAD
#  define HBOOST_NO_CXX11_HDR_TUPLE
#  define HBOOST_NO_CXX11_HDR_TYPEINDEX
#  define HBOOST_NO_CXX11_HDR_UNORDERED_MAP
#  define HBOOST_NO_CXX11_HDR_UNORDERED_SET
#  define HBOOST_NO_CXX11_NUMERIC_LIMITS
#  define HBOOST_NO_CXX11_ALLOCATOR
#  define HBOOST_NO_CXX11_SMART_PTR
#  define HBOOST_NO_CXX11_HDR_FUNCTIONAL
#  define HBOOST_NO_CXX11_STD_ALIGN
#  define HBOOST_NO_CXX11_ADDRESSOF
#  define HBOOST_NO_CXX11_HDR_ATOMIC
#  define HBOOST_NO_CXX11_ATOMIC_SMART_PTR
#  define HBOOST_NO_CXX11_HDR_CHRONO
#  define HBOOST_NO_CXX11_HDR_TYPE_TRAITS
#  define HBOOST_NO_CXX11_HDR_FUTURE
#elif _LIBCPP_VERSION < 3700
//
// These appear to be unusable/incomplete so far:
//
#  define HBOOST_NO_CXX11_HDR_ATOMIC
#  define HBOOST_NO_CXX11_ATOMIC_SMART_PTR
#  define HBOOST_NO_CXX11_HDR_CHRONO
#  define HBOOST_NO_CXX11_HDR_TYPE_TRAITS
#  define HBOOST_NO_CXX11_HDR_FUTURE
#endif


#if _LIBCPP_VERSION < 3700
// libc++ uses a non-standard messages_base
#define HBOOST_NO_STD_MESSAGES
#endif

#if defined(__has_include)
#if !__has_include(<shared_mutex>)
#  define HBOOST_NO_CXX14_HDR_SHARED_MUTEX
#elif __cplusplus <= 201103
#  define HBOOST_NO_CXX14_HDR_SHARED_MUTEX
#endif
#elif __cplusplus < 201402
#  define HBOOST_NO_CXX14_HDR_SHARED_MUTEX
#endif

//  --- end ---
