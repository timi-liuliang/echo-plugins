/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_VISUALC_H
#define HBOOST_PREDEF_COMPILER_VISUALC_H

/* Other compilers that emulate this one need to be detected first. */

#include <hboost/predef/compiler/clang.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_MSVC`]

[@http://en.wikipedia.org/wiki/Visual_studio Microsoft Visual C/C++] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`_MSC_VER`] [__predef_detection__]]

    [[`_MSC_FULL_VER`] [V.R.P]]
    [[`_MSC_VER`] [V.R.0]]
    ]
 */

#define HBOOST_COMP_MSVC HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(_MSC_VER)
#   if !defined (_MSC_FULL_VER)
#       define HBOOST_COMP_MSVC_BUILD 0
#   else
        /* how many digits does the build number have? */
#       if _MSC_FULL_VER / 10000 == _MSC_VER
            /* four digits */
#           define HBOOST_COMP_MSVC_BUILD (_MSC_FULL_VER % 10000)
#       elif _MSC_FULL_VER / 100000 == _MSC_VER
            /* five digits */
#           define HBOOST_COMP_MSVC_BUILD (_MSC_FULL_VER % 100000)
#       else
#           error "Cannot determine build number from _MSC_FULL_VER"
#       endif
#   endif
    /*
    VS2014 was skipped in the release sequence for MS. Which
    means that the compiler and VS product versions are no longer
    in sync. Hence we need to use different formulas for
    mapping from MSC version to VS product version.
    */
#   if (_MSC_VER >= 1900)
#       define HBOOST_COMP_MSVC_DETECTION HBOOST_VERSION_NUMBER(\
            _MSC_VER/100-5,\
            _MSC_VER%100,\
            HBOOST_COMP_MSVC_BUILD)
#   else
#       define HBOOST_COMP_MSVC_DETECTION HBOOST_VERSION_NUMBER(\
            _MSC_VER/100-6,\
            _MSC_VER%100,\
            HBOOST_COMP_MSVC_BUILD)
#   endif
#endif

#ifdef HBOOST_COMP_MSVC_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_MSVC_EMULATED HBOOST_COMP_MSVC_DETECTION
#   else
#       undef HBOOST_COMP_MSVC
#       define HBOOST_COMP_MSVC HBOOST_COMP_MSVC_DETECTION
#   endif
#   define HBOOST_COMP_MSVC_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_MSVC_NAME "Microsoft Visual C/C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_MSVC,HBOOST_COMP_MSVC_NAME)

#ifdef HBOOST_COMP_MSVC_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_MSVC_EMULATED,HBOOST_COMP_MSVC_NAME)
#endif
