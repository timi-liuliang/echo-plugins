/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_PGI_H
#define HBOOST_PREDEF_COMPILER_PGI_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_PGI`]

[@http://en.wikipedia.org/wiki/The_Portland_Group Portland Group C/C++] compiler.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__PGI`] [__predef_detection__]]

    [[`__PGIC__`, `__PGIC_MINOR__`, `__PGIC_PATCHLEVEL__`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_PGI HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__PGI)
#   if !defined(HBOOST_COMP_PGI_DETECTION) && (defined(__PGIC__) && defined(__PGIC_MINOR__) && defined(__PGIC_PATCHLEVEL__))
#       define HBOOST_COMP_PGI_DETECTION HBOOST_VERSION_NUMBER(__PGIC__,__PGIC_MINOR__,__PGIC_PATCHLEVEL__)
#   endif
#   if !defined(HBOOST_COMP_PGI_DETECTION)
#       define HBOOST_COMP_PGI_DETECTION HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#ifdef HBOOST_COMP_PGI_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_PGI_EMULATED HBOOST_COMP_PGI_DETECTION
#   else
#       undef HBOOST_COMP_PGI
#       define HBOOST_COMP_PGI HBOOST_COMP_PGI_DETECTION
#   endif
#   define HBOOST_COMP_PGI_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_PGI_NAME "Portland Group C/C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_PGI,HBOOST_COMP_PGI_NAME)

#ifdef HBOOST_COMP_PGI_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_PGI_EMULATED,HBOOST_COMP_PGI_NAME)
#endif
