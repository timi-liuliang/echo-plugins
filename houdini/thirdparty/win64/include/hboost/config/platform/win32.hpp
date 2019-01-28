//  (C) Copyright John Maddock 2001 - 2003. 
//  (C) Copyright Bill Kempf 2001. 
//  (C) Copyright Aleksey Gurtovoy 2003. 
//  (C) Copyright Rene Rivera 2005.
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Win32 specific config options:

#define HBOOST_PLATFORM "Win32"

//  Get the information about the MinGW runtime, i.e. __MINGW32_*VERSION.
#if defined(__MINGW32__)
#  include <_mingw.h>
#endif

#if defined(__GNUC__) && !defined(HBOOST_NO_SWPRINTF)
#  define HBOOST_NO_SWPRINTF
#endif

//  Default defines for HBOOST_SYMBOL_EXPORT and HBOOST_SYMBOL_IMPORT
//  If a compiler doesn't support __declspec(dllexport)/__declspec(dllimport),
//  its hboost/config/compiler/ file must define HBOOST_SYMBOL_EXPORT and
//  HBOOST_SYMBOL_IMPORT
#ifndef HBOOST_SYMBOL_EXPORT
#  define HBOOST_HAS_DECLSPEC
#  define HBOOST_SYMBOL_EXPORT __declspec(dllexport)
#  define HBOOST_SYMBOL_IMPORT __declspec(dllimport)
#endif

#if defined(__MINGW32__) && ((__MINGW32_MAJOR_VERSION > 2) || ((__MINGW32_MAJOR_VERSION == 2) && (__MINGW32_MINOR_VERSION >= 0)))
#  define HBOOST_HAS_STDINT_H
#  ifndef __STDC_LIMIT_MACROS
#     define __STDC_LIMIT_MACROS
#  endif
#  define HBOOST_HAS_DIRENT_H
#  define HBOOST_HAS_UNISTD_H
#endif

#if defined(__MINGW32__) && (__GNUC__ >= 4)
// Mingw has these functions but there are persistent problems
// with calls to these crashing, so disable for now:
//#  define HBOOST_HAS_EXPM1
//#  define HBOOST_HAS_LOG1P
#  define HBOOST_HAS_GETTIMEOFDAY
#endif
//
// Win32 will normally be using native Win32 threads,
// but there is a pthread library avaliable as an option,
// we used to disable this when HBOOST_DISABLE_WIN32 was 
// defined but no longer - this should allow some
// files to be compiled in strict mode - while maintaining
// a consistent setting of HBOOST_HAS_THREADS across
// all translation units (needed for shared_ptr etc).
//

#ifndef HBOOST_HAS_PTHREADS
#  define HBOOST_HAS_WINTHREADS
#endif

//
// WinCE configuration:
//
#if defined(_WIN32_WCE) || defined(UNDER_CE)
#  define HBOOST_NO_ANSI_APIS
// Windows CE does not have a conforming signature for swprintf
#  define HBOOST_NO_SWPRINTF
#else
#  define HBOOST_HAS_GETSYSTEMTIMEASFILETIME
#  define HBOOST_HAS_THREADEX
#  define HBOOST_HAS_GETSYSTEMTIMEASFILETIME
#endif

//
// Windows Runtime
//
#if defined(WINAPI_FAMILY) && \
  (WINAPI_FAMILY == WINAPI_FAMILY_APP || WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
#  define HBOOST_NO_ANSI_APIS
#endif

#ifndef HBOOST_DISABLE_WIN32
// WEK: Added
#define HBOOST_HAS_FTIME
#define HBOOST_WINDOWS 1

#endif
