/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_DIGITALMARS_H
#define HBOOST_PREDEF_COMPILER_DIGITALMARS_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_DMC`]

[@http://en.wikipedia.org/wiki/Digital_Mars Digital Mars] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__DMC__`] [__predef_detection__]]

    [[`__DMC__`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_DMC HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__DMC__)
#   define HBOOST_COMP_DMC_DETECTION HBOOST_PREDEF_MAKE_0X_VRP(__DMC__)
#endif

#ifdef HBOOST_COMP_DMC_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_DMC_EMULATED HBOOST_COMP_DMC_DETECTION
#   else
#       undef HBOOST_COMP_DMC
#       define HBOOST_COMP_DMC HBOOST_COMP_DMC_DETECTION
#   endif
#   define HBOOST_COMP_DMC_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_DMC_NAME "Digital Mars"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_DMC,HBOOST_COMP_DMC_NAME)

#ifdef HBOOST_COMP_DMC_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_DMC_EMULATED,HBOOST_COMP_DMC_NAME)
#endif
