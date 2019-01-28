/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_KAI_H
#define HBOOST_PREDEF_COMPILER_KAI_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_KCC`]

Kai C++ compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__KCC`] [__predef_detection__]]

    [[`__KCC_VERSION`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_KCC HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__KCC)
#   define HBOOST_COMP_KCC_DETECTION HBOOST_PREDEF_MAKE_0X_VRPP(__KCC_VERSION)
#endif

#ifdef HBOOST_COMP_KCC_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_KCC_EMULATED HBOOST_COMP_KCC_DETECTION
#   else
#       undef HBOOST_COMP_KCC
#       define HBOOST_COMP_KCC HBOOST_COMP_KCC_DETECTION
#   endif
#   define HBOOST_COMP_KCC_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_KCC_NAME "Kai C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_KCC,HBOOST_COMP_KCC_NAME)

#ifdef HBOOST_COMP_KCC_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_KCC_EMULATED,HBOOST_COMP_KCC_NAME)
#endif
