/*
 *
 * Copyright (c) 1998-2002
 * John Maddock
 *
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         config.hpp
  *   VERSION      see <hboost/version.hpp>
  *   DESCRIPTION: regex extended config setup.
  */

#ifndef HBOOST_REGEX_CONFIG_HPP
#define HBOOST_REGEX_CONFIG_HPP
/*
 * Borland C++ Fix/error check
 * this has to go *before* we include any std lib headers:
 */
#if defined(__BORLANDC__)
#  include <hboost/regex/config/borland.hpp>
#endif
#include <hboost/version.hpp>

/*****************************************************************************
 *
 *  Include all the headers we need here:
 *
 ****************************************************************************/

#ifdef __cplusplus

#  ifndef HBOOST_REGEX_USER_CONFIG
#     define HBOOST_REGEX_USER_CONFIG <hboost/regex/user.hpp>
#  endif

#  include HBOOST_REGEX_USER_CONFIG

#  include <hboost/config.hpp>
#  include <hboost/predef.h>

#else
   /*
    * C build,
    * don't include <hboost/config.hpp> because that may
    * do C++ specific things in future...
    */
#  include <stdlib.h>
#  include <stddef.h>
#  ifdef _MSC_VER
#     define HBOOST_MSVC _MSC_VER
#  endif
#endif

/*****************************************************************************
 *
 *  Boilerplate regex config options:
 *
 ****************************************************************************/

/* Obsolete macro, use HBOOST_VERSION instead: */
#define HBOOST_RE_VERSION 320

/* fix: */
#if defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif


/*
* Define a macro for the namespace that details are placed in, this includes the Boost
* version number to avoid mismatched header and library versions:
*/
#define HBOOST_REGEX_DETAIL_NS HBOOST_JOIN(re_detail_, HBOOST_VERSION)

/*
 * Fix for gcc prior to 3.4: std::ctype<wchar_t> doesn't allow
 * masks to be combined, for example:
 * std::use_facet<std::ctype<wchar_t> >.is(std::ctype_base::lower|std::ctype_base::upper, L'a');
 * returns *false*.
 */
#ifdef __GLIBCPP__
#  define HBOOST_REGEX_BUGGY_CTYPE_FACET
#endif

/*
 * Intel C++ before 8.0 ends up with unresolved externals unless we turn off
 * extern template support:
 */
#if defined(HBOOST_INTEL) && defined(__cplusplus) && (HBOOST_INTEL <= 800)
#  define HBOOST_REGEX_NO_EXTERNAL_TEMPLATES
#endif
/*
 * Visual C++ doesn't support external templates with C++ extensions turned off:
 */
#if defined(_MSC_VER) && !defined(_MSC_EXTENSIONS)
#  define HBOOST_REGEX_NO_EXTERNAL_TEMPLATES
#endif
 /*
 * Oracle compiler in C++11 mode doesn't like external templates for some reason:
 */
#ifdef __SUNPRO_CC
#  define HBOOST_REGEX_NO_EXTERNAL_TEMPLATES
#endif
 /*
 * Shared regex lib will crash without this, frankly it looks a lot like a gcc bug:
 */
#if defined(__MINGW32__)
#  define HBOOST_REGEX_NO_EXTERNAL_TEMPLATES
#endif

/*
 * If there isn't good enough wide character support then there will
 * be no wide character regular expressions:
 */
#if (defined(HBOOST_NO_CWCHAR) || defined(HBOOST_NO_CWCTYPE) || defined(HBOOST_NO_STD_WSTRING))
#  if !defined(HBOOST_NO_WREGEX)
#     define HBOOST_NO_WREGEX
#  endif
#else
#  if defined(__sgi) && (defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION))
      /* STLPort on IRIX is misconfigured: <cwctype> does not compile
       * as a temporary fix include <wctype.h> instead and prevent inclusion
       * of STLPort version of <cwctype> */
#     include <wctype.h>
#     define __STLPORT_CWCTYPE
#     define _STLP_CWCTYPE
#  endif

#ifdef __cplusplus
#  include <hboost/regex/config/cwchar.hpp>
#endif

#endif

/*
 * If Win32 support has been disabled for boost in general, then
 * it is for regex in particular:
 */
#if defined(HBOOST_DISABLE_WIN32) && !defined(HBOOST_REGEX_NO_W32)
#  define HBOOST_REGEX_NO_W32
#endif

/* disable our own file-iterators and mapfiles if we can't
 * support them: */
