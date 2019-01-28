// Copyright (C) 2001-2003
// William E. Kempf
// Copyright (C) 2011-2013 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_THREAD_CONFIG_WEK01032003_HPP
#define HBOOST_THREAD_CONFIG_WEK01032003_HPP

#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>
#include <hboost/thread/detail/platform.hpp>

//#define HBOOST_THREAD_DONT_PROVIDE_INTERRUPTIONS
// ATTRIBUTE_MAY_ALIAS

#if defined(__GNUC__) && !defined(__INTEL_COMPILER)

  // GCC since 3.3 has may_alias attribute that helps to alleviate optimizer issues with
  // regard to violation of the strict aliasing rules.

  #define HBOOST_THREAD_DETAIL_USE_ATTRIBUTE_MAY_ALIAS
  #define HBOOST_THREAD_ATTRIBUTE_MAY_ALIAS __attribute__((__may_alias__))
#else
  #define HBOOST_THREAD_ATTRIBUTE_MAY_ALIAS
#endif


#if defined HBOOST_THREAD_THROW_IF_PRECONDITION_NOT_SATISFIED
#define HBOOST_THREAD_ASSERT_PRECONDITION(EXPR, EX) \
        if (EXPR) {} else hboost::throw_exception(EX)
#define HBOOST_THREAD_VERIFY_PRECONDITION(EXPR, EX) \
        if (EXPR) {} else hboost::throw_exception(EX)
#define HBOOST_THREAD_THROW_ELSE_RETURN(EX, RET) \
        hboost::throw_exception(EX)
#else
#define HBOOST_THREAD_ASSERT_PRECONDITION(EXPR, EX)
#define HBOOST_THREAD_VERIFY_PRECONDITION(EXPR, EX) \
        (void)(EXPR)
#define HBOOST_THREAD_THROW_ELSE_RETURN(EX, RET) \
        return (RET)
#endif

// This compiler doesn't support Boost.Chrono
#if defined __IBMCPP__ && (__IBMCPP__ < 1100) \
  && ! defined HBOOST_THREAD_DONT_USE_CHRONO
#define HBOOST_THREAD_DONT_USE_CHRONO
#if ! defined HBOOST_THREAD_USES_DATETIME
#define HBOOST_THREAD_USES_DATETIME
#endif
#endif

// This compiler doesn't support Boost.Move
#if HBOOST_WORKAROUND(__SUNPRO_CC, < 0x5100) \
  && ! defined HBOOST_THREAD_DONT_USE_MOVE
#define HBOOST_THREAD_DONT_USE_MOVE
#endif

// This compiler doesn't support Boost.Container Allocators files
#if defined __SUNPRO_CC \
  && ! defined HBOOST_THREAD_DONT_PROVIDE_FUTURE_CTOR_ALLOCATORS
#define HBOOST_THREAD_DONT_PROVIDE_FUTURE_CTOR_ALLOCATORS
#endif

#if defined _WIN32_WCE && _WIN32_WCE==0x501 \
  && ! defined HBOOST_THREAD_DONT_PROVIDE_FUTURE_CTOR_ALLOCATORS
#define HBOOST_THREAD_DONT_PROVIDE_FUTURE_CTOR_ALLOCATORS
#endif


#if defined HBOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX || defined HBOOST_NO_CXX11_HDR_INITIALIZER_LIST
#define HBOOST_THREAD_NO_CXX11_HDR_INITIALIZER_LIST
#define HBOOST_THREAD_NO_MAKE_LOCK_GUARD
#define HBOOST_THREAD_NO_MAKE_STRICT_LOCK
#define HBOOST_THREAD_NO_MAKE_NESTED_STRICT_LOCK
#endif

