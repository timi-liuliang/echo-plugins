/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_METAWARE_H
#define HBOOST_PREDEF_COMPILER_METAWARE_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_HIGHC`]

MetaWare High C/C++ compiler.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__HIGHC__`] [__predef_detection__]]
    ]
 */

#define HBOOST_COMP_HIGHC HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__HIGHC__)
#   define HBOOST_COMP_HIGHC_DETECTION HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#ifdef HBOOST_COMP_HIGHC_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_HIGHC_EMULATED HBOOST_COMP_HIGHC_DETECTION
#   else
#       undef HBOOST_COMP_HIGHC
#       define HBOOST_COMP_HIGHC HBOOST_COMP_HIGHC_DETECTION
#   endif
#   define HBOOST_COMP_HIGHC_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_HIGHC_NAME "MetaWare High C/C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_HIGHC,HBOOST_COMP_HIGHC_NAME)

#ifdef HBOOST_COMP_HIGHC_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_HIGHC_EMULATED,HBOOST_COMP_HIGHC_NAME)
#endif
