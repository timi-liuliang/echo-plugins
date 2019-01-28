/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_INTEL_H
#define HBOOST_PREDEF_COMPILER_INTEL_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_INTEL`]

[@http://en.wikipedia.org/wiki/Intel_C%2B%2B Intel C/C++] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__INTEL_COMPILER`] [__predef_detection__]]
    [[`__ICL`] [__predef_detection__]]
    [[`__ICC`] [__predef_detection__]]
    [[`__ECC`] [__predef_detection__]]

    [[`__INTEL_COMPILER`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_INTEL HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || \
    defined(__ECC)
#   if !defined(HBOOST_COMP_INTEL_DETECTION) && defined(__INTEL_COMPILER)
#       define HBOOST_COMP_INTEL_DETECTION HBOOST_PREDEF_MAKE_10_VRP(__INTEL_COMPILER)
#   endif
#   if !defined(HBOOST_COMP_INTEL_DETECTION)
#       define HBOOST_COMP_INTEL_DETECTION HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#ifdef HBOOST_COMP_INTEL_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_INTEL_EMULATED HBOOST_COMP_INTEL_DETECTION
#   else
#       undef HBOOST_COMP_INTEL
#       define HBOOST_COMP_INTEL HBOOST_COMP_INTEL_DETECTION
#   endif
#   define HBOOST_COMP_INTEL_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_INTEL_NAME "Intel C/C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_INTEL,HBOOST_COMP_INTEL_NAME)

#ifdef HBOOST_COMP_INTEL_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_INTEL_EMULATED,HBOOST_COMP_INTEL_NAME)
#endif