#if defined(HBOOST_NO_CXX11_HDR_TUPLE) || defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)
#define HBOOST_THREAD_NO_MAKE_UNIQUE_LOCKS
#define HBOOST_THREAD_NO_SYNCHRONIZE
#elif defined _MSC_VER && _MSC_VER <= 1600
// C++ features supported by VC++ 10 (aka 2010)
#define HBOOST_THREAD_NO_MAKE_UNIQUE_LOCKS
#define HBOOST_THREAD_NO_SYNCHRONIZE
#endif

/// BASIC_THREAD_ID
#if ! defined HBOOST_THREAD_DONT_PROVIDE_BASIC_THREAD_ID \
 && ! defined HBOOST_THREAD_PROVIDES_BASIC_THREAD_ID
#define HBOOST_THREAD_PROVIDES_BASIC_THREAD_ID
#endif

/// RVALUE_REFERENCES_DONT_MATCH_FUNTION_PTR
//#if defined HBOOST_NO_CXX11_RVALUE_REFERENCES || defined HBOOST_MSVC
#define HBOOST_THREAD_RVALUE_REFERENCES_DONT_MATCH_FUNTION_PTR
//#endif

// Default version
#if !defined HBOOST_THREAD_VERSION
#define HBOOST_THREAD_VERSION 2
#else
#if HBOOST_THREAD_VERSION!=2  && HBOOST_THREAD_VERSION!=3 && HBOOST_THREAD_VERSION!=4
#error "HBOOST_THREAD_VERSION must be 2, 3 or 4"
#endif
#endif

// CHRONO
// Uses Boost.Chrono by default if not stated the opposite defining HBOOST_THREAD_DONT_USE_CHRONO
#if ! defined HBOOST_THREAD_DONT_USE_CHRONO \
  && ! defined HBOOST_THREAD_USES_CHRONO
#define HBOOST_THREAD_USES_CHRONO
#endif

#if ! defined HBOOST_THREAD_DONT_USE_ATOMIC \
  && ! defined HBOOST_THREAD_USES_ATOMIC
#define HBOOST_THREAD_USES_ATOMIC
//#define HBOOST_THREAD_DONT_USE_ATOMIC
#endif

#if defined HBOOST_THREAD_USES_ATOMIC
// Andrey Semashev
#define HBOOST_THREAD_ONCE_ATOMIC
#else
//#elif ! defined HBOOST_NO_CXX11_THREAD_LOCAL && ! defined HBOOST_NO_THREAD_LOCAL && ! defined HBOOST_THREAD_NO_UINT32_PSEUDO_ATOMIC
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2444.html#Appendix
#define HBOOST_THREAD_ONCE_FAST_EPOCH
#endif
#if HBOOST_THREAD_VERSION==2

// PROVIDE_PROMISE_LAZY
#if ! defined HBOOST_THREAD_DONT_PROVIDE_PROMISE_LAZY \
  && ! defined HBOOST_THREAD_PROVIDES_PROMISE_LAZY
#define HBOOST_THREAD_PROVIDES_PROMISE_LAZY
#endif

// PROVIDE_THREAD_EQ
#if ! defined HBOOST_THREAD_DONT_PROVIDE_THREAD_EQ \
  && ! defined HBOOST_THREAD_PROVIDES_THREAD_EQ
#define HBOOST_THREAD_PROVIDES_THREAD_EQ
#endif

#endif

#if HBOOST_THREAD_VERSION>=3

// ONCE_CXX11
// fixme HBOOST_THREAD_PROVIDES_ONCE_CXX11 doesn't works when thread.cpp is compiled HBOOST_THREAD_VERSION 3
#if ! defined HBOOST_THREAD_DONT_PROVIDE_ONCE_CXX11 \
 && ! defined HBOOST_THREAD_PROVIDES_ONCE_CXX11
#define HBOOST_THREAD_DONT_PROVIDE_ONCE_CXX11
#endif

