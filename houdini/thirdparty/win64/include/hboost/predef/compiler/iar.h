/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_IAR_H
#define HBOOST_PREDEF_COMPILER_IAR_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_IAR`]

IAR C/C++ compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__IAR_SYSTEMS_ICC__`] [__predef_detection__]]

    [[`__VER__`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_IAR HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__IAR_SYSTEMS_ICC__)
#   define HBOOST_COMP_IAR_DETECTION HBOOST_PREDEF_MAKE_10_VVRR(__VER__)
#endif

#ifdef HBOOST_COMP_IAR_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_IAR_EMULATED HBOOST_COMP_IAR_DETECTION
#   else
#       undef HBOOST_COMP_IAR
#       define HBOOST_COMP_IAR HBOOST_COMP_IAR_DETECTION
#   endif
#   define HBOOST_COMP_IAR_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_IAR_NAME "IAR C/C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_IAR,HBOOST_COMP_IAR_NAME)

#ifdef HBOOST_COMP_IAR_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_IAR_EMULATED,HBOOST_COMP_IAR_NAME)
#endif