#if defined(_WIN32)
#  if defined(HBOOST_REGEX_NO_W32) || HBOOST_PLAT_WINDOWS_RUNTIME
#    define HBOOST_REGEX_NO_FILEITER
#  endif
#else // defined(_WIN32)
#  if !defined(HBOOST_HAS_DIRENT_H)
#    define HBOOST_REGEX_NO_FILEITER
#  endif
#endif

/* backwards compatibitity: */
#if defined(HBOOST_RE_NO_LIB)
#  define HBOOST_REGEX_NO_LIB
#endif

#if defined(__GNUC__) && (defined(_WIN32) || defined(__CYGWIN__))
/* gcc on win32 has problems if you include <windows.h>
   (sporadically generates bad code). */
#  define HBOOST_REGEX_NO_W32
#endif
#if defined(__COMO__) && !defined(HBOOST_REGEX_NO_W32) && !defined(_MSC_EXTENSIONS)
#  define HBOOST_REGEX_NO_W32
#endif

/*****************************************************************************
 *
 *  Wide character workarounds:
 *
 ****************************************************************************/

/*
 * define HBOOST_REGEX_HAS_OTHER_WCHAR_T when wchar_t is a native type, but the users
 * code may be built with wchar_t as unsigned short: basically when we're building
 * with MSVC and the /Zc:wchar_t option we place some extra unsigned short versions
 * of the non-inline functions in the library, so that users can still link to the lib,
 * irrespective of whether their own code is built with /Zc:wchar_t.
 * Note that this does NOT WORK with VC10 and VC14 when the C++ locale is in effect as
 * the locale's <unsigned short> facets simply do not compile in that case.
 * As we default to the C++ locale when compiling for the windows runtime we
 * skip in this case aswell.
 */
#if defined(__cplusplus) && \
      (defined(HBOOST_MSVC) || defined(__ICL)) && \
      !defined(HBOOST_NO_INTRINSIC_WCHAR_T) && \
      defined(HBOOST_WINDOWS) && \
      !defined(__SGI_STL_PORT) && \
      !defined(_STLPORT_VERSION) && \
      !defined(HBOOST_RWSTD_VER) && \
      ((_MSC_VER < 1600) || !defined(HBOOST_REGEX_USE_CPP_LOCALE)) && \
      !HBOOST_PLAT_WINDOWS_RUNTIME
#  define HBOOST_REGEX_HAS_OTHER_WCHAR_T
#  ifdef HBOOST_MSVC
#     pragma warning(push)
#     pragma warning(disable : 4251 4231)
#     if HBOOST_MSVC < 1600
#        pragma warning(disable : 4660)
#     endif
#  endif
#  if defined(_DLL) && defined(HBOOST_MSVC) && (HBOOST_MSVC < 1600)
#     include <string>
      extern template class __declspec(dllimport) std::basic_string<unsigned short>;
#  endif
#  ifdef HBOOST_MSVC
#     pragma warning(pop)
#  endif
#endif


/*****************************************************************************
 *
 *  Set up dll import/export options:
 *
 ****************************************************************************/

#ifndef HBOOST_SYMBOL_EXPORT
#  define HBOOST_SYMBOL_EXPORT
#  define HBOOST_SYMBOL_IMPORT
#endif

#if (defined(HBOOST_REGEX_DYN_LINK) || defined(HBOOST_ALL_DYN_LINK)) && !defined(HBOOST_REGEX_STATIC_LINK)
#  if defined(HBOOST_REGEX_SOURCE)
#     define HBOOST_REGEX_DECL HBOOST_SYMBOL_EXPORT
#     define HBOOST_REGEX_BUILD_DLL
#  else
#     define HBOOST_REGEX_DECL HBOOST_SYMBOL_IMPORT
#  endif
#else
#  define HBOOST_REGEX_DECL
#endif

#if !defined(HBOOST_REGEX_NO_LIB) && !defined(HBOOST_REGEX_SOURCE) && !defined(HBOOST_ALL_NO_LIB) && defined(__cplusplus)
#  define HBOOST_LIB_NAME hboost_regex
#  if defined(HBOOST_REGEX_DYN_LINK) || defined(HBOOST_ALL_DYN_LINK)
#     define HBOOST_DYN_LINK
#  endif
#  ifdef HBOOST_REGEX_DIAG
#     define HBOOST_LIB_DIAGNOSTIC
#  endif
#  include <hboost/config/auto_link.hpp>
#endif

