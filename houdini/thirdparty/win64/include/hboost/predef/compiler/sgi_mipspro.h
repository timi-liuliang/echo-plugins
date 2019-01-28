/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_SGI_MIPSPRO_H
#define HBOOST_PREDEF_COMPILER_SGI_MIPSPRO_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_SGI`]

[@http://en.wikipedia.org/wiki/MIPSpro SGI MIPSpro] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__sgi`] [__predef_detection__]]
    [[`sgi`] [__predef_detection__]]

    [[`_SGI_COMPILER_VERSION`] [V.R.P]]
    [[`_COMPILER_VERSION`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_SGI HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__sgi) || defined(sgi)
#   if !defined(HBOOST_COMP_SGI_DETECTION) && defined(_SGI_COMPILER_VERSION)
#       define HBOOST_COMP_SGI_DETECTION HBOOST_PREDEF_MAKE_10_VRP(_SGI_COMPILER_VERSION)
#   endif
#   if !defined(HBOOST_COMP_SGI_DETECTION) && defined(_COMPILER_VERSION)
#       define HBOOST_COMP_SGI_DETECTION HBOOST_PREDEF_MAKE_10_VRP(_COMPILER_VERSION)
#   endif
#   if !defined(HBOOST_COMP_SGI_DETECTION)
#       define HBOOST_COMP_SGI_DETECTION HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#ifdef HBOOST_COMP_SGI_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_SGI_EMULATED HBOOST_COMP_SGI_DETECTION
#   else
#       undef HBOOST_COMP_SGI
#       define HBOOST_COMP_SGI HBOOST_COMP_SGI_DETECTION
#   endif
#   define HBOOST_COMP_SGI_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_SGI_NAME "SGI MIPSpro"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_SGI,HBOOST_COMP_SGI_NAME)

#ifdef HBOOST_COMP_SGI_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_SGI_EMULATED,HBOOST_COMP_SGI_NAME)
#endif
