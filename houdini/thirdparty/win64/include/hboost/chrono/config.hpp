//  hboost/chrono/config.hpp  -------------------------------------------------//

//  Copyright Beman Dawes 2003, 2006, 2008
//  Copyright 2009-2011 Vicente J. Botet Escriba
//  Copyright (c) Microsoft Corporation 2014

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/chrono for documentation.

#ifndef HBOOST_CHRONO_CONFIG_HPP
#define HBOOST_CHRONO_CONFIG_HPP

#include <hboost/config.hpp>
#include <hboost/predef.h>

#if !defined HBOOST_CHRONO_VERSION
#define HBOOST_CHRONO_VERSION 1
#else
#if HBOOST_CHRONO_VERSION!=1  && HBOOST_CHRONO_VERSION!=2
#error "HBOOST_CHRONO_VERSION must be 1 or 2"
#endif
#endif

#if defined(HBOOST_CHRONO_SOURCE) && !defined(HBOOST_USE_WINDOWS_H)
#define HBOOST_USE_WINDOWS_H
#endif

#if ! defined HBOOST_CHRONO_PROVIDES_DATE_IO_FOR_SYSTEM_CLOCK_TIME_POINT \
    && ! defined HBOOST_CHRONO_DONT_PROVIDE_DATE_IO_FOR_SYSTEM_CLOCK_TIME_POINT

# define HBOOST_CHRONO_PROVIDES_DATE_IO_FOR_SYSTEM_CLOCK_TIME_POINT

#endif

//  HBOOST_CHRONO_POSIX_API, HBOOST_CHRONO_MAC_API, or HBOOST_CHRONO_WINDOWS_API
//  can be defined by the user to specify which API should be used

#if defined(HBOOST_CHRONO_WINDOWS_API)
# warning Boost.Chrono will use the Windows API
#elif defined(HBOOST_CHRONO_MAC_API)
# warning Boost.Chrono will use the Mac API
#elif defined(HBOOST_CHRONO_POSIX_API)
# warning Boost.Chrono will use the POSIX API
#endif

# if defined( HBOOST_CHRONO_WINDOWS_API ) && defined( HBOOST_CHRONO_POSIX_API )
#   error both HBOOST_CHRONO_WINDOWS_API and HBOOST_CHRONO_POSIX_API are defined
# elif defined( HBOOST_CHRONO_WINDOWS_API ) && defined( HBOOST_CHRONO_MAC_API )
#   error both HBOOST_CHRONO_WINDOWS_API and HBOOST_CHRONO_MAC_API are defined
# elif defined( HBOOST_CHRONO_MAC_API ) && defined( HBOOST_CHRONO_POSIX_API )
#   error both HBOOST_CHRONO_MAC_API and HBOOST_CHRONO_POSIX_API are defined
# elif !defined( HBOOST_CHRONO_WINDOWS_API ) && !defined( HBOOST_CHRONO_MAC_API ) && !defined( HBOOST_CHRONO_POSIX_API )
#   if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32))
#     define HBOOST_CHRONO_WINDOWS_API
#   elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#     define HBOOST_CHRONO_MAC_API
#   else
#     define HBOOST_CHRONO_POSIX_API
#   endif
# endif

# if defined( HBOOST_CHRONO_WINDOWS_API )
#   ifndef UNDER_CE
#     define HBOOST_CHRONO_HAS_PROCESS_CLOCKS
#   endif
#   define HBOOST_CHRONO_HAS_CLOCK_STEADY
#   if HBOOST_PLAT_WINDOWS_DESKTOP
#     define HBOOST_CHRONO_HAS_THREAD_CLOCK
#   endif
#   define HBOOST_CHRONO_THREAD_CLOCK_IS_STEADY true
# endif

# if defined( HBOOST_CHRONO_MAC_API )
#   define HBOOST_CHRONO_HAS_PROCESS_CLOCKS
#   define HBOOST_CHRONO_HAS_CLOCK_STEADY
#   define HBOOST_CHRONO_HAS_THREAD_CLOCK
#   define HBOOST_CHRONO_THREAD_CLOCK_IS_STEADY true
# endif

# if defined( HBOOST_CHRONO_POSIX_API )
#   define HBOOST_CHRONO_HAS_PROCESS_CLOCKS
#   include <time.h>  //to check for CLOCK_REALTIME and CLOCK_MONOTONIC and _POSIX_THREAD_CPUTIME
#   if defined(CLOCK_MONOTONIC)
#      define HBOOST_CHRONO_HAS_CLOCK_STEADY
#   endif
#   if defined(_POSIX_THREAD_CPUTIME) && !defined(HBOOST_DISABLE_THREADS)
#     define HBOOST_CHRONO_HAS_THREAD_CLOCK
#     define HBOOST_CHRONO_THREAD_CLOCK_IS_STEADY true
#   endif
#   if defined(CLOCK_THREAD_CPUTIME_ID) && !defined(HBOOST_DISABLE_THREADS)
#     define HBOOST_CHRONO_HAS_THREAD_CLOCK
#     define HBOOST_CHRONO_THREAD_CLOCK_IS_STEADY true
#   endif
#   if defined(sun) || defined(__sun)
#     undef HBOOST_CHRONO_HAS_THREAD_CLOCK
#     undef HBOOST_CHRONO_THREAD_CLOCK_IS_STEADY
#   endif
#   if (defined(__HP_aCC) || defined(__GNUC__)) && defined(__hpux)
#     undef HBOOST_CHRONO_HAS_THREAD_CLOCK
#     undef HBOOST_CHRONO_THREAD_CLOCK_IS_STEADY
#   endif
#   if defined(__VXWORKS__)
#     undef HBOOST_CHRONO_HAS_PROCESS_CLOCKS
#   endif
# endif

