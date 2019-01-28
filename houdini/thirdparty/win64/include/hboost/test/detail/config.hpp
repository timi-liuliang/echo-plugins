//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//!@file
//!@brief a central place for global configuration switches
// ***************************************************************************

#ifndef HBOOST_TEST_CONFIG_HPP_071894GER
#define HBOOST_TEST_CONFIG_HPP_071894GER

// Boost
#include <hboost/config.hpp> // compilers workarounds
#include <hboost/detail/workaround.hpp>

#if defined(_WIN32) && !defined(HBOOST_DISABLE_WIN32) &&                  \
    (!defined(__COMO__) && !defined(__MWERKS__) && !defined(__GNUC__) || \
    HBOOST_WORKAROUND(__MWERKS__, >= 0x3000))
#  define HBOOST_SEH_BASED_SIGNAL_HANDLING
#endif

#if defined(__COMO__) && defined(_MSC_VER)
// eh.h uses type_info without declaring it.
class type_info;
#  define HBOOST_SEH_BASED_SIGNAL_HANDLING
#endif

//____________________________________________________________________________//

#if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x570)) || \
    HBOOST_WORKAROUND(__IBMCPP__, HBOOST_TESTED_AT(600))     || \
    (defined __sgi && HBOOST_WORKAROUND(_COMPILER_VERSION, HBOOST_TESTED_AT(730)))
#  define HBOOST_TEST_SHIFTED_LINE
#endif

//____________________________________________________________________________//

#if defined(HBOOST_MSVC) || (defined(__BORLANDC__) && !defined(HBOOST_DISABLE_WIN32))
#  define HBOOST_TEST_CALL_DECL __cdecl
#else
#  define HBOOST_TEST_CALL_DECL /**/
#endif

//____________________________________________________________________________//

#if !defined(HBOOST_NO_STD_LOCALE) && !defined(__MWERKS__)
#  define HBOOST_TEST_USE_STD_LOCALE 1
#endif

//____________________________________________________________________________//

#if HBOOST_WORKAROUND(__BORLANDC__, <= 0x570)            || \
    HBOOST_WORKAROUND( __COMO__, <= 0x433 )              || \
    HBOOST_WORKAROUND( __INTEL_COMPILER, <= 800 )        || \
    defined(__sgi) && _COMPILER_VERSION <= 730          || \
    HBOOST_WORKAROUND(__IBMCPP__, HBOOST_TESTED_AT(600))  || \
    defined(__DECCXX)                                   || \
    defined(__DMC__)
#  define HBOOST_TEST_NO_PROTECTED_USING
#endif

//____________________________________________________________________________//

#if defined(__GNUC__) || HBOOST_WORKAROUND(HBOOST_MSVC, == 1400)
#define HBOOST_TEST_PROTECTED_VIRTUAL virtual
#else
#define HBOOST_TEST_PROTECTED_VIRTUAL
#endif

//____________________________________________________________________________//

#if !defined(__BORLANDC__) && !HBOOST_WORKAROUND( __SUNPRO_CC, < 0x5100 )
#define HBOOST_TEST_SUPPORT_TOKEN_ITERATOR 1
#endif

//____________________________________________________________________________//

#if defined(HBOOST_ALL_DYN_LINK) && !defined(HBOOST_TEST_DYN_LINK)
#  define HBOOST_TEST_DYN_LINK
#endif

#if defined(HBOOST_TEST_INCLUDED)
#  undef HBOOST_TEST_DYN_LINK
#endif

#if defined(HBOOST_TEST_DYN_LINK)
#  define HBOOST_TEST_ALTERNATIVE_INIT_API

#  ifdef HBOOST_TEST_SOURCE
#    define HBOOST_TEST_DECL HBOOST_SYMBOL_EXPORT
#  else
#    define HBOOST_TEST_DECL HBOOST_SYMBOL_IMPORT
#  endif  // HBOOST_TEST_SOURCE
#else
#  define HBOOST_TEST_DECL
#endif

#if !defined(HBOOST_TEST_MAIN) && defined(HBOOST_AUTO_TEST_MAIN)
#define HBOOST_TEST_MAIN HBOOST_AUTO_TEST_MAIN
#endif

#if !defined(HBOOST_TEST_MAIN) && defined(HBOOST_TEST_MODULE)
#define HBOOST_TEST_MAIN HBOOST_TEST_MODULE
#endif



#ifndef HBOOST_PP_VARIADICS /* we can change this only if not already defined) */

#ifdef __PGI
#define HBOOST_PP_VARIADICS 1
#endif

#if HBOOST_CLANG
#define HBOOST_PP_VARIADICS 1
#endif

#if defined(HBOOST_GCC) && (HBOOST_GCC >= 4 * 10000 + 8 * 100)
#define HBOOST_PP_VARIADICS 1
#endif

#endif /* ifndef HBOOST_PP_VARIADICS */

#endif // HBOOST_TEST_CONFIG_HPP_071894GER
