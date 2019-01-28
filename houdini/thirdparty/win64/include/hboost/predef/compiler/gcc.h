/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_GCC_H
#define HBOOST_PREDEF_COMPILER_GCC_H

/* Other compilers that emulate this one need to be detected first. */

#include <hboost/predef/compiler/clang.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_GNUC`]

[@http://en.wikipedia.org/wiki/GNU_Compiler_Collection Gnu GCC C/C++] compiler.
Version number available as major, minor, and patch (if available).

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__GNUC__`] [__predef_detection__]]

    [[`__GNUC__`, `__GNUC_MINOR__`, `__GNUC_PATCHLEVEL__`] [V.R.P]]
    [[`__GNUC__`, `__GNUC_MINOR__`] [V.R.0]]
    ]
 */

#define HBOOST_COMP_GNUC HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__GNUC__)
#   if !defined(HBOOST_COMP_GNUC_DETECTION) && defined(__GNUC_PATCHLEVEL__)
#       define HBOOST_COMP_GNUC_DETECTION \
            HBOOST_VERSION_NUMBER(__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__)
#   endif
#   if !defined(HBOOST_COMP_GNUC_DETECTION)
#       define HBOOST_COMP_GNUC_DETECTION \
            HBOOST_VERSION_NUMBER(__GNUC__,__GNUC_MINOR__,0)
#   endif
#endif

#ifdef HBOOST_COMP_GNUC_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_GNUC_EMULATED HBOOST_COMP_GNUC_DETECTION
#   else
#       undef HBOOST_COMP_GNUC
#       define HBOOST_COMP_GNUC HBOOST_COMP_GNUC_DETECTION
#   endif
#   define HBOOST_COMP_GNUC_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_GNUC_NAME "Gnu GCC C/C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_GNUC,HBOOST_COMP_GNUC_NAME)

#ifdef HBOOST_COMP_GNUC_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_GNUC_EMULATED,HBOOST_COMP_GNUC_NAME)
#endif