/*****************************************************************************
 *
 *  Set up function call type:
 *
 ****************************************************************************/

#if defined(HBOOST_MSVC) && defined(_MSC_EXTENSIONS)
#if defined(_DEBUG) || defined(__MSVC_RUNTIME_CHECKS) || defined(_MANAGED) || defined(HBOOST_REGEX_NO_FASTCALL)
#  define HBOOST_REGEX_CALL __cdecl
#else
#  define HBOOST_REGEX_CALL __fastcall
#endif
#  define HBOOST_REGEX_CCALL __cdecl
#endif

#if defined(__BORLANDC__) && !defined(HBOOST_DISABLE_WIN32)
#  define HBOOST_REGEX_CALL __fastcall
#  define HBOOST_REGEX_CCALL __stdcall
#endif

#ifndef HBOOST_REGEX_CALL
#  define HBOOST_REGEX_CALL
#endif
#ifndef HBOOST_REGEX_CCALL
#define HBOOST_REGEX_CCALL
#endif

/*****************************************************************************
 *
 *  Set up localisation model:
 *
 ****************************************************************************/

/* backwards compatibility: */
#ifdef HBOOST_RE_LOCALE_C
#  define HBOOST_REGEX_USE_C_LOCALE
#endif

#ifdef HBOOST_RE_LOCALE_CPP
#  define HBOOST_REGEX_USE_CPP_LOCALE
#endif

#if defined(__CYGWIN__)
#  define HBOOST_REGEX_USE_C_LOCALE
#endif

/* use C++ locale when targeting windows store */
#if HBOOST_PLAT_WINDOWS_RUNTIME
#  define HBOOST_REGEX_USE_CPP_LOCALE
#  define HBOOST_REGEX_NO_WIN32_LOCALE
#endif

/* Win32 defaults to native Win32 locale: */
#if defined(_WIN32) && \
    !defined(HBOOST_REGEX_USE_WIN32_LOCALE) && \
    !defined(HBOOST_REGEX_USE_C_LOCALE) && \
    !defined(HBOOST_REGEX_USE_CPP_LOCALE) && \
    !defined(HBOOST_REGEX_NO_W32) && \
    !defined(HBOOST_REGEX_NO_WIN32_LOCALE)
#  define HBOOST_REGEX_USE_WIN32_LOCALE
#endif
/* otherwise use C++ locale if supported: */
#if !defined(HBOOST_REGEX_USE_WIN32_LOCALE) && !defined(HBOOST_REGEX_USE_C_LOCALE) && !defined(HBOOST_REGEX_USE_CPP_LOCALE) && !defined(HBOOST_NO_STD_LOCALE)
#  define HBOOST_REGEX_USE_CPP_LOCALE
#endif
/* otherwise use C+ locale: */
#if !defined(HBOOST_REGEX_USE_WIN32_LOCALE) && !defined(HBOOST_REGEX_USE_C_LOCALE) && !defined(HBOOST_REGEX_USE_CPP_LOCALE)
#  define HBOOST_REGEX_USE_C_LOCALE
#endif

#ifndef HBOOST_REGEX_MAX_STATE_COUNT
#  define HBOOST_REGEX_MAX_STATE_COUNT 100000000
#endif


/*****************************************************************************
 *
 *  Error Handling for exception free compilers:
 *
 ****************************************************************************/

#ifdef HBOOST_NO_EXCEPTIONS
/*
 * If there are no exceptions then we must report critical-errors
 * the only way we know how; by terminating.
 */
#include <stdexcept>
#include <string>
#include <hboost/throw_exception.hpp>