// THREAD_DESTRUCTOR_CALLS_TERMINATE_IF_JOINABLE
#if ! defined HBOOST_THREAD_DONT_PROVIDE_THREAD_DESTRUCTOR_CALLS_TERMINATE_IF_JOINABLE \
 && ! defined HBOOST_THREAD_PROVIDES_THREAD_DESTRUCTOR_CALLS_TERMINATE_IF_JOINABLE
#define HBOOST_THREAD_PROVIDES_THREAD_DESTRUCTOR_CALLS_TERMINATE_IF_JOINABLE
#endif

// THREAD_MOVE_ASSIGN_CALLS_TERMINATE_IF_JOINABLE
#if ! defined HBOOST_THREAD_DONT_PROVIDE_THREAD_MOVE_ASSIGN_CALLS_TERMINATE_IF_JOINABLE \
 && ! defined HBOOST_THREAD_PROVIDES_THREAD_MOVE_ASSIGN_CALLS_TERMINATE_IF_JOINABLE
#define HBOOST_THREAD_PROVIDES_THREAD_MOVE_ASSIGN_CALLS_TERMINATE_IF_JOINABLE
#endif

// PROVIDE_FUTURE
#if ! defined HBOOST_THREAD_DONT_PROVIDE_FUTURE \
 && ! defined HBOOST_THREAD_PROVIDES_FUTURE
#define HBOOST_THREAD_PROVIDES_FUTURE
#endif

// FUTURE_CTOR_ALLOCATORS
#if ! defined HBOOST_THREAD_DONT_PROVIDE_FUTURE_CTOR_ALLOCATORS \
 && ! defined HBOOST_THREAD_PROVIDES_FUTURE_CTOR_ALLOCATORS
#define HBOOST_THREAD_PROVIDES_FUTURE_CTOR_ALLOCATORS
#endif

// SHARED_MUTEX_UPWARDS_CONVERSIONS
#if ! defined HBOOST_THREAD_DONT_PROVIDE_SHARED_MUTEX_UPWARDS_CONVERSIONS \
 && ! defined HBOOST_THREAD_PROVIDES_SHARED_MUTEX_UPWARDS_CONVERSIONS
#define HBOOST_THREAD_PROVIDES_SHARED_MUTEX_UPWARDS_CONVERSIONS
#endif

// PROVIDE_EXPLICIT_LOCK_CONVERSION
#if ! defined HBOOST_THREAD_DONT_PROVIDE_EXPLICIT_LOCK_CONVERSION \
 && ! defined HBOOST_THREAD_PROVIDES_EXPLICIT_LOCK_CONVERSION
#define HBOOST_THREAD_PROVIDES_EXPLICIT_LOCK_CONVERSION
#endif

// GENERIC_SHARED_MUTEX_ON_WIN
#if ! defined HBOOST_THREAD_DONT_PROVIDE_GENERIC_SHARED_MUTEX_ON_WIN \
 && ! defined HBOOST_THREAD_PROVIDES_GENERIC_SHARED_MUTEX_ON_WIN
#define HBOOST_THREAD_PROVIDES_GENERIC_SHARED_MUTEX_ON_WIN
#endif

// USE_MOVE
#if ! defined HBOOST_THREAD_DONT_USE_MOVE \
 && ! defined HBOOST_THREAD_USES_MOVE
#define HBOOST_THREAD_USES_MOVE
#endif

#endif

// deprecated since version 4
#if HBOOST_THREAD_VERSION < 4

// NESTED_LOCKS
#if ! defined HBOOST_THREAD_PROVIDES_NESTED_LOCKS \
 && ! defined HBOOST_THREAD_DONT_PROVIDE_NESTED_LOCKS
#define HBOOST_THREAD_PROVIDES_NESTED_LOCKS
#endif

// CONDITION
#if ! defined HBOOST_THREAD_PROVIDES_CONDITION \
 && ! defined HBOOST_THREAD_DONT_PROVIDE_CONDITION
#define HBOOST_THREAD_PROVIDES_CONDITION
#endif

