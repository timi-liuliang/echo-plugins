/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_CLANG_H
#define HBOOST_PREDEF_COMPILER_CLANG_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_CLANG`]

[@http://en.wikipedia.org/wiki/Clang Clang] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__clang__`] [__predef_detection__]]

    [[`__clang_major__`, `__clang_minor__`, `__clang_patchlevel__`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_CLANG HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__clang__)
#   define HBOOST_COMP_CLANG_DETECTION HBOOST_VERSION_NUMBER(__clang_major__,__clang_minor__,__clang_patchlevel__)
#endif

#ifdef HBOOST_COMP_CLANG_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_CLANG_EMULATED HBOOST_COMP_CLANG_DETECTION
#   else
#       undef HBOOST_COMP_CLANG
#       define HBOOST_COMP_CLANG HBOOST_COMP_CLANG_DETECTION
#   endif
#   define HBOOST_COMP_CLANG_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_CLANG_NAME "Clang"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_CLANG,HBOOST_COMP_CLANG_NAME)

#ifdef HBOOST_COMP_CLANG_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_CLANG_EMULATED,HBOOST_COMP_CLANG_NAME)
#endif
