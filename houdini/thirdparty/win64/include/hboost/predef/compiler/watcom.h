/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_WATCOM_H
#define HBOOST_PREDEF_COMPILER_WATCOM_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_WATCOM`]

[@http://en.wikipedia.org/wiki/Watcom Watcom C++] compiler.
Version number available as major, and minor.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__WATCOMC__`] [__predef_detection__]]

    [[`__WATCOMC__`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_WATCOM HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__WATCOMC__)
#   define HBOOST_COMP_WATCOM_DETECTION HBOOST_PREDEF_MAKE_10_VVRR(__WATCOMC__)
#endif

#ifdef HBOOST_COMP_WATCOM_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_WATCOM_EMULATED HBOOST_COMP_WATCOM_DETECTION
#   else
#       undef HBOOST_COMP_WATCOM
#       define HBOOST_COMP_WATCOM HBOOST_COMP_WATCOM_DETECTION
#   endif
#   define HBOOST_COMP_WATCOM_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_WATCOM_NAME "Watcom C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_WATCOM,HBOOST_COMP_WATCOM_NAME)

#ifdef HBOOST_COMP_WATCOM_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_WATCOM_EMULATED,HBOOST_COMP_WATCOM_NAME)
#endif