// USE_DATETIME
#if ! defined HBOOST_THREAD_DONT_USE_DATETIME \
 && ! defined HBOOST_THREAD_USES_DATETIME
#define HBOOST_THREAD_USES_DATETIME
#endif
#endif

#if HBOOST_THREAD_VERSION>=4

// SIGNATURE_PACKAGED_TASK
#if ! defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK \
 && ! defined HBOOST_THREAD_DONT_PROVIDE_SIGNATURE_PACKAGED_TASK
#define HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
#endif

// VARIADIC_THREAD
#if ! defined HBOOST_THREAD_PROVIDES_VARIADIC_THREAD \
 && ! defined HBOOST_THREAD_DONT_PROVIDE_VARIADIC_THREAD

#if ! defined(HBOOST_NO_SFINAE_EXPR) && \
    ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && \
    ! defined(HBOOST_NO_CXX11_DECLTYPE) && \
    ! defined(HBOOST_NO_CXX11_DECLTYPE_N3276) && \
    ! defined(HBOOST_NO_CXX11_TRAILING_RESULT_TYPES) && \
    ! defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) && \
    ! defined(HBOOST_NO_CXX11_HDR_TUPLE)

#define HBOOST_THREAD_PROVIDES_VARIADIC_THREAD
#endif
#endif

#if ! defined HBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY \
 && ! defined HBOOST_THREAD_DONT_PROVIDE_FUTURE_WHEN_ALL_WHEN_ANY

#if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && \
    ! defined(HBOOST_NO_CXX11_HDR_TUPLE)

#define HBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
#endif
#endif

//    ! defined(HBOOST_NO_SFINAE_EXPR) &&
//    ! defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) &&
//    ! defined(HBOOST_NO_CXX11_AUTO) &&
//    ! defined(HBOOST_NO_CXX11_DECLTYPE) &&
//    ! defined(HBOOST_NO_CXX11_DECLTYPE_N3276) &&


// MAKE_READY_AT_THREAD_EXIT
#if ! defined HBOOST_THREAD_PROVIDES_MAKE_READY_AT_THREAD_EXIT \
 && ! defined HBOOST_THREAD_DONT_PROVIDE_MAKE_READY_AT_THREAD_EXIT

//#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
#define HBOOST_THREAD_PROVIDES_MAKE_READY_AT_THREAD_EXIT
//#endif
#endif

// FUTURE_CONTINUATION
#if ! defined HBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION \
 && ! defined HBOOST_THREAD_DONT_PROVIDE_FUTURE_CONTINUATION
#define HBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#endif

#if ! defined HBOOST_THREAD_PROVIDES_FUTURE_UNWRAP \
 && ! defined HBOOST_THREAD_DONT_PROVIDE_FUTURE_UNWRAP
#define HBOOST_THREAD_PROVIDES_FUTURE_UNWRAP
#endif

// FUTURE_INVALID_AFTER_GET
#if ! defined HBOOST_THREAD_PROVIDES_FUTURE_INVALID_AFTER_GET \
 && ! defined HBOOST_THREAD_DONT_PROVIDE_FUTURE_INVALID_AFTER_GET
#define HBOOST_THREAD_PROVIDES_FUTURE_INVALID_AFTER_GET
#endif

// NESTED_LOCKS
#if ! defined HBOOST_THREAD_PROVIDES_NESTED_LOCKS \
 && ! defined HBOOST_THREAD_DONT_PROVIDE_NESTED_LOCKS
#define HBOOST_THREAD_DONT_PROVIDE_NESTED_LOCKS
#endif

// CONDITION
#if ! defined HBOOST_THREAD_PROVIDES_CONDITION \
 && ! defined HBOOST_THREAD_DONT_PROVIDE_CONDITION
#define HBOOST_THREAD_DONT_PROVIDE_CONDITION
#endif

#endif // HBOOST_THREAD_VERSION>=4

