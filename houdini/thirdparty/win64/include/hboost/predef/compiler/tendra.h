/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_TENDRA_H
#define HBOOST_PREDEF_COMPILER_TENDRA_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_TENDRA`]

[@http://en.wikipedia.org/wiki/TenDRA_Compiler TenDRA C/C++] compiler.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__TenDRA__`] [__predef_detection__]]
    ]
 */

#define HBOOST_COMP_TENDRA HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__TenDRA__)
#   define HBOOST_COMP_TENDRA_DETECTION HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#ifdef HBOOST_COMP_TENDRA_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_TENDRA_EMULATED HBOOST_COMP_TENDRA_DETECTION
#   else
#       undef HBOOST_COMP_TENDRA
#       define HBOOST_COMP_TENDRA HBOOST_COMP_TENDRA_DETECTION
#   endif
#   define HBOOST_COMP_TENDRA_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_TENDRA_NAME "TenDRA C/C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_TENDRA,HBOOST_COMP_TENDRA_NAME)

#ifdef HBOOST_COMP_TENDRA_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_TENDRA_EMULATED,HBOOST_COMP_TENDRA_NAME)
#endif
