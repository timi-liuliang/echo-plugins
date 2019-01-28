
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_CONFIG_HPP_INCLUDED
#define HBOOST_TT_CONFIG_HPP_INCLUDED

#ifndef HBOOST_CONFIG_HPP
#include <hboost/config.hpp>
#endif
#include <hboost/version.hpp>
#include <hboost/detail/workaround.hpp>

//
// whenever we have a conversion function with ellipses
// it needs to be declared __cdecl to suppress compiler
// warnings from MS and Borland compilers (this *must*
// appear before we include is_same.hpp below):
#if defined(HBOOST_MSVC) || (defined(__BORLANDC__) && !defined(HBOOST_DISABLE_WIN32))
#   define HBOOST_TT_DECL __cdecl
#else
#   define HBOOST_TT_DECL /**/
#endif

# if (HBOOST_WORKAROUND(__MWERKS__, < 0x3000)                         \
    || HBOOST_WORKAROUND(__IBMCPP__, < 600 )                         \
    || HBOOST_WORKAROUND(__BORLANDC__, < 0x5A0)                      \
    || defined(__ghs)                                               \
    || HBOOST_WORKAROUND(__HP_aCC, < 60700)           \
    || HBOOST_WORKAROUND(MPW_CPLUS, HBOOST_TESTED_AT(0x890))          \
    || HBOOST_WORKAROUND(__SUNPRO_CC, HBOOST_TESTED_AT(0x580)))       \
    && defined(HBOOST_NO_IS_ABSTRACT)

#   define HBOOST_TT_NO_CONFORMING_IS_CLASS_IMPLEMENTATION 1

#endif

#ifndef HBOOST_TT_NO_CONFORMING_IS_CLASS_IMPLEMENTATION
# define HBOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION 1
#endif

//
// define HBOOST_TT_TEST_MS_FUNC_SIGS
// when we want to test __stdcall etc function types with is_function etc
// (Note, does not work with Borland, even though it does support __stdcall etc):
//
#if defined(_MSC_EXTENSIONS) && !defined(__BORLANDC__)
#  define HBOOST_TT_TEST_MS_FUNC_SIGS
#endif

//
// define HBOOST_TT_NO_CV_FUNC_TEST
// if tests for cv-qualified member functions don't 
// work in is_member_function_pointer
//
#if HBOOST_WORKAROUND(__MWERKS__, < 0x3000) || HBOOST_WORKAROUND(__IBMCPP__, <= 600)
#  define HBOOST_TT_NO_CV_FUNC_TEST
#endif

//
// Macros that have been deprecated, defined here for backwards compatibility:
//
#define HBOOST_BROKEN_COMPILER_TYPE_TRAITS_SPECIALIZATION(x)
#define HBOOST_TT_BROKEN_COMPILER_SPEC(x)

#endif // HBOOST_TT_CONFIG_HPP_INCLUDED