// INTERRUPTIONS
#if ! defined HBOOST_THREAD_PROVIDES_INTERRUPTIONS \
 && ! defined HBOOST_THREAD_DONT_PROVIDE_INTERRUPTIONS
#define HBOOST_THREAD_PROVIDES_INTERRUPTIONS
#endif

// CORRELATIONS

// EXPLICIT_LOCK_CONVERSION.
#if defined HBOOST_THREAD_PROVIDES_EXPLICIT_LOCK_CONVERSION
#define HBOOST_THREAD_EXPLICIT_LOCK_CONVERSION explicit
#else
#define HBOOST_THREAD_EXPLICIT_LOCK_CONVERSION
#endif

// HBOOST_THREAD_PROVIDES_GENERIC_SHARED_MUTEX_ON_WIN is defined if HBOOST_THREAD_PROVIDES_SHARED_MUTEX_UPWARDS_CONVERSIONS
#if defined HBOOST_THREAD_PROVIDES_SHARED_MUTEX_UPWARDS_CONVERSIONS \
&& ! defined HBOOST_THREAD_PROVIDES_GENERIC_SHARED_MUTEX_ON_WIN
#define HBOOST_THREAD_PROVIDES_GENERIC_SHARED_MUTEX_ON_WIN
#endif

// For C++11 call_once interface the compiler MUST support constexpr.
// Otherwise once_flag would be initialized during dynamic initialization stage, which is not thread-safe.
#if defined(HBOOST_THREAD_PROVIDES_ONCE_CXX11)
#if defined(HBOOST_NO_CXX11_CONSTEXPR)
#undef HBOOST_THREAD_PROVIDES_ONCE_CXX11
#endif
#endif

#if defined(HBOOST_THREAD_PLATFORM_WIN32) && defined HBOOST_THREAD_DONT_USE_DATETIME
#undef HBOOST_THREAD_DONT_USE_DATETIME
#define HBOOST_THREAD_USES_DATETIME
#endif

#if defined(HBOOST_THREAD_PLATFORM_WIN32) && defined HBOOST_THREAD_DONT_USE_CHRONO
#undef HBOOST_THREAD_DONT_USE_CHRONO
#define HBOOST_THREAD_USES_CHRONO
#endif

// HBOOST_THREAD_PROVIDES_DEPRECATED_FEATURES_SINCE_V3_0_0 defined by default up to Boost 1.55
// HBOOST_THREAD_DONT_PROVIDE_DEPRECATED_FEATURES_SINCE_V3_0_0 defined by default up to Boost 1.55
#if defined HBOOST_THREAD_PROVIDES_DEPRECATED_FEATURES_SINCE_V3_0_0

#if  ! defined HBOOST_THREAD_PROVIDES_THREAD_EQ
#define HBOOST_THREAD_PROVIDES_THREAD_EQ
#endif

#endif


//#if ! defined HBOOST_NO_CXX11_RVALUE_REFERENCES || defined HBOOST_THREAD_USES_MOVE
#if ! defined HBOOST_NO_CXX11_RVALUE_REFERENCES
#define HBOOST_THREAD_FUTURE_USES_OPTIONAL
#endif

#if HBOOST_WORKAROUND(__BORLANDC__, < 0x600)
#  pragma warn -8008 // Condition always true/false
#  pragma warn -8080 // Identifier declared but never used
#  pragma warn -8057 // Parameter never used
#  pragma warn -8066 // Unreachable code
#endif

#include <hboost/thread/detail/platform.hpp>

#if defined(HBOOST_THREAD_PLATFORM_WIN32)
#else
  #   if defined(HBOOST_HAS_PTHREAD_DELAY_NP) || defined(HBOOST_HAS_NANOSLEEP)
  #     define HBOOST_THREAD_SLEEP_FOR_IS_STEADY
  #   endif
#endif