#if defined(HBOOST_CHRONO_THREAD_DISABLED) && defined(HBOOST_CHRONO_HAS_THREAD_CLOCK)
#undef HBOOST_CHRONO_HAS_THREAD_CLOCK
#undef HBOOST_CHRONO_THREAD_CLOCK_IS_STEADY
#endif

// unicode support  ------------------------------//

#if defined(HBOOST_NO_CXX11_UNICODE_LITERALS) || defined(HBOOST_NO_CXX11_CHAR16_T) || defined(HBOOST_NO_CXX11_CHAR32_T)
//~ #define HBOOST_CHRONO_HAS_UNICODE_SUPPORT
#else
#define HBOOST_CHRONO_HAS_UNICODE_SUPPORT 1
#endif

#ifndef HBOOST_CHRONO_LIB_CONSTEXPR
#if defined( HBOOST_NO_CXX11_NUMERIC_LIMITS )
#define HBOOST_CHRONO_LIB_CONSTEXPR
#elif defined(_LIBCPP_VERSION) &&  !defined(_LIBCPP_CONSTEXPR)
  #define HBOOST_CHRONO_LIB_CONSTEXPR
#else
  #define HBOOST_CHRONO_LIB_CONSTEXPR HBOOST_CONSTEXPR
#endif
#endif

#if defined( HBOOST_NO_CXX11_NUMERIC_LIMITS )
#  define HBOOST_CHRONO_LIB_NOEXCEPT_OR_THROW throw()
#else
#ifdef HBOOST_NO_CXX11_NOEXCEPT
#  define HBOOST_CHRONO_LIB_NOEXCEPT_OR_THROW throw()
#else
#  define HBOOST_CHRONO_LIB_NOEXCEPT_OR_THROW noexcept
#endif
#endif

#if defined HBOOST_CHRONO_PROVIDE_HYBRID_ERROR_HANDLING \
 && defined HBOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
#error "HBOOST_CHRONO_PROVIDE_HYBRID_ERROR_HANDLING && HBOOST_CHRONO_PROVIDE_HYBRID_ERROR_HANDLING defined"
#endif

#if defined HBOOST_CHRONO_PROVIDES_DEPRECATED_IO_SINCE_V2_0_0 \
 && defined HBOOST_CHRONO_DONT_PROVIDES_DEPRECATED_IO_SINCE_V2_0_0
#error "HBOOST_CHRONO_PROVIDES_DEPRECATED_IO_SINCE_V2_0_0 && HBOOST_CHRONO_DONT_PROVIDES_DEPRECATED_IO_SINCE_V2_0_0 defined"
#endif

#if ! defined HBOOST_CHRONO_PROVIDE_HYBRID_ERROR_HANDLING \
 && ! defined HBOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
#define HBOOST_CHRONO_PROVIDE_HYBRID_ERROR_HANDLING
#endif

#if (HBOOST_CHRONO_VERSION == 2)
#if ! defined HBOOST_CHRONO_PROVIDES_DEPRECATED_IO_SINCE_V2_0_0 \
 && ! defined HBOOST_CHRONO_DONT_PROVIDES_DEPRECATED_IO_SINCE_V2_0_0
#define HBOOST_CHRONO_DONT_PROVIDES_DEPRECATED_IO_SINCE_V2_0_0
#endif
#endif

#ifdef HBOOST_CHRONO_HEADER_ONLY
#define HBOOST_CHRONO_INLINE inline
#define HBOOST_CHRONO_STATIC inline
#define HBOOST_CHRONO_DECL

#else
#define HBOOST_CHRONO_INLINE
#define HBOOST_CHRONO_STATIC static

//  enable dynamic linking on Windows  ---------------------------------------//

// we need to import/export our code only if the user has specifically
// asked for it by defining either HBOOST_ALL_DYN_LINK if they want all boost
// libraries to be dynamically linked, or HBOOST_CHRONO_DYN_LINK
// if they want just this one to be dynamically liked:
#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_CHRONO_DYN_LINK)
// export if this is our own source, otherwise import:
#ifdef HBOOST_CHRONO_SOURCE
# define HBOOST_CHRONO_DECL HBOOST_SYMBOL_EXPORT
#else
# define HBOOST_CHRONO_DECL HBOOST_SYMBOL_IMPORT
#endif  // HBOOST_CHRONO_SOURCE
#endif  // DYN_LINK
//
// if HBOOST_CHRONO_DECL isn't defined yet define it now:
#ifndef HBOOST_CHRONO_DECL
#define HBOOST_CHRONO_DECL
#endif



//  enable automatic library variant selection  ------------------------------//

#if !defined(HBOOST_CHRONO_SOURCE) && !defined(HBOOST_ALL_NO_LIB) && !defined(HBOOST_CHRONO_NO_LIB)
//
// Set the name of our library; this will get undef'ed by auto_link.hpp
// once it's done with it:
//
#define HBOOST_LIB_NAME hboost_chrono
//
// If we're importing code from a dll, then tell auto_link.hpp about it:
//
#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_CHRONO_DYN_LINK)
#  define HBOOST_DYN_LINK
#endif
//
// And include the header that does the work:
//
#include <hboost/config/auto_link.hpp>
#endif  // auto-linking disabled
#endif // HBOOST_CHRONO_HEADER_ONLY
#endif // HBOOST_CHRONO_CONFIG_HPP

