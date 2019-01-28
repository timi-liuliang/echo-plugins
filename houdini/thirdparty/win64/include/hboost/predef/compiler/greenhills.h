/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_GREENHILLS_H
#define HBOOST_PREDEF_COMPILER_GREENHILLS_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_GHS`]

[@http://en.wikipedia.org/wiki/Green_Hills_Software Green Hills C/C++] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__ghs`] [__predef_detection__]]
    [[`__ghs__`] [__predef_detection__]]

    [[`__GHS_VERSION_NUMBER__`] [V.R.P]]
    [[`__ghs`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_GHS HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__ghs) || defined(__ghs__)
#   if !defined(HBOOST_COMP_GHS_DETECTION) && defined(__GHS_VERSION_NUMBER__)
#       define HBOOST_COMP_GHS_DETECTION HBOOST_PREDEF_MAKE_10_VRP(__GHS_VERSION_NUMBER__)
#   endif
#   if !defined(HBOOST_COMP_GHS_DETECTION) && defined(__ghs)
#       define HBOOST_COMP_GHS_DETECTION HBOOST_PREDEF_MAKE_10_VRP(__ghs)
#   endif
#   if !defined(HBOOST_COMP_GHS_DETECTION)
#       define HBOOST_COMP_GHS_DETECTION HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#ifdef HBOOST_COMP_GHS_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_GHS_EMULATED HBOOST_COMP_GHS_DETECTION
#   else
#       undef HBOOST_COMP_GHS
#       define HBOOST_COMP_GHS HBOOST_COMP_GHS_DETECTION
#   endif
#   define HBOOST_COMP_GHS_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_GHS_NAME "Green Hills C/C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_GHS,HBOOST_COMP_GHS_NAME)

#ifdef HBOOST_COMP_GHS_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_GHS_EMULATED,HBOOST_COMP_GHS_NAME)
#endif