// provided for backwards compatibility, since this
// macro was used for several releases by mistake.
#if defined(HBOOST_THREAD_DYN_DLL) && ! defined HBOOST_THREAD_DYN_LINK
# define HBOOST_THREAD_DYN_LINK
#endif

// compatibility with the rest of Boost's auto-linking code:
#if defined(HBOOST_THREAD_DYN_LINK) || defined(HBOOST_ALL_DYN_LINK)
# undef  HBOOST_THREAD_USE_LIB
# if !defined(HBOOST_THREAD_USE_DLL)
#  define HBOOST_THREAD_USE_DLL
# endif
#endif

#if defined(HBOOST_THREAD_BUILD_DLL)   //Build dll
#elif defined(HBOOST_THREAD_BUILD_LIB) //Build lib
#elif defined(HBOOST_THREAD_USE_DLL)   //Use dll
#elif defined(HBOOST_THREAD_USE_LIB)   //Use lib
#else //Use default
#   if defined(HBOOST_THREAD_PLATFORM_WIN32)
#       if defined(HBOOST_MSVC) || defined(HBOOST_INTEL_WIN) \
      || defined(__MINGW32__) || defined(MINGW32) || defined(HBOOST_MINGW32)
      //For compilers supporting auto-tss cleanup
            //with Boost.Threads lib, use Boost.Threads lib
#           define HBOOST_THREAD_USE_LIB
#       else
            //For compilers not yet supporting auto-tss cleanup
            //with Boost.Threads lib, use Boost.Threads dll
#           define HBOOST_THREAD_USE_DLL
#       endif
#   else
#       define HBOOST_THREAD_USE_LIB
#   endif
#endif

#if defined(HBOOST_HAS_DECLSPEC)
#   if defined(HBOOST_THREAD_BUILD_DLL) //Build dll
#       define HBOOST_THREAD_DECL HBOOST_SYMBOL_EXPORT
//#       define HBOOST_THREAD_DECL __declspec(dllexport)

#   elif defined(HBOOST_THREAD_USE_DLL) //Use dll
#       define HBOOST_THREAD_DECL HBOOST_SYMBOL_IMPORT
//#       define HBOOST_THREAD_DECL __declspec(dllimport)
#   else
#       define HBOOST_THREAD_DECL
#   endif
#elif (__GNUC__ == 4 && __GNUC_MINOR__ >= 1) || (__GNUC__ > 4)
#  define HBOOST_THREAD_DECL HBOOST_SYMBOL_VISIBLE

#else
#   define HBOOST_THREAD_DECL
#endif // HBOOST_HAS_DECLSPEC

//
// Automatically link to the correct build variant where possible.
//
#if !defined(HBOOST_ALL_NO_LIB) && !defined(HBOOST_THREAD_NO_LIB) && !defined(HBOOST_THREAD_BUILD_DLL) && !defined(HBOOST_THREAD_BUILD_LIB)
//
// Tell the autolink to link dynamically, this will get undef'ed by auto_link.hpp
// once it's done with it:
//
#if defined(HBOOST_THREAD_USE_DLL)
#   define HBOOST_DYN_LINK
#endif
//
// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it:
//
#if defined(HBOOST_THREAD_LIB_NAME)
#    define HBOOST_LIB_NAME HBOOST_THREAD_LIB_NAME
#else
#    define HBOOST_LIB_NAME hboost_thread
#endif
//
// If we're importing code from a dll, then tell auto_link.hpp about it:
//
// And include the header that does the work:
//
#include <hboost/config/auto_link.hpp>
#endif  // auto-linking disabled

#endif // HBOOST_THREAD_CONFIG_WEK1032003_HPP

// Change Log:
//   22 Jan 05 Roland Schwarz (speedsnail)
//      Usage of HBOOST_HAS_DECLSPEC macro.
//      Default again is static lib usage.
//      HBOOST_DYN_LINK only defined when autolink included.