#  define HBOOST_REGEX_NOEH_ASSERT(x)\
if(0 == (x))\
{\
   std::string s("Error: critical regex++ failure in: ");\
   s.append(#x);\
   std::runtime_error e(s);\
   hboost::throw_exception(e);\
}
#else
/*
 * With exceptions then error handling is taken care of and
 * there is no need for these checks:
 */
#  define HBOOST_REGEX_NOEH_ASSERT(x)
#endif


/*****************************************************************************
 *
 *  Stack protection under MS Windows:
 *
 ****************************************************************************/

#if !defined(HBOOST_REGEX_NO_W32) && !defined(HBOOST_REGEX_V3)
#  if(defined(_WIN32) || defined(_WIN64) || defined(_WINCE)) \
        && !defined(__GNUC__) \
        && !(defined(__BORLANDC__) && (__BORLANDC__ >= 0x600)) \
        && !(defined(__MWERKS__) && (__MWERKS__ <= 0x3003))
#     define HBOOST_REGEX_HAS_MS_STACK_GUARD
#  endif
#elif defined(HBOOST_REGEX_HAS_MS_STACK_GUARD)
#  undef HBOOST_REGEX_HAS_MS_STACK_GUARD
#endif

#if defined(__cplusplus) && defined(HBOOST_REGEX_HAS_MS_STACK_GUARD)

namespace hboost{
namespace HBOOST_REGEX_DETAIL_NS{

HBOOST_REGEX_DECL void HBOOST_REGEX_CALL reset_stack_guard_page();

}
}

#endif


/*****************************************************************************
 *
 *  Algorithm selection and configuration:
 *
 ****************************************************************************/

#if !defined(HBOOST_REGEX_RECURSIVE) && !defined(HBOOST_REGEX_NON_RECURSIVE)
#  if defined(HBOOST_REGEX_HAS_MS_STACK_GUARD) && !defined(_STLP_DEBUG) && !defined(__STL_DEBUG) && !(defined(HBOOST_MSVC) && (HBOOST_MSVC >= 1400))
#     define HBOOST_REGEX_RECURSIVE
#  else
#     define HBOOST_REGEX_NON_RECURSIVE
#  endif
#endif

#ifdef HBOOST_REGEX_NON_RECURSIVE
#  ifdef HBOOST_REGEX_RECURSIVE
#     error "Can't set both HBOOST_REGEX_RECURSIVE and HBOOST_REGEX_NON_RECURSIVE"
#  endif
#  ifndef HBOOST_REGEX_BLOCKSIZE
#     define HBOOST_REGEX_BLOCKSIZE 4096
#  endif
#  if HBOOST_REGEX_BLOCKSIZE < 512
#     error "HBOOST_REGEX_BLOCKSIZE must be at least 512"
#  endif
#  ifndef HBOOST_REGEX_MAX_BLOCKS
#     define HBOOST_REGEX_MAX_BLOCKS 1024
#  endif
#  ifdef HBOOST_REGEX_HAS_MS_STACK_GUARD
#     undef HBOOST_REGEX_HAS_MS_STACK_GUARD
#  endif
#  ifndef HBOOST_REGEX_MAX_CACHE_BLOCKS
#     define HBOOST_REGEX_MAX_CACHE_BLOCKS 16
#  endif
#endif


/*****************************************************************************
 *
 *  helper memory allocation functions:
 *
 ****************************************************************************/

#if defined(__cplusplus) && defined(HBOOST_REGEX_NON_RECURSIVE)
namespace hboost{ namespace HBOOST_REGEX_DETAIL_NS{

HBOOST_REGEX_DECL void* HBOOST_REGEX_CALL get_mem_block();
HBOOST_REGEX_DECL void HBOOST_REGEX_CALL put_mem_block(void*);

}} /* namespaces */
#endif

/*****************************************************************************
 *
 *  Diagnostics:
 *
 ****************************************************************************/

#ifdef HBOOST_REGEX_CONFIG_INFO
HBOOST_REGEX_DECL void HBOOST_REGEX_CALL print_regex_library_info();
#endif

#if defined(HBOOST_REGEX_DIAG)
#  pragma message ("HBOOST_REGEX_DECL" HBOOST_STRINGIZE(=HBOOST_REGEX_DECL))
#  pragma message ("HBOOST_REGEX_CALL" HBOOST_STRINGIZE(=HBOOST_REGEX_CALL))
#  pragma message ("HBOOST_REGEX_CCALL" HBOOST_STRINGIZE(=HBOOST_REGEX_CCALL))
#ifdef HBOOST_REGEX_USE_C_LOCALE
#  pragma message ("Using C locale in regex traits class")
#elif HBOOST_REGEX_USE_CPP_LOCALE
#  pragma message ("Using C++ locale in regex traits class")
#else
#  pragma message ("Using Win32 locale in regex traits class")
#endif
#if defined(HBOOST_REGEX_DYN_LINK) || defined(HBOOST_ALL_DYN_LINK)
#  pragma message ("Dynamic linking enabled")
#endif
#if defined(HBOOST_REGEX_NO_LIB) || defined(HBOOST_ALL_NO_LIB)
#  pragma message ("Auto-linking disabled")
#endif
#ifdef HBOOST_REGEX_NO_EXTERNAL_TEMPLATES
#  pragma message ("Extern templates disabled")
#endif

#endif

#endif




