/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_PALM_H
#define HBOOST_PREDEF_COMPILER_PALM_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_PALM`]

Palm C/C++ compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`_PACC_VER`] [__predef_detection__]]

    [[`_PACC_VER`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_PALM HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(_PACC_VER)
#   define HBOOST_COMP_PALM_DETECTION HBOOST_PREDEF_MAKE_0X_VRRPP000(_PACC_VER)
#endif

#ifdef HBOOST_COMP_PALM_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_PALM_EMULATED HBOOST_COMP_PALM_DETECTION
#   else
#       undef HBOOST_COMP_PALM
#       define HBOOST_COMP_PALM HBOOST_COMP_PALM_DETECTION
#   endif
#   define HBOOST_COMP_PALM_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_PALM_NAME "Palm C/C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_PALM,HBOOST_COMP_PALM_NAME)

#ifdef HBOOST_COMP_PALM_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_PALM_EMULATED,HBOOST_COMP_PALM_NAME)
#endif
