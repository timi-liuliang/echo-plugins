/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_DIAB_H
#define HBOOST_PREDEF_COMPILER_DIAB_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_DIAB`]

[@http://www.windriver.com/products/development_suite/wind_river_compiler/ Diab C/C++] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__DCC__`] [__predef_detection__]]

    [[`__VERSION_NUMBER__`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_DIAB HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__DCC__)
#   define HBOOST_COMP_DIAB_DETECTION HBOOST_PREDEF_MAKE_10_VRPP(__VERSION_NUMBER__)
#endif

#ifdef HBOOST_COMP_DIAB_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_DIAB_EMULATED HBOOST_COMP_DIAB_DETECTION
#   else
#       undef HBOOST_COMP_DIAB
#       define HBOOST_COMP_DIAB HBOOST_COMP_DIAB_DETECTION
#   endif
#   define HBOOST_COMP_DIAB_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_DIAB_NAME "Diab C/C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_DIAB,HBOOST_COMP_DIAB_NAME)

#ifdef HBOOST_COMP_DIAB_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_DIAB_EMULATED,HBOOST_COMP_DIAB_NAME)
#endif
