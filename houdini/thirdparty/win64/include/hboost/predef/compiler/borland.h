/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_BORLAND_H
#define HBOOST_PREDEF_COMPILER_BORLAND_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_BORLAND`]

[@http://en.wikipedia.org/wiki/C_plus_plus_builder Borland C++] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__BORLANDC__`] [__predef_detection__]]
    [[`__CODEGEARC__`] [__predef_detection__]]

    [[`__BORLANDC__`] [V.R.P]]
    [[`__CODEGEARC__`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_BORLAND HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__BORLANDC__) || defined(__CODEGEARC__)
#   if !defined(HBOOST_COMP_BORLAND_DETECTION) && (defined(__CODEGEARC__))
#       define HBOOST_COMP_BORLAND_DETECTION HBOOST_PREDEF_MAKE_0X_VVRP(__CODEGEARC__)
#   endif
#   if !defined(HBOOST_COMP_BORLAND_DETECTION)
#       define HBOOST_COMP_BORLAND_DETECTION HBOOST_PREDEF_MAKE_0X_VVRP(__BORLANDC__)
#   endif
#endif

#ifdef HBOOST_COMP_BORLAND_DETECTION
#   define HBOOST_COMP_BORLAND_AVAILABLE
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_BORLAND_EMULATED HBOOST_COMP_BORLAND_DETECTION
#   else
#       undef HBOOST_COMP_BORLAND
#       define HBOOST_COMP_BORLAND HBOOST_COMP_BORLAND_DETECTION
#   endif
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_BORLAND_NAME "Borland C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_BORLAND,HBOOST_COMP_BORLAND_NAME)

#ifdef HBOOST_COMP_BORLAND_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_BORLAND_EMULATED,HBOOST_COMP_BORLAND_NAME)
#endif
